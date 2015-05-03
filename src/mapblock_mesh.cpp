/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mapblock_mesh.cpp
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

#include "mapblock_mesh.h"
#include "light.h"
#include "mapblock.h"
#include "map.h"
#include "main.h" // For g_settings and g_texturesource
#include "content_mapblock.h"
#include "content_nodemeta.h"
#include "settings.h"
#include "profiler.h"
#include "mesh.h"
#include "base64.h"
#include "sound.h"

void MeshMakeData::fill(u32 daynight_ratio, MapBlock *block)
{
	m_daynight_ratio = daynight_ratio;
	m_blockpos = block->getPos();
	if (m_env)
		m_vmanip.m_env = m_env;

	v3s16 blockpos_nodes = m_blockpos*MAP_BLOCKSIZE;

	/*
		There is no harm not copying the TempMods of the neighbors
		because they are already copied to this block
	*/
	m_temp_mods.clear();
	block->copyTempMods(m_temp_mods);

	/*
		Copy data
	*/

	// Allocate this block + neighbors
	m_vmanip.clear();
	m_vmanip.addArea(VoxelArea(blockpos_nodes-v3s16(1,1,1)*MAP_BLOCKSIZE,
			blockpos_nodes+v3s16(1,1,1)*MAP_BLOCKSIZE*2-v3s16(1,1,1)));

	{
		//TimeTaker timer("copy central block data");
		// 0ms

		// Copy our data
		block->copyTo(m_vmanip);
	}
	{
		//TimeTaker timer("copy neighbor block data");
		// 0ms

		/*
			Copy neighbors. This is lightning fast.
			Copying only the borders would be *very* slow.
		*/

		// Get map
		Map *map = block->getParent();

		for(u16 i=0; i<6; i++)
		{
			const v3s16 &dir = g_6dirs[i];
			v3s16 bp = m_blockpos + dir;
			MapBlock *b = map->getBlockNoCreateNoEx(bp);
			if(b)
				b->copyTo(m_vmanip);
		}
	}
}

video::SColor MapBlock_LightColor(u8 alpha, u8 light, bool selected)
{
	float lim = 80;
	float power = 0.8;
	u8 r = light;
	u8 g = light;
	u8 b = light;
	// selected nodes glow a bit
	if (selected) {
		// TODO: make this colour a setting
		r = 128;
		g = 128;
		b = 255;
	}else if (light <= lim) {
		// Emphase blue a bit in darker places
		b = MYMAX(0, pow((float)light/lim, power)*lim);
	}

	return video::SColor(alpha,r,g,b);
}

