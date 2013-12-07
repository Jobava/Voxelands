/************************************************************************
* content_craftitem.cpp
* minetest-classic - 3d voxel world sandbox game
* Copyright (C) Lisa Milne 2013 <lisa@ltmnet.com>
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

#include "content_craftitem.h"
#include "content_mapnode.h"
#include "content_craft.h"
#include <map>

std::map<content_t,struct CraftItemFeatures> g_content_craftitem_features;

CraftItemFeatures & content_craftitem_features(content_t i)
{
	if ((i&CONTENT_CRAFTITEM_MASK) != CONTENT_CRAFTITEM_MASK)
		return g_content_craftitem_features[CONTENT_IGNORE];

	std::map<content_t,struct CraftItemFeatures>::iterator it = g_content_craftitem_features.find(i);
	if (it == g_content_craftitem_features.end())
		return g_content_craftitem_features[CONTENT_IGNORE];
	return it->second;
}

CraftItemFeatures & content_craftitem_features(std::string subname)
{
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
	f->gui_name = "Paper";
	crafting::setRow1Recipe(CONTENT_PAPYRUS,CONTENT_CRAFTITEM_PAPER);

	i = CONTENT_CRAFTITEM_BOOK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_BOOK;
	f->texture = "book.png";
	f->name = "book";
	f->gui_name = "Book";
	crafting::setCol1Recipe(CONTENT_CRAFTITEM_PAPER,CONTENT_CRAFTITEM_BOOK);

	i = CONTENT_CRAFTITEM_CHARCOAL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CHARCOAL;
	f->texture = "lump_of_charcoal.png";
	f->name = "lump_of_charcoal";
	f->gui_name = "Charcoal Lump";
	f->fuel_time = 40;

	i = CONTENT_CRAFTITEM_COAL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COAL;
	f->texture = "lump_of_coal.png";
	f->name = "lump_of_coal";
	f->gui_name = "Coal Lump";
	f->fuel_time = 40;

	i = CONTENT_CRAFTITEM_IRON;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_IRON;
	f->texture = "lump_of_iron.png";
	f->name = "lump_of_iron";
	f->gui_name = "Iron Lump";
	f->cook_result = "CraftItem steel_ingot 1";

	i = CONTENT_CRAFTITEM_CLAY;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CLAY;
	f->texture = "lump_of_clay.png";
	f->name = "lump_of_clay";
	f->gui_name = "Clay Lump";
	f->cook_result = "CraftItem clay_brick 1";

	i = CONTENT_CRAFTITEM_TIN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_TIN;
	f->texture = "lump_of_tin.png";
	f->name = "lump_of_tin";
	f->gui_name = "Tin Lump";
	f->cook_result = "CraftItem tin_ingot 1";

	i = CONTENT_CRAFTITEM_COPPER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COPPER;
	f->texture = "lump_of_copper.png";
	f->name = "lump_of_copper";
	f->gui_name = "Copper Lump";
	f->cook_result = "CraftItem copper_ingot 1";

	i = CONTENT_CRAFTITEM_SILVER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SILVER;
	f->texture = "lump_of_silver.png";
	f->name = "lump_of_silver";
	f->gui_name = "Silver Lump";
	f->cook_result = "CraftItem silver_ingot 1";

	i = CONTENT_CRAFTITEM_GOLD;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_GOLD;
	f->texture = "lump_of_gold.png";
	f->name = "lump_of_gold";
	f->gui_name = "Gold Lump";
	f->cook_result = "CraftItem gold_ingot 1";

	i = CONTENT_CRAFTITEM_QUARTZ;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_QUARTZ;
	f->texture = "lump_of_quartz.png";
	f->name = "lump_of_quartz";
	f->gui_name = "Quartz Crystal";

	i = CONTENT_CRAFTITEM_TIN_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_TIN_INGOT;
	f->texture = "tin_ingot.png";
	f->name = "tin_ingot";
	f->gui_name = "Tin Ingot";

	i = CONTENT_CRAFTITEM_COPPER_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COPPER_INGOT;
	f->texture = "copper_ingot.png";
	f->name = "copper_ingot";
	f->gui_name = "Copper Ingot";

	i = CONTENT_CRAFTITEM_SILVER_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SILVER_INGOT;
	f->texture = "silver_ingot.png";
	f->name = "silver_ingot";
	f->gui_name = "Silver Ingot";

	i = CONTENT_CRAFTITEM_GOLD_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_GOLD_INGOT;
	f->texture = "gold_ingot.png";
	f->name = "gold_ingot";
	f->gui_name = "Gold Ingot";

	i = CONTENT_CRAFTITEM_FLINT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FLINT;
	f->texture = "lump_of_flint.png";
	f->name = "lump_of_flint";
	f->gui_name = "Flint";

	i = CONTENT_CRAFTITEM_STEEL_INGOT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_STEEL_INGOT;
	f->texture = "steel_ingot.png";
	f->name = "steel_ingot";
	f->gui_name = "Steel Ingot";

	i = CONTENT_CRAFTITEM_CLAY_BRICK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_CLAY_BRICK;
	f->texture = "clay_brick.png";
	f->name = "clay_brick";
	f->gui_name = "Brick";

	i = CONTENT_CRAFTITEM_RAT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_RAT;
	f->texture = "rat.png";
	f->name = "rat";
	f->gui_name = "Rat";
	f->cook_result = "CraftItem cooked_rat 1";
	f->drop_count = 1;

	i = CONTENT_CRAFTITEM_COOKED_RAT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_COOKED_RAT;
	f->texture = "cooked_rat.png";
	f->name = "cooked_rat";
	f->gui_name = "Cooked Rat";
	f->cook_result = "CraftItem scorched_stuff 1";
	f->edible = 6;

	i = CONTENT_CRAFTITEM_SCORCHED_STUFF;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SCORCHED_STUFF;
	f->texture = "scorched_stuff.png";
	f->name = "scorched_stuff";
	f->gui_name = "Scorched Stuff";

	i = CONTENT_CRAFTITEM_FIREFLY;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_FIREFLY;
	f->texture = "firefly.png";
	f->name = "firefly";
	f->gui_name = "Firefly";
	f->drop_count = 1;

	i = CONTENT_CRAFTITEM_APPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_APPLE;
	f->texture = "apple.png^[forcesingle";
	f->name = "apple";
	f->gui_name = "Apple";
	f->edible = 4;

	i = CONTENT_CRAFTITEM_APPLE_IRON;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_APPLE_IRON;
	f->texture = "apple_iron.png";
	f->name = "apple_iron";
	f->gui_name = "apple_iron";
	f->edible = 8;
	{
		u16 recipe[9] = {
			CONTENT_CRAFTITEM_STEEL_INGOT,		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_IGNORE,	CONTENT_CRAFTITEM_APPLE,		CONTENT_IGNORE,
			CONTENT_CRAFTITEM_STEEL_INGOT,		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STEEL_INGOT
		};
		crafting::setRecipe(recipe,CONTENT_CRAFTITEM_APPLE_IRON,1);
	}

	i = CONTENT_CRAFTITEM_DYE_BLUE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_BLUE;
	f->texture = "dye_blue.png";
	f->name = "dye_blue";
	f->gui_name = "Blue Dye";
	crafting::set1To2Recipe(CONTENT_FLOWER_TULIP,CONTENT_CRAFTITEM_DYE_BLUE);

	i = CONTENT_CRAFTITEM_DYE_GREEN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_GREEN;
	f->texture = "dye_green.png";
	f->name = "dye_green";
	f->gui_name = "Green Dye";
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_DYE_GREEN);

	i = CONTENT_CRAFTITEM_DYE_ORANGE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_ORANGE;
	f->texture = "dye_orange.png";
	f->name = "dye_orange";
	f->gui_name = "Orange Dye";
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_DYE_ORANGE);

	i = CONTENT_CRAFTITEM_DYE_PURPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_PURPLE;
	f->texture = "dye_purple.png";
	f->name = "dye_purple";
	f->gui_name = "Purple Dye";
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_DYE_PURPLE);

	i = CONTENT_CRAFTITEM_DYE_RED;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_RED;
	f->texture = "dye_red.png";
	f->name = "dye_red";
	f->gui_name = "Red Dye";
	crafting::set1To2Recipe(CONTENT_FLOWER_ROSE,CONTENT_CRAFTITEM_DYE_RED);

	i = CONTENT_CRAFTITEM_DYE_YELLOW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_YELLOW;
	f->texture = "dye_yellow.png";
	f->name = "dye_yellow";
	f->gui_name = "Yellow Dye";
	crafting::set1To2Recipe(CONTENT_FLOWER_DAFFODIL,CONTENT_CRAFTITEM_DYE_YELLOW);

	i = CONTENT_CRAFTITEM_DYE_WHITE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_WHITE;
	f->texture = "dye_white.png";
	f->name = "dye_white";
	f->gui_name = "White Dye";
	crafting::set2Any3Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CRAFTITEM_DYE_RED,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CRAFTITEM_DYE_WHITE);

	i = CONTENT_CRAFTITEM_DYE_BLACK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_BLACK;
	f->texture = "dye_black.png";
	f->name = "dye_black";
	f->gui_name = "Black Dye";
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_CRAFTITEM_COAL,CONTENT_CRAFTITEM_DYE_BLACK);

	i = CONTENT_CRAFTITEM_QUARTZ_DUST;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_QUARTZ_DUST;
	f->texture = "quartz_dust.png";
	f->name = "quartz_dust";
	f->gui_name = "Quartz Dust";
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_QUARTZ,CONTENT_CRAFTITEM_QUARTZ_DUST);

	i = CONTENT_CRAFTITEM_MESEDUST;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_MESEDUST;
	f->texture = "mese_dust.png";
	f->name = "mese_dust";
	f->gui_name = "Mese Dust";
	f->drop_count = 1;
	crafting::set1To2Recipe(CONTENT_MESE,CONTENT_CRAFTITEM_MESEDUST);

	i = CONTENT_CRAFTITEM_SALTPETER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SALTPETER;
	f->texture = "saltpeter.png";
	f->name = "saltpeter";
	f->gui_name = "Saltpeter";

	i = CONTENT_CRAFTITEM_GUNPOWDER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_GUNPOWDER;
	f->texture = "gunpowder.png";
	f->name = "gunpowder";
	f->gui_name = "Gun Powder";
	crafting::set1Any3Recipe(CONTENT_CRAFTITEM_CHARCOAL,CONTENT_CRAFTITEM_FLINT,CONTENT_CRAFTITEM_SALTPETER,CONTENT_CRAFTITEM_GUNPOWDER);

	i = CONTENT_CRAFTITEM_SNOW_BALL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_SNOW_BALL;
	f->texture = "snow_ball.png";
	f->name = "snow_ball";
	f->gui_name = "Snow Ball";

	i = CONTENT_CRAFTITEM_STICK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_STICK;
	f->texture = "stick.png";
	f->name = "Stick";
	f->gui_name = "Stick";
	f->fuel_time = 30/16;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_PINE_PLANK,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_LEAVES,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_APPLE_LEAVES,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_JUNGLELEAVES,CONTENT_CRAFTITEM_STICK);
	crafting::set1To2Recipe(CONTENT_CONIFER_LEAVES,CONTENT_CRAFTITEM_STICK);

	i = CONTENT_CRAFTITEM_PINE_PLANK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_PINE_PLANK;
	f->texture = "pine_plank.png";
	f->name = "pine_plank";
	f->gui_name = "Pine Plank";
	f->fuel_time = 30/16;
	crafting::set1To4Recipe(CONTENT_WOOD_PINE,CONTENT_CRAFTITEM_PINE_PLANK);

	i = CONTENT_CRAFTITEM_WOOD_PLANK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_WOOD_PLANK;
	f->texture = "wood_plank.png";
	f->name = "wood_plank";
	f->gui_name = "Wood Plank";
	f->fuel_time = 30/16;
	crafting::set1To4Recipe(CONTENT_WOOD,CONTENT_CRAFTITEM_WOOD_PLANK);

	i = CONTENT_CRAFTITEM_JUNGLE_PLANK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_JUNGLE_PLANK;
	f->texture = "jungle_plank.png";
	f->name = "jungle_plank";
	f->gui_name = "Jungle Wood Plank";
	f->fuel_time = 30/16;
	crafting::set1To4Recipe(CONTENT_JUNGLEWOOD,CONTENT_CRAFTITEM_JUNGLE_PLANK);

}
