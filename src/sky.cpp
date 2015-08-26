/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* sky.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2013-2015 <lisa@ltmnet.com>
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

#include "sky.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"
#include "S3DVertex.h"
#include "utility.h" // MYMIN
#include "path.h" // getTexturePath
#include "noise.h" // easeCurve
#include "main.h" // g_profiler
#include "profiler.h"

//! constructor
Sky::Sky(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id):
		scene::ISceneNode(parent, mgr, id),
		m_first_update(true),
		m_brightness(0.5),
		m_cloud_brightness(0.5),
		m_moon_phase(0.0),
		m_moon_phase_pending(0.0),
		m_bgcolor_bright_f(1,1,1,1),
		m_skycolor_bright_f(1,0,0,0),
		m_cloudcolor_bright_f(1,1,1,1)
{
	setAutomaticCulling(scene::EAC_OFF);
	Box.MaxEdge.set(0,0,0);
	Box.MinEdge.set(0,0,0);

	// create material

	video::SMaterial mat;
	mat.Lighting = false;
	mat.ZBuffer = video::ECFN_NEVER;
	mat.ZWriteEnable = false;
	mat.AntiAliasing=0;
	mat.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
	mat.TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
	mat.BackfaceCulling = false;

	m_materials[0] = mat;

	m_materials[1] = mat;
	m_materials[1].MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	m_materials[2] = mat;
	m_materials[2].setTexture(0, mgr->getVideoDriver()->getTexture(getTexturePath("sun.png").c_str()));
	m_materials[2].MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	m_materials[3] = mat;
	m_materials[3].setTexture(0, mgr->getVideoDriver()->getTexture(getTexturePath("moon.png").c_str()));
	m_materials[3].MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	for (u32 i=0; i<SKY_STAR_COUNT; i++) {
		m_stars[i] = v3f(
			myrand_range(-10000,10000),
			myrand_range(-10000,10000),
			myrand_range(-10000,10000)
		);
		m_stars[i].normalize();
	}
}

void Sky::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this, scene::ESNRP_SKY_BOX);

	scene::ISceneNode::OnRegisterSceneNode();
}

const core::aabbox3d<f32>& Sky::getBoundingBox() const
{
	return Box;
}

