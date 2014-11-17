/*
Minetest-c55
Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "content_mapblock.h"
#include "content_mapnode.h"
#include "main.h" // For g_settings and g_texturesource
#include "mineral.h"
#include "mapblock_mesh.h" // For MapBlock_LightColor()
#include "settings.h"
#include "environment.h"
#include "nodemetadata.h"

#ifndef SERVER
// Create a cuboid.
// collector - the MeshCollector for the resulting polygons
// box - the position and size of the box
// tiles - the tiles (materials) to use (for all 6 faces)
// tilecount - number of entries in tiles, 1<=tilecount<=6
// c - vertex colour - used for all
// txc - texture coordinates - this is a list of texture coordinates
// for the opposite corners of each face - therefore, there
// should be (2+2)*6=24 values in the list. Alternatively, pass
// NULL to use the entire texture for each face. The order of
// the faces in the list is up-down-right-left-back-front
// (compatible with ContentFeatures). If you specified 0,0,1,1
// for each face, that would be the same as passing NULL.
void makeRotatedCuboid(MeshCollector *collector, v3f pos, const aabb3f &box,
	TileSpec *tiles, int tilecount,
	video::SColor c[8], const f32* txc, v3s16 angle)
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

	video::S3DVertex vertices[24] = {
		// up
		video::S3DVertex(min.X,max.Y,max.Z, 0,1,0, c[0], txc[0],txc[1]),
		video::S3DVertex(max.X,max.Y,max.Z, 0,1,0, c[1], txc[2],txc[1]),
		video::S3DVertex(max.X,max.Y,min.Z, 0,1,0, c[2], txc[2],txc[3]),
		video::S3DVertex(min.X,max.Y,min.Z, 0,1,0, c[3], txc[0],txc[3]),
		// down
		video::S3DVertex(min.X,min.Y,min.Z, 0,-1,0, c[4], txc[4],txc[5]),
		video::S3DVertex(max.X,min.Y,min.Z, 0,-1,0, c[5], txc[6],txc[5]),
		video::S3DVertex(max.X,min.Y,max.Z, 0,-1,0, c[6], txc[6],txc[7]),
		video::S3DVertex(min.X,min.Y,max.Z, 0,-1,0, c[7], txc[4],txc[7]),
		// right
		video::S3DVertex(max.X,max.Y,min.Z, 1,0,0, c[2], txc[ 8],txc[9]),
		video::S3DVertex(max.X,max.Y,max.Z, 1,0,0, c[1], txc[10],txc[9]),
		video::S3DVertex(max.X,min.Y,max.Z, 1,0,0, c[6], txc[10],txc[11]),
		video::S3DVertex(max.X,min.Y,min.Z, 1,0,0, c[5], txc[ 8],txc[11]),
		// left
		video::S3DVertex(min.X,max.Y,max.Z, -1,0,0, c[0], txc[12],txc[13]),
		video::S3DVertex(min.X,max.Y,min.Z, -1,0,0, c[3], txc[14],txc[13]),
		video::S3DVertex(min.X,min.Y,min.Z, -1,0,0, c[4], txc[14],txc[15]),
		video::S3DVertex(min.X,min.Y,max.Z, -1,0,0, c[7], txc[12],txc[15]),
		// back
		video::S3DVertex(max.X,max.Y,max.Z, 0,0,1, c[1], txc[16],txc[17]),
		video::S3DVertex(min.X,max.Y,max.Z, 0,0,1, c[0], txc[18],txc[17]),
		video::S3DVertex(min.X,min.Y,max.Z, 0,0,1, c[7], txc[18],txc[19]),
		video::S3DVertex(max.X,min.Y,max.Z, 0,0,1, c[6], txc[16],txc[19]),
		// front
		video::S3DVertex(min.X,max.Y,min.Z, 0,0,-1, c[3], txc[20],txc[21]),
		video::S3DVertex(max.X,max.Y,min.Z, 0,0,-1, c[2], txc[22],txc[21]),
		video::S3DVertex(max.X,min.Y,min.Z, 0,0,-1, c[5], txc[22],txc[23]),
		video::S3DVertex(min.X,min.Y,min.Z, 0,0,-1, c[4], txc[20],txc[23]),
	};


	for (s32 j=0; j<24; j++) {
		int tileindex = MYMIN(j/4, tilecount-1);
		if (angle.Y)
			vertices[j].Pos.rotateXZBy(angle.Y);
		if (angle.X)
			vertices[j].Pos.rotateYZBy(angle.X);
		if (angle.Z)
			vertices[j].Pos.rotateXYBy(angle.Z);
		vertices[j].Pos += pos;
		vertices[j].TCoords *= tiles[tileindex].texture.size;
		vertices[j].TCoords += tiles[tileindex].texture.pos;
	}
	u16 indices[] = {0,1,2,2,3,0};
	// Add to mesh collector
	for (s32 j=0; j<24; j+=4) {
		int tileindex = MYMIN(j/4, tilecount-1);
		collector->append(tiles[tileindex].getMaterial(), vertices+j, 4, indices, 6);
	}
}
void makeAngledCuboid(MeshCollector *collector, v3f pos, const aabb3f &box,
	TileSpec *tiles, int tilecount,
	video::SColor c[8], const f32* txc, s16 angle)
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

	video::S3DVertex vertices[24] = {
		// up
		video::S3DVertex(min.X,max.Y,max.Z, 0,1,0, c[0], txc[0],txc[1]),
		video::S3DVertex(max.X,max.Y,max.Z, 0,1,0, c[1], txc[2],txc[1]),
		video::S3DVertex(max.X,max.Y,min.Z, 0,1,0, c[2], txc[2],txc[3]),
		video::S3DVertex(min.X,max.Y,min.Z, 0,1,0, c[3], txc[0],txc[3]),
		// down
		video::S3DVertex(min.X,min.Y,min.Z, 0,-1,0, c[4], txc[4],txc[5]),
		video::S3DVertex(max.X,min.Y,min.Z, 0,-1,0, c[5], txc[6],txc[5]),
		video::S3DVertex(max.X,min.Y,max.Z, 0,-1,0, c[6], txc[6],txc[7]),
		video::S3DVertex(min.X,min.Y,max.Z, 0,-1,0, c[7], txc[4],txc[7]),
		// right
		video::S3DVertex(max.X,max.Y,min.Z, 1,0,0, c[2], txc[ 8],txc[9]),
		video::S3DVertex(max.X,max.Y,max.Z, 1,0,0, c[1], txc[10],txc[9]),
		video::S3DVertex(max.X,min.Y,max.Z, 1,0,0, c[6], txc[10],txc[11]),
		video::S3DVertex(max.X,min.Y,min.Z, 1,0,0, c[5], txc[ 8],txc[11]),
		// left
		video::S3DVertex(min.X,max.Y,max.Z, -1,0,0, c[0], txc[12],txc[13]),
		video::S3DVertex(min.X,max.Y,min.Z, -1,0,0, c[3], txc[14],txc[13]),
		video::S3DVertex(min.X,min.Y,min.Z, -1,0,0, c[4], txc[14],txc[15]),
		video::S3DVertex(min.X,min.Y,max.Z, -1,0,0, c[7], txc[12],txc[15]),
		// back
		video::S3DVertex(max.X,max.Y,max.Z, 0,0,1, c[1], txc[16],txc[17]),
		video::S3DVertex(min.X,max.Y,max.Z, 0,0,1, c[0], txc[18],txc[17]),
		video::S3DVertex(min.X,min.Y,max.Z, 0,0,1, c[7], txc[18],txc[19]),
		video::S3DVertex(max.X,min.Y,max.Z, 0,0,1, c[6], txc[16],txc[19]),
		// front
		video::S3DVertex(min.X,max.Y,min.Z, 0,0,-1, c[3], txc[20],txc[21]),
		video::S3DVertex(max.X,max.Y,min.Z, 0,0,-1, c[2], txc[22],txc[21]),
		video::S3DVertex(max.X,min.Y,min.Z, 0,0,-1, c[5], txc[22],txc[23]),
		video::S3DVertex(min.X,min.Y,min.Z, 0,0,-1, c[4], txc[20],txc[23]),
	};


	for (s32 j=0; j<24; j++) {
		int tileindex = MYMIN(j/4, tilecount-1);
		if (angle)
			vertices[j].Pos.rotateXZBy(angle);
		vertices[j].Pos += pos;
		vertices[j].TCoords *= tiles[tileindex].texture.size;
		vertices[j].TCoords += tiles[tileindex].texture.pos;
	}
	u16 indices[] = {0,1,2,2,3,0};
	// Add to mesh collector
	for (s32 j=0; j<24; j+=4) {
		int tileindex = MYMIN(j/4, tilecount-1);
		collector->append(tiles[tileindex].getMaterial(), vertices+j, 4, indices, 6);
	}
}
void makeCuboid(MeshCollector *collector, const aabb3f &box,
	TileSpec *tiles, int tilecount,
	video::SColor c[8], const f32* txc)
{
	makeAngledCuboid(collector,v3f(0,0,0),box,tiles,tilecount,c,txc,0);
}

/*
 * makes one tri/poly for a roof section
 */
