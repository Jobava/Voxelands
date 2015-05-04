/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mapblock.cpp
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

#include "content_mapblock.h"
#include "content_mapnode.h"
#include "main.h" // For g_settings and g_texturesource
#include "mineral.h"
#include "mapblock_mesh.h" // For MapBlock_LightColor()
#include "settings.h"
#include "environment.h"
#include "nodemetadata.h"
#include "sound.h"

#ifndef SERVER
#if 0
/*
 * get the light values for a node
 * smooth lighting gets per-vertex
 * standard lighting gets per-face
 * TODO: smooth lighting currently gets the light for each surrounding node up
 * to eight times, probably a better way to do this
 */
static void getLights(v3s16 pos, video::SColor *lights, MeshMakeData *data, bool smooth_lighting, u8 vertex_alpha)
{
	NodeMod mod;
	data->m_temp_mods.get(pos-(data->m_blockpos*MAP_BLOCKSIZE),&mod);
	if (mod == NODEMOD_SELECTION) {
		video::SColor c = MapBlock_LightColor(vertex_alpha, 255, true);
		for (int i=0; i<14; i++) {
			lights[i] = c;
		}
		return;
	}
	if (!smooth_lighting) {
		u8 l = 0;
		u32 lt = 0;
		u32 ltp;
		u8 ld = 1;
		for (s16 tx=-1; tx<2; tx++) {
		for (s16 ty=-1; ty<2; ty++) {
		for (s16 tz=-1; tz<2; tz++) {
			MapNode tn = data->m_vmanip.getNodeRO(pos + v3s16(tx,ty,tz));
			if (
				ty<1
				&& (
					tn.getContent() != CONTENT_AIR
					&& content_features(tn).light_source == 0
					&& content_features(tn).param_type != CPT_LIGHT
				)
			)
				continue;
			ltp = decode_light(tn.getLightBlend(data->m_daynight_ratio));
			if (!ltp)
				continue;
			lt += ltp;
			ld++;
		}
		}
		}
		if (ld)
			l = lt/ld;
		video::SColor c = MapBlock_LightColor(vertex_alpha, l);
		for (int i=0; i<14; i++) {
			lights[i] = c;
		}
		return;
	}
	static const v3s16 corners[8] = {
		v3s16(-1,1,1),
		v3s16(1,1,1),
		v3s16(1,1,-1),
		v3s16(-1,1,-1),
		v3s16(-1,-1,-1),
		v3s16(1,-1,-1),
		v3s16(1,-1,1),
		v3s16(-1,-1,1)
	};
	u8 o[8];
	u8 inner;
	u8 l;
	for (int i=0; i<8; i++) {
		l = getSmoothLight(pos,corners[i],data->m_vmanip,data->m_daynight_ratio);
		o[i] = l;
		lights[i] = MapBlock_LightColor(vertex_alpha, l);
	}
	{
		MapNode tn = data->m_vmanip.getNodeRO(pos);
		inner = decode_light(tn.getLightBlend(data->m_daynight_ratio));
	}
	int faces[6][4] = {
		{0,1,2,3},
		{4,5,6,7},
		{0,3,4,7},
		{1,2,5,6},
		{0,1,6,7},
		{2,3,4,5}
	};
	/*
		0: up
		1: down
		2: right
		3: left
		4: back
		5: front
	*/
	for (int f=0; f<6; f++) {
		l = inner;
		for (int i=0; i<4; i++) {
			if (o[faces[f][i]] > l)
				l = o[faces[f][i]];
		}
		lights[f+8] = MapBlock_LightColor(vertex_alpha, l);
	}
}
static void getLights(v3s16 pos, video::SColor *lights, MeshMakeData *data, bool smooth_lighting)
{
	getLights(pos,lights,data,smooth_lighting,255);
}
static void getRoofLights(v3s16 pos, video::SColor *lights, MeshMakeData *data, v3s16 dir)
{
	bool selected = false;
	NodeMod mod;
	data->m_temp_mods.get(pos-(data->m_blockpos*MAP_BLOCKSIZE),&mod);
	if (mod == NODEMOD_SELECTION)
		selected = true;
	u8 l = 0;
	u32 lt = 0;
	u32 ltp;
	u8 ld = 0;
	MapNode tn = data->m_vmanip.getNodeRO(pos + v3s16(0,1,0));
	ltp = decode_light(tn.getLightBlend(data->m_daynight_ratio));
	if (ltp < 20 || ltp > 200) {
		for (s16 tx=-1; tx<2; tx++) {
		for (s16 ty=0; ty<2; ty++) {
		for (s16 tz=-1; tz<2; tz++) {
			if ((dir.X && tx != dir.X) || (dir.Z && tz != dir.Z))
				continue;
			tn = data->m_vmanip.getNodeRO(pos + v3s16(tx,ty,tz));
			if (
				ty<1
				&& (
					tn.getContent() != CONTENT_AIR
					&& content_features(tn).light_source == 0
					&& content_features(tn).param_type != CPT_LIGHT
				)
			)
				continue;
			ltp = decode_light(tn.getLightBlend(data->m_daynight_ratio));
			if (!ltp)
				continue;
			lt += ltp;
			ld++;
		}
		}
		}
		l = lt;
		if (ld > 1)
			l = lt/ld;
	}else{
		l = ltp;
	}
	lights[0] = MapBlock_LightColor(255, l, selected);

	tn = data->m_vmanip.getNodeRO(pos + v3s16(0,-1,0));
	ltp = decode_light(tn.getLightBlend(data->m_daynight_ratio));
	l = 0;
	ld = 0;
	lt = 0;
	if (ltp < 20) {
		for (s16 tx=-1; tx<2; tx++) {
		for (s16 ty=-1; ty<1; ty++) {
		for (s16 tz=-1; tz<2; tz++) {
			if ((dir.X && tx == dir.X) || (dir.Z && tz == dir.Z))
				continue;
			tn = data->m_vmanip.getNodeRO(pos + v3s16(tx,ty,tz));
			if (
				ty<1
				&& (
					tn.getContent() != CONTENT_AIR
					&& content_features(tn).light_source == 0
					&& content_features(tn).param_type != CPT_LIGHT
				)
			)
				continue;
			ltp = decode_light(tn.getLightBlend(data->m_daynight_ratio));
			if (!ltp)
				continue;
			lt += ltp;
			ld++;
		}
		}
		}
		l = lt;
		if (ld > 1)
			l = lt/ld;
	}else{
		l = ltp;
	}
	lights[1] = MapBlock_LightColor(255, l, selected);
}
#endif

static void meshgen_fullbright_lights(std::vector<video::SColor> *colours, u8 alpha, u16 count)
{
	video::SColor c(alpha,255,255,255);
	for (u16 i=0; i<count; i++) {
		for (u16 k=0; k<18; k++) {
			colours[k].push_back(c);
		}
	}
}

static void meshgen_custom_lights(std::vector<video::SColor> *colours, u8 alpha, u8 red, u8 green, u8 blue, u16 count)
{
	video::SColor c(alpha,red,green,blue);
	for (u16 i=0; i<count; i++) {
		for (u16 k=0; k<18; k++) {
			colours[k].push_back(c);
		}
	}
}

static void meshgen_selected_lights(std::vector<video::SColor> *colours, u8 alpha, u16 count)
{
	video::SColor c(alpha,128,128,255);
	for (u16 i=0; i<count; i++) {
		for (u16 k=0; k<18; k++) {
			colours[k].push_back(c);
		}
	}
}

/*
 * what this should do:
 * get daynight_ratio from index
 * get all 4 corner vertex light values for the face
 * interpolate to the requested vertex position
 */
static void meshgen_lights_vertex(
	MeshMakeData *data,
	MapNode &n,
	v3s16 p,
	std::vector<video::SColor> *colours,
	u8 alpha,
	v3s16 face,
	u16 daynight_ratio_index,
	video::S3DVertex &vertex
)
{
	meshgen_fullbright_lights(colours,alpha,1);
}

/*
 * what this should do:
 * get daynight_ratio from index
 * return face lighting (see also old roof lighting)
 */
static void meshgen_lights_face(
	MeshMakeData *data,
	MapNode &n,
	v3s16 p,
	std::vector<video::SColor> *colours,
	u8 alpha,
	v3s16 face,
	u16 daynight_ratio_index,
	u16 count,
	video::S3DVertex *vertexes
)
{
	u16 daynight_ratio = daynight_ratio_from_index(daynight_ratio_index);
	MapNode n1 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes+p+face);
	u8 light = decode_light(getFaceLight(daynight_ratio, n, n1, face));
	if ((face.X && face.Y) || (face.X && face.Z) || (face.Y && face.Z)) {
		u32 l = light;
		u16 nc = 1;
		if (face.X) {
			n1 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes+p+v3s16(face.X,0,0));
			l += decode_light(getFaceLight(daynight_ratio, n, n1, face));
			nc++;
		}
		if (face.Y) {
			n1 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes+p+v3s16(0,face.Y,0));
			l += decode_light(getFaceLight(daynight_ratio, n, n1, face));
			nc++;
		}
		if (face.Z) {
			n1 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes+p+v3s16(0,0,face.Z));
			l += decode_light(getFaceLight(daynight_ratio, n, n1, face));
			nc++;
		}
		if (nc > 1)
			l /= nc;
		light = l;
	}
	video::SColor c = MapBlock_LightColor(alpha,light);
	for (u16 i=0; i<count; i++) {
		colours[daynight_ratio_index].push_back(c);
	}
}

/*
 * what this should do:
 * MeshMakeData has a m_smooth_lighting value in it, don't check config for every vertex!
 * for each vertex:
 *	for each daynight_ratio value (18 of them):
 *		call meshgen_lights_vertex
 */
static void meshgen_lights(
	MeshMakeData *data,
	MapNode &n,
	v3s16 p,
	std::vector<video::SColor> *colours,
	u8 alpha,
	v3s16 face,
	u16 count,
	video::S3DVertex *vertexes
)
{
	if (data->m_smooth_lighting) {
		//v3s16 vertex_dirs[4];
		//getNodeVertexDirs(face_dir_corrected, vertex_dirs);
		//for (u16 i=0; i<4; i++) {
			//lights[i] = getSmoothLight(blockpos_nodes + p_corrected, vertex_dirs[i], vmanip, daynight_ratio);
		//}
		for (u16 i=0; i<count; i++) {
			for (u16 k=0; k<18; k++) {
				meshgen_lights_vertex(data,n,p,colours,alpha,face,k,vertexes[i]);
			}
		}
	}else{
		for (u16 k=0; k<18; k++) {
			meshgen_lights_face(data,n,p,colours,alpha,face,k,count,vertexes);
		}
	}
}

/* TODO: there are other cases that should return false */
static bool meshgen_hardface(MeshMakeData *data, v3s16 p, MapNode &n, v3s16 pos)
{
	MapNode nn = data->m_vmanip.getNodeRO(data->m_blockpos_nodes+p+pos);
	if (content_features(nn).draw_type == CDT_CUBELIKE)
		return false;
	return true;
}

static int meshgen_check_walllike(MeshMakeData *data, MapNode n, v3s16 p, u8 d[8])
{
	static const v3s16 fence_dirs[8] = {
		v3s16(1,0,0),
		v3s16(-1,0,0),
		v3s16(0,0,1),
		v3s16(0,0,-1),
		v3s16(1,0,1),
		v3s16(1,0,-1),
		v3s16(-1,0,1),
		v3s16(-1,0,-1)
	};
	static const int showcheck[4][2] = {
		{0,2},
		{0,3},
		{1,2},
		{1,3}
	};
	v3s16 p2;
	MapNode n2;
	content_t c2;
	const ContentFeatures *f2;
	for (s16 i=0; i<8; i++) {
		d[i] = 0;
	}
	for (int k=0; k<8; k++) {
		if (k > 3 && (d[showcheck[k-4][0]] || d[showcheck[k-4][1]]))
					continue;
		p2 = p+fence_dirs[k];
		n2 = data->m_vmanip.getNodeRO(p2);
		c2 = n2.getContent();
		f2 = &content_features(c2);
		if (
			f2->draw_type == CDT_FENCELIKE
			|| f2->draw_type == CDT_WALLLIKE
			|| c2 == CONTENT_WOOD_GATE
			|| c2 == CONTENT_WOOD_GATE_OPEN
			|| c2 == CONTENT_STEEL_GATE
			|| c2 == CONTENT_STEEL_GATE_OPEN
			|| (
				c2 != CONTENT_IGNORE
				&& c2 == content_features(n).special_alternate_node
			)
		) {
			d[k] = 1;
		}
	}
	u8 ps = d[0]+d[1]+d[2]+d[3]+d[4]+d[5]+d[6]+d[7];
	p2 = p;
	p2.Y++;
	n2 = data->m_vmanip.getNodeRO(p2);
	c2 = n2.getContent();
	f2 = &content_features(c2);
	if (
		f2->draw_type != CDT_WALLLIKE
		&& f2->draw_type != CDT_AIRLIKE
	) {
		if (
			f2->draw_type == CDT_TORCHLIKE
			|| f2->draw_type == CDT_FENCELIKE
		)
			return 0;
		return 1;
	}
	if (f2->draw_type == CDT_WALLLIKE) {
		u8 ad[8];
		int ap = meshgen_check_walllike(data,n2,p2,ad);
		if ((ad[0]+ad[1]+ad[2]+ad[3]+ad[4]+ad[5]+ad[6]+ad[7]) == 2) {
			if (ap != 2)
				return 1;
		}else{
			return 1;
		}
	}
	if (ps == 2) {
		if (
			d[4]
			|| d[5]
			|| d[6]
			|| d[7]
			|| (d[0] && d[2])
			|| (d[1] && d[3])
			|| (d[0] && d[3])
			|| (d[1] && d[2])
		) {
			return 0;
		}
	}else{
		return 0;
	}
	return 2;
}

/* TODO: optimise the fuck out of this, make less faces where possible */
static void meshgen_cuboid(
	MeshMakeData *data,
	MapNode &n,
	v3s16 p,
	v3f pos,
	const aabb3f &box,
	TileSpec *tiles,
	int tilecount,
	bool selected,
	const f32* txc,
	v3s16 angle,
	v3f centre,
	u8 *cols=NULL
)
{
	assert(tilecount >= 1 && tilecount <= 6);

	v3f min = box.MinEdge;
	v3f max = box.MaxEdge;

	if (txc == NULL) {
		static const f32 txc_default[24] = {
			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
			0,0,1,1
		};
		txc = txc_default;
	}

	static v3s16 faces[6] = {
		v3s16( 0, 1, 0),
		v3s16( 0,-1, 0),
		v3s16( 1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16( 0, 0, 1),
		v3s16( 0, 0,-1)
	};

	video::S3DVertex vertices[6][4] = {
		{	// up
			video::S3DVertex(min.X,max.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[0],txc[1]),
			video::S3DVertex(max.X,max.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[2],txc[1]),
			video::S3DVertex(max.X,max.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[2],txc[3]),
			video::S3DVertex(min.X,max.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[0],txc[3])
		},{	// down
			video::S3DVertex(min.X,min.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[4],txc[5]),
			video::S3DVertex(max.X,min.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[6],txc[5]),
			video::S3DVertex(max.X,min.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[6],txc[7]),
			video::S3DVertex(min.X,min.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[4],txc[7])
		},{	// right
			video::S3DVertex(max.X,max.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[ 8],txc[9]),
			video::S3DVertex(max.X,max.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[10],txc[9]),
			video::S3DVertex(max.X,min.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[10],txc[11]),
			video::S3DVertex(max.X,min.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[ 8],txc[11])
		},{	// left
			video::S3DVertex(min.X,max.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[12],txc[13]),
			video::S3DVertex(min.X,max.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[14],txc[13]),
			video::S3DVertex(min.X,min.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[14],txc[15]),
			video::S3DVertex(min.X,min.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[12],txc[15])
		},{	// back
			video::S3DVertex(max.X,max.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[16],txc[17]),
			video::S3DVertex(min.X,max.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[18],txc[17]),
			video::S3DVertex(min.X,min.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[18],txc[19]),
			video::S3DVertex(max.X,min.Y,max.Z, 0,0,0, video::SColor(255,255,255,255), txc[16],txc[19])
		},{	// front
			video::S3DVertex(min.X,max.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[20],txc[21]),
			video::S3DVertex(max.X,max.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[22],txc[21]),
			video::S3DVertex(max.X,min.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[22],txc[23]),
			video::S3DVertex(min.X,min.Y,min.Z, 0,0,0, video::SColor(255,255,255,255), txc[20],txc[23])
		}
	};

	u16 indices[] = {0,1,2,2,3,0};
	for (u16 i=0; i<6; i++) {
		int tileindex = MYMIN(i, tilecount-1);
		for (s32 j=0; j<4; j++) {
			vertices[i][j].Pos -= centre;
			if (angle.Y)
				vertices[i][j].Pos.rotateXZBy(angle.Y);
			if (angle.X)
				vertices[i][j].Pos.rotateYZBy(angle.X);
			if (angle.Z)
				vertices[i][j].Pos.rotateXYBy(angle.Z);
			vertices[i][j].Pos += centre;
			vertices[i][j].TCoords *= tiles[tileindex].texture.size;
			vertices[i][j].TCoords += tiles[tileindex].texture.pos;
		}
		std::vector<video::SColor> colours[18];
		if (cols) {
			meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
		}else if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,faces[i],4,vertices[i]);
		}
		for (s32 j=0; j<4; j++) {
			vertices[i][j].Pos += pos;
		}
		data->append(tiles[tileindex].getMaterial(), vertices[i], 4, indices, 6, colours);
	}
}

/* TODO: this can also have the fuck optimised out of it, make less faces where possible */
static void meshgen_build_nodebox(MeshMakeData *data, v3s16 p, MapNode &n, bool selected, std::vector<NodeBox> &boxes, TileSpec *tiles)
{
	v3f pos = intToFloat(p,BS);
	for (std::vector<NodeBox>::iterator i = boxes.begin(); i != boxes.end(); i++) {
		NodeBox box = *i;

		// Compute texture coords
		f32 tx1 = (box.m_box.MinEdge.X/BS)+0.5;
		f32 ty1 = (box.m_box.MinEdge.Y/BS)+0.5;
		f32 tz1 = (box.m_box.MinEdge.Z/BS)+0.5;
		f32 tx2 = (box.m_box.MaxEdge.X/BS)+0.5;
		f32 ty2 = (box.m_box.MaxEdge.Y/BS)+0.5;
		f32 tz2 = (box.m_box.MaxEdge.Z/BS)+0.5;
		f32 txc[24] = {
			// up
			tx1, 1-tz2, tx2, 1-tz1,
			// down
			tx1, tz1, tx2, tz2,
			// right
			tz1, 1-ty2, tz2, 1-ty1,
			// left
			1-tz2, 1-ty2, 1-tz1, 1-ty1,
			// back
			1-tx2, 1-ty2, 1-tx1, 1-ty1,
			// front
			tx1, 1-ty2, tx2, 1-ty1,
		};
		meshgen_cuboid(data,n,p, pos, box.m_box, tiles, 6, selected, txc, box.m_angle, box.m_centre);
	}
}

/* TODO: calculate faces better, or pass faces as argument */
void meshgen_rooftri(MeshMakeData *data, MapNode &n, v3s16 p, v3f corners[3], v3f pos, TileSpec &tile, bool selected, s16 rot, v3s16 face)
{
	// vertices for top and bottom tri
	v3f top_v[3];
	v3f btm_v[3];
	// tex coords for top and bottom tri
	v2f top_t[3];
	v2f btm_t[3];
	// faces for top and bottom tri
	v3s16 upface = face;
	v3s16 downface = face;
	if (downface.X)
		downface.X *= -1;
	if (downface.Y)
		downface.Y *= -1;
	if (downface.Z)
		downface.Z *= -1;
	for (int i=0; i<3; i++) {
		top_v[i].X = (corners[i].X*BS);
		top_v[i].Y = ((corners[i].Y+0.01)*BS);
		top_v[i].Z = (corners[i].Z*BS);
		top_t[i].X = (corners[i].X+0.5);
		top_t[i].Y = (corners[i].Z+0.5);
		if (rot)
			top_t[i] = top_t[i].rotateBy(rot,v2f(0.5,0.5));
		top_t[i].X = (top_t[i].X*tile.texture.size.X)+tile.texture.pos.X;
		top_t[i].Y = (top_t[i].Y*tile.texture.size.Y)+tile.texture.pos.Y;

		// reverse winding for bottom
		btm_v[2-i].X = (corners[i].X*BS);
		btm_v[2-i].Y = ((corners[i].Y-0.01)*BS);
		btm_v[2-i].Z = (corners[i].Z*BS);
		btm_t[2-i].X = top_t[i].X;
		btm_t[2-i].Y = top_t[i].Y;
	}

	{
		video::S3DVertex tri_v[3] = {
			video::S3DVertex(btm_v[0].X, btm_v[0].Y, btm_v[0].Z, 0,0,0, video::SColor(255,255,255,255), btm_t[0].X, btm_t[0].Y),
			video::S3DVertex(btm_v[1].X, btm_v[1].Y, btm_v[1].Z, 0,0,0, video::SColor(255,255,255,255), btm_t[1].X, btm_t[1].Y),
			video::S3DVertex(btm_v[2].X, btm_v[2].Y, btm_v[2].Z, 0,0,0, video::SColor(255,255,255,255), btm_t[2].X, btm_t[2].Y),
		};
		u16 indices[] = {0,1,2};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,3);
		}else{
			meshgen_lights(data,n,p,colours,255,downface,3,tri_v);
		}
		tri_v[0].Pos += pos;
		tri_v[1].Pos += pos;
		tri_v[2].Pos += pos;
		data->append(tile.getMaterial(),tri_v, 3, indices, 3, colours);
	}
	{
		video::S3DVertex tri_v[3] = {
			video::S3DVertex(top_v[0].X, top_v[0].Y, top_v[0].Z, 0,0,0, video::SColor(255,255,255,255), top_t[0].X, top_t[0].Y),
			video::S3DVertex(top_v[1].X, top_v[1].Y, top_v[1].Z, 0,0,0, video::SColor(255,255,255,255), top_t[1].X, top_t[1].Y),
			video::S3DVertex(top_v[2].X, top_v[2].Y, top_v[2].Z, 0,0,0, video::SColor(255,255,255,255), top_t[2].X, top_t[2].Y),
		};
		u16 indices[] = {0,1,2};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,3);
		}else{
			meshgen_lights(data,n,p,colours,255,upface,3,tri_v);
		}
		tri_v[0].Pos += pos;
		tri_v[1].Pos += pos;
		tri_v[2].Pos += pos;
		data->append(tile.getMaterial(),tri_v, 3, indices, 3, colours);
	}
}

