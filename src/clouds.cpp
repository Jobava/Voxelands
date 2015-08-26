/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* clouds.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2014 <lisa@ltmnet.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*
* License updated from GPLv2 or later to GPLv3 or later by Lisa Milne
* for Voxelands.
************************************************************************/

#include "clouds.h"
#include "noise.h"
#include "constants.h"
#include "debug.h"
#include "main.h" // For g_profiler and g_settings
#include "profiler.h"
#include "settings.h"

Clouds::Clouds(
		scene::ISceneNode* parent,
		scene::ISceneManager* mgr,
		s32 id,
		u32 seed
):
	scene::ISceneNode(parent, mgr, id),
	m_seed(seed),
	m_camera_pos(0,0),
	m_camera_offset(0,0,0),
	m_time(0)
{
	dstream<<__FUNCTION_NAME<<std::endl;

	m_material.setFlag(video::EMF_LIGHTING, false);
	m_material.setFlag(video::EMF_BACK_FACE_CULLING, false);
	m_material.setFlag(video::EMF_BILINEAR_FILTER, false);
	m_material.setFlag(video::EMF_FOG_ENABLE, true);
	m_material.setFlag(video::EMF_ANTI_ALIASING, true);

	m_cloud_y = BS*100;

	m_box = core::aabbox3d<f32>(-BS*1000000,m_cloud_y-BS,-BS*1000000, BS*1000000,m_cloud_y+BS,BS*1000000);

	m_face_count = 1;
	if (g_settings->getBool("enable_3d_clouds"))
		m_face_count = 6;
}

Clouds::~Clouds()
{
	dstream<<__FUNCTION_NAME<<std::endl;
}

void Clouds::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);

	ISceneNode::OnRegisterSceneNode();
}

#define MYROUND(x) (x > 0.0 ? (int)x : (int)x - 1)

