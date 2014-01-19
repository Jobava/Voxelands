/************************************************************************
* content_toolitem.cpp
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

#include "content_toolitem.h"
#include "content_craftitem.h"
#include "content_craft.h"
#include "content_mapnode.h"
#include <map>

std::map<content_t,struct ToolItemFeatures> g_content_toolitem_features;

ToolItemFeatures & content_toolitem_features(content_t i)
{
	if ((i&CONTENT_TOOLITEM_MASK) != CONTENT_TOOLITEM_MASK)
		return g_content_toolitem_features[CONTENT_IGNORE];

	std::map<content_t,struct ToolItemFeatures>::iterator it = g_content_toolitem_features.find(i);
	if (it == g_content_toolitem_features.end())
		return g_content_toolitem_features[CONTENT_IGNORE];
	return it->second;
}

ToolItemFeatures & content_toolitem_features(std::string subname)
{
	for (std::map<content_t,struct ToolItemFeatures>::iterator i = g_content_toolitem_features.begin(); i!=g_content_toolitem_features.end(); i++) {
		if (i->second.name == subname)
			return i->second;
	}
	return g_content_toolitem_features[CONTENT_IGNORE];
}

DiggingProperties getDiggingProperties(content_t content, content_t tool)
{
	ToolItemFeatures t_features = content_toolitem_features(tool);
	ContentFeatures &c_features = content_features(content);
	f32 time = t_features.dig_time*c_features.hardness;
	f32 wear = 0;
	if (t_features.hardness && c_features.hardness)
		wear = 65535/t_features.hardness*c_features.hardness;
	f32 diggable = true;
	if (c_features.type == CMT_STONE && t_features.type != TT_PICK) {
		diggable = false;
	}else{
		switch (t_features.type) {
		case TT_SPECIAL:
			diggable = false;
			break;
		case TT_AXE:
			if (c_features.type != CMT_WOOD && c_features.type != CMT_PLANT)
				time *= 10.;
			break;
		case TT_PICK:
			if (c_features.type != CMT_STONE)
				time *= 2.;
			break;
		case TT_SHOVEL:
			if (c_features.type != CMT_DIRT)
				time *= 10.;
			break;
		case TT_SWORD:
		case TT_SHEAR:
			if (c_features.type != CMT_PLANT)
				time *= 10.;
			break;
		case TT_BUCKET:
			if (c_features.type != CMT_LIQUID)
				time = 10.;
			break;
		case TT_NONE:
		default:
			break;
		}
	}


	return DiggingProperties(diggable,time,wear);
}

void content_toolitem_init()
{
	g_content_toolitem_features.clear();

	content_t i;
	ToolItemFeatures *f = NULL;

	i = CONTENT_TOOLITEM_WPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodpick.png";
	f->name = "WPick";
	f->description = "Wooden Pick";
	f->type = TT_PICK;
	f->hardness = 30.;
	f->dig_time = 1.5;
	crafting::setPickRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WPICK);
	crafting::setPickRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WPICK);

	i = CONTENT_TOOLITEM_STPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stonepick.png";
	f->name = "STPick";
	f->description = "Stone Pick";
	f->type = TT_PICK;
	f->hardness = 100.;
	f->dig_time = 0.75;
	crafting::setPickRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STPICK);

	i = CONTENT_TOOLITEM_STEELPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelpick.png";
	f->name = "SteelPick";
	f->description = "Steel Pick";
	f->type = TT_PICK;
	f->hardness = 333.;
	f->dig_time = 0.5;
	crafting::setPickRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELPICK);

	i = CONTENT_TOOLITEM_MESEPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_mesepick.png";
	f->name = "MesePick";
	f->description = "Mese Pick";
	f->type = TT_PICK;
	f->hardness = 1337.;
	f->dig_time = 0.;
	crafting::setPickRecipe(CONTENT_MESE,CONTENT_TOOLITEM_MESEPICK);

	i = CONTENT_TOOLITEM_WSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodshovel.png";
	f->name = "WShovel";
	f->description = "Wooden Shovel";
	f->type = TT_SHOVEL;
	f->hardness = 50.;
	f->dig_time = 0.4;
	crafting::setShovelRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WSHOVEL);
	crafting::setShovelRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WSHOVEL);

	i = CONTENT_TOOLITEM_STSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stoneshovel.png";
	f->name = "STShovel";
	f->description = "Stone Shovel";
	f->type = TT_SHOVEL;
	f->hardness = 150.;
	f->dig_time = 0.2;
	crafting::setShovelRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STSHOVEL);

	i = CONTENT_TOOLITEM_STEELSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelshovel.png";
	f->name = "SteelShovel";
	f->description = "Steel Shovel";
	f->type = TT_SHOVEL;
	f->hardness = 400.;
	f->dig_time = 0.15;
	crafting::setShovelRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSHOVEL);

	i = CONTENT_TOOLITEM_WAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodaxe.png";
	f->name = "WAxe";
	f->description = "Wooden Axe";
	f->type = TT_AXE;
	f->hardness = 30.;
	f->dig_time = 1.5;
	crafting::setAxeRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WAXE);
	crafting::setAxeRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WAXE);

	i = CONTENT_TOOLITEM_STAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stoneaxe.png";
	f->name = "STAxe";
	f->description = "Stone Axe";
	f->type = TT_AXE;
	f->hardness = 100.;
	f->dig_time = 0.75;
	crafting::setAxeRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STAXE);

	i = CONTENT_TOOLITEM_STEELAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelaxe.png";
	f->name = "SteelAxe";
	f->description = "Steel Axe";
	f->type = TT_AXE;
	f->hardness = 333.;
	f->dig_time = 0.5;
	crafting::setAxeRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELAXE);

	i = CONTENT_TOOLITEM_WSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodsword.png";
	f->name = "WSword";
	f->description = "Wooden Sword";
	f->type = TT_SWORD;
	f->hardness = 20.;
	f->dig_time = 1.5;
	crafting::setSwordRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WSWORD);
	crafting::setSwordRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WSWORD);

	i = CONTENT_TOOLITEM_STSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stonesword.png";
	f->name = "STSword";
	f->description = "Stone Sword";
	f->type = TT_SWORD;
	f->hardness = 50.;
	f->dig_time = 1.0;
	crafting::setSwordRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STSWORD);

	i = CONTENT_TOOLITEM_STEELSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelsword.png";
	f->name = "SteelSword";
	f->description = "Steel Sword";
	f->type = TT_SWORD;
	f->hardness = 80.;
	f->dig_time = 0.5;
	crafting::setSwordRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSWORD);

	i = CONTENT_TOOLITEM_STEELSHEARS;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_shears.png";
	f->name = "Shears";
	f->description = "Steel Shears";
	f->type = TT_SHEAR;
	f->hardness = 333.;
	f->dig_time = 0.5;
	crafting::setShearsRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSHEARS);

	i = CONTENT_TOOLITEM_WBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodbucket.png";
	f->name = "WBucket";
	f->description = "Wooden Bucket";
	f->liquids_pointable = true;
	f->type = TT_BUCKET;
	f->hardness = 30.;
	f->dig_time = 0.0;
	crafting::setURecipe(CONTENT_CRAFTITEM_STICK,CONTENT_TOOLITEM_WBUCKET);

	i = CONTENT_TOOLITEM_TINBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_tinbucket.png";
	f->name = "TinBucket";
	f->description = "Tin Bucket";
	f->liquids_pointable = true;
	f->type = TT_BUCKET;
	f->hardness = 50.;
	f->dig_time = 0.0;
	crafting::setURecipe(CONTENT_CRAFTITEM_TIN_INGOT,CONTENT_TOOLITEM_TINBUCKET);

	i = CONTENT_TOOLITEM_WBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodbucket_water.png";
	f->name = "WBucket_water";
	f->description = "Wooden Bucket of Water";
	f->type = TT_SPECIAL;

	i = CONTENT_TOOLITEM_TINBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_tinbucket_water.png";
	f->name = "TinBucket_water";
	f->description = "Tin Bucket of Water";
	f->type = TT_SPECIAL;

	i = CONTENT_TOOLITEM_STEELBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket.png";
	f->name = "SteelBucket";
	f->description = "Steel Bucket";
	f->liquids_pointable = true;
	f->type = TT_BUCKET;
	f->hardness = 80.;
	f->dig_time = 0.0;
	crafting::setURecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELBUCKET);

	i = CONTENT_TOOLITEM_STEELBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket_water.png";
	f->name = "SteelBucket_water";
	f->description = "Steel Bucket of Water";
	f->type = TT_SPECIAL;

	i = CONTENT_TOOLITEM_STEELBUCKET_LAVA;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket_lava.png";
	f->name = "SteelBucket_lava";
	f->description = "Steel Bucket of Lava";
	f->fuel_time = 80;
	f->type = TT_SPECIAL;

	i = CONTENT_TOOLITEM_FIRESTARTER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_fire_starter.png";
	f->name = "FireStarter";
	f->description = "Fire Starter";
	f->liquids_pointable = true;
	f->type = TT_SPECIAL;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FLINT,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_FIRESTARTER);

	i = CONTENT_TOOLITEM_CROWBAR;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "crowbar.png";
	f->name = "crowbar";
	f->description = "Crowbar";
	f->type = TT_SPECIAL;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_CROWBAR);
}