/* TODO: calculate faces better, or pass faces as argument */
void meshgen_leaftri(MeshMakeData *data, MapNode &n, v3s16 p, v3f corners[3], v3f pos, TileSpec &tile, bool selected, s16 rot)
{
	// vertices
	v3f v[3];
	// tex coords
	v2f t[3];
	for (int i=0; i<3; i++) {
		v[i].X = (corners[i].X*BS);
		v[i].Y = (corners[i].Y*BS);
		v[i].Z = (corners[i].Z*BS);
		t[i].X = (corners[i].X+0.5);
		t[i].Y = (corners[i].Z+0.5);
		if (rot)
			t[i] = t[i].rotateBy(rot,v2f(0.5,0.5));
		t[i].X = (t[i].X*tile.texture.size.X)+tile.texture.pos.X;
		t[i].Y = (t[i].Y*tile.texture.size.Y)+tile.texture.pos.Y;
	}

	{
		video::S3DVertex tri_v[3] = {
			video::S3DVertex(v[0].X, v[0].Y, v[0].Z, 0,0,0, video::SColor(255,255,255,255),  t[0].X, t[0].Y),
			video::S3DVertex(v[1].X, v[1].Y, v[1].Z, 0,0,0, video::SColor(255,255,255,255),  t[1].X, t[1].Y),
			video::S3DVertex(v[2].X, v[2].Y, v[2].Z, 0,0,0, video::SColor(255,255,255,255),  t[2].X, t[2].Y),
		};
		u16 indices[] = {0,1,2};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,3);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(0,0,0),3,tri_v);
		}
		tri_v[0].Pos += pos;
		tri_v[1].Pos += pos;
		tri_v[2].Pos += pos;
		data->append(tile.getMaterial(),tri_v, 3, indices, 3, colours);
	}
}

void meshgen_cubelike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	v3f pos = intToFloat(p, BS);
	if (meshgen_hardface(data,p,n,v3s16(-1,0,0))) {
		TileSpec tile = getNodeTile(n,p,v3s16(-1,0,0),data->m_temp_mods,NULL);
		video::S3DVertex vertices[4] = {
			video::S3DVertex(-0.5*BS, 0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y0()),
			video::S3DVertex(-0.5*BS, 0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y0()),
			video::S3DVertex(-0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y1()),
			video::S3DVertex(-0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y1())
		};

		u16 indices[6] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(-1,0,0),4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices, 4, indices, 6, colours);
	}
	if (meshgen_hardface(data,p,n,v3s16(1,0,0))) {
		TileSpec tile = getNodeTile(n,p,v3s16(1,0,0),data->m_temp_mods,NULL);
		video::S3DVertex vertices[4] = {
			video::S3DVertex(0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y1()),
			video::S3DVertex(0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y1()),
			video::S3DVertex(0.5*BS, 0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y0()),
			video::S3DVertex(0.5*BS, 0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y0())
		};

		u16 indices[6] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(1,0,0),4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices, 4, indices, 6, colours);
	}
	if (meshgen_hardface(data,p,n,v3s16(0,-1,0))) {
		TileSpec tile = getNodeTile(n,p,v3s16(0,-1,0),data->m_temp_mods,NULL);
		video::S3DVertex vertices[4] = {
			video::S3DVertex( 0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y0()),
			video::S3DVertex(-0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y0()),
			video::S3DVertex(-0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y1()),
			video::S3DVertex( 0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y1())
		};

		u16 indices[6] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(0,-1,0),4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices, 4, indices, 6, colours);
	}
	if (meshgen_hardface(data,p,n,v3s16(0,1,0))) {
		TileSpec tile = getNodeTile(n,p,v3s16(0,1,0),data->m_temp_mods,NULL);
		video::S3DVertex vertices[4] = {
			video::S3DVertex( 0.5*BS, 0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y1()),
			video::S3DVertex(-0.5*BS, 0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y1()),
			video::S3DVertex(-0.5*BS, 0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y0()),
			video::S3DVertex( 0.5*BS, 0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y0())
		};

		u16 indices[6] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(0,1,0),4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices, 4, indices, 6, colours);
	}
	if (meshgen_hardface(data,p,n,v3s16(0,0,-1))) {
		TileSpec tile = getNodeTile(n,p,v3s16(0,0,-1),data->m_temp_mods,NULL);
		video::S3DVertex vertices[4] = {
			video::S3DVertex(-0.5*BS, 0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y0()),
			video::S3DVertex( 0.5*BS, 0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y0()),
			video::S3DVertex( 0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y1()),
			video::S3DVertex(-0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y1())
		};

		u16 indices[6] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(0,0,-1),4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices, 4, indices, 6, colours);
	}
	if (meshgen_hardface(data,p,n,v3s16(0,0,1))) {
		TileSpec tile = getNodeTile(n,p,v3s16(0,0,1),data->m_temp_mods,NULL);
		video::S3DVertex vertices[4] = {
			video::S3DVertex( 0.5*BS, 0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y0()),
			video::S3DVertex(-0.5*BS, 0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y0()),
			video::S3DVertex(-0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y1()),
			video::S3DVertex( 0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y1())
		};

		u16 indices[6] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(0,0,1),4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices, 4, indices, 6, colours);
	}
}

/* TODO: should use custom vertexes instead of boxes for curved rails */
void meshgen_raillike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	bool is_rail_x [] = { false, false };  /* x-1, x+1 */
	bool is_rail_z [] = { false, false };  /* z-1, z+1 */

	bool is_rail_z_minus_y [] = { false, false };  /* z-1, z+1; y-1 */
	bool is_rail_x_minus_y [] = { false, false };  /* x-1, z+1; y-1 */
	bool is_rail_z_plus_y [] = { false, false };  /* z-1, z+1; y+1 */
	bool is_rail_x_plus_y [] = { false, false };  /* x-1, x+1; y+1 */

	MapNode n_minus_x = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1,0,0));
	MapNode n_plus_x = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(1,0,0));
	MapNode n_minus_z = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0,0,-1));
	MapNode n_plus_z = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0,0,1));
	MapNode n_plus_x_plus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(1, 1, 0));
	MapNode n_plus_x_minus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(1, -1, 0));
	MapNode n_minus_x_plus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1, 1, 0));
	MapNode n_minus_x_minus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1, -1, 0));
	MapNode n_plus_z_plus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0, 1, 1));
	MapNode n_minus_z_plus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0, 1, -1));
	MapNode n_plus_z_minus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0, -1, 1));
	MapNode n_minus_z_minus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0, -1, -1));

	content_t thiscontent = n.getContent();

	if (n_minus_x.getContent() == thiscontent)
		is_rail_x[0] = true;
	if (n_minus_x_minus_y.getContent() == thiscontent)
		is_rail_x_minus_y[0] = true;
	if (n_minus_x_plus_y.getContent() == thiscontent)
		is_rail_x_plus_y[0] = true;
	if (n_plus_x.getContent() == thiscontent)
		is_rail_x[1] = true;
	if (n_plus_x_minus_y.getContent() == thiscontent)
		is_rail_x_minus_y[1] = true;
	if (n_plus_x_plus_y.getContent() == thiscontent)
		is_rail_x_plus_y[1] = true;
	if (n_minus_z.getContent() == thiscontent)
		is_rail_z[0] = true;
	if (n_minus_z_minus_y.getContent() == thiscontent)
		is_rail_z_minus_y[0] = true;
	if (n_minus_z_plus_y.getContent() == thiscontent)
		is_rail_z_plus_y[0] = true;
	if (n_plus_z.getContent() == thiscontent)
		is_rail_z[1] = true;
	if (n_plus_z_minus_y.getContent() == thiscontent)
		is_rail_z_minus_y[1] = true;
	if (n_plus_z_plus_y.getContent() == thiscontent)
		is_rail_z_plus_y[1] = true;

	bool is_rail_x_all[] = {false, false};
	bool is_rail_z_all[] = {false, false};
	is_rail_x_all[0] = is_rail_x[0] || is_rail_x_minus_y[0] || is_rail_x_plus_y[0];
	is_rail_x_all[1] = is_rail_x[1] || is_rail_x_minus_y[1] || is_rail_x_plus_y[1];
	is_rail_z_all[0] = is_rail_z[0] || is_rail_z_minus_y[0] || is_rail_z_plus_y[0];
	is_rail_z_all[1] = is_rail_z[1] || is_rail_z_minus_y[1] || is_rail_z_plus_y[1];

	// reasonable default, flat straight unrotated rail
	bool is_straight = true;
	int adjacencies = 0;
	int angle = 0;
	u8 type = 0;

	// check for sloped rail
	if (is_rail_x_plus_y[0] || is_rail_x_plus_y[1] || is_rail_z_plus_y[0] || is_rail_z_plus_y[1]) {
		adjacencies = 5; //5 means sloped
		is_straight = true; // sloped is always straight
	}else{
		// is really straight, rails on both sides
		is_straight = (is_rail_x_all[0] && is_rail_x_all[1]) || (is_rail_z_all[0] && is_rail_z_all[1]);
		adjacencies = is_rail_x_all[0] + is_rail_x_all[1] + is_rail_z_all[0] + is_rail_z_all[1];
	}

	switch (adjacencies) {
	case 1:
		if(is_rail_x_all[0] || is_rail_x_all[1])
			angle = 90;
		break;
	case 2:
		if(!is_straight)
			type = 1; // curved
		if(is_rail_x_all[0] && is_rail_x_all[1])
			angle = 90;
		if(is_rail_z_all[0] && is_rail_z_all[1]){
			if (is_rail_z_plus_y[0])
				angle = 180;
		}
		else if(is_rail_x_all[0] && is_rail_z_all[0])
			angle = 270;
		else if(is_rail_x_all[0] && is_rail_z_all[1])
			angle = 180;
		else if(is_rail_x_all[1] && is_rail_z_all[1])
			angle = 90;
		break;
	case 3:
		// here is where the potential to 'switch' a junction is, but not implemented at present
		type = 2; // t-junction
		if(!is_rail_x_all[1])
			angle=180;
		if(!is_rail_z_all[0])
			angle=90;
		if(!is_rail_z_all[1])
			angle=270;
		break;
	case 4:
		type = 3; // crossing
		break;
	case 5: //sloped
		if(is_rail_z_plus_y[0])
			angle = 180;
		if(is_rail_x_plus_y[0])
			angle = 90;
		if(is_rail_x_plus_y[1])
			angle = 270;
		break;
	default:
		break;
	}

	static const v3s16 tile_dirs[6] = {
		v3s16(0, 1, 0),
		v3s16(0, -1, 0),
		v3s16(1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16(0, 0, 1),
		v3s16(0, 0, -1)
	};
	TileSpec tiles[6];
	TileSpec *tile;
	for (int i = 0; i < 6; i++) {
		// Handles facedir rotation for textures
		tiles[i] = getNodeTile(n,p,tile_dirs[i],data->m_temp_mods);
	}
	v3f pos = intToFloat(p,BS);

	switch (type) {
	case 0: // straight
	{
		if (is_rail_x_plus_y[0] || is_rail_x_plus_y[1] || is_rail_z_plus_y[0] || is_rail_z_plus_y[1]) {
			aabb3f track[5] = {
				aabb3f(-0.375*BS,-0.03125*BS,-0.4375*BS,0.375*BS,0.03125*BS,-0.25*BS),
				aabb3f(-0.375*BS,-0.03125*BS,0.0625*BS,0.375*BS,0.03125*BS,0.25*BS),
				aabb3f(-0.375*BS,-0.03125*BS,0.5*BS,0.375*BS,0.03125*BS,0.6875*BS),
				aabb3f(0.1875*BS,0.03125*BS,-0.64*BS,0.25*BS,0.09375*BS,0.77*BS),
				aabb3f(-0.25*BS,0.03125*BS,-0.64*BS,-0.1875*BS,0.09375*BS,0.77*BS)
			};
			tile = &tiles[0];
			v3s16 an(0,angle,0);
			switch (angle) {
			case 90:
				an.Z = -45;
				break;
			case 180:
				an.X = 45;
				break;
			case 270:
				an.Z = 45;
				break;
			default:
				an.X = -45;
			}
			for (int bi=0; bi<5; bi++) {
				if (bi == 3)
					tile = &tiles[1];
				f32 tx1 = (track[bi].MinEdge.X/BS)+0.5;
				f32 ty1 = (track[bi].MinEdge.Y/BS)+0.5;
				f32 tz1 = (track[bi].MinEdge.Z/BS)+0.5;
				f32 tx2 = (track[bi].MaxEdge.X/BS)+0.5;
				f32 ty2 = (track[bi].MaxEdge.Y/BS)+0.5;
				f32 tz2 = (track[bi].MaxEdge.Z/BS)+0.5;
				f32 txc[24] = {
					// up
					tx1, 1-tz2, tx2, 1-tz1,
					// down
					tx1, tz1, tx2, tz2,
					// right
					tz1, 1-ty2, tz2, 1-ty1,
					// left
					1-tz2, 1-ty2, 1-tz1, 1-ty1,
					// back
					1-tx2, 1-ty2, 1-tx1, 1-ty1,
					// front
					tx1, 1-ty2, tx2, 1-ty1,
				};
				meshgen_cuboid(data,n,p,pos,track[bi],tile,1, selected,txc,an, v3f(0,0,0));
			}
		}else{
			aabb3f track[4] = {
				aabb3f(-0.375*BS,-0.5*BS,-0.375*BS,0.375*BS,-0.4375*BS,-0.1875*BS),
				aabb3f(-0.375*BS,-0.5*BS,0.1875*BS,0.375*BS,-0.4375*BS,0.375*BS),
				aabb3f(-0.25*BS,-0.4375*BS,-0.5*BS,-0.1875*BS,-0.375*BS,0.5*BS),
				aabb3f(0.1875*BS,-0.4375*BS,-0.5*BS,0.25*BS,-0.375*BS,0.5*BS)
			};
			tile = &tiles[0];
			for (int bi=0; bi<4; bi++) {
				if (bi == 2)
					tile = &tiles[1];
				f32 tx1 = (track[bi].MinEdge.X/BS)+0.5;
				f32 ty1 = (track[bi].MinEdge.Y/BS)+0.5;
				f32 tz1 = (track[bi].MinEdge.Z/BS)+0.5;
				f32 tx2 = (track[bi].MaxEdge.X/BS)+0.5;
				f32 ty2 = (track[bi].MaxEdge.Y/BS)+0.5;
				f32 tz2 = (track[bi].MaxEdge.Z/BS)+0.5;
				f32 txc[24] = {
					// up
					tx1, 1-tz2, tx2, 1-tz1,
					// down
					tx1, tz1, tx2, tz2,
					// right
					tz1, 1-ty2, tz2, 1-ty1,
					// left
					1-tz2, 1-ty2, 1-tz1, 1-ty1,
					// back
					1-tx2, 1-ty2, 1-tx1, 1-ty1,
					// front
					tx1, 1-ty2, tx2, 1-ty1,
				};
				meshgen_cuboid(data,n,p,pos,track[bi],tile,1, selected,txc,v3s16(0,angle,0), v3f(0,0,0));
			}
		}
		break;
	}
	case 1: // curved
	{
		aabb3f track[10] = {
			aabb3f(-0.4375*BS,-0.5*BS,-0.375*BS,0.3125*BS,-0.4375*BS,-0.1875*BS),
			aabb3f(-0.375*BS,-0.499*BS,-0.4375*BS,-0.1875*BS,-0.4385*BS,0.3125*BS),

			aabb3f(0.1875*BS,-0.4375*BS,-0.5*BS,0.25*BS,-0.375*BS,-0.0625*BS),
			aabb3f(0.1575*BS,-0.4365*BS,-0.125*BS,0.22*BS,-0.374*BS,0.0),
			aabb3f(0.15*BS,-0.4375*BS,-0.0625*BS,0.2135*BS,-0.375*BS,0.0625*BS),
			aabb3f(-0.125*BS,-0.4365*BS,0.1575*BS,0.0,-0.374*BS,0.22*BS),
			aabb3f(-0.5*BS,-0.4375*BS,0.1875*BS,-0.0625*BS,-0.375*BS,0.25*BS),

			aabb3f(-0.25*BS,-0.4375*BS,-0.5*BS,-0.1875*BS,-0.375*BS,-0.3125*BS),
			aabb3f(0.3425*BS,-0.4385*BS,-0.0625*BS,0.415*BS,-0.374*BS,0.0625*BS),
			aabb3f(-0.5*BS,-0.4375*BS,-0.25*BS,-0.3125*BS,-0.375*BS,-0.1875*BS)
		};
		tile = &tiles[0];
		s16 a[10] = {110,70, 90,110,135,70,90, 90,-45,90};
		for (int bi=0; bi<10; bi++) {
			if (bi == 2)
				tile = &tiles[1];
			f32 tx1 = (track[bi].MinEdge.X/BS)+0.5;
			f32 ty1 = (track[bi].MinEdge.Y/BS)+0.5;
			f32 tz1 = (track[bi].MinEdge.Z/BS)+0.5;
			f32 tx2 = (track[bi].MaxEdge.X/BS)+0.5;
			f32 ty2 = (track[bi].MaxEdge.Y/BS)+0.5;
			f32 tz2 = (track[bi].MaxEdge.Z/BS)+0.5;
			f32 txc[24] = {
				// up
				tx1, 1-tz2, tx2, 1-tz1,
				// down
				tx1, tz1, tx2, tz2,
				// right
				tz1, 1-ty2, tz2, 1-ty1,
				// left
				1-tz2, 1-ty2, 1-tz1, 1-ty1,
				// back
				1-tx2, 1-ty2, 1-tx1, 1-ty1,
				// front
				tx1, 1-ty2, tx2, 1-ty1,
			};
			meshgen_cuboid(data,n,p,pos,track[bi],tile,1, selected,txc,v3s16(0,angle+a[bi],0),v3f(0,0,0));
		}
		break;
	}
	case 2: // t-junction
	{
		aabb3f track[13] = {
			aabb3f(-0.375*BS,-0.5*BS,-0.375*BS,0.375*BS,-0.4375*BS,-0.1875*BS),
			aabb3f(-0.375*BS,-0.5*BS,0.1875*BS,0.375*BS,-0.4375*BS,0.375*BS),
			aabb3f(-0.4375*BS,-0.499*BS,-0.375*BS,0.3125*BS,-0.4385*BS,-0.1875*BS),

			aabb3f(-0.25*BS,-0.4375*BS,-0.5*BS,-0.1875*BS,-0.375*BS,0.5*BS),

			aabb3f(0.1875*BS,-0.4375*BS,-0.5*BS,0.25*BS,-0.375*BS,0.125*BS),
			aabb3f(0.1875*BS,-0.4375*BS,0.25*BS,0.25*BS,-0.375*BS,0.5*BS),

			aabb3f(-0.25*BS,-0.4375*BS,-0.5*BS,-0.1875*BS,-0.375*BS,-0.3125*BS),
			aabb3f(0.3425*BS,-0.4385*BS,-0.0625*BS,0.415*BS,-0.374*BS,0.0625*BS),

			aabb3f(0.1875*BS,-0.4375*BS,-0.5*BS,0.25*BS,-0.375*BS,-0.1875*BS),
			aabb3f(0.1875*BS,-0.4375*BS,-0.125*BS,0.25*BS,-0.375*BS,-0.0625*BS),
			aabb3f(0.1575*BS,-0.4365*BS,-0.125*BS,0.22*BS,-0.374*BS,0.0),
			aabb3f(0.15*BS,-0.4375*BS,-0.0625*BS,0.2135*BS,-0.375*BS,0.0625*BS),
			aabb3f(-0.125*BS,-0.4365*BS,0.1575*BS,0.0,-0.374*BS,0.22*BS)
		};
		tile = &tiles[0];
		s16 a[13] = {0,0,110, 0, 0,0, 90,-45, 90,90,110,135,70};
		for (int bi=0; bi<13; bi++) {
			if (bi == 3)
				tile = &tiles[1];
			f32 tx1 = (track[bi].MinEdge.X/BS)+0.5;
			f32 ty1 = (track[bi].MinEdge.Y/BS)+0.5;
			f32 tz1 = (track[bi].MinEdge.Z/BS)+0.5;
			f32 tx2 = (track[bi].MaxEdge.X/BS)+0.5;
			f32 ty2 = (track[bi].MaxEdge.Y/BS)+0.5;
			f32 tz2 = (track[bi].MaxEdge.Z/BS)+0.5;
			f32 txc[24] = {
				// up
				tx1, 1-tz2, tx2, 1-tz1,
				// down
				tx1, tz1, tx2, tz2,
				// right
				tz1, 1-ty2, tz2, 1-ty1,
				// left
				1-tz2, 1-ty2, 1-tz1, 1-ty1,
				// back
				1-tx2, 1-ty2, 1-tx1, 1-ty1,
				// front
				tx1, 1-ty2, tx2, 1-ty1,
			};
			meshgen_cuboid(data,n,p,pos,track[bi],tile,1, selected,txc,v3s16(0,angle+a[bi],0),v3f(0,0,0));
		}
		break;
	}
	case 3: // crossing
	{
		aabb3f track[20] = {
			aabb3f(-0.375*BS,-0.5*BS,-0.3125*BS,0.375*BS,-0.4375*BS,-0.125*BS),
			aabb3f(-0.375*BS,-0.5*BS,0.125*BS,0.375*BS,-0.4375*BS,0.3125*BS),
			aabb3f(-0.3125*BS,-0.499*BS,-0.375*BS,-0.125*BS,-0.4385*BS,0.375*BS),
			aabb3f( 0.125*BS,-0.499*BS,-0.375*BS,0.3125*BS,-0.4385*BS,0.375*BS),

			aabb3f(-0.25*BS,-0.4375*BS,-0.5*BS,-0.1875*BS,-0.375*BS,-0.1875*BS),
			aabb3f(-0.25*BS,-0.4375*BS,0.1875*BS,-0.1875*BS,-0.375*BS,0.5*BS),
			aabb3f(-0.25*BS,-0.4375*BS,-0.125*BS,-0.1875*BS,-0.375*BS,0.125*BS),

			aabb3f(0.1875*BS,-0.4375*BS,-0.5*BS,0.25*BS,-0.375*BS,-0.1875*BS),
			aabb3f(0.1875*BS,-0.4375*BS,0.1875*BS,0.25*BS,-0.375*BS,0.5*BS),
			aabb3f(0.1875*BS,-0.4375*BS,-0.125*BS,0.25*BS,-0.375*BS,0.125*BS),

			aabb3f(-0.5*BS,-0.4375*BS,0.1875*BS,-0.25*BS,-0.375*BS,0.25*BS),
			aabb3f(0.25*BS,-0.4375*BS,0.1875*BS,0.5*BS,-0.375*BS,0.25*BS),
			aabb3f(-0.125*BS,-0.4375*BS,0.1875*BS,0.125*BS,-0.375*BS,0.25*BS),

			aabb3f(-0.5*BS,-0.4375*BS,-0.25*BS,-0.25*BS,-0.375*BS,-0.1875*BS),
			aabb3f(0.25*BS,-0.4375*BS,-0.25*BS,0.5*BS,-0.375*BS,-0.1875*BS),
			aabb3f(-0.125*BS,-0.4375*BS,-0.25*BS,0.125*BS,-0.375*BS,-0.1875*BS)
		};
		tile = &tiles[0];
		for (int bi=0; bi<16; bi++) {
			if (bi == 4)
				tile = &tiles[1];
			f32 tx1 = (track[bi].MinEdge.X/BS)+0.5;
			f32 ty1 = (track[bi].MinEdge.Y/BS)+0.5;
			f32 tz1 = (track[bi].MinEdge.Z/BS)+0.5;
			f32 tx2 = (track[bi].MaxEdge.X/BS)+0.5;
			f32 ty2 = (track[bi].MaxEdge.Y/BS)+0.5;
			f32 tz2 = (track[bi].MaxEdge.Z/BS)+0.5;
			f32 txc[24] = {
				// up
				tx1, 1-tz2, tx2, 1-tz1,
				// down
				tx1, tz1, tx2, tz2,
				// right
				tz1, 1-ty2, tz2, 1-ty1,
				// left
				1-tz2, 1-ty2, 1-tz1, 1-ty1,
				// back
				1-tx2, 1-ty2, 1-tx1, 1-ty1,
				// front
				tx1, 1-ty2, tx2, 1-ty1,
			};
			meshgen_cuboid(data,n,p,pos,track[bi],tile,1, selected,txc,v3s16(0,angle,0),v3f(0,0,0));
		}
		break;
	}
	default:;
	}
}