void makeRoofTri(MeshCollector *collector, v3f corners[3], v3f pos, TileSpec *tiles, int tilecount, video::SColor c[2], s16 rot)
{
	assert(tilecount >= 1 && tilecount <= 6);
	// vertices for top and bottom tri
	v3f top_v[3];
	v3f btm_v[3];
	// tex coords for top and bottom tri
	v2f top_t[3];
	v2f btm_t[3];
	for (int i=0; i<3; i++) {
		top_v[i].X = (corners[i].X*BS)+pos.X;
		top_v[i].Y = ((corners[i].Y+0.01)*BS)+pos.Y;
		top_v[i].Z = (corners[i].Z*BS)+pos.Z;
		top_t[i].X = (corners[i].X+0.5);
		top_t[i].Y = (corners[i].Z+0.5);
		if (rot)
			top_t[i] = top_t[i].rotateBy(rot,v2f(0.5,0.5));
		top_t[i].X = (top_t[i].X*tiles[0].texture.size.X)+tiles[0].texture.pos.X;
		top_t[i].Y = (top_t[i].Y*tiles[0].texture.size.Y)+tiles[0].texture.pos.Y;

		// reverse winding for bottom
		btm_v[2-i].X = (corners[i].X*BS)+pos.X;
		btm_v[2-i].Y = ((corners[i].Y-0.01)*BS)+pos.Y;
		btm_v[2-i].Z = (corners[i].Z*BS)+pos.Z;
		btm_t[2-i].X = top_t[i].X;
		btm_t[2-i].Y = top_t[i].Y;
	}

	{
		video::S3DVertex tri_v[3] = {
			video::S3DVertex(btm_v[0].X, btm_v[0].Y, btm_v[0].Z, 0,0,0, c[1], btm_t[0].X, btm_t[0].Y),
			video::S3DVertex(btm_v[1].X, btm_v[1].Y, btm_v[1].Z, 0,0,0, c[1], btm_t[1].X, btm_t[1].Y),
			video::S3DVertex(btm_v[2].X, btm_v[2].Y, btm_v[2].Z, 0,0,0, c[1], btm_t[2].X, btm_t[2].Y),
		};
		u16 indices[] = {0,1,2};
		collector->append(tiles[0].getMaterial(),tri_v, 3, indices, 3);
	}
	{
		video::S3DVertex tri_v[3] = {
			video::S3DVertex(top_v[0].X, top_v[0].Y, top_v[0].Z, 0,0,0, c[0], top_t[0].X, top_t[0].Y),
			video::S3DVertex(top_v[1].X, top_v[1].Y, top_v[1].Z, 0,0,0, c[0], top_t[1].X, top_t[1].Y),
			video::S3DVertex(top_v[2].X, top_v[2].Y, top_v[2].Z, 0,0,0, c[0], top_t[2].X, top_t[2].Y),
		};
		u16 indices[] = {0,1,2};
		collector->append(tiles[0].getMaterial(),tri_v, 3, indices, 3);
	}
}

/*
 * get the light values for a node
 * smooth lighting gets per-vertex
 * standard lighting gets per-face
 * TODO: smooth lighting currently gets the light for each surrounding node up
 * to eight times, probably a better way to do this
 */
