/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* clouds.h
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

#ifndef CLOUDS_HEADER
#define CLOUDS_HEADER

#include "common_irrlicht.h"
#include <iostream>

class Clouds : public scene::ISceneNode
{
public:
	Clouds(
			scene::ISceneNode* parent,
			scene::ISceneManager* mgr,
			s32 id,
			float cloud_y,
			u32 seed
	);

	~Clouds();

	/*
		ISceneNode methods
	*/

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return m_box;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return m_material;
	}

	/*
		Other stuff
	*/

	void step(float dtime);

	void update(v2f camera_p, float brightness);

	void updateCameraOffset(v3s16 camera_offset)
	{
		m_camera_offset = camera_offset;
	}

private:
	video::SMaterial m_material;
	core::aabbox3d<f32> m_box;
	float m_cloud_y;
	float m_brightness;
	u32 m_seed;
	v2f m_camera_pos;
	v3s16 m_camera_offset;
	float m_time;
};



#endif