/*
	Gets node tile from any place relative to block.
	Returns TILE_NODE if doesn't exist or should not be drawn.
*/
TileSpec getNodeTile(MapNode mn, v3s16 p, v3s16 face_dir, NodeModMap &temp_mods, NodeMetadata *meta)
{
	TileSpec spec;
	spec = mn.getTile(face_dir,false);

	if (meta) {
		FaceText ft = mn.getFaceText(face_dir);
		if (ft.m_hastext) {
			std::string txt("");
			switch (ft.m_type) {
			case FTT_BOOKCONTENT:
				txt = ((BookNodeMetadata*)meta)->getContent();
				break;
			case FTT_OWNER:
				txt = meta->getOwner();
				break;
			case FTT_INVOWNER:
				txt = meta->getInventoryOwner();
				break;
			default:
				txt = meta->getText();
				break;
			}
			if (txt != "") {
				// Get original texture name
				u32 orig_id = spec.texture.id;
				std::string orig_name = g_texturesource->getTextureName(orig_id);
				// Create new texture name
				std::ostringstream os;
				os<<orig_name<<"^[text:";
				os<<ft.m_pos.UpperLeftCorner.X;
				os<<",";
				os<<ft.m_pos.UpperLeftCorner.Y;
				os<<",";
				os<<ft.m_pos.LowerRightCorner.X;
				os<<",";
				os<<ft.m_pos.LowerRightCorner.Y;
				os<<",";
				os<<base64_encode((const unsigned char*)txt.c_str(),txt.size());

				// Get new texture
				u32 new_id = g_texturesource->getTextureId(os.str());

				spec.texture = g_texturesource->getTexture(new_id);
			}
		}
	}

	std::string rot = mn.getTileRotationString(face_dir);
	if (rot != "") {
		// Get original texture name
		u32 orig_id = spec.texture.id;
		std::string orig_name = g_texturesource->getTextureName(orig_id);
		// new name
		std::string texture_name = orig_name + rot;
		// Get new texture
		u32 new_id = g_texturesource->getTextureId(texture_name);
		spec.texture = g_texturesource->getTexture(new_id);
	}

	/*
		Check temporary modifications on this node
	*/
	NodeMod mod;
	if (temp_mods.get(p, &mod)) {
		if (mod == NODEMOD_CRACK) {
			/*
				Get texture id, translate it to name, append stuff to
				name, get texture id
			*/

			// Get original texture name
			u32 orig_id = spec.texture.id;
			std::string orig_name = g_texturesource->getTextureName(orig_id);

			// Create new texture name
			std::ostringstream os;
			os<<orig_name<<"^[crack"<<mod.param;

			// Get new texture
			u32 new_id = g_texturesource->getTextureId(os.str());

			spec.texture = g_texturesource->getTexture(new_id);
		}
		if (mod == NODEMOD_SELECTION) {
			// Get original texture name
			u32 orig_id = spec.texture.id;
			std::string orig_name = g_texturesource->getTextureName(orig_id);

			// Create new texture name
			std::ostringstream os;
			os<<orig_name<<"^[forcesingle";

			// Get new texture
			u32 new_id = g_texturesource->getTextureId(os.str());

			spec.texture = g_texturesource->getTexture(new_id);
		}
	}

	return spec;
}

/*
	Gets node meta tile from any place relative to block.
	Returns TILE_NODE if doesn't exist or should not be drawn.
*/
TileSpec getMetaTile(MapNode mn, v3s16 p, v3s16 face_dir,
		NodeModMap &temp_mods)
{
	TileSpec spec;
	spec = mn.getMetaTile(face_dir);

	/*
		Check temporary modifications on this node
	*/
	NodeMod mod;
	if (temp_mods.get(p, &mod)) {
		if (mod == NODEMOD_CRACK) {
			/*
				Get texture id, translate it to name, append stuff to
				name, get texture id
			*/

			// Get original texture name
			u32 orig_id = spec.texture.id;
			std::string orig_name = g_texturesource->getTextureName(orig_id);

			// Create new texture name
			std::ostringstream os;
			os<<orig_name<<"^[crack"<<mod.param;

			// Get new texture
			u32 new_id = g_texturesource->getTextureId(os.str());

			spec.texture = g_texturesource->getTexture(new_id);
		}
		if (mod == NODEMOD_SELECTION) {
			// Get original texture name
			u32 orig_id = spec.texture.id;
			std::string orig_name = g_texturesource->getTextureName(orig_id);

			// Create new texture name
			std::ostringstream os;
			os<<orig_name<<"^[forcesingle";

			// Get new texture
			u32 new_id = g_texturesource->getTextureId(os.str());

			spec.texture = g_texturesource->getTexture(new_id);
		}
	}

	return spec;
}

v3s16 dirs8[8] = {
	v3s16(0,0,0),
	v3s16(0,0,1),
	v3s16(0,1,0),
	v3s16(0,1,1),
	v3s16(1,0,0),
	v3s16(1,1,0),
	v3s16(1,0,1),
	v3s16(1,1,1),
};

// Calculate lighting at the XYZ- corner of p
u8 getSmoothLight(v3s16 p, VoxelManipulator &vmanip, u32 daynight_ratio)
{
	u16 ambient_occlusion = 0;
	u16 light = 0;
	u16 light_count = 0;
	for(u32 i=0; i<8; i++) {
		MapNode n = vmanip.getNodeNoEx(p - dirs8[i]);
		if (
			content_features(n).param_type == CPT_LIGHT
		) {
			light += decode_light(n.getLightBlend(daynight_ratio));
			light_count++;
		}else if (
			content_features(n).draw_type == CDT_NODEBOX
			|| content_features(n).draw_type == CDT_NODEBOX_META
		) {
			// not quite right, but it gets rid of glowing nodes
			light += decode_light(n.getLightBlend(daynight_ratio));
			light_count++;
		}else if (n.getContent() != CONTENT_IGNORE) {
			ambient_occlusion++;
		}
	}

	if (light_count == 0)
		return 0;

	light /= light_count;

	if (ambient_occlusion > 4) {
		ambient_occlusion -= 4;
		light = (float)light / ((float)ambient_occlusion * 0.5 + 1.0);
	}

	return light;
}