//! renders the node.
void Sky::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	ScopeProfiler sp(g_profiler, "Sky::render()", SPT_AVG);

	// draw perspective skybox
	core::matrix4 translate(AbsoluteTransformation);
	translate.setTranslation(camera->getAbsolutePosition());

	// Draw the sky box between the near and far clip plane
	const f32 viewDistance = (camera->getNearValue() + camera->getFarValue()) * 0.5f;
	core::matrix4 scale;
	scale.setScale(core::vector3df(viewDistance, viewDistance, viewDistance));

	driver->setTransform(video::ETS_WORLD, translate * scale);

	if (!m_sunlight_seen)
		return;

	float sunsize = 0.07;
	float moonsize = 0.04;

	float nightlength = 0.41;
	float wn = nightlength / 2;
	float wicked_time_of_day = 0;
	if (m_time_of_day > wn && m_time_of_day < 1.0 - wn) {
		wicked_time_of_day = (m_time_of_day - wn)/(1.0-wn*2)*0.5 + 0.25;
	}else if (m_time_of_day < 0.5) {
		wicked_time_of_day = m_time_of_day / wn * 0.25;
	}else{
		wicked_time_of_day = 1.0 - ((1.0-m_time_of_day) / wn * 0.25);
	}

	const f32 t = 1.0f;
	const f32 o = 0.0f;
	static const u16 indices[4] = {0,1,2,3};
	video::S3DVertex vertices[4];

	// Draw sun
	if (wicked_time_of_day > 0.15 && wicked_time_of_day < 0.85) {
		driver->setMaterial(m_materials[2]);
		float d = sunsize * 1.7;
		video::SColor c(255,255,255,255);
		vertices[0] = video::S3DVertex(-d,-d,-1, 0,0,1, c, t, t);
		vertices[1] = video::S3DVertex( d,-d,-1, 0,0,1, c, o, t);
		vertices[2] = video::S3DVertex( d, d,-1, 0,0,1, c, o, o);
		vertices[3] = video::S3DVertex(-d, d,-1, 0,0,1, c, t, o);
		for (u32 i=0; i<4; i++) {
			// Switch from -Z (south) to +X (east)
			vertices[i].Pos.rotateXZBy(90);
			vertices[i].Pos.rotateXYBy(wicked_time_of_day * 360 - 90);
		}
		driver->drawIndexedTriangleFan(&vertices[0], 4, indices, 2);
	}
	// Draw moon
	if (wicked_time_of_day < 0.3 || wicked_time_of_day > 0.7) {
		int mp = m_moon_phase*10;
		if (mp != 0) {
			driver->setMaterial(m_materials[3]);
			float d = moonsize * 1.9;
			video::SColor c(255,255,255,255);
			if (mp == 5) {
				vertices[0] = video::S3DVertex(-d,-d,-1, 0,0,1, c, t, t);
				vertices[1] = video::S3DVertex( d,-d,-1, 0,0,1, c, o, t);
				vertices[2] = video::S3DVertex( d, d,-1, 0,0,1, c, o, o);
				vertices[3] = video::S3DVertex(-d, d,-1, 0,0,1, c, t, o);
			}else if (mp < 5) {
				float dp = -d+(d*(m_moon_phase*4));
				vertices[0] = video::S3DVertex(-d,-d,-1, 0,0,1, c, t, t);
				vertices[1] = video::S3DVertex(dp,-d,-1, 0,0,1, c, 1.0-(m_moon_phase*2), t);
				vertices[2] = video::S3DVertex(dp, d,-1, 0,0,1, c, 1.0-(m_moon_phase*2), o);
				vertices[3] = video::S3DVertex(-d, d,-1, 0,0,1, c, t, o);
			}else{
				float dp = d-(d*((1.0-m_moon_phase)*4));
				vertices[0] = video::S3DVertex(dp,-d,-1, 0,0,1, c, (1.0-m_moon_phase)*2, t);
				vertices[1] = video::S3DVertex( d,-d,-1, 0,0,1, c, o, t);
				vertices[2] = video::S3DVertex( d, d,-1, 0,0,1, c, o, o);
				vertices[3] = video::S3DVertex(dp, d,-1, 0,0,1, c, (1.0-m_moon_phase)*2, o);
			}
			for (u32 i=0; i<4; i++) {
				// Switch from -Z (south) to -X (west)
				vertices[i].Pos.rotateXZBy(-90);
				vertices[i].Pos.rotateXYBy(wicked_time_of_day * 360 - 90);
			}
			driver->drawIndexedTriangleFan(&vertices[0], 4, indices, 2);
		}
	}else{
		m_moon_phase = m_moon_phase_pending;
	}

	// Stars
	float starbrightness = MYMAX(
		0,
		MYMIN(
			1,
			(0.285 - fabs(
				wicked_time_of_day < 0.5 ? wicked_time_of_day : (1.0 - wicked_time_of_day)
			)) * 10
		)
	);
	float f = starbrightness*120;
	if (f >= m_skycolor.getBlue()) {
		driver->setMaterial(m_materials[1]);
		video::SColor starcolor(255, f,f,f);
		u16 indices[SKY_STAR_COUNT*4];
		video::S3DVertex vertices[SKY_STAR_COUNT*4];
		for (u32 i=0; i<SKY_STAR_COUNT; i++) {
			indices[i] = i;
			v3f p = m_stars[i];
			core::CMatrix4<f32> a;
			p.rotateXYBy(wicked_time_of_day * 360 - 90);
			vertices[i].Pos = p;
			vertices[i].Color = starcolor;
		}
		driver->drawVertexPrimitiveList(
			vertices,
			SKY_STAR_COUNT,
			indices,
			SKY_STAR_COUNT,
			video::EVT_STANDARD,
			scene::EPT_POINTS,
			video::EIT_16BIT
		);
	}
}

