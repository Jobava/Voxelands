/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* farmesh.h
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

#ifndef FARMESH_HEADER
#define FARMESH_HEADER

/*
	A quick messy implementation of terrain rendering for a long
	distance according to map seed
*/

#include "common_irrlicht.h"
#include "mapgen.h"

#define FARMESH_MATERIAL_COUNT 2

class Client;

class FarMesh : public scene::ISceneNode
{
public:
	FarMesh(
			scene::ISceneNode* parent,
			scene::ISceneManager* mgr,
			s32 id,
			uint64_t seed,
			MapGenType type,
			Client *client
	);

	~FarMesh();

	/*
		ISceneNode methods
	*/

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return m_box;
	}

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

	/*
		Other stuff
	*/

	void step(float dtime);

	void update(v2f camera_p, float brightness, s16 render_range);

private:
	video::SMaterial m_materials[FARMESH_MATERIAL_COUNT];
	core::aabbox3d<f32> m_box;
	float m_brightness;
	uint64_t m_seed;
	MapGenType m_type;
	v2f m_camera_pos;
	float m_time;
	Client *m_client;
	s16 m_render_range;
};

#endif