void meshgen_plantlike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	ContentFeatures *f = &content_features(n);
	TileSpec tile = f->tiles[0];
	v3f offset(0,0,0);
	if (data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0,-1,0)).getContent() == CONTENT_FLOWER_POT)
		offset = v3f(0,-0.25*BS,0);

	f32 v = tile.texture.y0();
	f32 h = 0.5;
	bool is_scaled = false;
	v3f scale(1.0,1.0,1.0);
	switch (f->draw_type) {
	case CDT_PLANTLIKE_SML:
		is_scaled = true;
		scale = v3f(0.8,0.8,0.8);
		break;
	case CDT_PLANTLIKE_LGE:
	{
		is_scaled = true;
		scale = v3f(1.3,1.0,1.0);
		MapNode n2 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0,1,0));
		h = 1.0;
		if (
			content_features(n2).draw_type == CDT_PLANTLIKE_LGE
			|| content_features(n2).draw_type == CDT_PLANTLIKE
			|| content_features(n2).draw_type == CDT_PLANTLIKE_SML
		) {
			v = (0.333*tile.texture.size.Y)+tile.texture.y0();
			h = 0.5;
		}
		n2 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0,-1,0));
		if (n2.getContent() == CONTENT_FLOWER_POT) {
			offset = v3f(0,-0.25*BS,0);
			if (h == 0.5) {
				v = (0.25*tile.texture.size.Y)+tile.texture.y0();
				h = 0.75;
			}
		}
	}
		break;
	case CDT_PLANTGROWTH_1:
		v = (0.75*tile.texture.size.Y)+tile.texture.y0();
		h = -0.25;
		break;
	case CDT_PLANTGROWTH_2:
		v = (0.5*tile.texture.size.Y)+tile.texture.y0();
		h = 0.0;
		break;
	case CDT_PLANTGROWTH_3:
		v = (0.25*tile.texture.size.Y)+tile.texture.y0();
		h = 0.25;
		break;
	default:;
	}

	v3f pos = offset+intToFloat(p,BS);

	for (u32 j=0; j<2; j++) {
		video::S3DVertex vertices[4] = {
			video::S3DVertex(-0.5*BS,-0.5*BS,0., 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y1()),
			video::S3DVertex( 0.5*BS,-0.5*BS,0., 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y1()),
			video::S3DVertex( 0.5*BS,   h*BS,0., 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), v),
			video::S3DVertex(-0.5*BS,   h*BS,0., 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), v)
		};

		s16 angle = 45;
		if (j == 1)
			angle = -45;

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos.rotateXZBy(angle);
			if (is_scaled)
				vertices[i].Pos *= scale;
		}

		u16 indices[] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(0,0,0),4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices, 4, indices, 6, colours);
	}
}

void meshgen_liquid(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	ContentFeatures *f = &content_features(n);
	TileSpec *tiles = f->tiles;
	bool top_is_same_liquid = false;
	MapNode ntop = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0,1,0));
	if (ntop.getContent() == f->liquid_alternative_flowing || ntop.getContent() == f->liquid_alternative_source)
		top_is_same_liquid = true;

	float node_liquid_level = 0.875;

	// Neighbor liquid levels (key = relative position)
	// Includes current node
	core::map<v3s16, f32> neighbor_levels;
	core::map<v3s16, content_t> neighbor_contents;
	core::map<v3s16, u8> neighbor_flags;
	const u8 neighborflag_top_is_same_liquid = 0x01;
	v3s16 neighbor_dirs[9] = {
		v3s16(0,0,0),
		v3s16(0,0,1),
		v3s16(0,0,-1),
		v3s16(1,0,0),
		v3s16(-1,0,0),
		v3s16(1,0,1),
		v3s16(-1,0,-1),
		v3s16(1,0,-1),
		v3s16(-1,0,1),
	};
	for (u32 i=0; i<9; i++) {
		content_t content = CONTENT_AIR;
		float level = -0.5 * BS;
		u8 flags = 0;
		// Check neighbor
		v3s16 p2 = p + neighbor_dirs[i];
		MapNode n2 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p2);
		if (n2.getContent() != CONTENT_IGNORE) {
			content = n2.getContent();

			if (n2.getContent() == f->liquid_alternative_source) {
				p2.Y += 1;
				n2 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p2);
				if (content_features(n2).liquid_type == LIQUID_NONE) {
					level = 0.5*BS;
				}else{
					level = (-0.5+node_liquid_level) * BS;
				}
				p2.Y -= 1;
			}else if (n2.getContent() == f->liquid_alternative_flowing) {
				level = (-0.5 + ((float)(n2.param2&LIQUID_LEVEL_MASK)
						+ 0.5) / 8.0 * node_liquid_level) * BS;
			}

			// Check node above neighbor.
			// NOTE: This doesn't get executed if neighbor
			//       doesn't exist
			p2.Y += 1;
			n2 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p2);
			if (
				n2.getContent() == f->liquid_alternative_source
				|| n2.getContent() == f->liquid_alternative_flowing
			)
				flags |= neighborflag_top_is_same_liquid;
		}

		neighbor_levels.insert(neighbor_dirs[i], level);
		neighbor_contents.insert(neighbor_dirs[i], content);
		neighbor_flags.insert(neighbor_dirs[i], flags);
	}

	// Corner heights (average between four liquids)
	f32 corner_levels[4];

	v3s16 halfdirs[4] = {
		v3s16(0,0,0),
		v3s16(1,0,0),
		v3s16(1,0,1),
		v3s16(0,0,1),
	};
	for (u32 i=0; i<4; i++) {
		v3s16 cornerdir = halfdirs[i];
		float cornerlevel = 0;
		u32 valid_count = 0;
		u32 air_count = 0;
		for (u32 j=0; j<4; j++) {
			v3s16 neighbordir = cornerdir - halfdirs[j];
			content_t content = neighbor_contents[neighbordir];
			// If top is liquid, draw starting from top of node
			if ((neighbor_flags[neighbordir]&neighborflag_top_is_same_liquid) != 0) {
				cornerlevel = 0.5*BS;
				valid_count = 1;
				break;
			// Source is always the same height
			}else if (content == f->liquid_alternative_source) {
				cornerlevel = (-0.5+node_liquid_level)*BS;
				valid_count = 1;
				break;
			// Flowing liquid has level information
			}else if (content == f->liquid_alternative_flowing) {
				cornerlevel += neighbor_levels[neighbordir];
				valid_count++;
			}else if (content == CONTENT_AIR) {
				air_count++;
			}
		}
		if (air_count >= 2) {
			cornerlevel = -0.5*BS;
		}else if (valid_count > 0) {
			cornerlevel /= valid_count;
		}
		corner_levels[i] = cornerlevel;
	}

	/*
		Generate sides
	*/

	v3s16 side_dirs[4] = {
		v3s16(1,0,0),
		v3s16(-1,0,0),
		v3s16(0,0,1),
		v3s16(0,0,-1),
	};
	s16 side_corners[4][2] = {
		{1, 2},
		{3, 0},
		{2, 3},
		{0, 1},
	};
	v3f pos = intToFloat(p,BS);
	for (u32 i=0; i<4; i++) {
		v3s16 dir = side_dirs[i];

		/*
			If our topside is liquid and neighbor's topside
			is liquid, don't draw side face
		*/
		if (top_is_same_liquid && (neighbor_flags[dir]&neighborflag_top_is_same_liquid) != 0)
			continue;

		content_t neighbor_content = neighbor_contents[dir];
		ContentFeatures &n_feat = content_features(neighbor_content);

		// Don't draw face if neighbor is blocking the view
		if (n_feat.solidness == 2)
			continue;

		bool neighbor_is_same_liquid = false;
		if (
			neighbor_content == f->liquid_alternative_source
			|| neighbor_content == f->liquid_alternative_flowing
		)
			neighbor_is_same_liquid = true;

		// Don't draw any faces if neighbor same is liquid and top is
		// same liquid
		if (neighbor_is_same_liquid == true && top_is_same_liquid == false)
			continue;

		video::S3DVertex vertices[4] = {
			video::S3DVertex(-0.5*BS,0,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[i].texture.x0(), tiles[i].texture.y1()),
			video::S3DVertex( 0.5*BS,0,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[i].texture.x1(), tiles[i].texture.y1()),
			video::S3DVertex( 0.5*BS,0,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[i].texture.x1(), tiles[i].texture.y0()),
			video::S3DVertex(-0.5*BS,0,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[i].texture.x0(), tiles[i].texture.y0()),
		};

		// If our topside is liquid, set upper border of face at upper border of node
		if (top_is_same_liquid) {
			vertices[2].Pos.Y = 0.5*BS;
			vertices[3].Pos.Y = 0.5*BS;
		// Otherwise upper position of face is corner levels
		}else{
			vertices[2].Pos.Y = corner_levels[side_corners[i][0]];
			vertices[3].Pos.Y = corner_levels[side_corners[i][1]];
		}

		// If neighbor is liquid, lower border of face is corner liquid levels
		if (neighbor_is_same_liquid) {
			vertices[0].Pos.Y = corner_levels[side_corners[i][1]];
			vertices[1].Pos.Y = corner_levels[side_corners[i][0]];
		// If neighbor is not liquid, lower border of face is lower border of node
		}else{
			vertices[0].Pos.Y = -0.5*BS;
			vertices[1].Pos.Y = -0.5*BS;
		}

		s16 angle = 0;
		switch (i) {
		case 0:
			angle = -90;
			break;
		case 1:
			angle = 90;
			break;
		case 3:
			angle = 180;
			break;
		default:;
		}

		for (s32 j=0; j<4; j++) {
			if (angle)
				vertices[j].Pos.rotateXZBy(angle);
		}

		u16 indices[] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,f->vertex_alpha,4);
		}else{
			meshgen_lights(data,n,p,colours,f->vertex_alpha,dir,4,vertices);
		}

		for (s32 j=0; j<4; j++) {
			vertices[j].Pos += pos;
		}

		data->append(tiles[i].getMaterial(), vertices, 4, indices, 6, colours);
	}

	/*
		Generate top side, if appropriate
	*/

	if (top_is_same_liquid == false) {
		video::S3DVertex vertices[4] = {
			video::S3DVertex(-0.5*BS,0, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
			video::S3DVertex( 0.5*BS,0, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
			video::S3DVertex( 0.5*BS,0,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
			video::S3DVertex(-0.5*BS,0,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0())
		};

		// This fixes a strange bug
		s32 corner_resolve[4] = {3,2,1,0};

		for (s32 i=0; i<4; i++) {
			s32 j = corner_resolve[i];
			vertices[i].Pos.Y += corner_levels[j];
		}

		u16 indices[] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,f->vertex_alpha,4);
		}else{
			meshgen_lights(data,n,p,colours,f->vertex_alpha,v3s16(0,1,0),4,vertices);
		}

		for (s32 j=0; j<4; j++) {
			vertices[j].Pos += pos;
		}

		data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
	}
}

void meshgen_liquid_source(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	ContentFeatures *f = &content_features(n);
	TileSpec *tiles = f->tiles;
	//    x,y       -,-  +,-  +,+  -,+
	bool drop[4] = {true,true,true,true};
	v3s16 n2p = data->m_blockpos_nodes + p + v3s16(0,1,0);
	MapNode n2 = data->m_vmanip.getNodeRO(n2p);
	ContentFeatures *f2 = &content_features(n2);
	if (f2->liquid_type != LIQUID_NONE) {
		drop[0] = false;
		drop[1] = false;
		drop[2] = false;
		drop[3] = false;
	}else{
		v3s16 dirs[8] = {
			v3s16(-1,1,-1),
			v3s16(0,1,-1),
			v3s16(1,1,-1),
			v3s16(1,1,0),
			v3s16(1,1,1),
			v3s16(0,1,1),
			v3s16(-1,1,1),
			v3s16(-1,1,0),
		};
		for (u32 i=0; i<8; i++) {
			n2p = data->m_blockpos_nodes + p + dirs[i];
			n2 = data->m_vmanip.getNodeRO(n2p);
			f2 = &content_features(n2);
			if (f2->liquid_type == LIQUID_NONE)
				continue;
			switch (i) {
			case 0:
				drop[0] = false;
				break;
			case 1:
				drop[0] = false;
				drop[1] = false;
				break;
			case 2:
				drop[1] = false;
				break;
			case 3:
				drop[1] = false;
				drop[2] = false;
				break;
			case 4:
				drop[2] = false;
				break;
			case 5:
				drop[2] = false;
				drop[3] = false;
				break;
			case 6:
				drop[3] = false;
				break;
			case 7:
				drop[3] = false;
				drop[0] = false;
				break;
			default:;
			}
		}
	}

	v3f pos = intToFloat(p,BS);

	for (u32 j=0; j<6; j++) {
		// Check this neighbor
		n2p = data->m_blockpos_nodes + p + g_6dirs[j];
		n2 = data->m_vmanip.getNodeRO(n2p);
		f2 = &content_features(n2);
		if (f2->liquid_type != LIQUID_NONE) {
			if (n2.getContent() == f->liquid_alternative_flowing)
				continue;
			if (n2.getContent() == f->liquid_alternative_source)
				continue;
		}else if (f2->draw_type == CDT_CUBELIKE) {
			if (g_6dirs[j].Y != 1)
				continue;
			if (!drop[0] && !drop[1] && !drop[2] && !drop[3])
				continue;
		}else if (n2.getContent() == CONTENT_IGNORE) {
			continue;
		}

		// The face at Z+
		video::S3DVertex vertices[4] = {
			video::S3DVertex(-0.5*BS,-0.5*BS,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[j].texture.x0(), tiles[j].texture.y1()),
			video::S3DVertex( 0.5*BS,-0.5*BS,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[j].texture.x1(), tiles[j].texture.y1()),
			video::S3DVertex( 0.5*BS, 0.5*BS,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[j].texture.x1(), tiles[j].texture.y0()),
			video::S3DVertex(-0.5*BS, 0.5*BS,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[j].texture.x0(), tiles[j].texture.y0()),
		};
		switch (j) {
		case 0: // Z+
			if (drop[0])
				vertices[2].Pos.Y = 0.375*BS;
			if (drop[1])
				vertices[3].Pos.Y = 0.375*BS;
			break;
		case 1: // Y+
			if (!drop[1] && !drop[2] && !drop[3] && drop[0]) {
				for(u16 i=0; i<4; i++) {
					vertices[i].Pos.rotateXYBy(90);
				}
				vertices[2].Pos.Z = 0.375*BS;
			}else if (!drop[0] && !drop[1] && !drop[3] && drop[2]) {
				for(u16 i=0; i<4; i++) {
					vertices[i].Pos.rotateXYBy(90);
				}
				vertices[0].Pos.Z = 0.375*BS;
			}else{
				if (drop[0])
					vertices[3].Pos.Z = 0.375*BS;
				if (drop[1])
					vertices[2].Pos.Z = 0.375*BS;
				if (drop[2])
					vertices[1].Pos.Z = 0.375*BS;
				if (drop[3])
					vertices[0].Pos.Z = 0.375*BS;
			}
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateYZBy(-90);
			}
			break;
		case 2: // X+
			if (drop[1])
				vertices[2].Pos.Y = 0.375*BS;
			if (drop[2])
				vertices[3].Pos.Y = 0.375*BS;
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateXZBy(-90);
			}
			break;
		case 3: // Z-
			if (drop[2])
				vertices[2].Pos.Y = 0.375*BS;
			if (drop[3])
				vertices[3].Pos.Y = 0.375*BS;
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateXZBy(180);
			}
			break;
		case 4: // Y-
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateYZBy(90);
			}
			break;
		case 5: // X-
			if (drop[3])
				vertices[2].Pos.Y = 0.375*BS;
			if (drop[0])
				vertices[3].Pos.Y = 0.375*BS;
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateXZBy(90);
			}
			break;
		default:;
		}

		u16 indices[] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,f->vertex_alpha,4);
		}else{
			meshgen_lights(data,n,p,colours,f->vertex_alpha,g_6dirs[j],4,vertices);
		}

		for(u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}
		data->append(tiles[j].getMaterial(), vertices, 4, indices, 6, colours);
	}
}

void meshgen_nodebox(MeshMakeData *data, v3s16 p, MapNode &n, bool selected, bool has_meta)
{
	static const v3s16 tile_dirs[6] = {
		v3s16(0, 1, 0),
		v3s16(0, -1, 0),
		v3s16(1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16(0, 0, 1),
		v3s16(0, 0, -1)
	};

	TileSpec tiles[6];
	NodeMetadata *meta = data->m_env->getMap().getNodeMetadata(p+data->m_blockpos_nodes);
	for (int i = 0; i < 6; i++) {
		// Handles facedir rotation for textures
		tiles[i] = getNodeTile(n,p,tile_dirs[i],data->m_temp_mods,meta);
	}

	std::vector<NodeBox> boxes = content_features(n).getNodeBoxes(n);
	meshgen_build_nodebox(data,p,n,selected,boxes,tiles);
	if (!meta || !has_meta)
		return;

	boxes = meta->getNodeBoxes(n);
	if (boxes.size() > 0) {
		for (int i = 0; i < 6; i++) {
			// Handles facedir rotation for textures
			tiles[i] = getMetaTile(n,p,tile_dirs[i],data->m_temp_mods);
		}
		meshgen_build_nodebox(data,p,n,selected,boxes,tiles);
	}
}

void meshgen_glasslike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	static const v3s16 tile_dirs[6] = {
		v3s16(0, 1, 0),
		v3s16(0, -1, 0),
		v3s16(1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16(0, 0, 1),
		v3s16(0, 0, -1)
	};

	TileSpec tiles[6];
	for (int i = 0; i < 6; i++) {
		// Handles facedir rotation for textures
		tiles[i] = getNodeTile(n,p,tile_dirs[i],data->m_temp_mods);
	}

	v3f pos = intToFloat(p,BS);

	for (u32 j=0; j<6; j++) {
		// Check this neighbor
		v3s16 n2p = data->m_blockpos_nodes + p + g_6dirs[j];
		MapNode n2 = data->m_vmanip.getNodeRO(n2p);
		// Don't make face if neighbor is of same type
		if (n2.getContent() == n.getContent())
			continue;

		// The face at Z+
		video::S3DVertex vertices[4] = {
			video::S3DVertex(-0.5*BS,-0.5*BS,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[j].texture.x0(), tiles[j].texture.y1()),
			video::S3DVertex( 0.5*BS,-0.5*BS,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[j].texture.x1(), tiles[j].texture.y1()),
			video::S3DVertex( 0.5*BS, 0.5*BS,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[j].texture.x1(), tiles[j].texture.y0()),
			video::S3DVertex(-0.5*BS, 0.5*BS,0.5*BS, 0,0,0, video::SColor(255,255,255,255), tiles[j].texture.x0(), tiles[j].texture.y0()),
		};

		s16 yrot = 0;
		s16 xrot = 0;

		// Rotations in the g_6dirs format
		switch (j) {
		case 1: // Y+
			xrot = -90;
			break;
		case 2: // X+
			yrot = -90;
			break;
		case 3: // Z-
			yrot = 180;
			break;
		case 4: // Y-
			xrot = 90;
			break;
		case 5: // X-
			yrot = 90;
			break;
		default:;
		}

		if (yrot){
			for (u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateXZBy(yrot);
			}
		}else if (xrot) {
			for (u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateYZBy(xrot);
			}
		}

		u16 indices[] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,g_6dirs[j],4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tiles[j].getMaterial(), vertices, 4, indices, 6, colours);
	}
}

