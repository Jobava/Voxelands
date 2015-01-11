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
			// mesh buffers have a limit of 65535 (16bit)
			// for vertices, so don't use a buffer that
			// doesn't have enough space
			u32 vc = numVertices+pp.vertices.size();
			if (vc > 65535)
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

		// optimise vertices by removing duplicates
		for (u32 i=0; i<numIndices; i++) {
			video::S3DVertex v = vertices[indices[i]];
			u32 vc = p->vertices.size();
			bool dup = false;
			for (u32 k=0; k<vc; k++) {
				if (p->vertices[k] == v) {
					dup = true;
					p->indices.push_back(k);
					break;
				}
			}
			if (dup)
				continue;
			p->indices.push_back(vc);
			p->vertices.push_back(v);
		}
	}

	void fillMesh(scene::SMesh *mesh)
	{
		printf("m_prebuffers.size() = %u\n",m_prebuffers.size());
		for (u32 i=0; i<m_prebuffers.size(); i++) {
			PreMeshBuffer &p = m_prebuffers[i];

			// calculate normals
			u16 numVertices = p.vertices.size();
			u16 numIndices = p.indices.size();
			for (u16 k=0; (k+2)<numIndices; k+=3) {
				u16 i1 = p.indices[k];
				u16 i2 = p.indices[k+1];
				u16 i3 = p.indices[k+2];
				v3f v1 = p.vertices[i1].Pos;
				v3f v2 = p.vertices[i2].Pos;
				v3f v3 = p.vertices[i3].Pos;

				v3f b1 = v1-v2;
				v3f b2 = v2-v3;
				v3f cp;

				v3f normal;

				cp = b1.crossProduct(b2);
				normal = cp.normalize();

				p.vertices[i1].Normal += normal;
				p.vertices[i2].Normal += normal;
				p.vertices[i3].Normal += normal;
			}
			for (u16 k=0; k<numVertices; k++) {
				p.vertices[k].Normal = p.vertices[k].Normal.normalize();
			}

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

			buf->append(p.vertices.pointer(), p.vertices.size(),
					p.indices.pointer(), p.indices.size());
		}
	}

private:
	core::array<PreMeshBuffer> m_prebuffers;
};

// Helper functions
video::SColor MapBlock_LightColor(u8 alpha, u8 light, bool selected=false);
TileSpec getNodeTile(MapNode mn, v3s16 p, v3s16 face_dir, NodeModMap &temp_mods);
TileSpec getMetaTile(MapNode mn, v3s16 p, v3s16 face_dir, NodeModMap &temp_mods);

class MapBlock;
class Environment;

struct MeshLightSource {
	v3s16 pos;
	u8 value;
	video::SColor colour;
};

struct MeshMakeData
{
	u32 m_daynight_ratio;
	NodeModMap m_temp_mods;
	VoxelManipulator m_vmanip;
	v3s16 m_blockpos;
	Environment *m_env;
	std::vector<MeshLightSource> m_light_sources;

	void addLight(v3s16 pos, u8 brightness, video::SColor colour)
	{
		MeshLightSource l;
		for (std::vector<MeshLightSource>::iterator i=m_light_sources.begin(); i != m_light_sources.end(); i++) {
			if ((*i).pos == pos) {
				(*i).value = brightness;
				(*i).colour = colour;
				return;
			}
		}
		l.pos = pos;
		l.value = brightness;
		l.colour = colour;
		m_light_sources.push_back(l);
	}

	/*
		Copy central data directly from block, and other data from
		parent of block.
	*/
	void fill(u32 daynight_ratio, MapBlock *block);
};

// This is the highest-level function in here
scene::SMesh* makeMapBlockMesh(MeshMakeData *data);

class MapBlockMesh
{
public:
	MapBlockMesh(MeshMakeData *data, v3s16 camera_offset);
	~MapBlockMesh();

	scene::SMesh* getMesh()
	{
		return m_mesh;
	}

	void updateCameraOffset(v3s16 camera_offset);

	void addLight(v3s16 pos, u8 brightness, video::SColor colour)
	{
		MeshLightSource l;
		for (std::vector<MeshLightSource>::iterator i=m_light_sources.begin(); i != m_light_sources.end(); i++) {
			if ((*i).pos == pos) {
				(*i).value = brightness;
				(*i).colour = colour;
				return;
			}
		}
		l.pos = pos;
		l.value = brightness;
		l.colour = colour;
		m_light_sources.push_back(l);
	}

	std::vector<MeshLightSource> m_light_sources;

private:
	scene::SMesh *m_mesh;
	v3s16 m_camera_offset;
};

#endif

