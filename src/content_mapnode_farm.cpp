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
#include "content_nodemeta.h"

void content_mapnode_farm(bool repeat)
{
	content_t i;
	ContentFeatures *f = NULL;

	i = CONTENT_FARM_DIRT;
	f = &content_features(i);
	f->description = std::string("Farm Dirt");
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
	f->description = std::string("Fertilizer");
	f->setAllTextures("fertilizer.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"fertilizer.png", "fertilizer.png", "fertilizer.png");
	f->type = CMT_DIRT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_MUSH,CONTENT_CRAFTITEM_ASH,CONTENT_FERTILIZER);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TRELLIS;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->air_equivalent = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	crafting::set5Recipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_TRELLIS);
	crafting::set5Recipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_TRELLIS);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_WHEAT;
	f = &content_features(i);
	f->description = std::string("Wheat Seeds");
	f->setAllTextures("farm_seeds_wheat.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	lists::add("creative",i);

	i = CONTENT_SEEDS_MELON;
	f = &content_features(i);
	f->description = std::string("Melon Seeds");
	f->setAllTextures("farm_seeds_melon.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_MELONSLICE,CONTENT_SEEDS_MELON);
	lists::add("creative",i);

	i = CONTENT_SEEDS_PUMPKIN;
	f = &content_features(i);
	f->description = std::string("Pumpkin Seeds");
	f->setAllTextures("farm_seeds_pumpkin.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_PUMPKINSLICE,CONTENT_SEEDS_PUMPKIN);
	lists::add("creative",i);

	i = CONTENT_SEEDS_POTATO;
	f = &content_features(i);
	f->description = std::string("Potato Seeds");
	f->setAllTextures("farm_seeds_potato.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	lists::add("creative",i);

	i = CONTENT_SEEDS_CARROT;
	f = &content_features(i);
	f->description = std::string("Carrot Seeds");
	f->setAllTextures("farm_seeds_carrot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	lists::add("creative",i);

	i = CONTENT_SEEDS_BEETROOT;
	f = &content_features(i);
	f->description = std::string("Beetroot Seeds");
	f->setAllTextures("farm_seeds_beetroot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	lists::add("creative",i);

	i = CONTENT_SEEDS_GRAPE;
	f = &content_features(i);
	f->description = std::string("Grape Seeds");
	f->setAllTextures("farm_seeds_grape.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_GRAPE,CONTENT_SEEDS_GRAPE);
	lists::add("creative",i);

	i = CONTENT_SEEDS_COTTON;
	f = &content_features(i);
	f->description = std::string("Cotton Seeds");
	f->setAllTextures("farm_seeds_cotton.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;
	lists::add("creative",i);

	i = CONTENT_FARM_WHEAT_1;
	f = &content_features(i);
	f->description = std::string("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_WHEAT)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_WHEAT_2;
	f = &content_features(i);
	f->description = std::string("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_WHEAT)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_WHEAT_3;
	f = &content_features(i);
	f->description = std::string("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_WHEAT)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_wheat 1");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_WHEAT;
	f = &content_features(i);
	f->description = std::string("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_WHEAT)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_wheat 2");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_MELON_1;
	f = &content_features(i);
	f->description = std::string("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_MELON)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 3;
	content_nodebox_plant_1(f);
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_MELON_2;
	f = &content_features(i);
	f->description = std::string("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_MELON)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 2;
	content_nodebox_plant_2(f);
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_MELON_3;
	f = &content_features(i);
	f->description = std::string("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_MELON)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 1;
	content_nodebox_plant_3(f);
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_MELON;
	f = &content_features(i);
	f->description = std::string("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->setInventoryTextureCube("farm_melon.png","farm_melon.png","farm_melon.png");
	f->draw_type = CDT_CUBELIKE;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_FARM_MELON)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_PUMPKIN_1;
	f = &content_features(i);
	f->description = std::string("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_PUMPKIN)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 3;
	content_nodebox_plant_1(f);
	f->type = CMT_PLANT;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_PUMPKIN_2;
	f = &content_features(i);
	f->description = std::string("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_PUMPKIN)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 2;
	content_nodebox_plant_2(f);
	f->type = CMT_PLANT;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_PUMPKIN_3;
	f = &content_features(i);
	f->description = std::string("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_PUMPKIN)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 1;
	content_nodebox_plant_3(f);
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_PUMPKIN;
	f = &content_features(i);
	f->description = std::string("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->setInventoryTextureCube("farm_pumpkin_top.png","farm_pumpkin.png","farm_pumpkin.png");
	f->draw_type = CDT_CUBELIKE;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_FARM_PUMPKIN)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	lists::add("creative", i);
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_PUMPKIN_JACK;
	f = &content_features(i);
	f->description = std::string("Jack' O Lantern");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->setTexture(5, "farm_pumpkin_jack.png"); // Z-
	f->setInventoryTextureCube("farm_pumpkin_top.png","farm_pumpkin_jack.png","farm_pumpkin.png");
	f->draw_type = CDT_NODEBOX;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->light_source = LIGHT_MAX-1;
	content_nodebox_jackolantern(f);
	f->setInventoryTextureNodeBox(i,"farm_pumpkin_top.png","farm_pumpkin_jack.png","farm_pumpkin.png");
	crafting::set1Any2Recipe(CONTENT_TORCH,CONTENT_FARM_PUMPKIN,CONTENT_FARM_PUMPKIN_JACK);
	lists::add("creative", i);
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_POTATO_1;
	f = &content_features(i);
	f->description = std::string("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_POTATO)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 3;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_POTATO_2;
	f = &content_features(i);
	f->description = std::string("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_POTATO)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_POTATO_3;
	f = &content_features(i);
	f->description = std::string("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_POTATO)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_potato 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_POTATO;
	f = &content_features(i);
	f->description = std::string("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_POTATO)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_potato 3");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_CARROT_1;
	f = &content_features(i);
	f->description = std::string("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_CARROT)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 3;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_CARROT_2;
	f = &content_features(i);
	f->description = std::string("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_CARROT)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_CARROT_3;
	f = &content_features(i);
	f->description = std::string("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_CARROT)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_carrot 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_CARROT;
	f = &content_features(i);
	f->description = std::string("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_CARROT)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_carrot 3");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_BEETROOT_1;
	f = &content_features(i);
	f->description = std::string("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_BEETROOT)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 3;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_BEETROOT_2;
	f = &content_features(i);
	f->description = std::string("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_BEETROOT)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_BEETROOT_3;
	f = &content_features(i);
	f->description = std::string("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_BEETROOT)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_beetroot 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_BEETROOT;
	f = &content_features(i);
	f->description = std::string("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_BEETROOT)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_beetroot 3");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_GRAPEVINE_1;
	f = &content_features(i);
	f->description = std::string("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 3;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_GRAPEVINE_2;
	f = &content_features(i);
	f->description = std::string("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->extra_dug_item = std::string("CraftItem mush 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_GRAPEVINE_3;
	f = &content_features(i);
	f->description = std::string("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_grape 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_GRAPEVINE;
	f = &content_features(i);
	f->description = std::string("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("CraftItem harvested_grape 3");
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_COTTON_1;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_COTTON)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_COTTON_2;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_COTTON)+" 1";
	f->extra_dug_item = std::string("CraftItem string 1");
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_COTTON_3;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_COTTON)+" 1";
	f->extra_dug_item = std::string("CraftItem string 2");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_COTTON;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_COTTON)+" 2";
	f->extra_dug_item = std::string("CraftItem string 6");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_TRELLIS_GRAPE_1;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis_grape_1.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_TRELLIS_GRAPE_2;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis_grape_2.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_TRELLIS_GRAPE_3;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis_grape_3.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_grape 1");
	f->extra_dug_item_rarity = 1;
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_FARM_TRELLIS_GRAPE;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis_grape.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_grape 3");
	f->extra_dug_item_rarity = 1;
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->pressure_type = CST_CRUSHABLE;

	i = CONTENT_TRELLIS_DEAD_VINE;
	f = &content_features(i);
	f->description = std::string("Dead Vine");
	f->setAllTextures("trellis_dead_vine.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ") + itos(CONTENT_DEAD_VINE)+" 1";
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.3;
	f->pressure_type = CST_CRUSHABLE;
}