void meshgen_torchlike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	static const f32 txc[24] = {
		0.625,0.125,0.75,0.25,
		0.625,0.625,0.625,0.75,
		0,0,0.125,1,
		0,0,0.125,1,
		0,0,0.125,1,
		0,0,0.125,1
	};
	static v3s16 faces[6] = {
		v3s16( 0, 1, 0),
		v3s16( 0,-1, 0),
		v3s16( 1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16( 0, 0, 1),
		v3s16( 0, 0,-1),
	};
	v3s16 dir = unpackDir(n.param2);
	video::S3DVertex vertices[6][4] = {
		{	// up
			video::S3DVertex(-0.0625*BS,0.125*BS,0.0625*BS, 0,1,0, video::SColor(255,255,255,255), txc[0],txc[1]),
			video::S3DVertex(0.0625*BS,0.125*BS,0.0625*BS, 0,1,0, video::SColor(255,255,255,255), txc[2],txc[1]),
			video::S3DVertex(0.0625*BS,0.125*BS,-0.0625*BS, 0,1,0, video::SColor(255,255,255,255), txc[2],txc[3]),
			video::S3DVertex(-0.0625*BS,0.125*BS,-0.0625*BS, 0,1,0, video::SColor(255,255,255,255), txc[0],txc[3])
		},{	// down
			video::S3DVertex(-0.0625*BS,-0.5*BS,-0.0625*BS, 0,-1,0, video::SColor(255,255,255,255), txc[4],txc[5]),
			video::S3DVertex(0.0625*BS,-0.5*BS,-0.0625*BS, 0,-1,0, video::SColor(255,255,255,255), txc[6],txc[5]),
			video::S3DVertex(0.0625*BS,-0.5*BS,0.0625*BS, 0,-1,0, video::SColor(255,255,255,255), txc[6],txc[7]),
			video::S3DVertex(-0.0625*BS,-0.5*BS,0.0625*BS, 0,-1,0, video::SColor(255,255,255,255), txc[4],txc[7])
		},{	// right
			video::S3DVertex(0.0625*BS,0.125*BS,-0.0625*BS, 1,0,0, video::SColor(255,255,255,255), txc[ 8],txc[9]),
			video::S3DVertex(0.0625*BS,0.125*BS,0.0625*BS, 1,0,0, video::SColor(255,255,255,255), txc[10],txc[9]),
			video::S3DVertex(0.0625*BS,-0.5*BS,0.0625*BS, 1,0,0, video::SColor(255,255,255,255), txc[10],txc[11]),
			video::S3DVertex(0.0625*BS,-0.5*BS,-0.0625*BS, 1,0,0, video::SColor(255,255,255,255), txc[ 8],txc[11])
		},{	// left
			video::S3DVertex(-0.0625*BS,0.125*BS,0.0625*BS, -1,0,0, video::SColor(255,255,255,255), txc[12],txc[13]),
			video::S3DVertex(-0.0625*BS,0.125*BS,-0.0625*BS, -1,0,0, video::SColor(255,255,255,255), txc[14],txc[13]),
			video::S3DVertex(-0.0625*BS,-0.5*BS,-0.0625*BS, -1,0,0, video::SColor(255,255,255,255), txc[14],txc[15]),
			video::S3DVertex(-0.0625*BS,-0.5*BS,0.0625*BS, -1,0,0, video::SColor(255,255,255,255), txc[12],txc[15])
		},{	// back
			video::S3DVertex(0.0625*BS,0.125*BS,0.0625*BS, 0,0,1, video::SColor(255,255,255,255), txc[16],txc[17]),
			video::S3DVertex(-0.0625*BS,0.125*BS,0.0625*BS, 0,0,1, video::SColor(255,255,255,255), txc[18],txc[17]),
			video::S3DVertex(-0.0625*BS,-0.5*BS,0.0625*BS, 0,0,1, video::SColor(255,255,255,255), txc[18],txc[19]),
			video::S3DVertex(0.0625*BS,-0.5*BS,0.0625*BS, 0,0,1, video::SColor(255,255,255,255), txc[16],txc[19])
		},{	// front
			video::S3DVertex(-0.0625*BS,0.125*BS,-0.0625*BS, 0,0,-1, video::SColor(255,255,255,255), txc[20],txc[21]),
			video::S3DVertex(0.0625*BS,0.125*BS,-0.0625*BS, 0,0,-1, video::SColor(255,255,255,255), txc[22],txc[21]),
			video::S3DVertex(0.0625*BS,-0.5*BS,-0.0625*BS, 0,0,-1, video::SColor(255,255,255,255), txc[22],txc[23]),
			video::S3DVertex(-0.0625*BS,-0.5*BS,-0.0625*BS, 0,0,-1, video::SColor(255,255,255,255), txc[20],txc[23])
		}
	};

	TileSpec tile = content_features(n).tiles[0];
	f32 sx = tile.texture.x1()-tile.texture.x0();
	f32 sy = tile.texture.y1()-tile.texture.y0();

	if (dir.Y == 1) { // roof
		for (s32 i=0; i<6; i++) {
			for (int j=0; j<4; j++) {
				vertices[i][j].Pos.rotateXYBy(175);
				vertices[i][j].Pos.rotateYZBy(5);
				vertices[i][j].TCoords *= v2f(sx,sy);
				vertices[i][j].TCoords += v2f(
					tile.texture.x0(),
					tile.texture.y0()
				);
			}
		}
	}else if (dir.Y == -1) { // floor
		for (s32 i=0; i<6; i++) {
			for (int j=0; j<4; j++) {
				vertices[i][j].TCoords *= v2f(sx,sy);
				vertices[i][j].TCoords += v2f(
					tile.texture.x0(),
					tile.texture.y0()
				);
			}
		}
	}else{ // wall
		for (s32 i=0; i<6; i++) {
			for (int j=0; j<4; j++) {
				vertices[i][j].Pos.Y += 0.25*BS;
				vertices[i][j].Pos.rotateYZBy(-5);
				vertices[i][j].Pos += v3f(0.,0.,0.4*BS);
				if (dir.X == 1) {
					vertices[i][j].Pos.rotateXZBy(-90);
				}else if (dir.X == -1) {
					vertices[i][j].Pos.rotateXZBy(90);
				}else if (dir.Z == 1) {
					vertices[i][j].Pos.rotateXZBy(0);
				}else if (dir.Z == -1) {
					vertices[i][j].Pos.rotateXZBy(180);
				}

				vertices[i][j].TCoords *= v2f(sx,sy);
				vertices[i][j].TCoords += v2f(
					tile.texture.x0(),
					tile.texture.y0()
				);
			}
		}
	}

	u16 indices[] = {0,1,2,2,3,0};


	v3f pos = intToFloat(p,BS);
	// Add to mesh collector
	for (s32 j=0; j<6; j++) {
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,faces[j],4,vertices[j]);
		}

		for (u16 i=0; i<4; i++) {
			vertices[j][i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices[j], 4, indices, 6, colours);
	}
}

void meshgen_fencelike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	static const v3s16 tile_dirs[6] = {
		v3s16( 0, 1, 0),
		v3s16( 0,-1, 0),
		v3s16( 1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16( 0, 0, 1),
		v3s16( 0, 0,-1)
	};
	static const v3s16 fence_dirs[8] = {
		v3s16(1,0,0),
		v3s16(-1,0,0),
		v3s16(0,0,1),
		v3s16(0,0,-1),
		v3s16(1,0,1),
		v3s16(1,0,-1),
		v3s16(-1,0,1),
		v3s16(-1,0,-1)
	};
	static const int showcheck[4][2] = {
		{0,2},
		{0,3},
		{1,2},
		{1,3}
	};
	static const int shown_angles[8] = {0,0,0,0,45,135,45,315};
	bool shown_dirs[8] = {false,false,false,false,false,false,false,false};
	n.param2 = 0;

	TileSpec tiles[6];
	for (int i = 0; i < 6; i++) {
		// Handles facedir rotation for textures
		tiles[i] = getNodeTile(n,p,tile_dirs[i],data->m_temp_mods);
	}

	v3f pos = intToFloat(p, BS);
	std::vector<NodeBox> boxes = content_features(n).getNodeBoxes(n);
	int bi = 1;
	v3s16 p2 = p;
	p2.Y++;
	MapNode n2 = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p2);
	const ContentFeatures *f2 = &content_features(n2);
	aabb3f box;
	if (f2->draw_type == CDT_AIRLIKE || f2->draw_type == CDT_TORCHLIKE)
		bi = 0;
	{
		NodeBox box = boxes[bi];

		// Compute texture coords
		f32 tx1 = (box.m_box.MinEdge.X/BS)+0.5;
		f32 ty1 = (box.m_box.MinEdge.Y/BS)+0.5;
		f32 tz1 = (box.m_box.MinEdge.Z/BS)+0.5;
		f32 tx2 = (box.m_box.MaxEdge.X/BS)+0.5;
		f32 ty2 = (box.m_box.MaxEdge.Y/BS)+0.5;
		f32 tz2 = (box.m_box.MaxEdge.Z/BS)+0.5;
		f32 txc[24] = {
			// up
			tx1, 1-tz2, tx2, 1-tz1,
			// down
			tx1, tz1, tx2, tz2,
			// right
			tz1, 1-ty2, tz2, 1-ty1,
			// left
			1-tz2, 1-ty2, 1-tz1, 1-ty1,
			// back
			1-tx2, 1-ty2, 1-tx1, 1-ty1,
			// front
			tx1, 1-ty2, tx2, 1-ty1,
		};
		meshgen_cuboid(data,n,p, pos, box.m_box, tiles, 6,  selected, txc, v3s16(0,0,0),v3f(0,0,0));
	}

	int bps = ((boxes.size()-2)/4); // boxes per section
	u8 np = 1;

	for (int k=0; k<8; k++) {
		if (k > 3 && (shown_dirs[showcheck[k-4][0]] || shown_dirs[showcheck[k-4][1]]))
					continue;
		p2 = data->m_blockpos_nodes+p+fence_dirs[k];
		n2 = data->m_vmanip.getNodeRO(p2);
		f2 = &content_features(n2);
		if (
			f2->draw_type == CDT_FENCELIKE
			|| f2->draw_type == CDT_WALLLIKE
			|| n2.getContent() == CONTENT_WOOD_GATE
			|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
			|| n2.getContent() == CONTENT_STEEL_GATE
			|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
			|| (
				n2.getContent() != CONTENT_IGNORE
				&& n2.getContent() == content_features(n).special_alternate_node
			)
		) {
			shown_dirs[k] = true;
			n.param2 |= (np<<k);
			for (int i=0; i<bps; i++) {
				NodeBox box = boxes[i+2+(bps*(k%4))];

				// Compute texture coords
				f32 tx1 = (box.m_box.MinEdge.X/BS)+0.5;
				f32 ty1 = (box.m_box.MinEdge.Y/BS)+0.5;
				f32 tz1 = (box.m_box.MinEdge.Z/BS)+0.5;
				f32 tx2 = (box.m_box.MaxEdge.X/BS)+0.5;
				f32 ty2 = (box.m_box.MaxEdge.Y/BS)+0.5;
				f32 tz2 = (box.m_box.MaxEdge.Z/BS)+0.5;
				f32 txc[24] = {
					// up
					tx1, 1-tz2, tx2, 1-tz1,
					// down
					tx1, tz1, tx2, tz2,
					// right
					tz1, 1-ty2, tz2, 1-ty1,
					// left
					1-tz2, 1-ty2, 1-tz1, 1-ty1,
					// back
					1-tx2, 1-ty2, 1-tx1, 1-ty1,
					// front
					tx1, 1-ty2, tx2, 1-ty1,
				};
				if (k > 3) {
					switch (k) {
					case 4:
						box.m_box.MaxEdge.X *= 1.414;
						break;
					case 5:
						box.m_box.MinEdge.X *= 1.414;
						break;
					case 6:
						box.m_box.MaxEdge.Z *= 1.414;
						break;
					case 7:
						box.m_box.MinEdge.Z *= 1.414;
						break;
					default:;
					}
				}
				meshgen_cuboid(data,n,p, pos, box.m_box, tiles, 6,  selected, txc, v3s16(0,shown_angles[k],0),v3f(0,0,0));
			}
		}
	}
}

void meshgen_firelike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	TileSpec tile = getNodeTile(n,p,v3s16(0,1,0),data->m_temp_mods);
	content_t current = n.getContent();
	content_t n2c;
	MapNode n2;
	v3s16 n2p;
	static const v3s16 dirs[6] = {
		v3s16( 0, 1, 0),
		v3s16( 0,-1, 0),
		v3s16( 1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16( 0, 0, 1),
		v3s16( 0, 0,-1)
	};
	int doDraw[6] = {0,0,0,0,0,0};
	int i;
	// Draw the full flame even if there are no surrounding nodes
	bool drawAllFaces = true;
	// Check for adjacent nodes
	for (i = 0; i < 6; i++) {
		n2p = data->m_blockpos_nodes + p + dirs[i];
		n2 = data->m_vmanip.getNodeRO(n2p);
		n2c = n2.getContent();
		if (n2c != CONTENT_IGNORE && n2c != CONTENT_AIR && n2c != current) {
			doDraw[i] = 1;
			drawAllFaces = false;
		}
	}
	v3f pos = intToFloat(p,BS);
	for (u32 j=0; j<4; j++) {
		video::S3DVertex vertices[4] = {
			video::S3DVertex(-0.5*BS,-0.5*BS,0.369*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y1()),
			video::S3DVertex( 0.5*BS,-0.5*BS,0.369*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y1()),
			video::S3DVertex( 0.5*BS, 0.5*BS,0.369*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x1(), tile.texture.y0()),
			video::S3DVertex(-0.5*BS, 0.5*BS,0.369*BS, 0,0,0, video::SColor(255,255,255,255), tile.texture.x0(), tile.texture.y0())
		};
		int vOffset = 1; // Vertical offset of faces after rotation
		// Calculate which faces should be drawn
		if(j == 0 && (drawAllFaces || (doDraw[3] == 1 || doDraw[1] == 1))) {
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateXZBy(90);
				vertices[i].Pos.rotateXYBy(-15);
				vertices[i].Pos.Y -= vOffset;
			}
		}else if(j == 1 && (drawAllFaces || (doDraw[5] == 1 || doDraw[1] == 1))) {
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateXZBy(180);
				vertices[i].Pos.rotateYZBy(15);
				vertices[i].Pos.Y -= vOffset;
			}
		}else if(j == 2 && (drawAllFaces || (doDraw[2] == 1 || doDraw[1] == 1))) {
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateXZBy(270);
				vertices[i].Pos.rotateXYBy(15);
				vertices[i].Pos.Y -= vOffset;
			}
		}else if(j == 3 && (drawAllFaces || (doDraw[4] == 1 || doDraw[1] == 1))) {
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateYZBy(-15);
				vertices[i].Pos.Y -= vOffset;
			}
		}else if(j == 3 && (drawAllFaces || (doDraw[0] == 1 && doDraw[1] == 0))) {
			for(u16 i=0; i<4; i++) {
				vertices[i].Pos.rotateYZBy(-90);
				vertices[i].Pos.Y += vOffset;
			}
		}else{
			// Skip faces that aren't adjacent to a node
			continue;
		}
		u16 indices[] = {0,1,2,2,3,0};
		std::vector<video::SColor> colours[18];
		if (selected) {
			meshgen_selected_lights(colours,255,4);
		}else{
			meshgen_lights(data,n,p,colours,255,v3s16(0,0,0),4,vertices);
		}

		for (u16 i=0; i<4; i++) {
			vertices[i].Pos += pos;
		}

		data->append(tile.getMaterial(), vertices, 4, indices, 6, colours);
	}
}

void meshgen_walllike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	static const v3s16 tile_dirs[6] = {
		v3s16(0, 1, 0),
		v3s16(0, -1, 0),
		v3s16(1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16(0, 0, 1),
		v3s16(0, 0, -1)
	};
	static const int shown_angles[8] = {0,0,0,0,45,135,45,315};
	n.param2 = 0;

	TileSpec tiles[6];
	for (int i = 0; i < 6; i++) {
		// Handles facedir rotation for textures
		tiles[i] = getNodeTile(n,p,tile_dirs[i],data->m_temp_mods);
	}

	v3f pos = intToFloat(p, BS);
	std::vector<NodeBox> boxes = content_features(n).getNodeBoxes(n);
	v3s16 p2 = p;
	p2.Y++;
	NodeBox box;
	u8 d[8];
	int bi = meshgen_check_walllike(data,n,p+data->m_blockpos_nodes,d);
	{
		box = boxes[bi];

		// Compute texture coords
		f32 tx1 = (box.m_box.MinEdge.X/BS)+0.5;
		f32 ty1 = (box.m_box.MinEdge.Y/BS)+0.5;
		f32 tz1 = (box.m_box.MinEdge.Z/BS)+0.5;
		f32 tx2 = (box.m_box.MaxEdge.X/BS)+0.5;
		f32 ty2 = (box.m_box.MaxEdge.Y/BS)+0.5;
		f32 tz2 = (box.m_box.MaxEdge.Z/BS)+0.5;
		f32 txc[24] = {
			// up
			tx1, 1-tz2, tx2, 1-tz1,
			// down
			tx1, tz1, tx2, tz2,
			// right
			tz1, 1-ty2, tz2, 1-ty1,
			// left
			1-tz2, 1-ty2, 1-tz1, 1-ty1,
			// back
			1-tx2, 1-ty2, 1-tx1, 1-ty1,
			// front
			tx1, 1-ty2, tx2, 1-ty1,
		};
		meshgen_cuboid(data,n,p,pos,box.m_box,tiles,6,selected,txc,v3s16(0,0,0),v3f(0,0,0));
	}

	int bps = ((boxes.size()-3)/4); // boxes per section
	u8 np = 1;

	for (int k=0; k<8; k++) {
		if (d[k]) {
			n.param2 |= (np<<k);
			for (int i=0; i<bps; i++) {
				box = boxes[i+3+(bps*(k%4))];

				// Compute texture coords
				f32 tx1 = (box.m_box.MinEdge.X/BS)+0.5;
				f32 ty1 = (box.m_box.MinEdge.Y/BS)+0.5;
				f32 tz1 = (box.m_box.MinEdge.Z/BS)+0.5;
				f32 tx2 = (box.m_box.MaxEdge.X/BS)+0.5;
				f32 ty2 = (box.m_box.MaxEdge.Y/BS)+0.5;
				f32 tz2 = (box.m_box.MaxEdge.Z/BS)+0.5;
				f32 txc[24] = {
					// up
					tx1, 1-tz2, tx2, 1-tz1,
					// down
					tx1, tz1, tx2, tz2,
					// right
					tz1, 1-ty2, tz2, 1-ty1,
					// left
					1-tz2, 1-ty2, 1-tz1, 1-ty1,
					// back
					1-tx2, 1-ty2, 1-tx1, 1-ty1,
					// front
					tx1, 1-ty2, tx2, 1-ty1,
				};
				if (k > 3) {
					switch (k) {
					case 4:
						box.m_box.MaxEdge.X *= 1.414;
						break;
					case 5:
						box.m_box.MinEdge.X *= 1.414;
						break;
					case 6:
						box.m_box.MaxEdge.Z *= 1.414;
						break;
					case 7:
						box.m_box.MinEdge.Z *= 1.414;
						break;
					default:;
					}
				}
				meshgen_cuboid(data,n,p, pos, box.m_box, tiles, 6,  selected, txc, v3s16(0,shown_angles[k],0),v3f(0,0,0));
			}
		}
	}
}