static void getLights(v3s16 pos, video::SColor *lights, MeshMakeData *data, bool smooth_lighting, u8 vertex_alpha)
{
	bool selected = false;
	NodeMod mod;
	data->m_temp_mods.get(pos-(data->m_blockpos*MAP_BLOCKSIZE),&mod);
	if (mod == NODEMOD_SELECTION)
		selected = true;
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
		video::SColor c = MapBlock_LightColor(vertex_alpha, l, selected);
		for (int i=0; i<8; i++) {
			lights[i] = c;
		}
		return;
	}
	static const v3s16 corners[8] = {
		v3s16(0,1,1),
		v3s16(1,1,1),
		v3s16(1,1,0),
		v3s16(0,1,0),
		v3s16(0,0,0),
		v3s16(1,0,0),
		v3s16(1,0,1),
		v3s16(0,0,1)
	};
	u8 l;
	u32 lt;
	u32 ltp;
	u8 ld;
	MapNode tn;
	for (int i=0; i<8; i++) {
		l = 0;
		lt = 0;
		ld = 1;
		for (s16 tx=-1; tx<1; tx++) {
		for (s16 ty=-1; ty<1; ty++) {
		for (s16 tz=-1; tz<1; tz++) {
			tn = data->m_vmanip.getNodeRO(pos + v3s16(tx,ty,tz) + corners[i]);
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
		lights[i] = MapBlock_LightColor(vertex_alpha, l, selected);
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

#ifndef SERVER
static int mapblock_mesh_check_walllike(MeshMakeData *data, MapNode n, v3s16 p, u8 d[8])
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
	const ContentFeatures *f2;
	for (s16 i=0; i<8; i++) {
		d[i] = 0;
	}
	for (int k=0; k<8; k++) {
		if (k > 3 && (d[showcheck[k-4][0]] || d[showcheck[k-4][1]]))
					continue;
		p2 = p+fence_dirs[k];
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
			d[k] = 1;
		}
	}
	u8 ps = d[0]+d[1]+d[2]+d[3]+d[4]+d[5]+d[6]+d[7];
	p2 = p;
	p2.Y++;
	n2 = data->m_vmanip.getNodeRO(p2);
	if (
		content_features(n2).draw_type != CDT_WALLLIKE
		&& content_features(n2).draw_type != CDT_AIRLIKE
	) {
		if (
			content_features(n2).draw_type == CDT_TORCHLIKE
			|| content_features(n2).draw_type == CDT_FENCELIKE
		)
			return 0;
		return 1;
	}
	if (content_features(n2).draw_type == CDT_WALLLIKE) {
		u8 ad[8];
		int ap = mapblock_mesh_check_walllike(data,n2,p2,ad);
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

void mapblock_mesh_generate_special(MeshMakeData *data,
		MeshCollector &collector)
{
	/*
		Some settings
	*/
	bool new_style_water = g_settings->getBool("new_style_water");
	bool smooth_lighting = g_settings->getBool("smooth_lighting");

	float node_liquid_level = 1.0;
	if(new_style_water)
		node_liquid_level = 0.85;

	v3s16 blockpos_nodes = data->m_blockpos*MAP_BLOCKSIZE;
	bool selected = false;

	for(s16 z=0; z<MAP_BLOCKSIZE; z++)
	for(s16 y=0; y<MAP_BLOCKSIZE; y++)
	for(s16 x=0; x<MAP_BLOCKSIZE; x++)
	{
		v3s16 p(x,y,z);

		MapNode n = data->m_vmanip.getNodeNoEx(blockpos_nodes+p);
		NodeMod mod;
		data->m_temp_mods.get(p,&mod);
		selected = (mod == NODEMOD_SELECTION);

		/*
			Add torches to mesh
		*/
		switch (content_features(n).draw_type) {
		case CDT_CUBELIKE:
		case CDT_AIRLIKE:
			break;
		case CDT_LIQUID:
		{
			bool top_is_same_liquid = false;
			MapNode ntop = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x,y+1,z));
			content_t c_flowing = content_features(n).liquid_alternative_flowing;
			content_t c_source = content_features(n).liquid_alternative_source;
			if(ntop.getContent() == c_flowing || ntop.getContent() == c_source)
				top_is_same_liquid = true;

			u8 l = 0;
			// Use the light of the node on top if possible
			if (content_features(ntop).param_type == CPT_LIGHT){
				l = decode_light(ntop.getLightBlend(data->m_daynight_ratio));
			// Otherwise use the light of this node (the liquid)
			}else{
				l = decode_light(n.getLightBlend(data->m_daynight_ratio));
			}
			video::SColor c = MapBlock_LightColor(content_features(n).vertex_alpha, l, selected);

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
			for(u32 i=0; i<9; i++)
			{
				content_t content = CONTENT_AIR;
				float level = -0.5 * BS;
				u8 flags = 0;
				// Check neighbor
				v3s16 p2 = p + neighbor_dirs[i];
				MapNode n2 = data->m_vmanip.getNodeRO(blockpos_nodes + p2);
				if(n2.getContent() != CONTENT_IGNORE) {
					content = n2.getContent();

					if (n2.getContent() == c_source) {
						p2.Y += 1;
						n2 = data->m_vmanip.getNodeRO(blockpos_nodes + p2);
						if (content_features(n2).liquid_type == LIQUID_NONE) {
							level = 0.5*BS;
						}else{
							level = (-0.5+node_liquid_level) * BS;
						}
						p2.Y -= 1;
					}else if(n2.getContent() == c_flowing) {
						level = (-0.5 + ((float)(n2.param2&LIQUID_LEVEL_MASK)
								+ 0.5) / 8.0 * node_liquid_level) * BS;
					}

					// Check node above neighbor.
					// NOTE: This doesn't get executed if neighbor
					//       doesn't exist
					p2.Y += 1;
					n2 = data->m_vmanip.getNodeRO(blockpos_nodes + p2);
					if(n2.getContent() == c_source ||
							n2.getContent() == c_flowing)
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
			for(u32 i=0; i<4; i++)
			{
				v3s16 cornerdir = halfdirs[i];
				float cornerlevel = 0;
				u32 valid_count = 0;
				u32 air_count = 0;
				for(u32 j=0; j<4; j++)
				{
					v3s16 neighbordir = cornerdir - halfdirs[j];
					content_t content = neighbor_contents[neighbordir];
					// If top is liquid, draw starting from top of node
					if(neighbor_flags[neighbordir] &
							neighborflag_top_is_same_liquid)
					{
						cornerlevel = 0.5*BS;
						valid_count = 1;
						break;
					}
					// Source is always the same height
					else if(content == c_source)
					{
						cornerlevel = (-0.5+node_liquid_level)*BS;
						valid_count = 1;
						break;
					}
					// Flowing liquid has level information
					else if(content == c_flowing)
					{
						cornerlevel += neighbor_levels[neighbordir];
						valid_count++;
					}
					else if(content == CONTENT_AIR)
					{
						air_count++;
					}
				}
				if(air_count >= 2)
					cornerlevel = -0.5*BS;
				else if(valid_count > 0)
					cornerlevel /= valid_count;
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
			for(u32 i=0; i<4; i++)
			{
				v3s16 dir = side_dirs[i];

				/*
					If our topside is liquid and neighbor's topside
					is liquid, don't draw side face
				*/
				if(top_is_same_liquid &&
						neighbor_flags[dir] & neighborflag_top_is_same_liquid)
					continue;

				content_t neighbor_content = neighbor_contents[dir];
				ContentFeatures &n_feat = content_features(neighbor_content);

				// Don't draw face if neighbor is blocking the view
				if(n_feat.solidness == 2)
					continue;

				bool neighbor_is_same_liquid = (neighbor_content == c_source
						|| neighbor_content == c_flowing);

				// Don't draw any faces if neighbor same is liquid and top is
				// same liquid
				if(neighbor_is_same_liquid == true
						&& top_is_same_liquid == false)
					continue;

				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,0,BS/2, 0,0,0, c,
							content_features(n).tiles[i].texture.x0(), content_features(n).tiles[i].texture.y1()),
					video::S3DVertex(BS/2,0,BS/2, 0,0,0, c,
							content_features(n).tiles[i].texture.x1(), content_features(n).tiles[i].texture.y1()),
					video::S3DVertex(BS/2,0,BS/2, 0,0,0, c,
							content_features(n).tiles[i].texture.x1(), content_features(n).tiles[i].texture.y0()),
					video::S3DVertex(-BS/2,0,BS/2, 0,0,0, c,
							content_features(n).tiles[i].texture.x0(), content_features(n).tiles[i].texture.y0()),
				};

				/*
					If our topside is liquid, set upper border of face
					at upper border of node
				*/
				if(top_is_same_liquid)
				{
					vertices[2].Pos.Y = 0.5*BS;
					vertices[3].Pos.Y = 0.5*BS;
				}
				/*
					Otherwise upper position of face is corner levels
				*/
				else
				{
					vertices[2].Pos.Y = corner_levels[side_corners[i][0]];
					vertices[3].Pos.Y = corner_levels[side_corners[i][1]];
				}

				/*
					If neighbor is liquid, lower border of face is corner
					liquid levels
				*/
				if(neighbor_is_same_liquid)
				{
					vertices[0].Pos.Y = corner_levels[side_corners[i][1]];
					vertices[1].Pos.Y = corner_levels[side_corners[i][0]];
				}
				/*
					If neighbor is not liquid, lower border of face is
					lower border of node
				*/
				else
				{
					vertices[0].Pos.Y = -0.5*BS;
					vertices[1].Pos.Y = -0.5*BS;
				}

				for(s32 j=0; j<4; j++)
				{
					if(dir == v3s16(0,0,1))
						vertices[j].Pos.rotateXZBy(0);
					if(dir == v3s16(0,0,-1))
						vertices[j].Pos.rotateXZBy(180);
					if(dir == v3s16(-1,0,0))
						vertices[j].Pos.rotateXZBy(90);
					if(dir == v3s16(1,0,-0))
						vertices[j].Pos.rotateXZBy(-90);

					vertices[j].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[i].getMaterial(), vertices, 4, indices, 6);
			}

			/*
				Generate top side, if appropriate
			*/

			if(top_is_same_liquid == false)
			{
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,0,BS/2, 0,0,0, c,
							content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,0,BS/2, 0,0,0, c,
							content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,0,-BS/2, 0,0,0, c,
							content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y0()),
					video::S3DVertex(-BS/2,0,-BS/2, 0,0,0, c,
							content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y0()),
				};

				// This fixes a strange bug
				s32 corner_resolve[4] = {3,2,1,0};

				for(s32 i=0; i<4; i++)
				{
					s32 j = corner_resolve[i];
					vertices[i].Pos.Y += corner_levels[j];
					vertices[i].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
			}
		}
		/*
			Add water sources to mesh if using new style
		*/
		break;
		case CDT_LIQUID_SOURCE:
		if (new_style_water)
		{
			static const u8 l[6][4] = {
				{0,1,6,7},
				{0,1,2,3},
				{1,2,5,6},
				{2,3,4,5},
				{4,5,6,7},
				{0,3,4,7}
			};
			video::SColor c[8];
			getLights(blockpos_nodes+p,c,data,smooth_lighting,content_features(n).vertex_alpha);

			for(u32 j=0; j<6; j++)
			{
				// Check this neighbor
				v3s16 n2p = blockpos_nodes + p + g_6dirs[j];
				MapNode n2 = data->m_vmanip.getNodeRO(n2p);
				if (content_features(n2).liquid_type != LIQUID_NONE) {
					if (n2.getContent() == content_features(n).liquid_alternative_flowing)
						continue;
					if (n2.getContent() == content_features(n).liquid_alternative_source)
						continue;
				}else if (g_6dirs[j].Y != 1 && n2.getContent() != CONTENT_AIR && content_features(n2).draw_type == CDT_CUBELIKE) {
					continue;
				}else if (n2.getContent() == CONTENT_IGNORE) {
					continue;
				}

				// The face at Z+
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-BS/2,BS/2, 0,0,0, c[l[j][0]],
						content_features(n).tiles[j].texture.x0(), content_features(n).tiles[j].texture.y1()),
					video::S3DVertex(BS/2,-BS/2,BS/2, 0,0,0, c[l[j][1]],
						content_features(n).tiles[j].texture.x1(), content_features(n).tiles[j].texture.y1()),
					video::S3DVertex(BS/2,BS/2,BS/2, 0,0,0, c[l[j][2]],
						content_features(n).tiles[j].texture.x1(), content_features(n).tiles[j].texture.y0()),
					video::S3DVertex(-BS/2,BS/2,BS/2, 0,0,0, c[l[j][3]],
						content_features(n).tiles[j].texture.x0(), content_features(n).tiles[j].texture.y0()),
				};

				// Rotations in the g_6dirs format
				switch (j) {
				case 0: // Z+
					for(u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(0);
					}
					break;
				case 1: // Y+
					for(u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateYZBy(-90);
					}
					break;
				case 2: // X+
					for(u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(-90);
					}
					break;
				case 3: // Z-
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
					for(u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(90);
					}
					break;
				default:;
				}

				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
					if (j == 1 || (j != 4 && i<2)) {
						vertices[i].Pos.Y -=0.15*BS;
					}
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[j].getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_TORCHLIKE:
		{
			static const f32 txc[24] = {
				0.625,0.125,0.75,0.25,
				0.625,0.625,0.625,0.75,
				0,0,0.125,1,
				0,0,0.125,1,
				0,0,0.125,1,
				0,0,0.125,1
			};
			video::SColor c = MapBlock_LightColor(255,255,selected);//(255,255,255,255);
			v3f pos = intToFloat(p+blockpos_nodes, BS);
			v3s16 dir = unpackDir(n.param2);
			video::S3DVertex *v;
			video::S3DVertex vertices[3][24] = {
				{ // roof
					// up
					video::S3DVertex(-0.1*BS, 0.5*BS,0., 0,1,0, c, txc[4],txc[5]),
					video::S3DVertex(0.,0.5*BS,0., 0,1,0, c, txc[6],txc[5]),
					video::S3DVertex(0.,0.5*BS,-0.1*BS, 0,1,0, c, txc[6],txc[7]),
					video::S3DVertex(-0.1*BS, 0.5*BS,-0.1*BS, 0,1,0, c, txc[4],txc[7]),
					// down
					video::S3DVertex(0.,-0.1*BS,0., 0,-1,0, c, txc[0],txc[1]),
					video::S3DVertex(0.1*BS,-0.1*BS,0., 0,-1,0, c, txc[2],txc[1]),
					video::S3DVertex(0.1*BS,-0.1*BS,0.1*BS, 0,-1,0, c, txc[2],txc[3]),
					video::S3DVertex(0.,-0.1*BS,0.1*BS, 0,-1,0, c, txc[0],txc[3]),
					// right
					video::S3DVertex(0.,0.5*BS,-0.1*BS, 1,0,0, c, txc[ 8],txc[11]),
					video::S3DVertex(0.,0.5*BS,0., 1,0,0, c, txc[10],txc[11]),
					video::S3DVertex(0.1*BS,-0.1*BS,0.1*BS, 1,0,0, c, txc[10],txc[9]),
					video::S3DVertex(0.1*BS,-0.1*BS,0., 1,0,0, c, txc[ 8],txc[9]),
					// left
					video::S3DVertex(-0.1*BS,0.5*BS,0., -1,0,0, c, txc[12],txc[15]),
					video::S3DVertex(-0.1*BS,0.5*BS,-0.1*BS, -1,0,0, c, txc[14],txc[15]),
					video::S3DVertex(0.,-0.1*BS,0., -1,0,0, c, txc[14],txc[13]),
					video::S3DVertex(0.,-0.1*BS,0.1*BS, -1,0,0, c, txc[12],txc[13]),
					// back
					video::S3DVertex(0.,0.5*BS,0., 0,0,1, c, txc[16],txc[19]),
					video::S3DVertex(-0.1*BS,0.5*BS,0., 0,0,1, c, txc[18],txc[19]),
					video::S3DVertex(0.,-0.1*BS,0.1*BS, 0,0,1, c, txc[18],txc[17]),
					video::S3DVertex(0.1*BS,-0.1*BS,0.1*BS, 0,0,1, c, txc[16],txc[17]),
					// front
					video::S3DVertex(-0.1*BS,0.5*BS,-0.1*BS, 0,0,-1, c, txc[20],txc[23]),
					video::S3DVertex(0.,0.5*BS,-0.1*BS, 0,0,-1, c, txc[22],txc[23]),
					video::S3DVertex(0.1*BS,-0.1*BS,0., 0,0,-1, c, txc[22],txc[21]),
					video::S3DVertex(0.,-0.1*BS,0., 0,0,-1, c, txc[20],txc[21]),
				},{ // floor
					// up
					//video::S3DVertex(min.X,max.Y,max.Z, 0,1,0, c, txc[0],txc[1]),
					video::S3DVertex(-0.05*BS,0.1*BS,0.05*BS, 0,1,0, c, txc[0],txc[1]),
					video::S3DVertex(0.05*BS,0.1*BS,0.05*BS, 0,1,0, c, txc[2],txc[1]),
					video::S3DVertex(0.05*BS,0.1*BS,-0.05*BS, 0,1,0, c, txc[2],txc[3]),
					video::S3DVertex(-0.05*BS,0.1*BS,-0.05*BS, 0,1,0, c, txc[0],txc[3]),
					// down
					video::S3DVertex(-0.05*BS,-0.5*BS,-0.05*BS, 0,-1,0, c, txc[4],txc[5]),
					video::S3DVertex(0.05*BS,-0.5*BS,-0.05*BS, 0,-1,0, c, txc[6],txc[5]),
					video::S3DVertex(0.05*BS,-0.5*BS,0.05*BS, 0,-1,0, c, txc[6],txc[7]),
					video::S3DVertex(-0.05*BS,-0.5*BS,0.05*BS, 0,-1,0, c, txc[4],txc[7]),
					// right
					video::S3DVertex(0.05*BS,0.1*BS,-0.05*BS, 1,0,0, c, txc[ 8],txc[9]),
					video::S3DVertex(0.05*BS,0.1*BS,0.05*BS, 1,0,0, c, txc[10],txc[9]),
					video::S3DVertex(0.05*BS,-0.5*BS,0.05*BS, 1,0,0, c, txc[10],txc[11]),
					video::S3DVertex(0.05*BS,-0.5*BS,-0.05*BS, 1,0,0, c, txc[ 8],txc[11]),
					// left
					video::S3DVertex(-0.05*BS,0.1*BS,0.05*BS, -1,0,0, c, txc[12],txc[13]),
					video::S3DVertex(-0.05*BS,0.1*BS,-0.05*BS, -1,0,0, c, txc[14],txc[13]),
					video::S3DVertex(-0.05*BS,-0.5*BS,-0.05*BS, -1,0,0, c, txc[14],txc[15]),
					video::S3DVertex(-0.05*BS,-0.5*BS,0.05*BS, -1,0,0, c, txc[12],txc[15]),
					// back
					video::S3DVertex(0.05*BS,0.1*BS,0.05*BS, 0,0,1, c, txc[16],txc[17]),
					video::S3DVertex(-0.05*BS,0.1*BS,0.05*BS, 0,0,1, c, txc[18],txc[17]),
					video::S3DVertex(-0.05*BS,-0.5*BS,0.05*BS, 0,0,1, c, txc[18],txc[19]),
					video::S3DVertex(0.05*BS,-0.5*BS,0.05*BS, 0,0,1, c, txc[16],txc[19]),
					// front
					video::S3DVertex(-0.05*BS,0.1*BS,-0.05*BS, 0,0,-1, c, txc[20],txc[21]),
					video::S3DVertex(0.05*BS,0.1*BS,-0.05*BS, 0,0,-1, c, txc[22],txc[21]),
					video::S3DVertex(0.05*BS,-0.5*BS,-0.05*BS, 0,0,-1, c, txc[22],txc[23]),
					video::S3DVertex(-0.05*BS,-0.5*BS,-0.05*BS, 0,0,-1, c, txc[20],txc[23]),
				},{ // wall
					// up
					//video::S3DVertex(min.X,max.Y,max.Z, 0,1,0, c, txc[0],txc[1]),
					video::S3DVertex(-0.05*BS,0.3*BS,0.4*BS, 0,1,0, c, txc[0],txc[1]),
					video::S3DVertex(0.05*BS,0.3*BS,0.4*BS, 0,1,0, c, txc[2],txc[1]),
					video::S3DVertex(0.05*BS,0.3*BS,0.3*BS, 0,1,0, c, txc[2],txc[3]),
					video::S3DVertex(-0.05*BS,0.3*BS,0.3*BS, 0,1,0, c, txc[0],txc[3]),
					// down
					video::S3DVertex(-0.05*BS,-0.3*BS,0.4*BS, 0,-1,0, c, txc[4],txc[5]),
					video::S3DVertex(0.05*BS,-0.3*BS,0.4*BS, 0,-1,0, c, txc[6],txc[5]),
					video::S3DVertex(0.05*BS,-0.3*BS,0.5*BS, 0,-1,0, c, txc[6],txc[7]),
					video::S3DVertex(-0.05*BS,-0.3*BS,0.5*BS, 0,-1,0, c, txc[4],txc[7]),
					// right
					video::S3DVertex(0.05*BS,0.3*BS,0.3*BS, 1,0,0, c, txc[ 8],txc[9]),
					video::S3DVertex(0.05*BS,0.3*BS,0.4*BS, 1,0,0, c, txc[10],txc[9]),
					video::S3DVertex(0.05*BS,-0.3*BS,0.5*BS, 1,0,0, c, txc[10],txc[11]),
					video::S3DVertex(0.05*BS,-0.3*BS,0.4*BS, 1,0,0, c, txc[ 8],txc[11]),
					// left
					video::S3DVertex(-0.05*BS,0.3*BS,0.4*BS, -1,0,0, c, txc[12],txc[13]),
					video::S3DVertex(-0.05*BS,0.3*BS,0.3*BS, -1,0,0, c, txc[14],txc[13]),
					video::S3DVertex(-0.05*BS,-0.3*BS,0.4*BS, -1,0,0, c, txc[14],txc[15]),
					video::S3DVertex(-0.05*BS,-0.3*BS,0.5*BS, -1,0,0, c, txc[12],txc[15]),
					// back
					video::S3DVertex(0.05*BS,0.3*BS,0.4*BS, 0,0,1, c, txc[16],txc[17]),
					video::S3DVertex(-0.05*BS,0.3*BS,0.4*BS, 0,0,1, c, txc[18],txc[17]),
					video::S3DVertex(-0.05*BS,-0.3*BS,0.5*BS, 0,0,1, c, txc[18],txc[19]),
					video::S3DVertex(0.05*BS,-0.3*BS,0.5*BS, 0,0,1, c, txc[16],txc[19]),
					// front
					video::S3DVertex(-0.05*BS,0.3*BS,0.3*BS, 0,0,-1, c, txc[20],txc[21]),
					video::S3DVertex(0.05*BS,0.3*BS,0.3*BS, 0,0,-1, c, txc[22],txc[21]),
					video::S3DVertex(0.05*BS,-0.3*BS,0.4*BS, 0,0,-1, c, txc[22],txc[23]),
					video::S3DVertex(-0.05*BS,-0.3*BS,0.4*BS, 0,0,-1, c, txc[20],txc[23]),
				}
			};

			if (dir.Y == 1) { // roof
				for (s32 i=0; i<24; i++) {
					vertices[0][i].Pos += intToFloat(p, BS);
				}
				v = vertices[0];
			}else if (dir.Y == -1) { // floor
				for (s32 i=0; i<24; i++) {
					vertices[1][i].Pos += intToFloat(p, BS);
				}
				v = vertices[1];
			}else{ // wall
				for (s32 i=0; i<24; i++) {
					if(dir == v3s16(1,0,0))
						vertices[2][i].Pos.rotateXZBy(-90);
					if(dir == v3s16(-1,0,0))
						vertices[2][i].Pos.rotateXZBy(90);
					if(dir == v3s16(0,0,1))
						vertices[2][i].Pos.rotateXZBy(0);
					if(dir == v3s16(0,0,-1))
						vertices[2][i].Pos.rotateXZBy(180);

					vertices[2][i].Pos += intToFloat(p, BS);
				}
				v = vertices[2];
			}

			f32 sx = content_features(n.getContent()).tiles[0].texture.x1()-content_features(n.getContent()).tiles[0].texture.x0();
			f32 sy = content_features(n.getContent()).tiles[0].texture.y1()-content_features(n.getContent()).tiles[0].texture.y0();
			for (s32 j=0; j<24; j++) {
				v[j].TCoords *= v2f(sx,sy);
				v[j].TCoords += v2f(
					content_features(n.getContent()).tiles[0].texture.x0(),
					content_features(n.getContent()).tiles[0].texture.y0()
				);
			}

			u16 indices[] = {0,1,2,2,3,0};

			// Add to mesh collector
			for (s32 j=0; j<24; j+=4) {
				collector.append(content_features(n.getContent()).tiles[0].getMaterial(), &v[j], 4, indices, 6);
			}
		}
		break;
		/*
			Add leaves if using new style
			Add glass
		*/
		break;
		case CDT_GLASSLIKE:
		{
			static const u8 l[6][4] = {
				{0,1,6,7},
				{0,1,2,3},
				{1,2,5,6},
				{2,3,4,5},
				{4,5,6,7},
				{0,3,4,7}
			};
			video::SColor c[8];
			getLights(blockpos_nodes+p,c,data,smooth_lighting);
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

			for(u32 j=0; j<6; j++)
			{
				// Check this neighbor
				v3s16 n2p = blockpos_nodes + p + g_6dirs[j];
				MapNode n2 = data->m_vmanip.getNodeRO(n2p);

				// The face at Z+
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-BS/2,BS/2, 0,0,0, c[l[j][0]],
						tiles[j].texture.x0(), tiles[j].texture.y1()),
					video::S3DVertex(BS/2,-BS/2,BS/2, 0,0,0, c[l[j][1]],
						tiles[j].texture.x1(), tiles[j].texture.y1()),
					video::S3DVertex(BS/2,BS/2,BS/2, 0,0,0, c[l[j][2]],
						tiles[j].texture.x1(), tiles[j].texture.y0()),
					video::S3DVertex(-BS/2,BS/2,BS/2, 0,0,0, c[l[j][3]],
						tiles[j].texture.x0(), tiles[j].texture.y0()),
				};

				// Rotations in the g_6dirs format
				switch (j) {
				case 0: // Z+
					for(u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(0);
					}
					break;
				case 1: // Y+
					for(u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateYZBy(-90);
					}
					break;
				case 2: // X+
					for(u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(-90);
					}
					break;
				case 3: // Z-
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
					for(u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(90);
					}
					break;
				default:;
				}

				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(tiles[j].getMaterial(), vertices, 4, indices, 6);
			}
		}
		/*
			Add wall
		*/
		break;
		case CDT_WALLLIKE:
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
			video::SColor c[8];
			getLights(blockpos_nodes+p,c,data,smooth_lighting);

			v3f pos = intToFloat(p, BS);
			std::vector<NodeBox> boxes = content_features(n).getNodeBoxes(n);
			v3s16 p2 = p;
			p2.Y++;
			MapNode n2 = data->m_vmanip.getNodeRO(blockpos_nodes + p2);
			NodeBox box;
			u8 d[8];
			int bi = mapblock_mesh_check_walllike(data,n,p+blockpos_nodes,d);
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
				makeAngledCuboid(&collector,pos,box.m_box,tiles,6,c,txc,0);
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
						makeAngledCuboid(&collector, pos, box.m_box, tiles, 6,  c, txc, shown_angles[k]);
					}
				}
			}
			data->m_env->getMap().addNodeWithEvent(p+blockpos_nodes,n);
		}
		break;
		/*
			Add fence
		*/
		case CDT_FENCELIKE:
		{
			static const v3s16 tile_dirs[6] = {
				v3s16(0, 1, 0),
				v3s16(0, -1, 0),
				v3s16(1, 0, 0),
				v3s16(-1, 0, 0),
				v3s16(0, 0, 1),
				v3s16(0, 0, -1)
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
			video::SColor c[8];
			getLights(blockpos_nodes+p,c,data,smooth_lighting);

			v3f pos = intToFloat(p, BS);
			std::vector<NodeBox> boxes = content_features(n).getNodeBoxes(n);
			int bi = 1;
			v3s16 p2 = p;
			p2.Y++;
			MapNode n2 = data->m_vmanip.getNodeRO(blockpos_nodes + p2);
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
				makeAngledCuboid(&collector, pos, box.m_box, tiles, 6,  c, txc, 0);
			}

			int bps = ((boxes.size()-2)/4); // boxes per section
			u8 np = 1;

			for (int k=0; k<8; k++) {
				if (k > 3 && (shown_dirs[showcheck[k-4][0]] || shown_dirs[showcheck[k-4][1]]))
							continue;
				p2 = blockpos_nodes+p+fence_dirs[k];
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
						makeAngledCuboid(&collector, pos, box.m_box, tiles, 6,  c, txc, shown_angles[k]);
					}
				}
			}
			data->m_env->getMap().addNodeWithEvent(p+blockpos_nodes,n);
		}
		break;
		case CDT_WIRELIKE:
		{
			MapNode n_plus_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y+1,z));
			MapNode n_minus_x = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x-1,y,z));
			MapNode n_plus_x = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x+1,y,z));
			MapNode n_minus_z = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y,z-1));
			MapNode n_plus_z = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y,z+1));
			MapNode n_minus_xy = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x-1,y+1,z));
			MapNode n_plus_xy = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x+1,y+1,z));
			MapNode n_minus_zy = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y+1,z-1));
			MapNode n_plus_zy = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y+1,z+1));
			MapNode n_minus_x_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x-1,y-1,z));
			MapNode n_plus_x_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x+1,y-1,z));
			MapNode n_minus_z_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y-1,z-1));
			MapNode n_plus_z_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y-1,z+1));
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
			TileSpec tile = getNodeTile(n,p,v3s16(0,0,0),data->m_temp_mods);
			video::SColor c;
			if (selected) {
				c = video::SColor(255,64,64,255);
			}else{
				NodeMetadata *meta = data->m_env->getMap().getNodeMetadata(p+blockpos_nodes);
				if (meta && meta->getEnergy()) {
					u8 e = meta->getEnergy();
					e = (e*16)-1;
					if (e < 80)
						e = 80;
					c = video::SColor(255,e,e,e);
				}else{
					c = video::SColor(250,64,64,64);
				}
			}
			f32 sy = tile.texture.y1()-tile.texture.y0();
			if (!x_plus && !x_minus && !z_plus && !z_minus) {
				{
					video::S3DVertex vertices[4] = {
						video::S3DVertex(-BS/2,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()),
						video::S3DVertex(BS/2,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()),
						video::S3DVertex(BS/2,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()),
						video::S3DVertex(-BS/2,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
				{
					video::S3DVertex vertices[4] = {
						video::S3DVertex(-BS/2,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()),
						video::S3DVertex(BS/2,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()),
						video::S3DVertex(BS/2,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()),
						video::S3DVertex(-BS/2,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
			}else{
				if (x_plus) {
					video::S3DVertex vertices[4] = {
						video::S3DVertex(0,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()+(sy/2)),
						video::S3DVertex(BS/2,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()),
						video::S3DVertex(BS/2,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()),
						video::S3DVertex(0,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()+(sy/2)),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
				if (x_minus) {
					video::S3DVertex vertices[4] = {
						video::S3DVertex(-BS/2,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()),
						video::S3DVertex(0,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()-(sy/2)),
						video::S3DVertex(0,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()-(sy/2)),
						video::S3DVertex(-BS/2,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
				if (z_plus) {
					video::S3DVertex vertices[4] = {
						video::S3DVertex(-BS/2,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()),
						video::S3DVertex(BS/2,-0.49*BS,BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()),
						video::S3DVertex(BS/2,-0.49*BS,0, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()+(sy/2)),
						video::S3DVertex(-BS/2,-0.49*BS,0, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()+(sy/2)),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
				if (z_minus) {
					video::S3DVertex vertices[4] = {
						video::S3DVertex(-BS/2,-0.49*BS,0, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()-(sy/2)),
						video::S3DVertex(BS/2,-0.49*BS,0, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()-(sy/2)),
						video::S3DVertex(BS/2,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()),
						video::S3DVertex(-BS/2,-0.49*BS,-BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
				if (x_plus_y) {
					video::S3DVertex vertices[4] = {
						video::S3DVertex(0.49*BS,-BS/2,-BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()),
						video::S3DVertex(0.49*BS,-BS/2,BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()),
						video::S3DVertex(0.49*BS,BS/2,BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()),
						video::S3DVertex(0.49*BS,BS/2,-BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
				if (x_minus_y) {
					video::S3DVertex vertices[4] = {
						video::S3DVertex(-0.49*BS,-BS/2,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()),
						video::S3DVertex(-0.49*BS,-BS/2,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()),
						video::S3DVertex(-0.49*BS,BS/2,-BS/2, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()),
						video::S3DVertex(-0.49*BS,BS/2,BS/2, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
				if (z_plus_y) {
					video::S3DVertex vertices[4] = {
						video::S3DVertex(BS/2,-BS/2,0.49*BS, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()),
						video::S3DVertex(-BS/2,-BS/2,0.49*BS, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()),
						video::S3DVertex(-BS/2,BS/2,0.49*BS, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()),
						video::S3DVertex(BS/2,BS/2,0.49*BS, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
				if (z_minus_y) {
					video::S3DVertex vertices[4] = {
						video::S3DVertex(-BS/2,-BS/2,-0.49*BS, 0,0,0, c,
							tile.texture.x0(), tile.texture.y1()),
						video::S3DVertex(BS/2,-BS/2,-0.49*BS, 0,0,0, c,
							tile.texture.x1(), tile.texture.y1()),
						video::S3DVertex(BS/2,BS/2,-0.49*BS, 0,0,0, c,
							tile.texture.x1(), tile.texture.y0()),
						video::S3DVertex(-BS/2,BS/2,-0.49*BS, 0,0,0, c,
							tile.texture.x0(), tile.texture.y0()),
					};
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos += intToFloat(p, BS);
					}
					u16 indices[] = {0,1,2,2,3,0};
					// Add to mesh collector
					collector.append(tile.getMaterial(), vertices, 4, indices, 6);
				}
			}
		}
		break;
		case CDT_3DWIRELIKE:
		{
			MapNode n_plus_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y+1,z));
			MapNode n_minus_x = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x-1,y,z));
			MapNode n_plus_x = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x+1,y,z));
			MapNode n_minus_z = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y,z-1));
			MapNode n_plus_z = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y,z+1));
			MapNode n_minus_xy = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x-1,y+1,z));
			MapNode n_plus_xy = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x+1,y+1,z));
			MapNode n_minus_zy = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y+1,z-1));
			MapNode n_plus_zy = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y+1,z+1));
			MapNode n_minus_x_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x-1,y-1,z));
			MapNode n_plus_x_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x+1,y-1,z));
			MapNode n_minus_z_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y-1,z-1));
			MapNode n_plus_z_y = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y-1,z+1));
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
			TileSpec tile = getNodeTile(n,p,v3s16(0,0,0),data->m_temp_mods);
			video::SColor c;
			video::SColor c8[8];
			if (selected) {
				c = video::SColor(255,64,64,255);
			}else{
				NodeMetadata *meta = data->m_env->getMap().getNodeMetadata(p+blockpos_nodes);
				if (meta && meta->getEnergy()) {
					u8 e = meta->getEnergy();
					e = (e*16)-1;
					if (e < 80)
						e = 80;
					c = video::SColor(255,e,e,e);
				}else{
					c = video::SColor(250,64,64,64);
				}
			}
			for (int k=0; k<8; k++) {
				c8[k] = c;
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
				box.MinEdge += pos;
				box.MaxEdge += pos;

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
				makeCuboid(&collector, box, tiles, 6,  c8, txc);
			}
		}
		break;
		case CDT_RAILLIKE:
		{
			bool is_rail_x [] = { false, false };  /* x-1, x+1 */
			bool is_rail_z [] = { false, false };  /* z-1, z+1 */

			bool is_rail_z_minus_y [] = { false, false };  /* z-1, z+1; y-1 */
			bool is_rail_x_minus_y [] = { false, false };  /* x-1, z+1; y-1 */
			bool is_rail_z_plus_y [] = { false, false };  /* z-1, z+1; y+1 */
			bool is_rail_x_plus_y [] = { false, false };  /* x-1, x+1; y+1 */

			MapNode n_minus_x = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x-1,y,z));
			MapNode n_plus_x = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x+1,y,z));
			MapNode n_minus_z = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x,y,z-1));
			MapNode n_plus_z = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x,y,z+1));
			MapNode n_plus_x_plus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x+1, y+1, z));
			MapNode n_plus_x_minus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x+1, y-1, z));
			MapNode n_minus_x_plus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x-1, y+1, z));
			MapNode n_minus_x_minus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x-1, y-1, z));
			MapNode n_plus_z_plus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x, y+1, z+1));
			MapNode n_minus_z_plus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x, y+1, z-1));
			MapNode n_plus_z_minus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x, y-1, z+1));
			MapNode n_minus_z_minus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x, y-1, z-1));

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
			u8 tileindex = 0;

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
					tileindex = 1; // curved
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
				tileindex = 2; // t-junction
				if(!is_rail_x_all[1])
					angle=180;
				if(!is_rail_z_all[0])
					angle=90;
				if(!is_rail_z_all[1])
					angle=270;
				break;
			case 4:
				tileindex = 3; // crossing
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
			video::SColor c[8];
			getLights(blockpos_nodes+p,c,data,smooth_lighting);
			v3f pos = intToFloat(p,BS);

			switch (tileindex) {
			case 0:
			{
				if (is_rail_x_plus_y[0] || is_rail_x_plus_y[1] || is_rail_z_plus_y[0] || is_rail_z_plus_y[1]) {
					aabb3f track[7] = {
						aabb3f(-0.4375*BS,-0.03125*BS,-0.5625*BS,0.4375*BS,0.03125*BS,-0.4375*BS),
						aabb3f(-0.4375*BS,-0.03125*BS,-0.3125*BS,0.4375*BS,0.03125*BS,-0.1875*BS),
						aabb3f(-0.4375*BS,-0.03125*BS,-0.0625*BS,0.4375*BS,0.03125*BS,0.0625*BS),
						aabb3f(-0.4375*BS,-0.03125*BS,0.1875*BS,0.4375*BS,0.03125*BS,0.3125*BS),
						aabb3f(-0.4375*BS,-0.03125*BS,0.4375*BS,0.4375*BS,0.03125*BS,0.5625*BS),
						aabb3f(0.25*BS,0.03125*BS,-0.64*BS,0.3125*BS,0.09375*BS,0.77*BS),
						aabb3f(-0.3125*BS,0.03125*BS,-0.64*BS,-0.25*BS,0.09375*BS,0.77*BS)
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
					for (int bi=0; bi<7; bi++) {
						if (bi == 5)
							tile = &tiles[1];
						makeRotatedCuboid(&collector,pos,track[bi],tile,1,c,NULL,an);
					}
				}else{
					aabb3f track[6] = {
						aabb3f(-0.4375*BS,-0.5*BS,-0.4375*BS,0.4375*BS,-0.4375*BS,-0.3125*BS),
						aabb3f(-0.4375*BS,-0.5*BS,-0.1875*BS,0.4375*BS,-0.4375*BS,-0.0625*BS),
						aabb3f(-0.4375*BS,-0.5*BS,0.0625*BS,0.4375*BS,-0.4375*BS,0.1875*BS),
						aabb3f(-0.4375*BS,-0.5*BS,0.3125*BS,0.4375*BS,-0.4375*BS,0.4375*BS),
						aabb3f(-0.3125*BS,-0.4375*BS,-0.5*BS,-0.25*BS,-0.375*BS,0.5*BS),
						aabb3f(0.25*BS,-0.4375*BS,-0.5*BS,0.3125*BS,-0.375*BS,0.5*BS)
					};
					tile = &tiles[0];
					for (int bi=0; bi<6; bi++) {
						if (bi == 4)
							tile = &tiles[1];
						makeAngledCuboid(&collector,pos,track[bi],tile,1,c,NULL,angle);
					}
				}
				break;
			}
			case 1:
			{
				aabb3f track[20] = {
					aabb3f(-0.4375*BS,-0.5*BS,-0.4375*BS,0.4375*BS,-0.4375*BS,-0.3125*BS),
					aabb3f(-0.4375*BS,-0.5*BS,-0.3125*BS,-0.3125*BS,-0.4375*BS,0.4375*BS),
					aabb3f(-0.3125*BS,-0.5*BS,-0.3125*BS,-0.25*BS,-0.4375*BS,-0.1875*BS),
					aabb3f(-0.25*BS,-0.5*BS,-0.3125*BS,-0.1875*BS,-0.4375*BS,-0.125*BS),
					aabb3f(-0.1875*BS,-0.5*BS,-0.25*BS,-0.125*BS,-0.4375*BS,-0.0625*BS),
					aabb3f(-0.125*BS,-0.5*BS,-0.1875*BS,-0.0625*BS,-0.4375*BS,0.*BS),
					aabb3f(-0.0625*BS,-0.5*BS,-0.125*BS,0.*BS,-0.4375*BS,0.0625*BS),
					aabb3f(0.*BS,-0.5*BS,-0.0625*BS,0.0625*BS,-0.4375*BS,0.125*BS),
					aabb3f(0.0625*BS,-0.5*BS,0.*BS,0.125*BS,-0.4375*BS,0.1875*BS),
					aabb3f(0.125*BS,-0.5*BS,0.0625*BS,0.1875*BS,-0.4375*BS,0.25*BS),
					aabb3f(0.1875*BS,-0.5*BS,0.125*BS,0.25*BS,-0.4375*BS,0.3125*BS),
					aabb3f(0.25*BS,-0.5*BS,0.1875*BS,0.3125*BS,-0.4375*BS,0.25*BS),
					aabb3f(0.25*BS,-0.4375*BS,-0.5*BS,0.3125*BS,-0.375*BS,-0.0625*BS),
					aabb3f(-0.3125*BS,-0.4375*BS,-0.5*BS,-0.25*BS,-0.375*BS,-0.3125*BS),
					aabb3f(-0.5*BS,-0.4375*BS,-0.3125*BS,-0.3125*BS,-0.375*BS,-0.25*BS),
					aabb3f(-0.5*BS,-0.4375*BS,0.25*BS,-0.0625*BS,-0.375*BS,0.3125*BS),
					aabb3f(0.0625*BS,-0.4375*BS,0.125*BS,0.125*BS,-0.375*BS,0.1875*BS),
					aabb3f(0.1875*BS,-0.4375*BS,-0.0625*BS,0.25*BS,-0.375*BS,0.0625*BS),
					aabb3f(-0.0625*BS,-0.4375*BS,0.1875*BS,0.0625*BS,-0.375*BS,0.25*BS),
					aabb3f(0.125*BS,-0.4375*BS,0.0625*BS,0.1875*BS,-0.375*BS,0.125*BS)
				};
				tile = &tiles[0];
				for (int bi=0; bi<20; bi++) {
					if (bi == 12)
						tile = &tiles[1];
					makeAngledCuboid(&collector,pos,track[bi],tile,1,c,NULL,angle+90);
				}
				break;
			}
			case 2:
			{
				aabb3f track[17] = {
					aabb3f(-0.3125*BS,-0.5*BS,-0.4375*BS,0.4375*BS,-0.4375*BS,-0.3125*BS),
					aabb3f(-0.3125*BS,-0.5*BS,-0.1875*BS,0.4375*BS,-0.4375*BS,-0.0625*BS),
					aabb3f(-0.3125*BS,-0.5*BS,0.0625*BS,0.4375*BS,-0.4375*BS,0.1875*BS),
					aabb3f(-0.3125*BS,-0.5*BS,0.3125*BS,0.4375*BS,-0.4375*BS,0.4375*BS),
					aabb3f(-0.4375*BS,-0.5*BS,-0.4375*BS,-0.3125*BS,-0.4375*BS,0.4375*BS),
					aabb3f(0.25*BS,-0.4375*BS,-0.5*BS,0.3125*BS,-0.375*BS,0.5*BS),
					aabb3f(-0.3125*BS,-0.4375*BS,-0.5*BS,-0.25*BS,-0.375*BS,0.1875*BS),
					aabb3f(-0.3125*BS,-0.4375*BS,0.3125*BS,-0.25*BS,-0.375*BS,0.5*BS),
					aabb3f(-0.5*BS,-0.4375*BS,-0.3125*BS,-0.3125*BS,-0.375*BS,-0.25*BS),
					aabb3f(-0.5*BS,-0.4375*BS,0.25*BS,-0.25*BS,-0.375*BS,0.3125*BS),
					aabb3f(0.125*BS,-0.4375*BS,0.0625*BS,0.1875*BS,-0.375*BS,0.125*BS),
					aabb3f(0.0625*BS,-0.4375*BS,0.125*BS,0.125*BS,-0.375*BS,0.1875*BS),
					aabb3f(-0.0625*BS,-0.4375*BS,0.1875*BS,0.0625*BS,-0.375*BS,0.25*BS),
					aabb3f(0.1875*BS,-0.4375*BS,-0.0625*BS,0.25*BS,-0.375*BS,0.0625*BS),
					aabb3f(-0.1875*BS,-0.4375*BS,0.25*BS,-0.0625*BS,-0.375*BS,0.3125*BS),
					aabb3f(-0.1875*BS,-0.4375*BS,0.3125*BS,-0.125*BS,-0.375*BS,0.4375*BS),
					aabb3f(-0.4375*BS,-0.4375*BS,0.125*BS,-0.3125*BS,-0.375*BS,0.1875*BS)
				};
				tile = &tiles[0];
				for (int bi=0; bi<17; bi++) {
					if (bi == 5)
						tile = &tiles[1];
					makeAngledCuboid(&collector,pos,track[bi],tile,1,c,NULL,angle+180);
				}
				break;
			}
			case 3:
			{
				aabb3f track[20] = {
					aabb3f(-0.4375*BS,-0.5*BS,-0.4375*BS,0.4375*BS,-0.4375*BS,-0.3125*BS),
					aabb3f(-0.4375*BS,-0.5*BS,0.3125*BS,0.4375*BS,-0.4375*BS,0.4375*BS),
					aabb3f(-0.4375*BS,-0.5*BS,-0.3125*BS,-0.3125*BS,-0.4375*BS,0.3125*BS),
					aabb3f(0.3125*BS,-0.5*BS,-0.3125*BS,0.4375*BS,-0.4375*BS,0.3125*BS),
					aabb3f(-0.0625*BS,-0.5*BS,-0.3125*BS,0.0625*BS,-0.4375*BS,0.3125*BS),
					aabb3f(0.0625*BS,-0.5*BS,-0.0625*BS,0.3125*BS,-0.4375*BS,0.0625*BS),
					aabb3f(-0.3125*BS,-0.5*BS,-0.0625*BS,-0.0625*BS,-0.4375*BS,0.0625*BS),
					aabb3f(-0.3125*BS,-0.4375*BS,-0.5*BS,-0.25*BS,-0.375*BS,-0.25*BS),
					aabb3f(0.25*BS,-0.4375*BS,-0.5*BS,0.3125*BS,-0.375*BS,-0.25*BS),
					aabb3f(-0.5*BS,-0.4375*BS,0.25*BS,-0.3125*BS,-0.375*BS,0.3125*BS),
					aabb3f(-0.5*BS,-0.4375*BS,-0.3125*BS,-0.3125*BS,-0.375*BS,-0.25*BS),
					aabb3f(0.3125*BS,-0.4375*BS,-0.3125*BS,0.5*BS,-0.375*BS,-0.25*BS),
					aabb3f(0.3125*BS,-0.4375*BS,0.25*BS,0.5*BS,-0.375*BS,0.3125*BS),
					aabb3f(-0.1875*BS,-0.4375*BS,0.25*BS,0.1875*BS,-0.375*BS,0.3125*BS),
					aabb3f(-0.1875*BS,-0.4375*BS,-0.3125*BS,0.1875*BS,-0.375*BS,-0.25*BS),
					aabb3f(0.25*BS,-0.4375*BS,0.25*BS,0.3125*BS,-0.375*BS,0.5*BS),
					aabb3f(0.25*BS,-0.4375*BS,-0.1875*BS,0.3125*BS,-0.375*BS,0.1875*BS),
					aabb3f(-0.3125*BS,-0.4375*BS,0.25*BS,-0.25*BS,-0.375*BS,0.5*BS),
					aabb3f(-0.3125*BS,-0.4375*BS,-0.1875*BS,-0.25*BS,-0.375*BS,0.1875*BS),
					aabb3f(-0.1875*BS,-0.4375*BS,-0.1875*BS,0.1875*BS,-0.375*BS,0.1875*BS)
				};
				tile = &tiles[0];
				for (int bi=0; bi<20; bi++) {
					if (bi == 7)
						tile = &tiles[1];
					makeAngledCuboid(&collector,pos,track[bi],tile,1,c,NULL,angle);
				}
				break;
			}
			default:;
			}
		}
		break;
		case CDT_ROOFLIKE:
		{
			bool is_roof_x [] = { false, false };  /* x-1, x+1 */
			bool is_roof_z [] = { false, false };  /* z-1, z+1 */

			bool is_roof_z_minus_y [] = { false, false };  /* z-1, z+1; y-1 */
			bool is_roof_x_minus_y [] = { false, false };  /* x-1, z+1; y-1 */
			bool is_roof_z_plus_y [] = { false, false };  /* z-1, z+1; y+1 */
			bool is_roof_x_plus_y [] = { false, false };  /* x-1, x+1; y+1 */

			MapNode n_minus_x = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x-1,y,z));
			MapNode n_plus_x = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x+1,y,z));
			MapNode n_minus_z = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x,y,z-1));
			MapNode n_plus_z = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x,y,z+1));
			MapNode n_plus_x_plus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x+1, y+1, z));
			MapNode n_plus_x_minus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x+1, y-1, z));
			MapNode n_minus_x_plus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x-1, y+1, z));
			MapNode n_minus_x_minus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x-1, y-1, z));
			MapNode n_plus_z_plus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x, y+1, z+1));
			MapNode n_minus_z_plus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x, y+1, z-1));
			MapNode n_plus_z_minus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x, y-1, z+1));
			MapNode n_minus_z_minus_y = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x, y-1, z-1));

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

			bool is_roof_x_all[] = {false, false};
			bool is_roof_z_all[] = {false, false};
			is_roof_x_all[0] = is_roof_x[0] || is_roof_x_minus_y[0] || is_roof_x_plus_y[0];
			is_roof_x_all[1] = is_roof_x[1] || is_roof_x_minus_y[1] || is_roof_x_plus_y[1];
			is_roof_z_all[0] = is_roof_z[0] || is_roof_z_minus_y[0] || is_roof_z_plus_y[0];
			is_roof_z_all[1] = is_roof_z[1] || is_roof_z_minus_y[1] || is_roof_z_plus_y[1];

			u8 adjacencies = is_roof_x[0] + is_roof_x[1] + is_roof_z[0] + is_roof_z[1];

			// get the tile, with crack if being dug
			TileSpec tile = getNodeTile(n,p,v3s16(0,1,0),data->m_temp_mods);
			video::SColor c[2];
			getRoofLights(blockpos_nodes+p,c,data,v3s16(0,0,0));
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
						abv = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x-1, y+1, z-1));
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
						abv = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x-1, y+1, z+1));
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
						abv = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x+1, y+1, z-1));
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
						abv = data->m_vmanip.getNodeRO(blockpos_nodes + v3s16(x+1, y+1, z+1));
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,-1));
				}else if (angle == 90) {
					cnr[0][0] = v3f(-0.5,0.5,-0.5);
					cnr[0][1] = v3f(0.5,-0.5,-0.5);
					cnr[0][2] = v3f(0.5,-0.5,0.5);
					cnr[1][0] = v3f(0.5,-0.5,0.5);
					cnr[1][1] = v3f(-0.5,0.5,0.5);
					cnr[1][2] = v3f(-0.5,0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(1,1,0));
				}else if (angle == 180) {
					cnr[0][0] = v3f(-0.5,0.5,-0.5);
					cnr[0][1] = v3f(0.5,0.5,-0.5);
					cnr[0][2] = v3f(0.5,-0.5,0.5);
					cnr[1][0] = v3f(0.5,-0.5,0.5);
					cnr[1][1] = v3f(-0.5,-0.5,0.5);
					cnr[1][2] = v3f(-0.5,0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,1));
				}else if (angle == 270) {
					cnr[0][0] = v3f(-0.5,-0.5,-0.5);
					cnr[0][1] = v3f(0.5,0.5,-0.5);
					cnr[0][2] = v3f(0.5,0.5,0.5);
					cnr[1][0] = v3f(0.5,0.5,0.5);
					cnr[1][1] = v3f(-0.5,-0.5,0.5);
					cnr[1][2] = v3f(-0.5,-0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(-1,1,0));
				}
				s16 a = 180-angle;
				if (a < 0)
					a += 360;
				for (int s=0; s<2; s++) {
					makeRoofTri(&collector,cnr[s],pos,&tile,1,c,a);
				}
			}
			break;
			case 1:
			{
				v3f cnr[4][3];
				getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,0));
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
					makeRoofTri(&collector,cnr[s],pos,&toptile,1,c,a);
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,-1));
				}else if (angle == 180) {
					cnr[0][0] = v3f(-0.5,0.5,-0.5);
					cnr[0][1] = v3f(0.5,-0.5,-0.5);
					cnr[0][2] = v3f(0.5,-0.5,0.5);
					cnr[1][0] = v3f(0.5,-0.5,0.5);
					cnr[1][1] = v3f(-0.5,0.5,0.5);
					cnr[1][2] = v3f(-0.5,0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(1,1,0));
				}else if (angle == 270) {
					cnr[0][0] = v3f(-0.5,0.5,-0.5);
					cnr[0][1] = v3f(0.5,0.5,-0.5);
					cnr[0][2] = v3f(0.5,-0.5,0.5);
					cnr[1][0] = v3f(0.5,-0.5,0.5);
					cnr[1][1] = v3f(-0.5,-0.5,0.5);
					cnr[1][2] = v3f(-0.5,0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,1));
				}else if (angle == 0) {
					cnr[0][0] = v3f(-0.5,-0.5,-0.5);
					cnr[0][1] = v3f(0.5,0.5,-0.5);
					cnr[0][2] = v3f(0.5,0.5,0.5);
					cnr[1][0] = v3f(0.5,0.5,0.5);
					cnr[1][1] = v3f(-0.5,-0.5,0.5);
					cnr[1][2] = v3f(-0.5,-0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(-1,1,0));
				}
				s16 a = 270-angle;
				if (a < 0)
					a += 360;
				for (int s=0; s<2; s++) {
					makeRoofTri(&collector,cnr[s],pos,&tile,1,c,a);
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(-1,1,0));
				}else if (angle == 90) {
					cnr[0][0] = v3f(0.,0.,0.);
					cnr[0][1] = v3f(0.,0.,-0.5);
					cnr[0][2] = v3f(-0.5,-0.5,-0.5);
					cnr[1][0] = v3f(0.,0.,0.);
					cnr[1][1] = v3f(0.5,-0.5,-0.5);
					cnr[1][2] = v3f(0.,0.,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,-1));
				}else if (angle == 180) {
					cnr[0][0] = v3f(0.,0.,0.);
					cnr[0][1] = v3f(0.5,0.,0.);
					cnr[0][2] = v3f(0.5,-0.5,-0.5);
					cnr[1][0] = v3f(0.,0.,0.);
					cnr[1][1] = v3f(0.5,-0.5,0.5);
					cnr[1][2] = v3f(0.5,0.,0.);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(1,1,0));
				}else if (angle == 270) {
					cnr[0][0] = v3f(0.,0.,0.);
					cnr[0][1] = v3f(0.,0.,0.5);
					cnr[0][2] = v3f(-0.5,-0.5,0.5);
					cnr[1][0] = v3f(0.,0.,0.);
					cnr[1][1] = v3f(0.5,-0.5,0.5);
					cnr[1][2] = v3f(0.,0.,0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,1));
				}
				s16 a = angle;
				if (a < 180)
					a += 180;
				for (int s=0; s<2; s++) {
					makeRoofTri(&collector,cnr[s],pos,&toptile,1,c,a);
					a -= 180;
				}
			}
			break;
			case 3:
			{
				v3f cnr[2][3];
				getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,0));
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
					makeRoofTri(&collector,cnr[s],pos,&toptile,1,c,a);
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
					makeRoofTri(&collector,cnr[s],pos,&toptile,1,c,a);
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(1,1,-1));
					a1 = 180;
					a2 = 90;
				}else if (angle == 90) {
					cnr[0][0] = v3f(-0.5,-0.5,0.5);
					cnr[0][1] = v3f(0.5,-0.5,0.5);
					cnr[0][2] = v3f(-0.5,0.5,-0.5);
					cnr[1][0] = v3f(-0.5,0.5,-0.5);
					cnr[1][1] = v3f(0.5,-0.5,0.5);
					cnr[1][2] = v3f(0.5,-0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(1,1,1));
					a1 = 0;
					a2 = 90;
				}else if (angle == 180) {
					cnr[0][0] = v3f(-0.5,-0.5,-0.5);
					cnr[0][1] = v3f(0.5,0.5,-0.5);
					cnr[0][2] = v3f(-0.5,-0.5,0.5);
					cnr[1][0] = v3f(-0.5,-0.5,0.5);
					cnr[1][1] = v3f(0.5,0.5,-0.5);
					cnr[1][2] = v3f(0.5,-0.5,0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(-1,1,1));
					a1 = 270;
					a2 = 0;
				}else if (angle == 270) {
					cnr[0][0] = v3f(-0.5,-0.5,0.5);
					cnr[0][1] = v3f(0.5,0.5,0.5);
					cnr[0][2] = v3f(-0.5,-0.5,-0.5);
					cnr[1][0] = v3f(-0.5,-0.5,-0.5);
					cnr[1][1] = v3f(0.5,0.5,0.5);
					cnr[1][2] = v3f(0.5,-0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(-1,1,-1));
				}
				s16 a = a1;
				for (int s=0; s<2; s++) {
					makeRoofTri(&collector,cnr[s],pos,&tile,1,c,a);
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
				getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,0));
				for (int s=0; s<8; s++) {
					makeRoofTri(&collector,cnr[s],pos,&toptile,1,c,a[s]);
				}
			}
			break;
			case 6:
			{
				v3f cnr[6][3];
				getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,0));
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
					makeRoofTri(&collector,cnr[s],pos,&toptile,1,c,a[s]);
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(-1,1,1));
				}else if (angle == 90) {
					cnr[0][0] = v3f(0.5,0.5,0.5);
					cnr[0][1] = v3f(-0.5,0.5,0.5);
					cnr[0][2] = v3f(-0.5,-0.5,-0.5);
					cnr[1][0] = v3f(0.5,0.5,0.5);
					cnr[1][1] = v3f(0.5,0.5,-0.5);
					cnr[1][2] = v3f(-0.5,-0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(-1,1,-1));
					a1 = 180;
					a2 = 270;
				}else if (angle == 180) {
					cnr[0][0] = v3f(0.5,-0.5,-0.5);
					cnr[0][1] = v3f(-0.5,0.5,-0.5);
					cnr[0][2] = v3f(-0.5,0.5,0.5);
					cnr[1][0] = v3f(0.5,-0.5,-0.5);
					cnr[1][1] = v3f(0.5,0.5,0.5);
					cnr[1][2] = v3f(-0.5,0.5,0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(1,1,-1));
					a1 = 90;
					a2 = 180;
				}else if (angle == 270) {
					cnr[0][0] = v3f(0.5,-0.5,0.5);
					cnr[0][1] = v3f(-0.5,0.5,0.5);
					cnr[0][2] = v3f(-0.5,0.5,-0.5);
					cnr[1][0] = v3f(0.5,-0.5,0.5);
					cnr[1][1] = v3f(0.5,0.5,-0.5);
					cnr[1][2] = v3f(-0.5,0.5,-0.5);
					getRoofLights(blockpos_nodes+p,c,data,v3s16(1,1,1));
					a1 = 90;
					a2 = 0;
				}
				s16 a = a1;
				for (int s=0; s<2; s++) {
					makeRoofTri(&collector,cnr[s],pos,&tile,1,c,a);
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
				getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,0));
				for (int s=0; s<4; s++) {
					makeRoofTri(&collector,cnr[s],pos,&toptile,1,c,(90*s)+90+(180*(!(s%2))));
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(1,1,0));
					a[0] = 180;
					a[1] = 180;
					a[4] = 90;
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,1));
					a[0] = 270;
					a[1] = 270;
					a[2] = 90;
					a[3] = 90;
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(-1,1,0));
					a[0] = 180;
					a[1] = 180;
					a[4] = 270;
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
					getRoofLights(blockpos_nodes+p,c,data,v3s16(0,1,-1));
					a[0] = 270;
					a[1] = 270;
					a[2] = 90;
					a[3] = 90;
					a[4] = 180;
				}
				for (int s=0; s<5; s++) {
					makeRoofTri(&collector,cnr[s],pos,&toptile,1,c,a[s]);
				}
			}
			break;
			default:
				break;
			}
		}
		break;
		case CDT_PLANTLIKE_LGE:
		{
			MapNode n2 = data->m_vmanip.getNodeRO(blockpos_nodes + p + v3s16(0,1,0));
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l, selected);
			f32 tuv[4] = {
				content_features(n).tiles[0].texture.x0(),
				content_features(n).tiles[0].texture.x1(),
				content_features(n).tiles[0].texture.y0(),
				content_features(n).tiles[0].texture.y1()
			};
			s32 h = 1;
			f32 s = 1.3;
			if (
				content_features(n2).draw_type == CDT_PLANTLIKE_LGE
				|| content_features(n2).draw_type == CDT_PLANTLIKE
				|| content_features(n2).draw_type == CDT_PLANTLIKE_SML
			) {
				tuv[2] = (0.333*content_features(n).tiles[0].texture.size.Y)+content_features(n).tiles[0].texture.y0();
				h = 2;
				s = 1.0;
			}
			video::S3DVertex base_vertices[4] = {
				base_vertices[0] = video::S3DVertex(-BS/2,-BS/2,0, 0,0,0, c,tuv[0], tuv[3]),
				base_vertices[1] = video::S3DVertex(BS/2,-BS/2,0, 0,0,0, c,tuv[1], tuv[3]),
				base_vertices[2] = video::S3DVertex(BS/2,BS/h,0, 0,0,0, c,tuv[1], tuv[2]),
				base_vertices[3] = video::S3DVertex(-BS/2,BS/h,0, 0,0,0, c,tuv[0], tuv[2])
			};
			for (u32 j=0; j<2; j++) {
				video::S3DVertex vertices[4] = {
					base_vertices[0],
					base_vertices[1],
					base_vertices[2],
					base_vertices[3]
				};

				switch (j) {
				case 0:
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(45);
					}
					break;
				case 1:
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(-45);
					}
					break;
				case 2:
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(135);
					}
					break;
				case 3:
					for (u16 i=0; i<4; i++) {
						vertices[i].Pos.rotateXZBy(-135);
					}
					break;
				default:;
				}

				for (u16 i=0; i<4; i++) {
					vertices[i].Pos.X *= 1.3;
					vertices[i].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_PLANTLIKE:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l, selected);

			for(u32 j=0; j<2; j++)
			{
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y0()),
					video::S3DVertex(-BS/2,BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y0()),
				};

				if(j == 0)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(45);
				}
				else if(j == 1)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-45);
				}
				else if(j == 2)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(135);
				}
				else if(j == 3)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-135);
				}

				for(u16 i=0; i<4; i++)
				{
					vertices[i].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_PLANTLIKE_SML:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l, selected);

			for(u32 j=0; j<2; j++)
			{
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y0()),
					video::S3DVertex(-BS/2,BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y0()),
				};

				if(j == 0)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(45);
				}
				else if(j == 1)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-45);
				}
				else if(j == 2)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(135);
				}
				else if(j == 3)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-135);
				}

				for(u16 i=0; i<4; i++)
				{
					vertices[i].Pos *= 0.8;
					vertices[i].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_PLANTGROWTH_1:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l, selected);
			f32 h = (0.75*content_features(n).tiles[0].texture.size.Y)+content_features(n).tiles[0].texture.y0();

			for(u32 j=0; j<2; j++)
			{
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-0.5*BS,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.5*BS,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.25*BS,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), h),
					video::S3DVertex(-BS/2,-0.25*BS,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), h),
				};

				if(j == 0)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(45);
				}
				else if(j == 1)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-45);
				}
				else if(j == 2)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(135);
				}
				else if(j == 3)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-135);
				}

				for(u16 i=0; i<4; i++)
				{
					vertices[i].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_PLANTGROWTH_2:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l, selected);
			f32 h = (0.5*content_features(n).tiles[0].texture.size.Y)+content_features(n).tiles[0].texture.y0();

			for(u32 j=0; j<2; j++)
			{
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,0,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), h),
					video::S3DVertex(-BS/2,0,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), h),
				};

				if(j == 0)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(45);
				}
				else if(j == 1)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-45);
				}
				else if(j == 2)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(135);
				}
				else if(j == 3)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-135);
				}

				for(u16 i=0; i<4; i++)
				{
					vertices[i].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_PLANTGROWTH_3:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l, selected);
			f32 h = (0.25*content_features(n).tiles[0].texture.size.Y)+content_features(n).tiles[0].texture.y0();

			for(u32 j=0; j<2; j++)
			{
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-0.5*BS,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-0.5*BS,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,0.25*BS,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), h),
					video::S3DVertex(-BS/2,0.25*BS,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), h),
				};

				if(j == 0)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(45);
				}
				else if(j == 1)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-45);
				}
				else if(j == 2)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(135);
				}
				else if(j == 3)
				{
					for(u16 i=0; i<4; i++)
						vertices[i].Pos.rotateXZBy(-135);
				}

				for(u16 i=0; i<4; i++)
				{
					vertices[i].Pos += intToFloat(p, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_FIRELIKE:
		{
			TileSpec tile = getNodeTile(n,p,v3s16(0,1,0),data->m_temp_mods);
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l, selected);
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
				n2p = blockpos_nodes + p + dirs[i];
				n2 = data->m_vmanip.getNodeRO(n2p);
				n2c = n2.getContent();
				if (n2c != CONTENT_IGNORE && n2c != CONTENT_AIR && n2c != current) {
					doDraw[i] = 1;
					drawAllFaces = false;
				}
			}
			for (u32 j=0; j<4; j++) {
				video::S3DVertex vertices[4] = {
					video::S3DVertex(-0.5*BS,-0.5*BS,0.369*BS, 0,0,0, c, tile.texture.x0(), tile.texture.y1()),
					video::S3DVertex(0.5*BS,-0.5*BS,0.369*BS, 0,0,0, c, tile.texture.x1(), tile.texture.y1()),
					video::S3DVertex(0.5*BS,0.5*BS,0.369*BS, 0,0,0, c, tile.texture.x1(), tile.texture.y0()),
					video::S3DVertex(-0.5*BS,0.5*BS,0.369*BS, 0,0,0, c, tile.texture.x0(), tile.texture.y0())
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
				for (u16 i=0; i<4; i++) {
					vertices[i].Pos += intToFloat(p, BS);
				}
				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(tile.getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_NODEBOX:
		case CDT_NODEBOX_META:
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
			video::SColor c[8];
			getLights(blockpos_nodes+p,c,data,smooth_lighting);

			v3f pos = intToFloat(p, BS);
			std::vector<NodeBox> boxes = content_features(n).getNodeBoxes(n);
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
				makeRotatedCuboid(&collector, pos, box.m_box, tiles, 6, c, txc, box.m_angle);
			}
			if (content_features(n).draw_type == CDT_NODEBOX_META) {
				NodeMetadata *meta = data->m_env->getMap().getNodeMetadata(p+blockpos_nodes);
				if (meta == NULL)
					break;
				boxes = meta->getNodeBoxes(n);
				if (boxes.size() > 0) {
					for (int i = 0; i < 6; i++) {
						// Handles facedir rotation for textures
						tiles[i] = getMetaTile(n,p,tile_dirs[i],data->m_temp_mods);
					}
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
						makeRotatedCuboid(&collector, pos, box.m_box, tiles, 6, c, txc, box.m_angle);
					}
				}
			}
		}
		break;
		}
	}
}
#endif