void Clouds::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (SceneManager->getSceneNodeRenderPass() != scene::ESNRP_SOLID)
		return;

	ScopeProfiler sp(g_profiler, "Rendering of clouds, avg", SPT_AVG);

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->setMaterial(m_material);

	/*
		Clouds move from X+ towards X-
	*/

	const s16 cloud_radius_i = 12;
	const float cloud_size = BS*64;
	const v2f cloud_speed(0, -BS*2);
	const float cloud_full_radius = cloud_size * cloud_radius_i;

	// Position of cloud noise origin in world coordinates
	v2f world_cloud_origin_pos_f = m_time*cloud_speed;
	// Position of cloud noise origin from the camera
	v2f cloud_origin_from_camera_f = world_cloud_origin_pos_f - m_camera_pos;
	// The center point of drawing in the noise
	v2f center_of_drawing_in_noise_f = -cloud_origin_from_camera_f;
	// The integer center point of drawing in the noise
	v2s16 center_of_drawing_in_noise_i(
		MYROUND(center_of_drawing_in_noise_f.X / cloud_size),
		MYROUND(center_of_drawing_in_noise_f.Y / cloud_size)
	);
	// The world position of the integer center point of drawing in the noise
	v2f world_center_of_drawing_in_noise_f = v2f(
		center_of_drawing_in_noise_i.X * cloud_size,
		center_of_drawing_in_noise_i.Y * cloud_size
	) + world_cloud_origin_pos_f;

	video::SColorf c_top_f(m_color);
	video::SColorf c_side_1_f(m_color);
	video::SColorf c_side_2_f(m_color);
	video::SColorf c_bottom_f(m_color);
	c_side_1_f.r *= 0.95;
	c_side_1_f.g *= 0.95;
	c_side_1_f.b *= 0.95;
	c_side_2_f.r *= 0.90;
	c_side_2_f.g *= 0.90;
	c_side_2_f.b *= 0.90;
	c_bottom_f.r *= 0.80;
	c_bottom_f.g *= 0.80;
	c_bottom_f.b *= 0.80;
	c_top_f.a = 1.0;
	c_side_1_f.a = 1.0;
	c_side_2_f.a = 1.0;
	c_bottom_f.a = 1.0;
	video::SColor c_top = c_top_f.toSColor();
	video::SColor c_side_1 = c_side_1_f.toSColor();
	video::SColor c_side_2 = c_side_2_f.toSColor();
	video::SColor c_bottom = c_bottom_f.toSColor();

	video::SColor c[6] = {
		c_top,
		c_side_1,
		c_side_2,
		c_side_1,
		c_side_2,
		c_bottom
	};

	// Get fog parameters for setting them back later
	video::SColor fog_color(0,0,0,0);
	video::E_FOG_TYPE fog_type = video::EFT_FOG_LINEAR;
	f32 fog_start = 0;
	f32 fog_end = 0;
	f32 fog_density = 0;
	bool fog_pixelfog = false;
	bool fog_rangefog = false;

	driver->getFog(
		fog_color,
		fog_type,
		fog_start,
		fog_end,
		fog_density,
		fog_pixelfog,
		fog_rangefog
	);

	// Set our own fog
	driver->setFog(
		fog_color,
		fog_type,
		cloud_full_radius * 0.5,
		cloud_full_radius*1.2,
		fog_density,
		fog_pixelfog,
		fog_rangefog
	);

	for (s16 zi=-cloud_radius_i; zi<cloud_radius_i; zi++)
	for (s16 xi=-cloud_radius_i; xi<cloud_radius_i; xi++) {
		v2s16 p_in_noise_i(
			xi+center_of_drawing_in_noise_i.X,
			zi+center_of_drawing_in_noise_i.Y
		);

		v2f p0 = v2f(xi,zi)*cloud_size + world_center_of_drawing_in_noise_f;

		double noise = noise2d_perlin_abs(
			(float)p_in_noise_i.X*cloud_size/BS/200,
			(float)p_in_noise_i.Y*cloud_size/BS/200,
			m_seed,
			3,
			0.4
		);
		if (noise < 0.95)
			continue;

		video::S3DVertex v[4] = {
			video::S3DVertex(0,0,0, 0,0,0, c[0], 0, 1),
			video::S3DVertex(0,0,0, 0,0,0, c[0], 1, 1),
			video::S3DVertex(0,0,0, 0,0,0, c[0], 1, 0),
			video::S3DVertex(0,0,0, 0,0,0, c[0], 0, 0)
		};

		f32 rx = cloud_size;
		f32 ry = 8*BS;
		f32 rz = cloud_size;

		for (int i=0; i<m_face_count; i++) {
			switch (i) {
			case 0:	// top
				v[0].Pos.X=-rx; v[0].Pos.Y= ry; v[0].Pos.Z=-rz;
				v[1].Pos.X=-rx; v[1].Pos.Y= ry; v[1].Pos.Z= rz;
				v[2].Pos.X= rx; v[2].Pos.Y= ry; v[2].Pos.Z= rz;
				v[3].Pos.X= rx; v[3].Pos.Y= ry, v[3].Pos.Z=-rz;
				break;
			case 1: // back
				v[0].Pos.X=-rx; v[0].Pos.Y= ry; v[0].Pos.Z=-rz;
				v[1].Pos.X= rx; v[1].Pos.Y= ry; v[1].Pos.Z=-rz;
				v[2].Pos.X= rx; v[2].Pos.Y=-ry; v[2].Pos.Z=-rz;
				v[3].Pos.X=-rx; v[3].Pos.Y=-ry, v[3].Pos.Z=-rz;
				break;
			case 2: //right
				v[0].Pos.X= rx; v[0].Pos.Y= ry; v[0].Pos.Z=-rz;
				v[1].Pos.X= rx; v[1].Pos.Y= ry; v[1].Pos.Z= rz;
				v[2].Pos.X= rx; v[2].Pos.Y=-ry; v[2].Pos.Z= rz;
				v[3].Pos.X= rx; v[3].Pos.Y=-ry, v[3].Pos.Z=-rz;
				break;
			case 3: // front
				v[0].Pos.X= rx; v[0].Pos.Y= ry; v[0].Pos.Z= rz;
				v[1].Pos.X=-rx; v[1].Pos.Y= ry; v[1].Pos.Z= rz;
				v[2].Pos.X=-rx; v[2].Pos.Y=-ry; v[2].Pos.Z= rz;
				v[3].Pos.X= rx; v[3].Pos.Y=-ry, v[3].Pos.Z= rz;
				break;
			case 4: // left
				v[0].Pos.X=-rx; v[0].Pos.Y= ry; v[0].Pos.Z= rz;
				v[1].Pos.X=-rx; v[1].Pos.Y= ry; v[1].Pos.Z=-rz;
				v[2].Pos.X=-rx; v[2].Pos.Y=-ry; v[2].Pos.Z=-rz;
				v[3].Pos.X=-rx; v[3].Pos.Y=-ry, v[3].Pos.Z= rz;
				break;
			case 5: // bottom
				v[0].Pos.X= rx; v[0].Pos.Y=-ry; v[0].Pos.Z= rz;
				v[1].Pos.X=-rx; v[1].Pos.Y=-ry; v[1].Pos.Z= rz;
				v[2].Pos.X=-rx; v[2].Pos.Y=-ry; v[2].Pos.Z=-rz;
				v[3].Pos.X= rx; v[3].Pos.Y=-ry, v[3].Pos.Z=-rz;
				break;
			}

			v3f pos = v3f(p0.X,m_cloud_y,p0.Y);
			pos -= intToFloat(m_camera_offset, BS);

			for (u16 k=0; k<4; k++) {
				v[k].Pos += pos;
				v[k].Color= c[i];
			}
			u16 indices[] = {0,1,2,2,3,0};
			driver->drawVertexPrimitiveList(v, 4, indices, 2, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
		}

	}

	driver->setFog(
		fog_color,
		fog_type,
		fog_start,
		fog_end,
		fog_density,
		fog_pixelfog,
		fog_rangefog
	);
}

void Clouds::step(float dtime)
{
	m_time += dtime;
}

void Clouds::update(v2f camera_p, video::SColorf color)
{
	m_camera_pos = camera_p;
	m_color = color;
}

