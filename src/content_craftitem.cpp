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

	i = CONTENT_CRAFTITEM_BOOK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_BOOK;
	f->texture = "book.png";
	f->name = "book";
	f->gui_name = "Book";

	i = CONTENT_CRAFTITEM_LUMP_OF_CHARCOAL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_CHARCOAL;
	f->texture = "lump_of_charcoal.png";
	f->name = "lump_of_charcoal";
	f->gui_name = "Charcoal Lump";

	i = CONTENT_CRAFTITEM_LUMP_OF_COAL;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_COAL;
	f->texture = "lump_of_coal.png";
	f->name = "lump_of_coal";
	f->gui_name = "Coal Lump";

	i = CONTENT_CRAFTITEM_LUMP_OF_IRON;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_IRON;
	f->texture = "lump_of_iron.png";
	f->name = "lump_of_iron";
	f->gui_name = "Iron Lump";
	f->cook_result = "CraftItem steel_ingot 1";

	i = CONTENT_CRAFTITEM_LUMP_OF_CLAY;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_CLAY;
	f->texture = "lump_of_clay.png";
	f->name = "lump_of_clay";
	f->gui_name = "Clay Lump";
	f->cook_result = "CraftItem clay_brick 1";

	i = CONTENT_CRAFTITEM_LUMP_OF_TIN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_TIN;
	f->texture = "lump_of_tin.png";
	f->name = "lump_of_tin";
	f->gui_name = "Tin Lump";
	f->cook_result = "CraftItem tin_ingot 1";

	i = CONTENT_CRAFTITEM_LUMP_OF_COPPER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_COPPER;
	f->texture = "lump_of_copper.png";
	f->name = "lump_of_copper";
	f->gui_name = "Copper Lump";
	f->cook_result = "CraftItem copper_ingot 1";

	i = CONTENT_CRAFTITEM_LUMP_OF_SILVER;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_SILVER;
	f->texture = "lump_of_silver.png";
	f->name = "lump_of_silver";
	f->gui_name = "Silver Lump";
	f->cook_result = "CraftItem silver_ingot 1";

	i = CONTENT_CRAFTITEM_LUMP_OF_GOLD;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_GOLD;
	f->texture = "lump_of_gold.png";
	f->name = "lump_of_gold";
	f->gui_name = "Gold Lump";
	f->cook_result = "CraftItem gold_ingot 1";

	i = CONTENT_CRAFTITEM_LUMP_OF_QUARTZ;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_QUARTZ;
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

	i = CONTENT_CRAFTITEM_LUMP_OF_FLINT;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_LUMP_OF_FLINT;
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
	f->texture = "apple.png";
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

	i = CONTENT_CRAFTITEM_DYE_BLUE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_BLUE;
	f->texture = "dye_blue.png";
	f->name = "dye_blue";
	f->gui_name = "Blue Dye";

	i = CONTENT_CRAFTITEM_DYE_GREEN;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_GREEN;
	f->texture = "dye_green.png";
	f->name = "dye_green";
	f->gui_name = "Green Dye";

	i = CONTENT_CRAFTITEM_DYE_ORANGE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_ORANGE;
	f->texture = "dye_orange.png";
	f->name = "dye_orange";
	f->gui_name = "Orange Dye";

	i = CONTENT_CRAFTITEM_DYE_PURPLE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_PURPLE;
	f->texture = "dye_purple.png";
	f->name = "dye_purple";
	f->gui_name = "Purple Dye";

	i = CONTENT_CRAFTITEM_DYE_RED;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_RED;
	f->texture = "dye_red.png";
	f->name = "dye_red";
	f->gui_name = "Red Dye";

	i = CONTENT_CRAFTITEM_DYE_YELLOW;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_YELLOW;
	f->texture = "dye_yellow.png";
	f->name = "dye_yellow";
	f->gui_name = "Yellow Dye";

	i = CONTENT_CRAFTITEM_DYE_WHITE;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_WHITE;
	f->texture = "dye_white.png";
	f->name = "dye_white";
	f->gui_name = "White Dye";

	i = CONTENT_CRAFTITEM_DYE_BLACK;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_DYE_BLACK;
	f->texture = "dye_black.png";
	f->name = "dye_black";
	f->gui_name = "Black Dye";

	i = CONTENT_CRAFTITEM_QUARTZ_DUST;
	f = &g_content_craftitem_features[i];
	f->content = CONTENT_CRAFTITEM_QUARTZ_DUST;
	f->texture = "quartz_dust.png";
	f->name = "quartz_dust";
	f->gui_name = "Quartz Dust";

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

}
