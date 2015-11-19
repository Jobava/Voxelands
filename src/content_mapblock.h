/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mapblock.h
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

#ifndef CONTENT_MAPBLOCK_HEADER
#define CONTENT_MAPBLOCK_HEADER

#ifndef SERVER

#include "mapblock_mesh.h"
#include "utility.h"

void meshgen_preset_smooth_lights(MeshMakeData *data, v3s16 p);
void meshgen_cubelike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_raillike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_plantlike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_liquid(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_liquid_source(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_nodebox(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected, bool meta);
void meshgen_glasslike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_torchlike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_fencelike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_firelike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_walllike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_rooflike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_leaflike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_wirelike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected, bool is3d);
void meshgen_stairlike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_slablike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_trunklike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_flaglike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_melonlike(MeshMakeData *data, v3s16 p, MapNode &n, SelectedNode &selected);
void meshgen_farnode(MeshMakeData *data, v3s16 p, MapNode &n);

#endif

#endif

