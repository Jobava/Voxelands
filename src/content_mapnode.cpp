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

// For g_settings
#include "main.h"

#include "content_mapnode.h"
#include "mapnode.h"
#include "content_nodemeta.h"
#include "settings.h"

#define WATER_ALPHA 160

#define WATER_VISC 1
#define LAVA_VISC 7

// TODO: Get rid of these and set up some attributes like toughness,
//       fluffyness, and a funciton to calculate time and durability loss
//       (and sound? and whatever else) from them
void setStoneLikeDiggingProperties(DiggingPropertiesList &list, float toughness);
void setDirtLikeDiggingProperties(DiggingPropertiesList &list, float toughness);
void setWoodLikeDiggingProperties(DiggingPropertiesList &list, float toughness);
void setLeafLikeDiggingProperties(DiggingPropertiesList &list, float toughness);
void setWaterLikeDiggingProperties(DiggingPropertiesList &list, float toughness);

static void content_mapnode_nodebox_stair(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.,
		0.,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_stairud(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		0.,
		0.5*BS,
		0.,
		0.5*BS
	));
}

static void content_mapnode_nodebox_slab(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0,
		0.5*BS
	));
}

static void content_mapnode_nodebox_slabud(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_door(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.35*BS
	));
}

static void content_mapnode_nodebox_doorw(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3*BS,
		-0.35*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.35*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.3*BS,
		-0.5*BS,
		-0.3*BS,
		0.3*BS,
		-0.35*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS,
		-0.3*BS,
		-0.5*BS,
		0.5*BS,
		0.3*BS,
		-0.35*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.05*BS,
		-0.3*BS,
		-0.45*BS,
		0.05*BS,
		0.3*BS,
		-0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.05*BS,
		-0.45*BS,
		0.1*BS,
		0.05*BS,
		-0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.1*BS,
		-0.05*BS,
		-0.45*BS,
		0.5*BS,
		0.05*BS,
		-0.4*BS
	));
}

static void content_mapnode_nodebox_doorol(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		-0.35*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_doorwol(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		-0.35*BS,
		-0.3*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3*BS,
		-0.5*BS,
		-0.35*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.3*BS,
		-0.5*BS,
		-0.35*BS,
		0.3*BS,
		-0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.3*BS,
		0.3*BS,
		-0.35*BS,
		0.3*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.45*BS,
		-0.3*BS,
		-0.05*BS,
		-0.4*BS,
		0.3*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.45*BS,
		-0.05*BS,
		-0.3*BS,
		-0.4*BS,
		0.05*BS,
		-0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.45*BS,
		-0.05*BS,
		0.05*BS,
		-0.4*BS,
		0.05*BS,
		0.3*BS
	));
}

static void content_mapnode_nodebox_dooror(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_doorwor(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		0.3*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		-0.3*BS,
		-0.5*BS,
		0.5*BS,
		0.3*BS,
		-0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		-0.3*BS,
		0.3*BS,
		0.5*BS,
		0.3*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.3*BS,
		-0.05*BS,
		0.45*BS,
		0.3*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.05*BS,
		-0.3*BS,
		0.45*BS,
		0.05*BS,
		-0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.05*BS,
		0.05*BS,
		0.45*BS,
		0.05*BS,
		0.3*BS
	));
}

static void content_mapnode_nodebox_hatch(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.35*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_hatchw(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.35*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.35*BS,
		0.3*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS,
		0.35*BS,
		-0.3*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.35*BS,
		-0.3*BS,
		-0.3*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.05*BS,
		0.4*BS,
		-0.3*BS,
		0.05*BS,
		0.45*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS,
		0.4*BS,
		-0.05*BS,
		-0.05*BS,
		0.45*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.05*BS,
		0.4*BS,
		-0.05*BS,
		0.3*BS,
		0.45*BS,
		0.05*BS
	));
}

static void content_mapnode_nodebox_hatcho(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_hatchwo(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		0.3*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		-0.3*BS,
		-0.5*BS,
		0.5*BS,
		0.3*BS,
		-0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.35*BS,
		-0.3*BS,
		0.3*BS,
		0.5*BS,
		0.3*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.3*BS,
		-0.05*BS,
		0.45*BS,
		0.3*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.05*BS,
		-0.3*BS,
		0.45*BS,
		0.05*BS,
		-0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.05*BS,
		0.05*BS,
		0.45*BS,
		0.05*BS,
		0.3*BS
	));
}

static void content_mapnode_nodebox_gate(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		-0.4*BS,
		-0.05*BS,
		0.4*BS,
		-0.2*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		0.1*BS,
		-0.05*BS,
		0.4*BS,
		0.3*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		-0.4*BS,
		0.4*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.4*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.1*BS,
		-0.2*BS,
		-0.05*BS,
		0.1*BS,
		0.4*BS,
		0.05*BS
	));
}

static void content_mapnode_nodebox_gateo(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		-0.4*BS,
		0.4*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.4*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.4*BS,
		0.4*BS,
		-0.4*BS,
		0.4*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.4*BS,
		0.4*BS,
		0.5*BS,
		0.4*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.1*BS,
		-0.05*BS,
		-0.4*BS,
		0.3*BS,
		0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.4*BS,
		-0.05*BS,
		-0.4*BS,
		-0.2*BS,
		0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		0.1*BS,
		-0.05*BS,
		0.5*BS,
		0.3*BS,
		0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.4*BS,
		-0.05*BS,
		0.5*BS,
		-0.2*BS,
		0.4*BS
	));
}

static void content_mapnode_nodebox_carpet(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.4*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_bed_head(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.125*BS,
		-0.5*BS,
		0.4375*BS,
		0.125*BS,
		0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		0.125*BS,
		0.*BS,
		0.375*BS,
		0.1875*BS,
		0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		0.375*BS,
		-0.375*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.5*BS,
		0.375*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		-0.125*BS,
		0.4375*BS,
		0.375*BS,
		0.*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		0.1925*BS,
		0.4375*BS,
		0.375*BS,
		0.375*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4375*BS,
		-0.125*BS,
		-0.5*BS,
		0.5*BS,
		0.*BS,
		0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.125*BS,
		-0.5*BS,
		-0.4375*BS,
		0.*BS,
		0.375*BS
	));
}

static void content_mapnode_nodebox_bed_foot(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.125*BS,
		-0.4375*BS,
		0.4375*BS,
		0.125*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		-0.375*BS,
		0.5*BS,
		-0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		-0.125*BS,
		-0.5*BS,
		0.375*BS,
		0.*BS,
		-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		0.1925*BS,
		-0.5*BS,
		0.375*BS,
		0.25*BS,
		-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4375*BS,
		-0.125*BS,
		-0.375*BS,
		0.5*BS,
		0.*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.125*BS,
		-0.375*BS,
		-0.4375*BS,
		0.*BS,
		0.5*BS
	));
}

/*
	A conversion table for backwards compatibility.
	Maps <=v19 content types to current ones.
	Should never be touched.
*/
content_t trans_table_19[21][2] = {
	{CONTENT_GRASS, 1},
	{CONTENT_TREE, 4},
	{CONTENT_LEAVES, 5},
	{CONTENT_FARM_DIRT, 6},
	{CONTENT_MESE, 7},
	{CONTENT_MUD, 8},
	{CONTENT_COTTON, 10},
	{CONTENT_BORDERSTONE, 11},
	{CONTENT_WOOD, 12},
	{CONTENT_SAND, 13},
	{CONTENT_COBBLE, 18},
	{CONTENT_STEEL, 19},
	{CONTENT_GLASS, 20},
	{CONTENT_MOSSYCOBBLE, 22},
	{CONTENT_GRAVEL, 23},
	{CONTENT_SANDSTONE, 24},
	{CONTENT_CACTUS, 25},
	{CONTENT_BRICK, 26},
	{CONTENT_CLAY, 27},
	{CONTENT_PAPYRUS, 28},
	{CONTENT_BOOKSHELF, 29},
};