void meshgen_rooflike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	bool is_roof_x [] = { false, false };  /* x-1, x+1 */
	bool is_roof_z [] = { false, false };  /* z-1, z+1 */

	bool is_roof_z_minus_y [] = { false, false };  /* z-1, z+1; y-1 */
	bool is_roof_x_minus_y [] = { false, false };  /* x-1, z+1; y-1 */
	bool is_roof_z_plus_y [] = { false, false };  /* z-1, z+1; y+1 */
	bool is_roof_x_plus_y [] = { false, false };  /* x-1, x+1; y+1 */

	MapNode n_minus_x = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1,0,0));
	MapNode n_plus_x = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(1,0,0));
	MapNode n_minus_z = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0,0,-1));
	MapNode n_plus_z = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0,0,1));
	MapNode n_plus_x_plus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(1, 1, 0));
	MapNode n_plus_x_minus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(1, -1, 0));
	MapNode n_minus_x_plus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1, 1, 0));
	MapNode n_minus_x_minus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1, -1, 0));
	MapNode n_plus_z_plus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0, 1, 1));
	MapNode n_minus_z_plus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0, 1, -1));
	MapNode n_plus_z_minus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0, -1, 1));
	MapNode n_minus_z_minus_y = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(0, -1, -1));

	if (content_features(n_minus_x).draw_type == CDT_ROOFLIKE)
		is_roof_x[0] = true;
	if (content_features(n_minus_x_minus_y).draw_type == CDT_ROOFLIKE)
		is_roof_x_minus_y[0] = true;
	if (content_features(n_minus_x_plus_y).draw_type == CDT_ROOFLIKE)
		is_roof_x_plus_y[0] = true;
	if (content_features(n_plus_x).draw_type == CDT_ROOFLIKE)
		is_roof_x[1] = true;
	if (content_features(n_plus_x_minus_y).draw_type == CDT_ROOFLIKE)
		is_roof_x_minus_y[1] = true;
	if (content_features(n_plus_x_plus_y).draw_type == CDT_ROOFLIKE)
		is_roof_x_plus_y[1] = true;
	if (content_features(n_minus_z).draw_type == CDT_ROOFLIKE)
		is_roof_z[0] = true;
	if (content_features(n_minus_z_minus_y).draw_type == CDT_ROOFLIKE)
		is_roof_z_minus_y[0] = true;
	if (content_features(n_minus_z_plus_y).draw_type == CDT_ROOFLIKE)
		is_roof_z_plus_y[0] = true;
	if (content_features(n_plus_z).draw_type == CDT_ROOFLIKE)
		is_roof_z[1] = true;
	if (content_features(n_plus_z_minus_y).draw_type == CDT_ROOFLIKE)
		is_roof_z_minus_y[1] = true;
	if (content_features(n_plus_z_plus_y).draw_type == CDT_ROOFLIKE)
		is_roof_z_plus_y[1] = true;

	u8 adjacencies = is_roof_x[0] + is_roof_x[1] + is_roof_z[0] + is_roof_z[1];

	// get the tile, with crack if being dug
	TileSpec tile = getNodeTile(n,p,v3s16(0,1,0),data->m_temp_mods);
	TileSpec toptile = getNodeTile(n,p,v3s16(0,-1,0),data->m_temp_mods);

	u8 type = 0;
	s16 angle = 0;

	MapNode abv;

	v3f pos = intToFloat(p, BS);

	if (adjacencies == 1) {
		// cross X
		if (is_roof_x[0] || is_roof_x[1]) {
			if (is_roof_z_plus_y[0]) {
				type = 0;
				angle = 180;
			}else if (is_roof_z_plus_y[1]) {
				type = 0;
				angle = 0;
			}else if (is_roof_x[0] && is_roof_x_minus_y[1]) {
				type = 9;
				angle = 0;
			}else if (is_roof_x[1] && is_roof_x_minus_y[0]) {
				type = 9;
				angle = 180;
			}else{
				type = 1;
				angle = 0;
			}
		}
		// cross Z
		else if (is_roof_z[0] || is_roof_z[1]) {
			if (is_roof_x_plus_y[1]) {
				type = 0;
				angle = 270;
			}else if (is_roof_x_plus_y[0]) {
				type = 0;
				angle = 90;
			}else if (is_roof_z[0] && is_roof_z_minus_y[1]) {
				type = 9;
				angle = 90;
			}else if (is_roof_z[1] && is_roof_z_minus_y[0]) {
				type = 9;
				angle = 270;
			}else{
				type = 1;
				angle = 90;
			}
		}
	}else if (adjacencies == 2) {
		// cross X
		if (is_roof_x[0] && is_roof_x[1]) {
			if (is_roof_z_plus_y[0]) {
				type = 0;
				angle = 180;
			}else if (is_roof_z_plus_y[1]) {
				type = 0;
				angle = 0;
			}else{
				type = 1;
				angle = 0;
			}
		}
		// cross Z
		else if (is_roof_z[0] && is_roof_z[1]) {
			if (is_roof_x_plus_y[1]) {
				type = 0;
				angle = 270;
			}else if (is_roof_x_plus_y[0]) {
				type = 0;
				angle = 90;
			}else{
				type = 1;
				angle = 90;
			}
		}else if (is_roof_x[0] && is_roof_z[0]) {
			if (is_roof_x_plus_y[1] && is_roof_z_plus_y[1]) {
				type = 7;
				angle = 90;
			}else if (is_roof_x_plus_y[1]) {
				type = 2;
				angle = 0;
			}else if (is_roof_z_plus_y[1]) {
				type = 2;
				angle = 90;
			}else{
				abv = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1, 1,-1));
				if (content_features(abv).draw_type == CDT_ROOFLIKE) {
					type = 4;
					angle = 90;
				}else{
					type = 3;
					angle = 0;
				}
			}
		}else if (is_roof_x[0] && is_roof_z[1]) {
			if (is_roof_x_plus_y[1] && is_roof_z_plus_y[0]) {
				type = 7;
				angle = 0;
			}else if (is_roof_x_plus_y[1]) {
				type = 2;
				angle = 0;
			}else if (is_roof_z_plus_y[0]) {
				type = 2;
				angle = 270;
			}else{
				abv = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1, 1, 1));
				if (content_features(abv).draw_type == CDT_ROOFLIKE) {
					type = 4;
					angle = 0;
				}else{
					type = 3;
					angle = 270;
				}
			}
		}else if (is_roof_x[1] && is_roof_z[0]) {
			if (is_roof_x_plus_y[0] && is_roof_z_plus_y[1]) {
				type = 7;
				angle = 180;
			}else if (is_roof_x_plus_y[0]) {
				type = 2;
				angle = 180;
			}else if (is_roof_z_plus_y[1]) {
				type = 2;
				angle = 90;
			}else{
				abv = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16( 1, 1,-1));
				if (content_features(abv).draw_type == CDT_ROOFLIKE) {
					type = 4;
					angle = 180;
				}else{
					type = 3;
					angle = 90;
				}
			}
		}else if (is_roof_x[1] && is_roof_z[1]) {
			if (is_roof_x_plus_y[0] && is_roof_z_plus_y[0]) {
				type = 7;
				angle = 270;
			}else if (is_roof_x_plus_y[0]) {
				type = 2;
				angle = 180;
			}else if (is_roof_z_plus_y[0]) {
				type = 2;
				angle = 270;
			}else{
				abv = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16( 1, 1, 1));
				if (content_features(abv).draw_type == CDT_ROOFLIKE) {
					type = 4;
					angle = 270;
				}else{
					type = 3;
					angle = 180;
				}
			}
		}
	}else if (adjacencies == 3) {
		if (is_roof_x[0] && is_roof_x[1] && is_roof_z[0]) {
			if (is_roof_z_plus_y[1]) {
				type = 2;
				angle = 90;
			}else{
				type = 6;
				angle = 0;
			}
		}else if (is_roof_x[0] && is_roof_x[1] && is_roof_z[1]) {
			if (is_roof_z_plus_y[0]) {
				type = 2;
				angle = 270;
			}else{
				type = 6;
				angle = 180;
			}
		}else if (is_roof_x[0] && is_roof_z[0] && is_roof_z[1]) {
			if (is_roof_x_plus_y[1]) {
				type = 2;
				angle = 0;
			}else{
				type = 6;
				angle = 270;
			}
		}else if (is_roof_x[1] && is_roof_z[0] && is_roof_z[1]) {
			if (is_roof_x_plus_y[0]) {
				type = 2;
				angle = 180;
			}else{
				type = 6;
				angle = 90;
			}
		}
	}else if (adjacencies == 4) {
		type = 5;
		angle = 0;
	}else{
		if (is_roof_z_plus_y[0]) {
			type = 0;
			angle = 180;
		}else if (is_roof_z_plus_y[1]) {
			type = 0;
		}else if (is_roof_x_plus_y[1]) {
			type = 0;
			angle = 270;
		}else if (is_roof_x_plus_y[0]) {
			type = 0;
			angle = 90;
		}else{
			type = 8;
			angle = 0;
		}
	}

	v3s16 face(0,1,0);
	/*
		0: slope
		1: top
		2: top butting to slope
		3: top corner
		4: outer corner
		5: top X
		6: top T
		7: inner corner
		8: top cap
		9: top end cap
	*/
	switch (type) {
	case 0:
	{
		v3f cnr[2][3];
		if (angle == 0) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.5,0.5);
			cnr[1][0] = v3f(0.5,0.5,0.5);
			cnr[1][1] = v3f(-0.5,0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			face = v3s16(0,1,-1);
		}else if (angle == 90) {
			cnr[0][0] = v3f(-0.5,0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.5,-0.5,0.5);
			cnr[1][1] = v3f(-0.5,0.5,0.5);
			cnr[1][2] = v3f(-0.5,0.5,-0.5);
			face = v3s16(1,1,0);
		}else if (angle == 180) {
			cnr[0][0] = v3f(-0.5,0.5,-0.5);
			cnr[0][1] = v3f(0.5,0.5,-0.5);
			cnr[0][2] = v3f(0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.5,-0.5,0.5);
			cnr[1][1] = v3f(-0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,0.5,-0.5);
			face = v3s16(0,1,1);
		}else if (angle == 270) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.5,0.5);
			cnr[1][0] = v3f(0.5,0.5,0.5);
			cnr[1][1] = v3f(-0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			face = v3s16(-1,1,0);
		}
		s16 a = 180-angle;
		if (a < 0)
			a += 360;
		for (int s=0; s<2; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,tile,selected,a,face);
		}
	}
	break;
	case 1:
	{
		v3f cnr[4][3];
		if (angle == 0 || angle == 180) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.,0.);
			cnr[1][0] = v3f(0.5,0.,0.);
			cnr[1][1] = v3f(-0.5,0.,0.);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			cnr[2][0] = v3f(-0.5,0.,0.);
			cnr[2][1] = v3f(0.5,0.,0.);
			cnr[2][2] = v3f(0.5,-0.5,0.5);
			cnr[3][0] = v3f(0.5,-0.5,0.5);
			cnr[3][1] = v3f(-0.5,-0.5,0.5);
			cnr[3][2] = v3f(-0.5,0.,0.);
		}else if (angle == 90 || angle == 270) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(-0.5,-0.5,0.5);
			cnr[0][2] = v3f(0.,0.,0.5);
			cnr[1][0] = v3f(0.,0.,0.5);
			cnr[1][1] = v3f(0.,0.,-0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			cnr[2][0] = v3f(0.,0.,-0.5);
			cnr[2][1] = v3f(0.,0.,0.5);
			cnr[2][2] = v3f(0.5,-0.5,0.5);
			cnr[3][0] = v3f(0.5,-0.5,0.5);
			cnr[3][1] = v3f(0.5,-0.5,-0.5);
			cnr[3][2] = v3f(0.,0.,-0.5);
		}
		s16 a = angle;
		if (a < 180)
			a += 180;
		for (int s=0; s<4; s++) {
			if (s == 2)
				a -= 180;
			meshgen_rooftri(data,n,p,cnr[s],pos,toptile,selected,a,face);
		}
	}
	break;
	case 2:
	{
		v3f cnr[2][3];
		if (angle == 90) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.5,0.5);
			cnr[1][0] = v3f(0.5,0.5,0.5);
			cnr[1][1] = v3f(-0.5,0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			face = v3s16(0,1,-1);
		}else if (angle == 180) {
			cnr[0][0] = v3f(-0.5,0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.5,-0.5,0.5);
			cnr[1][1] = v3f(-0.5,0.5,0.5);
			cnr[1][2] = v3f(-0.5,0.5,-0.5);
			face = v3s16(1,1,0);
		}else if (angle == 270) {
			cnr[0][0] = v3f(-0.5,0.5,-0.5);
			cnr[0][1] = v3f(0.5,0.5,-0.5);
			cnr[0][2] = v3f(0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.5,-0.5,0.5);
			cnr[1][1] = v3f(-0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,0.5,-0.5);
			face = v3s16(0,1,1);
		}else if (angle == 0) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.5,0.5);
			cnr[1][0] = v3f(0.5,0.5,0.5);
			cnr[1][1] = v3f(-0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			face = v3s16(-1,1,0);
		}
		s16 a = 270-angle;
		if (a < 0)
			a += 360;
		for (int s=0; s<2; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,tile,selected,a,face);
		}
	}
	{
		v3f cnr[2][3];
		if (angle == 0) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(-0.5,0.,0.);
			cnr[0][2] = v3f(-0.5,-0.5,-0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(-0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,0.,0.);
			face = v3s16(-1,1,0);
		}else if (angle == 90) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(0.,0.,-0.5);
			cnr[0][2] = v3f(-0.5,-0.5,-0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.5,-0.5,-0.5);
			cnr[1][2] = v3f(0.,0.,-0.5);
			face = v3s16(0,1,-1);
		}else if (angle == 180) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(0.5,0.,0.);
			cnr[0][2] = v3f(0.5,-0.5,-0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.5,-0.5,0.5);
			cnr[1][2] = v3f(0.5,0.,0.);
			face = v3s16(1,1,0);
		}else if (angle == 270) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(0.,0.,0.5);
			cnr[0][2] = v3f(-0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.5,-0.5,0.5);
			cnr[1][2] = v3f(0.,0.,0.5);
			face = v3s16(0,1,1);
		}
		s16 a = angle;
		if (a < 180)
			a += 180;
		for (int s=0; s<2; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,toptile,selected,a,face);
			a -= 180;
		}
	}
	break;
	case 3:
	{
		v3f cnr[2][3];
		s16 a1 = angle;
		s16 a2 = angle + 90;
		if (angle == 0) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(-0.5,0.,0.);
			cnr[0][2] = v3f(-0.5,-0.5,-0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.,0.,-0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			a1 = 180;
			a2 = 270;
		}else if (angle == 90) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(0.,0.,-0.5);
			cnr[0][2] = v3f(0.5,-0.5,-0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.5,0.,0.);
			cnr[1][2] = v3f(0.5,-0.5,-0.5);
		}else if (angle == 180) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(0.,0.,0.5);
			cnr[0][2] = v3f(0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.5,0.,0.);
			cnr[1][2] = v3f(0.5,-0.5,0.5);
			a1 = 90;
			a2 = 0;
		}else if (angle == 270) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(0.,0.,0.5);
			cnr[0][2] = v3f(-0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(-0.5,0.,0.);
			cnr[1][2] = v3f(-0.5,-0.5,0.5);
		}
		s16 a = a1;
		for (int s=0; s<2; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,toptile,selected,a,face);
			a = a2;
		}
	}
	{
		v3f cnr[4][3];
		s16 a1 = angle;
		s16 a2 = angle + 90;
		if (angle == 0) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(-0.5,-0.5,0.5);
			cnr[0][2] = v3f(-0.5,0.,0.);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,0.5);
			cnr[2][0] = v3f(0.,0.,0.);
			cnr[2][1] = v3f(0.,0.,-0.5);
			cnr[2][2] = v3f(0.5,-0.5,-0.5);
			cnr[3][0] = v3f(0.,0.,0.);
			cnr[3][1] = v3f(0.5,-0.5,-0.5);
			cnr[3][2] = v3f(0.5,-0.5,0.5);
		}else if (angle == 90) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(-0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.,0.,-0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(-0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			cnr[2][0] = v3f(0.,0.,0.);
			cnr[2][1] = v3f(0.5,0.,0.);
			cnr[2][2] = v3f(0.5,-0.5,0.5);
			cnr[3][0] = v3f(0.,0.,0.);
			cnr[3][1] = v3f(0.5,-0.5,0.5);
			cnr[3][2] = v3f(-0.5,-0.5,0.5);
			a1 = 270;
			a2 = 0;
		}else if (angle == 180) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.,0.);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(-0.5,-0.5,-0.5);
			cnr[1][2] = v3f(0.5,-0.5,-0.5);
			cnr[2][0] = v3f(0.,0.,0.);
			cnr[2][1] = v3f(0.,0.,0.5);
			cnr[2][2] = v3f(-0.5,-0.5,0.5);
			cnr[3][0] = v3f(0.,0.,0.);
			cnr[3][1] = v3f(-0.5,-0.5,0.5);
			cnr[3][2] = v3f(-0.5,-0.5,-0.5);
		}else if (angle == 270) {
			cnr[0][0] = v3f(0.,0.,0.);
			cnr[0][1] = v3f(0.5,-0.5,0.5);
			cnr[0][2] = v3f(0.,0.,0.5);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.5,-0.5,-0.5);
			cnr[1][2] = v3f(0.5,-0.5,0.5);
			cnr[2][0] = v3f(0.,0.,0.);
			cnr[2][1] = v3f(-0.5,0.,0.);
			cnr[2][2] = v3f(-0.5,-0.5,-0.5);
			cnr[3][0] = v3f(0.,0.,0.);
			cnr[3][1] = v3f(-0.5,-0.5,-0.5);
			cnr[3][2] = v3f(0.5,-0.5,-0.5);
			a1 = 90;
			a2 = 180;
		}
		s16 a = a1;
		for (int s=0; s<4; s++) {
			if (s == 2)
				a = a2;
			meshgen_rooftri(data,n,p,cnr[s],pos,toptile,selected,a,face);
		}
	}
	break;
	case 4:
	{
		v3f cnr[2][3];
		s16 a1 = angle;
		s16 a2 = angle - 90;
		if (angle == 0) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(-0.5,0.5,0.5);
			cnr[1][0] = v3f(-0.5,0.5,0.5);
			cnr[1][1] = v3f(0.5,-0.5,-0.5);
			cnr[1][2] = v3f(0.5,-0.5,0.5);
			a1 = 180;
			a2 = 90;
			face = v3s16(1,1,-1);
		}else if (angle == 90) {
			cnr[0][0] = v3f(-0.5,-0.5,0.5);
			cnr[0][1] = v3f(0.5,-0.5,0.5);
			cnr[0][2] = v3f(-0.5,0.5,-0.5);
			cnr[1][0] = v3f(-0.5,0.5,-0.5);
			cnr[1][1] = v3f(0.5,-0.5,0.5);
			cnr[1][2] = v3f(0.5,-0.5,-0.5);
			a1 = 0;
			a2 = 90;
			face = v3s16(1,1,1);
		}else if (angle == 180) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,0.5,-0.5);
			cnr[0][2] = v3f(-0.5,-0.5,0.5);
			cnr[1][0] = v3f(-0.5,-0.5,0.5);
			cnr[1][1] = v3f(0.5,0.5,-0.5);
			cnr[1][2] = v3f(0.5,-0.5,0.5);
			a1 = 270;
			a2 = 0;
			face = v3s16(-1,1,1);
		}else if (angle == 270) {
			cnr[0][0] = v3f(-0.5,-0.5,0.5);
			cnr[0][1] = v3f(0.5,0.5,0.5);
			cnr[0][2] = v3f(-0.5,-0.5,-0.5);
			cnr[1][0] = v3f(-0.5,-0.5,-0.5);
			cnr[1][1] = v3f(0.5,0.5,0.5);
			cnr[1][2] = v3f(0.5,-0.5,-0.5);
			face = v3s16(-1,1,-1);
		}
		s16 a = a1;
		for (int s=0; s<2; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,tile,selected,a,face);
			a = a2;
		}
	}
	break;
	case 5:
	{
		v3f cnr[8][3] = {
			{
				v3f(0.,0.,0.),
				v3f(-0.5,0.,0.),
				v3f(-0.5,-0.5,-0.5)
			},{
				v3f(-0.5,0.,0.),
				v3f(-0.5,-0.5,0.5),
				v3f(0.,0.,0.)
			},{
				v3f(0.,0.,0.),
				v3f(0.5,0.,0.),
				v3f(0.5,-0.5,-0.5)
			},{
				v3f(0.5,0.,0.),
				v3f(0.5,-0.5,0.5),
				v3f(0.,0.,0.)
			},{
				v3f(0.,0.,0.),
				v3f(0.,0.,-0.5),
				v3f(-0.5,-0.5,-0.5)
			},{
				v3f(0.,0.,-0.5),
				v3f(0.5,-0.5,-0.5),
				v3f(0.,0.,0.)
			},{
				v3f(0.,0.,0.),
				v3f(0.,0.,0.5),
				v3f(-0.5,-0.5,0.5)
			},{
				v3f(0.,0.,0.5),
				v3f(0.5,-0.5,0.5),
				v3f(0.,0.,0.)
			}
		};
		s16 a[8] = {
			180,
			0,
			180,
			0,
			270,
			90,
			270,
			90,
		};
		for (int s=0; s<8; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,toptile,selected,a[s],face);
		}
	}
	break;
	case 6:
	{
		v3f cnr[6][3];
		s16 a[6] = {0,0,0,0,0,0};
		if (angle == 0) {
			cnr[0][0] = v3f(-0.5,-0.5,0.5);
			cnr[0][1] = v3f(0.5,-0.5,0.5);
			cnr[0][2] = v3f(0.5,0.,0.);
			cnr[1][0] = v3f(0.5,0.,0.);
			cnr[1][1] = v3f(-0.5,0.,0.);
			cnr[1][2] = v3f(-0.5,-0.5,0.5);
			cnr[2][0] = v3f(-0.5,-0.5,-0.5);
			cnr[2][1] = v3f(0.5,-0.5,-0.5);
			cnr[2][2] = v3f(0.5,0.,0.);
			cnr[3][0] = v3f(0.5,0.,0.);
			cnr[3][1] = v3f(-0.5,0.,0.);
			cnr[3][2] = v3f(-0.5,-0.5,-0.5);
			cnr[4][0] = v3f(0.,0.,-0.5);
			cnr[4][1] = v3f(-0.5,-0.5,-0.5);
			cnr[4][2] = v3f(0.,0.,0.);
			cnr[5][0] = v3f(0.,0.,0.);
			cnr[5][1] = v3f(0.5,-0.5,-0.5);
			cnr[5][2] = v3f(0.,0.,-0.5);
			a[2] = 180;
			a[3] = 180;
			a[4] = 270;
			a[5] = 90;
		}else if (angle == 90) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(-0.5,-0.5,0.5);
			cnr[0][2] = v3f(0.,0.,0.5);
			cnr[1][0] = v3f(0.,0.,0.5);
			cnr[1][1] = v3f(0.,0.,-0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			cnr[2][0] = v3f(0.5,-0.5,-0.5);
			cnr[2][1] = v3f(0.5,-0.5,0.5);
			cnr[2][2] = v3f(0.,0.,0.5);
			cnr[3][0] = v3f(0.,0.,0.5);
			cnr[3][1] = v3f(0.,0.,-0.5);
			cnr[3][2] = v3f(0.5,-0.5,-0.5);
			cnr[4][0] = v3f(0.5,0.,0.);
			cnr[4][1] = v3f(0.5,-0.5,-0.5);
			cnr[4][2] = v3f(0.,0.,0.);
			cnr[5][0] = v3f(0.,0.,0.);
			cnr[5][1] = v3f(0.5,-0.5,0.5);
			cnr[5][2] = v3f(0.5,0.,0.);
			a[0] = 270;
			a[1] = 270;
			a[2] = 90;
			a[3] = 90;
			a[4] = 180;
		}else if (angle == 180) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.,0.);
			cnr[1][0] = v3f(0.5,0.,0.);
			cnr[1][1] = v3f(-0.5,0.,0.);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			cnr[2][0] = v3f(-0.5,-0.5,0.5);
			cnr[2][1] = v3f(0.5,-0.5,0.5);
			cnr[2][2] = v3f(0.5,0.,0.);
			cnr[3][0] = v3f(0.5,0.,0.);
			cnr[3][1] = v3f(-0.5,0.,0.);
			cnr[3][2] = v3f(-0.5,-0.5,0.5);
			cnr[4][0] = v3f(0.,0.,0.5);
			cnr[4][1] = v3f(-0.5,-0.5,0.5);
			cnr[4][2] = v3f(0.,0.,0.);
			cnr[5][0] = v3f(0.,0.,0.);
			cnr[5][1] = v3f(0.5,-0.5,0.5);
			cnr[5][2] = v3f(0.,0.,0.5);
			a[0] = 180;
			a[1] = 180;
			a[4] = 270;
			a[5] = 90;
		}else if (angle == 270) {
			cnr[0][0] = v3f(0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,0.5);
			cnr[0][2] = v3f(0.,0.,0.5);
			cnr[1][0] = v3f(0.,0.,0.5);
			cnr[1][1] = v3f(0.,0.,-0.5);
			cnr[1][2] = v3f(0.5,-0.5,-0.5);
			cnr[2][0] = v3f(-0.5,-0.5,-0.5);
			cnr[2][1] = v3f(-0.5,-0.5,0.5);
			cnr[2][2] = v3f(0.,0.,0.5);
			cnr[3][0] = v3f(0.,0.,0.5);
			cnr[3][1] = v3f(0.,0.,-0.5);
			cnr[3][2] = v3f(-0.5,-0.5,-0.5);
			cnr[4][0] = v3f(-0.5,0.,0.);
			cnr[4][1] = v3f(-0.5,-0.5,-0.5);
			cnr[4][2] = v3f(0.,0.,0.);
			cnr[5][0] = v3f(0.,0.,0.);
			cnr[5][1] = v3f(-0.5,-0.5,0.5);
			cnr[5][2] = v3f(-0.5,0.,0.);
			a[0] = 90;
			a[1] = 90;
			a[2] = 270;
			a[3] = 270;
			a[4] = 180;
		}
		for (int s=0; s<6; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,toptile,selected,a[s],face);
		}
	}
	break;
	case 7:
	{
		v3f cnr[2][3];
		s16 a1 = angle;
		s16 a2 = angle - 90;
		if (angle == 0) {
			cnr[0][0] = v3f(0.5,0.5,-0.5);
			cnr[0][1] = v3f(-0.5,0.5,-0.5);
			cnr[0][2] = v3f(-0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.5,0.5,-0.5);
			cnr[1][1] = v3f(0.5,0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,0.5);
			face = v3s16(-1,1,1);
		}else if (angle == 90) {
			cnr[0][0] = v3f(0.5,0.5,0.5);
			cnr[0][1] = v3f(-0.5,0.5,0.5);
			cnr[0][2] = v3f(-0.5,-0.5,-0.5);
			cnr[1][0] = v3f(0.5,0.5,0.5);
			cnr[1][1] = v3f(0.5,0.5,-0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
			a1 = 180;
			a2 = 270;
			face = v3s16(-1,1,-1);
		}else if (angle == 180) {
			cnr[0][0] = v3f(0.5,-0.5,-0.5);
			cnr[0][1] = v3f(-0.5,0.5,-0.5);
			cnr[0][2] = v3f(-0.5,0.5,0.5);
			cnr[1][0] = v3f(0.5,-0.5,-0.5);
			cnr[1][1] = v3f(0.5,0.5,0.5);
			cnr[1][2] = v3f(-0.5,0.5,0.5);
			a1 = 90;
			a2 = 180;
			face = v3s16(1,1,-1);
		}else if (angle == 270) {
			cnr[0][0] = v3f(0.5,-0.5,0.5);
			cnr[0][1] = v3f(-0.5,0.5,0.5);
			cnr[0][2] = v3f(-0.5,0.5,-0.5);
			cnr[1][0] = v3f(0.5,-0.5,0.5);
			cnr[1][1] = v3f(0.5,0.5,-0.5);
			cnr[1][2] = v3f(-0.5,0.5,-0.5);
			a1 = 90;
			a2 = 0;
			face = v3s16(1,1,1);
		}
		s16 a = a1;
		for (int s=0; s<2; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,tile,selected,a,face);
			a = a2;
		}
	}
	break;
	case 8:
	{
		v3f cnr[4][3];
		cnr[0][0] = v3f(0.,0.,0.);
		cnr[0][1] = v3f(-0.5,-0.5,-0.5);
		cnr[0][2] = v3f(-0.5,-0.5,0.5);
		cnr[1][0] = v3f(0.,0.,0.);
		cnr[1][1] = v3f(-0.5,-0.5,-0.5);
		cnr[1][2] = v3f(0.5,-0.5,-0.5);
		cnr[2][0] = v3f(0.,0.,0.);
		cnr[2][1] = v3f(0.5,-0.5,-0.5);
		cnr[2][2] = v3f(0.5,-0.5,0.5);
		cnr[3][0] = v3f(0.,0.,0.);
		cnr[3][1] = v3f(-0.5,-0.5,0.5);
		cnr[3][2] = v3f(0.5,-0.5,0.5);
		for (int s=0; s<4; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,toptile,selected,(90*s)+90+(180*(!(s%2))),face);
		}
	}
	break;
	case 9:
	{
		v3f cnr[5][3];
		s16 a[5] = {0,0,0,0,0};
		if (angle == 0) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.,0.,0.);
			cnr[1][0] = v3f(-0.5,-0.5,-0.5);
			cnr[1][1] = v3f(0.,0.,0.);
			cnr[1][2] = v3f(-0.5,0.,0.);
			cnr[2][0] = v3f(0.5,-0.5,0.5);
			cnr[2][1] = v3f(-0.5,-0.5,0.5);
			cnr[2][2] = v3f(0.,0.,0.);
			cnr[3][0] = v3f(0.,0.,0.);
			cnr[3][1] = v3f(-0.5,-0.5,0.5);
			cnr[3][2] = v3f(-0.5,0.,0.);
			cnr[4][0] = v3f(0.5,-0.5,-0.5);
			cnr[4][1] = v3f(0.5,-0.5,0.5);
			cnr[4][2] = v3f(0.,0.,0.);
			a[0] = 180;
			a[1] = 180;
			a[4] = 90;
			face = v3s16(1,1,0);
		}else if (angle == 90) {
			cnr[0][0] = v3f(-0.5,-0.5,0.5);
			cnr[0][1] = v3f(-0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.,0.,0.);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(-0.5,-0.5,-0.5);
			cnr[1][2] = v3f(0.,0.,-0.5);
			cnr[2][0] = v3f(0.5,-0.5,-0.5);
			cnr[2][1] = v3f(0.5,-0.5,0.5);
			cnr[2][2] = v3f(0.,0.,0.);
			cnr[3][0] = v3f(0.5,-0.5,-0.5);
			cnr[3][1] = v3f(0.,0.,0.);
			cnr[3][2] = v3f(0.,0.,-0.5);
			cnr[4][0] = v3f(-0.5,-0.5,0.5);
			cnr[4][1] = v3f(0.5,-0.5,0.5);
			cnr[4][2] = v3f(0.,0.,0.);
			a[0] = 270;
			a[1] = 270;
			a[2] = 90;
			a[3] = 90;
			face = v3s16(0,1,1);
		}else if (angle == 180) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.,0.,0.);
			cnr[1][0] = v3f(0.5,-0.5,-0.5);
			cnr[1][1] = v3f(0.,0.,0.);
			cnr[1][2] = v3f(0.5,0.,0.);
			cnr[2][0] = v3f(0.5,-0.5,0.5);
			cnr[2][1] = v3f(-0.5,-0.5,0.5);
			cnr[2][2] = v3f(0.,0.,0.);
			cnr[3][0] = v3f(0.,0.,0.);
			cnr[3][1] = v3f(0.5,-0.5,0.5);
			cnr[3][2] = v3f(0.5,0.,0.);
			cnr[4][0] = v3f(-0.5,-0.5,0.5);
			cnr[4][1] = v3f(-0.5,-0.5,-0.5);
			cnr[4][2] = v3f(0.,0.,0.);
			a[0] = 180;
			a[1] = 180;
			a[4] = 270;
			face = v3s16(-1,1,0);
		}else if (angle == 270) {
			cnr[0][0] = v3f(-0.5,-0.5,0.5);
			cnr[0][1] = v3f(-0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.,0.,0.);
			cnr[1][0] = v3f(0.,0.,0.);
			cnr[1][1] = v3f(0.,0.,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,0.5);
			cnr[2][0] = v3f(0.5,-0.5,-0.5);
			cnr[2][1] = v3f(0.5,-0.5,0.5);
			cnr[2][2] = v3f(0.,0.,0.);
			cnr[3][0] = v3f(0.,0.,0.);
			cnr[3][1] = v3f(0.,0.,0.5);
			cnr[3][2] = v3f(0.5,-0.5,0.5);
			cnr[4][0] = v3f(0.5,-0.5,-0.5);
			cnr[4][1] = v3f(-0.5,-0.5,-0.5);
			cnr[4][2] = v3f(0.,0.,0.);
			a[0] = 270;
			a[1] = 270;
			a[2] = 90;
			a[3] = 90;
			a[4] = 180;
			face = v3s16(0,1,-1);
		}
		for (int s=0; s<5; s++) {
			meshgen_rooftri(data,n,p,cnr[s],pos,toptile,selected,a[s],face);
		}
	}
	break;
	default:
		break;
	}
}

