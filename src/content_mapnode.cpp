/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mapnode.cpp
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

// For g_settings
#include "main.h"

#include "content_mapnode.h"
#include "mapnode.h"
#include "content_nodemeta.h"
#include "settings.h"
#include "content_craftitem.h"
#include "content_toolitem.h"
#include "content_craft.h"
#include "content_list.h"
#include "content_nodebox.h"
#include "content_mapnode_util.h"
#include "mineral.h"
#ifndef SERVER
#include "tile.h"
#endif
#include "intl.h"

#define WATER_ALPHA 160

#define WATER_VISC 1
#define LAVA_VISC 7


MapNode mapnode_translate_from_internal(MapNode n_from, u8 version)
{
	return n_from;
}
MapNode mapnode_translate_to_internal(MapNode n_from, u8 version)
{
	MapNode result = n_from;
	switch (n_from.getContent()) {
	case CONTENT_FARM_WHEAT_1:
		result.setContent(CONTENT_FARM_WHEAT);
		result.param2 = 4;
		break;
	case CONTENT_FARM_WHEAT_2:
		result.setContent(CONTENT_FARM_WHEAT);
		result.param2 = 8;
		break;
	case CONTENT_FARM_WHEAT_3:
		result.setContent(CONTENT_FARM_WHEAT);
		result.param2 = 12;
		break;
	case CONTENT_FARM_POTATO_1:
		result.setContent(CONTENT_FARM_POTATO);
		result.param2 = 4;
		break;
	case CONTENT_FARM_POTATO_2:
		result.setContent(CONTENT_FARM_POTATO);
		result.param2 = 8;
		break;
	case CONTENT_FARM_POTATO_3:
		result.setContent(CONTENT_FARM_POTATO);
		result.param2 = 12;
		break;
	case CONTENT_FARM_CARROT_1:
		result.setContent(CONTENT_FARM_CARROT);
		result.param2 = 4;
		break;
	case CONTENT_FARM_CARROT_2:
		result.setContent(CONTENT_FARM_CARROT);
		result.param2 = 8;
		break;
	case CONTENT_FARM_CARROT_3:
		result.setContent(CONTENT_FARM_CARROT);
		result.param2 = 12;
		break;
	case CONTENT_FARM_BEETROOT_1:
		result.setContent(CONTENT_FARM_BEETROOT);
		result.param2 = 4;
		break;
	case CONTENT_FARM_BEETROOT_2:
		result.setContent(CONTENT_FARM_BEETROOT);
		result.param2 = 8;
		break;
	case CONTENT_FARM_BEETROOT_3:
		result.setContent(CONTENT_FARM_BEETROOT);
		result.param2 = 12;
		break;
	case CONTENT_FARM_GRAPEVINE_1:
		result.setContent(CONTENT_FARM_GRAPEVINE);
		result.param2 = 4;
		break;
	case CONTENT_FARM_GRAPEVINE_2:
		result.setContent(CONTENT_FARM_GRAPEVINE);
		result.param2 = 8;
		break;
	case CONTENT_FARM_GRAPEVINE_3:
		result.setContent(CONTENT_FARM_GRAPEVINE);
		result.param2 = 12;
		break;
	case CONTENT_FARM_COTTON_1:
		result.setContent(CONTENT_FARM_COTTON);
		result.param2 = 4;
		break;
	case CONTENT_FARM_COTTON_2:
		result.setContent(CONTENT_FARM_COTTON);
		result.param2 = 8;
		break;
	case CONTENT_FARM_COTTON_3:
		result.setContent(CONTENT_FARM_COTTON);
		result.param2 = 12;
		break;
	case CONTENT_FARM_PUMPKIN_1:
		result.setContent(CONTENT_FARM_PUMPKIN);
		result.param2 = 4;
		break;
	case CONTENT_FARM_PUMPKIN_2:
		result.setContent(CONTENT_FARM_PUMPKIN);
		result.param2 = 8;
		break;
	case CONTENT_FARM_PUMPKIN_3:
		result.setContent(CONTENT_FARM_PUMPKIN);
		result.param2 = 12;
		break;
	case CONTENT_FARM_MELON_1:
		result.setContent(CONTENT_FARM_MELON);
		result.param2 = 4;
		break;
	case CONTENT_FARM_MELON_2:
		result.setContent(CONTENT_FARM_MELON);
		result.param2 = 8;
		break;
	case CONTENT_FARM_MELON_3:
		result.setContent(CONTENT_FARM_MELON);
		result.param2 = 12;
		break;
	case CONTENT_FARM_TRELLIS_GRAPE_1:
		result.setContent(CONTENT_FARM_TRELLIS_GRAPE);
		result.param2 = 4;
		break;
	case CONTENT_FARM_TRELLIS_GRAPE_2:
		result.setContent(CONTENT_FARM_TRELLIS_GRAPE);
		result.param2 = 8;
		break;
	case CONTENT_FARM_TRELLIS_GRAPE_3:
		result.setContent(CONTENT_FARM_TRELLIS_GRAPE);
		result.param2 = 12;
		break;
	default:;
	}
	return result;
}

