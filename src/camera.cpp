/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* camera.cpp
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

#include "camera.h"
#include "debug.h"
#include "client.h"
#include "main.h" // for g_settings
#include "map.h"
#include "mesh.h"
#include "player.h"
#include "tile.h"
#include <cmath>
#include <SAnimatedMesh.h>
#include "settings.h"
#include "path.h"
#include "constants.h"

#define CAMERA_OFFSET_STEP 200

Camera::Camera(scene::ISceneManager* smgr, MapDrawControl& draw_control, Client *client):
	m_client(client),
	m_smgr(smgr),
	m_playernode(NULL),
	m_headnode(NULL),
	m_cameranode(NULL),

	m_wieldmgr(NULL),
	m_wieldnode(NULL),
	m_wieldnode_baserotation(-100, 110, -100),
	m_wieldnode_baseposition(45, -35, 65),

	m_draw_control(draw_control),

	m_camera_position(0,0,0),
	m_camera_direction(0,0,0),
	m_camera_offset(0,0,0),

	m_aspect(1.0),
	m_fov_x(1.0),
	m_fov_y(1.0),

	m_added_frametime(0),
	m_added_frames(0),
	m_range_old(0),
	m_frametime_old(0),
	m_frametime_counter(0),
	m_time_per_range(30. / 50), // a sane default of 30ms per 50 nodes of range

	m_view_bobbing(true),
	m_view_bobbing_anim(0),
	m_view_bobbing_state(0),
	m_view_bobbing_speed(0),

	m_digging_anim(0),
	m_digging_button(-1)
{
	// note: making the camera node a child of the player node
	// would lead to unexpected behaviour, so we don't do that.
	m_playernode = smgr->addEmptySceneNode(smgr->getRootSceneNode());
	m_headnode = smgr->addEmptySceneNode(m_playernode);
	m_cameranode = smgr->addCameraSceneNode(smgr->getRootSceneNode());
	m_cameranode->bindTargetAndRotation(true);

	// This needs to be in its own scene manager. It is drawn after
	// all other 3D scene nodes and before the GUI.
	m_wieldmgr = smgr->createNewSceneManager();
	m_wieldmgr->addCameraSceneNode();
	m_wieldnode = new ExtrudedSpriteSceneNode(m_wieldmgr->getRootSceneNode(), m_wieldmgr);

	// Get FOV setting
	m_fov = g_settings->getFloat("fov");
	m_fov = MYMAX(m_fov, 10.0);
	m_fov = MYMIN(m_fov, 170.0);

	m_view_bobbing_amount = g_settings->getFloat("view_bobbing_amount");

	f32 wanted_fps = g_settings->getFloat("wanted_fps");
	wanted_fps = MYMAX(wanted_fps, 1.0);
	m_wanted_frametime = 1.0 / wanted_fps;
}

Camera::~Camera()
{
	m_wieldmgr->drop();
	m_wieldnode->drop();
}

bool Camera::successfullyCreated(std::wstring& error_message)
{
	if (m_playernode == NULL) {
		error_message = L"Failed to create the player scene node";
		return false;
	}
	if (m_headnode == NULL) {
		error_message = L"Failed to create the head scene node";
		return false;
	}
	if (m_cameranode == NULL) {
		error_message = L"Failed to create the camera scene node";
		return false;
	}
	if (m_wieldmgr == NULL) {
		error_message = L"Failed to create the wielded item scene manager";
		return false;
	}
	if (m_wieldnode == NULL) {
		error_message = L"Failed to create the wielded item scene node";
		return false;
	}
	return true;
}

// Returns the fractional part of x
inline f32 my_modf(f32 x)
{
	double dummy;
	return modf(x, &dummy);
}