// Calculate lighting at the given corner of p
u8 getSmoothLight(v3s16 p, v3s16 corner,
		VoxelManipulator &vmanip, u32 daynight_ratio)
{
	if (corner.X == 1) {
		p.X += 1;
	}else{
		assert(corner.X == -1);
	}
	if (corner.Y == 1) {
		p.Y += 1;
	}else{
		assert(corner.Y == -1);
	}
	if (corner.Z == 1) {
		p.Z += 1;
	}else{
		assert(corner.Z == -1);
	}

	return getSmoothLight(p, vmanip, daynight_ratio);
}

MapBlockMesh::MapBlockMesh(MeshMakeData *data, v3s16 camera_offset):
	m_mesh(NULL),
	m_camera_offset(camera_offset)
{
	generate(data,camera_offset);
}

MapBlockMesh::~MapBlockMesh()
{
	m_mesh->drop();
	m_mesh = NULL;
}

void MapBlockMesh::generate(MeshMakeData *data, v3s16 camera_offset)
{
	data->m_blockpos_nodes = data->m_blockpos*MAP_BLOCKSIZE;
	data->m_smooth_lighting = g_settings->getBool("smooth_lighting");
	bool selected = false;

	for(s16 z=0; z<MAP_BLOCKSIZE; z++)
	for(s16 y=0; y<MAP_BLOCKSIZE; y++)
	for(s16 x=0; x<MAP_BLOCKSIZE; x++)
	{
		v3s16 p(x,y,z);

		MapNode n = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes+p);
		NodeMod mod;
		data->m_temp_mods.get(p,&mod);
		selected = (mod == NODEMOD_SELECTION);

		if (g_sound) {
			std::string snd = content_features(n).sound_ambient;
			std::map<v3s16,MapBlockSound>::iterator i = data->m_sounds->find(p);
			if (snd != "") {
				bool add_sound = true;
				if (i != data->m_sounds->end()) {
					if (i->second.name == snd && g_sound->soundExists(i->second.id)) {
						add_sound = false;
					}else{
						g_sound->stopSound(i->second.id);
					}
				}
				if (add_sound && content_features(n).liquid_type != LIQUID_NONE) {
					if (data->m_vmanip.getNodeRO(data->m_blockpos_nodes+p+v3s16(0,1,0)).getContent() != CONTENT_AIR) {
						add_sound = false;
					}else if (content_features(n).param2_type != CPT_LIQUID || n.param2 < 4 || n.param2 > 7) {
						add_sound = false;
					}else{
						int adj = 0;
						for (s16 x=-1; x<2; x++) {
							for (s16 z=-1; z<2; z++) {
								if (!x && !z)
									continue;
								content_t ac = data->m_vmanip.getNodeRO(data->m_blockpos_nodes+p+v3s16(x,0,z)).getContent();
								if (
									ac == content_features(n).liquid_alternative_flowing
									|| ac == content_features(n).liquid_alternative_source
								)
									adj++;
							}
						}
						if (adj > 3)
							add_sound = false;
					}
				}
				if (add_sound) {
					v3f pf = intToFloat(p+data->m_blockpos_nodes,BS);
					MapBlockSound bsnd;
					bsnd.id = g_sound->playSoundAt(snd,true,pf, true);
					bsnd.name = snd;
					if (bsnd.id > 0)
						(*data->m_sounds)[p] = bsnd;
				}
			}else if (i != data->m_sounds->end()) {
				g_sound->stopSound(i->second.id);
				data->m_sounds->erase(i);
			}
		}
		switch (content_features(n).draw_type) {
		case CDT_AIRLIKE:
			break;
		case CDT_CUBELIKE:
			meshgen_cubelike(data,p,n,selected);
			break;
		case CDT_RAILLIKE:
			meshgen_raillike(data,p,n,selected);
			break;
		case CDT_PLANTLIKE:
		case CDT_PLANTLIKE_SML:
		case CDT_PLANTLIKE_LGE:
		case CDT_PLANTGROWTH_1:
		case CDT_PLANTGROWTH_2:
		case CDT_PLANTGROWTH_3:
			meshgen_plantlike(data,p,n,selected);
			break;
		case CDT_LIQUID:
			meshgen_liquid(data,p,n,selected);
			break;
		case CDT_LIQUID_SOURCE:
			meshgen_liquid_source(data,p,n,selected);
			break;
		case CDT_NODEBOX:
			meshgen_nodebox(data,p,n,selected,false);
			break;
		case CDT_GLASSLIKE:
			meshgen_glasslike(data,p,n,selected);
			break;
		case CDT_TORCHLIKE:
			meshgen_torchlike(data,p,n,selected);
			break;
		case CDT_FENCELIKE:
			meshgen_fencelike(data,p,n,selected);
			break;
		case CDT_FIRELIKE:
			meshgen_firelike(data,p,n,selected);
			break;
		case CDT_WALLLIKE:
			meshgen_walllike(data,p,n,selected);
			break;
		case CDT_ROOFLIKE:
			meshgen_rooflike(data,p,n,selected);
			break;
		case CDT_LEAFLIKE:
			meshgen_leaflike(data,p,n,selected);
			break;
		case CDT_NODEBOX_META:
			meshgen_nodebox(data,p,n,selected,true);
			break;
		case CDT_WIRELIKE:
			meshgen_wirelike(data,p,n,selected,false);
			break;
		case CDT_3DWIRELIKE:
			meshgen_wirelike(data,p,n,selected,true);
			break;
		case CDT_STAIRLIKE:
			meshgen_stairlike(data,p,n,selected);
			break;
		case CDT_SLABLIKE:
			meshgen_slablike(data,p,n,selected);
			break;
		case CDT_TRUNKLIKE:
			meshgen_trunklike(data,p,n,selected);
			break;
		}
	}

	if (m_mesh) {
		scene::SMesh *m = m_mesh;
		m_mesh = NULL;
		m->drop();
	}
	m_meshdata.swap(data->m_meshdata);
	refresh(data->m_daynight_ratio);
	m_mesh->recalculateBoundingBox();

	translateMesh(m_mesh, intToFloat(data->m_blockpos * MAP_BLOCKSIZE - camera_offset, BS));
}