MapNode mapnode_translate_from_internal(MapNode n_from, u8 version)
{
	MapNode result = n_from;
	if(version <= 19)
	{
		content_t c_from = n_from.getContent();
		for(u32 i=0; i<sizeof(trans_table_19)/sizeof(trans_table_19[0]); i++)
		{
			if(trans_table_19[i][0] == c_from)
			{
				result.setContent(trans_table_19[i][1]);
				break;
			}
		}
	}
	return result;
}
MapNode mapnode_translate_to_internal(MapNode n_from, u8 version)
{
	MapNode result = n_from;
	if(version <= 19)
	{
		content_t c_from = n_from.getContent();
		for(u32 i=0; i<sizeof(trans_table_19)/sizeof(trans_table_19[0]); i++)
		{
			if(trans_table_19[i][1] == c_from)
			{
				result.setContent(trans_table_19[i][0]);
				break;
			}
		}
	}
	// stairs, slabs, gates, doors, and hatches were moved for 1311.00
	if (n_from.getContent() >= 0x821 && n_from.getContent() <= 0x87F) {
		// slabs
		if (n_from.getContent() == 0x821) {
			result.setContent(CONTENT_ROUGHSTONE_SLAB);
		}else if (n_from.getContent() == 0x822) {
			result.setContent(CONTENT_MOSSYCOBBLE_SLAB);
		}else if (n_from.getContent() == 0x823) {
			result.setContent(CONTENT_STONE_SLAB);
		}else if (n_from.getContent() == 0x824) {
			result.setContent(CONTENT_WOOD_SLAB);
		}else if (n_from.getContent() == 0x825) {
			result.setContent(CONTENT_JUNGLE_SLAB);
		}else if (n_from.getContent() == 0x826) {
			result.setContent(CONTENT_BRICK_SLAB);
		}else if (n_from.getContent() == 0x827) {
			result.setContent(CONTENT_SANDSTONE_SLAB);
		}
		// stairs
		if (n_from.getContent() == 0x828) {
			result.setContent(CONTENT_ROUGHSTONE_STAIR);
		}else if (n_from.getContent() == 0x829) {
			result.setContent(CONTENT_MOSSYCOBBLE_STAIR);
		}else if (n_from.getContent() == 0x82a) {
			result.setContent(CONTENT_STONE_STAIR);
		}else if (n_from.getContent() == 0x82b) {
			result.setContent(CONTENT_WOOD_STAIR);
		}else if (n_from.getContent() == 0x82c) {
			result.setContent(CONTENT_JUNGLE_STAIR);
		}else if (n_from.getContent() == 0x82d) {
			result.setContent(CONTENT_BRICK_STAIR);
		}else if (n_from.getContent() == 0x82e) {
			result.setContent(CONTENT_SANDSTONE_STAIR);
		}
		// upside down slabs
		if (n_from.getContent() == 0x831) {
			result.setContent(CONTENT_ROUGHSTONE_SLAB_UD);
		}else if (n_from.getContent() == 0x832) {
			result.setContent(CONTENT_MOSSYCOBBLE_SLAB_UD);
		}else if (n_from.getContent() == 0x833) {
			result.setContent(CONTENT_STONE_SLAB_UD);
		}else if (n_from.getContent() == 0x834) {
			result.setContent(CONTENT_WOOD_SLAB_UD);
		}else if (n_from.getContent() == 0x835) {
			result.setContent(CONTENT_JUNGLE_SLAB_UD);
		}else if (n_from.getContent() == 0x836) {
			result.setContent(CONTENT_BRICK_SLAB_UD);
		}else if (n_from.getContent() == 0x837) {
			result.setContent(CONTENT_SANDSTONE_SLAB_UD);
		}
		// upside down stairs
		if (n_from.getContent() == 0x838) {
			result.setContent(CONTENT_ROUGHSTONE_STAIR_UD);
		}else if (n_from.getContent() == 0x839) {
			result.setContent(CONTENT_MOSSYCOBBLE_STAIR_UD);
		}else if (n_from.getContent() == 0x83a) {
			result.setContent(CONTENT_STONE_STAIR_UD);
		}else if (n_from.getContent() == 0x83b) {
			result.setContent(CONTENT_WOOD_STAIR_UD);
		}else if (n_from.getContent() == 0x83c) {
			result.setContent(CONTENT_JUNGLE_STAIR_UD);
		}else if (n_from.getContent() == 0x83d) {
			result.setContent(CONTENT_BRICK_STAIR_UD);
		}else if (n_from.getContent() == 0x83e) {
			result.setContent(CONTENT_SANDSTONE_STAIR_UD);
		}


		// doors
		if (n_from.getContent() == 0x840) {
			result.setContent(CONTENT_WOOD_DOOR_LB);
		}else if (n_from.getContent() == 0x841) {
			result.setContent(CONTENT_WOOD_DOOR_LT);
		}else if (n_from.getContent() == 0x842) {
			result.setContent(CONTENT_STEEL_DOOR_LB);
		}else if (n_from.getContent() == 0x843) {
			result.setContent(CONTENT_STEEL_DOOR_LT);
		}else if (n_from.getContent() == 0x844) {
			result.setContent(CONTENT_WOOD_W_DOOR_LB);
		}else if (n_from.getContent() == 0x845) {
			result.setContent(CONTENT_WOOD_W_DOOR_LT);
		}else if (n_from.getContent() == 0x846) {
			result.setContent(CONTENT_STEEL_W_DOOR_LB);
		}else if (n_from.getContent() == 0x847) {
			result.setContent(CONTENT_STEEL_W_DOOR_LT);
		}else if (n_from.getContent() == 0x848) {
			result.setContent(CONTENT_WOOD_DOOR_RB);
		}else if (n_from.getContent() == 0x849) {
			result.setContent(CONTENT_WOOD_DOOR_RT);
		}else if (n_from.getContent() == 0x84A) {
			result.setContent(CONTENT_STEEL_DOOR_RB);
		}else if (n_from.getContent() == 0x84B) {
			result.setContent(CONTENT_STEEL_DOOR_RT);
		}else if (n_from.getContent() == 0x84C) {
			result.setContent(CONTENT_WOOD_W_DOOR_RB);
		}else if (n_from.getContent() == 0x84D) {
			result.setContent(CONTENT_WOOD_W_DOOR_RT);
		}else if (n_from.getContent() == 0x84E) {
			result.setContent(CONTENT_STEEL_W_DOOR_RB);
		}else if (n_from.getContent() == 0x84F) {
			result.setContent(CONTENT_STEEL_W_DOOR_RT);
		}
		// hatches
		if (n_from.getContent() == 0x850) {
			result.setContent(CONTENT_WOOD_HATCH);
		}else if (n_from.getContent() == 0x852) {
			result.setContent(CONTENT_STEEL_HATCH);
		}else if (n_from.getContent() == 0x854) {
			result.setContent(CONTENT_WOOD_W_HATCH);
		}else if (n_from.getContent() == 0x856) {
			result.setContent(CONTENT_STEEL_W_HATCH);
		}
		// gates
		if (n_from.getContent() == 0x858) {
			result.setContent(CONTENT_WOOD_GATE);
		}else if (n_from.getContent() == 0x85A) {
			result.setContent(CONTENT_STEEL_GATE);
		}

		// open doors
		if (n_from.getContent() == 0x860) {
			result.setContent(CONTENT_WOOD_DOOR_LB_OPEN);
		}else if (n_from.getContent() == 0x861) {
			result.setContent(CONTENT_WOOD_DOOR_LT_OPEN);
		}else if (n_from.getContent() == 0x862) {
			result.setContent(CONTENT_STEEL_DOOR_LB_OPEN);
		}else if (n_from.getContent() == 0x863) {
			result.setContent(CONTENT_STEEL_DOOR_LT_OPEN);
		}else if (n_from.getContent() == 0x864) {
			result.setContent(CONTENT_WOOD_W_DOOR_LB_OPEN);
		}else if (n_from.getContent() == 0x865) {
			result.setContent(CONTENT_WOOD_W_DOOR_LT_OPEN);
		}else if (n_from.getContent() == 0x866) {
			result.setContent(CONTENT_STEEL_W_DOOR_LB_OPEN);
		}else if (n_from.getContent() == 0x867) {
			result.setContent(CONTENT_STEEL_W_DOOR_LT_OPEN);
		}else if (n_from.getContent() == 0x868) {
			result.setContent(CONTENT_WOOD_DOOR_RB_OPEN);
		}else if (n_from.getContent() == 0x869) {
			result.setContent(CONTENT_WOOD_DOOR_RT_OPEN);
		}else if (n_from.getContent() == 0x86A) {
			result.setContent(CONTENT_STEEL_DOOR_RB_OPEN);
		}else if (n_from.getContent() == 0x86B) {
			result.setContent(CONTENT_STEEL_DOOR_RT_OPEN);
		}else if (n_from.getContent() == 0x86C) {
			result.setContent(CONTENT_WOOD_W_DOOR_RB_OPEN);
		}else if (n_from.getContent() == 0x86D) {
			result.setContent(CONTENT_WOOD_W_DOOR_RT_OPEN);
		}else if (n_from.getContent() == 0x86E) {
			result.setContent(CONTENT_STEEL_W_DOOR_RB_OPEN);
		}else if (n_from.getContent() == 0x86F) {
			result.setContent(CONTENT_STEEL_W_DOOR_RT_OPEN);
		}
		// open hatches
		if (n_from.getContent() == 0x870) {
			result.setContent(CONTENT_WOOD_HATCH_OPEN);
		}else if (n_from.getContent() == 0x872) {
			result.setContent(CONTENT_STEEL_HATCH_OPEN);
		}else if (n_from.getContent() == 0x874) {
			result.setContent(CONTENT_WOOD_W_HATCH_OPEN);
		}else if (n_from.getContent() == 0x876) {
			result.setContent(CONTENT_STEEL_W_HATCH_OPEN);
		}
		// open gates
		if (n_from.getContent() == 0x878) {
			result.setContent(CONTENT_WOOD_GATE_OPEN);
		}else if (n_from.getContent() == 0x87A) {
			result.setContent(CONTENT_STEEL_GATE_OPEN);
		}
	}
	return result;
}