void Camera::step(f32 dtime)
{
	if (m_view_bobbing_state != 0) {
		f32 offset = dtime * m_view_bobbing_speed * 0.030;
		if (m_view_bobbing_state == 2) {
			// Animation is getting turned off
			if(m_view_bobbing_anim < 0.25){
				m_view_bobbing_anim -= offset;
			} else if(m_view_bobbing_anim > 0.75){
				m_view_bobbing_anim += offset;
			} if(m_view_bobbing_anim < 0.5){
				m_view_bobbing_anim += offset;
				if(m_view_bobbing_anim > 0.5)
					m_view_bobbing_anim = 0.5;
			} else {
				m_view_bobbing_anim -= offset;
				if(m_view_bobbing_anim < 0.5)
					m_view_bobbing_anim = 0.5;
			}
			if(m_view_bobbing_anim <= 0 || m_view_bobbing_anim >= 1 ||
					fabs(m_view_bobbing_anim - 0.5) < 0.01)
			{
				m_view_bobbing_anim = 0;
				m_view_bobbing_state = 0;
			}
		}else{
			float was = m_view_bobbing_anim;
			m_view_bobbing_anim = my_modf(m_view_bobbing_anim + offset);
			if (
				was == 0
				|| (was < 0.5f && m_view_bobbing_anim >= 0.5f)
				|| (was > 0.5f && m_view_bobbing_anim <= 0.5f)
			) {
				m_client->playStepSound();
			}
		}
	}

	if (m_digging_button != -1) {
		f32 offset = dtime * 3.5;
		float m_digging_anim_was = m_digging_anim;
		m_digging_anim += offset;
		if (m_digging_anim >= 1) {
			m_digging_anim = 0;
			m_digging_button = -1;
		}
		float lim = 0.15;
		if (m_digging_anim_was < lim && m_digging_anim >= lim) {
			if (m_digging_button == 0) {
				m_client->playDigSound(CONTENT_IGNORE);
			}else if(m_digging_button == 1) {
				m_client->playPlaceSound(CONTENT_IGNORE);
			}
		}
	}
}