void MapBlockMesh::refresh(u32 daynight_ratio)
{
	if (m_mesh == NULL) {
		m_mesh = new scene::SMesh();
		for (u32 i=0; i<m_meshdata.size(); i++) {
			MeshData &d = m_meshdata[i];
			// Create meshbuffer
			// This is a "Standard MeshBuffer",
			// it's a typedeffed CMeshBuffer<video::S3DVertex>
			scene::SMeshBuffer *buf = new scene::SMeshBuffer();
			// Set material
			buf->Material = d.material;
			// Add to mesh
			m_mesh->addMeshBuffer(buf);
			// Mesh grabbed it
			buf->drop();

			buf->append(d.vertices.data(), d.vertices.size(), d.indices.data(), d.indices.size());
		}
	}

	u16 ci = daynight_ratio_index(daynight_ratio);

	u16 mc = m_mesh->getMeshBufferCount();
	for (u16 j=0; j<mc; j++) {
		scene::IMeshBuffer *buf = m_mesh->getMeshBuffer(j);
		video::S3DVertex *vertices = (video::S3DVertex*)buf->getVertices();
		u16 vc = buf->getVertexCount();
		video::SColor *c = m_meshdata[j].colours[ci].data();
		for (u16 i=0; i<vc; i++) {
			vertices[i].Color = c[i];
		}
	}
}

void MapBlockMesh::updateCameraOffset(v3s16 camera_offset)
{
	if (camera_offset != m_camera_offset) {
		translateMesh(m_mesh, intToFloat(m_camera_offset-camera_offset, BS));
		m_camera_offset = camera_offset;
	}
}