void meshgen_leaflike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	bool is_xp = false;
	bool is_xm = false;
	bool is_zp = false;
	bool is_zm = false;
	bool is_xpzm = false;
	bool is_xmzp = false;
	bool is_xmzm = false;

	content_t thiscontent = n.getContent();

	content_t n_xp = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16( 1,0, 0)).getContent();
	content_t n_xm = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1,0, 0)).getContent();
	content_t n_zp = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16( 0,0, 1)).getContent();
	content_t n_zm = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16( 0,0,-1)).getContent();
	content_t n_xpzm = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16( 1,0,-1)).getContent();
	content_t n_xmzp = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1,0, 1)).getContent();
	content_t n_xmzm = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + v3s16(-1,0,-1)).getContent();

	if (n_xp == thiscontent || content_features(n_xp).special_alternate_node == thiscontent)
		is_xp = true;
	if (n_xm == thiscontent || content_features(n_xm).special_alternate_node == thiscontent)
		is_xm = true;
	if (n_zp == thiscontent || content_features(n_zp).special_alternate_node == thiscontent)
		is_zp = true;
	if (n_zm == thiscontent || content_features(n_zm).special_alternate_node == thiscontent)
		is_zm = true;
	if (n_xpzm == thiscontent || content_features(n_xpzm).special_alternate_node == thiscontent)
		is_xpzm = true;
	if (n_xmzp == thiscontent || content_features(n_xmzp).special_alternate_node == thiscontent)
		is_xmzp = true;
	if (n_xmzm == thiscontent || content_features(n_xmzm).special_alternate_node == thiscontent)
		is_xmzm = true;

	// get the tile, with crack if being dug
	TileSpec tile = getNodeTile(n,p,v3s16(0,1,0),data->m_temp_mods);
	TileSpec toptile = getNodeTile(n,p,v3s16(0,-1,0),data->m_temp_mods);

	u8 type = 0;
	s16 angle = 0;

	MapNode abv;

	v3f pos = intToFloat(p, BS);

	if (is_xp && is_xm) {
		if (is_zm || (is_xpzm && is_xmzm) || content_features(n_zm).walkable)
			angle = 180;
	}else if (is_zp && is_zm) {
		if (is_xm || (is_xmzp && is_xmzm) || content_features(n_xm).walkable) {
			angle = 90;
		}else{
			angle = 270;
		}
	}else if (is_xp && is_zp) {
		type = 1;
		angle = 270;
	}else if (is_xp && is_zm) {
		type = 1;
		angle = 180;
	}else if (is_xm && is_zp) {
		type = 1;
	}else if (is_xm && is_zm) {
		type = 1;
		angle = 90;
	}else if (is_xp || is_xm) {
		if (is_zm || (is_xp && is_xpzm) || (is_xm && is_xmzm) || content_features(n_zm).walkable)
			angle = 180;
	}else if (is_zp || is_zm) {
		if (is_xm || (is_zp && is_xmzp) || (is_zm && is_xmzm) || content_features(n_xm).walkable) {
			angle = 90;
		}else{
			angle = 270;
		}
	}else{
		type = 2;
	}
	/*
		0: slope
		1: corner
		2: top cap
	*/
	switch (type) {
	case 0:
	{
		v3f cnr[2][3];
		if (angle == 0) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.5,0.5);
			cnr[1][0] = v3f(0.5,0.5,0.5);
			cnr[1][1] = v3f(-0.5,0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
		}else if (angle == 90) {
			cnr[0][0] = v3f(-0.5,0.5,-0.5);
			cnr[0][1] = v3f(0.5,-0.5,-0.5);
			cnr[0][2] = v3f(0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.5,-0.5,0.5);
			cnr[1][1] = v3f(-0.5,0.5,0.5);
			cnr[1][2] = v3f(-0.5,0.5,-0.5);
		}else if (angle == 180) {
			cnr[0][0] = v3f(-0.5,0.5,-0.5);
			cnr[0][1] = v3f(0.5,0.5,-0.5);
			cnr[0][2] = v3f(0.5,-0.5,0.5);
			cnr[1][0] = v3f(0.5,-0.5,0.5);
			cnr[1][1] = v3f(-0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,0.5,-0.5);
		}else if (angle == 270) {
			cnr[0][0] = v3f(-0.5,-0.5,-0.5);
			cnr[0][1] = v3f(0.5,0.5,-0.5);
			cnr[0][2] = v3f(0.5,0.5,0.5);
			cnr[1][0] = v3f(0.5,0.5,0.5);
			cnr[1][1] = v3f(-0.5,-0.5,0.5);
			cnr[1][2] = v3f(-0.5,-0.5,-0.5);
		}
		s16 a = 180-angle;
		if (a < 0)
			a += 360;
		for (int s=0; s<2; s++) {
			meshgen_leaftri(data,n,p,cnr[s],pos,tile,selected,a);
		}
	}
	break;
	case 1:
	{
		v3f cnr[3];
		s16 a = angle;
		if (angle == 0) {
			cnr[0] = v3f(-0.5,-0.5,-0.5);
			cnr[1] = v3f(0.5,-0.5,0.5);
			cnr[2] = v3f(-0.5,0.5,0.5);
			a = 180;
		}else if (angle == 90) {
			cnr[0] = v3f(-0.5,-0.5,0.5);
			cnr[1] = v3f(0.5,-0.5,-0.5);
			cnr[2] = v3f(-0.5,0.5,-0.5);
			a = 0;
		}else if (angle == 180) {
			cnr[0] = v3f(-0.5,-0.5,-0.5);
			cnr[1] = v3f(0.5,0.5,-0.5);
			cnr[2] = v3f(0.5,-0.5,0.5);
			a = 270;
		}else if (angle == 270) {
			cnr[0] = v3f(-0.5,-0.5,0.5);
			cnr[1] = v3f(0.5,0.5,0.5);
			cnr[2] = v3f(0.5,-0.5,-0.5);
		}
		meshgen_leaftri(data,n,p,cnr,pos,tile,selected,a);
	}
	break;
	case 2:
	{
		v3f cnr[4][3];
		cnr[0][0] = v3f(0.,0.,0.);
		cnr[0][1] = v3f(-0.5,-0.5,-0.5);
		cnr[0][2] = v3f(-0.5,-0.5,0.5);
		cnr[1][0] = v3f(0.,0.,0.);
		cnr[1][1] = v3f(-0.5,-0.5,-0.5);
		cnr[1][2] = v3f(0.5,-0.5,-0.5);
		cnr[2][0] = v3f(0.,0.,0.);
		cnr[2][1] = v3f(0.5,-0.5,-0.5);
		cnr[2][2] = v3f(0.5,-0.5,0.5);
		cnr[3][0] = v3f(0.,0.,0.);
		cnr[3][1] = v3f(-0.5,-0.5,0.5);
		cnr[3][2] = v3f(0.5,-0.5,0.5);
		for (int s=0; s<4; s++) {
			meshgen_leaftri(data,n,p,cnr[s],pos,toptile,selected,(90*s)+90+(180*(!(s%2))));
		}
	}
	break;
	default:;
	}
}