void content_mapnode_init(bool repeat)
{
	// Read some settings
	bool invisible_stone = g_settings->getBool("invisible_stone");
#ifndef SERVER
	bool opaque_water = g_settings->getBool("opaque_water");
#endif

	content_t i;
	ContentFeatures *f = NULL;

	i = CONTENT_STONE;
	f = &content_features(i);
	f->description = wgettext("Stone");
	f->setAllTextures("stone.png");
	f->setInventoryTextureCube("stone.png", "stone.png", "stone.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->often_contains_mineral = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	if (invisible_stone)
		f->solidness = 0; // For debugging, hides regular stone
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_LIMESTONE;
	f = &content_features(i);
	f->description = wgettext("Limestone");
	f->setAllTextures("limestone.png");
	f->setInventoryTextureCube("limestone.png", "limestone.png", "limestone.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->often_contains_mineral = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_LIMESTONE)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.6;
	if (invisible_stone)
		f->solidness = 0; // For debugging, hides regular stone
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_ROCK;
	f = &content_features(i);
	f->description = wgettext("Rock");
	f->setAllTextures("stone.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 0.01;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->buildable_to = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->suffocation_per_second = 0;
	content_nodebox_rock(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	crafting::set1To2Recipe(CONTENT_STONE_KNOB,CONTENT_ROCK);
	crafting::set1To2Recipe(CONTENT_ROUGHSTONE_KNOB,CONTENT_ROCK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ICE;
	f = &content_features(i);
	f->description = wgettext("Ice");
	f->setAllTextures("ice.png");
	f->setInventoryTextureCube("ice.png", "ice.png", "ice.png");
	f->draw_type = CDT_CUBELIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->warmth_per_second = 2;
	lists::add("creative",i);

	i = CONTENT_COAL;
	f = &content_features(i);
	f->description = wgettext("Coal Block");
	f->setAllTextures("coal.png");
	f->setInventoryTextureCube("coal.png", "coal.png", "coal.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->fuel_time = 100;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_COAL,CONTENT_COAL);
	crafting::setUncraftHardBlockRecipe(CONTENT_COAL,CONTENT_CRAFTITEM_COAL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CHARCOAL;
	f = &content_features(i);
	f->description = wgettext("Charcoal Block");
	f->setAllTextures("charcoal.png");
	f->setInventoryTextureCube("charcoal.png", "charcoal.png", "charcoal.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->fuel_time = 100;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_CHARCOAL,CONTENT_CHARCOAL);
	crafting::setUncraftHardBlockRecipe(CONTENT_CHARCOAL,CONTENT_CRAFTITEM_CHARCOAL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONEBRICK;
	f = &content_features(i);
	f->description = wgettext("Stone Brick");
	f->setAllTextures("stonebrick.png");
	f->setInventoryTextureCube("stonebrick.png", "stonebrick.png", "stonebrick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBrickRecipe(CONTENT_STONE,CONTENT_STONEBRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONEBLOCK;
	f = &content_features(i);
	f->description = wgettext("Stone Block");
	f->setAllTextures("stoneblock.png");
	f->setInventoryTextureCube("stoneblock.png", "stoneblock.png", "stoneblock.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBlockRecipe(CONTENT_STONE,CONTENT_STONEBLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROUGHSTONEBRICK;
	f = &content_features(i);
	f->description = wgettext("Rough Stone Brick");
	f->setAllTextures("roughstone_brick.png");
	f->setInventoryTextureCube("roughstone_brick.png", "roughstone_brick.png", "roughstone_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_STONEBRICK)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBrickRecipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONEBRICK);
	lists::add("craftguide",i);
	lists::add("player-creative",i);
	lists::add("cooking",i);
	lists::add("creative",i);

	i = CONTENT_ROUGHSTONEBLOCK;
	f = &content_features(i);
	f->description = wgettext("Rough Stone Block");
	f->setAllTextures("roughstone_block.png");
	f->setInventoryTextureCube("roughstone_block.png", "roughstone_block.png", "roughstone_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_STONEBLOCK)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBlockRecipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONEBLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_GRASS;
	f = &content_features(i);
	f->description = wgettext("Grass");
	f->setAllTextures("mud.png^grass_side.png");
	f->setTexture(0, "grass.png");
	f->setTexture(1, "mud.png");
	f->setInventoryTextureCube("grass.png","mud.png^grass_side.png","mud.png^grass_side.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WILDGRASS_SHORT)+" 1";
	f->extra_dug_item_rarity = 10;
	f->sound_step = "grass-step";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->farm_ploughable = true;
	lists::add("decrafting",i);

	i = CONTENT_GROWING_GRASS;
	f = &content_features(i);
	f->description = wgettext("Growing Grass");
	f->setAllTextures("mud.png");
	f->setTexture(0, "grass_growing.png");
	f->setInventoryTextureCube("grass.png","mud.png","mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->param2_type = CPT_PLANTGROWTH;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->special_alternate_node = CONTENT_GRASS;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->farm_ploughable = true;

	i = CONTENT_GRASS_FOOTSTEPS;
	f = &content_features(i);
	f->description = wgettext("Grass");
	f->setAllTextures("mud.png^grass_side.png");
	f->setTexture(0, "grass_footsteps.png");
	f->setTexture(1, "mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->sound_step = "grass-step";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->farm_ploughable = true;

	i = CONTENT_GRASS_AUTUMN;
	f = &content_features(i);
	f->description = wgettext("Grass");
	f->setAllTextures("mud.png^grass_side_autumn.png");
	f->setTexture(0, "grass_autumn.png");
	f->setTexture(1, "mud.png");
	f->setInventoryTextureCube("grass_autumn.png","mud.png^grass_side_autumn.png","mud.png^grass_side_autumn.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_DEADGRASS)+" 1";
	f->extra_dug_item_rarity = 10;
	f->sound_step = "grass-step";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->farm_ploughable = true;
	lists::add("decrafting",i);

	i = CONTENT_GROWING_GRASS_AUTUMN;
	f = &content_features(i);
	f->description = wgettext("Growing Grass");
	f->setAllTextures("mud.png");
	f->setTexture(0, "grass_growing_autumn.png");
	f->setInventoryTextureCube("grass_autumn.png","mud.png","mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->param2_type = CPT_PLANTGROWTH;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->special_alternate_node = CONTENT_GRASS_AUTUMN;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->farm_ploughable = true;

	i = CONTENT_GRASS_FOOTSTEPS_AUTUMN;
	f = &content_features(i);
	f->description = wgettext("Grass");
	f->setAllTextures("mud.png^grass_side_autumn.png");
	f->setTexture(0, "grass_footsteps_autumn.png");
	f->setTexture(1, "mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->sound_step = "grass-step";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->farm_ploughable = true;

	i = CONTENT_MUDSNOW;
	f = &content_features(i);
	f->description = wgettext("Muddy Snow");
	f->setAllTextures("mud.png^snow_side.png");
	f->setTexture(0, "snow.png");
	f->setTexture(1, "mud.png");
	f->setInventoryTextureCube("snow.png","mud.png^snow_side.png","mud.png^snow_side.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->extra_dug_item = std::string("CraftItem snow_ball 1");
	f->extra_dug_item_rarity = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("decrafting",i);

	i = CONTENT_MUD;
	f = &content_features(i);
	f->description = wgettext("Mud");
	f->setAllTextures("mud.png");
	f->setInventoryTextureCube("mud.png", "mud.png", "mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROCK)+" 1";
	f->extra_dug_item_rarity = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->farm_ploughable = true;
	lists::add("creative",i);

	i = CONTENT_ASH;
	f = &content_features(i);
	f->description = wgettext("Ash Block");
	f->setAllTextures("ash.png");
	f->setInventoryTextureCube("ash.png", "ash.png", "ash.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_ash 9");
	f->type = CMT_DIRT;
	f->hardness = 0.5;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_ASH,CONTENT_ASH);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SAND;
	f = &content_features(i);
	f->description = wgettext("Sand");
	f->setAllTextures("sand.png");
	f->setInventoryTextureCube("sand.png", "sand.png", "sand.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->pressure_type = CST_DROPABLE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->extra_dug_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_SALTPETER)+" 1";
	f->extra_dug_item_rarity = 30;
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_GLASS)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("creative",i);
	lists::add("cooking",i);
	lists::add("decrafting",i);

	i = CONTENT_GRAVEL;
	f = &content_features(i);
	f->description = wgettext("Gravel");
	f->setAllTextures("gravel.png");
	f->setInventoryTextureCube("gravel.png", "gravel.png", "gravel.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->pressure_type = CST_DROPABLE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->extra_dug_item = std::string("CraftItem lump_of_flint 1");
	f->extra_dug_item_rarity = 10;
	f->type = CMT_DIRT;
	f->hardness = 1.75;
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SANDSTONE;
	f = &content_features(i);
	f->description = wgettext("Sand Stone");
	f->setAllTextures("sandstone.png");
	f->setInventoryTextureCube("sandstone.png", "sandstone.png", "sandstone.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setSoftBlockRecipe(CONTENT_SAND,CONTENT_SANDSTONE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SANDSTONE_BRICK;
	f = &content_features(i);
	f->description = wgettext("Sand Stone Bricks");
	f->setAllTextures("sandstone_brick.png");
	f->setInventoryTextureCube("sandstone_brick.png", "sandstone_brick.png", "sandstone_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setBrickRecipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_BRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SANDSTONE_BLOCK;
	f = &content_features(i);
	f->description = wgettext("Sand Stone Blocks");
	f->setAllTextures("sandstone_block.png");
	f->setInventoryTextureCube("sandstone_block.png", "sandstone_block.png", "sandstone_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setBlockRecipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_BLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY;
	f = &content_features(i);
	f->description = wgettext("Clay");
	f->setAllTextures("clay.png");
	f->setInventoryTextureCube("clay.png", "clay.png", "clay.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_TERRACOTTA)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_CLAY,CONTENT_CLAY);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_BLUE;
	f = &content_features(i);
	f->description = wgettext("Blue Clay");
	f->setAllTextures("clay_blue.png");
	f->setInventoryTextureCube("clay_blue.png", "clay_blue.png", "clay_blue.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_blue 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CLAY,CONTENT_CLAY_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_GREEN;
	f = &content_features(i);
	f->description = wgettext("Green Clay");
	f->setAllTextures("clay_green.png");
	f->setInventoryTextureCube("clay_green.png", "clay_green.png", "clay_green.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_green 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CLAY,CONTENT_CLAY_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_ORANGE;
	f = &content_features(i);
	f->description = wgettext("Orange Clay");
	f->setAllTextures("clay_orange.png");
	f->setInventoryTextureCube("clay_orange.png", "clay_orange.png", "clay_orange.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_orange 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CLAY,CONTENT_CLAY_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_PURPLE;
	f = &content_features(i);
	f->description = wgettext("Purple Clay");
	f->setAllTextures("clay_purple.png");
	f->setInventoryTextureCube("clay_purple.png", "clay_purple.png", "clay_purple.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_purple 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CLAY,CONTENT_CLAY_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_RED;
	f = &content_features(i);
	f->description = wgettext("Red Clay");
	f->setAllTextures("clay_red.png");
	f->setInventoryTextureCube("clay_red.png", "clay_red.png", "clay_red.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_red 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_CLAY,CONTENT_CLAY_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_YELLOW;
	f = &content_features(i);
	f->description = wgettext("Yellow Clay");
	f->setAllTextures("clay_yellow.png");
	f->setInventoryTextureCube("clay_yellow.png", "clay_yellow.png", "clay_yellow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_yellow 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CLAY,CONTENT_CLAY_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_BLACK;
	f = &content_features(i);
	f->description = wgettext("Black Clay");
	f->setAllTextures("clay_black.png");
	f->setInventoryTextureCube("clay_black.png", "clay_black.png", "clay_black.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_black 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CLAY,CONTENT_CLAY_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_BRICK;
	f = &content_features(i);
	f->description = wgettext("Brick");
	f->setAllTextures("brick.png");
	f->setTexture(0,"brick_top.png");
	f->setTexture(1,"brick_bottom.png");
	f->setTexture(2,"brick_side.png");
	f->setTexture(3,"brick_side.png");
	f->setInventoryTextureCube("brick_top.png", "brick.png", "brick_side.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_CLAY_BRICK,CONTENT_BRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_TERRACOTTA;
	f = &content_features(i);
	f->description = wgettext("Terracotta");
	f->setAllTextures("terracotta.png");
	f->setInventoryTextureCube("terracotta.png", "terracotta.png", "terracotta.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	lists::add("creative",i);

	i = CONTENT_TERRACOTTA_BRICK;
	f = &content_features(i);
	f->description = wgettext("Terracotta Brick");
	f->setAllTextures("terracotta_brick.png");
	f->setInventoryTextureCube("terracotta_brick.png", "terracotta_brick.png", "terracotta_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBrickRecipe(CONTENT_TERRACOTTA,CONTENT_TERRACOTTA_BRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TERRACOTTA_BLOCK;
	f = &content_features(i);
	f->description = wgettext("Terracotta Block");
	f->setAllTextures("terracotta_block.png");
	f->setInventoryTextureCube("terracotta_block.png", "terracotta_block.png", "terracotta_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBlockRecipe(CONTENT_TERRACOTTA,CONTENT_TERRACOTTA_BLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TERRACOTTA_TILE;
	f = &content_features(i);
	f->description = wgettext("Terracotta Tile");
	f->setAllTextures("terracotta_tile.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->suffocation_per_second = 0;
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"terracotta_tile.png", "terracotta_tile.png", "terracotta_tile.png");
	crafting::setTileRecipe(CONTENT_TERRACOTTA,CONTENT_TERRACOTTA_TILE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS;
	f = &content_features(i);
	f->description = wgettext("Glass");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	lists::add("creative",i);

	i = CONTENT_GLASS_BLUE;
	f = &content_features(i);
	f->description = wgettext("Blue Glass");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_GLASS,CONTENT_GLASS_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_GREEN;
	f = &content_features(i);
	f->description = wgettext("Green Glass");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_GLASS,CONTENT_GLASS_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_ORANGE;
	f = &content_features(i);
	f->description = wgettext("Orange Glass");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_GLASS,CONTENT_GLASS_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PURPLE;
	f = &content_features(i);
	f->description = wgettext("Purple Glass");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_GLASS,CONTENT_GLASS_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_RED;
	f = &content_features(i);
	f->description = wgettext("Red Glass");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_GLASS,CONTENT_GLASS_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_YELLOW;
	f = &content_features(i);
	f->description = wgettext("Yellow Glass");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_GLASS,CONTENT_GLASS_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_BLACK;
	f = &content_features(i);
	f->description = wgettext("Black Glass");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_GLASS,CONTENT_GLASS_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE;
	f = &content_features(i);
	f->description = wgettext("Glass Pane");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol3Recipe(CONTENT_GLASS,CONTENT_GLASS_PANE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_BLUE;
	f = &content_features(i);
	f->description = wgettext("Blue Glass Pane");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol3Recipe(CONTENT_GLASS_BLUE,CONTENT_GLASS_PANE_BLUE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_GREEN;
	f = &content_features(i);
	f->description = wgettext("Green Glass Pane");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol3Recipe(CONTENT_GLASS_GREEN,CONTENT_GLASS_PANE_GREEN);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_ORANGE;
	f = &content_features(i);
	f->description = wgettext("Orange Glass Pane");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol3Recipe(CONTENT_GLASS_ORANGE,CONTENT_GLASS_PANE_ORANGE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_PURPLE;
	f = &content_features(i);
	f->description = wgettext("Purple Glass Pane");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol3Recipe(CONTENT_GLASS_PURPLE,CONTENT_GLASS_PANE_PURPLE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_RED;
	f = &content_features(i);
	f->description = wgettext("Red Glass Pane");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol3Recipe(CONTENT_GLASS_RED,CONTENT_GLASS_PANE_RED);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_YELLOW;
	f = &content_features(i);
	f->description = wgettext("Yellow Glass Pane");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol3Recipe(CONTENT_GLASS_YELLOW,CONTENT_GLASS_PANE_YELLOW);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_BLACK;
	f = &content_features(i);
	f->description = wgettext("Black Glass Pane");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol3Recipe(CONTENT_GLASS_BLACK,CONTENT_GLASS_PANE_BLACK);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASSLIGHT;
	f = &content_features(i);
	f->description = wgettext("Glass Light");
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
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->light_source = LIGHT_MAX-1;
	crafting::setSurroundRecipe(CONTENT_GLASS,CONTENT_CRAFTITEM_FIREFLY,CONTENT_GLASSLIGHT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD;
	f = &content_features(i);
	f->description = wgettext("Wood");
	f->setAllTextures("wood.png");
	f->setInventoryTextureCube("wood.png", "wood.png", "wood.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1To4Recipe(CONTENT_TREE,CONTENT_WOOD);
	crafting::set1To4Recipe(CONTENT_APPLE_TREE,CONTENT_WOOD);
	//crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_WOOD);
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,
			CONTENT_IGNORE,			CONTENT_IGNORE,	CONTENT_IGNORE,
			CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK
		};
		crafting::setRecipe(r,CONTENT_WOOD,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_JUNGLEWOOD;
	f = &content_features(i);
	f->description = wgettext("Jungle Wood");
	f->setAllTextures("junglewood.png");
	f->setInventoryTextureCube("junglewood.png", "junglewood.png", "junglewood.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1To4Recipe(CONTENT_JUNGLETREE,CONTENT_JUNGLEWOOD);
	//crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_JUNGLEWOOD);
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,
			CONTENT_IGNORE,			CONTENT_IGNORE,	CONTENT_IGNORE,
			CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK
		};
		crafting::setRecipe(r,CONTENT_JUNGLEWOOD,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_PINE;
	f = &content_features(i);
	f->description = wgettext("Pine");
	f->setAllTextures("pine.png");
	f->setInventoryTextureCube("pine.png", "pine.png", "pine.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->type = CMT_WOOD;
	f->hardness = 0.5;
	crafting::set1To4Recipe(CONTENT_CONIFER_TREE,CONTENT_WOOD_PINE);
	//crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_PINE_PLANK,CONTENT_WOOD_PINE);
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,
			CONTENT_IGNORE,			CONTENT_IGNORE,	CONTENT_IGNORE,
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK
		};
		crafting::setRecipe(r,CONTENT_WOOD_PINE,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SPONGE;
	f = &content_features(i);
	f->description = wgettext("Sponge");
	f->setAllTextures("sponge.png");
	f->setInventoryTextureCube("sponge.png", "sponge.png", "sponge.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	lists::add("creative",i);

	i = CONTENT_SPONGE_FULL;
	f = &content_features(i);
	f->description = wgettext("Waterlogged Sponge");
	f->setAllTextures("sponge_full.png");
	f->setInventoryTextureCube("sponge_full.png", "sponge_full.png", "sponge_full.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->ondig_special_drop = CONTENT_SPONGE;
	f->ondig_special_tool = TT_BUCKET;
	f->ondig_special_tool_append = std::string("_water");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_HAY;
	f = &content_features(i);
	f->description = wgettext("Hay Bale");
	f->setAllTextures("hay_bale.png");
	f->setInventoryTextureCube("hay_bale.png", "hay_bale.png", "hay_bale.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1;
	f->fuel_time = 20;
	f->type = CMT_PLANT;
	f->hardness = 0.6;
	f->pressure_type = CST_CRUSHABLE;
	crafting::setSoftBlockRecipe(CONTENT_DEADGRASS,CONTENT_HAY);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_APPLE_PIE_RAW;
	f = &content_features(i);
	f->description = wgettext("Raw Apple Pie");
	f->setTexture(0, "apple_pie_raw.png");
	f->setTexture(1, "apple_pie_raw.png");
	f->setTexture(2, "apple_pie_raw.png");
	f->setTexture(3, "apple_pie_raw.png");
	f->setTexture(4, "apple_pie_raw.png");
	f->setTexture(5, "apple_pie_raw.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_APPLE_PIE)+"  1";
	content_nodebox_pie(f);
	f->setInventoryTextureNodeBox(i, "apple_pie_raw.png", "apple_pie_raw.png", "apple_pie_raw.png");
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_APPLE,CONTENT_CRAFTITEM_DOUGH,CONTENT_APPLE_PIE_RAW);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_APPLE_PIE;
	f = &content_features(i);
	f->description = wgettext("Apple Pie");
	f->setTexture(0, "apple_pie.png");
	f->setTexture(1, "apple_pie.png");
	f->setTexture(2, "apple_pie.png");
	f->setTexture(3, "apple_pie.png");
	f->setTexture(4, "apple_pie.png");
	f->setTexture(5, "apple_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_nodebox_pie(f);
	f->setInventoryTextureNodeBox(i, "apple_pie.png", "apple_pie.png", "apple_pie.png");
	f->dug_item = std::string("CraftItem apple_pie_slice 1");
	f->ondig_replace_node=CONTENT_APPLE_PIE_3;
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_APPLE_PIE_3;
	f = &content_features(i);
	f->setTexture(0, "apple_pie.png");
	f->setTexture(1, "apple_pie.png");
	f->setTexture(2, "apple_pie.png^[transformR90");
	f->setTexture(3, "apple_pie.png");
	f->setTexture(4, "apple_pie.png");
	f->setTexture(5, "apple_pie.png^[transformR270");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_nodebox_pie_3(f);
	f->dug_item = std::string("CraftItem apple_pie_slice 1");
	f->ondig_replace_node=CONTENT_APPLE_PIE_2;
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_APPLE_PIE_2;
	f = &content_features(i);
	f->setTexture(0, "apple_pie.png");
	f->setTexture(1, "apple_pie.png");
	f->setTexture(2, "apple_pie.png^[transformFY");
	f->setTexture(3, "apple_pie.png");
	f->setTexture(4, "apple_pie.png");
	f->setTexture(5, "apple_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_nodebox_pie_2(f);
	f->dug_item = std::string("CraftItem apple_pie_slice 1");
	f->ondig_replace_node=CONTENT_APPLE_PIE_1;
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_APPLE_PIE_1;
	f = &content_features(i);
	f->setTexture(0, "apple_pie.png");
	f->setTexture(1, "apple_pie.png");
	f->setTexture(2, "apple_pie.png^[transformFY");
	f->setTexture(3, "apple_pie.png");
	f->setTexture(4, "apple_pie.png^[transformFY");
	f->setTexture(5, "apple_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_nodebox_pie_1(f);
	f->dug_item = std::string("CraftItem apple_pie_slice 1");
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_PUMPKIN_PIE_RAW;
	f = &content_features(i);
	f->description = wgettext("Raw Pumpkin Pie");
	f->setTexture(0, "pumpkin_pie_raw.png");
	f->setTexture(1, "pumpkin_pie_raw.png");
	f->setTexture(2, "pumpkin_pie_raw.png");
	f->setTexture(3, "pumpkin_pie_raw.png");
	f->setTexture(4, "pumpkin_pie_raw.png");
	f->setTexture(5, "pumpkin_pie_raw.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_PUMPKIN_PIE)+"  1";
	content_nodebox_pie(f);
	f->setInventoryTextureNodeBox(i, "pumpkin_pie_raw.png", "pumpkin_pie_raw.png", "pumpkin_pie_raw.png");
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_PUMPKINSLICE,CONTENT_CRAFTITEM_DOUGH,CONTENT_PUMPKIN_PIE_RAW);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_PUMPKIN_PIE;
	f = &content_features(i);
	f->description = wgettext("Pumpkin Pie");
	f->setTexture(0, "pumpkin_pie.png");
	f->setTexture(1, "pumpkin_pie.png");
	f->setTexture(2, "pumpkin_pie.png");
	f->setTexture(3, "pumpkin_pie.png");
	f->setTexture(4, "pumpkin_pie.png");
	f->setTexture(5, "pumpkin_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_nodebox_pie(f);
	f->setInventoryTextureNodeBox(i, "pumpkin_pie.png", "pumpkin_pie.png", "pumpkin_pie.png");
	f->dug_item = std::string("CraftItem pumpkin_pie_slice 1");
	f->ondig_replace_node=CONTENT_PUMPKIN_PIE_3;
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_PUMPKIN_PIE_3;
	f = &content_features(i);
	f->setTexture(0, "pumpkin_pie.png");
	f->setTexture(1, "pumpkin_pie.png");
	f->setTexture(2, "pumpkin_pie.png^[transformR90");
	f->setTexture(3, "pumpkin_pie.png");
	f->setTexture(4, "pumpkin_pie.png");
	f->setTexture(5, "pumpkin_pie.png^[transformR270");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_nodebox_pie_3(f);
	f->dug_item = std::string("CraftItem pumpkin_pie_slice 1");
	f->ondig_replace_node=CONTENT_PUMPKIN_PIE_2;
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_PUMPKIN_PIE_2;
	f = &content_features(i);
	f->setTexture(0, "pumpkin_pie.png");
	f->setTexture(1, "pumpkin_pie.png");
	f->setTexture(2, "pumpkin_pie.png^[transformFY");
	f->setTexture(3, "pumpkin_pie.png");
	f->setTexture(4, "pumpkin_pie.png");
	f->setTexture(5, "pumpkin_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_nodebox_pie_2(f);
	f->dug_item = std::string("CraftItem pumpkin_pie_slice 1");
	f->ondig_replace_node=CONTENT_PUMPKIN_PIE_1;
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_PUMPKIN_PIE_1;
	f = &content_features(i);
	f->setTexture(0, "pumpkin_pie.png");
	f->setTexture(1, "pumpkin_pie.png");
	f->setTexture(2, "pumpkin_pie.png^[transformFY");
	f->setTexture(3, "pumpkin_pie.png");
	f->setTexture(4, "pumpkin_pie.png^[transformFY");
	f->setTexture(5, "pumpkin_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_nodebox_pie_1(f);
	f->dug_item = std::string("CraftItem pumpkin_pie_slice 1");
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_SNOW_BLOCK;
	f = &content_features(i);
	f->description = wgettext("Snow");
	f->setAllTextures("snow.png");
	f->setInventoryTextureCube("snow.png", "snow.png", "snow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem snow_ball 9");
	f->type = CMT_DIRT;
	f->hardness = 0.3;
	f->warmth_per_second = 2;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_SNOW_BALL,CONTENT_SNOW_BLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SNOWMAN;
	f = &content_features(i);
	f->description = wgettext("Snowman");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->solidness = 0;
	f->setAllTextures("snowman.png");
	f->setTexture(0,"snowman_top.png");
	f->setTexture(1,"snowman_top.png");
	f->setTexture(5,"snowman_front.png");
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem snow_ball 18");
	f->extra_dug_item = std::string("CraftItem Stick 3");
	f->extra_dug_item_rarity = 1;
	f->rotate_tile_with_nodebox = true;
	f->type = CMT_DIRT;
	f->pressure_type = CST_CRUSHABLE;
	f->hardness = 0.3;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,
		-0.5*BS,
		-0.3125*BS,
		0.3125*BS,
		0.125*BS,
		0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.25*BS,
		0.125*BS,
		-0.25*BS,
		0.25*BS,
		0.5*BS,
		0.25*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.0625*BS,
		0.3125*BS,
		-0.375*BS,
		0.*BS,
		0.375*BS,
		-0.25*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.0625*BS,
		-0.0625*BS,
		0.5*BS,
		0.*BS,
		0.*BS
	));
	f->setInventoryTextureNodeBox(i,"snow.png", "snow.png", "snow.png");
	{
		content_t r[9] = {
			CONTENT_IGNORE,			CONTENT_IGNORE,		CONTENT_IGNORE,
			CONTENT_IGNORE,			CONTENT_SNOW_BLOCK,	CONTENT_CRAFTITEM_CARROT,
			CONTENT_CRAFTITEM_STICK,	CONTENT_SNOW_BLOCK,	CONTENT_CRAFTITEM_STICK
		};
		crafting::setRecipe(r,CONTENT_SNOWMAN,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SNOW;
	f = &content_features(i);
	f->description = wgettext("Snow");
	f->setAllTextures("snow.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->buildable_to = true;
	f->material_pointable = false;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("CraftItem snow_ball 1");
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"snow.png", "snow.png", "snow.png");
	f->type = CMT_DIRT;
	f->hardness = 0.3;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->warmth_per_second = 2;
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_COTTON;
	f = &content_features(i);
	f->description = wgettext("Cotton");
	f->setAllTextures("cotton.png");
	f->setInventoryTextureCube("cotton.png", "cotton.png", "cotton.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_DROPABLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_BLUE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_GREEN,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_ORANGE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_PURPLE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_RED,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_YELLOW,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_BLACK,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_BLUE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_GREEN,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_ORANGE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_PURPLE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_RED,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_YELLOW,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_BLACK,CONTENT_COTTON);
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_STRING,CONTENT_COTTON);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_BLUE;
	f = &content_features(i);
	f->description = wgettext("Blue Cotton");
	f->setAllTextures("cotton_blue.png");
	f->setInventoryTextureCube("cotton_blue.png", "cotton_blue.png", "cotton_blue.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_DROPABLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_COTTON,CONTENT_COTTON_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_GREEN;
	f = &content_features(i);
	f->description = wgettext("Green Cotton");
	f->setAllTextures("cotton_green.png");
	f->setInventoryTextureCube("cotton_green.png", "cotton_green.png", "cotton_green.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_DROPABLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_COTTON,CONTENT_COTTON_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_ORANGE;
	f = &content_features(i);
	f->description = wgettext("Orange Cotton");
	f->setAllTextures("cotton_orange.png");
	f->setInventoryTextureCube("cotton_orange.png", "cotton_orange.png", "cotton_orange.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_DROPABLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_COTTON,CONTENT_COTTON_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_PURPLE;
	f = &content_features(i);
	f->description = wgettext("Purple Cotton");
	f->setAllTextures("cotton_purple.png");
	f->setInventoryTextureCube("cotton_purple.png", "cotton_purple.png", "cotton_purple.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_DROPABLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_COTTON,CONTENT_COTTON_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_RED;
	f = &content_features(i);
	f->description = wgettext("Red Cotton");
	f->setAllTextures("cotton_red.png");
	f->setInventoryTextureCube("cotton_red.png", "cotton_red.png", "cotton_red.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_DROPABLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_COTTON,CONTENT_COTTON_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_YELLOW;
	f = &content_features(i);
	f->description = wgettext("Yellow Cotton");
	f->setAllTextures("cotton_yellow.png");
	f->setInventoryTextureCube("cotton_yellow.png", "cotton_yellow.png", "cotton_yellow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_DROPABLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_COTTON,CONTENT_COTTON_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_BLACK;
	f = &content_features(i);
	f->description = wgettext("Black Cotton");
	f->setAllTextures("cotton_black.png");
	f->setInventoryTextureCube("cotton_black.png", "cotton_black.png", "cotton_black.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_DROPABLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_COTTON,CONTENT_COTTON_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET;
	f = &content_features(i);
	f->description = wgettext("Carpet");
	f->setAllTextures("cotton.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton.png", "cotton.png", "cotton.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_COTTON,CONTENT_CARPET);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_BLUE;
	f = &content_features(i);
	f->description = wgettext("Blue Carpet");
	f->setAllTextures("cotton_blue.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_blue.png", "cotton_blue.png", "cotton_blue.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_COTTON_BLUE,CONTENT_CARPET_BLUE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CARPET,CONTENT_CARPET_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_GREEN;
	f = &content_features(i);
	f->description = wgettext("Green Carpet");
	f->setAllTextures("cotton_green.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_green.png", "cotton_green.png", "cotton_green.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_COTTON_GREEN,CONTENT_CARPET_GREEN);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CARPET,CONTENT_CARPET_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_ORANGE;
	f = &content_features(i);
	f->description = wgettext("Orange Carpet");
	f->setAllTextures("cotton_orange.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_orange.png", "cotton_orange.png", "cotton_orange.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_COTTON_ORANGE,CONTENT_CARPET_ORANGE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CARPET,CONTENT_CARPET_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_PURPLE;
	f = &content_features(i);
	f->description = wgettext("Purple Carpet");
	f->setAllTextures("cotton_purple.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_purple.png", "cotton_purple.png", "cotton_purple.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_COTTON_PURPLE,CONTENT_CARPET_PURPLE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CARPET,CONTENT_CARPET_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_RED;
	f = &content_features(i);
	f->description = wgettext("Red Carpet");
	f->setAllTextures("cotton_red.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_red.png", "cotton_red.png", "cotton_red.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_COTTON_RED,CONTENT_CARPET_RED);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_CARPET,CONTENT_CARPET_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_YELLOW;
	f = &content_features(i);
	f->description = wgettext("Yellow Carpet");
	f->setAllTextures("cotton_yellow.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_yellow.png", "cotton_yellow.png", "cotton_yellow.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	f->pressure_type = CST_CRUSHABLE;
	crafting::setTileRecipe(CONTENT_COTTON_YELLOW,CONTENT_CARPET_YELLOW);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CARPET,CONTENT_CARPET_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_BLACK;
	f = &content_features(i);
	f->description = wgettext("Black Carpet");
	f->setAllTextures("cotton_black.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_black.png", "cotton_black.png", "cotton_black.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_COTTON_BLACK,CONTENT_CARPET_BLACK);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CARPET,CONTENT_CARPET_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_AIR;
	f = &content_features(i);
	f->description = wgettext("Air");
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
	f->pressure_type = CST_CRUSHED;
	f->suffocation_per_second = 0;

	i = CONTENT_VACUUM;
	f = &content_features(i);
	f->description = wgettext("Vacuum");
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
	f->pressure_type = CST_CRUSHED;
	f->suffocation_per_second = 15;
	f->pressure_per_second = 10;

	i = CONTENT_WATER;
	f = &content_features(i);
	f->description = wgettext("Water");
	f->setAllTextures("water.png");
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
	f->pressure_type = CST_CRUSHABLE;
	f->liquid_type = LIQUID_FLOWING;
	f->liquid_alternative_flowing = CONTENT_WATER;
	f->liquid_alternative_source = CONTENT_WATERSOURCE;
	f->liquid_viscosity = WATER_VISC;
#ifndef SERVER
	if(!opaque_water)
		f->vertex_alpha = WATER_ALPHA;
	f->setAllTextureFlags(0);
	f->setAllTextureTypes(MATERIAL_ALPHA_VERTEX);
	f->post_effect_color = video::SColor(64, 100, 100, 200);
#endif
	f->sound_ambient = "env-water";
	f->suffocation_per_second = 2;

	i = CONTENT_WATERSOURCE;
	f = &content_features(i);
	f->description = wgettext("Water");
	f->setAllTextures("water.png");
	f->setInventoryTextureCube("water.png", "water.png", "water.png");
	f->solidness = 0; // drawn separately, makes no faces
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID_SOURCE;
	f->light_propagates = true;
	f->walkable = false;
	f->pointable = false;
	f->buildable_to = true;
	f->liquid_type = LIQUID_SOURCE;
	f->dug_item = std::string("_water"); // won't resolve to an item, but will work when added to a bucket tool name
	f->liquid_alternative_flowing = CONTENT_WATER;
	f->liquid_alternative_source = CONTENT_WATERSOURCE;
	f->liquid_viscosity = WATER_VISC;
#ifndef SERVER
	if(!opaque_water)
		f->vertex_alpha = WATER_ALPHA;
	f->setAllTextureFlags(0);
	f->setAllTextureTypes(MATERIAL_ALPHA_VERTEX);
	f->post_effect_color = video::SColor(64, 100, 100, 200);
#endif
	f->type = CMT_LIQUID;
	f->hardness = 0.5;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 2;

	i = CONTENT_LAVA;
	f = &content_features(i);
	f->description = wgettext("Lava");
	f->setAllTextures("lava.png");
	f->setInventoryTextureCube("lava.png", "lava.png", "lava.png");
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
	f->fuel_time = 100;
	f->pressure_type = CST_CRUSHABLE;
	f->liquid_type = LIQUID_FLOWING;
	f->liquid_alternative_flowing = CONTENT_LAVA;
	f->liquid_alternative_source = CONTENT_LAVASOURCE;
	f->liquid_viscosity = LAVA_VISC;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif

	i = CONTENT_LAVASOURCE;
	f = &content_features(i);
	f->description = wgettext("Lava");
	f->setAllTextures("lava.png");
	f->setInventoryTextureCube("lava.png", "lava.png", "lava.png");
	f->solidness = 0; // drawn separately, makes no faces
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
	f->fuel_time = 100;
	f->liquid_type = LIQUID_SOURCE;
	f->dug_item = std::string("_lava"); // won't resolve to an item, but will work when added to a bucket tool name
	f->liquid_alternative_flowing = CONTENT_LAVA;
	f->liquid_alternative_source = CONTENT_LAVASOURCE;
	f->liquid_viscosity = LAVA_VISC;
	f->damage_per_second = 4*2;
	f->sound_ambient = "env-lava";
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif
	f->type = CMT_LIQUID;
	f->hardness = 0.5;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_ROUGHSTONE;
	f = &content_features(i);
	f->description = wgettext("Rough Stone");
	f->setAllTextures("roughstone.png");
	f->setInventoryTextureCube("roughstone.png", "roughstone.png", "roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_STONE)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::setBlockRecipe(CONTENT_COBBLE,CONTENT_ROUGHSTONE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_COBBLE;
	f = &content_features(i);
	f->description = wgettext("Cobble Stone");
	f->setAllTextures("cobble.png");
	f->setInventoryTextureCube("cobble.png", "cobble.png", "cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::set5Recipe(CONTENT_ROUGHSTONE,CONTENT_COBBLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MOSSYCOBBLE;
	f = &content_features(i);
	f->description = wgettext("Mossy Cobble Stone");
	f->setAllTextures("mossycobble.png");
	f->setInventoryTextureCube("mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.8;
	lists::add("creative",i);

	i = CONTENT_STEEL;
	f = &content_features(i);
	f->description = wgettext("Steel Block");
	f->setAllTextures("steel_block.png");
	f->setInventoryTextureCube("steel_block.png", "steel_block.png", "steel_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->destructive_mob_safe = true;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_STEEL);
	crafting::setUncraftHardBlockRecipe(CONTENT_STEEL,CONTENT_CRAFTITEM_STEEL_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MITHRIL_BLOCK;
	f = &content_features(i);
	f->description = wgettext("Mithril Block");
	f->setAllTextures("mithril_block.png");
	f->setInventoryTextureCube("mithril_block.png", "mithril_block.png", "mithril_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 10.0;
	f->destructive_mob_safe = true;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_MITHRIL_UNBOUND,CONTENT_MITHRIL_BLOCK);
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_MITHRIL,CONTENT_MITHRIL_BLOCK);
	crafting::setUncraftHardBlockRecipe(CONTENT_MITHRIL_BLOCK,CONTENT_CRAFTITEM_MITHRIL_UNBOUND);
	lists::add("craftguide",i);
	lists::add("creative",i);

	content_nodedef_knob(CONTENT_STONE_KNOB, CONTENT_STONE, CMT_STONE, "stone.png", wgettext("Stone Knob"));
	content_nodedef_knob(CONTENT_ROUGHSTONE_KNOB, CONTENT_ROUGHSTONE, CMT_STONE, "roughstone.png", wgettext("Rough Stone Knob"));
	content_nodedef_knob(CONTENT_SANDSTONE_KNOB, CONTENT_SANDSTONE, CMT_STONE, "sandstone.png", wgettext("Sandstone Knob"));
	content_nodedef_knob(CONTENT_WOOD_KNOB, CONTENT_CRAFTITEM_WOOD_PLANK, CMT_WOOD, "wood.png", wgettext("Wooden Knob"));
	content_nodedef_knob(CONTENT_JUNGLEWOOD_KNOB, CONTENT_CRAFTITEM_JUNGLE_PLANK, CMT_WOOD, "junglewood.png", wgettext("Junglewood Knob"));
	content_nodedef_knob(CONTENT_PINE_KNOB, CONTENT_CRAFTITEM_PINE_PLANK, CMT_WOOD, "pine.png", wgettext("Pine Knob"));

	i = CONTENT_COPPER;
	f = &content_features(i);
	f->description = wgettext("Copper Block");
	f->setAllTextures("copper_block.png");
	f->setInventoryTextureCube("copper_block.png", "copper_block.png", "copper_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->destructive_mob_safe = true;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_COPPER);
	crafting::setUncraftHardBlockRecipe(CONTENT_COPPER,CONTENT_CRAFTITEM_COPPER_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GOLD;
	f = &content_features(i);
	f->description = wgettext("Gold Block");
	f->setAllTextures("gold_block.png");
	f->setInventoryTextureCube("gold_block.png", "gold_block.png", "gold_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->destructive_mob_safe = true;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_GOLD_INGOT,CONTENT_GOLD);
	crafting::setUncraftHardBlockRecipe(CONTENT_GOLD,CONTENT_CRAFTITEM_GOLD_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SILVER;
	f = &content_features(i);
	f->description = wgettext("Silver Block");
	f->setAllTextures("silver_block.png");
	f->setInventoryTextureCube("silver_block.png", "silver_block.png", "silver_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->destructive_mob_safe = true;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_SILVER_INGOT,CONTENT_SILVER);
	crafting::setUncraftHardBlockRecipe(CONTENT_SILVER,CONTENT_CRAFTITEM_SILVER_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TIN;
	f = &content_features(i);
	f->description = wgettext("Tin Block");
	f->setAllTextures("tin_block.png");
	f->setInventoryTextureCube("tin_block.png", "tin_block.png", "tin_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->destructive_mob_safe = true;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_TIN_INGOT,CONTENT_TIN);
	crafting::setUncraftHardBlockRecipe(CONTENT_TIN,CONTENT_CRAFTITEM_TIN_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_QUARTZ;
	f = &content_features(i);
	f->description = wgettext("Quartz Block");
	f->setAllTextures("quartz_block.png");
	f->setInventoryTextureCube("quartz_block.png", "quartz_block.png", "quartz_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->destructive_mob_safe = true;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_QUARTZ,CONTENT_QUARTZ);
	crafting::setUncraftHardBlockRecipe(CONTENT_QUARTZ,CONTENT_CRAFTITEM_QUARTZ);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONE_TILE;
	f = &content_features(i);
	f->description = wgettext("Stone Tiles");
	f->setAllTextures("stone_tile.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"stone_tile.png", "stone_tile.png", "stone_tile.png");
	f->fuel_time = 5;
	f->type = CMT_STONE;
	f->hardness = 0.5;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_STONE,CONTENT_STONE_TILE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_TILE;
	f = &content_features(i);
	f->description = wgettext("Wood Tiles");
	f->setAllTextures("wood_tile.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"wood_tile.png", "wood_tile.png", "wood_tile.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_WOOD;
	f->hardness = 0.5;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setTileRecipe(CONTENT_WOOD,CONTENT_WOOD_TILE);
	lists::add("craftguide",i);
	lists::add("creative",i);

}