void content_mapnode_init()
{
	// Read some settings
	bool new_style_water = g_settings->getBool("new_style_water");
	bool new_style_leaves = g_settings->getBool("new_style_leaves");
	bool invisible_stone = g_settings->getBool("invisible_stone");
#ifndef SERVER
	bool opaque_water = g_settings->getBool("opaque_water");
#endif

	content_t i;
	ContentFeatures *f = NULL;

	i = CONTENT_STONE;
	f = &content_features(i);
	f->description = std::string("Stone");
	f->setAllTextures("stone.png");
	f->setInventoryTextureCube("stone.png", "stone.png", "stone.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->often_contains_mineral = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);
	if(invisible_stone)
		f->solidness = 0; // For debugging, hides regular stone

	i = CONTENT_ICE;
	f = &content_features(i);
	f->description = std::string("Ice");
	f->setAllTextures("ice.png");
	f->setInventoryTextureCube("ice.png", "ice.png", "ice.png");
	f->draw_type = CDT_GLASSLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_COAL;
	f = &content_features(i);
	f->description = std::string("Coal Block");
	f->setAllTextures("coal.png");
	f->setInventoryTextureCube("coal.png", "coal.png", "coal.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CHARCOAL;
	f = &content_features(i);
	f->description = std::string("Charcoal Block");
	f->setAllTextures("charcoal.png");
	f->setInventoryTextureCube("charcoal.png", "charcoal.png", "charcoal.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_STONEBRICK;
	f = &content_features(i);
	f->description = std::string("Stone Brick");
	f->setAllTextures("stonebrick.png");
	f->setInventoryTextureCube("stonebrick.png", "stonebrick.png", "stonebrick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_STONEBLOCK;
	f = &content_features(i);
	f->description = std::string("Stone Block");
	f->setAllTextures("stoneblock.png");
	f->setInventoryTextureCube("stoneblock.png", "stoneblock.png", "stoneblock.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_ROUGHSTONEBRICK;
	f = &content_features(i);
	f->description = std::string("Rough Stone Brick");
	f->setAllTextures("roughstone_brick.png");
	f->setInventoryTextureCube("roughstone_brick.png", "roughstone_brick.png", "roughstone_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_ROUGHSTONEBLOCK;
	f = &content_features(i);
	f->description = std::string("Rough Stone Block");
	f->setAllTextures("roughstone_block.png");
	f->setInventoryTextureCube("roughstone_block.png", "roughstone_block.png", "roughstone_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_GRASS;
	f = &content_features(i);
	f->description = std::string("Grass");
	f->setAllTextures("mud.png^grass_side.png");
	f->setTexture(0, "grass.png");
	f->setTexture(1, "mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_MUDSNOW;
	f = &content_features(i);
	f->description = std::string("Muddy Snow");
	f->setAllTextures("mud.png^snow_side.png");
	f->setTexture(0, "snow.png");
	f->setTexture(1, "mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_FARM_DIRT;
	f = &content_features(i);
	f->description = std::string("Farm Dirt");
	f->setAllTextures("dirt.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_MUD;
	f = &content_features(i);
	f->description = std::string("Mud");
	f->setAllTextures("mud.png");
	f->setInventoryTextureCube("mud.png", "mud.png", "mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SAND;
	f = &content_features(i);
	f->description = std::string("Sand");
	f->setAllTextures("sand.png");
	f->setInventoryTextureCube("sand.png", "sand.png", "sand.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_GLASS)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_GRAVEL;
	f = &content_features(i);
	f->description = std::string("Gravel");
	f->setAllTextures("gravel.png");
	f->setInventoryTextureCube("gravel.png", "gravel.png", "gravel.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->extra_dug_item = std::string("CraftItem lump_of_flint 1");
	f->extra_dug_item_rarity = 10;
	setDirtLikeDiggingProperties(f->digging_properties, 1.75);

	i = CONTENT_SANDSTONE;
	f = &content_features(i);
	f->description = std::string("Sand Stone");
	f->setAllTextures("sandstone.png");
	f->setInventoryTextureCube("sandstone.png", "sandstone.png", "sandstone.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SANDSTONE_BRICK;
	f = &content_features(i);
	f->description = std::string("Sand Stone Bricks");
	f->setAllTextures("sandstone_brick.png");
	f->setInventoryTextureCube("sandstone_brick.png", "sandstone_brick.png", "sandstone_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SANDSTONE_BLOCK;
	f = &content_features(i);
	f->description = std::string("Sand Stone Blocks");
	f->setAllTextures("sandstone_block.png");
	f->setInventoryTextureCube("sandstone_block.png", "sandstone_block.png", "sandstone_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY;
	f = &content_features(i);
	f->description = std::string("Clay");
	f->setAllTextures("clay.png");
	f->setInventoryTextureCube("clay.png", "clay.png", "clay.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_TERRACOTTA)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Clay");
	f->setAllTextures("clay_blue.png");
	f->setInventoryTextureCube("clay_blue.png", "clay_blue.png", "clay_blue.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_blue 1");
	f->extra_dug_item_rarity = 4;
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Clay");
	f->setAllTextures("clay_green.png");
	f->setInventoryTextureCube("clay_green.png", "clay_green.png", "clay_green.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_green 1");
	f->extra_dug_item_rarity = 4;
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Clay");
	f->setAllTextures("clay_orange.png");
	f->setInventoryTextureCube("clay_orange.png", "clay_orange.png", "clay_orange.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_orange 1");
	f->extra_dug_item_rarity = 4;
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Clay");
	f->setAllTextures("clay_purple.png");
	f->setInventoryTextureCube("clay_purple.png", "clay_purple.png", "clay_purple.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_purple 1");
	f->extra_dug_item_rarity = 4;
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY_RED;
	f = &content_features(i);
	f->description = std::string("Red Clay");
	f->setAllTextures("clay_red.png");
	f->setInventoryTextureCube("clay_red.png", "clay_red.png", "clay_red.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_red 1");
	f->extra_dug_item_rarity = 4;
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Clay");
	f->setAllTextures("clay_yellow.png");
	f->setInventoryTextureCube("clay_yellow.png", "clay_yellow.png", "clay_yellow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_yellow 1");
	f->extra_dug_item_rarity = 4;
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Clay");
	f->setAllTextures("clay_black.png");
	f->setInventoryTextureCube("clay_black.png", "clay_black.png", "clay_black.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_black 1");
	f->extra_dug_item_rarity = 4;
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_BRICK;
	f = &content_features(i);
	f->description = std::string("Brick");
	f->setAllTextures("brick.png");
	f->setInventoryTextureCube("brick.png", "brick.png", "brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_TERRACOTTA;
	f = &content_features(i);
	f->description = std::string("Terracotta");
	f->setAllTextures("terracotta.png");
	f->setInventoryTextureCube("terracotta.png", "terracotta.png", "terracotta.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_TERRACOTTA_BRICK;
	f = &content_features(i);
	f->description = std::string("Terracotta Brick");
	f->setAllTextures("terracotta_brick.png");
	f->setInventoryTextureCube("terracotta_brick.png", "terracotta_brick.png", "terracotta_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_TERRACOTTA_BLOCK;
	f = &content_features(i);
	f->description = std::string("Terracotta Block");
	f->setAllTextures("terracotta_block.png");
	f->setInventoryTextureCube("terracotta_block.png", "terracotta_block.png", "terracotta_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_TERRACOTTA_TILE;
	f = &content_features(i);
	f->description = std::string("Terracotta Tile");
	f->setAllTextures("terracotta_tile.png");
	f->setInventoryTextureCube("terracotta_tile.png", "terracotta_tile.png", "terracotta_tile.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_TREE;
	f = &content_features(i);
	f->description = std::string("Tree");
	f->setAllTextures("tree.png");
	f->setTexture(0, "tree_top.png");
	f->setTexture(1, "tree_top.png");
	f->setInventoryTextureCube("tree_top.png", "tree.png", "tree.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("CraftItem lump_of_charcoal 1");
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_JUNGLETREE;
	f = &content_features(i);
	f->description = std::string("Jungle Tree");
	f->setAllTextures("jungletree.png");
	f->setTexture(0, "jungletree_top.png");
	f->setTexture(1, "jungletree_top.png");
	f->setInventoryTextureCube("jungletree_top.png", "jungletree.png", "jungletree.png");
	f->draw_type = CDT_CUBELIKE;
	//f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_JUNGLEGRASS;
	f = &content_features(i);
	f->description = std::string("Jungle Grass");
	f->setInventoryTexture("junglegrass.png");
	f->setAllTextures("junglegrass.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE_LGE;
	//f->is_ground_content = true;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_LEAVES;
	f = &content_features(i);
	f->description = std::string("Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("leaves.png", "leaves.png", "leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAPLING)+" 1";
	f->extra_dug_item_rarity = 20;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setLeafLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_JUNGLELEAVES;
	f = &content_features(i);
	f->description = std::string("Jungle Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("jungleleaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("jungleleaves.png", "jungleleaves.png", "jungleleaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:jungleleaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_JUNGLESAPLING)+" 1";
	f->extra_dug_item_rarity = 20;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setLeafLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_CONIFER_LEAVES;
	f = &content_features(i);
	f->description = std::string("Conifer Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("conifer_leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("conifer_leaves.png", "conifer_leaves.png", "conifer_leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:conifer_leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CONIFER_SAPLING)+" 1";
	f->extra_dug_item_rarity = 20;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setLeafLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_TRIMMED_LEAVES;
	f = &content_features(i);
	f->description = std::string("Trimmed Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("leaves.png", "leaves.png", "leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setLeafLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_CACTUS;
	f = &content_features(i);
	f->description = std::string("Cactus");
	f->setAllTextures("cactus_side.png");
	f->setTexture(0, "cactus_top.png");
	f->setTexture(1, "cactus_top.png");
	f->setInventoryTextureCube("cactus_top.png", "cactus_side.png", "cactus_side.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_PAPYRUS;
	f = &content_features(i);
	f->description = std::string("Papyrus");
	f->setInventoryTexture("papyrus.png");
	f->setAllTextures("papyrus.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BOOKSHELF;
	f = &content_features(i);
	f->description = std::string("Book Shelf");
	f->setAllTextures("bookshelf.png");
	f->setTexture(0, "wood.png");
	f->setTexture(1, "wood.png");
	f->setInventoryTextureCube("wood.png", "bookshelf.png", "bookshelf.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_GLASS;
	f = &content_features(i);
	f->description = std::string("Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass.png", "glass.png", "glass.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_blue.png", "glass_blue.png", "glass_blue.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_green.png", "glass_green.png", "glass_green.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_orange.png", "glass_orange.png", "glass_orange.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_purple.png", "glass_purple.png", "glass_purple.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_RED;
	f = &content_features(i);
	f->description = std::string("Red Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_red.png", "glass_red.png", "glass_red.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_yellow.png", "glass_yellow.png", "glass_yellow.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_black.png", "glass_black.png", "glass_black.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PANE;
	f = &content_features(i);
	f->description = std::string("Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_side.png");
	f->setTexture(4,"glass.png");
	f->setTexture(5,"glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_side.png", "glass.png", "glass_pane_side.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PANE_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_blue_side.png");
	f->setTexture(4,"glass_blue.png");
	f->setTexture(5,"glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_blue_side.png", "glass_blue.png", "glass_pane_blue_side.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PANE_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_green_side.png");
	f->setTexture(4,"glass_green.png");
	f->setTexture(5,"glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_green_side.png", "glass_green.png", "glass_pane_green_side.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PANE_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_orange_side.png");
	f->setTexture(4,"glass_orange.png");
	f->setTexture(5,"glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_orange_side.png", "glass_orange.png", "glass_pane_orange_side.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PANE_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_purple_side.png");
	f->setTexture(4,"glass_purple.png");
	f->setTexture(5,"glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_purple_side.png", "glass_purple.png", "glass_pane_purple_side.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PANE_RED;
	f = &content_features(i);
	f->description = std::string("Red Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_red_side.png");
	f->setTexture(4,"glass_red.png");
	f->setTexture(5,"glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_red_side.png", "glass_red.png", "glass_pane_red_side.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PANE_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_yellow_side.png");
	f->setTexture(4,"glass_yellow.png");
	f->setTexture(5,"glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_yellow_side.png", "glass_yellow.png", "glass_pane_yellow_side.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PANE_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_black_side.png");
	f->setTexture(4,"glass_black.png");
	f->setTexture(5,"glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_black_side.png", "glass_black.png", "glass_pane_black_side.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASSLIGHT;
	f = &content_features(i);
	f->description = std::string("Glass Light");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glasslight.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glasslight.png", "glasslight.png", "glasslight.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);
	f->light_source = LIGHT_MAX-1;

	i = CONTENT_FENCE;
	f = &content_features(i);
	f->description = std::string("Fence");
	f->setAllTextures("wood.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_FENCELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("fence.png");
	f->used_texturenames["fence.png"] = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_RAIL;
	f = &content_features(i);
	f->description = std::string("Rail");
	f->setInventoryTexture("rail.png");
	f->used_texturenames["rail.png"] = true;
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_RAILLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->walkable = false;
	setDirtLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_LADDER;
	f = &content_features(i);
	f->description = std::string("Ladder");
	f->setInventoryTexture("ladder.png");
	f->setAllTextures("ladder.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLMOUNT;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem ")+itos(i)+" 1";
	f->wall_mounted = true;
	f->solidness = 0;
	f->air_equivalent = true;
	f->walkable = false;
	f->climbable = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.5);

	i = CONTENT_BORDERSTONE;
	f = &content_features(i);
	f->description = std::string("Border Stone");
	f->setAllTextures("borderstone.png");
	f->setInventoryTextureCube("borderstone.png", "borderstone.png", "borderstone.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new BorderStoneNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 2.0);

	i = CONTENT_WOOD;
	f = &content_features(i);
	f->description = std::string("Wood");
	f->setAllTextures("wood.png");
	f->setInventoryTextureCube("wood.png", "wood.png", "wood.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_JUNGLEWOOD;
	f = &content_features(i);
	f->description = std::string("Jungle Wood");
	f->setAllTextures("junglewood.png");
	f->setInventoryTextureCube("junglewood.png", "junglewood.png", "junglewood.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_WOOD_FINISHED;
	f = &content_features(i);
	f->description = std::string("Finished Wood");
	f->setAllTextures("finished_wood.png");
	f->setInventoryTextureCube("finished_wood.png", "finished_wood.png", "finished_wood.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.5);

	i = CONTENT_MESE;
	f = &content_features(i);
	f->description = std::string("Mese");
	f->setAllTextures("mese.png");
	f->setInventoryTextureCube("mese.png", "mese.png", "mese.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("CraftItem saltpeter 1");
	setStoneLikeDiggingProperties(f->digging_properties, 0.5);

	i = CONTENT_MESE_DIGGING;
	f = &content_features(i);
	f->description = std::string("Mese");
	f->setAllTextures("mese.png");
	f->setInventoryTextureCube("mese.png", "mese.png", "mese.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MESE)+" 1";
	f->light_source = 10;
	setStoneLikeDiggingProperties(f->digging_properties, 0.5);

	i = CONTENT_SPONGE;
	f = &content_features(i);
	f->description = std::string("Sponge");
	f->setAllTextures("sponge.png");
	f->setInventoryTextureCube("sponge.png", "sponge.png", "sponge.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SPONGE_FULL;
	f = &content_features(i);
	f->description = std::string("Waterlogged Sponge");
	f->setAllTextures("sponge_full.png");
	f->setInventoryTextureCube("sponge_full.png", "sponge_full.png", "sponge_full.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_HAY;
	f = &content_features(i);
	f->description = std::string("Hay Bale");
	f->setAllTextures("hay_bale.png");
	f->setInventoryTextureCube("hay_bale.png", "hay_bale.png", "hay_bale.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setLeafLikeDiggingProperties(f->digging_properties, 0.6);

	i = CONTENT_SNOW_BLOCK;
	f = &content_features(i);
	f->description = std::string("Snow");
	f->setAllTextures("snow.png");
	f->setInventoryTextureCube("snow.png", "snow.png", "snow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem snow_ball 9");
	setDirtLikeDiggingProperties(f->digging_properties, 0.3);

	i = CONTENT_SNOW;
	f = &content_features(i);
	f->description = std::string("Snow");
	f->setAllTextures("snow.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->buildable_to = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("CraftItem snow_ball 1");
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"snow.png", "snow.png", "snow.png");
	setDirtLikeDiggingProperties(f->digging_properties, 0.3);

	i = CONTENT_COTTON;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("cotton.png");
	f->setInventoryTextureCube("cotton.png", "cotton.png", "cotton.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_COTTON_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Cotton");
	f->setAllTextures("cotton_blue.png");
	f->setInventoryTextureCube("cotton_blue.png", "cotton_blue.png", "cotton_blue.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_COTTON_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Cotton");
	f->setAllTextures("cotton_green.png");
	f->setInventoryTextureCube("cotton_green.png", "cotton_green.png", "cotton_green.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_COTTON_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Cotton");
	f->setAllTextures("cotton_orange.png");
	f->setInventoryTextureCube("cotton_orange.png", "cotton_orange.png", "cotton_orange.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_COTTON_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Cotton");
	f->setAllTextures("cotton_purple.png");
	f->setInventoryTextureCube("cotton_purple.png", "cotton_purple.png", "cotton_purple.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_COTTON_RED;
	f = &content_features(i);
	f->description = std::string("Red Cotton");
	f->setAllTextures("cotton_red.png");
	f->setInventoryTextureCube("cotton_red.png", "cotton_red.png", "cotton_red.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_COTTON_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Cotton");
	f->setAllTextures("cotton_yellow.png");
	f->setInventoryTextureCube("cotton_yellow.png", "cotton_yellow.png", "cotton_yellow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_COTTON_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Cotton");
	f->setAllTextures("cotton_black.png");
	f->setInventoryTextureCube("cotton_black.png", "cotton_black.png", "cotton_black.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CARPET;
	f = &content_features(i);
	f->description = std::string("Carpet");
	f->setAllTextures("cotton.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton.png", "cotton.png", "cotton.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CARPET_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Carpet");
	f->setAllTextures("cotton_blue.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_blue.png", "cotton_blue.png", "cotton_blue.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CARPET_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Carpet");
	f->setAllTextures("cotton_green.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_green.png", "cotton_green.png", "cotton_green.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CARPET_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Carpet");
	f->setAllTextures("cotton_orange.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_orange.png", "cotton_orange.png", "cotton_orange.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CARPET_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Carpet");
	f->setAllTextures("cotton_purple.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_purple.png", "cotton_purple.png", "cotton_purple.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CARPET_RED;
	f = &content_features(i);
	f->description = std::string("Red Carpet");
	f->setAllTextures("cotton_red.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_red.png", "cotton_red.png", "cotton_red.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CARPET_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Carpet");
	f->setAllTextures("cotton_yellow.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_yellow.png", "cotton_yellow.png", "cotton_yellow.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CARPET_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Carpet");
	f->setAllTextures("cotton_black.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_black.png", "cotton_black.png", "cotton_black.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_AIR;
	f = &content_features(i);
	f->description = std::string("Air");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_AIRLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->air_equivalent = true;

	i = CONTENT_WATER;
	f = &content_features(i);
	f->description = std::string("Water");
	f->setInventoryTextureCube("water.png", "water.png", "water.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID;
	f->light_propagates = true;
	f->solidness = 0; // Drawn separately, makes no faces
	f->visual_solidness = 1;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->liquid_type = LIQUID_FLOWING;
	f->liquid_alternative_flowing = CONTENT_WATER;
	f->liquid_alternative_source = CONTENT_WATERSOURCE;
	f->liquid_viscosity = WATER_VISC;
#ifndef SERVER
	if(!opaque_water)
		f->vertex_alpha = WATER_ALPHA;
	f->post_effect_color = video::SColor(64, 100, 100, 200);
	if(f->special_material == NULL && g_texturesource)
	{
		// Flowing water material
		f->special_material = new video::SMaterial;
		f->special_material->setFlag(video::EMF_LIGHTING, false);
		f->special_material->setFlag(video::EMF_BACK_FACE_CULLING, false);
		f->special_material->setFlag(video::EMF_BILINEAR_FILTER, false);
		f->special_material->setFlag(video::EMF_FOG_ENABLE, true);
		if(!opaque_water)
			f->special_material->MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
		AtlasPointer *pa_water1 = new AtlasPointer(g_texturesource->getTexture(
				g_texturesource->getTextureId("water.png")));
		f->special_material->setTexture(0, pa_water1->atlas);

		// Flowing water material, backface culled
		f->special_material2 = new video::SMaterial;
		*f->special_material2 = *f->special_material;
		f->special_material2->setFlag(video::EMF_BACK_FACE_CULLING, true);

		f->special_atlas = pa_water1;
	}
#endif

	i = CONTENT_WATERSOURCE;
	f = &content_features(i);
	f->description = std::string("Water");
	//f->setInventoryTexture("water.png");
	f->setInventoryTextureCube("water.png", "water.png", "water.png");
	if(new_style_water)
	{
		f->solidness = 0; // drawn separately, makes no faces
	}
	else // old style
	{
		f->solidness = 1;
#ifndef SERVER
		TileSpec t;
		if(g_texturesource)
			t.texture = g_texturesource->getTexture("water.png");

		if(!opaque_water){
			t.alpha = WATER_ALPHA;
			t.material_type = MATERIAL_ALPHA_VERTEX;
		}
		t.material_flags &= ~MATERIAL_FLAG_BACKFACE_CULLING;
		f->setAllTiles(t);
#endif
	}
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID_SOURCE;
	f->light_propagates = true;
	f->walkable = false;
	f->pointable = false;
	//f->diggable = false;
	f->buildable_to = true;
	f->liquid_type = LIQUID_SOURCE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->liquid_alternative_flowing = CONTENT_WATER;
	f->liquid_alternative_source = CONTENT_WATERSOURCE;
	f->liquid_viscosity = WATER_VISC;
#ifndef SERVER
	if(!opaque_water)
		f->vertex_alpha = WATER_ALPHA;
	f->post_effect_color = video::SColor(64, 100, 100, 200);
	if(f->special_material == NULL && g_texturesource)
	{
		// New-style water source material (mostly unused)
		f->special_material = new video::SMaterial;
		f->special_material->setFlag(video::EMF_LIGHTING, false);
		f->special_material->setFlag(video::EMF_BACK_FACE_CULLING, false);
		f->special_material->setFlag(video::EMF_BILINEAR_FILTER, false);
		f->special_material->setFlag(video::EMF_FOG_ENABLE, true);
		f->special_material->MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
		AtlasPointer *pa_water1 = new AtlasPointer(g_texturesource->getTexture(
				g_texturesource->getTextureId("water.png")));
		f->special_material->setTexture(0, pa_water1->atlas);
		f->special_atlas = pa_water1;
	}
#endif
	setWaterLikeDiggingProperties(f->digging_properties, 0.5);

	i = CONTENT_LAVA;
	f = &content_features(i);
	f->description = std::string("Lava");
	f->setInventoryTextureCube("lava.png", "lava.png", "lava.png");
	f->used_texturenames["lava.png"] = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID;
	f->light_propagates = false;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->visual_solidness = 1; // Does not completely cover block boundaries
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->flammable = 2; // can be set on fire
	f->liquid_type = LIQUID_FLOWING;
	f->liquid_alternative_flowing = CONTENT_LAVA;
	f->liquid_alternative_source = CONTENT_LAVASOURCE;
	f->liquid_viscosity = LAVA_VISC;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
	if(f->special_material == NULL && g_texturesource)
	{
		// Flowing lava material
		f->special_material = new video::SMaterial;
		f->special_material->setFlag(video::EMF_LIGHTING, false);
		f->special_material->setFlag(video::EMF_BACK_FACE_CULLING, false);
		f->special_material->setFlag(video::EMF_BILINEAR_FILTER, false);
		f->special_material->setFlag(video::EMF_FOG_ENABLE, true);
		f->special_material->MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

		AtlasPointer *pa_lava1 = new AtlasPointer(
			g_texturesource->getTexture(
				g_texturesource->getTextureId("lava.png")));
		f->special_material->setTexture(0, pa_lava1->atlas);

		// Flowing lava material, backface culled
		f->special_material2 = new video::SMaterial;
		*f->special_material2 = *f->special_material;
		f->special_material2->setFlag(video::EMF_BACK_FACE_CULLING, true);

		f->special_atlas = pa_lava1;
	}
#endif

	i = CONTENT_LAVASOURCE;
	f = &content_features(i);
	f->description = std::string("Lava");
	f->setInventoryTextureCube("lava.png", "lava.png", "lava.png");
	f->used_texturenames["ladder.png"] = true;
	if(new_style_water)
	{
		f->solidness = 0; // drawn separately, makes no faces
	}
	else // old style
	{
		f->solidness = 2;
#ifndef SERVER
		TileSpec t;
		if(g_texturesource)
			t.texture = g_texturesource->getTexture("lava.png");

		//t.alpha = 255;
		//t.material_type = MATERIAL_ALPHA_VERTEX;
		//t.material_flags &= ~MATERIAL_FLAG_BACKFACE_CULLING;
		f->setAllTiles(t);
#endif
	}
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID_SOURCE;
	f->light_propagates = false;
	f->light_source = LIGHT_MAX-1;
	f->walkable = false;
	f->pointable = false;
	//f->diggable = false;
	f->buildable_to = true;
	f->flammable = 2; // can be set on fire
	f->liquid_type = LIQUID_SOURCE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->liquid_alternative_flowing = CONTENT_LAVA;
	f->liquid_alternative_source = CONTENT_LAVASOURCE;
	f->liquid_viscosity = LAVA_VISC;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
	if(f->special_material == NULL && g_texturesource)
	{
		// New-style lava source material (mostly unused)
		f->special_material = new video::SMaterial;
		f->special_material->setFlag(video::EMF_LIGHTING, false);
		f->special_material->setFlag(video::EMF_BACK_FACE_CULLING, false);
		f->special_material->setFlag(video::EMF_BILINEAR_FILTER, false);
		f->special_material->setFlag(video::EMF_FOG_ENABLE, true);
		f->special_material->MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		AtlasPointer *pa_lava1 = new AtlasPointer(
			g_texturesource->getTexture(
				g_texturesource->getTextureId("lava.png")));
		f->special_material->setTexture(0, pa_lava1->atlas);

		f->special_atlas = pa_lava1;
	}
#endif
	setWaterLikeDiggingProperties(f->digging_properties, 0.5);

	i = CONTENT_FIRE;
	f = &content_features(i);
	f->description = std::string("Fire");
	f->setAllTextures("fire.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif

	i = CONTENT_FIRE_SHORTTERM;
	f = &content_features(i);
	f->description = std::string("Fire");
	f->setAllTextures("fire.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif

	i = CONTENT_TORCH;
	f = &content_features(i);
	f->description = std::string("Torch");
	f->setAllTextures("torch.png");
	f->setInventoryTexture("torch_inventory.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_TORCHLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_source = LIGHT_MAX-1;
	f->digging_properties.set("", DiggingProperties(true, 0.0, 0));

	i = CONTENT_SIGN_WALL;
	f = &content_features(i);
	f->description = std::string("Sign");
	f->setAllTextures("sign_wall.png");
	f->setInventoryTexture("sign_inventory.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_SIGNLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->digging_properties.set("", DiggingProperties(true, 0.5, 0));

	i = CONTENT_SIGN;
	f = &content_features(i);
	f->description = std::string("Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back.png");
	f->setTexture(5, "sign_front.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->digging_properties.set("", DiggingProperties(true, 0.5, 0));
	f->setNodeBox(core::aabbox3d<f32>(
		-0.05*BS,
		-0.5*BS,
		-0.05*BS,
		0.05*BS,
		0.5*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		0.,
		-0.1*BS,
		0.4*BS,
		0.4*BS,
		-0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_front.png", "sign.png");

	i = CONTENT_SIGN_UD;
	f = &content_features(i);
	f->description = std::string("Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back_ud.png");
	f->setTexture(5, "sign_front_ud.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->digging_properties.set("", DiggingProperties(true, 0.5, 0));
	f->setNodeBox(core::aabbox3d<f32>(
		-0.05*BS,
		-0.5*BS,
		-0.05*BS,
		0.05*BS,
		0.5*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		-0.4*BS,
		-0.1*BS,
		0.4*BS,
		0.,
		-0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_front.png", "sign.png");

	i = CONTENT_CHEST;
	f = &content_features(i);
	f->description = std::string("Chest");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("chest_side.png");
	f->setTexture(0, "chest_top.png");
	f->setTexture(1, "chest_top.png");
	f->setTexture(5, "chest_front.png"); // Z-
	f->setInventoryTexture("chest_top.png");
	f->setInventoryTextureCube("chest_top.png", "chest_front.png", "chest_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new ChestNodeMetadata();
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_LOCKABLE_CHEST;
	f = &content_features(i);
	f->description = std::string("Locking Chest");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("chest_side.png");
	f->setTexture(0, "chest_top.png");
	f->setTexture(1, "chest_top.png");
	f->setTexture(5, "chest_lock.png"); // Z-
	f->setInventoryTexture("chest_lock.png");
	f->setInventoryTextureCube("chest_top.png", "chest_lock.png", "chest_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingChestNodeMetadata();
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_FURNACE;
	f = &content_features(i);
	f->description = std::string("Furnace");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("furnace_side.png");
	f->setTexture(5, "furnace_front.png"); // Z-
	f->setInventoryTextureCube("furnace_side.png", "furnace_front.png", "furnace_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FurnaceNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 3.0);

	i = CONTENT_ROUGHSTONE;
	f = &content_features(i);
	f->description = std::string("Rough Stone");
	f->setAllTextures("roughstone.png");
	f->setInventoryTextureCube("roughstone.png", "roughstone.png", "roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_STONE)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_COBBLE;
	f = &content_features(i);
	f->description = std::string("Cobble Stone");
	f->setAllTextures("cobble.png");
	f->setInventoryTextureCube("cobble.png", "cobble.png", "cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_MOSSYCOBBLE;
	f = &content_features(i);
	f->description = std::string("Mossy Cobble Stone");
	f->setAllTextures("mossycobble.png");
	f->setInventoryTextureCube("mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 0.8);

	i = CONTENT_STEEL;
	f = &content_features(i);
	f->description = std::string("Steel Block");
	f->setAllTextures("steel_block.png");
	f->setInventoryTextureCube("steel_block.png", "steel_block.png", "steel_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_COPPER;
	f = &content_features(i);
	f->description = std::string("Copper Block");
	f->setAllTextures("copper_block.png");
	f->setInventoryTextureCube("copper_block.png", "copper_block.png", "copper_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_GOLD;
	f = &content_features(i);
	f->description = std::string("Gold Block");
	f->setAllTextures("gold_block.png");
	f->setInventoryTextureCube("gold_block.png", "gold_block.png", "gold_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_SILVER;
	f = &content_features(i);
	f->description = std::string("Silver Block");
	f->setAllTextures("silver_block.png");
	f->setInventoryTextureCube("silver_block.png", "silver_block.png", "silver_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_TIN;
	f = &content_features(i);
	f->description = std::string("Tin Block");
	f->setAllTextures("tin_block.png");
	f->setInventoryTextureCube("tin_block.png", "tin_block.png", "tin_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_QUARTZ;
	f = &content_features(i);
	f->description = std::string("Quartz Block");
	f->setAllTextures("quartz_block.png");
	f->setInventoryTextureCube("quartz_block.png", "quartz_block.png", "quartz_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_NC;
	f = &content_features(i);
	f->description = std::string("Nyan Cat");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("nc_side.png");
	f->setTexture(5, "nc_front.png"); // Z-
	f->setTexture(4, "nc_back.png"); // Z+
	f->setInventoryTextureCube("nc_front.png", "nc_side.png", "nc_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 3.0);

	i = CONTENT_NC_RB;
	f = &content_features(i);
	f->description = std::string("Rainbow");
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("nc_rb.png");
	f->setInventoryTextureCube("nc_rb.png", "nc_rb.png", "nc_rb.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 3.0);

	i = CONTENT_SAPLING;
	f = &content_features(i);
	f->description = std::string("Sapling");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->setAllTextures("sapling.png");
	f->setInventoryTexture("sapling.png");
	f->setAllTextureFlags(0);
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_propagates = true;
	f->air_equivalent = false;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->digging_properties.set("", DiggingProperties(true, 0.0, 0));

	i = CONTENT_JUNGLESAPLING;
	f = &content_features(i);
	f->description = std::string("Jungle Sapling");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->setAllTextures("junglesapling.png");
	f->setInventoryTexture("junglesapling.png");
	f->setAllTextureFlags(0);
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_propagates = true;
	f->air_equivalent = false;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->digging_properties.set("", DiggingProperties(true, 0.0, 0));

	i = CONTENT_CONIFER_SAPLING;
	f = &content_features(i);
	f->description = std::string("Conifer Sapling");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->setAllTextures("conifer_sapling.png");
	f->setInventoryTexture("conifer_sapling.png");
	f->setAllTextureFlags(0);
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_propagates = true;
	f->air_equivalent = false;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->digging_properties.set("", DiggingProperties(true, 0.0, 0));

	i = CONTENT_APPLE;
	f = &content_features(i);
	f->description = std::string("Apple");
	f->setInventoryTexture("apple.png");
	f->setAllTextures("apple.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE_SML;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("CraftItem apple 1");
	f->digging_properties.set("", DiggingProperties(true, 0.0, 0));

	// slabs
	i = CONTENT_ROUGHSTONE_SLAB;
	f = &content_features(i);
	f->description = std::string("Rough Stone Slab");
	f->setAllTextures("roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_ROUGHSTONE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_COBBLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Cobble Stone Slab");
	f->setAllTextures("cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_COBBLE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_MOSSYCOBBLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Mossy Cobble Stone Slab");
	f->setAllTextures("mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_MOSSYCOBBLE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.8);

	i = CONTENT_STONE_SLAB;
	f = &content_features(i);
	f->description = std::string("Stone Slab");
	f->setAllTextures("stone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_STONE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_WOOD_SLAB;
	f = &content_features(i);
	f->description = std::string("Wood Slab");
	f->setAllTextures("wood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_WOOD;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_JUNGLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Jungle Wood Slab");
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_JUNGLEWOOD;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_BRICK_SLAB;
	f = &content_features(i);
	f->description = std::string("Brick Slab");
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	f->slab_cube_type = CONTENT_BRICK;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SANDSTONE_SLAB;
	f = &content_features(i);
	f->description = std::string("Sand Stone Slab");
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->slab_cube_type = CONTENT_SANDSTONE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_GLASS_SLAB;
	f = &content_features(i);
	f->description = std::string("Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass.png", "glass.png", "glass.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_BLUE_SLAB;
	f = &content_features(i);
	f->description = std::string("Blue Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_BLUE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_blue.png", "glass_blue.png", "glass_blue.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_GREEN_SLAB;
	f = &content_features(i);
	f->description = std::string("Green Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_GREEN;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_green.png", "glass_green.png", "glass_green.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_ORANGE_SLAB;
	f = &content_features(i);
	f->description = std::string("Orange Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_ORANGE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_orange.png", "glass_orange.png", "glass_orange.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PURPLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Purple Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_PURPLE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_purple.png", "glass_purple.png", "glass_purple.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_RED_SLAB;
	f = &content_features(i);
	f->description = std::string("Red Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_RED;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_red.png", "glass_red.png", "glass_red.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_YELLOW_SLAB;
	f = &content_features(i);
	f->description = std::string("Yellow Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_YELLOW;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_yellow.png", "glass_yellow.png", "glass_yellow.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_BLACK_SLAB;
	f = &content_features(i);
	f->description = std::string("Black Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_BLACK;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_black.png", "glass_black.png", "glass_black.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	// upside down slabs
	i = CONTENT_ROUGHSTONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_ROUGHSTONE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_COBBLE_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Cobble Stone Slab");
	f->setAllTextures("cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_COBBLE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_MOSSYCOBBLE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MOSSYCOBBLE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_MOSSYCOBBLE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.8);

	i = CONTENT_STONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("stone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STONE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_STONE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_WOOD_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("wood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_SLAB)+" 1";
	f->slab_cube_type = CONTENT_WOOD;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_JUNGLE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_JUNGLE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_JUNGLEWOOD;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_BRICK_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	f->slab_cube_type = CONTENT_BRICK;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SANDSTONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->slab_cube_type = CONTENT_SANDSTONE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_GLASS_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass.png", "glass.png", "glass.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_BLUE_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Blue Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_BLUE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_blue.png", "glass_blue.png", "glass_blue.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_GREEN_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Green Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_GREEN;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_green.png", "glass_green.png", "glass_green.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_ORANGE_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Orange Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_ORANGE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_orange.png", "glass_orange.png", "glass_orange.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_PURPLE_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Purple Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_PURPLE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_purple.png", "glass_purple.png", "glass_purple.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_RED_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Red Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_RED;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_red.png", "glass_red.png", "glass_red.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_YELLOW_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Yellow Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_YELLOW;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_yellow.png", "glass_yellow.png", "glass_yellow.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_GLASS_BLACK_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Black Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_BLACK;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_black.png", "glass_black.png", "glass_black.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	// stairs
	i = CONTENT_ROUGHSTONE_STAIR;
	f = &content_features(i);
	f->description = std::string("Rough Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("roughstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_COBBLE_STAIR;
	f = &content_features(i);
	f->description = std::string("Cobble Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("cobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_MOSSYCOBBLE_STAIR;
	f = &content_features(i);
	f->description = std::string("Mossy Cobble Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("mossycobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.8);

	i = CONTENT_STONE_STAIR;
	f = &content_features(i);
	f->description = std::string("Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("stone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->often_contains_mineral = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_WOOD_STAIR;
	f = &content_features(i);
	f->description = std::string("Wood Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("wood.png");
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_JUNGLE_STAIR;
	f = &content_features(i);
	f->description = std::string("Jungle Wood Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_BRICK_STAIR;
	f = &content_features(i);
	f->description = std::string("Brick Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SANDSTONE_STAIR;
	f = &content_features(i);
	f->description = std::string("Sand Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	// upside down stairs
	i = CONTENT_ROUGHSTONE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("roughstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_COBBLE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("cobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_MOSSYCOBBLE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("mossycobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MOSSYCOBBLE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.8);

	i = CONTENT_STONE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("stone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STONE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_WOOD_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("wood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_JUNGLE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_JUNGLE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_BRICK_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SANDSTONE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	// doors
	i = CONTENT_WOOD_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(2,"door_wood_b.png^^[transformFX");
	f->setTexture(3,"door_wood_b.png^^[transformFX");
	f->setTexture(4,"door_wood_b.png^^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(2,"door_wood_t.png^^[transformFX");
	f->setTexture(3,"door_wood_t.png^^[transformFX");
	f->setTexture(4,"door_wood_t.png^^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(2,"door_steel_b.png^^[transformFX");
	f->setTexture(3,"door_steel_b.png^^[transformFX");
	f->setTexture(4,"door_steel_b.png^^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_t.png");
	f->setTexture(2,"door_steel_t.png^^[transformFX");
	f->setTexture(3,"door_steel_t.png^^[transformFX");
	f->setTexture(4,"door_steel_t.png^^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_GLASS_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(2,"door_glass_b.png^^[transformFX");
	f->setTexture(3,"door_glass_b.png^^[transformFX");
	f->setTexture(4,"door_glass_b.png^^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);
	i = CONTENT_GLASS_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(2,"door_glass_t.png^^[transformFX");
	f->setTexture(3,"door_glass_t.png^^[transformFX");
	f->setTexture(4,"door_glass_t.png^^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_WOOD_W_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(2,"door_wood_wb.png^[transformFX");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setTexture(4,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_W_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(2,"door_wood_wt.png^[transformFX");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setTexture(4,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorw(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(2,"door_steel_wb.png^[transformFX");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setTexture(4,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_W_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(2,"door_steel_wt.png^[transformFX");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setTexture(4,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_doorw(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	// right doors
	i = CONTENT_WOOD_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_b.png^[transformFX");
	f->setTexture(2,"door_wood_b.png");
	f->setTexture(3,"door_wood_b.png");
	f->setTexture(4,"door_wood_b.png");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_t.png^[transformFX");
	f->setTexture(2,"door_wood_t.png");
	f->setTexture(3,"door_wood_t.png");
	f->setTexture(4,"door_wood_t.png");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_GLASS_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_b.png^[transformFX");
	f->setTexture(2,"door_glass_b.png");
	f->setTexture(3,"door_glass_b.png");
	f->setTexture(4,"door_glass_b.png");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);
	i = CONTENT_GLASS_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_t.png^[transformFX");
	f->setTexture(2,"door_glass_t.png");
	f->setTexture(3,"door_glass_t.png");
	f->setTexture(4,"door_glass_t.png");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_STEEL_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png^[transformFX");
	f->setTexture(2,"door_steel_b.png");
	f->setTexture(3,"door_steel_b.png");
	f->setTexture(4,"door_steel_b.png");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_t.png^[transformFX");
	f->setTexture(2,"door_steel_t.png");
	f->setTexture(3,"door_steel_t.png");
	f->setTexture(4,"door_steel_t.png");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_WOOD_W_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png^[transformFX");
	f->setTexture(2,"door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png");
	f->setTexture(4,"door_wood_wb.png");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_W_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png^[transformFX");
	f->setTexture(2,"door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png");
	f->setTexture(4,"door_wood_wt.png");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorw(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png^[transformFX");
	f->setTexture(2,"door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png");
	f->setTexture(4,"door_steel_wb.png");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_W_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png^[transformFX");
	f->setTexture(2,"door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png");
	f->setTexture(4,"door_steel_wt.png");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_doorw(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	// open doors
	i = CONTENT_WOOD_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(3,"door_wood_b.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(3,"door_wood_t.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_GLASS_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(3,"door_glass_b.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);
	i = CONTENT_GLASS_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(3,"door_glass_t.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_STEEL_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_b.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_t.png");
	f->setTexture(3,"door_steel_t.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_WOOD_W_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_W_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorwol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_W_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorwol(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	// right open doors
	i = CONTENT_WOOD_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(3,"door_wood_b.png^[transformFX");
	f->setTexture(4,"door_wood_b.png^[transformFX");
	f->setTexture(5,"door_wood_b.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_dooror(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(3,"door_wood_t.png^[transformFX");
	f->setTexture(4,"door_wood_t.png^[transformFX");
	f->setTexture(5,"door_wood_t.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_dooror(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_GLASS_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(2,"door_glass_b.png^^[transformFX");
	f->setTexture(3,"door_glass_b.png^^[transformFX");
	f->setTexture(4,"door_glass_b.png^^[transformFX");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_dooror(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);
	i = CONTENT_GLASS_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(2,"door_glass_t.png^^[transformFX");
	f->setTexture(3,"door_glass_t.png^^[transformFX");
	f->setTexture(4,"door_glass_t.png^^[transformFX");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_dooror(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_STEEL_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_b.png^[transformFX");
	f->setTexture(4,"door_steel_b.png^[transformFX");
	f->setTexture(5,"door_steel_b.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_t.png^[transformFX");
	f->setTexture(4,"door_steel_t.png^[transformFX");
	f->setTexture(5,"door_steel_t.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_WOOD_W_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setTexture(4,"door_wood_wb.png^[transformFX");
	f->setTexture(5,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_dooror(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_W_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setTexture(4,"door_wood_wt.png^[transformFX");
	f->setTexture(5,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_doorwor(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setTexture(4,"door_steel_wb.png^[transformFX");
	f->setTexture(5,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_W_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setTexture(4,"door_steel_wt.png^[transformFX");
	f->setTexture(5,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_RT)+" 1";
	content_mapnode_nodebox_doorwor(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	// hatches
	i = CONTENT_WOOD_HATCH;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_inv.png");
	f->wield_nodebox = false;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_hatch(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_HATCH;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_inv.png");
	f->wield_nodebox = false;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_hatch(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_WOOD_W_HATCH;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Hatch");
	f->setAllTextures("hatch_wood_w.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_hatchw(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_HATCH;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Hatch");
	f->setAllTextures("hatch_steel_w.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_hatchw(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	// gates
	i = CONTENT_WOOD_GATE;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Gate");
	f->setAllTextures("gate_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_gate(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_GATE;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Gate");
	f->setAllTextures("gate_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_gate(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	// open hatches
	i = CONTENT_WOOD_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood.png");
	f->setTexture(2,"hatch_wood.png^[transformR90");
	f->setTexture(3,"hatch_wood.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_HATCH)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_hatcho(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel.png");
	f->setTexture(2,"hatch_steel.png^[transformR90");
	f->setTexture(3,"hatch_steel.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_HATCH)+" 1";
	content_mapnode_nodebox_hatcho(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_WOOD_W_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood_w.png");
	f->setTexture(2,"hatch_wood.png^[transformR90");
	f->setTexture(3,"hatch_wood.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_HATCH)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_hatchwo(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel_w.png");
	f->setTexture(2,"hatch_steel.png^[transformR90");
	f->setTexture(3,"hatch_steel.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_HATCH)+" 1";
	content_mapnode_nodebox_hatchwo(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	// open gates
	i = CONTENT_WOOD_GATE_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Gate");
	f->setAllTextures("gate_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_GATE)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_gateo(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_GATE_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Gate");
	f->setAllTextures("gate_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_GATE)+" 1";
	content_mapnode_nodebox_gateo(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	// plants
	i = CONTENT_WILDGRASS_SHORT;
	f = &content_features(i);
	f->description = std::string("Wild Grass");
	f->setInventoryTexture("wildgrass_short.png");
	f->setAllTextures("wildgrass_short.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	//f->pointable = false;
	f->buildable_to = true;
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_WILDGRASS_LONG;
	f = &content_features(i);
	f->description = std::string("Wild Grass");
	f->setInventoryTexture("wildgrass_long.png");
	f->setAllTextures("wildgrass_long.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WILDGRASS_SHORT)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->buildable_to = true;
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_DEADGRASS;
	f = &content_features(i);
	f->description = std::string("Dead Grass");
	f->setInventoryTexture("deadgrass.png");
	f->setAllTextures("deadgrass.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->buildable_to = true;
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_FLOWER_STEM;
	f = &content_features(i);
	f->description = std::string("Flower Stem");
	f->setInventoryTexture("flower_stem.png");
	f->setAllTextures("flower_stem.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->buildable_to = true;
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_FLOWER_ROSE;
	f = &content_features(i);
	f->description = std::string("Rose");
	f->setInventoryTexture("flower_rose.png");
	f->setAllTextures("flower_rose.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->buildable_to = true;
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_FLOWER_DAFFODIL;
	f = &content_features(i);
	f->description = std::string("Daffodil");
	f->setInventoryTexture("flower_daffodil.png");
	f->setAllTextures("flower_daffodil.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->buildable_to = true;
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_FLOWER_TULIP;
	f = &content_features(i);
	f->description = std::string("Tulip");
	f->setInventoryTexture("flower_tulip.png");
	f->setAllTextures("flower_tulip.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->buildable_to = true;
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_FLOWER_POT_RAW;
	f = &content_features(i);
	f->param_type = CPT_LIGHT;
	f->description = std::string("Unbaked Flower Pot");
	f->setAllTextures("flower_pot_raw.png");
	f->setTexture(0,"flower_pot_raw_top.png");
	f->setTexture(1,"flower_pot_raw_bottom.png");
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_FLOWER_POT)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 0.75);
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		0.3*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS,
		0.2*BS,
		-0.3*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		-0.3*BS,
		-0.3*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS,
		-0.5*BS,
		-0.3*BS,
		0.3*BS,
		0.35*BS,
		0.3*BS
	));
	f->setInventoryTextureNodeBox(i,"flower_pot_raw_top.png","flower_pot_raw.png","flower_pot_raw.png");

	i = CONTENT_FLOWER_POT;
	f = &content_features(i);
	f->param_type = CPT_LIGHT;
	f->description = std::string("Flower Pot");
	f->setAllTextures("flower_pot.png");
	f->setTexture(0,"flower_pot_top.png");
	f->setTexture(1,"flower_pot_bottom.png");
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 0.75);
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		0.3*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS,
		0.2*BS,
		-0.3*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		-0.3*BS,
		-0.3*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS,
		-0.5*BS,
		-0.3*BS,
		0.3*BS,
		0.35*BS,
		0.3*BS
	));
	f->setInventoryTextureNodeBox(i,"flower_pot_top.png","flower_pot.png","flower_pot.png");

	// walls
	i = CONTENT_COBBLE_WALL;
	f = &content_features(i);
	f->description = std::string("Cobblestone Wall");
	f->setAllTextures("cobble.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("cobble_wall_inv.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_ROUGHSTONE_WALL;
	f = &content_features(i);
	f->description = std::string("Rough Stone Wall");
	f->setAllTextures("roughstone.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("roughstone_wall_inv.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.9);

	i = CONTENT_MOSSYCOBBLE_WALL;
	f = &content_features(i);
	f->description = std::string("Mossy Cobblestone Wall");
	f->setAllTextures("mossycobble.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("mossycobble_wall_inv.png");
	setStoneLikeDiggingProperties(f->digging_properties, 0.8);

	i = CONTENT_STONE_WALL;
	f = &content_features(i);
	f->description = std::string("Stone Wall");
	f->setAllTextures("stone.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("stone_wall_inv.png");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SANDSTONE_WALL;
	f = &content_features(i);
	f->description = std::string("Sand Stone Wall");
	f->setAllTextures("sandstone.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("sandstone_wall_inv.png");
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_TNT;
	f = &content_features(i);
	f->description = std::string("TNT");
	f->setAllTextures("tnt.png");
	f->setTexture(0, "tnt_top.png");
	f->setTexture(1, "tnt_bottom.png");
	f->setInventoryTextureCube("tnt_top.png", "tnt.png", "tnt.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new TNTNodeMetadata();
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_FLASH;
	f = &content_features(i);
	f->description = std::string("In-Progress explosion - how did you get this???");
	f->setAllTextures("flash.png");
	f->solidness = 0; // drawn separately, makes no faces
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 50;
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
	f->post_effect_color = video::SColor(192, 255, 255, 64);
#endif

	i = CONTENT_STEAM;
	f = &content_features(i);
	f->description = std::string("Steam");
	f->setAllTextures("steam.png");
	f->solidness = 0;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 4;
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
	f->post_effect_color = video::SColor(120, 200, 200, 200);
#endif

	i = CONTENT_CHAIR;
	f = &content_features(i);
	f->description = std::string("Chair");
	f->setAllTextures("finished_wood.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->rotate_tile_with_nodebox = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->setNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.5*BS, 0.20*BS, -0.2*BS, 0.5*BS, 0.30*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.2*BS, -0.5*BS, 0.20*BS, 0.3*BS, 0.5*BS, 0.30*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.5*BS, -0.30*BS, -0.2*BS, -0.1*BS, -0.20*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.2*BS, -0.5*BS, -0.30*BS, 0.3*BS, -0.1*BS, -0.20*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.1*BS, -0.30*BS, 0.3*BS, 0.0*BS, 0.20*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.2*BS, 0.1*BS, 0.25*BS, 0.2*BS, 0.4*BS, 0.26*BS
	));
	f->setInventoryTextureNodeBox(i,"finished_wood.png", "finished_wood.png", "finished_wood.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_TABLE;
	f = &content_features(i);
	f->description = std::string("Table");
	f->setAllTextures("finished_wood.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4*BS, -0.5*BS, -0.4*BS, -0.3*BS, 0.4*BS, -0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS, -0.5*BS, -0.4*BS, 0.4*BS, 0.4*BS, -0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS, -0.5*BS, 0.3*BS, -0.3*BS, 0.4*BS, 0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS, -0.5*BS, 0.3*BS, 0.4*BS, 0.4*BS, 0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS, 0.4*BS, -0.5*BS, 0.5*BS, 0.5*BS, 0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS, -0.2*BS, -0.3*BS, -0.3*BS, -0.1*BS, 0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS, -0.2*BS, -0.4*BS, 0.4*BS, -0.1*BS, 0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.2*BS, -0.4*BS, 0.4*BS, -0.1*BS, -0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.2*BS, 0.3*BS, 0.3*BS, -0.1*BS, 0.4*BS
	));
	f->setInventoryTextureNodeBox(i,"finished_wood.png", "finished_wood.png", "finished_wood.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_side.png");
	f->setTexture(3,"bed_side.png^[transformFX");
	f->setTexture(4,"bed_end.png");
	f->setTexture(5,"bed_end.png");
	f->setInventoryTexture("bed_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_side.png^[transformFX");
	f->setTexture(3,"bed_side.png");
	f->setTexture(4,"bed_end.png");
	f->setTexture(5,"bed_end.png");
	f->setInventoryTexture("bed_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_BLUE_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_blue_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_blue_side.png");
	f->setTexture(3,"bed_blue_side.png^[transformFX");
	f->setTexture(4,"bed_blue_end.png");
	f->setTexture(5,"bed_blue_end.png");
	f->setInventoryTexture("bed_blue_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_BLUE_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_blue_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_blue_side.png^[transformFX");
	f->setTexture(3,"bed_blue_side.png");
	f->setTexture(4,"bed_blue_end.png");
	f->setTexture(5,"bed_blue_end.png");
	f->setInventoryTexture("bed_blue_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_BLUE_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_GREEN_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_green_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_green_side.png");
	f->setTexture(3,"bed_green_side.png^[transformFX");
	f->setTexture(4,"bed_green_end.png");
	f->setTexture(5,"bed_green_end.png");
	f->setInventoryTexture("bed_green_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_GREEN_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_green_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_green_side.png^[transformFX");
	f->setTexture(3,"bed_green_side.png");
	f->setTexture(4,"bed_green_end.png");
	f->setTexture(5,"bed_green_end.png");
	f->setInventoryTexture("bed_green_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_GREEN_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_ORANGE_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_orange_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_orange_side.png");
	f->setTexture(3,"bed_orange_side.png^[transformFX");
	f->setTexture(4,"bed_orange_end.png");
	f->setTexture(5,"bed_orange_end.png");
	f->setInventoryTexture("bed_orange_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_ORANGE_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_orange_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_orange_side.png^[transformFX");
	f->setTexture(3,"bed_orange_side.png");
	f->setTexture(4,"bed_orange_end.png");
	f->setTexture(5,"bed_orange_end.png");
	f->setInventoryTexture("bed_orange_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_ORANGE_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_PURPLE_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_purple_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_purple_side.png");
	f->setTexture(3,"bed_purple_side.png^[transformFX");
	f->setTexture(4,"bed_purple_end.png");
	f->setTexture(5,"bed_purple_end.png");
	f->setInventoryTexture("bed_purple_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_PURPLE_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_purple_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_purple_side.png^[transformFX");
	f->setTexture(3,"bed_purple_side.png");
	f->setTexture(4,"bed_purple_end.png");
	f->setTexture(5,"bed_purple_end.png");
	f->setInventoryTexture("bed_purple_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_PURPLE_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_RED_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_red_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_red_side.png");
	f->setTexture(3,"bed_red_side.png^[transformFX");
	f->setTexture(4,"bed_red_end.png");
	f->setTexture(5,"bed_red_end.png");
	f->setInventoryTexture("bed_red_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_RED_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_red_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_red_side.png^[transformFX");
	f->setTexture(3,"bed_red_side.png");
	f->setTexture(4,"bed_red_end.png");
	f->setTexture(5,"bed_red_end.png");
	f->setInventoryTexture("bed_red_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_RED_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_YELLOW_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_yellow_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_yellow_side.png");
	f->setTexture(3,"bed_yellow_side.png^[transformFX");
	f->setTexture(4,"bed_yellow_end.png");
	f->setTexture(5,"bed_yellow_end.png");
	f->setInventoryTexture("bed_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_YELLOW_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_yellow_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_yellow_side.png^[transformFX");
	f->setTexture(3,"bed_yellow_side.png");
	f->setTexture(4,"bed_yellow_end.png");
	f->setTexture(5,"bed_yellow_end.png");
	f->setInventoryTexture("bed_yellow_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_YELLOW_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_BLACK_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_black_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_black_side.png");
	f->setTexture(3,"bed_black_side.png^[transformFX");
	f->setTexture(4,"bed_black_end.png");
	f->setTexture(5,"bed_black_end.png");
	f->setInventoryTexture("bed_black_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BED_BLACK_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"bed_black_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"bed_black_side.png^[transformFX");
	f->setTexture(3,"bed_black_side.png");
	f->setTexture(4,"bed_black_end.png");
	f->setTexture(5,"bed_black_end.png");
	f->setInventoryTexture("bed_black_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_BLACK_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	// NOTE: Remember to add frequently used stuff to the texture atlas in tile.cpp


	/*
		Add MesePick to everything
	*/
	for(u16 i=0; i<=MAX_CONTENT; i++)
	{
		content_features(i).digging_properties.set("MesePick",
				DiggingProperties(true, 0.0, 65535./1337));
	}

}

void setStoneLikeDiggingProperties(DiggingPropertiesList &list, float toughness)
{
	list.set("",
			DiggingProperties(true, 15.0*toughness, 0));

	list.set("WPick",
			DiggingProperties(true, 1.3*toughness, 65535./30.*toughness));
	list.set("STPick",
			DiggingProperties(true, 0.75*toughness, 65535./100.*toughness));
	list.set("SteelPick",
			DiggingProperties(true, 0.50*toughness, 65535./333.*toughness));
}

void setDirtLikeDiggingProperties(DiggingPropertiesList &list, float toughness)
{
	list.set("",
			DiggingProperties(true, 0.75*toughness, 0));

	list.set("WShovel",
			DiggingProperties(true, 0.4*toughness, 65535./50.*toughness));
	list.set("STShovel",
			DiggingProperties(true, 0.2*toughness, 65535./150.*toughness));
	list.set("SteelShovel",
			DiggingProperties(true, 0.15*toughness, 65535./400.*toughness));
}

void setWoodLikeDiggingProperties(DiggingPropertiesList &list, float toughness)
{
	list.set("",
			DiggingProperties(true, 3.0*toughness, 0));

	list.set("WAxe",
			DiggingProperties(true, 1.5*toughness, 65535./30.*toughness));
	list.set("STAxe",
			DiggingProperties(true, 0.75*toughness, 65535./100.*toughness));
	list.set("SteelAxe",
			DiggingProperties(true, 0.5*toughness, 65535./333.*toughness));
}

void setLeafLikeDiggingProperties(DiggingPropertiesList &list, float toughness)
{
	list.set("",
			DiggingProperties(true, 3.0*toughness, 0));

	list.set("WAxe",
			DiggingProperties(true, 1.5*toughness, 65535./20.*toughness));
	list.set("STAxe",
			DiggingProperties(true, 0.75*toughness, 65535./80.*toughness));
	list.set("SteelAxe",
			DiggingProperties(true, 0.5*toughness, 65535./200.*toughness));
	list.set("Shears",
			DiggingProperties(true, 0.5*toughness, 65535./333.*toughness));
}

void setWaterLikeDiggingProperties(DiggingPropertiesList &list, float toughness)
{
	list.set("",
			DiggingProperties(true, 150000.0*toughness, 0));

	list.set("WBucket",
			DiggingProperties(true, 0.0, 65535./30.*toughness));

	list.set("TinBucket",
			DiggingProperties(true, 0.0, 65535./50.*toughness));

	list.set("SteelBucket",
			DiggingProperties(true, 0.0, 65535./80.*toughness));
}
