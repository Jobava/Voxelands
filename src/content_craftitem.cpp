/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_craftitem.cpp
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
*
* License updated from GPLv2 or later to GPLv3 or later by Lisa Milne
* for Voxelands.
************************************************************************/

#include "content_craftitem.h"
#include "content_mapnode.h"
#include "content_craft.h"
#include "content_mob.h"
#include "content_list.h"
#include <map>
#include "intl.h"
#include "player.h"

std::map<content_t,struct CraftItemFeatures> g_content_craftitem_features;

CraftItemFeatures & content_craftitem_features(content_t i)
{
	if ((i&CONTENT_CRAFTITEM_MASK) != CONTENT_CRAFTITEM_MASK)
		return g_content_craftitem_features[CONTENT_IGNORE];

	// convert deprecated items to new
	if (i == (CONTENT_CRAFTITEM_MASK | 0x2D)) // rotten fruit
		i = CONTENT_CRAFTITEM_MUSH;
	if (i == (CONTENT_CRAFTITEM_MASK | 0x15)) // scorched stuff
		i = CONTENT_CRAFTITEM_ASH;

	std::map<content_t,struct CraftItemFeatures>::iterator it = g_content_craftitem_features.find(i);
	if (it == g_content_craftitem_features.end())
		return g_content_craftitem_features[CONTENT_IGNORE];
	return it->second;
}

CraftItemFeatures & content_craftitem_features(std::string subname)
{
	// convert deprecated items to new
	if (subname == "scorched_stuff")
		return content_craftitem_features(CONTENT_CRAFTITEM_ASH);
	if (subname == "rotten_fruit")
		return content_craftitem_features(CONTENT_CRAFTITEM_MUSH);

	for (std::map<content_t,struct CraftItemFeatures>::iterator i = g_content_craftitem_features.begin(); i!=g_content_craftitem_features.end(); i++) {
		if (i->second.name == subname)
			return i->second;
	}
	return g_content_craftitem_features[CONTENT_IGNORE];
}