void Camera::update(LocalPlayer* player, f32 frametime, v2u32 screensize)
{
	// Set player node transformation
	m_playernode->setPosition(player->getPosition());
	m_playernode->setRotation(v3f(0, -1 * player->getYaw(), 0));
	m_playernode->updateAbsolutePosition();

	// Set head node transformation
	m_headnode->setPosition(player->getEyeOffset());
	m_headnode->setRotation(v3f(player->getPitch(), 0, 0));
	m_headnode->updateAbsolutePosition();

	// Compute relative camera position and target
	v3f rel_cam_pos = v3f(0,0,0);
	v3f rel_cam_target = v3f(0,0,1);
	v3f rel_cam_up = v3f(0,1,0);

	if (m_view_bobbing_anim != 0)
	{
		f32 bobfrac = my_modf(m_view_bobbing_anim * 2);
		f32 bobdir = (m_view_bobbing_anim < 0.5) ? 1.0 : -1.0;
		f32 bobknob = 1.2;
		f32 bobtmp = sin(pow(bobfrac, bobknob) * PI);

		v3f bobvec = v3f(
			0.3 * bobdir * sin(bobfrac * PI),
			-0.28 * bobtmp * bobtmp,
			0.);

		float f = 1.0;
		f *= m_view_bobbing_amount;
		rel_cam_pos += bobvec * f;
		rel_cam_target += bobvec * f;
		rel_cam_target.Z -= 0.005 * bobvec.Z * f;
		rel_cam_up.rotateXYBy(-0.03 * bobdir * bobtmp * PI * f);

	}

	// Compute absolute camera position and target
	m_headnode->getAbsoluteTransformation().transformVect(m_camera_position, rel_cam_pos);
	m_headnode->getAbsoluteTransformation().rotateVect(m_camera_direction, rel_cam_target - rel_cam_pos);

	v3f abs_cam_up;
	m_headnode->getAbsoluteTransformation().rotateVect(abs_cam_up, rel_cam_up);

	// Update offset if too far away from the center of the map

	m_camera_offset.X += CAMERA_OFFSET_STEP*
			(((s16)(m_camera_position.X/BS) - m_camera_offset.X)/CAMERA_OFFSET_STEP);
	m_camera_offset.Y += CAMERA_OFFSET_STEP*
			(((s16)(m_camera_position.Y/BS) - m_camera_offset.Y)/CAMERA_OFFSET_STEP);
	m_camera_offset.Z += CAMERA_OFFSET_STEP*
			(((s16)(m_camera_position.Z/BS) - m_camera_offset.Z)/CAMERA_OFFSET_STEP);

	// Set camera node transformation
	m_cameranode->setPosition(m_camera_position-intToFloat(m_camera_offset, BS));
	m_cameranode->setUpVector(abs_cam_up);
	// *100.0 helps in large map coordinates
	m_cameranode->setTarget(m_camera_position-intToFloat(m_camera_offset, BS) + 100 * m_camera_direction);

	// FOV and aspect ratio
	m_aspect = (f32)screensize.X / (f32) screensize.Y;
	m_fov_y = m_fov * PI / 180.0;
	m_fov_x = 2 * atan(0.5 * m_aspect * tan(m_fov_y));
	m_cameranode->setAspectRatio(m_aspect);
	m_cameranode->setFOV(m_fov_y);

	// Position the wielded item
	v3f wield_position = m_wieldnode_baseposition;
	v3f wield_rotation = m_wieldnode_baserotation;
	if (m_digging_button != -1)
	{
		f32 digfrac = m_digging_anim;
		wield_position.X -= 30 * sin(pow(digfrac, 0.8f) * PI);
		wield_position.Y += 15 * sin(digfrac * 2 * PI);
		wield_position.Z += 5 * digfrac;

		// Euler angles are PURE EVIL, so why not use quaternions?
		core::quaternion quat_begin(wield_rotation * core::DEGTORAD);
		core::quaternion quat_end(v3f(90, -10, -130) * core::DEGTORAD);
		core::quaternion quat_slerp;
		quat_slerp.slerp(quat_begin, quat_end, sin(digfrac * PI));
		quat_slerp.toEuler(wield_rotation);
		wield_rotation *= core::RADTODEG;
	}
	else {
		f32 bobfrac = my_modf(m_view_bobbing_anim);
		wield_position.X -= sin(bobfrac*PI*2.0) * 3.0;
		wield_position.Y += sin(my_modf(bobfrac*2.0)*PI) * 3.0;
	}
	m_wieldnode->setPosition(wield_position);
	m_wieldnode->setRotation(wield_rotation);

	u8 li = decode_light(player->light);
	// Set brightness one lower than incoming light
	diminish_light(li);
	m_wieldnode->updateLight(li);

	// Render distance feedback loop
	updateViewingRange(frametime);

	// If the player seems to be walking on solid ground,
	// view bobbing is enabled and free_move is off,
	// start (or continue) the view bobbing animation.
	v3f speed = player->getSpeed();
	if (
		(hypot(speed.X, speed.Z) > BS)
		&& (player->touching_ground)
		&& m_view_bobbing == true
		&& player->control.free == false
	) {
		// Start animation
		m_view_bobbing_state = 1;
		m_view_bobbing_speed = MYMIN(speed.getLength(), 40);
	}else if (m_view_bobbing_state == 1) {
		// Stop animation
		m_view_bobbing_state = 2;
		m_view_bobbing_speed = 60;
	}
}

