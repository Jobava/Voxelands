/************************************************************************
* content_toolitem.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa Milne 2014 <lisa@ltmnet.com>
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
#include "content_list.h"
#include "content_mapnode.h"
#include <map>
#include "gettext.h"

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
			if (c_features.type == CMT_PLANT) {
				time *= 2.;
			}else if (c_features.type != CMT_WOOD) {
				time *= 10.;
			}
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
			if (c_features.type != CMT_PLANT)
				time = 10.;
			break;
		case TT_SHEAR:
			if (c_features.type != CMT_PLANT)
				time *= 10.;
			break;
		case TT_BUCKET:
			if (c_features.type != CMT_LIQUID)
				time = 10.;
			break;
		case TT_SPEAR:
			if (c_features.type != CMT_DIRT)
				time = 10.;
			break;
		case TT_NONE:
			if (c_features.type == CMT_DIRT)
				time *= 0.75;
			break;
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
	f->description = wgettext("Wooden Pick");
	f->type = TT_PICK;
	f->hardness = 30.;
	f->dig_time = 1.5;
	crafting::setPickRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WPICK);
	crafting::setPickRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WPICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stonepick.png";
	f->name = "STPick";
	f->description = wgettext("Stone Pick");
	f->type = TT_PICK;
	f->hardness = 100.;
	f->dig_time = 0.75;
	crafting::setPickRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STPICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_FLINTPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_flintpick.png";
	f->name = "FPick";
	f->description = wgettext("Flint Pick");
	f->type = TT_PICK;
	f->hardness = 180.;
	f->dig_time = 1.2;
	crafting::setPickRecipe(CONTENT_CRAFTITEM_FLINT,CONTENT_TOOLITEM_FLINTPICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelpick.png";
	f->name = "SteelPick";
	f->description = wgettext("Steel Pick");
	f->type = TT_PICK;
	f->hardness = 333.;
	f->dig_time = 0.5;
	crafting::setPickRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELPICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_MESEPICK;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_mesepick.png";
	f->name = "MesePick";
	f->description = wgettext("Mese Pick");
	f->type = TT_PICK;
	f->hardness = 1337.;
	f->dig_time = 0.;
	crafting::setPickRecipe(CONTENT_MESE,CONTENT_TOOLITEM_MESEPICK);
	lists::add("craftguide",i);
	lists::add("player-creative",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_WSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodshovel.png";
	f->name = "WShovel";
	f->description = wgettext("Wooden Shovel");
	f->type = TT_SHOVEL;
	f->hardness = 50.;
	f->dig_time = 0.4;
	crafting::setShovelRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WSHOVEL);
	crafting::setShovelRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WSHOVEL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stoneshovel.png";
	f->name = "STShovel";
	f->description = wgettext("Stone Shovel");
	f->type = TT_SHOVEL;
	f->hardness = 150.;
	f->dig_time = 0.2;
	crafting::setShovelRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STSHOVEL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_FLINTSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_flintshovel.png";
	f->name = "FShovel";
	f->description = wgettext("Flint Shovel");
	f->type = TT_SHOVEL;
	f->hardness = 250.;
	f->dig_time = 0.17;
	crafting::setShovelRecipe(CONTENT_CRAFTITEM_FLINT,CONTENT_TOOLITEM_FLINTSHOVEL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELSHOVEL;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelshovel.png";
	f->name = "SteelShovel";
	f->description = wgettext("Steel Shovel");
	f->type = TT_SHOVEL;
	f->hardness = 400.;
	f->dig_time = 0.15;
	crafting::setShovelRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSHOVEL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_WAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodaxe.png";
	f->name = "WAxe";
	f->description = wgettext("Wooden Axe");
	f->type = TT_AXE;
	f->hardness = 30.;
	f->dig_time = 1.5;
	crafting::setAxeRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WAXE);
	crafting::setAxeRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WAXE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stoneaxe.png";
	f->name = "STAxe";
	f->description = wgettext("Stone Axe");
	f->type = TT_AXE;
	f->hardness = 100.;
	f->dig_time = 0.75;
	crafting::setAxeRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STAXE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_FLINTAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_flintaxe.png";
	f->name = "FAxe";
	f->description = wgettext("Flint Axe");
	f->type = TT_AXE;
	f->hardness = 180.;
	f->dig_time = 0.65;
	crafting::setAxeRecipe(CONTENT_CRAFTITEM_FLINT,CONTENT_TOOLITEM_FLINTAXE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELAXE;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelaxe.png";
	f->name = "SteelAxe";
	f->description = wgettext("Steel Axe");
	f->type = TT_AXE;
	f->hardness = 333.;
	f->dig_time = 0.5;
	crafting::setAxeRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELAXE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_WSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodsword.png";
	f->name = "WSword";
	f->description = wgettext("Wooden Sword");
	f->type = TT_SWORD;
	f->hardness = 120.;
	f->dig_time = 1.5;
	crafting::setSwordRecipe(CONTENT_WOOD,CONTENT_TOOLITEM_WSWORD);
	crafting::setSwordRecipe(CONTENT_JUNGLEWOOD,CONTENT_TOOLITEM_WSWORD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stonesword.png";
	f->name = "STSword";
	f->description = wgettext("Stone Sword");
	f->type = TT_SWORD;
	f->hardness = 300.;
	f->dig_time = 1.0;
	crafting::setSwordRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STSWORD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELSWORD;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelsword.png";
	f->name = "SteelSword";
	f->description = wgettext("Steel Sword");
	f->type = TT_SWORD;
	f->hardness = 380.;
	f->dig_time = 0.5;
	crafting::setSwordRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSWORD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_FLINTSHEARS;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_flintshears.png";
	f->name = "FShears";
	f->description = wgettext("Flint Shears");
	f->type = TT_SHEAR;
	f->hardness = 200.;
	f->dig_time = 0.7;
	crafting::setShearsRecipe(CONTENT_CRAFTITEM_FLINT,CONTENT_TOOLITEM_FLINTSHEARS);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELSHEARS;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelshears.png";
	f->name = "Shears";
	f->description = wgettext("Steel Shears");
	f->type = TT_SHEAR;
	f->hardness = 333.;
	f->dig_time = 0.5;
	crafting::setShearsRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSHEARS);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_WBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodbucket.png";
	f->name = "WBucket";
	f->description = wgettext("Wooden Bucket");
	f->liquids_pointable = true;
	f->type = TT_BUCKET;
	f->hardness = 30.;
	f->dig_time = 0.0;
	crafting::setURecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_TOOLITEM_WBUCKET);
	crafting::setURecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_TOOLITEM_WBUCKET);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_TINBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_tinbucket.png";
	f->name = "TinBucket";
	f->description = wgettext("Tin Bucket");
	f->liquids_pointable = true;
	f->type = TT_BUCKET;
	f->hardness = 50.;
	f->dig_time = 0.0;
	crafting::setURecipe(CONTENT_CRAFTITEM_TIN_INGOT,CONTENT_TOOLITEM_TINBUCKET);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_WBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_woodbucket_water.png";
	f->name = "WBucket_water";
	f->description = wgettext("Wooden Bucket of Water");
	f->type = TT_SPECIAL;
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_TINBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_tinbucket_water.png";
	f->name = "TinBucket_water";
	f->description = wgettext("Tin Bucket of Water");
	f->type = TT_SPECIAL;
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELBUCKET;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket.png";
	f->name = "SteelBucket";
	f->description = wgettext("Steel Bucket");
	f->liquids_pointable = true;
	f->type = TT_BUCKET;
	f->hardness = 80.;
	f->dig_time = 0.0;
	crafting::setURecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELBUCKET);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELBUCKET_WATER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket_water.png";
	f->name = "SteelBucket_water";
	f->description = wgettext("Steel Bucket of Water");
	f->type = TT_SPECIAL;
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELBUCKET_LAVA;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelbucket_lava.png";
	f->name = "SteelBucket_lava";
	f->description = wgettext("Steel Bucket of Lava");
	f->fuel_time = 80;
	f->type = TT_SPECIAL;
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_FIRESTARTER;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_fire_starter.png";
	f->name = "FireStarter";
	f->description = wgettext("Fire Starter");
	f->liquids_pointable = true;
	f->type = TT_SPECIAL;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_FLINT,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_FIRESTARTER);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_CROWBAR;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "crowbar.png";
	f->name = "crowbar";
	f->description = wgettext("Crowbar");
	f->type = TT_SPECIAL;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_CROWBAR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_KEY;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "key.png";
	f->name = "key";
	f->description = wgettext("Key");
	f->type = TT_SPECIAL;
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_GOLD_INGOT,CONTENT_TOOLITEM_KEY);
	lists::add("craftguide",i);

	i = CONTENT_TOOLITEM_STONESPEAR;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_stonespear.png";
	f->name = "stone_spear";
	f->description = wgettext("Stone Spear");
	f->type = TT_SPEAR;
	f->hardness = 100.;
	f->dig_time = 1.5;
	crafting::setSpearRecipe(CONTENT_ROUGHSTONE,CONTENT_TOOLITEM_STONESPEAR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_FLINTSPEAR;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_flintspear.png";
	f->name = "flint_spear";
	f->description = wgettext("Flint Spear");
	f->type = TT_SPEAR;
	f->hardness = 200.;
	f->dig_time = 1.0;
	crafting::setSpearRecipe(CONTENT_CRAFTITEM_FLINT,CONTENT_TOOLITEM_FLINTSPEAR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TOOLITEM_STEELSPEAR;
	f = &g_content_toolitem_features[i];
	f->content = i;
	f->texture = "tool_steelspear.png";
	f->name = "steel_spear";
	f->description = wgettext("Steel Spear");
	f->type = TT_SPEAR;
	f->hardness = 300.;
	f->dig_time = 0.5;
	crafting::setSpearRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_TOOLITEM_STEELSPEAR);
	lists::add("craftguide",i);
	lists::add("creative",i);
}
