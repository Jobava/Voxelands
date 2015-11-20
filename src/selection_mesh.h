/************************************************************************
* selection_mesh.h
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
************************************************************************/

#ifndef SELECTION_MESH_HEADER
#define SELECTION_MESH_HEADER

#ifndef SERVER

#include "mapblock_mesh.h"

class Client;

void selection_draw(video::IVideoDriver* driver, Client &client, v3s16 camera_offset, std::vector<SelectedNode> &select);

class SelectionMesh
{
public:
	SelectionMesh(MeshMakeData *data);
	~SelectionMesh();

	scene::SMesh* getMesh()
	{
		return m_mesh;
	}

	void generate(MeshMakeData *data);
	void refresh(u32 daynight_ratio);

private:
	v3s16 m_pos;
	scene::SMesh *m_mesh;
	std::vector<MeshData> m_meshdata;
};

#endif

#endif
