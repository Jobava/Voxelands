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
void makeCuboid(MeshCollector *collector, const aabb3f &box,
	TileSpec *tiles, int tilecount,
	video::SColor &c, const f32* txc)
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
		video::S3DVertex(min.X,max.Y,max.Z, 0,1,0, c, txc[0],txc[1]),
		video::S3DVertex(max.X,max.Y,max.Z, 0,1,0, c, txc[2],txc[1]),
		video::S3DVertex(max.X,max.Y,min.Z, 0,1,0, c, txc[2],txc[3]),
		video::S3DVertex(min.X,max.Y,min.Z, 0,1,0, c, txc[0],txc[3]),
		// down
		video::S3DVertex(min.X,min.Y,min.Z, 0,-1,0, c, txc[4],txc[5]),
		video::S3DVertex(max.X,min.Y,min.Z, 0,-1,0, c, txc[6],txc[5]),
		video::S3DVertex(max.X,min.Y,max.Z, 0,-1,0, c, txc[6],txc[7]),
		video::S3DVertex(min.X,min.Y,max.Z, 0,-1,0, c, txc[4],txc[7]),
		// right
		video::S3DVertex(max.X,max.Y,min.Z, 1,0,0, c, txc[ 8],txc[9]),
		video::S3DVertex(max.X,max.Y,max.Z, 1,0,0, c, txc[10],txc[9]),
		video::S3DVertex(max.X,min.Y,max.Z, 1,0,0, c, txc[10],txc[11]),
		video::S3DVertex(max.X,min.Y,min.Z, 1,0,0, c, txc[ 8],txc[11]),
		// left
		video::S3DVertex(min.X,max.Y,max.Z, -1,0,0, c, txc[12],txc[13]),
		video::S3DVertex(min.X,max.Y,min.Z, -1,0,0, c, txc[14],txc[13]),
		video::S3DVertex(min.X,min.Y,min.Z, -1,0,0, c, txc[14],txc[15]),
		video::S3DVertex(min.X,min.Y,max.Z, -1,0,0, c, txc[12],txc[15]),
		// back
		video::S3DVertex(max.X,max.Y,max.Z, 0,0,1, c, txc[16],txc[17]),
		video::S3DVertex(min.X,max.Y,max.Z, 0,0,1, c, txc[18],txc[17]),
		video::S3DVertex(min.X,min.Y,max.Z, 0,0,1, c, txc[18],txc[19]),
		video::S3DVertex(max.X,min.Y,max.Z, 0,0,1, c, txc[16],txc[19]),
		// front
		video::S3DVertex(min.X,max.Y,min.Z, 0,0,-1, c, txc[20],txc[21]),
		video::S3DVertex(max.X,max.Y,min.Z, 0,0,-1, c, txc[22],txc[21]),
		video::S3DVertex(max.X,min.Y,min.Z, 0,0,-1, c, txc[22],txc[23]),
		video::S3DVertex(min.X,min.Y,min.Z, 0,0,-1, c, txc[20],txc[23]),
	};


	for (s32 j=0; j<24; j++) {
		int tileindex = MYMIN(j/4, tilecount-1);
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
#endif

#ifndef SERVER
static void mapblock_mesh_check_walllike(MeshMakeData *data, v3s16 p, u8 d[9])
{
	d[5] = 0;
	// Now a section of fence, +X, if there's a post there
	v3s16 p2 = p;
	p2.X++;
	MapNode n2 = data->m_vmanip.getNodeNoEx(p2);
	const ContentFeatures *f2 = &content_features(n2);
	if (
		f2->draw_type == CDT_FENCELIKE
		|| f2->draw_type == CDT_WALLLIKE
		|| n2.getContent() == CONTENT_WOOD_GATE
		|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
		|| n2.getContent() == CONTENT_STEEL_GATE
		|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
	) {
		d[0] = 1;
	}else{
		d[0] = 0;
	}

	// Now a section of fence, -X, if there's a post there
	p2 = p;
	p2.X--;
	n2 = data->m_vmanip.getNodeNoEx(p2);
	f2 = &content_features(n2);
	if (
		f2->draw_type == CDT_FENCELIKE
		|| f2->draw_type == CDT_WALLLIKE
		|| n2.getContent() == CONTENT_WOOD_GATE
		|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
		|| n2.getContent() == CONTENT_STEEL_GATE
		|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
	) {
		d[1] = 1;
	}else{
		d[1] = 0;
	}

	// Now a section of fence, +Z, if there's a post there
	p2 = p;
	p2.Z++;
	n2 = data->m_vmanip.getNodeNoEx(p2);
	f2 = &content_features(n2);
	if (
		f2->draw_type == CDT_FENCELIKE
		|| f2->draw_type == CDT_WALLLIKE
		|| n2.getContent() == CONTENT_WOOD_GATE
		|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
		|| n2.getContent() == CONTENT_STEEL_GATE
		|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
	) {
		d[2] = 1;
	}else{
		d[2] = 0;
	}

	// Now a section of fence, +Z, if there's a post there
	p2 = p;
	p2.Z--;
	n2 = data->m_vmanip.getNodeNoEx(p2);
	f2 = &content_features(n2);
	if (
		f2->draw_type == CDT_FENCELIKE
		|| f2->draw_type == CDT_WALLLIKE
		|| n2.getContent() == CONTENT_WOOD_GATE
		|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
		|| n2.getContent() == CONTENT_STEEL_GATE
		|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
	) {
		d[3] = 1;
	}else{
		d[3] = 0;
	}
	d[4] = d[0]+d[1]+d[2]+d[3];
	p2 = p;
	p2.Y++;
	n2 = data->m_vmanip.getNodeNoEx(p2);
	u8 ad[6];
	d[5] = 0;
	d[6] = 0;
	d[7] = 0;
	d[8] = 0;
	if (content_features(n2).draw_type == CDT_WALLLIKE) {
		mapblock_mesh_check_walllike(data, p2,ad);
		if (ad[0] && d[0])
			d[5] = 1;
		if (ad[1] && d[1])
			d[6] = 1;
		if (ad[2] && d[2])
			d[7] = 1;
		if (ad[3] && d[3])
			d[8] = 1;
	}
	if (d[4] == 2) {
		d[4] = 0;
		if (d[0] && d[2]) {
			d[4] = 1;
		}else if (d[1] && d[3]) {
			d[4] = 1;
		}else if (d[0] && d[3]) {
			d[4] = 1;
		}else if (d[1] && d[2]) {
			d[4] = 1;
		}else if (content_features(n2).draw_type == CDT_WALLLIKE) {
			d[4] = ad[4];
		}else if (n2.getContent() != CONTENT_AIR && n2.getContent() != CONTENT_IGNORE) {
			d[4] = 1;
		}
	}else{
		d[4] = 1;
	}
}

void mapblock_mesh_generate_special(MeshMakeData *data,
		MeshCollector &collector)
{
	/*
		Some settings
	*/
	bool new_style_water = g_settings->getBool("new_style_water");

	float node_liquid_level = 1.0;
	if(new_style_water)
		node_liquid_level = 0.85;

	v3s16 blockpos_nodes = data->m_blockpos*MAP_BLOCKSIZE;

	for(s16 z=0; z<MAP_BLOCKSIZE; z++)
	for(s16 y=0; y<MAP_BLOCKSIZE; y++)
	for(s16 x=0; x<MAP_BLOCKSIZE; x++)
	{
		v3s16 p(x,y,z);

		MapNode n = data->m_vmanip.getNodeNoEx(blockpos_nodes+p);

		/*
			Add torches to mesh
		*/
		switch (content_features(n).draw_type) {
		case CDT_CUBELIKE:
		case CDT_AIRLIKE:
			break;
		case CDT_LIQUID:
		{
			assert(content_features(n).special_material);
			video::SMaterial &liquid_material =
					*content_features(n).special_material;
			video::SMaterial &liquid_material_bfculled =
					*content_features(n).special_material2;

			assert(content_features(n).special_atlas);
			AtlasPointer &pa_liquid1 =
					*content_features(n).special_atlas;

			bool top_is_same_liquid = false;
			MapNode ntop = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y+1,z));
			content_t c_flowing = content_features(n).liquid_alternative_flowing;
			content_t c_source = content_features(n).liquid_alternative_source;
			if(ntop.getContent() == c_flowing || ntop.getContent() == c_source)
				top_is_same_liquid = true;

			u8 l = 0;
			// Use the light of the node on top if possible
			if(content_features(ntop).param_type == CPT_LIGHT)
				l = decode_light(ntop.getLightBlend(data->m_daynight_ratio));
			// Otherwise use the light of this node (the liquid)
			else
				l = decode_light(n.getLightBlend(data->m_daynight_ratio));
			video::SColor c = MapBlock_LightColor(
					content_features(n).vertex_alpha, l);

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
				MapNode n2 = data->m_vmanip.getNodeNoEx(blockpos_nodes + p2);
				if(n2.getContent() != CONTENT_IGNORE)
				{
					content = n2.getContent();

					if(n2.getContent() == c_source)
						level = (-0.5+node_liquid_level) * BS;
					else if(n2.getContent() == c_flowing)
						level = (-0.5 + ((float)(n2.param2&LIQUID_LEVEL_MASK)
								+ 0.5) / 8.0 * node_liquid_level) * BS;

					// Check node above neighbor.
					// NOTE: This doesn't get executed if neighbor
					//       doesn't exist
					p2.Y += 1;
					n2 = data->m_vmanip.getNodeNoEx(blockpos_nodes + p2);
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

				// Use backface culled material if neighbor doesn't have a
				// solidness of 0
				video::SMaterial *current_material = &liquid_material;
				if(n_feat.solidness != 0 || n_feat.visual_solidness != 0)
					current_material = &liquid_material_bfculled;

				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,0,BS/2, 0,0,0, c,
							pa_liquid1.x0(), pa_liquid1.y1()),
					video::S3DVertex(BS/2,0,BS/2, 0,0,0, c,
							pa_liquid1.x1(), pa_liquid1.y1()),
					video::S3DVertex(BS/2,0,BS/2, 0,0,0, c,
							pa_liquid1.x1(), pa_liquid1.y0()),
					video::S3DVertex(-BS/2,0,BS/2, 0,0,0, c,
							pa_liquid1.x0(), pa_liquid1.y0()),
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

					vertices[j].Pos += intToFloat(p + blockpos_nodes, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(*current_material, vertices, 4, indices, 6);
			}

			/*
				Generate top side, if appropriate
			*/

			if(top_is_same_liquid == false)
			{
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,0,BS/2, 0,0,0, c,
							pa_liquid1.x0(), pa_liquid1.y1()),
					video::S3DVertex(BS/2,0,BS/2, 0,0,0, c,
							pa_liquid1.x1(), pa_liquid1.y1()),
					video::S3DVertex(BS/2,0,-BS/2, 0,0,0, c,
							pa_liquid1.x1(), pa_liquid1.y0()),
					video::S3DVertex(-BS/2,0,-BS/2, 0,0,0, c,
							pa_liquid1.x0(), pa_liquid1.y0()),
				};

				// This fixes a strange bug
				s32 corner_resolve[4] = {3,2,1,0};

				for(s32 i=0; i<4; i++)
				{
					s32 j = corner_resolve[i];
					vertices[i].Pos.Y += corner_levels[j];
					vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(liquid_material, vertices, 4, indices, 6);
			}
		}
		/*
			Add water sources to mesh if using new style
		*/
		break;
		case CDT_LIQUID_SOURCE:
		if (new_style_water)
		{
			assert(content_features(n).special_material);
			video::SMaterial &liquid_material =
					*content_features(n).special_material;
			assert(content_features(n).special_atlas);
			AtlasPointer &pa_liquid1 =
					*content_features(n).special_atlas;

			MapNode n = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y+1,z));
			if(n.getContent() != CONTENT_AIR)
				continue;

			u8 l = decode_light(n.getLightBlend(data->m_daynight_ratio));
			video::SColor c = MapBlock_LightColor(
					content_features(n).vertex_alpha, l);

			video::S3DVertex vertices[4] =
			{
				video::S3DVertex(-BS/2,0,BS/2, 0,0,0, c,
						pa_liquid1.x0(), pa_liquid1.y1()),
				video::S3DVertex(BS/2,0,BS/2, 0,0,0, c,
						pa_liquid1.x1(), pa_liquid1.y1()),
				video::S3DVertex(BS/2,0,-BS/2, 0,0,0, c,
						pa_liquid1.x1(), pa_liquid1.y0()),
				video::S3DVertex(-BS/2,0,-BS/2, 0,0,0, c,
						pa_liquid1.x0(), pa_liquid1.y0()),
			};

			for(s32 i=0; i<4; i++)
			{
				vertices[i].Pos.Y += (-0.5+node_liquid_level)*BS;
				vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
			}

			u16 indices[] = {0,1,2,2,3,0};
			// Add to mesh collector
			collector.append(liquid_material, vertices, 4, indices, 6);
		}
		break;
		case CDT_TORCHLIKE:
		{
			static const f32 txc[24] = {
				0.6,0.1,0.9,0.4,
				0.6,0.6,0.6,0.9,
				0,0,0.3,1,
				0,0,0.3,1,
				0,0,0.3,1,
				0,0,0.3,1
			};
			video::SColor c(255,255,255,255);
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
					vertices[0][i].Pos += intToFloat(p + blockpos_nodes, BS);
				}
				v = vertices[0];
			}else if (dir.Y == -1) { // floor
				for (s32 i=0; i<24; i++) {
					vertices[1][i].Pos += intToFloat(p + blockpos_nodes, BS);
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

					vertices[2][i].Pos += intToFloat(p + blockpos_nodes, BS);
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
		/*
			Signs on walls
		*/
		break;
		case CDT_SIGNLIKE:
		{
			u8 l = decode_light(n.getLightBlend(data->m_daynight_ratio));
			video::SColor c = MapBlock_LightColor(255, l);

			static const f32 txc[24] = {
				0.1,0,0.9,0.1,
				0.1,0,0.9,0.1,
				0.1,0.3,0.9,0.7,
				0.1,0.3,0.9,0.7,
				0.1,0.3,0.9,0.7,
				0.1,0.3,0.9,0.7
			};
			video::S3DVertex vertices[24] = {
				// up
				video::S3DVertex(-0.4*BS,0.2*BS,0.5*BS, 0,1,0, c, txc[0],txc[1]),
				video::S3DVertex(0.4*BS,0.2*BS,0.5*BS, 0,1,0, c, txc[2],txc[1]),
				video::S3DVertex(0.4*BS,0.2*BS,0.45*BS, 0,1,0, c, txc[2],txc[3]),
				video::S3DVertex(-0.4*BS,0.2*BS,0.45*BS, 0,1,0, c, txc[0],txc[3]),
				// down
				video::S3DVertex(-0.4*BS,-0.2*BS,0.45*BS, 0,-1,0, c, txc[4],txc[5]),
				video::S3DVertex(0.4*BS,-0.2*BS,0.45*BS, 0,-1,0, c, txc[6],txc[5]),
				video::S3DVertex(0.4*BS,-0.2*BS,0.5*BS, 0,-1,0, c, txc[6],txc[7]),
				video::S3DVertex(-0.4*BS,-0.2*BS,0.5*BS, 0,-1,0, c, txc[4],txc[7]),
				// right
				video::S3DVertex(0.4*BS,0.2*BS,0.45*BS, 1,0,0, c, txc[ 8],txc[9]),
				video::S3DVertex(0.4*BS,0.2*BS,0.5*BS, 1,0,0, c, txc[10],txc[9]),
				video::S3DVertex(0.4*BS,-0.2*BS,0.5*BS, 1,0,0, c, txc[10],txc[11]),
				video::S3DVertex(0.4*BS,-0.2*BS,0.45*BS, 1,0,0, c, txc[ 8],txc[11]),
				// left
				video::S3DVertex(-0.4*BS,0.2*BS,0.5*BS, -1,0,0, c, txc[12],txc[13]),
				video::S3DVertex(-0.4*BS,0.2*BS,0.45*BS, -1,0,0, c, txc[14],txc[13]),
				video::S3DVertex(-0.4*BS,-0.2*BS,0.45*BS, -1,0,0, c, txc[14],txc[15]),
				video::S3DVertex(-0.4*BS,-0.2*BS,0.5*BS, -1,0,0, c, txc[12],txc[15]),
				// back
				video::S3DVertex(0.4*BS,0.2*BS,0.5*BS, 0,0,1, c, txc[16],txc[17]),
				video::S3DVertex(-0.4*BS,0.2*BS,0.5*BS, 0,0,1, c, txc[18],txc[17]),
				video::S3DVertex(-0.4*BS,-0.2*BS,0.5*BS, 0,0,1, c, txc[18],txc[19]),
				video::S3DVertex(0.4*BS,-0.2*BS,0.5*BS, 0,0,1, c, txc[16],txc[19]),
				// front
				video::S3DVertex(-0.4*BS,0.2*BS,0.45*BS, 0,0,-1, c, txc[20],txc[21]),
				video::S3DVertex(0.4*BS,0.2*BS,0.45*BS, 0,0,-1, c, txc[22],txc[21]),
				video::S3DVertex(0.4*BS,-0.2*BS,0.45*BS, 0,0,-1, c, txc[22],txc[23]),
				video::S3DVertex(-0.4*BS,-0.2*BS,0.45*BS, 0,0,-1, c, txc[20],txc[23]),
			};

			f32 sx = content_features(n).tiles[0].texture.x1()-content_features(n).tiles[0].texture.x0();
			f32 sy = content_features(n).tiles[0].texture.y1()-content_features(n).tiles[0].texture.y0();
			for (s32 j=0; j<24; j++) {
				vertices[j].TCoords *= v2f(sx,sy);
				vertices[j].TCoords += v2f(content_features(n).tiles[0].texture.x0(),content_features(n).tiles[0].texture.y0());
			}
			v3s16 dir = unpackDir(n.param2);

			for (s32 i=0; i<24; i++) {
				if(dir == v3s16(1,0,0))
					vertices[i].Pos.rotateXZBy(-90);
				if(dir == v3s16(-1,0,0))
					vertices[i].Pos.rotateXZBy(90);
				if(dir == v3s16(0,0,1))
					vertices[i].Pos.rotateXZBy(0);
				if(dir == v3s16(0,0,-1))
					vertices[i].Pos.rotateXZBy(180);
				if(dir == v3s16(0,-1,0))
					vertices[i].Pos.rotateYZBy(90);
				if(dir == v3s16(0,1,0))
					vertices[i].Pos.rotateYZBy(-90);

				vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
			}

			u16 indices[] = {0,1,2,2,3,0};
			// Add to mesh collector
			for (s32 j=0; j<24; j+=4) {
				collector.append(content_features(n).tiles[0].getMaterial(), &vertices[j], 4, indices, 6);
			}
		}
		/*
			Signs on walls
		*/
		break;
		case CDT_WALLMOUNT:
		{
			// Set material
			video::SMaterial material = content_features(n).tiles[0].getMaterial();
			material.setFlag(video::EMF_LIGHTING, false);
			material.setFlag(video::EMF_BACK_FACE_CULLING, false);
			material.setFlag(video::EMF_BILINEAR_FILTER, false);
			material.setFlag(video::EMF_FOG_ENABLE, true);
			material.MaterialType
					= video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			//material.setTexture(0, pa_current->atlas);

			u8 l = decode_light(n.getLightBlend(data->m_daynight_ratio));
			video::SColor c = MapBlock_LightColor(255, l);

			float d = (float)BS/16;
			// Wall at X+ of node
			video::S3DVertex vertices[4] =
			{
				video::S3DVertex(BS/2-d,-BS/2,-BS/2, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y1()),
				video::S3DVertex(BS/2-d,-BS/2,BS/2, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y1()),
				video::S3DVertex(BS/2-d,BS/2,BS/2, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y0()),
				video::S3DVertex(BS/2-d,BS/2,-BS/2, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y0()),
			};

			v3s16 dir = unpackDir(n.param2);

			for(s32 i=0; i<4; i++)
			{
				if(dir == v3s16(1,0,0))
					vertices[i].Pos.rotateXZBy(0);
				if(dir == v3s16(-1,0,0))
					vertices[i].Pos.rotateXZBy(180);
				if(dir == v3s16(0,0,1))
					vertices[i].Pos.rotateXZBy(90);
				if(dir == v3s16(0,0,-1))
					vertices[i].Pos.rotateXZBy(-90);
				if(dir == v3s16(0,-1,0))
					vertices[i].Pos.rotateXYBy(-90);
				if(dir == v3s16(0,1,0))
					vertices[i].Pos.rotateXYBy(90);

				vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
			}

			u16 indices[] = {0,1,2,2,3,0};
			// Add to mesh collector
			collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
		}
		/*
			Add leaves if using new style
		*/
		/*
			Add glass
		*/
		break;
		case CDT_GLASSLIKE:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l);

			for(u32 j=0; j<6; j++)
			{
				// Check this neighbor
				v3s16 n2p = blockpos_nodes + p + g_6dirs[j];
				MapNode n2 = data->m_vmanip.getNodeNoEx(n2p);

				// The face at Z+
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-BS/2,BS/2, 0,0,0, c,
						content_features(n).tiles[j].texture.x0(), content_features(n).tiles[j].texture.y1()),
					video::S3DVertex(BS/2,-BS/2,BS/2, 0,0,0, c,
						content_features(n).tiles[j].texture.x1(), content_features(n).tiles[j].texture.y1()),
					video::S3DVertex(BS/2,BS/2,BS/2, 0,0,0, c,
						content_features(n).tiles[j].texture.x1(), content_features(n).tiles[j].texture.y0()),
					video::S3DVertex(-BS/2,BS/2,BS/2, 0,0,0, c,
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
					vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[j].getMaterial(), vertices, 4, indices, 6);
			}
		}
		/*
			Add wall
		*/
		break;
		case CDT_WALLLIKE:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l);

			const f32 post_rad=(f32)0.2*BS;
			const f32 wall_rad=(f32)0.15*BS;
			u8 d[9];
			float height;
			mapblock_mesh_check_walllike(data, blockpos_nodes+p,d);

			v3f pos = intToFloat(p+blockpos_nodes, BS);
			if (d[0]) {
				height = d[5] ? 0.5 : 0.3;
				aabb3f bar(
					0.,-(0.5*BS),-wall_rad,
					0.5*BS,height*BS,wall_rad
				);
				bar.MinEdge += pos;
				bar.MaxEdge += pos;
				f32 xrailuv[24]={
					0.,0.35,0.35,0.65,
					0.,0.35,0.35,0.65,
					0.35,0.2,0.65,1,
					0.35,0.2,0.65,1,
					0.,0.2,0.5,1,
					0.,0.2,0.5,1
				};
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, xrailuv);
			}

			// Now a section of fence, -X, if there's a post there
			if (d[1]) {
				height = d[6] ? 0.5 : 0.3;
				aabb3f bar(
					-0.5*BS,-(0.5*BS),-wall_rad,
					0.,height*BS,wall_rad
				);
				bar.MinEdge += pos;
				bar.MaxEdge += pos;
				f32 xrailuv[24]={
					0.65,0.35,1,0.65,
					0.65,0.35,1,0.65,
					0.35,0.2,0.65,1,
					0.35,0.2,0.65,1,
					0.5,0.2,1,1,
					0.5,0.2,1,1
				};
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, xrailuv);
			}

			// Now a section of fence, +Z, if there's a post there
			if (d[2]) {
				height = d[7] ? 0.5 : 0.3;
				aabb3f bar(
					-wall_rad,-(0.5*BS),0.,
					wall_rad,height*BS,0.5*BS
				);
				bar.MinEdge += pos;
				bar.MaxEdge += pos;
				f32 zrailuv[24]={
					0.5,0.35,1,0.65,
					0.5,0.35,1,0.65,
					0.5,0.2,1,1,
					0.5,0.2,1,1,
					0.35,0.2,0.65,1,
					0.35,0.2,0.65,1
				};
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, zrailuv);
			}

			// Now a section of fence, +Z, if there's a post there
			if (d[3]) {
				height = d[8] ? 0.5 : 0.3;
				aabb3f bar(
					-wall_rad,-(0.5*BS),-0.5*BS,
					wall_rad,height*BS,0.
				);
				bar.MinEdge += pos;
				bar.MaxEdge += pos;
				f32 zrailuv[24]={
					0,0.35,0.5,0.65,
					0,0.35,0.5,0.65,
					0,0.2,0.5,1,
					0,0.2,0.5,1,
					0.35,0.2,0.65,1,
					0.35,0.2,0.65,1
				};
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, zrailuv);
			}
			if (d[4]) {
				aabb3f post(-post_rad,-BS/2,-post_rad,post_rad,BS/2,post_rad);
				post.MinEdge += pos;
				post.MaxEdge += pos;
				f32 postuv[24]={
					0.3,0.3,0.7,0.7,
					0.3,0.3,0.7,0.7,
					0.3,0,0.7,1,
					0.3,0,0.7,1,
					0.3,0,0.7,1,
					0.3,0,0.7,1
				};
				makeCuboid(&collector, post, content_features(n).tiles, 6,  c, postuv);
			}
		}
		/*
			Add fence
		*/
		break;
		case CDT_FENCELIKE:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l);

			const f32 post_rad=(f32)BS/10;
			const f32 bar_rad=(f32)BS/20;

			// The post - always present
			v3f pos = intToFloat(p+blockpos_nodes, BS);

			// The post - always present
			aabb3f post(-post_rad,-BS/2,-post_rad,post_rad,BS/2,post_rad);
			post.MinEdge += pos;
			post.MaxEdge += pos;
			f32 postuv[24]={
				0.4,0.4,0.6,0.6,
				0.4,0.4,0.6,0.6,
				0.35,0,0.65,1,
				0.35,0,0.65,1,
				0.35,0,0.65,1,
				0.35,0,0.65,1
			};
			makeCuboid(&collector, post, content_features(n).tiles, 6,  c, postuv);

			// Now a section of fence, +X, if there's a post there
			v3s16 p2 = p;
			p2.X++;
			MapNode n2 = data->m_vmanip.getNodeNoEx(blockpos_nodes + p2);
			const ContentFeatures *f2 = &content_features(n2);
			if (
				f2->draw_type == CDT_FENCELIKE
				|| f2->draw_type == CDT_WALLLIKE
				|| n2.getContent() == CONTENT_WOOD_GATE
				|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
				|| n2.getContent() == CONTENT_STEEL_GATE
				|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
			) {
				aabb3f bar(post_rad,-bar_rad+BS/4,-bar_rad,
						0.5*BS,bar_rad+BS/4,bar_rad);
				bar.MinEdge += pos;
				bar.MaxEdge += pos;
				f32 xrailuv[24]={
					0.5,0.4,1,0.6,
					0.5,0.4,1,0.6,
					0.5,0.4,1,0.6,
					0.5,0.4,1,0.6,
					0.5,0.4,1,0.6,
					0.5,0.4,1,0.6
				};
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, xrailuv);
				bar.MinEdge.Y -= BS/2;
				bar.MaxEdge.Y -= BS/2;
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, xrailuv);
			}

			// Now a section of fence, +Z, if there's a post there
			p2 = p;
			p2.X--;
			n2 = data->m_vmanip.getNodeNoEx(blockpos_nodes + p2);
			f2 = &content_features(n2);
			if (
				f2->draw_type == CDT_FENCELIKE
				|| f2->draw_type == CDT_WALLLIKE
				|| n2.getContent() == CONTENT_WOOD_GATE
				|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
				|| n2.getContent() == CONTENT_STEEL_GATE
				|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
			) {
				aabb3f bar(-0.5*BS,-bar_rad+BS/4,-bar_rad,
						-post_rad,bar_rad+BS/4,bar_rad);
				bar.MinEdge += pos;
				bar.MaxEdge += pos;
				f32 xrailuv[24]={
					0,0.4,0.5,0.6,
					0,0.4,0.5,0.6,
					0,0.4,0.5,0.6,
					0,0.4,0.5,0.6,
					0,0.4,0.5,0.6,
					0,0.4,0.5,0.6
				};
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, xrailuv);
				bar.MinEdge.Y -= BS/2;
				bar.MaxEdge.Y -= BS/2;
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, xrailuv);
			}

			// Now a section of fence, +Z, if there's a post there
			p2 = p;
			p2.Z++;
			n2 = data->m_vmanip.getNodeNoEx(blockpos_nodes + p2);
			f2 = &content_features(n2);
			if (
				f2->draw_type == CDT_FENCELIKE
				|| f2->draw_type == CDT_WALLLIKE
				|| n2.getContent() == CONTENT_WOOD_GATE
				|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
				|| n2.getContent() == CONTENT_STEEL_GATE
				|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
			) {
				aabb3f bar(-bar_rad,-bar_rad+BS/4,post_rad,
						bar_rad,bar_rad+BS/4,0.5*BS);
				bar.MinEdge += pos;
				bar.MaxEdge += pos;
				f32 zrailuv[24]={
					0,0.4,1,0.6,
					0,0.4,1,0.6,
					0,0.4,1,0.6,
					0,0.4,1,0.6,
					0,0.4,1,0.6,
					0,0.4,1,0.6
				};
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, zrailuv);
				bar.MinEdge.Y -= BS/2;
				bar.MaxEdge.Y -= BS/2;
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, zrailuv);
			}

			// Now a section of fence, +Z, if there's a post there
			p2 = p;
			p2.Z--;
			n2 = data->m_vmanip.getNodeNoEx(blockpos_nodes + p2);
			f2 = &content_features(n2);
			if (
				f2->draw_type == CDT_FENCELIKE
				|| f2->draw_type == CDT_WALLLIKE
				|| n2.getContent() == CONTENT_WOOD_GATE
				|| n2.getContent() == CONTENT_WOOD_GATE_OPEN
				|| n2.getContent() == CONTENT_STEEL_GATE
				|| n2.getContent() == CONTENT_STEEL_GATE_OPEN
			) {
				aabb3f bar(-bar_rad,-bar_rad+BS/4,-0.5*BS,
						bar_rad,bar_rad+BS/4,-post_rad);
				bar.MinEdge += pos;
				bar.MaxEdge += pos;
				f32 zrailuv[24]={
					0,0.4,1,0.6,
					0,0.4,1,0.6,
					0,0.4,1,0.6,
					0,0.4,1,0.6,
					0,0.4,1,0.6,
					0,0.4,1,0.6
				};
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, zrailuv);
				bar.MinEdge.Y -= BS/2;
				bar.MaxEdge.Y -= BS/2;
				makeCuboid(&collector, bar, content_features(n).tiles, 6,  c, zrailuv);
			}

		}
		break;
		case CDT_RAILLIKE:
		{
			bool is_rail_x [] = { false, false };  /* x-1, x+1 */
			bool is_rail_z [] = { false, false };  /* z-1, z+1 */
			content_t type = n.getContent();

			MapNode n_minus_x = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x-1,y,z));
			MapNode n_plus_x = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x+1,y,z));
			MapNode n_minus_z = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y,z-1));
			MapNode n_plus_z = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x,y,z+1));

			if(n_minus_x.getContent() == type)
				is_rail_x[0] = true;
			if(n_plus_x.getContent() == type)
				is_rail_x[1] = true;
			if(n_minus_z.getContent() == type)
				is_rail_z[0] = true;
			if(n_plus_z.getContent() == type)
				is_rail_z[1] = true;

			int adjacencies = is_rail_x[0] + is_rail_x[1] + is_rail_z[0] + is_rail_z[1];

			// Assign textures
			const char *texturename = "rail.png";
			if(adjacencies < 2)
				texturename = "rail.png";
			else if(adjacencies == 2)
			{
				if((is_rail_x[0] && is_rail_x[1]) || (is_rail_z[0] && is_rail_z[1]))
					texturename = "rail.png";
				else
					texturename = "rail_curved.png";
			}
			else if(adjacencies == 3)
				texturename = "rail_t_junction.png";
			else if(adjacencies == 4)
				texturename = "rail_crossing.png";

			AtlasPointer ap = g_texturesource->getTexture(texturename);

			video::SMaterial material_rail;
			material_rail.setFlag(video::EMF_LIGHTING, false);
			material_rail.setFlag(video::EMF_BACK_FACE_CULLING, false);
			material_rail.setFlag(video::EMF_BILINEAR_FILTER, false);
			material_rail.setFlag(video::EMF_FOG_ENABLE, true);
			material_rail.MaterialType
					= video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			material_rail.setTexture(0, ap.atlas);

			u8 l = decode_light(n.getLightBlend(data->m_daynight_ratio));
			video::SColor c = MapBlock_LightColor(255, l);

			float d = (float)BS/16;
			video::S3DVertex vertices[4] =
			{
				video::S3DVertex(-BS/2,-BS/2+d,-BS/2, 0,0,0, c,
						ap.x0(), ap.y1()),
				video::S3DVertex(BS/2,-BS/2+d,-BS/2, 0,0,0, c,
						ap.x1(), ap.y1()),
				video::S3DVertex(BS/2,-BS/2+d,BS/2, 0,0,0, c,
						ap.x1(), ap.y0()),
				video::S3DVertex(-BS/2,-BS/2+d,BS/2, 0,0,0, c,
						ap.x0(), ap.y0()),
			};

			// Rotate textures
			int angle = 0;

			if(adjacencies == 1)
			{
				if(is_rail_x[0] || is_rail_x[1])
					angle = 90;
			}
			else if(adjacencies == 2)
			{
				if(is_rail_x[0] && is_rail_x[1])
					angle = 90;
				else if(is_rail_x[0] && is_rail_z[0])
					angle = 270;
				else if(is_rail_x[0] && is_rail_z[1])
					angle = 180;
				else if(is_rail_x[1] && is_rail_z[1])
					angle = 90;
			}
			else if(adjacencies == 3)
			{
				if(!is_rail_x[0])
					angle=0;
				if(!is_rail_x[1])
					angle=180;
				if(!is_rail_z[0])
					angle=90;
				if(!is_rail_z[1])
					angle=270;
			}

			if(angle != 0) {
				for(u16 i=0; i<4; i++)
					vertices[i].Pos.rotateXZBy(angle);
			}

			for(s32 i=0; i<4; i++)
			{
				vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
			}

			u16 indices[] = {0,1,2,2,3,0};
			collector.append(material_rail, vertices, 4, indices, 6);
		}
		break;
		case CDT_PLANTLIKE_LGE:
		{
			u8 l = decode_light(undiminish_light(n.getLightBlend(data->m_daynight_ratio)));
			video::SColor c = MapBlock_LightColor(255, l);

			for(u32 j=0; j<2; j++)
			{
				video::S3DVertex vertices[4] =
				{
					video::S3DVertex(-BS/2,-BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x0(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,-BS/2,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y1()),
					video::S3DVertex(BS/2,BS/1,0, 0,0,0, c,
						content_features(n).tiles[0].texture.x1(), content_features(n).tiles[0].texture.y0()),
					video::S3DVertex(-BS/2,BS/1,0, 0,0,0, c,
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
					vertices[i].Pos *= 1.3;
					vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
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
			video::SColor c = MapBlock_LightColor(255, l);

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
					vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
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
			video::SColor c = MapBlock_LightColor(255, l);

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
					vertices[i].Pos += intToFloat(p + blockpos_nodes, BS);
				}

				u16 indices[] = {0,1,2,2,3,0};
				// Add to mesh collector
				collector.append(content_features(n).tiles[0].getMaterial(), vertices, 4, indices, 6);
			}
		}
		break;
		case CDT_NODEBOX:
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
				tiles[i] = n.getTile(tile_dirs[i]);
			}
			u8 l = 0;
			if (content_features(n).param_type == CPT_LIGHT) {
				l = decode_light(n.getLightBlend(data->m_daynight_ratio));
			}else{
				u32 lt = 0;
				u32 ltp;
				u8 ld = 0;
				for (s16 tx=-1; tx<2; tx++) {
				for (s16 ty=-1; ty<2; ty++) {
				for (s16 tz=-1; tz<2; tz++) {
					if (!tx && !ty && !tz)
						continue;
					MapNode tn = data->m_vmanip.getNodeNoEx(blockpos_nodes + v3s16(x+tx,y+ty,z+tz));
					if (ty<1 && tn.getContent() != CONTENT_AIR)
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
			}

			video::SColor c = MapBlock_LightColor(255, l);

			v3f pos = intToFloat(p+blockpos_nodes, BS);
			std::vector<aabb3f> boxes = content_features(n).getNodeBoxes(n);
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
				makeCuboid(&collector, box, tiles, 6,  c, txc);
			}
		}
		break;
		}
	}
}
#endif

