/************************************************************************
* selection_mesh.cpp
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

#include "common_irrlicht.h"
#include "selection_mesh.h"
#include "content_mapblock.h"
#include "settings.h"
#include "main.h" // For g_settings and g_texturesource
#include "mesh.h"
#include "mapblock.h"
#include "client.h"

static std::map<v3s16,SelectedNode> selected;
static std::vector<SelectionMesh*> meshes;

static void selection_clear()
{
	selected.clear();

	/* delete meshes */
	std::vector<SelectionMesh*> omeshes;
	omeshes.swap(meshes);
	for (std::vector<SelectionMesh*>::iterator i = omeshes.begin(); i != omeshes.end(); i++) {
		SelectionMesh *mesh = *i;
		if (mesh)
			delete mesh;
	}
}

static void selection_generate(Client &client)
{
	std::map<v3s16, std::map<v3s16,SelectedNode> > selected_blocks;

	Map &map = client.getEnv().getMap();
	u32 dn_ratio = client.getEnv().getDayNightRatio();

	/* split into mapblocks */
	for (std::map<v3s16,SelectedNode>::iterator i = selected.begin(); i != selected.end(); i++) {
		v3s16 bp = getNodeBlockPos(i->first);
		selected_blocks[bp][i->first] = i->second;
	}

	/* iterate over mapblocks */
	for (std::map<v3s16, std::map<v3s16,SelectedNode> >::iterator i = selected_blocks.begin(); i != selected_blocks.end(); i++) {
		/* create selection meshes */
		v3s16 bp = i->first;
		std::map<v3s16,SelectedNode> bselected = i->second;
		MeshMakeData data;
		MapBlock *block = map.getBlockNoCreateNoEx(bp);
		data.m_selected = bselected;
		data.m_env = &client.getEnv();
		data.fill(dn_ratio,block);
		SelectionMesh *mesh = new SelectionMesh(&data);
		meshes.push_back(mesh);
	}
}

void selection_draw(video::IVideoDriver* driver, Client &client, std::vector<SelectedNode> &select)
{
	if (!select.size()) {
		if (selected.size())
			selection_clear();
		return;
	}

	bool match = true;
	std::map<v3s16,SelectedNode> nselected;

	for (std::vector<SelectedNode>::iterator i = select.begin(); i != select.end(); i++) {
		std::map<v3s16,SelectedNode>::iterator n = selected.find(i->pos);
		if (
			n == selected.end()
			|| n->second.crack != i->crack
			|| n->second.has_crack != i->has_crack
			|| n->second.is_coloured != i->is_coloured
			|| n->second.content != i->content
		)
			match = false;
		nselected[i->pos] = *i;
	}

	if (!match || nselected.size() != selected.size()) {
		selection_clear();
		selected.swap(nselected);
		selection_generate(client);
	}

	if (!meshes.size())
		return;

	bool render_trilinear = g_settings->getBool("trilinear_filter");
	bool render_bilinear = g_settings->getBool("bilinear_filter");
	bool render_anisotropic = g_settings->getBool("anisotropic_filter");

	for (std::vector<SelectionMesh*>::iterator i = meshes.begin(); i != meshes.end(); i++) {
			SelectionMesh *mesh = *i;
		if (!mesh || !mesh->getMesh())
			continue;
		scene::SMesh *m = mesh->getMesh();
		if (!m)
			continue;

		u32 c = m->getMeshBufferCount();
		for (u32 i=0; i<c; i++) {
			scene::IMeshBuffer *buf = m->getMeshBuffer(i);
			if (buf == NULL)
				continue;
			if (buf->getVertexCount() == 0)
				continue;

			buf->getMaterial().setFlag(video::EMF_TRILINEAR_FILTER, render_trilinear);
			buf->getMaterial().setFlag(video::EMF_BILINEAR_FILTER, render_bilinear);
			buf->getMaterial().setFlag(video::EMF_ANISOTROPIC_FILTER, render_anisotropic);

			driver->setMaterial(buf->getMaterial());
			driver->drawMeshBuffer(buf);
		}
	}
}

SelectionMesh::SelectionMesh(MeshMakeData *data):
	m_mesh(NULL)
{
	generate(data);
}

SelectionMesh::~SelectionMesh()
{
	m_mesh->drop();
	m_mesh = NULL;
}

void SelectionMesh::generate(MeshMakeData *data)
{
	DSTACK(__FUNCTION_NAME);

	BEGIN_DEBUG_EXCEPTION_HANDLER

	data->m_BSd = 0.01;
	data->m_BS = (float)BS+data->m_BSd;
	data->m_smooth_lighting = g_settings->getBool("smooth_lighting");
	m_pos = data->m_blockpos;

	for (std::map<v3s16,SelectedNode>::iterator i = data->m_selected.begin(); i != data->m_selected.end(); i++) {
		SelectedNode selected = i->second;
		v3s16 p = selected.pos-data->m_blockpos_nodes;

		MapNode n = data->m_vmanip.getNodeNoEx(selected.pos);

		if (data->m_smooth_lighting && !selected.is_coloured)
			meshgen_preset_smooth_lights(data,p);
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
			meshgen_plantlike(data,p,n,selected);
			break;
		case CDT_LIQUID:
			meshgen_liquid(data,p,n,selected);
			break;
		case CDT_LIQUID_SOURCE:
			meshgen_liquid_source(data,p,n,selected);
			break;
		case CDT_NODEBOX:
		case CDT_NODEBOX_META:
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
		case CDT_FLAGLIKE:
			meshgen_flaglike(data,p,n,selected);
			break;
		case CDT_MELONLIKE:
			meshgen_melonlike(data,p,n,selected);
			break;
		default:;
		}
	}

	scene::SMesh *mesh = new scene::SMesh();
	for (u32 i=0; i<data->m_meshdata.size(); i++) {
		MeshData &d = data->m_meshdata[i];
		// Create meshbuffer
		// This is a "Standard MeshBuffer",
		// it's a typedeffed CMeshBuffer<video::S3DVertex>
		scene::SMeshBuffer *buf = new scene::SMeshBuffer();
		// Set material
		buf->Material = d.material;
		// Add to mesh
		mesh->addMeshBuffer(buf);
		// Mesh grabbed it
		buf->drop();

		buf->append(d.vertices.data(), d.vertices.size(), d.indices.data(), d.indices.size());
	}

	translateMesh(mesh, intToFloat(data->m_blockpos_nodes, BS));

	if (m_mesh)
		m_mesh->drop();
	m_mesh = mesh;
	m_meshdata.swap(data->m_meshdata);
	refresh(data->m_daynight_ratio);
	m_mesh->recalculateBoundingBox();

	END_DEBUG_EXCEPTION_HANDLER(errorstream)
}

void SelectionMesh::refresh(u32 daynight_ratio)
{
	if (m_mesh == NULL)
		return;

	u16 mc = m_mesh->getMeshBufferCount();
	for (u16 j=0; j<mc; j++) {
		scene::IMeshBuffer *buf = m_mesh->getMeshBuffer(j);
		if (buf == 0)
			continue;
		u16 vc = buf->getVertexCount();
		if (!vc)
			continue;
		video::S3DVertex *vertices = (video::S3DVertex*)buf->getVertices();
		if (vertices == 0)
			continue;
		u32 *c = m_meshdata[j].colours.data();
		for (u16 i=0; i<vc; i++) {
			vertices[i].Color = blend_light(c[i],daynight_ratio);
		}
	}
}