void Camera::updateViewingRange(f32 frametime_in)
{
	if (m_draw_control.range_all)
		return;

	m_added_frametime += frametime_in;
	m_added_frames += 1;

	// Actually this counter kind of sucks because frametime is busytime
	m_frametime_counter -= frametime_in;
	if (m_frametime_counter > 0)
		return;
	m_frametime_counter = 0.2;

	// Get current viewing range and FPS settings
	f32 viewing_range_min = g_settings->getS16("viewing_range_nodes_min");
	viewing_range_min = MYMAX(5.0, viewing_range_min);

	f32 viewing_range_max = g_settings->getS16("viewing_range_nodes_max");
	viewing_range_max = MYMAX(viewing_range_min, viewing_range_max);

	m_draw_control.wanted_min_range = viewing_range_min;
	m_draw_control.wanted_max_blocks = (2.0*m_draw_control.blocks_would_have_drawn)+1;
	if (m_draw_control.wanted_max_blocks < 10)
		m_draw_control.wanted_max_blocks = 10;

	f32 block_draw_ratio = 1.0;
	if (m_draw_control.blocks_would_have_drawn != 0)
		block_draw_ratio = (f32)m_draw_control.blocks_drawn / (f32)m_draw_control.blocks_would_have_drawn;

	// Calculate the average frametime in the case that all wanted
	// blocks had been drawn
	f32 frametime = m_added_frametime / m_added_frames / block_draw_ratio;

	m_added_frametime = 0.0;
	m_added_frames = 0;

	f32 wanted_frametime_change = m_wanted_frametime - frametime;

	// If needed frametime change is small, just return
	// This value was 0.4 for many months until 2011-10-18 by c55;
	// Let's see how this works out.
	if (fabs(wanted_frametime_change) < m_wanted_frametime*0.33)
		return;

	f32 range = m_draw_control.wanted_range;
	f32 new_range = range;

	f32 d_range = range - m_range_old;
	f32 d_frametime = frametime - m_frametime_old;
	if (d_range != 0)
		m_time_per_range = d_frametime / d_range;

	// The minimum allowed calculated frametime-range derivative:
	// Practically this sets the maximum speed of changing the range.
	// The lower this value, the higher the maximum changing speed.
	// A low value here results in wobbly range (0.001)
	// A high value here results in slow changing range (0.0025)
	// SUGG: This could be dynamically adjusted so that when
	//       the camera is turning, this is lower
	f32 min_time_per_range = 0.0010;
	if (m_time_per_range < min_time_per_range)
		m_time_per_range = min_time_per_range;

	f32 wanted_range_change = wanted_frametime_change / m_time_per_range;
	// Dampen the change a bit to kill oscillations
	wanted_range_change *= 0.5;

	// If needed range change is very small, just return
	if (fabs(wanted_range_change) < 0.001)
		return;

	new_range += wanted_range_change;
	new_range = MYMAX(new_range, viewing_range_min);
	new_range = MYMIN(new_range, viewing_range_max);

	m_draw_control.wanted_range = new_range;

	m_range_old = new_range;
	m_frametime_old = frametime;
	// Just so big a value that everything rendered is visible
	// Some more allowance than viewing_range_max * BS because of active objects etc.
	m_cameranode->setFarValue(viewing_range_max * BS * 10);
}

void Camera::wield(const InventoryItem* item)
{
	if (item != NULL) {
		bool haveWield = false;
		m_wieldnode_baserotation = v3f(-100, 110, -100);
		m_wieldnode_baseposition = v3f(45, -35, 65);

		// Try to make a MaterialItem cube.
		if (std::string(item->getName()) == "MaterialItem") {
			// A block-type material
			MaterialItem* mat_item = (MaterialItem*) item;
			content_t content = mat_item->getMaterial();
			if (content_features(content).solidness || content_features(content).visual_solidness) {
				m_wieldnode->setCube(content_features(content).tiles);
				m_wieldnode->setScale(v3f(30));
				haveWield = true;
			}else if (
				(
					content_features(content).draw_type == CDT_NODEBOX
					|| content_features(content).draw_type == CDT_NODEBOX_META
					|| content_features(content).draw_type == CDT_FENCELIKE
					|| content_features(content).draw_type == CDT_WALLLIKE
				)
				&& content_features(content).wield_nodebox == true
			) {
				m_wieldnode->setNodeBox(content);
				m_wieldnode->setScale(v3f(30));
				m_wieldnode_baserotation = v3f(-10, 10, -10);
				haveWield = true;
			}
		}

		// If that failed, make an extruded sprite.
		if (!haveWield) {
			m_wieldnode->setSprite(item->getImageRaw());
			m_wieldnode->setScale(v3f(40));
		}

		m_wieldnode->setVisible(true);
	}else{
		// Bare hands
		m_wieldnode->setArm(m_client->getLocalPlayer()->getTexture());
		m_wieldnode_baserotation = v3f(-30, 130, 20);
		m_wieldnode_baseposition = v3f(45, -43, 60);
		m_wieldnode->setScale(v3f(40));
		m_wieldnode->setVisible(true);
	}
}

void Camera::setDigging(s32 button)
{
	if (m_digging_button == -1)
		m_digging_button = button;
}

void Camera::drawWieldedTool()
{
	m_wieldmgr->getVideoDriver()->clearZBuffer();

	scene::ICameraSceneNode* cam = m_wieldmgr->getActiveCamera();
	cam->setAspectRatio(m_cameranode->getAspectRatio());
	cam->setFOV(m_cameranode->getFOV());
	cam->setNearValue(0.1);
	cam->setFarValue(100);
	m_wieldmgr->drawAll();
}

