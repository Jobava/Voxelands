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

#include "content_inventory.h"
#include "inventory.h"
#include "content_mapnode.h"
//#include "serverobject.h"
#include "content_sao.h"

std::string item_craft_get_image_name(const std::string &subname)
{
	if (subname == "Stick") {
		return "stick.png";
	}else if (subname == "paper") {
		return "paper.png";
	}else if (subname == "book") {
		return "book.png";
	}else if (subname == "lump_of_charcoal") {
		return "lump_of_charcoal.png";
	}else if (subname == "lump_of_coal") {
		return "lump_of_coal.png";
	}else if (subname == "lump_of_iron") {
		return "lump_of_iron.png";
	}else if (subname == "lump_of_clay") {
		return "lump_of_clay.png";
	}else if (subname == "lump_of_tin") {
		return "lump_of_tin.png";
	}else if (subname == "lump_of_copper") {
		return "lump_of_copper.png";
	}else if (subname == "lump_of_silver") {
		return "lump_of_silver.png";
	}else if (subname == "lump_of_gold") {
		return "lump_of_gold.png";
	}else if (subname == "lump_of_quartz") {
		return "lump_of_quartz.png";
	}else if (subname == "lump_of_flint") {
		return "lump_of_flint.png";
	}else if (subname == "steel_ingot") {
		return "steel_ingot.png";
	}else if (subname == "tin_ingot") {
		return "tin_ingot.png";
	}else if (subname == "copper_ingot") {
		return "copper_ingot.png";
	}else if (subname == "silver_ingot") {
		return "silver_ingot.png";
	}else if (subname == "gold_ingot") {
		return "gold_ingot.png";
	}else if (subname == "clay_brick") {
		return "clay_brick.png";
	}else if (subname == "rat") {
		return "rat.png";
	}else if (subname == "cooked_rat") {
		return "cooked_rat.png";
	}else if (subname == "scorched_stuff") {
		return "scorched_stuff.png";
	}else if (subname == "firefly") {
		return "firefly.png";
	}else if (subname == "apple") {
		return "apple.png^[forcesingle";
	}else if (subname == "apple_iron") {
		return "apple_iron.png";
	}else if (subname == "dye_blue") {
		return "dye_blue.png";
	}else if (subname == "dye_green") {
		return "dye_green.png";
	}else if (subname == "dye_orange") {
		return "dye_orange.png";
	}else if (subname == "dye_purple") {
		return "dye_purple.png";
	}else if (subname == "dye_red") {
		return "dye_red.png";
	}else if (subname == "dye_yellow") {
		return "dye_yellow.png";
	}else if (subname == "dye_white") {
		return "dye_white.png";
	}else if (subname == "dye_black") {
		return "dye_black.png";
	}else if (subname == "quartz_dust") {
		return "quartz_dust.png";
	}else if (subname == "saltpeter") {
		return "saltpeter.png";
	}else if (subname == "gunpowder") {
		return "gunpowder.png";
	}else if (subname == "snow_ball") {
		return "snow_ball.png";
	}else{
		return "cotton.png"; // just something
	}
}

