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
		-0.5*BS,
		0.5*BS,
		0.,
		0.
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
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_COBBLE)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);
	if(invisible_stone)
		f->solidness = 0; // For debugging, hides regular stone

	i = CONTENT_STONEBRICK;
	f = &content_features(i);
	f->description = std::string("Stone Brick");
	f->setAllTextures("stonebrick.png");
	f->setInventoryTextureCube("stonebrick.png", "stonebrick.png", "stonebrick.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_STONEBLOCK;
	f = &content_features(i);
	f->description = std::string("Stone Block");
	f->setAllTextures("stoneblock.png");
	f->setInventoryTextureCube("stoneblock.png", "stoneblock.png", "stoneblock.png");
	f->param_type = CPT_NONE;
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
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_FARM_DIRT;
	f = &content_features(i);
	f->description = std::string("Farm Dirt");
	f->setAllTextures("dirt.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_MUD;
	f = &content_features(i);
	f->description = std::string("Mud");
	f->setAllTextures("mud.png");
	f->setInventoryTextureCube("mud.png", "mud.png", "mud.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_SAND;
	f = &content_features(i);
	f->description = std::string("Sand");
	f->setAllTextures("sand.png");
	f->setInventoryTextureCube("sand.png", "sand.png", "sand.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_GRAVEL;
	f = &content_features(i);
	f->description = std::string("Gravel");
	f->setAllTextures("gravel.png");
	f->setInventoryTextureCube("gravel.png", "gravel.png", "gravel.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setDirtLikeDiggingProperties(f->digging_properties, 1.75);

	i = CONTENT_SANDSTONE;
	f = &content_features(i);
	f->description = std::string("Sand Stone");
	f->setAllTextures("sandstone.png");
	f->setInventoryTextureCube("sandstone.png", "sandstone.png", "sandstone.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_CLAY;
	f = &content_features(i);
	f->description = std::string("Clay");
	f->setAllTextures("clay.png");
	f->setInventoryTextureCube("clay.png", "clay.png", "clay.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_BRICK;
	f = &content_features(i);
	f->description = std::string("Brick");
	f->setAllTextures("brick.png");
	f->setInventoryTextureCube("brick.png", "brick.png", "brick.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	setStoneLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_TREE;
	f = &content_features(i);
	f->description = std::string("Tree");
	f->setAllTextures("tree.png");
	f->setTexture(0, "tree_top.png");
	f->setTexture(1, "tree_top.png");
	f->setInventoryTextureCube("tree_top.png", "tree.png", "tree.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_JUNGLETREE;
	f = &content_features(i);
	f->description = std::string("Jungle Tree");
	f->setAllTextures("jungletree.png");
	f->setTexture(0, "jungletree_top.png");
	f->setTexture(1, "jungletree_top.png");
	f->setInventoryTextureCube("jungletree_top.png", "jungletree.png", "jungletree.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	//f->is_ground_content = true;
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
	setWoodLikeDiggingProperties(f->digging_properties, 0.10);

	i = CONTENT_LEAVES;
	f = &content_features(i);
	f->description = std::string("Leaves");
	f->light_propagates = true;
	//f->param_type = CPT_MINERAL;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if(new_style_leaves)
	{
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("leaves.png");
		f->setInventoryTextureCube("leaves.png", "leaves.png", "leaves.png");
	}
	else
	{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:leaves.png");
	}
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAPLING)+" 1";
	f->extra_dug_item_rarity = 20;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setWoodLikeDiggingProperties(f->digging_properties, 0.15);

	i = CONTENT_CACTUS;
	f = &content_features(i);
	f->description = std::string("Cactus");
	f->setAllTextures("cactus_side.png");
	f->setTexture(0, "cactus_top.png");
	f->setTexture(1, "cactus_top.png");
	f->setInventoryTextureCube("cactus_top.png", "cactus_side.png", "cactus_side.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
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
	setWoodLikeDiggingProperties(f->digging_properties, 0.25);

	i = CONTENT_BOOKSHELF;
	f = &content_features(i);
	f->description = std::string("Book Shelf");
	f->setAllTextures("bookshelf.png");
	f->setTexture(0, "wood.png");
	f->setTexture(1, "wood.png");
	f->setInventoryTextureCube("wood.png", "bookshelf.png", "bookshelf.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_GLASS;
	f = &content_features(i);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass.png");
	f->setInventoryTextureCube("glass.png", "glass.png", "glass.png");
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
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_JUNGLEWOOD;
	f = &content_features(i);
	f->description = std::string("Jungle Wood");
	f->setAllTextures("junglewood.png");
	f->setInventoryTextureCube("junglewood.png", "junglewood.png", "junglewood.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_MESE;
	f = &content_features(i);
	f->description = std::string("Mese");
	f->setAllTextures("mese.png");
	f->setInventoryTextureCube("mese.png", "mese.png", "mese.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	setStoneLikeDiggingProperties(f->digging_properties, 0.5);

	i = CONTENT_COTTON;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("cotton.png");
	f->setInventoryTextureCube("cotton.png", "cotton.png", "cotton.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
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
	f->draw_type = CDT_LIQUID_SOURCE;
	f->light_propagates = true;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
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

	i = CONTENT_LAVA;
	f = &content_features(i);
	f->description = std::string("Lava");
	f->setInventoryTextureCube("lava.png", "lava.png", "lava.png");
	f->used_texturenames["lava.png"] = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_LIQUID;
	f->light_propagates = false;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->visual_solidness = 1; // Does not completely cover block boundaries
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
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
	f->draw_type = CDT_LIQUID_SOURCE;
	f->light_propagates = false;
	f->light_source = LIGHT_MAX-1;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
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

	i = CONTENT_TORCH;
	f = &content_features(i);
	f->description = std::string("Torch");
	f->setAllTextures("torch.png");
	f->setInventoryTexture("torch_inventory.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_TORCHLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->wall_mounted = true;
	f->air_equivalent = true;
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
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->digging_properties.set("", DiggingProperties(true, 0.5, 0));

	i = CONTENT_SIGN;
	f = &content_features(i);
	f->description = std::string("Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back.png");
	f->setTexture(5, "sign_front.png"); // Z-
	f->setInventoryTexture("sign_inventory.png");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SIGN_WALL)+" 1";
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

	i = CONTENT_SIGN_UD;
	f = &content_features(i);
	f->description = std::string("Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back_ud.png");
	f->setTexture(5, "sign_front_ud.png"); // Z-
	f->setInventoryTexture("sign_inventory.png");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SIGN_WALL)+" 1";
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
	f->used_texturenames["sapling.png"] = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_propagates = true;
	f->air_equivalent = false;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->digging_properties.set("", DiggingProperties(true, 0.0, 0));

	i = CONTENT_APPLE;
	f = &content_features(i);
	f->description = std::string("Apple");
	f->setInventoryTexture("apple.png");
	f->setAllTextures("apple.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE_SML;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->air_equivalent = true;
	f->dug_item = std::string("CraftItem apple 1");
	f->digging_properties.set("", DiggingProperties(true, 0.0, 0));

	// slabs
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
	f->param_type = CPT_MINERAL;
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
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_JUNGLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Jungle Wood Slab");
	f->setAllTextures("junglewood.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_JUNGLEWOOD;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_BRICK_SLAB;
	f = &content_features(i);
	f->description = std::string("Brick Slab");
	f->setAllTextures("brick.png");
	f->param_type = CPT_MINERAL;
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
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->slab_cube_type = CONTENT_SANDSTONE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	// upside down slabs
	i = CONTENT_COBBLE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_COBBLE_SLAB)+" 1";
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
	f->param_type = CPT_MINERAL;
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
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_JUNGLE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("junglewood.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_JUNGLE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_JUNGLEWOOD;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	setWoodLikeDiggingProperties(f->digging_properties, 1.0);

	i = CONTENT_BRICK_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("brick.png");
	f->param_type = CPT_MINERAL;
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
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->slab_cube_type = CONTENT_SANDSTONE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	setDirtLikeDiggingProperties(f->digging_properties, 1.0);

	// stairs
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
	i = CONTENT_COBBLE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("cobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_COBBLE_STAIR)+" 1";
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_lb.png");
	f->setInventoryTexture("door_wood_l_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_DOOR_LT;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_lt.png");
	f->setInventoryTexture("door_wood_l_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_DOOR_LB;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_lb.png");
	f->setInventoryTexture("door_steel_l_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_lt.png");
	f->setInventoryTexture("door_steel_l_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

	i = CONTENT_WOOD_W_DOOR_LB;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wlb.png");
	f->setInventoryTexture("door_wood_wl_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_W_DOOR_LT;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wlt.png");
	f->setInventoryTexture("door_wood_wl_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_doorw(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_DOOR_LB;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wlb.png");
	f->setInventoryTexture("door_steel_wl_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wlt.png");
	f->setInventoryTexture("door_steel_wl_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_rb.png");
	f->setInventoryTexture("door_wood_r_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_DOOR_RT;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_rt.png");
	f->setInventoryTexture("door_wood_r_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_DOOR_RB;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_rb.png");
	f->setInventoryTexture("door_steel_r_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_rt.png");
	f->setInventoryTexture("door_steel_r_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wrb.png");
	f->setInventoryTexture("door_wood_wr_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_W_DOOR_RT;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wrt.png");
	f->setInventoryTexture("door_wood_wr_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_doorw(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_DOOR_RB;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wrb.png");
	f->setInventoryTexture("door_steel_wr_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wrt.png");
	f->setInventoryTexture("door_steel_wr_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_lb.png");
	f->setInventoryTexture("door_wood_l_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_lt.png");
	f->setInventoryTexture("door_wood_l_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_lb.png");
	f->setInventoryTexture("door_steel_l_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_lt.png");
	f->setInventoryTexture("door_steel_l_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wlb.png");
	f->setInventoryTexture("door_wood_wl_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_W_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wlt.png");
	f->setInventoryTexture("door_wood_wl_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorwol(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wlb.png");
	f->setInventoryTexture("door_steel_wl_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wlt.png");
	f->setInventoryTexture("door_steel_wl_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_rb.png");
	f->setInventoryTexture("door_wood_r_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_rt.png");
	f->setInventoryTexture("door_wood_r_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_rb.png");
	f->setInventoryTexture("door_steel_r_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_rt.png");
	f->setInventoryTexture("door_steel_r_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wrb.png");
	f->setInventoryTexture("door_wood_wr_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);
	i = CONTENT_WOOD_W_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wrt.png");
	f->setInventoryTexture("door_wood_wr_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_RT)+" 1";
	content_mapnode_nodebox_doorwor(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wrb.png");
	f->setInventoryTexture("door_wood_wr_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wrt.png");
	f->setInventoryTexture("door_wood_wr_inv.png");
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
	f->setInventoryTexture("hatch_wood_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_hatch(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_HATCH;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel.png");
	f->setInventoryTexture("hatch_steel_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_hatch(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();

	i = CONTENT_WOOD_W_HATCH;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Hatch");
	f->setAllTextures("hatch_wood_w.png");
	f->setInventoryTexture("hatch_wood_w_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_hatchw(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	i = CONTENT_STEEL_W_HATCH;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Hatch");
	f->setAllTextures("hatch_steel_w.png");
	f->setInventoryTexture("hatch_steel_w_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_hatchw(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();

	// open hatches
	i = CONTENT_WOOD_HATCH_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood.png");
	f->setInventoryTexture("hatch_wood_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_HATCH)+" 1";
	content_mapnode_nodebox_hatcho(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_HATCH_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel.png");
	f->setInventoryTexture("hatch_steel_inv.png");
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
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood_w.png");
	f->setInventoryTexture("hatch_wood_w_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_HATCH)+" 1";
	content_mapnode_nodebox_hatchwo(f);
	setWoodLikeDiggingProperties(f->digging_properties, 0.75);

	setStoneLikeDiggingProperties(f->digging_properties, 5.0);
	i = CONTENT_STEEL_W_HATCH_OPEN;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel_w.png");
	f->setInventoryTexture("hatch_steel_w_inv.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_HATCH)+" 1";
	content_mapnode_nodebox_hatchwo(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	setStoneLikeDiggingProperties(f->digging_properties, 5.0);

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