void meshgen_wirelike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected, bool is3d)
{
	MapNode n_plus_y = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(0,1,0));
	MapNode n_minus_x = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(-1,0,0));
	MapNode n_plus_x = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(1,0,0));
	MapNode n_minus_z = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(0,0,-1));
	MapNode n_plus_z = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(0,0,1));
	MapNode n_minus_xy = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(-1,1,0));
	MapNode n_plus_xy = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(1,1,0));
	MapNode n_minus_zy = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(0,1,-1));
	MapNode n_plus_zy = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(0,1,1));
	MapNode n_minus_x_y = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(-1,-1,0));
	MapNode n_plus_x_y = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(1,-1,0));
	MapNode n_minus_z_y = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(0,-1,-1));
	MapNode n_plus_z_y = data->m_vmanip.getNodeNoEx(data->m_blockpos_nodes + p + v3s16(0,-1,1));
	bool x_plus = false;
	bool x_plus_y = false;
	bool x_minus = false;
	bool x_minus_y = false;
	bool z_plus = false;
	bool z_plus_y = false;
	bool z_minus = false;
	bool z_minus_y = false;
	bool y_plus = false;
	// +Y
	if (n_plus_y.getContent() == CONTENT_AIR || content_features(n_plus_y).energy_type != CET_NONE)
		y_plus = true;
	// +X
	if (
		content_features(n_plus_x).energy_type == CET_NONE
		&& content_features(n_plus_x).flammable != 2
	) {
		if (
			y_plus
			&& (
				content_features(n_plus_x).draw_type == CDT_CUBELIKE
				|| content_features(n_plus_x).draw_type == CDT_GLASSLIKE
			)
		) {
			if (content_features(n_plus_xy).energy_type != CET_NONE) {
				x_plus_y = true;
				x_plus = true;
			}
		}else if (
			n_plus_x.getContent() == CONTENT_AIR
			&& content_features(n_plus_x_y).energy_type != CET_NONE
		) {
			x_plus = true;
		}
	}else{
		x_plus = true;
	}
	// -X
	if (content_features(n_minus_x).energy_type == CET_NONE && content_features(n_minus_x).flammable != 2) {
		if (
			y_plus
			&& (
				content_features(n_minus_x).draw_type == CDT_CUBELIKE
				|| content_features(n_minus_x).draw_type == CDT_GLASSLIKE
			)
		) {
			if (content_features(n_minus_xy).energy_type != CET_NONE) {
				x_minus_y = true;
				x_minus = true;
			}
		}else if (
			n_minus_x.getContent() == CONTENT_AIR
			&& content_features(n_minus_x_y).energy_type != CET_NONE
		) {
			x_minus = true;
		}
	}else{
		x_minus = true;
	}
	// +Z
	if (
		content_features(n_plus_z).energy_type == CET_NONE
		&& content_features(n_plus_z).flammable != 2
	) {
		if (
			y_plus
			&& (
				content_features(n_plus_z).draw_type == CDT_CUBELIKE
				|| content_features(n_plus_z).draw_type == CDT_GLASSLIKE
			)
		) {
			if (content_features(n_plus_zy).energy_type != CET_NONE) {
				z_plus_y = true;
				z_plus = true;
			}
		}else if (
			n_plus_z.getContent() == CONTENT_AIR
			&& content_features(n_plus_z_y).energy_type != CET_NONE
		) {
			z_plus = true;
		}
	}else{
		z_plus = true;
	}
	// -Z
	if (
		content_features(n_minus_z).energy_type == CET_NONE
		&& content_features(n_minus_z).flammable != 2
	) {
		if (
			y_plus
			&& (
				content_features(n_minus_z).draw_type == CDT_CUBELIKE
				|| content_features(n_minus_z).draw_type == CDT_GLASSLIKE
			)
		) {
			if (content_features(n_minus_zy).energy_type != CET_NONE) {
				z_minus_y = true;
				z_minus = true;
			}
		}else if (
			n_minus_z.getContent() == CONTENT_AIR
			&& content_features(n_minus_z_y).energy_type != CET_NONE
		) {
			z_minus = true;
		}
	}else{
		z_minus = true;
	}

	static const v3s16 tile_dirs[6] = {
		v3s16(0, 1, 0),
		v3s16(0, -1, 0),
		v3s16(1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16(0, 0, 1),
		v3s16(0, 0, -1)
	};

	TileSpec tiles[6];
	for (int i = 0; i < 6; i++) {
		// Handles facedir rotation for textures
		tiles[i] = getNodeTile(n,p,tile_dirs[i],data->m_temp_mods);
	}

	v3f pos = intToFloat(p, BS);
	u8 cols[4] = {250,64,64,64};
	if (selected) {
		cols[0] = 255;
		cols[1] = 64;
		cols[2] = 64;
		cols[3] = 255;
	}else{
		NodeMetadata *meta = data->m_env->getMap().getNodeMetadata(p+data->m_blockpos_nodes);
		if (meta && meta->getEnergy()) {
			u8 e = meta->getEnergy();
			e = (e*16)-1;
			if (e < 80)
				e = 80;
			cols[0] = 255;
			cols[1] = e;
			cols[2] = e;
			cols[3] = e;
		}
	}

	if (is3d == false) {
		f32 sy = tiles[0].texture.y1()-tiles[0].texture.y0();
		if (!x_plus && !x_minus && !z_plus && !z_minus) {
			{
				video::S3DVertex vertices[4] = {
					video::S3DVertex(-BS/2,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()),
					video::S3DVertex(BS/2,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
					video::S3DVertex(-BS/2,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
			{
				video::S3DVertex vertices[4] = {
					video::S3DVertex(-BS/2,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
					video::S3DVertex(-BS/2,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
		}else{
			if (x_plus) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(0,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()+(sy/2)),
					video::S3DVertex(BS/2,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
					video::S3DVertex(0,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()+(sy/2)),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
			if (x_minus) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(-BS/2,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()),
					video::S3DVertex(0,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()-(sy/2)),
					video::S3DVertex(0,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()-(sy/2)),
					video::S3DVertex(-BS/2,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
			if (z_plus) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(-BS/2,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.49*BS,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.49*BS,0, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()+(sy/2)),
					video::S3DVertex(-BS/2,-0.49*BS,0, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()+(sy/2)),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
			if (z_minus) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(-BS/2,-0.49*BS,0, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()-(sy/2)),
					video::S3DVertex(BS/2,-0.49*BS,0, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()-(sy/2)),
					video::S3DVertex(BS/2,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
					video::S3DVertex(-BS/2,-0.49*BS,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
			if (x_plus_y) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(0.49*BS,-BS/2,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
					video::S3DVertex(0.49*BS,-BS/2,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
					video::S3DVertex(0.49*BS,BS/2,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
					video::S3DVertex(0.49*BS,BS/2,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
			if (x_minus_y) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(-0.49*BS,-BS/2,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
					video::S3DVertex(-0.49*BS,-BS/2,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
					video::S3DVertex(-0.49*BS,BS/2,-BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
					video::S3DVertex(-0.49*BS,BS/2,BS/2, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
			if (z_plus_y) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(BS/2,-BS/2,0.49*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
					video::S3DVertex(-BS/2,-BS/2,0.49*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
					video::S3DVertex(-BS/2,BS/2,0.49*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
					video::S3DVertex(BS/2,BS/2,0.49*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
			if (z_minus_y) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(-BS/2,-BS/2,-0.49*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-BS/2,-0.49*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y1()),
					video::S3DVertex(BS/2,BS/2,-0.49*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x1(), tiles[0].texture.y0()),
					video::S3DVertex(-BS/2,BS/2,-0.49*BS, 0,0,0, video::SColor(255,255,255,255), tiles[0].texture.x0(), tiles[0].texture.y0()),
				};
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				std::vector<video::SColor> colours[18];
				meshgen_custom_lights(colours,cols[0],cols[1],cols[2],cols[3],4);
				data->append(tiles[0].getMaterial(), vertices, 4, indices, 6, colours);
			}
		}
		return;
	}

	std::vector<aabb3f> boxes;
	if (!x_plus && !x_minus && !z_plus && !z_minus) {
		boxes.push_back(aabb3f(-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,-0.4375*BS,0.125*BS));
	}else{
		if (x_plus) {
			boxes.push_back(aabb3f(0.,-0.5*BS,-0.0625*BS,0.5*BS,-0.4375*BS,0.0625*BS));
		}
		if (x_minus) {
			boxes.push_back(aabb3f(-0.5*BS,-0.5*BS,-0.0625*BS,0.,-0.4375*BS,0.0625*BS));
		}
		if (z_plus) {
			boxes.push_back(aabb3f(-0.0625*BS,-0.5*BS,0.,0.0625*BS,-0.4375*BS,0.5*BS));
		}
		if (z_minus) {
			boxes.push_back(aabb3f(-0.0625*BS,-0.5*BS,-0.5*BS,0.0625*BS,-0.4375*BS,0.));
		}
		if (x_plus_y) {
			boxes.push_back(aabb3f(0.4375*BS,-0.4375*BS,-0.0625*BS,0.5*BS,0.5625*BS,0.0625*BS));
		}
		if (x_minus_y) {
			boxes.push_back(aabb3f(-0.5*BS,-0.4375*BS,-0.0625*BS,-0.4375*BS,0.5625*BS,0.0625*BS));
		}
		if (z_plus_y) {
			boxes.push_back(aabb3f(-0.0625*BS,-0.4375*BS,0.4375*BS,0.0625*BS,0.5625*BS,0.5*BS));
		}
		if (z_minus_y) {
			boxes.push_back(aabb3f(-0.0625*BS,-0.4375*BS,-0.5*BS,0.0625*BS,0.5625*BS,-0.4375*BS));
		}
		u8 cnt = x_plus+x_minus+z_plus+z_minus;
		if (
			cnt > 2
			|| (
				cnt == 2
				&& (
					(x_plus && z_plus)
					|| (x_minus && z_plus)
					|| (x_plus && z_minus)
					|| (x_minus && z_minus)
				)
			)
		) {
			boxes.push_back(aabb3f(-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,-0.375*BS,0.125*BS));
		}
	}
	for (std::vector<aabb3f>::iterator i = boxes.begin(); i != boxes.end(); i++) {
		aabb3f box = *i;

		// Compute texture coords
		f32 tx1 = (i->MinEdge.X/BS)+0.5;
		f32 ty1 = (i->MinEdge.Y/BS)+0.5;
		f32 tz1 = (i->MinEdge.Z/BS)+0.5;
		f32 tx2 = (i->MaxEdge.X/BS)+0.5;
		f32 ty2 = (i->MaxEdge.Y/BS)+0.5;
		f32 tz2 = (i->MaxEdge.Z/BS)+0.5;
		f32 txc[24] = {
			// up
			tx1, 1-tz2, tx2, 1-tz1,
			// down
			tx1, tz1, tx2, tz2,
			// right
			tz1, 1-ty2, tz2, 1-ty1,
			// left
			1-tz2, 1-ty2, 1-tz1, 1-ty1,
			// back
			1-tx2, 1-ty2, 1-tx1, 1-ty1,
			// front
			tx1, 1-ty2, tx2, 1-ty1,
		};
		meshgen_cuboid(data,n,p, pos, box, tiles, 6, selected, txc,v3s16(0,0,0),v3f(0,0,0), cols);
	}
}

void meshgen_stairlike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	static v3s16 faces[6] = {
		v3s16( 0, 1, 0),
		v3s16( 0,-1, 0),
		v3s16( 1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16( 0, 0, 1),
		v3s16( 0, 0,-1),
	};
	v3f pos = intToFloat(p, BS);
	s16 rot = n.getRotationAngle();

	// remove rotation from the node, we'll do it ourselves
	{
		content_t c = n.getContent();
		n.param1 = 0;
		n.setContent(c);
	}

	TileSpec tiles[6];
	NodeMetadata *meta = data->m_env->getMap().getNodeMetadata(p+data->m_blockpos_nodes);
	for (int i=0; i<6; i++) {
		// Handles facedir rotation for textures
		tiles[i] = getNodeTile(n,p,faces[i],data->m_temp_mods,meta);
	}

	bool urot = (n.getContent() >= CONTENT_SLAB_STAIR_UD_MIN && n.getContent() <= CONTENT_SLAB_STAIR_UD_MAX);

	video::S3DVertex vertices[6][16] = {
		{ // up
			video::S3DVertex(-0.5*BS, 0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,0.),
			video::S3DVertex( 0.5*BS, 0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,0.),
			video::S3DVertex( 0.5*BS, 0.5*BS , 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.25),
			video::S3DVertex(-0.5*BS, 0.5*BS , 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.25),

			video::S3DVertex(-0.5*BS, 0.25*BS, 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.25),
			video::S3DVertex( 0.5*BS, 0.25*BS, 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.25),
			video::S3DVertex( 0.5*BS, 0.25*BS, 0.     , 0,0,0, video::SColor(255,255,255,255), 1.,0.5),
			video::S3DVertex(-0.5*BS, 0.25*BS, 0.     , 0,0,0, video::SColor(255,255,255,255), 0.,0.5),

			video::S3DVertex(-0.5*BS, 0.     , 0.     , 0,0,0, video::SColor(255,255,255,255), 0.,0.5),
			video::S3DVertex( 0.5*BS, 0.     , 0.     , 0,0,0, video::SColor(255,255,255,255), 1.,0.5),
			video::S3DVertex( 0.5*BS, 0.     ,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.75),
			video::S3DVertex(-0.5*BS, 0.     ,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.75),

			video::S3DVertex(-0.5*BS,-0.25*BS,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.75),
			video::S3DVertex( 0.5*BS,-0.25*BS,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.75),
			video::S3DVertex( 0.5*BS,-0.25*BS,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,1.),
			video::S3DVertex(-0.5*BS,-0.25*BS,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,1.)
		},{ // down
			video::S3DVertex(-0.5*BS,-0.5*BS ,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,0.),
			video::S3DVertex( 0.5*BS,-0.5*BS ,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,0.),
			video::S3DVertex( 0.5*BS,-0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,1.),
			video::S3DVertex(-0.5*BS,-0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,1.)
		},{ // right
			video::S3DVertex( 0.5*BS, 0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,0.),
			video::S3DVertex( 0.5*BS,-0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,1.),
			video::S3DVertex( 0.5*BS,-0.5*BS ,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,1.),

			video::S3DVertex( 0.5*BS, 0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,0.),
			video::S3DVertex( 0.5*BS, 0.25*BS, 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.75,0.25),
			video::S3DVertex( 0.5*BS, 0.5*BS , 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.75,0.),

			video::S3DVertex( 0.5*BS, 0.25*BS, 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.75,0.25),
			video::S3DVertex( 0.5*BS, 0.     , 0.     , 0,0,0, video::SColor(255,255,255,255), 0.5,0.5),
			video::S3DVertex( 0.5*BS, 0.25*BS, 0.     , 0,0,0, video::SColor(255,255,255,255), 0.5,0.25),

			video::S3DVertex( 0.5*BS, 0.     , 0.     , 0,0,0, video::SColor(255,255,255,255), 0.5,0.5),
			video::S3DVertex( 0.5*BS,-0.25*BS,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.25,0.75),
			video::S3DVertex( 0.5*BS, 0.     ,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.5,0.75),

			video::S3DVertex( 0.5*BS,-0.25*BS,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.25,0.75),
			video::S3DVertex( 0.5*BS,-0.5*BS ,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,1.),
			video::S3DVertex( 0.5*BS,-0.25*BS,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,0.75)
		},{ // left
			video::S3DVertex(-0.5*BS, 0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,0.),
			video::S3DVertex(-0.5*BS,-0.5*BS ,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,1.),
			video::S3DVertex(-0.5*BS,-0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,1.),

			video::S3DVertex(-0.5*BS, 0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,0.),
			video::S3DVertex(-0.5*BS, 0.5*BS , 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.25,0.),
			video::S3DVertex(-0.5*BS, 0.25*BS, 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.25,0.25),

			video::S3DVertex(-0.5*BS, 0.25*BS, 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.25,0.25),
			video::S3DVertex(-0.5*BS, 0.25*BS, 0.     , 0,0,0, video::SColor(255,255,255,255), 0.5,0.25),
			video::S3DVertex(-0.5*BS, 0.     , 0.     , 0,0,0, video::SColor(255,255,255,255), 0.5,0.5),

			video::S3DVertex(-0.5*BS, 0.     , 0.     , 0,0,0, video::SColor(255,255,255,255), 0.5,0.5),
			video::S3DVertex(-0.5*BS, 0.     ,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.75,0.5),
			video::S3DVertex(-0.5*BS,-0.25*BS,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.75,0.75),

			video::S3DVertex(-0.5*BS,-0.25*BS,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.75,0.75),
			video::S3DVertex(-0.5*BS,-0.25*BS,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,0.75),
			video::S3DVertex(-0.5*BS,-0.5*BS ,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,1.)
		},{ // back
			video::S3DVertex( 0.5*BS, 0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,0.),
			video::S3DVertex(-0.5*BS, 0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,0.),
			video::S3DVertex(-0.5*BS,-0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,1.),
			video::S3DVertex( 0.5*BS,-0.5*BS , 0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,1.)
		},{ // front
			video::S3DVertex(-0.5*BS, 0.5*BS , 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.),
			video::S3DVertex( 0.5*BS, 0.5*BS , 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.),
			video::S3DVertex( 0.5*BS, 0.25*BS, 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.25),
			video::S3DVertex(-0.5*BS, 0.25*BS, 0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.25),

			video::S3DVertex(-0.5*BS, 0.25*BS, 0.     , 0,0,0, video::SColor(255,255,255,255), 0.,0.25),
			video::S3DVertex( 0.5*BS, 0.25*BS, 0.     , 0,0,0, video::SColor(255,255,255,255), 1.,0.25),
			video::S3DVertex( 0.5*BS, 0.     , 0.     , 0,0,0, video::SColor(255,255,255,255), 1.,0.5),
			video::S3DVertex(-0.5*BS, 0.     , 0.     , 0,0,0, video::SColor(255,255,255,255), 0.,0.5),

			video::S3DVertex(-0.5*BS, 0.     ,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.5),
			video::S3DVertex( 0.5*BS, 0.     ,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.5),
			video::S3DVertex( 0.5*BS,-0.25*BS,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.75),
			video::S3DVertex(-0.5*BS,-0.25*BS,-0.25*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.75),

			video::S3DVertex(-0.5*BS,-0.25*BS,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,0.75),
			video::S3DVertex( 0.5*BS,-0.25*BS,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,0.75),
			video::S3DVertex( 0.5*BS,-0.5*BS ,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 1.,1.),
			video::S3DVertex(-0.5*BS,-0.5*BS ,-0.5*BS , 0,0,0, video::SColor(255,255,255,255), 0.,1.)
		}
	};

	u16 indices[6][24] = {
		{ // up
			0,1,2,2,3,0,
			4,5,6,6,7,4,
			8,9,10,10,11,8,
			12,13,14,14,15,12
		},{ // down
			0,1,2,2,3,0
		},{ // right
			0,1,2,3,4,5,6,7,8,9,10,11,12,13,14
		},{ // left
			0,1,2,3,4,5,6,7,8,9,10,11,12,13,14
		},{ // back
			0,1,2,2,3,0
		},{ // front
			0,1,2,2,3,0,
			4,5,6,6,7,4,
			8,9,10,10,11,8,
			12,13,14,14,15,12
		}
	};
	v3s16 back(0,0,1);
	v3s16 front(0,0,-1);
	v3s16 left(-1,0,0);
	v3s16 right(1,0,0);
	back.rotateXZBy(rot);
	front.rotateXZBy(rot);
	left.rotateXZBy(rot);
	right.rotateXZBy(rot);
	if (urot) {
		v3s16 r = left;
		left = right;
		right = r;
	}
	MapNode nb = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + back);
	MapNode nf = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + front);
	MapNode nl = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + left);
	MapNode nr = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + right);
	ContentFeatures *fb = &content_features(nb);
	ContentFeatures *ff = &content_features(nf);
	ContentFeatures *fl = &content_features(nl);
	ContentFeatures *fr = &content_features(nr);
	s16 vcounts[6] = {16,4,15,15,4,16};
	s16 icounts[6] = {24,6,15,15,6,24};
	bool force_sides = false;
	if (
		ff->draw_type == CDT_SLABLIKE
		|| (
			ff->draw_type == CDT_STAIRLIKE
			&& (
				fl->draw_type == CDT_SLABLIKE
				|| fl->draw_type == CDT_STAIRLIKE
			) && (
				fr->draw_type == CDT_SLABLIKE
				|| fr->draw_type == CDT_STAIRLIKE
			)
		)
	) {
		// slab connection
		vcounts[0] = 12;
		icounts[0] = 18;
		vertices[0][8] =  video::S3DVertex(-0.5*BS, 0.    , 0.,     0,0,0, video::SColor(255,255,255,255), 0.,0.5);
		vertices[0][9] =  video::S3DVertex( 0.5*BS, 0.    , 0.,     0,0,0, video::SColor(255,255,255,255), 1.,0.5),
		vertices[0][10] = video::S3DVertex( 0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,1.0),
		vertices[0][11] = video::S3DVertex(-0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,1.0),

		vcounts[2] = 12;
		icounts[2] = 12;
		vertices[2][9] =  video::S3DVertex( 0.5*BS, 0.    , 0.,     0,0,0, video::SColor(255,255,255,255), 0.5,0.5);
		vertices[2][10] = video::S3DVertex( 0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,1.);
		vertices[2][11] = video::S3DVertex( 0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.5);

		vcounts[3] = 12;
		icounts[3] = 12;
		vertices[3][9] =  video::S3DVertex(-0.5*BS, 0.    , 0.,     0,0,0, video::SColor(255,255,255,255), 0.5,0.5);
		vertices[3][10] = video::S3DVertex(-0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.0,0.5);
		vertices[3][11] = video::S3DVertex(-0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.0,1.0);

		vcounts[5] = 12;
		icounts[5] = 18;
		vertices[5][8] =  video::S3DVertex(-0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.5);
		vertices[5][9] =  video::S3DVertex( 0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.5);
		vertices[5][10] = video::S3DVertex( 0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,1.);
		vertices[5][11] = video::S3DVertex(-0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,1.);
		force_sides = true;
	}
	// don't draw unseen faces
	bool skips[6] = {false,false,false,false,false,false};
	if (
		fb->draw_type == CDT_CUBELIKE
		|| (
			fb->draw_type == CDT_STAIRLIKE
			&& (
				nb.getRotationAngle() == rot+180
				|| nb.getRotationAngle() == rot-180
			)
		)
	)
		skips[4] = true;
	if (!force_sides && fl->draw_type == CDT_STAIRLIKE && nl.getRotationAngle() == rot)
		skips[3] = true;
	if (!force_sides && fr->draw_type == CDT_STAIRLIKE && nr.getRotationAngle() == rot)
		skips[2] = true;

	if (urot) {
		if (rot) {
			for (int i=0; i<6; i++) {
				if (skips[i])
					continue;
				v3s16 f = faces[i];
				f.rotateXYBy(180);
				f.rotateXZBy(rot);
				for (int j=0; j<vcounts[i]; j++) {
					vertices[i][j].Pos.rotateXYBy(180);
					vertices[i][j].Pos.rotateXZBy(rot);
					vertices[i][j].TCoords *= tiles[i].texture.size;
					vertices[i][j].TCoords += tiles[i].texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,vcounts[i]);
				}else{
					meshgen_lights(data,n,p,colours,255,f,vcounts[i],vertices[i]);
				}

				for (int j=0; j<vcounts[i]; j++) {
					vertices[i][j].Pos += pos;
				}

				data->append(tiles[i].getMaterial(), vertices[i], vcounts[i], indices[i], icounts[i], colours);
			}
		}else{
			for (int i=0; i<6; i++) {
				if (skips[i])
					continue;
				v3s16 f = faces[i];
				f.rotateXYBy(180);
				for (int j=0; j<vcounts[i]; j++) {
					vertices[i][j].Pos.rotateXYBy(180);
					vertices[i][j].TCoords *= tiles[i].texture.size;
					vertices[i][j].TCoords += tiles[i].texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,vcounts[i]);
				}else{
					meshgen_lights(data,n,p,colours,255,f,vcounts[i],vertices[i]);
				}

				for (int j=0; j<vcounts[i]; j++) {
					vertices[i][j].Pos += pos;
				}

				data->append(tiles[i].getMaterial(), vertices[i], vcounts[i], indices[i], icounts[i], colours);
			}
		}
	}else if (rot) {
		for (int i=0; i<6; i++) {
			if (skips[i])
				continue;
			v3s16 f = faces[i];
			f.rotateXZBy(rot);
			for (int j=0; j<vcounts[i]; j++) {
				vertices[i][j].Pos.rotateXZBy(rot);
				vertices[i][j].TCoords *= tiles[i].texture.size;
				vertices[i][j].TCoords += tiles[i].texture.pos;
			}
			std::vector<video::SColor> colours[18];
			if (selected) {
				meshgen_selected_lights(colours,255,vcounts[i]);
			}else{
				meshgen_lights(data,n,p,colours,255,f,vcounts[i],vertices[i]);
			}

			for (int j=0; j<vcounts[i]; j++) {
				vertices[i][j].Pos += pos;
			}

			data->append(tiles[i].getMaterial(), vertices[i], vcounts[i], indices[i], icounts[i], colours);
		}
	}else{
		for (int i=0; i<6; i++) {
			if (skips[i])
				continue;
			for (int j=0; j<vcounts[i]; j++) {
				vertices[i][j].TCoords *= tiles[i].texture.size;
				vertices[i][j].TCoords += tiles[i].texture.pos;
			}
			std::vector<video::SColor> colours[18];
			if (selected) {
				meshgen_selected_lights(colours,255,vcounts[i]);
			}else{
				meshgen_lights(data,n,p,colours,255,faces[i],vcounts[i],vertices[i]);
			}

			for (int j=0; j<vcounts[i]; j++) {
				vertices[i][j].Pos += pos;
			}

			data->append(tiles[i].getMaterial(), vertices[i], vcounts[i], indices[i], icounts[i], colours);
		}
	}
}

void meshgen_slablike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	static v3s16 faces[6] = {
		v3s16( 0, 1, 0),
		v3s16( 0,-1, 0),
		v3s16( 1, 0, 0),
		v3s16(-1, 0, 0),
		v3s16( 0, 0, 1),
		v3s16( 0, 0,-1),
	};
	static v3s16 ufaces[6] = {
		v3s16( 0,-1, 0),
		v3s16( 0, 1, 0),
		v3s16(-1, 0, 0),
		v3s16( 1, 0, 0),
		v3s16( 0, 0, 1),
		v3s16( 0, 0,-1),
	};

	TileSpec tiles[6];
	NodeMetadata *meta = data->m_env->getMap().getNodeMetadata(p+data->m_blockpos_nodes);
	for (int i = 0; i < 6; i++) {
		// Handles facedir rotation for textures
		tiles[i] = getNodeTile(n,p,faces[i],data->m_temp_mods,meta);
	}
	v3f pos = intToFloat(p, BS);

	bool urot = (n.getContent() >= CONTENT_SLAB_STAIR_UD_MIN && n.getContent() <= CONTENT_SLAB_STAIR_UD_MAX);
	float tex_v = 0.;
	if (urot)
		tex_v = 0.5;
	// flip lighting
	video::S3DVertex vertices[6][4] = {
		{ // up
			video::S3DVertex(-0.5*BS, 0.    , 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.),
			video::S3DVertex( 0.5*BS, 0.    , 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.),
			video::S3DVertex( 0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,1.),
			video::S3DVertex(-0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,1.)
		},{ // down
			video::S3DVertex(-0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.),
			video::S3DVertex( 0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.),
			video::S3DVertex( 0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,1.),
			video::S3DVertex(-0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,1.)
		},{ // right
			video::S3DVertex( 0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.+tex_v),
			video::S3DVertex( 0.5*BS, 0.    , 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.+tex_v),
			video::S3DVertex( 0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,.5+tex_v),
			video::S3DVertex( 0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,.5+tex_v)
		},{ // left
			video::S3DVertex(-0.5*BS, 0.    , 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.+tex_v),
			video::S3DVertex(-0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.+tex_v),
			video::S3DVertex(-0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,.5+tex_v),
			video::S3DVertex(-0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,.5+tex_v)
		},{ // back
			video::S3DVertex( 0.5*BS, 0.    , 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.+tex_v),
			video::S3DVertex(-0.5*BS, 0.    , 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.+tex_v),
			video::S3DVertex(-0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,.5+tex_v),
			video::S3DVertex( 0.5*BS,-0.5*BS, 0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,.5+tex_v)
		},{ // front
			video::S3DVertex(-0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,0.+tex_v),
			video::S3DVertex( 0.5*BS, 0.    ,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,0.+tex_v),
			video::S3DVertex( 0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 1.,.5+tex_v),
			video::S3DVertex(-0.5*BS,-0.5*BS,-0.5*BS, 0,0,0, video::SColor(255,255,255,255), 0.,.5+tex_v)
		}
	};
	u16 indices[6] = {0,1,2,2,3,0};
	v3s16 back(0,0,1);
	v3s16 front(0,0,-1);
	v3s16 left(-1,0,0);
	v3s16 right(1,0,0);
	v3s16 under(0,-1,0);
	if (urot) {
		left = v3s16(1,0,0);
		right = v3s16(-1,0,0);
		under = v3s16(0,1,0);
	}
	MapNode nb = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + back);
	MapNode nf = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + front);
	MapNode nl = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + left);
	MapNode nr = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + right);
	MapNode nu = data->m_vmanip.getNodeRO(data->m_blockpos_nodes + p + under);
	// don't draw unseen faces
	bool skips[6] = {false,false,false,false,false,false};
	if (
		content_features(nu).draw_type == CDT_CUBELIKE
		|| (
			content_features(nu).draw_type == CDT_SLABLIKE
			&& (
				(
					urot && nu.getContent() == (n.getContent()|CONTENT_SLAB_STAIR_UD_MAX)
				) || (
					!urot && (nu.getContent()|CONTENT_SLAB_STAIR_UD_MAX) == n.getContent()
				)
			)
		)
	)
		skips[1] = true;
	if (nr.getContent() == n.getContent() || content_features(nr).draw_type == CDT_CUBELIKE)
		skips[2] = true;
	if (nl.getContent() == n.getContent() || content_features(nl).draw_type == CDT_CUBELIKE)
		skips[3] = true;
	if (nb.getContent() == n.getContent() || content_features(nb).draw_type == CDT_CUBELIKE)
		skips[4] = true;
	if (nf.getContent() == n.getContent() || content_features(nf).draw_type == CDT_CUBELIKE)
		skips[5] = true;
	if (urot) {
		for (int i=0; i<6; i++) {
			if (skips[i])
				continue;
			for (int j=0; j<4; j++) {
				vertices[i][j].Pos.rotateXYBy(180);
				vertices[i][j].TCoords *= tiles[i].texture.size;
				vertices[i][j].TCoords += tiles[i].texture.pos;
			}
			std::vector<video::SColor> colours[18];
			if (selected) {
				meshgen_selected_lights(colours,255,4);
			}else{
				meshgen_lights(data,n,p,colours,255,ufaces[i],4,vertices[i]);
			}

			for (int j=0; j<4; j++) {
				vertices[i][j].Pos += pos;
			}

			data->append(tiles[i].getMaterial(), vertices[i], 4, indices, 6, colours);
		}
	}else{
		for (int i=0; i<6; i++) {
			if (skips[i])
				continue;
			for (int j=0; j<4; j++) {
				vertices[i][j].TCoords *= tiles[i].texture.size;
				vertices[i][j].TCoords += tiles[i].texture.pos;
			}
			std::vector<video::SColor> colours[18];
			if (selected) {
				meshgen_selected_lights(colours,255,4);
			}else{
				meshgen_lights(data,n,p,colours,255,faces[i],4,vertices[i]);
			}

			for (int j=0; j<4; j++) {
				vertices[i][j].Pos += pos;
			}

			data->append(tiles[i].getMaterial(), vertices[i], 4, indices, 6, colours);
		}
	}
}

void meshgen_trunklike(MeshMakeData *data, v3s16 p, MapNode &n, bool selected)
{
	bool x_plus = false;
	bool x_plus_any = false;
	bool y_plus = false;
	bool y_plus_any = false;
	bool z_plus = false;
	bool z_plus_any = false;
	bool x_minus = false;
	bool x_minus_any = false;
	bool y_minus = false;
	bool y_minus_any = false;
	bool z_minus = false;
	bool z_minus_any = false;
	content_t n2;
	v3s16 n2p;
	ContentFeatures *f2;

	content_t thiscontent = n.getContent();

	n2p = data->m_blockpos_nodes + p + v3s16(1,0,0);
	n2 = data->m_vmanip.getNodeRO(n2p).getContent();
	f2 = &content_features(n2);
	if (n2 == thiscontent) {
		x_plus = true;
		x_plus_any = true;
	}else if (f2->draw_type == CDT_CUBELIKE) {
		x_plus_any = true;
	}

	n2p = data->m_blockpos_nodes + p + v3s16(0,1,0);
	n2 = data->m_vmanip.getNodeRO(n2p).getContent();
	f2 = &content_features(n2);
	if (n2 == thiscontent) {
		y_plus = true;
		y_plus_any = true;
	}else if (f2->draw_type == CDT_CUBELIKE) {
		y_plus_any = true;
	}

	n2p = data->m_blockpos_nodes + p + v3s16(0,0,1);
	n2 = data->m_vmanip.getNodeRO(n2p).getContent();
	f2 = &content_features(n2);
	if (n2 == thiscontent) {
		z_plus = true;
		z_plus_any = true;
	}else if (f2->draw_type == CDT_CUBELIKE) {
		z_plus_any = true;
	}

	n2p = data->m_blockpos_nodes + p + v3s16(-1,0,0);
	n2 = data->m_vmanip.getNodeRO(n2p).getContent();
	f2 = &content_features(n2);
	if (n2 == thiscontent) {
		x_minus = true;
		x_minus_any = true;
	}else if (f2->draw_type == CDT_CUBELIKE) {
		x_minus_any = true;
	}

	n2p = data->m_blockpos_nodes + p + v3s16(0,-1,0);
	n2 = data->m_vmanip.getNodeRO(n2p).getContent();
	f2 = &content_features(n2);
	if (n2 == thiscontent) {
		y_minus = true;
		y_minus_any = true;
	}else if (f2->draw_type == CDT_CUBELIKE) {
		y_minus_any = true;
	}

	n2p = data->m_blockpos_nodes + p + v3s16(0,0,-1);
	n2 = data->m_vmanip.getNodeRO(n2p).getContent();
	f2 = &content_features(n2);
	if (n2 == thiscontent) {
		z_minus = true;
		z_minus_any = true;
	}else if (f2->draw_type == CDT_CUBELIKE) {
		z_minus_any = true;
	}
	TileSpec tile = getNodeTile(n,p,v3s16(1,0,0),data->m_temp_mods);
	TileSpec endtile = getNodeTile(n,p,v3s16(0,1,0),data->m_temp_mods);
	video::S3DVertex vertices[10] = {
		video::S3DVertex(0       ,-BS*0.5,BS*0.499, 0,0,0, video::SColor(255,255,255,255), 0.125, 0.),
		video::S3DVertex(BS*0.125,-BS*0.5,BS*0.499, 0,0,0, video::SColor(255,255,255,255), 0.25, 0.),
		video::S3DVertex(BS*0.375,-BS*0.5,BS*0.375, 0,0,0, video::SColor(255,255,255,255), 0.5, 0.),
		video::S3DVertex(BS*0.499,-BS*0.5,BS*0.125, 0,0,0, video::SColor(255,255,255,255), 0.75, 0.),
		video::S3DVertex(BS*0.499,-BS*0.5,0       , 0,0,0, video::SColor(255,255,255,255), 0.625, 0.),

		video::S3DVertex(0       , BS*0.5,BS*0.499, 0,0,0, video::SColor(255,255,255,255), 0.125, 1.0),
		video::S3DVertex(BS*0.125, BS*0.5,BS*0.499, 0,0,0, video::SColor(255,255,255,255), 0.25, 1.0),
		video::S3DVertex(BS*0.375, BS*0.5,BS*0.375, 0,0,0, video::SColor(255,255,255,255), 0.5, 1.0),
		video::S3DVertex(BS*0.499, BS*0.5,BS*0.125, 0,0,0, video::SColor(255,255,255,255), 0.75, 1.0),
		video::S3DVertex(BS*0.499, BS*0.5,0       , 0,0,0, video::SColor(255,255,255,255), 0.625, 1.0)
	};
	video::S3DVertex branch_vertices[10] = {
		video::S3DVertex(0       ,BS*0.125,BS*0.499, 0,0,0, video::SColor(255,255,255,255), 0.125, 0.625),
		video::S3DVertex(BS*0.125,BS*0.125,BS*0.499, 0,0,0, video::SColor(255,255,255,255), 0.25, 0.625),
		video::S3DVertex(BS*0.375,BS*0.125,BS*0.375, 0,0,0, video::SColor(255,255,255,255), 0.5, 0.625),
		video::S3DVertex(BS*0.499,BS*0.125,BS*0.125, 0,0,0, video::SColor(255,255,255,255), 0.75, 0.625),
		video::S3DVertex(BS*0.499,BS*0.125,0       , 0,0,0, video::SColor(255,255,255,255), 0.625, 0.625),

		video::S3DVertex(0       ,BS*0.5  ,BS*0.499, 0,0,0, video::SColor(255,255,255,255), 0.125, 1.0),
		video::S3DVertex(BS*0.125,BS*0.5  ,BS*0.499, 0,0,0, video::SColor(255,255,255,255), 0.25, 1.0),
		video::S3DVertex(BS*0.375,BS*0.5  ,BS*0.375, 0,0,0, video::SColor(255,255,255,255), 0.5, 1.0),
		video::S3DVertex(BS*0.499,BS*0.5  ,BS*0.125, 0,0,0, video::SColor(255,255,255,255), 0.75, 1.0),
		video::S3DVertex(BS*0.499,BS*0.5  ,0       , 0,0,0, video::SColor(255,255,255,255), 0.625, 1.0)
	};
	u16 indices[24] = {0,1,6,0,6,5,1,2,7,1,7,6,2,3,8,2,8,7,3,4,9,3,9,8};
	video::S3DVertex end_vertices[6] = {
		video::S3DVertex(BS*0.5  ,BS*0.5,0       , 0,0,0, video::SColor(255,255,255,255), 0.5, 0.),
		video::S3DVertex(BS*0.5  ,BS*0.5,BS*0.125, 0,0,0, video::SColor(255,255,255,255), 0.625, 0.),
		video::S3DVertex(BS*0.375,BS*0.5,BS*0.375, 0,0,0, video::SColor(255,255,255,255), 0.875, 0.125),
		video::S3DVertex(BS*0.125,BS*0.5,BS*0.5  , 0,0,0, video::SColor(255,255,255,255), 1.0, 0.375),
		video::S3DVertex(0       ,BS*0.5,BS*0.5  , 0,0,0, video::SColor(255,255,255,255), 1.0, 0.5),
		video::S3DVertex(0       ,BS*0.5,0       , 0,0,0, video::SColor(255,255,255,255), 0.5, 0.5)
	};
	u16 end_indices[12] = {5,1,0,5,2,1,5,3,2,5,4,3};
	u16 rots[4] = {0,90,180,270};
	v3s16 faces[3] = {
		v3s16( 0, 0,-1),
		v3s16(-1, 0,-1),
		v3s16(-1, 0, 0)
	};

	v3f pos = intToFloat(p,BS);

	if (y_plus || y_minus) { /* vertical trunk */
		for (u16 j=0; j<4; j++) {
			video::S3DVertex v[10];
			for (u16 i=0; i<10; i++) {
				v[i] = vertices[i];
				v[i].Pos.rotateXZBy(rots[j]);
				v[i].TCoords *= tile.texture.size;
				v[i].TCoords += tile.texture.pos;
			}
			std::vector<video::SColor> colours[18];
			if (selected) {
				meshgen_selected_lights(colours,255,10);
			}else{
				v3s16 f[3];
				for (u16 i=0; i<3; i++) {
					f[i] = faces[i];
					f[i].rotateXZBy(rots[j]);
				}
				meshgen_lights(data,n,p,colours,255,f[0],2,v);
				meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
				meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
				meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
				meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
				meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
			}

			for (int k=0; k<10; k++) {
				v[k].Pos += pos;
			}

			data->append(tile.getMaterial(), v, 10, indices, 24, colours);
		}
		if (!y_plus_any) {
			for (u16 j=0; j<4; j++) {
				video::S3DVertex v[6];
				for (u16 i=0; i<6; i++) {
					v[i] = end_vertices[i];
					v[i].Pos.rotateXZBy(rots[j]);
					v[i].TCoords *= endtile.texture.size;
					v[i].TCoords += endtile.texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,6);
				}else{
					meshgen_lights(data,n,p,colours,255,v3s16(0,1,0),6,v);
				}

				for (int k=0; k<6; k++) {
					v[k].Pos += pos;
				}

				data->append(endtile.getMaterial(), v, 6, end_indices, 12, colours);
			}
		}
		if (!y_minus_any) {
			for (u16 j=0; j<4; j++) {
				video::S3DVertex v[6];
				for (u16 i=0; i<6; i++) {
					v[i] = end_vertices[i];
					v[i].Pos.rotateXYBy(180);
					v[i].Pos.rotateXZBy(rots[j]);
					v[i].TCoords *= endtile.texture.size;
					v[i].TCoords += endtile.texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,6);
				}else{
					meshgen_lights(data,n,p,colours,255,v3s16(0,-1,0),6,v);
				}

				for (int k=0; k<6; k++) {
					v[k].Pos += pos;
				}

				data->append(endtile.getMaterial(), v, 6, end_indices, 12, colours);
			}
		}
		if (x_plus) {
			for (u16 j=0; j<4; j++) {
				video::S3DVertex v[10];
				for (u16 i=0; i<10; i++) {
					v[i] = branch_vertices[i];
					v[i].Pos += v3f(0,-BS*0.625,0);
					v[i].Pos.rotateXYBy(90);
					v[i].Pos.rotateYZBy(rots[j]);
					v[i].TCoords *= tile.texture.size;
					v[i].TCoords += tile.texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,10);
				}else{
					v3s16 f[3];
					for (u16 i=0; i<3; i++) {
						f[i] = faces[i];
						f[i].rotateXYBy(90);
						f[i].rotateYZBy(rots[j]);
					}
					meshgen_lights(data,n,p,colours,255,f[0],2,v);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
					meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
				}

				for (int k=0; k<10; k++) {
					v[k].Pos += pos;
				}

				data->append(tile.getMaterial(), v, 10, indices, 24, colours);
			}
		}
		if (x_minus) {
			for (u16 j=0; j<4; j++) {
				video::S3DVertex v[10];
				for (u16 i=0; i<10; i++) {
					v[i] = branch_vertices[i];
					v[i].Pos.rotateXYBy(90);
					v[i].Pos.rotateYZBy(rots[j]);
					v[i].TCoords *= tile.texture.size;
					v[i].TCoords += tile.texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,10);
				}else{
					v3s16 f[3];
					for (u16 i=0; i<3; i++) {
						f[i] = faces[i];
						f[i].rotateXYBy(90);
						f[i].rotateYZBy(rots[j]);
					}
					meshgen_lights(data,n,p,colours,255,f[0],2,v);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
					meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
				}

				for (int k=0; k<10; k++) {
					v[k].Pos += pos;
				}

				data->append(tile.getMaterial(), v, 10, indices, 24, colours);
			}
		}
		if (z_plus) {
			for (u16 j=0; j<4; j++) {
				video::S3DVertex v[10];
				for (u16 i=0; i<10; i++) {
					v[i] = branch_vertices[i];
					v[i].Pos.rotateYZBy(90);
					v[i].Pos.rotateXYBy(rots[j]);
					v[i].TCoords *= tile.texture.size;
					v[i].TCoords += tile.texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,10);
				}else{
					v3s16 f[3];
					for (u16 i=0; i<3; i++) {
						f[i] = faces[i];
						f[i].rotateYZBy(90);
						f[i].rotateXYBy(rots[j]);
					}
					meshgen_lights(data,n,p,colours,255,f[0],2,v);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
					meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
				}

				for (int k=0; k<10; k++) {
					v[k].Pos += pos;
				}

				data->append(tile.getMaterial(), v, 10, indices, 24, colours);
			}
		}
		if (z_minus) {
			for (u16 j=0; j<4; j++) {
				video::S3DVertex v[10];
				for (u16 i=0; i<10; i++) {
					v[i] = branch_vertices[i];
					v[i].Pos += v3f(0,-BS*0.625,0);
					v[i].Pos.rotateYZBy(90);
					v[i].Pos.rotateXYBy(rots[j]);
					v[i].TCoords *= tile.texture.size;
					v[i].TCoords += tile.texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,10);
				}else{
					v3s16 f[3];
					for (u16 i=0; i<3; i++) {
						f[i] = faces[i];
						f[i].rotateYZBy(90);
						f[i].rotateXYBy(rots[j]);
					}
					meshgen_lights(data,n,p,colours,255,f[0],2,v);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
					meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
				}

				for (int k=0; k<10; k++) {
					v[k].Pos += pos;
				}

				data->append(tile.getMaterial(), v, 10, indices, 24, colours);
			}
		}
	}else{ /* horizontal trunk */
		if ((x_plus && x_minus) || ((x_plus || x_minus) && (!z_plus || !z_minus))) { // centred along X
			for (u16 j=0; j<4; j++) {
				video::S3DVertex v[10];
				for (u16 i=0; i<10; i++) {
					v[i] = vertices[i];
					v[i].Pos.rotateXYBy(90);
					v[i].Pos.rotateYZBy(rots[j]);
					v[i].TCoords *= tile.texture.size;
					v[i].TCoords += tile.texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,10);
				}else{
					v3s16 f[3];
					for (u16 i=0; i<3; i++) {
						f[i] = faces[i];
						f[i].rotateXYBy(90);
						f[i].rotateYZBy(180+rots[j]);
					}
					meshgen_lights(data,n,p,colours,255,f[0],2,v);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
					meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
				}

				for (int k=0; k<10; k++) {
					v[k].Pos += pos;
				}

				data->append(tile.getMaterial(), v, 10, indices, 24, colours);
			}
			if (!x_plus_any) {
				for (u16 j=0; j<4; j++) {
					video::S3DVertex v[6];
					for (u16 i=0; i<6; i++) {
						v[i] = end_vertices[i];
						v[i].Pos.rotateXYBy(-90);
						v[i].Pos.rotateYZBy(rots[j]);
						v[i].TCoords *= endtile.texture.size;
						v[i].TCoords += endtile.texture.pos;
					}
					std::vector<video::SColor> colours[18];
					if (selected) {
						meshgen_selected_lights(colours,255,6);
					}else{
						meshgen_lights(data,n,p,colours,255,v3s16(1,0,0),6,v);
					}

					for (int k=0; k<6; k++) {
						v[k].Pos += pos;
					}

					data->append(endtile.getMaterial(), v, 6, end_indices, 12, colours);
				}
			}
			if (!x_minus_any) {
				for (u16 j=0; j<4; j++) {
					video::S3DVertex v[6];
					for (u16 i=0; i<6; i++) {
						v[i] = end_vertices[i];
						v[i].Pos.rotateXYBy(90);
						v[i].Pos.rotateYZBy(rots[j]);
						v[i].TCoords *= endtile.texture.size;
						v[i].TCoords += endtile.texture.pos;
					}
					std::vector<video::SColor> colours[18];
					if (selected) {
						meshgen_selected_lights(colours,255,6);
					}else{
						meshgen_lights(data,n,p,colours,255,v3s16(-1,0,0),6,v);
					}

					for (int k=0; k<6; k++) {
						v[k].Pos += pos;
					}

					data->append(endtile.getMaterial(), v, 6, end_indices, 12, colours);
				}
			}
			if (z_plus) {
				for (u16 j=0; j<4; j++) {
					video::S3DVertex v[10];
					for (u16 i=0; i<10; i++) {
						v[i] = branch_vertices[i];
						v[i].Pos.rotateYZBy(90);
						v[i].Pos.rotateXYBy(rots[j]);
						v[i].TCoords *= tile.texture.size;
						v[i].TCoords += tile.texture.pos;
					}
					std::vector<video::SColor> colours[18];
					if (selected) {
						meshgen_selected_lights(colours,255,10);
					}else{
						v3s16 f[3];
						for (u16 i=0; i<3; i++) {
							f[i] = faces[i];
							f[i].rotateYZBy(90);
							f[i].rotateXYBy(180+rots[j]);
						}
						meshgen_lights(data,n,p,colours,255,f[0],2,v);
						meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
						meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
						meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
						meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
						meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
					}

					for (int k=0; k<10; k++) {
						v[k].Pos += pos;
					}

					data->append(tile.getMaterial(), v, 10, indices, 24, colours);
				}
			}
			if (z_minus) {
				for (u16 j=0; j<4; j++) {
					video::S3DVertex v[10];
					for (u16 i=0; i<10; i++) {
						v[i] = branch_vertices[i];
						v[i].Pos += v3f(0,-BS*0.625,0);
						v[i].Pos.rotateYZBy(90);
						v[i].Pos.rotateXYBy(rots[j]);
						v[i].TCoords *= tile.texture.size;
						v[i].TCoords += tile.texture.pos;
					}
					std::vector<video::SColor> colours[18];
					if (selected) {
						meshgen_selected_lights(colours,255,10);
					}else{
						v3s16 f[3];
						for (u16 i=0; i<3; i++) {
							f[i] = faces[i];
							f[i].rotateYZBy(90);
							f[i].rotateXYBy(180+rots[j]);
						}
						meshgen_lights(data,n,p,colours,255,f[0],2,v);
						meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
						meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
						meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
						meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
						meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
					}

					for (int k=0; k<10; k++) {
						v[k].Pos += pos;
					}

					data->append(tile.getMaterial(), v, 10, indices, 24, colours);
				}
			}
		}else{ // centred along Z
			for (u16 j=0; j<4; j++) {
				video::S3DVertex v[10];
				for (u16 i=0; i<10; i++) {
					v[i] = vertices[i];
					v[i].Pos.rotateYZBy(90);
					v[i].Pos.rotateXYBy(rots[j]);
					v[i].TCoords *= tile.texture.size;
					v[i].TCoords += tile.texture.pos;
				}
				std::vector<video::SColor> colours[18];
				if (selected) {
					meshgen_selected_lights(colours,255,10);
				}else{
					v3s16 f[3];
					for (u16 i=0; i<3; i++) {
						f[i] = faces[i];
						f[i].rotateYZBy(90);
						f[i].rotateXYBy(180+rots[j]);
					}
					meshgen_lights(data,n,p,colours,255,f[0],2,v);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
					meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
					meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
					meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
				}

				for (int k=0; k<10; k++) {
					v[k].Pos += pos;
				}

				data->append(tile.getMaterial(), v, 10, indices, 24, colours);
			}
			if (!z_plus_any) {
				for (u16 j=0; j<4; j++) {
					video::S3DVertex v[6];
					for (u16 i=0; i<6; i++) {
						v[i] = end_vertices[i];
						v[i].Pos.rotateYZBy(90);
						v[i].Pos.rotateXYBy(rots[j]);
						v[i].TCoords *= endtile.texture.size;
						v[i].TCoords += endtile.texture.pos;
					}
					std::vector<video::SColor> colours[18];
					if (selected) {
						meshgen_selected_lights(colours,255,6);
					}else{
						meshgen_lights(data,n,p,colours,255,v3s16(0,0,1),6,v);
					}

					for (int k=0; k<6; k++) {
						v[k].Pos += pos;
					}

					data->append(endtile.getMaterial(), v, 6, end_indices, 12, colours);
				}
			}
			if (!z_minus_any) {
				for (u16 j=0; j<4; j++) {
					video::S3DVertex v[6];
					for (u16 i=0; i<6; i++) {
						v[i] = end_vertices[i];
						v[i].Pos.rotateYZBy(-90);
						v[i].Pos.rotateXYBy(rots[j]);
						v[i].TCoords *= endtile.texture.size;
						v[i].TCoords += endtile.texture.pos;
					}
					std::vector<video::SColor> colours[18];
					if (selected) {
						meshgen_selected_lights(colours,255,6);
					}else{
						meshgen_lights(data,n,p,colours,255,v3s16(0,0,-1),6,v);
					}

					for (int k=0; k<6; k++) {
						v[k].Pos += pos;
					}

					data->append(endtile.getMaterial(), v, 6, end_indices, 12, colours);
				}
			}
			if (x_plus) {
				for (u16 j=0; j<4; j++) {
					video::S3DVertex v[10];
					for (u16 i=0; i<10; i++) {
						v[i] = branch_vertices[i];
						v[i].Pos += v3f(0,-BS*0.625,0);
						v[i].Pos.rotateXYBy(90);
						v[i].Pos.rotateYZBy(rots[j]);
						v[i].TCoords *= tile.texture.size;
						v[i].TCoords += tile.texture.pos;
					}
					std::vector<video::SColor> colours[18];
					if (selected) {
						meshgen_selected_lights(colours,255,10);
					}else{
						v3s16 f[3];
						for (u16 i=0; i<3; i++) {
							f[i] = faces[i];
							f[i].rotateXYBy(90);
							f[i].rotateYZBy(180+rots[j]);
						}
						meshgen_lights(data,n,p,colours,255,f[0],2,v);
						meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
						meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
						meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
						meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
						meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
					}

					for (int k=0; k<10; k++) {
						v[k].Pos += pos;
					}

					data->append(tile.getMaterial(), v, 10, indices, 24, colours);
				}
			}
			if (x_minus) {
				for (u16 j=0; j<4; j++) {
					video::S3DVertex v[10];
					for (u16 i=0; i<10; i++) {
						v[i] = branch_vertices[i];
						v[i].Pos.rotateXYBy(90);
						v[i].Pos.rotateYZBy(rots[j]);
						v[i].TCoords *= tile.texture.size;
						v[i].TCoords += tile.texture.pos;
					}
					std::vector<video::SColor> colours[18];
					if (selected) {
						meshgen_selected_lights(colours,255,10);
					}else{
						v3s16 f[3];
						for (u16 i=0; i<3; i++) {
							f[i] = faces[i];
							f[i].rotateXYBy(90);
							f[i].rotateYZBy(180+rots[j]);
						}
						meshgen_lights(data,n,p,colours,255,f[0],2,v);
						meshgen_lights(data,n,p,colours,255,f[1],1,&v[2]);
						meshgen_lights(data,n,p,colours,255,f[2],2,&v[3]);
						meshgen_lights(data,n,p,colours,255,f[0],2,&v[5]);
						meshgen_lights(data,n,p,colours,255,f[1],1,&v[7]);
						meshgen_lights(data,n,p,colours,255,f[2],2,&v[8]);
					}

					for (int k=0; k<10; k++) {
						v[k].Pos += pos;
					}

					data->append(tile.getMaterial(), v, 10, indices, 24, colours);
				}
			}
		}
	}
}

#endif