std::string item_craft_get_name(const std::string &subname)
{
	if (subname == "paper") {
		return "Paper";
	}else if (subname == "book") {
		return "Book";
	}else if (subname == "lump_of_charcoal") {
		return "Charcoal Lump";
	}else if (subname == "lump_of_coal") {
		return "Coal Lump";
	}else if (subname == "lump_of_iron") {
		return "Iron Ore";
	}else if (subname == "lump_of_clay") {
		return "Clay Lump";
	}else if (subname == "lump_of_tin") {
		return "Tin Ore";
	}else if (subname == "lump_of_copper") {
		return "Copper Ore";
	}else if (subname == "lump_of_silver") {
		return "Silver Ore";
	}else if (subname == "lump_of_gold") {
		return "Gold Ore";
	}else if (subname == "lump_of_quartz") {
		return "Quartz Crystal";
	}else if (subname == "tin_ingot") {
		return "Tin Ingot";
	}else if (subname == "copper_ingot") {
		return "Copper Ingot";
	}else if (subname == "silver_ingot") {
		return "Silver Ingot";
	}else if (subname == "gold_ingot") {
		return "Gold Ingot";
	}else if (subname == "lump_of_flint") {
		return "Flint";
	}else if (subname == "steel_ingot") {
		return "Steel Ingot";
	}else if (subname == "clay_brick") {
		return "Clay Brick";
	}else if (subname == "rat") {
		return "Rat";
	}else if (subname == "cooked_rat") {
		return "Cooked Rat";
	}else if (subname == "scorched_stuff") {
		return "Scorched Stuff";
	}else if (subname == "firefly") {
		return "Firefly";
	}else if (subname == "apple") {
		return "Apple";
	}else if (subname == "apple_iron") {
		return "Iron Apple";
	}else if (subname == "dye_blue") {
		return "Blue Dye";
	}else if (subname == "dye_green") {
		return "Green Dye";
	}else if (subname == "dye_orange") {
		return "Orange Dye";
	}else if (subname == "dye_purple") {
		return "Purple Dye";
	}else if (subname == "dye_red") {
		return "Red Dye";
	}else if (subname == "dye_yellow") {
		return "Yellow Dye";
	}else if (subname == "dye_white") {
		return "White Dye";
	}else if (subname == "dye_black") {
		return "Black Dye";
	}else if (subname == "quartz_dust") {
		return "Quartz Dust";
	}else if (subname == "saltpeter") {
		return "Salt Peter";
	}else if (subname == "gunpowder") {
		return "Gun Powder";
	}else if (subname == "snow_ball") {
		return "Snow Ball";
	}

	return subname;
}

ServerActiveObject* item_craft_create_object(const std::string &subname,
		ServerEnvironment *env, u16 id, v3f pos)
{
	if(subname == "rat")
	{
		ServerActiveObject *obj = new RatSAO(env, id, pos);
		return obj;
	}
	else if(subname == "firefly")
	{
		ServerActiveObject *obj = new FireflySAO(env, id, pos);
		return obj;
	}

	return NULL;
}

s16 item_craft_get_drop_count(const std::string &subname)
{
	if(subname == "rat" || subname == "firefly")
		return 1;

	return -1;
}

bool item_craft_is_cookable(const std::string &subname)
{
	if (
		subname == "lump_of_iron"
		|| subname == "lump_of_tin"
		|| subname == "lump_of_copper"
		|| subname == "lump_of_silver"
		|| subname == "lump_of_gold"
		|| subname == "lump_of_clay"
		|| subname == "rat"
		|| subname == "cooked_rat"
	)
		return true;

	return false;
}

InventoryItem* item_craft_create_cook_result(const std::string &subname)
{
	if (subname == "lump_of_iron") {
		return new CraftItem("steel_ingot", 1);
	}else if (subname == "lump_of_tin") {
		return new CraftItem("tin_ingot", 1);
	}else if (subname == "lump_of_copper") {
		return new CraftItem("copper_ingot", 1);
	}else if (subname == "lump_of_silver") {
		return new CraftItem("silver_ingot", 1);
	}else if (subname == "lump_of_gold") {
		return new CraftItem("gold_ingot", 1);
	}else if (subname == "lump_of_clay") {
		return new CraftItem("clay_brick", 1);
	}else if (subname == "rat") {
		return new CraftItem("cooked_rat", 1);
	}else if (subname == "cooked_rat") {
		return new CraftItem("scorched_stuff", 1);
	}

	return NULL;
}

bool item_craft_is_eatable(const std::string &subname)
{
	if (
		subname == "cooked_rat"
		|| subname == "apple"
		|| subname == "apple_iron"
	)
		return true;
	return false;
}

s16 item_craft_eat_hp_change(const std::string &subname)
{
	if (subname == "cooked_rat") {
		return 6; // 3 hearts
	}else if (subname == "apple") {
		return 4; // 2 hearts
	}else if (subname == "apple_iron") {
		return 8; // 4 hearts
	}
	return 0;
}


