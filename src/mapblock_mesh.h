/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mapblock_mesh.h
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

#ifndef MAPBLOCK_MESH_HEADER
#define MAPBLOCK_MESH_HEADER

#include "common_irrlicht.h"
#include "mapblock_nodemod.h"
#include "voxel.h"
#include <vector>

/*
	Mesh making stuff
*/

/*
	This is used because CMeshBuffer::append() is very slow
*/
struct PreMeshBuffer
{
	video::SMaterial material;
	core::array<u16> indices;
	core::array<video::S3DVertex> vertices;
};

class MeshCollector
{
public:
	void append(
		video::SMaterial material,
		const video::S3DVertex* const vertices,
		u32 numVertices,
		const u16* const indices,
		u32 numIndices
	)
	{
		PreMeshBuffer *p = NULL;
		for (u32 i=0; i<m_prebuffers.size(); i++) {
			PreMeshBuffer &pp = m_prebuffers[i];
			if (pp.material != material)
				continue;
			if (pp.vertices.size() + numVertices > 65535)
				continue;

			p = &pp;
			break;
		}

		if (p == NULL) {
			PreMeshBuffer pp;
			pp.material = material;
			m_prebuffers.push_back(pp);
			p = &m_prebuffers[m_prebuffers.size()-1];
		}

		u32 vertex_count = p->vertices.size();
		for(u32 i=0; i<numIndices; i++) {
			u32 j = indices[i] + vertex_count;
			p->indices.push_back(j);
		}
		for(u32 i=0; i<numVertices; i++) {
			p->vertices.push_back(vertices[i]);
		}
	}

	void fillMesh(scene::SMesh *mesh)
	{
		/*dstream<<"Filling mesh with "<<m_prebuffers.size()
				<<" meshbuffers"<<std::endl;*/
		for(u32 i=0; i<m_prebuffers.size(); i++)
		{
			PreMeshBuffer &p = m_prebuffers[i];
			// Create meshbuffer
			// This is a "Standard MeshBuffer",
			// it's a typedeffed CMeshBuffer<video::S3DVertex>
			scene::SMeshBuffer *buf = new scene::SMeshBuffer();
			// Set material
			buf->Material = p.material;
			// Add to mesh
			mesh->addMeshBuffer(buf);
			// Mesh grabbed it
			buf->drop();

			buf->append(p.vertices.pointer(), p.vertices.size(), p.indices.pointer(), p.indices.size());
		}
	}

private:
	core::array<PreMeshBuffer> m_prebuffers;
};

// Helper functions
video::SColor MapBlock_LightColor(u8 alpha, u8 light, bool selected=false);
TileSpec getNodeTile(MapNode mn, v3s16 p, v3s16 face_dir, NodeModMap &temp_mods, NodeMetadata *meta = NULL);
TileSpec getMetaTile(MapNode mn, v3s16 p, v3s16 face_dir, NodeModMap &temp_mods);
u8 getSmoothLight(v3s16 p, VoxelManipulator &vmanip, u32 daynight_ratio);
u8 getSmoothLight(v3s16 p, v3s16 corner, VoxelManipulator &vmanip, u32 daynight_ratio);

class MapBlock;
class Environment;

struct MeshData
{
	bool single;
	v3s16 pos;
	video::SMaterial material;
	std::vector<u16> indices;
	std::vector<video::S3DVertex> vertices;
	std::vector<video::SColor> colours[18];
	std::vector<MeshData*> siblings;
	MeshData* parent;
};

struct MapBlockSound
{
	int id;
	std::string name;
};

struct MeshMakeData
{
	u32 m_daynight_ratio;
	NodeModMap m_temp_mods;
	VoxelManipulator m_vmanip;
	v3s16 m_blockpos;
	v3s16 m_blockpos_nodes;
	bool m_smooth_lighting;
	Environment *m_env;
	std::vector<MeshData> m_meshdata;
	MeshData *m_single;

	std::map<v3s16,MapBlockSound> *m_sounds;

	MeshMakeData():
		m_single(NULL),
		m_sounds(NULL)
	{}

	/*
		Copy central data directly from block, and other data from
		parent of block.
	*/
	void fill(u32 daynight_ratio, MapBlock *block);

	void startSingle(v3s16 pos, video::SMaterial material)
	{
		MeshData dd;
		dd.single = true;
		dd.pos = pos;
		dd.material = material;
		dd.parent = NULL;
		m_meshdata.push_back(dd);
		m_single = &m_meshdata[m_meshdata.size()-1];
	}
	void endSingle()
	{
		m_single = NULL;
	}
	void append(
		video::SMaterial material,
		const video::S3DVertex* const vertices,
		u32 v_count,
		const u16* const indices,
		u32 i_count,
		std::vector<video::SColor> colours[18]
	)
	{
		MeshData *d = NULL;
		if (m_single) {
			if (m_single->parent)
				m_single = m_single->parent;
			if (m_single->material != material) {
				for (u16 i=0; i<m_single->siblings.size(); i++) {
					if (m_single->siblings[i]->material == material) {
						d = m_single->siblings[i];
						break;
					}
				}
				if (d == NULL) {
					MeshData dd;
					dd.single = true;
					dd.pos = m_single->pos;
					dd.material = material;
					dd.parent = m_single;
					m_meshdata.push_back(dd);
					d = &m_meshdata[m_meshdata.size()-1];
					m_single->siblings.push_back(d);
				}
			}else{
				d = m_single;
			}
		}else{
			for (u32 i=0; i<m_meshdata.size(); i++) {
				MeshData &dd = m_meshdata[i];
				if (dd.material != material)
					continue;
				if (dd.vertices.size() + v_count > 65535)
					continue;

				d = &dd;
				break;
			}

			if (d == NULL) {
				MeshData dd;
				dd.single = false;
				dd.material = material;
				m_meshdata.push_back(dd);
				d = &m_meshdata[m_meshdata.size()-1];
			}
		}

		for (u32 i=0; i<18; i++) {
			for (u32 k=0; k<colours[i].size(); k++) {
				d->colours[i].push_back(colours[i][k]);
			}
		}

		u32 vertex_count = d->vertices.size();
		for(u32 i=0; i<i_count; i++) {
			u32 j = indices[i] + vertex_count;
			d->indices.push_back(j);
		}
		for(u32 i=0; i<v_count; i++) {
			d->vertices.push_back(vertices[i]);
		}
	}
};

class MapBlockMesh
{
public:
	MapBlockMesh(MeshMakeData *data, v3s16 camera_offset);
	~MapBlockMesh();

	scene::SMesh* getMesh()
	{
		return m_mesh;
	}

	void generate(MeshMakeData *data, v3s16 camera_offset);
	void refresh(u32 daynight_ratio);

	void updateCameraOffset(v3s16 camera_offset);

private:
	scene::SMesh *m_mesh;
	v3s16 m_camera_offset;
	std::vector<MeshData> m_meshdata;
};

#endif

