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
	f->gui_name = "Wooden Pick";
	crafting::setPickRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WPICK);
	crafting::setPickRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WPICK);

	i = CONTENT_TOOLITEM_STPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stonepick.png";
	f->name = "STPick";
	f->gui_name = "Stone Pick";
	crafting::setPickRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STPICK);

	i = CONTENT_TOOLITEM_STEELPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelpick.png";
	f->name = "SteelPick";
	f->gui_name = "Steel Pick";
	crafting::setPickRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELPICK);

	i = CONTENT_TOOLITEM_MESEPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_mesepick.png";
	f->name = "MesePick";
	f->gui_name = "Mese Pick";
	crafting::setPickRecipe(CONTENT_MESE,CONTENT_TOOLITEM_MESEPICK);

	i = CONTENT_TOOLITEM_WSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodshovel.png";
	f->name = "WShovel";
	f->gui_name = "Wooden Shovel";
	crafting::setShovelRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WSHOVEL);
	crafting::setShovelRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WSHOVEL);

	i = CONTENT_TOOLITEM_STSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stoneshovel.png";
	f->name = "STShovel";
	f->gui_name = "Stone Shovel";
	crafting::setShovelRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STSHOVEL);

	i = CONTENT_TOOLITEM_STEELSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelshovel.png";
	f->name = "SteelShovel";
	f->gui_name = "Steel Shovel";
	crafting::setShovelRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSHOVEL);

	i = CONTENT_TOOLITEM_WAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodaxe.png";
	f->name = "WAxe";
	f->gui_name = "Wooden Axe";
	crafting::setAxeRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WAXE);
	crafting::setAxeRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WAXE);

	i = CONTENT_TOOLITEM_STAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stoneaxe.png";
	f->name = "STAxe";
	f->gui_name = "Stone Axe";
	crafting::setAxeRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STAXE);

	i = CONTENT_TOOLITEM_STEELAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelaxe.png";
	f->name = "SteelAxe";
	f->gui_name = "Steel Axe";
	crafting::setAxeRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELAXE);

	i = CONTENT_TOOLITEM_WSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodsword.png";
	f->name = "WSword";
	f->gui_name = "Wooden Sword";
	crafting::setSwordRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WSWORD);
	crafting::setSwordRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WSWORD);

	i = CONTENT_TOOLITEM_STSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stonesword.png";
	f->name = "STSword";
	f->gui_name = "Stone Sword";
	crafting::setSwordRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STSWORD);

	i = CONTENT_TOOLITEM_STEELSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelsword.png";
	f->name = "SteelSword";
	f->gui_name = "Steel Sword";
	crafting::setSwordRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSWORD);

	i = CONTENT_TOOLITEM_STEELSHEARS;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_shears.png";
	f->name = "Shears";
	f->gui_name = "Steel Shears";
	crafting::setShearsRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSHEARS);

	i = CONTENT_TOOLITEM_WBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodbucket.png";
	f->name = "WBucket";
	f->gui_name = "Wooden Bucket";
	f->liquids_pointable = true;
	crafting::setURecipe(CONTENT_CRAFTITEM_STICK,CONTENT_TOOLITEM_WBUCKET);

	i = CONTENT_TOOLITEM_TINBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_tinbucket.png";
	f->name = "TinBucket";
	f->gui_name = "Tin Bucket";
	f->liquids_pointable = true;
	crafting::setURecipe(CONTENT_CRAFTITEM_TIN_INGOT,CONTENT_TOOLITEM_TINBUCKET);

	i = CONTENT_TOOLITEM_WBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodbucket_water.png";
	f->name = "WBucket_water";
	f->gui_name = "Wooden Bucket of Water";

	i = CONTENT_TOOLITEM_TINBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_tinbucket_water.png";
	f->name = "TinBucket_water";
	f->gui_name = "Tin Bucket of Water";

	i = CONTENT_TOOLITEM_STEELBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket.png";
	f->name = "SteelBucket";
	f->gui_name = "Steel Bucket";
	f->liquids_pointable = true;
	crafting::setURecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELBUCKET);

	i = CONTENT_TOOLITEM_STEELBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket_water.png";
	f->name = "SteelBucket_water";
	f->gui_name = "Steel Bucket of Water";

	i = CONTENT_TOOLITEM_STEELBUCKET_LAVA;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket_lava.png";
	f->name = "SteelBucket_lava";
	f->gui_name = "Steel Bucket of Lava";
	f->fuel_time = 80;

	i = CONTENT_TOOLITEM_FIRESTARTER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_fire_starter.png";
	f->name = "FireStarter";
	f->gui_name = "Fire Starter";
	f->liquids_pointable = true;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FLINT,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_FIRESTARTER);

	i = CONTENT_TOOLITEM_CROWBAR;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "crowbar.png";
	f->name = "crowbar";
	f->gui_name = "Crowbar";
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_CROWBAR);
}