void content_craftitem_init()
{
	g_content_craftitem_features.clear();

	content_t i;
	CraftItemFeatures *f = NULL;

	i = CONTENT_CRAFTITEM_PAPER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_PAPER;
	f->texture = "paper.png";
	f->name = "paper";
	f->description = wgettext("Paper");
	crafting::setRow1Recipe(CONTENT_PAPYRUS,CONTENT_CRAFTITEM_PAPER);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CHARCOAL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CHARCOAL;
	f->texture = "lump_of_charcoal.png";
	f->name = "lump_of_charcoal";
	f->description = wgettext("Charcoal Lump");
	f->fuel_time = 40;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COAL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COAL;
	f->texture = "lump_of_coal.png";
	f->name = "lump_of_coal";
	f->description = wgettext("Coal Lump");
	f->fuel_time = 40;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_IRON;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_IRON;
	f->texture = "lump_of_iron.png";
	f->name = "lump_of_iron";
	f->description = wgettext("Iron Lump");
	f->cook_result = CONTENT_CRAFTITEM_STEEL_INGOT;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_CLAY;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CLAY;
	f->texture = "lump_of_clay.png";
	f->name = "lump_of_clay";
	f->description = wgettext("Clay Lump");
	f->cook_result = CONTENT_CRAFTITEM_CLAY_BRICK;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_TIN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_TIN;
	f->texture = "lump_of_tin.png";
	f->name = "lump_of_tin";
	f->description = wgettext("Tin Lump");
	f->cook_result = CONTENT_CRAFTITEM_TIN_INGOT;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_COPPER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COPPER;
	f->texture = "lump_of_copper.png";
	f->name = "lump_of_copper";
	f->description = wgettext("Copper Lump");
	f->cook_result = CONTENT_CRAFTITEM_COPPER_INGOT;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_SILVER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SILVER;
	f->texture = "lump_of_silver.png";
	f->name = "lump_of_silver";
	f->description = wgettext("Silver Lump");
	f->cook_result = CONTENT_CRAFTITEM_SILVER_INGOT;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_GOLD;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_GOLD;
	f->texture = "lump_of_gold.png";
	f->name = "lump_of_gold";
	f->description = wgettext("Gold Lump");
	f->cook_result = CONTENT_CRAFTITEM_GOLD_INGOT;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_QUARTZ;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_QUARTZ;
	f->texture = "lump_of_quartz.png";
	f->name = "lump_of_quartz";
	f->description = wgettext("Quartz Crystal");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_TIN_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_TIN_INGOT;
	f->texture = "tin_ingot.png";
	f->name = "tin_ingot";
	f->description = wgettext("Tin Ingot");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COPPER_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COPPER_INGOT;
	f->texture = "copper_ingot.png";
	f->name = "copper_ingot";
	f->description = wgettext("Copper Ingot");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_SILVER_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SILVER_INGOT;
	f->texture = "silver_ingot.png";
	f->name = "silver_ingot";
	f->description = wgettext("Silver Ingot");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_GOLD_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_GOLD_INGOT;
	f->texture = "gold_ingot.png";
	f->name = "gold_ingot";
	f->description = wgettext("Gold Ingot");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FLINT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FLINT;
	f->texture = "lump_of_flint.png";
	f->name = "lump_of_flint";
	f->description = wgettext("Flint");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_STEEL_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_STEEL_INGOT;
	f->texture = "steel_ingot.png";
	f->name = "steel_ingot";
	f->description = wgettext("Steel Ingot");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CLAY_BRICK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CLAY_BRICK;
	f->texture = "clay_brick.png";
	f->name = "clay_brick";
	f->description = wgettext("Brick");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_RAT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_RAT;
	f->texture = "rat.png";
	f->name = "rat";
	f->description = wgettext("Rat");
	f->cook_result = CONTENT_CRAFTITEM_COOKED_RAT;
	f->drop_count = 1;
	f->drop_item = CONTENT_MOB_RAT;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_COOKED_RAT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COOKED_RAT;
	f->texture = "cooked_rat.png";
	f->name = "cooked_rat";
	f->description = wgettext("Cooked Rat");
	f->cook_result = CONTENT_CRAFTITEM_ASH;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 8;
	f->health_effect = 4;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_FIREFLY;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FIREFLY;
	f->texture = "firefly.png";
	f->name = "firefly";
	f->description = wgettext("Firefly");
	f->drop_count = 1;
	f->drop_item = CONTENT_MOB_FIREFLY;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_APPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_APPLE;
	f->texture = "apple.png^[forcesingle";
	f->name = "apple";
	f->description = wgettext("Apple");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 6;
	f->health_effect = 3;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_APPLE_IRON;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_APPLE_IRON;
	f->texture = "apple_iron.png";
	f->name = "apple_iron";
	f->description = wgettext("Iron Apple");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 16;
	f->health_effect = 8;
	{
		u16 recipe[9] = {
			CONTENT_CRAFTITEM_STEEL_INGOT,		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_IGNORE,	CONTENT_CRAFTITEM_APPLE,		CONTENT_IGNORE,
			CONTENT_CRAFTITEM_STEEL_INGOT,		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STEEL_INGOT
		};
		crafting::setRecipe(recipe,CONTENT_CRAFTITEM_APPLE_IRON,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DYE_BLUE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_BLUE;
	f->texture = "dye_blue.png";
	f->name = "dye_blue";
	f->description = wgettext("Blue Dye");
	crafting::set1To2Recipe(CONTENT_FLOWER_TULIP,CONTENT_CRAFTITEM_DYE_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DYE_GREEN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_GREEN;
	f->texture = "dye_green.png";
	f->name = "dye_green";
	f->description = wgettext("Green Dye");
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_DYE_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DYE_ORANGE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_ORANGE;
	f->texture = "dye_orange.png";
	f->name = "dye_orange";
	f->description = wgettext("Orange Dye");
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_DYE_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DYE_PURPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_PURPLE;
	f->texture = "dye_purple.png";
	f->name = "dye_purple";
	f->description = wgettext("Purple Dye");
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_DYE_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DYE_RED;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_RED;
	f->texture = "dye_red.png";
	f->name = "dye_red";
	f->description = wgettext("Red Dye");
	crafting::set1To2Recipe(CONTENT_FLOWER_ROSE,CONTENT_CRAFTITEM_DYE_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DYE_YELLOW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_YELLOW;
	f->texture = "dye_yellow.png";
	f->name = "dye_yellow";
	f->description = wgettext("Yellow Dye");
	crafting::set1To2Recipe(CONTENT_CACTUS_BLOSSOM,CONTENT_CRAFTITEM_DYE_YELLOW);
	crafting::set1To2Recipe(CONTENT_FLOWER_DAFFODIL,CONTENT_CRAFTITEM_DYE_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DYE_WHITE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_WHITE;
	f->texture = "dye_white.png";
	f->name = "dye_white";
	f->description = wgettext("White Dye");
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_APPLE_BLOSSOM,CONTENT_CRAFTITEM_DYE_WHITE);
	crafting::set2Any3Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_DYE_WHITE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DYE_BLACK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_BLACK;
	f->texture = "dye_black.png";
	f->name = "dye_black";
	f->description = wgettext("Black Dye");
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_CRAFTITEM_COAL,CONTENT_CRAFTITEM_DYE_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_QUARTZ_DUST;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_QUARTZ_DUST;
	f->texture = "quartz_dust.png";
	f->name = "quartz_dust";
	f->description = wgettext("Quartz Dust");
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_QUARTZ,CONTENT_CRAFTITEM_QUARTZ_DUST);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_SALTPETER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SALTPETER;
	f->texture = "saltpeter.png";
	f->name = "saltpeter";
	f->description = wgettext("Saltpeter");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_GUNPOWDER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_GUNPOWDER;
	f->texture = "gunpowder.png";
	f->name = "gunpowder";
	f->description = wgettext("Gun Powder");
	crafting::set1Any3Recipe(CONTENT_CRAFTITEM_CHARCOAL,CONTENT_CRAFTITEM_FLINT,CONTENT_CRAFTITEM_SALTPETER,CONTENT_CRAFTITEM_GUNPOWDER);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_SNOW_BALL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SNOW_BALL;
	f->texture = "snow_ball.png";
	f->name = "snow_ball";
	f->description = wgettext("Snow Ball");
	f->thrown_item = CONTENT_MOB_SNOWBALL;

	i = CONTENT_CRAFTITEM_STICK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_STICK;
	f->texture = "stick.png";
	f->name = "Stick";
	f->description = wgettext("Stick");
	f->fuel_time = 30/16;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_PINE_PLANK,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_LEAVES,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_APPLE_LEAVES,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_JUNGLELEAVES,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_CONIFER_LEAVES,CONTENT_CRAFTITEM_STICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_PINE_PLANK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_PINE_PLANK;
	f->texture = "pine_plank.png";
	f->name = "pine_plank";
	f->description = wgettext("Pine Plank");
	f->fuel_time = 30/16;
	crafting::set1To4Recipe(CONTENT_WOOD_PINE,CONTENT_CRAFTITEM_PINE_PLANK);
	crafting::set1To2Recipe(CONTENT_YOUNG_CONIFER_TREE,CONTENT_CRAFTITEM_PINE_PLANK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_WOOD_PLANK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_WOOD_PLANK;
	f->texture = "wood_plank.png";
	f->name = "wood_plank";
	f->description = wgettext("Wood Plank");
	f->fuel_time = 30/16;
	crafting::set1To4Recipe(CONTENT_WOOD,CONTENT_CRAFTITEM_WOOD_PLANK);
	crafting::set1To2Recipe(CONTENT_YOUNG_TREE,CONTENT_CRAFTITEM_WOOD_PLANK);
	crafting::set1To2Recipe(CONTENT_YOUNG_APPLE_TREE,CONTENT_CRAFTITEM_WOOD_PLANK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_JUNGLE_PLANK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_JUNGLE_PLANK;
	f->texture = "jungle_plank.png";
	f->name = "jungle_plank";
	f->description = wgettext("Jungle Wood Plank");
	f->fuel_time = 30/16;
	crafting::set1To4Recipe(CONTENT_JUNGLEWOOD,CONTENT_CRAFTITEM_JUNGLE_PLANK);
	crafting::set1To2Recipe(CONTENT_YOUNG_JUNGLETREE,CONTENT_CRAFTITEM_JUNGLE_PLANK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_TNT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_TNT;
	f->texture = "tnt_stick.png";
	f->name = "tnt_stick";
	f->description = wgettext("TNT Stick");
	{
		u16 recipe[9] = {
			CONTENT_CRAFTITEM_PAPER,	CONTENT_CRAFTITEM_GUNPOWDER,	CONTENT_CRAFTITEM_PAPER,
			CONTENT_IGNORE,			CONTENT_IGNORE,			CONTENT_IGNORE,
			CONTENT_IGNORE,			CONTENT_IGNORE,			CONTENT_IGNORE
		};
		crafting::setRecipe(recipe,CONTENT_CRAFTITEM_TNT,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_ASH;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_ASH;
	f->texture = "lump_of_ash.png";
	f->name = "lump_of_ash";
	f->description = wgettext("Ash");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_APPLE_BLOSSOM;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_APPLE_BLOSSOM;
	f->texture = "apple_blossom.png";
	f->name = "apple_blossom";
	f->description = wgettext("Apple Blossoms");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 2;
	f->health_effect = 1;
	f->fuel_time = 30/16;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CACTUS_FRUIT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CACTUS_FRUIT;
	f->texture = "cactus_fruit.png^[forcesingle";
	f->name = "cactus_fruit";
	f->description = wgettext("Cactus Berry");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 6;
	f->health_effect = 3;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_MUSH;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_MUSH;
	f->texture = "mush.png";
	f->name = "mush";
	f->cook_result = CONTENT_CRAFTITEM_ASH;
	f->description = wgettext("Mush");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->health_effect = -1;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_PUMPKINSLICE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_PUMPKINSLICE;
	f->texture = "pumpkin_slice.png";
	f->name = "pumpkin_slice";
	f->description = wgettext("Sliced Pumpkin");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 6;
	f->health_effect = 3;
	crafting::set1To2Recipe(CONTENT_FARM_PUMPKIN,CONTENT_CRAFTITEM_PUMPKINSLICE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_PUMPKIN_PIE_SLICE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_PUMPKIN_PIE_SLICE;
	f->texture = "pumpkin_pie_slice.png";
	f->name = "pumpkin_pie_slice";
	f->description = wgettext("Pumpkin Pie Slice");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 12;
	f->health_effect = 6;
	lists::add("craftguide",i);

	i = CONTENT_CRAFTITEM_APPLE_PIE_SLICE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_APPLE_PIE_SLICE;
	f->texture = "apple_pie_slice.png";
	f->name = "apple_pie_slice";
	f->description = wgettext("Apple Pie Slice");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 12;
	f->health_effect = 6;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_MELONSLICE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_MELONSLICE;
	f->texture = "melon_slice.png";
	f->name = "melon_slice";
	f->description = wgettext("Sliced Melon");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 8;
	f->health_effect = 4;
	crafting::set1To2Recipe(CONTENT_FARM_MELON,CONTENT_CRAFTITEM_MELONSLICE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_WHEAT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_WHEAT;
	f->texture = "harvested_wheat.png";
	f->name = "harvested_wheat";
	f->description = wgettext("Wheat");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 2;
	f->health_effect = 1;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FLOUR;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FLOUR;
	f->texture = "flour.png";
	f->name = "flour";
	f->description = wgettext("Flour");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 2;
	f->health_effect = 1;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_WHEAT,CONTENT_CRAFTITEM_WHEAT,CONTENT_CRAFTITEM_FLOUR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_DOUGH;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DOUGH;
	f->texture = "dough.png";
	f->name = "dough";
	f->description = wgettext("Dough");
	f->cook_result = CONTENT_CRAFTITEM_BREAD;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 4;
	f->health_effect = 2;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FLOUR,CONTENT_CRAFTITEM_FLOUR,CONTENT_CRAFTITEM_DOUGH);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_BREAD;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_BREAD;
	f->texture = "bread.png";
	f->name = "bread";
	f->description = wgettext("Bread");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 10;
	f->health_effect = 5;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_POTATO;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_POTATO;
	f->texture = "harvested_potato.png";
	f->name = "harvested_potato";
	f->description = wgettext("Potato");
	f->cook_result = CONTENT_CRAFTITEM_ROASTPOTATO;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 6;
	f->health_effect = 3;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_STARCH;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_STARCH;
	f->texture = "potato_starch.png";
	f->name = "potato_starch";
	f->description = wgettext("Potato Starch");
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_POTATO,CONTENT_CRAFTITEM_POTATO,CONTENT_CRAFTITEM_STARCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_ROASTPOTATO;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_ROASTPOTATO;
	f->texture = "roast_potato.png";
	f->name = "roast_potato";
	f->description = wgettext("Roast Potato");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 12;
	f->health_effect = 6;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CARROT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CARROT;
	f->texture = "harvested_carrot.png";
	f->name = "harvested_carrot";
	f->description = wgettext("Carrot");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 6;
	f->health_effect = 4;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CARROT_CAKE_RAW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CARROT_CAKE_RAW;
	f->texture = "carrot_cake_raw.png";
	f->name = "carrot_cake_raw";
	f->description = wgettext("Raw Carrot Cake");
	f->cook_result = CONTENT_CRAFTITEM_CARROT_CAKE;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 6;
	f->health_effect = 3;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_CARROT,CONTENT_CRAFTITEM_DOUGH,CONTENT_CRAFTITEM_CARROT_CAKE_RAW);
	lists::add("craftguide",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_CARROT_CAKE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CARROT_CAKE;
	f->texture = "carrot_cake.png";
	f->name = "carrot_cake";
	f->description = wgettext("Carrot Cake");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 12;
	f->health_effect = 6;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_BEETROOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_BEETROOT;
	f->texture = "harvested_beetroot.png";
	f->name = "harvested_beetroot";
	f->description = wgettext("Beetroot");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 6;
	f->health_effect = 3;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_GRAPE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_GRAPE;
	f->texture = "harvested_grape.png";
	f->name = "harvested_grape";
	f->description = wgettext("Bunch of Grapes");
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 6;
	f->health_effect = 4;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_STRING;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_STRING;
	f->texture = "string.png";
	f->name = "string";
	f->description = wgettext("String");
	crafting::set1To4Recipe(CONTENT_COTTON,CONTENT_CRAFTITEM_STRING);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_MITHRILDUST;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_MITHRILDUST;
	f->texture = "mithril_dust.png";
	f->name = "mese_dust";
	f->description = wgettext("Mithril Dust");
	f->drop_count = 1;
	f->drop_item = CONTENT_CIRCUIT_MITHRILWIRE;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_MITHRIL_RAW,CONTENT_CRAFTITEM_MITHRILDUST);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_RESIN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_RESIN;
	f->texture = "resin.png";
	f->name = "lump_of_resin";
	f->description = wgettext("Resin");
	f->drop_count = 1;
	lists::add("cooking",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_OERKKI_DUST;
	f->texture = "oerkki_dust.png";
	f->name = "oerkki_dust";
	f->description = wgettext("Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_HOME; // teleports player to default home location
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FISH;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FISH;
	f->texture = "fish.png";
	f->name = "fish";
	f->description = wgettext("Fish");
	f->cook_result = CONTENT_CRAFTITEM_COOKED_FISH;
	f->drop_count = 1;
	f->drop_item = CONTENT_MOB_FISH;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 1;
	f->health_effect = -1;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_COOKED_FISH;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COOKED_FISH;
	f->texture = "cooked_fish.png";
	f->name = "cooked_fish";
	f->description = wgettext("Cooked Fish");
	f->cook_result = CONTENT_CRAFTITEM_ASH;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 8;
	f->health_effect = 6;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_MEAT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_MEAT;
	f->texture = "meat.png";
	f->name = "meat";
	f->description = wgettext("Meat");
	f->cook_result = CONTENT_CRAFTITEM_COOKED_MEAT;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 1;
	f->health_effect = -1;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_COOKED_MEAT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COOKED_MEAT;
	f->texture = "cooked_meat.png";
	f->name = "cooked_meat";
	f->description = wgettext("Cooked Meat");
	f->cook_result = CONTENT_CRAFTITEM_ASH;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 10;
	f->health_effect = 6;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_COTTON_SHEET;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COTTON_SHEET;
	f->texture = "cotton_sheet.png";
	f->name = "cotton_sheet";
	f->description = wgettext("Cotton Sheet");
	crafting::setRow2Recipe(CONTENT_CRAFTITEM_STRING,CONTENT_CRAFTITEM_COTTON_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_COTTON_SHEET_BLUE,CONTENT_CRAFTITEM_COTTON_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_COTTON_SHEET_GREEN,CONTENT_CRAFTITEM_COTTON_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_COTTON_SHEET_ORANGE,CONTENT_CRAFTITEM_COTTON_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_COTTON_SHEET_PURPLE,CONTENT_CRAFTITEM_COTTON_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_COTTON_SHEET_RED,CONTENT_CRAFTITEM_COTTON_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_COTTON_SHEET_YELLOW,CONTENT_CRAFTITEM_COTTON_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_COTTON_SHEET_BLACK,CONTENT_CRAFTITEM_COTTON_SHEET);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COTTON_SHEET_BLUE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COTTON_SHEET_BLUE;
	f->texture = "cotton_sheet_blue.png";
	f->name = "cotton_sheet_blue";
	f->description = wgettext("Blue Cotton Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_COTTON_SHEET,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_COTTON_SHEET_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COTTON_SHEET_GREEN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COTTON_SHEET_GREEN;
	f->texture = "cotton_sheet_green.png";
	f->name = "cotton_sheet_green";
	f->description = wgettext("Green Cotton Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_COTTON_SHEET,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CRAFTITEM_COTTON_SHEET_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COTTON_SHEET_ORANGE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COTTON_SHEET_ORANGE;
	f->texture = "cotton_sheet_orange.png";
	f->name = "cotton_sheet_orange";
	f->description = wgettext("Orange Cotton Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_COTTON_SHEET,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CRAFTITEM_COTTON_SHEET_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COTTON_SHEET_PURPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COTTON_SHEET_PURPLE;
	f->texture = "cotton_sheet_purple.png";
	f->name = "cotton_sheet_purple";
	f->description = wgettext("Purple Cotton Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_COTTON_SHEET,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CRAFTITEM_COTTON_SHEET_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COTTON_SHEET_RED;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COTTON_SHEET_RED;
	f->texture = "cotton_sheet_red.png";
	f->name = "cotton_sheet_red";
	f->description = wgettext("Red Cotton Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_COTTON_SHEET,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_COTTON_SHEET_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COTTON_SHEET_YELLOW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COTTON_SHEET_YELLOW;
	f->texture = "cotton_sheet_yellow.png";
	f->name = "cotton_sheet_yellow";
	f->description = wgettext("Yellow Cotton Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_COTTON_SHEET,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_COTTON_SHEET_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COTTON_SHEET_BLACK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COTTON_SHEET_BLACK;
	f->texture = "cotton_sheet_black.png";
	f->name = "cotton_sheet_black";
	f->description = wgettext("Black Cotton Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_COTTON_SHEET,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CRAFTITEM_COTTON_SHEET_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CANVAS_SHEET;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CANVAS_SHEET;
	f->texture = "canvas_sheet.png";
	f->name = "canvas_sheet";
	f->description = wgettext("Canvas Sheet");
	crafting::setCol2Recipe(CONTENT_CRAFTITEM_COTTON_SHEET,CONTENT_CRAFTITEM_CANVAS_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_CANVAS_SHEET_BLUE,CONTENT_CRAFTITEM_CANVAS_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_CANVAS_SHEET_GREEN,CONTENT_CRAFTITEM_CANVAS_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_CANVAS_SHEET_ORANGE,CONTENT_CRAFTITEM_CANVAS_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_CANVAS_SHEET_PURPLE,CONTENT_CRAFTITEM_CANVAS_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_CANVAS_SHEET_RED,CONTENT_CRAFTITEM_CANVAS_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_CANVAS_SHEET_YELLOW,CONTENT_CRAFTITEM_CANVAS_SHEET);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_CANVAS_SHEET_BLACK,CONTENT_CRAFTITEM_CANVAS_SHEET);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CANVAS_SHEET_BLUE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CANVAS_SHEET_BLUE;
	f->texture = "canvas_sheet_blue.png";
	f->name = "canvas_sheet_blue";
	f->description = wgettext("Blue Canvas Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_CANVAS_SHEET,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_CANVAS_SHEET_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CANVAS_SHEET_GREEN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CANVAS_SHEET_GREEN;
	f->texture = "canvas_sheet_green.png";
	f->name = "canvas_sheet_green";
	f->description = wgettext("Green Canvas Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_CANVAS_SHEET,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CRAFTITEM_CANVAS_SHEET_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CANVAS_SHEET_ORANGE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CANVAS_SHEET_ORANGE;
	f->texture = "canvas_sheet_orange.png";
	f->name = "canvas_sheet_orange";
	f->description = wgettext("Orange Canvas Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_CANVAS_SHEET,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CRAFTITEM_CANVAS_SHEET_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CANVAS_SHEET_PURPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CANVAS_SHEET_PURPLE;
	f->texture = "canvas_sheet_purple.png";
	f->name = "canvas_sheet_purple";
	f->description = wgettext("Purple Canvas Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_CANVAS_SHEET,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CRAFTITEM_CANVAS_SHEET_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CANVAS_SHEET_RED;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CANVAS_SHEET_RED;
	f->texture = "canvas_sheet_red.png";
	f->name = "canvas_sheet_red";
	f->description = wgettext("Red Canvas Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_CANVAS_SHEET,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_CANVAS_SHEET_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CANVAS_SHEET_YELLOW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CANVAS_SHEET_YELLOW;
	f->texture = "canvas_sheet_yellow.png";
	f->name = "canvas_sheet_yellow";
	f->description = wgettext("Yellow Canvas Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_CANVAS_SHEET,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_CANVAS_SHEET_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_CANVAS_SHEET_BLACK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CANVAS_SHEET_BLACK;
	f->texture = "canvas_sheet_black.png";
	f->name = "canvas_sheet_black";
	f->description = wgettext("Black Canvas Sheet");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_CANVAS_SHEET,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CRAFTITEM_CANVAS_SHEET_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR;
	f->texture = "fur.png";
	f->name = "fur";
	f->description = wgettext("Fur");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR_WHITE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR_WHITE;
	f->texture = "fur_white.png";
	f->name = "fur_white";
	f->description = wgettext("White Fur");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_CRAFTITEM_FUR_WHITE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_FUR_WHITE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR_BLUE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR_BLUE;
	f->texture = "fur_blue.png";
	f->name = "fur_blue";
	f->description = wgettext("Blue Fur");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR_WHITE,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_FUR_BLUE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_FUR_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR_GREEN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR_GREEN;
	f->texture = "fur_green.png";
	f->name = "fur_green";
	f->description = wgettext("Green Fur");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR_WHITE,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CRAFTITEM_FUR_GREEN);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CRAFTITEM_FUR_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR_ORANGE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR_ORANGE;
	f->texture = "fur_orange.png";
	f->name = "fur_orange";
	f->description = wgettext("Orange Fur");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR_WHITE,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CRAFTITEM_FUR_ORANGE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CRAFTITEM_FUR_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR_PURPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR_PURPLE;
	f->texture = "fur_purple.png";
	f->name = "fur_purple";
	f->description = wgettext("Purple Fur");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR_WHITE,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CRAFTITEM_FUR_PURPLE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CRAFTITEM_FUR_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR_RED;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR_RED;
	f->texture = "fur_red.png";
	f->name = "fur_red";
	f->description = wgettext("Red Fur");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR_WHITE,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_FUR_RED);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_FUR_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR_YELLOW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR_YELLOW;
	f->texture = "fur_yellow.png";
	f->name = "fur_yellow";
	f->description = wgettext("Yellow Fur");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR_WHITE,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_FUR_YELLOW);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_FUR_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FUR_BLACK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FUR_BLACK;
	f->texture = "fur_black.png";
	f->name = "fur_black";
	f->description = wgettext("Black Fur");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR_WHITE,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CRAFTITEM_FUR_BLACK);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CRAFTITEM_FUR_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER;
	f->texture = "leather.png";
	f->name = "leather";
	f->description = wgettext("Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_ASH,CONTENT_CRAFTITEM_LEATHER);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FUR,CONTENT_CRAFTITEM_MUSH,CONTENT_CRAFTITEM_LEATHER);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER_WHITE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER_WHITE;
	f->texture = "leather_white.png";
	f->name = "leather_white";
	f->description = wgettext("White Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_CRAFTITEM_LEATHER_WHITE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_STARCH,CONTENT_CRAFTITEM_LEATHER_WHITE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER_BLUE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER_BLUE;
	f->texture = "leather_blue.png";
	f->name = "leather_blue";
	f->description = wgettext("Blue Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER_WHITE,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_LEATHER_BLUE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_LEATHER_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER_GREEN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER_GREEN;
	f->texture = "leather_green.png";
	f->name = "leather_green";
	f->description = wgettext("Green Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER_WHITE,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CRAFTITEM_LEATHER_GREEN);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CRAFTITEM_LEATHER_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER_ORANGE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER_ORANGE;
	f->texture = "leather_orange.png";
	f->name = "leather_orange";
	f->description = wgettext("Orange Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER_WHITE,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CRAFTITEM_LEATHER_ORANGE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CRAFTITEM_LEATHER_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER_PURPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER_PURPLE;
	f->texture = "leather_purple.png";
	f->name = "leather_purple";
	f->description = wgettext("Purple Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER_WHITE,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CRAFTITEM_LEATHER_PURPLE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CRAFTITEM_LEATHER_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER_RED;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER_RED;
	f->texture = "leather_red.png";
	f->name = "leather_red";
	f->description = wgettext("Red Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER_WHITE,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_LEATHER_RED);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_LEATHER_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER_YELLOW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER_YELLOW;
	f->texture = "leather_yellow.png";
	f->name = "leather_yellow";
	f->description = wgettext("Yellow Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER_WHITE,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_LEATHER_YELLOW);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_LEATHER_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_LEATHER_BLACK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LEATHER_BLACK;
	f->texture = "leather_black.png";
	f->name = "leather_black";
	f->description = wgettext("Black Leather");
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER_WHITE,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CRAFTITEM_LEATHER_BLACK);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_LEATHER,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CRAFTITEM_LEATHER_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_ARROW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_ARROW;
	f->texture = "arrow.png";
	f->name = "arrow";
	f->description = wgettext("Arrow");
	f->shot_item = CONTENT_MOB_ARROW;
	crafting::set1over4Recipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_CRAFTITEM_STICK,CONTENT_CRAFTITEM_ARROW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_FERTILIZER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FERTILIZER;
	f->texture = "fertilizer_item.png";
	f->name = "fertilizer";
	f->description = wgettext("Fertilizer");
	f->drop_count = 1;
	f->drop_item = CONTENT_FERTILIZER;
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_MUSH,CONTENT_CRAFTITEM_ASH,CONTENT_CRAFTITEM_FERTILIZER);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST_WHITE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "oerkki_dust_white.png";
	f->name = "oerkki_dust_white";
	f->description = wgettext("White Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_WHITE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_OERKKI_DUST,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_CRAFTITEM_OERKKI_DUST_WHITE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST_BLUE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "oerkki_dust_blue.png";
	f->name = "oerkki_dust_blue";
	f->description = wgettext("Blue Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_BLUE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_OERKKI_DUST,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_OERKKI_DUST_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST_GREEN;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "oerkki_dust_green.png";
	f->name = "oerkki_dust_green";
	f->description = wgettext("Green Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_GREEN;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_OERKKI_DUST,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CRAFTITEM_OERKKI_DUST_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST_ORANGE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "oerkki_dust_orange.png";
	f->name = "oerkki_dust_orange";
	f->description = wgettext("Orange Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_ORANGE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_OERKKI_DUST,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CRAFTITEM_OERKKI_DUST_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST_PURPLE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "oerkki_dust_purple.png";
	f->name = "oerkki_dust_purple";
	f->description = wgettext("Purple Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_PURPLE;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_OERKKI_DUST,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CRAFTITEM_OERKKI_DUST_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST_RED;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "oerkki_dust_red.png";
	f->name = "oerkki_dust_red";
	f->description = wgettext("Red Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_RED;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_OERKKI_DUST,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_OERKKI_DUST_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST_YELLOW;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "oerkki_dust_yellow.png";
	f->name = "oerkki_dust_yellow";
	f->description = wgettext("Yellow Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_YELLOW;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_OERKKI_DUST,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_OERKKI_DUST_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_OERKKI_DUST_BLACK;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "oerkki_dust_black.png";
	f->name = "oerkki_dust_black";
	f->description = wgettext("Black Oerkki Dust");
	f->drop_count = 1;
	f->teleports = PLAYERFLAG_BLACK;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_OERKKI_DUST,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CRAFTITEM_OERKKI_DUST_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_GLASS_BOTTLE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "glass_bottle.png";
	f->name = "glass_bottle";
	f->description = wgettext("Glass Bottle");
	f->drop_count = 1;
	{
		u16 r[9] = {
			CONTENT_GLASS,	CONTENT_IGNORE,	CONTENT_GLASS,
			CONTENT_GLASS,	CONTENT_IGNORE,	CONTENT_GLASS,
			CONTENT_IGNORE,	CONTENT_GLASS,	CONTENT_IGNORE
		};
		crafting::setRecipe(r,CONTENT_CRAFTITEM_GLASS_BOTTLE,5);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_GRAPE_JUICE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "drink_grape.png^glass_bottle.png";
	f->name = "grape_juice";
	f->description = wgettext("Grape Juice");
	f->stackable = false;
	f->drop_count = 1;
	f->consumable = true;
	f->sound_use = "use-drink";
	f->hunger_effect = 3;
	f->health_effect = 3;
	f->energy_effect = 30;
	f->onuse_replace_item = CONTENT_CRAFTITEM_GLASS_BOTTLE;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_GRAPE,CONTENT_CRAFTITEM_GLASS_BOTTLE,CONTENT_CRAFTITEM_GRAPE_JUICE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_APPLE_JUICE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "drink_apple.png^glass_bottle.png";
	f->name = "apple_juice";
	f->description = wgettext("Apple Juice");
	f->stackable = false;
	f->drop_count = 1;
	f->consumable = true;
	f->sound_use = "use-drink";
	f->hunger_effect = 2;
	f->health_effect = 3;
	f->energy_effect = 10;
	f->onuse_replace_item = CONTENT_CRAFTITEM_GLASS_BOTTLE;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_APPLE,CONTENT_CRAFTITEM_GLASS_BOTTLE,CONTENT_CRAFTITEM_APPLE_JUICE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_TEA_LEAVES;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "tea_leaves.png";
	f->name = "tea_leaves";
	f->description = wgettext("Tea Leaves");
	f->drop_count = 1;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 1;
	f->health_effect = 2;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_TEA;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "steel_bottle.png^tea_bag.png";
	f->name = "tea_drink";
	f->description = wgettext("Tea");
	f->stackable = false;
	f->drop_count = 1;
	f->consumable = true;
	f->sound_use = "use-drink";
	f->hunger_effect = 3;
	f->health_effect = 4;
	f->cold_effect = 300;
	f->energy_effect = 10;
	f->onuse_replace_item = CONTENT_CRAFTITEM_STEEL_BOTTLE;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_TEA_LEAVES,CONTENT_CRAFTITEM_STEEL_BOTTLE_WATER,CONTENT_CRAFTITEM_TEA);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COFFEE_BEANS;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "coffee_beans.png";
	f->name = "coffee_beans";
	f->description = wgettext("Roasted Coffee Beans");
	f->drop_count = 1;
	f->consumable = true;
	f->sound_use = "use-eat";
	f->hunger_effect = 2;
	f->health_effect = 3;
	f->energy_effect = 30;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_COFFEE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "steel_bottle.png^coffee_bean.png";
	f->name = "coffee_drink";
	f->description = wgettext("Coffee");
	f->stackable = false;
	f->drop_count = 1;
	f->consumable = true;
	f->sound_use = "use-drink";
	f->hunger_effect = 2;
	f->health_effect = 3;
	f->cold_effect = 10;
	f->energy_effect = 300;
	f->onuse_replace_item = CONTENT_CRAFTITEM_STEEL_BOTTLE;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_COFFEE_BEANS,CONTENT_CRAFTITEM_STEEL_BOTTLE_WATER,CONTENT_CRAFTITEM_COFFEE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_STEEL_BOTTLE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "steel_bottle.png";
	f->name = "steel_bottle";
	f->description = wgettext("Steel Bottle");
	f->drop_count = 1;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_IGNORE,			CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_IGNORE,			CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_IGNORE,			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_IGNORE
		};
		crafting::setRecipe(r,CONTENT_CRAFTITEM_STEEL_BOTTLE,5);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_GLASS_BOTTLE_WATER;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "drink_water.png^glass_bottle.png";
	f->name = "water_drink";
	f->description = wgettext("Bottle of Water");
	f->stackable = false;
	f->drop_count = 1;
	f->consumable = true;
	f->sound_use = "use-drink";
	f->hunger_effect = 1;
	f->health_effect = 1;
	f->onuse_replace_item = CONTENT_CRAFTITEM_GLASS_BOTTLE;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_STEEL_BOTTLE_WATER;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "steel_bottle.png^water_droplet.png";
	f->name = "hotwater_drink";
	f->description = wgettext("Bottle of Hot Water");
	f->stackable = false;
	f->drop_count = 1;
	f->consumable = true;
	f->sound_use = "use-drink";
	f->hunger_effect = 1;
	f->health_effect = 1;
	f->cold_effect = 5;
	f->onuse_replace_item = CONTENT_CRAFTITEM_STEEL_BOTTLE;
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_MITHRIL_RAW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_MITHRIL_RAW;
	f->texture = "mithril_raw.png";
	f->name = "mithril_raw";
	f->description = wgettext("Raw Mithril");
	f->cook_result = CONTENT_CRAFTITEM_MITHRIL_UNBOUND;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_CRAFTITEM_MITHRIL_UNBOUND;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_MITHRIL_UNBOUND;
	f->texture = "mithril_unbound.png";
	f->name = "mithril_unbound";
	f->enchanted_item = CONTENT_CRAFTITEM_MITHRIL;
	f->description = wgettext("Unbound Mithril");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_MITHRIL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_MITHRIL;
	f->texture = "mithril_unbound.png";
	f->name = "mithril_bound";
	f->param_type = CPT_ENCHANTMENT;
	f->description = wgettext("Mithril");
	f->overlay_base = "ingot_overlay";
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_RUBY;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "lump_of_ruby.png";
	f->name = "lump_of_ruby";
	f->description = wgettext("Ruby");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_TURQUOISE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "lump_of_turquoise.png";
	f->name = "lump_of_turquiose";
	f->description = wgettext("Turquiose");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_AMETHYST;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "lump_of_amethyst.png";
	f->name = "lump_of_amethyst";
	f->description = wgettext("Amethyst");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_SAPPHIRE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "lump_of_sapphire.png";
	f->name = "lump_of_sapphire";
	f->description = wgettext("Sapphire");
	lists::add("creative",i);

	i = CONTENT_CRAFTITEM_SUNSTONE;
	f = &g_content_craftitem_features[i];
	f->content = i;
	f->texture = "lump_of_sunstone.png";
	f->name = "lump_of_sunstone";
	f->description = wgettext("Sunstone");
	lists::add("creative",i);
}