void Sky::update(
	float time_of_day,
	float moon_phase,
	float time_brightness,
	float direct_brightness,
	bool sunlight_seen
)
{
	// Stabilize initial brightness and color values by flooding updates
	if (m_first_update) {
		m_first_update = false;
		m_moon_phase = moon_phase;
		for (u32 i=0; i<100; i++) {
			update(time_of_day, moon_phase, time_brightness, direct_brightness, sunlight_seen);
		}
		return;
	}

	m_time_of_day = time_of_day;
	m_moon_phase_pending = moon_phase;
	m_time_brightness = time_brightness;
	m_sunlight_seen = sunlight_seen;

	bool is_dawn = (time_brightness >= 0.20 && time_brightness < 0.50);

	video::SColorf bgcolor_bright_normal_f(170./255,200./255,230./255, 1.0);
	video::SColorf bgcolor_bright_indoor_f(100./255,100./255,100./255, 1.0);
	video::SColorf bgcolor_bright_dawn_f(0.666*1.2,0.549*1.0,0.220*1.2,1.0);

	video::SColorf skycolor_bright_normal_f = video::SColor(255, 121, 141, 232);
	video::SColorf skycolor_bright_dawn_f = video::SColor(255, 46, 60, 132);

	video::SColorf cloudcolor_bright_normal_f = video::SColor(255, 240,240,255);
	video::SColorf cloudcolor_bright_dawn_f(1.0, 0.7, 0.5, 1.0);

	if (sunlight_seen) {
		if (fabs(time_brightness - m_brightness) < 0.2) {
			m_brightness = m_brightness * 0.95 + time_brightness * 0.05;
		}else{
			m_brightness = m_brightness * 0.80 + time_brightness * 0.20;
		}
	}else if (direct_brightness < m_brightness) {
		m_brightness = m_brightness * 0.95 + direct_brightness * 0.05;
	}else{
		m_brightness = m_brightness * 0.98 + direct_brightness * 0.02;
	}

	m_clouds_visible = true;
	float color_change_fraction = 0.98;
	if (sunlight_seen) {
		if (is_dawn) {
			m_bgcolor_bright_f = m_bgcolor_bright_f.getInterpolated(bgcolor_bright_dawn_f, color_change_fraction);
			m_skycolor_bright_f = m_skycolor_bright_f.getInterpolated(skycolor_bright_dawn_f, color_change_fraction);
			m_cloudcolor_bright_f = m_cloudcolor_bright_f.getInterpolated(cloudcolor_bright_dawn_f, color_change_fraction);
		}else{
			m_bgcolor_bright_f = m_bgcolor_bright_f.getInterpolated(bgcolor_bright_normal_f, color_change_fraction);
			m_skycolor_bright_f = m_skycolor_bright_f.getInterpolated(skycolor_bright_normal_f, color_change_fraction);
			m_cloudcolor_bright_f = m_cloudcolor_bright_f.getInterpolated(cloudcolor_bright_normal_f, color_change_fraction);
		}
	}else{
		m_bgcolor_bright_f = m_bgcolor_bright_f.getInterpolated(bgcolor_bright_indoor_f, color_change_fraction);
		m_cloudcolor_bright_f = m_cloudcolor_bright_f.getInterpolated(cloudcolor_bright_normal_f, color_change_fraction);
		m_skycolor_bright_f = m_skycolor_bright_f.getInterpolated(bgcolor_bright_indoor_f, color_change_fraction);
		m_clouds_visible = false;
	}
	video::SColor bgcolor_bright = m_bgcolor_bright_f.toSColor();
	m_bgcolor = video::SColor(
		255,
		bgcolor_bright.getRed() * m_brightness,
		bgcolor_bright.getGreen() * m_brightness,
		bgcolor_bright.getBlue() * m_brightness
	);

	video::SColor skycolor_bright = m_skycolor_bright_f.toSColor();
	m_skycolor = video::SColor(
		255,
		skycolor_bright.getRed() * m_brightness,
		skycolor_bright.getGreen() * m_brightness,
		skycolor_bright.getBlue() * m_brightness
	);

	float cloud_direct_brightness = 0;
	if (sunlight_seen) {
		cloud_direct_brightness = time_brightness;
		if (time_brightness >= 0.2 && time_brightness < 0.7)
			cloud_direct_brightness *= 1.3;
	}else{
		cloud_direct_brightness = direct_brightness;
	}
	m_cloud_brightness = m_cloud_brightness * 0.95 + cloud_direct_brightness * (1.0 - 0.95);
	m_cloudcolor_f = video::SColorf(
		m_cloudcolor_bright_f.getRed() * m_cloud_brightness,
		m_cloudcolor_bright_f.getGreen() * m_cloud_brightness,
		m_cloudcolor_bright_f.getBlue() * m_cloud_brightness,
		1.0
	);
}
