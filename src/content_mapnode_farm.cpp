/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mapnode_farm.cpp
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
************************************************************************/

#include "content_mapnode.h"
#include "content_nodebox.h"
#include "content_list.h"
#include "content_craft.h"
#include "content_craftitem.h"
#include "content_nodemeta.h"
#include "settings.h"
#include "intl.h"

void content_mapnode_farm(bool repeat)
{
	content_t i;
	ContentFeatures *f = NULL;

	i = CONTENT_FARM_DIRT;
	f = &content_features(i);
	f->description = wgettext("Farm Dirt");
	f->setAllTextures("dirt.png");
	f->setInventoryTextureCube("dirt.png","dirt.png","dirt.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("decrafting",i);

	i = CONTENT_FERTILIZER;
	f = &content_features(i);
	f->description = wgettext("Fertilizer");
	f->setAllTextures("fertilizer.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_FERTILIZER)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"fertilizer.png", "fertilizer.png", "fertilizer.png");
	f->type = CMT_DIRT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_TRELLIS;
	f = &content_features(i);
	f->description = wgettext("Trellis");
	f->setAllTextures("trellis.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set5Recipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_TRELLIS);
	crafting::set5Recipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_TRELLIS);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_WHEAT;
	f = &content_features(i);
	f->description = wgettext("Wheat Seeds");
	f->setAllTextures("farm_seeds_wheat.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_FARM_WHEAT;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	lists::add("creative",i);

	i = CONTENT_SEEDS_MELON;
	f = &content_features(i);
	f->description = wgettext("Melon Seeds");
	f->setAllTextures("farm_seeds_melon.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_FARM_MELON;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_MELONSLICE,CONTENT_SEEDS_MELON);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_PUMPKIN;
	f = &content_features(i);
	f->description = wgettext("Pumpkin Seeds");
	f->setAllTextures("farm_seeds_pumpkin.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_FARM_PUMPKIN;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_PUMPKINSLICE,CONTENT_SEEDS_PUMPKIN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_POTATO;
	f = &content_features(i);
	f->description = wgettext("Potato Seeds");
	f->setAllTextures("farm_seeds_potato.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_FARM_POTATO;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_POTATO,CONTENT_SEEDS_POTATO);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_CARROT;
	f = &content_features(i);
	f->description = wgettext("Carrot Seeds");
	f->setAllTextures("farm_seeds_carrot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_FARM_CARROT;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_CARROT,CONTENT_SEEDS_CARROT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_BEETROOT;
	f = &content_features(i);
	f->description = wgettext("Beetroot Seeds");
	f->setAllTextures("farm_seeds_beetroot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_FARM_BEETROOT;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_BEETROOT,CONTENT_SEEDS_BEETROOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_GRAPE;
	f = &content_features(i);
	f->description = wgettext("Grape Seeds");
	f->setAllTextures("farm_seeds_grape.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_FARM_GRAPEVINE;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_GRAPE,CONTENT_SEEDS_GRAPE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_COTTON;
	f = &content_features(i);
	f->description = wgettext("Cotton Seeds");
	f->setAllTextures("farm_seeds_cotton.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_FARM_COTTON;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	lists::add("creative",i);

	i = CONTENT_FARM_WHEAT;
	f = &content_features(i);
	f->description = wgettext("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_PLANTGROWTH;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->plantgrowth_max_height = 2; // maximum growth height;
	f->solidness = 0; // drawn separately, makes no faces
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_WHEAT;
	f->plantgrowth_large_dug_node = CONTENT_CRAFTITEM_WHEAT;
	f->plantgrowth_large_gives_small = true;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_FARM_MELON;
	f = &content_features(i);
	f->description = wgettext("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->setInventoryTextureCube("farm_melon.png","farm_melon.png","farm_melon.png");
	f->draw_type = CDT_MELONLIKE;
	f->param2_type = CPT_PLANTGROWTH;
	f->flammable = 1;
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_MELON;
	f->plantgrowth_large_dug_node = CONTENT_FARM_MELON;
	f->plantgrowth_large_count = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_PUMPKIN;
	f = &content_features(i);
	f->description = wgettext("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->setInventoryTextureCube("farm_pumpkin_top.png","farm_pumpkin.png","farm_pumpkin.png");
	f->draw_type = CDT_MELONLIKE;
	f->param2_type = CPT_PLANTGROWTH;
	f->flammable = 1;
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_PUMPKIN;
	f->plantgrowth_large_dug_node = CONTENT_FARM_PUMPKIN;
	f->plantgrowth_large_count = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	lists::add("creative", i);
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_PUMPKIN_JACK;
	f = &content_features(i);
	if (g_settings->getBool("enable_supernatural")) {
		f->description = wgettext("Jack' O Lantern");
		f->setAllTextures("farm_pumpkin.png");
		f->setTexture(0,"farm_pumpkin_top.png");
		f->setTexture(1,"farm_pumpkin_top.png");
		f->setTexture(5, "farm_pumpkin_jack.png"); // Z-
		f->setInventoryTextureCube("farm_pumpkin_top.png","farm_pumpkin_jack.png","farm_pumpkin.png");
		f->draw_type = CDT_NODEBOX;
		f->solidness = 0; // drawn separately, makes no faces
		content_nodebox_jackolantern(f);
		f->setInventoryTextureNodeBox(i,"farm_pumpkin_top.png","farm_pumpkin_jack.png","farm_pumpkin.png");
	}else{
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
	}
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->light_source = LIGHT_MAX-1;
	crafting::set1Any2Recipe(CONTENT_TORCH,CONTENT_FARM_PUMPKIN,CONTENT_FARM_PUMPKIN_JACK);
	lists::add("creative", i);
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_POTATO;
	f = &content_features(i);
	f->description = wgettext("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_PLANTGROWTH;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_POTATO;
	f->plantgrowth_large_dug_node = CONTENT_CRAFTITEM_POTATO;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_FARM_CARROT;
	f = &content_features(i);
	f->description = wgettext("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_PLANTGROWTH;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_CARROT;
	f->plantgrowth_large_dug_node = CONTENT_CRAFTITEM_CARROT;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_FARM_BEETROOT;
	f = &content_features(i);
	f->description = wgettext("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_PLANTGROWTH;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_BEETROOT;
	f->plantgrowth_large_dug_node = CONTENT_CRAFTITEM_BEETROOT;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_FARM_GRAPEVINE;
	f = &content_features(i);
	f->description = wgettext("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_PLANTGROWTH;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_GRAPE;
	f->plantgrowth_large_dug_node = CONTENT_CRAFTITEM_GRAPE;
	f->plantgrowth_trellis_node = CONTENT_FARM_TRELLIS_GRAPE;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_FARM_COTTON;
	f = &content_features(i);
	f->description = wgettext("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_PLANTGROWTH;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->plantgrowth_max_height = 2; // maximum growth height;
	f->solidness = 0; // drawn separately, makes no faces
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_COTTON;
	f->plantgrowth_large_dug_node = CONTENT_CRAFTITEM_STRING;
	f->plantgrowth_large_count = 6;
	f->plantgrowth_large_gives_small = true;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_FARM_TRELLIS_GRAPE;
	f = &content_features(i);
	f->description = wgettext("Trellis Grape");
	f->setAllTextures("farm_grapevine.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_PLANTGROWTH;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->plantgrowth_small_dug_node = CONTENT_SEEDS_GRAPE;
	f->plantgrowth_large_dug_node = CONTENT_CRAFTITEM_GRAPE;
	f->plantgrowth_on_trellis = true;
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->plantgrowth_max_height = 5;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_DEAD_VINE;
	f = &content_features(i);
	f->description = wgettext("Dead Vine");
	f->setAllTextures("dead_vine.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ") + itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.3;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;

	i = CONTENT_TRELLIS_DEAD_VINE;
	f = &content_features(i);
	f->description = wgettext("Dead Vine");
	f->setAllTextures("dead_vine.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->plantgrowth_on_trellis = true;
	f->dug_item = std::string("MaterialItem2 ") + itos(CONTENT_DEAD_VINE)+" 1";
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
}
