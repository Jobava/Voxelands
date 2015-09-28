/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_craft.cpp
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

#include "content_craft.h"
#include "inventory.h"
#include "content_mapnode.h"
#include "content_craftitem.h"
#include "content_clothesitem.h"
#include "content_toolitem.h"
#include "content_list.h"
#include "player.h"
#include "mapnode.h" // For content_t
#include "settings.h" // for g_settings

#include <algorithm>
#include <set>

namespace crafting {

std::vector<CraftDef> shaped_recipes;
std::vector<CraftDefShapeless> shapeless_recipes;

void initCrafting()
{
	shaped_recipes.clear();
	shapeless_recipes.clear();
}

static bool checkRecipe(content_t recipe[9], content_t result)
{
	for (std::vector<CraftDef>::iterator i=shaped_recipes.begin(); i!=shaped_recipes.end(); i++) {
		CraftDef d = *i;
		if (d.result == result && d == recipe)
			return true;
	}
	return false;
}

static bool checkShapelessRecipe(content_t recipe[9], content_t result)
{
	for (std::vector<CraftDefShapeless>::iterator i=shapeless_recipes.begin(); i!=shapeless_recipes.end(); i++) {
		CraftDefShapeless d = *i;
		if (d.result == result && d == recipe)
			return true;
	}
	return false;
}

void setRecipe(content_t recipe[9], content_t result, u16 count)
{
	if (checkRecipe(recipe,result))
		return;
	CraftDef d;
	for (int i=0; i<9; i++) {
		d.recipe[i] = recipe[i];
	}
	d.result = result;
	d.result_count = count;
	shaped_recipes.push_back(d);
}

void setShapelessRecipe(content_t recipe[9], content_t result, u16 count)
{
	if (checkShapelessRecipe(recipe,result))
		return;
	CraftDefShapeless d;
	for (int i=0; i<9; i++) {
		d.recipe[i] = recipe[i];
	}
	d.result = result;
	d.result_count = count;
	shapeless_recipes.push_back(d);
}

// one input yields one result
void set1To1Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// one input yields two result
void set1To2Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,2);
}
// one input yields four result
void set1To4Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,4);
}
// one input1 over one input2 yields one result
void set1over1Recipe(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		CONTENT_IGNORE,	CONTENT_IGNORE,
		input2,		CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// one input1 over one input2 yields two result
void set1over2Recipe(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		CONTENT_IGNORE,	CONTENT_IGNORE,
		input2,		CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,2);
}
// one input1 over one input2 yields four result
void set1over4Recipe(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		CONTENT_IGNORE,	CONTENT_IGNORE,
		input2,		CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,4);
}
// four input in a square yields one result
void setSoftBlockRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		CONTENT_IGNORE,
		input,		input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// four input in a square yields four result
void setBlockRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		CONTENT_IGNORE,
		input,		input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,4);
}
// nine input yields one result
void setHardBlockRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		input,		input,		input,
		input,		input,		input
	};
	setRecipe(r,result,1);
}
// one input yields four result
void setUncraftBlockRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,4);
}
// one input yields nine result
void setUncraftHardBlockRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,9);
}
// four input in a Z yields four result
void setBrickRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,		input,
		input,		input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,4);
	r[0] = input;
	r[2] = CONTENT_IGNORE;
	r[3] = CONTENT_IGNORE;
	r[5] = input;
	setRecipe(r,result,4);
}
// four input1 surround one input2 yields four result
void setSurroundRecipe(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input1,	CONTENT_IGNORE,
		input1,		input2,	input1,
		CONTENT_IGNORE,	input1,	CONTENT_IGNORE
	};
	setRecipe(r,result,4);
}
// 5 input in an X yields five result
void set5Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	input,
		CONTENT_IGNORE,	input,		CONTENT_IGNORE,
		input,		CONTENT_IGNORE,	input
	};
	setRecipe(r,result,5);
}
// three input in a v yields count result
void setVRecipe(u16 input, u16 result, u16 count)
{
	u16 r[9] = {
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		input,		CONTENT_IGNORE,	input,
		CONTENT_IGNORE,	input,	CONTENT_IGNORE
	};
	setRecipe(r,result,count);
}
// five input in a u yields one result
void setURecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		input,		CONTENT_IGNORE,	input,
		input,		input,		input
	};
	setRecipe(r,result,1);
}
// seven input in a U yields one result
void setDeepURecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	input,
		input,		CONTENT_IGNORE,	input,
		input,		input,		input
	};
	setRecipe(r,result,1);
}
// three input in a horizontal row yields one result
void setRow1Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// three input in a horizontal row yields two result
void setRow2Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,2);
}
// three input in a horizontal row yields three result
void setRow3Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,3);
}
// three input in a vertical row yields one result
void setCol1Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	input,		CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// three input in a vertical row yields two result
void setCol2Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	input,		CONTENT_IGNORE
	};
	setRecipe(r,result,2);
}
// three input in a vertical row yields three result
void setCol3Recipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	input,		CONTENT_IGNORE
	};
	setRecipe(r,result,3);
}
// six input as a stair yields six result
void setStairRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	CONTENT_IGNORE,
		input,		input,		CONTENT_IGNORE,
		input,		input,		input
	};
	setRecipe(r,result,6);
	u16 r1[9] = {
		CONTENT_IGNORE,	CONTENT_IGNORE,	input,
		CONTENT_IGNORE,	input,		input,
		input,		input,		input
	};
	setRecipe(r1,result,6);
}
// two input in a horizontal row yields three result
void setTileRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,3);
}
// eight input in a circle yields one result
void setRoundRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		input,		CONTENT_IGNORE,	input,
		input,		input,		input
	};
	setRecipe(r,result,1);
}
// eight input1 surrounding one input2 yields one result
void setFilledRoundRecipe(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		input1,		input1,
		input1,		input2,		input1,
		input1,		input1,		input1
	};
	setRecipe(r,result,1);
}
// six input in two horizontal rows yields six result
void setWallRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		input,		input,		input,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,6);
}
// one each of inputs shapeless yields 1 result
void set1Any2Recipe(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		input2,		CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,1);
}
// one each of inputs shapeless yields 2 result
void set2Any2Recipe(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		input2,		CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,2);
}
// one each of inputs shapeless yields 3 result
void set1Any3Recipe(u16 input1, u16 input2, u16 input3, u16 result)
{
	u16 r[9] = {
		input1,		input2,		input3,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,1);
}
// one each of inputs shapeless yields 2 result
void set2Any3Recipe(u16 input1, u16 input2, u16 input3, u16 result)
{
	u16 r[9] = {
		input1,		input2,		input3,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,2);
}
// one each of inputs shapeless yields 3 result
void set3Any3Recipe(u16 input1, u16 input2, u16 input3, u16 result)
{
	u16 r[9] = {
		input1,		input2,		input3,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,3);
}
// special shortcuts
// bed recipe, input is bottom two cotton yields one result
void setBedRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_COTTON,	CONTENT_CRAFTITEM_PINE_PLANK,
		CONTENT_WOOD_PINE,		input,		CONTENT_WOOD_PINE,
		CONTENT_CRAFTITEM_PINE_PLANK,	input,		CONTENT_CRAFTITEM_PINE_PLANK
	};
	setRecipe(r,result,1);
}
// sign recipe, input is top six wood yields one result
void setSignRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,				input,
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
	r[7] = CONTENT_CRAFTITEM_PINE_PLANK;
	setRecipe(r,result,1);
	r[7] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
	setRecipe(r,result,1);
}
// shears recipe, input is blade yields one result
void setShearsRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,				CONTENT_IGNORE,	input,
		CONTENT_IGNORE,			input,		CONTENT_IGNORE,
		CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK
	};
	setRecipe(r,result,0);
}
// shovel recipe, input is blade yields one result
void setShovelRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,0);
	u16 r1[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,0);
	u16 r2[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r2,result,0);
}
// spear recipe, input is blade yields one result
void setSpearRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE
	};
	setRecipe(r,result,0);
}
// axe recipe, input is blade yields one result
void setAxeRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,				CONTENT_IGNORE,
		input,		CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,0);
	u16 r1[9] = {
		CONTENT_IGNORE,	input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,0);
	u16 r2[9] = {
		input,		input,				CONTENT_IGNORE,
		input,		CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r2,result,0);
	u16 r3[9] = {
		CONTENT_IGNORE,	input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r3,result,0);
	u16 r4[9] = {
		input,		input,				CONTENT_IGNORE,
		input,		CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r4,result,0);
	u16 r5[9] = {
		CONTENT_IGNORE,	input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r5,result,0);
}
// pick recipe, input is blade yields one result
void setPickRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,0);
	u16 r1[9] = {
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,0);
	u16 r2[9] = {
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r2,result,0);
}
// sword recipe, input is blade yields one result
void setSwordRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,0);
	u16 r1[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,0);
	u16 r2[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r2,result,0);
}
// gate recipe 1 2 1 in two rows yields one result
void setGateRecipe(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		input2,		input1,
		input1,		input2,		input1,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// pants recipe 7 input in an upside-down U yields one result
void setPantsRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		input,		CONTENT_IGNORE,	input,
		input,		CONTENT_IGNORE,	input
	};
	setRecipe(r,result,1);
}
// shirt recipe 5 input in a T yields one result
void setShirtRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,	input,
		CONTENT_IGNORE,	input,	CONTENT_IGNORE,
		CONTENT_IGNORE,	input,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// shirt recipe 5 input in a t yields one result
void setTShirtRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,	CONTENT_IGNORE,
		input,		input,	input,
		CONTENT_IGNORE,	input,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// boots recipe 4 input in 2 separated columns yields one result
void setBootsRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		CONTENT_IGNORE,	input,
		input,		CONTENT_IGNORE,	input,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// hat recipe, 5 input in an upside down u yields one result
void setHatRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		input,		CONTENT_IGNORE,	input,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// helmet recipe, 5 input in an upside down u around a glass pane yields one result
void setHelmetRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,			input,
		input,		CONTENT_GLASS_PANE,	input,
		CONTENT_IGNORE,	CONTENT_IGNORE,		CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// like boots, but the top 2 input are replaced with string
void setShoesRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_CRAFTITEM_STRING,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_STRING,
		input,				CONTENT_IGNORE,	input,
		CONTENT_IGNORE,			CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// pants recipe 5 input in an upside-down V yields one result
void setShortsRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,		CONTENT_IGNORE,
		input,		CONTENT_IGNORE,	input,
		input,		CONTENT_IGNORE,	input
	};
	setRecipe(r,result,1);
}

InventoryItem *getResult(InventoryItem **items)
{
	for (std::vector<CraftDef>::iterator i=shaped_recipes.begin(); i!=shaped_recipes.end(); i++) {
		CraftDef d = *i;
		if (d == items)
			return InventoryItem::create(d.result,d.result_count);
	}
	for (std::vector<CraftDefShapeless>::iterator i=shapeless_recipes.begin(); i!=shapeless_recipes.end(); i++) {
		CraftDefShapeless d = *i;
		if (d == items)
			return InventoryItem::create(d.result,d.result_count);
	}

	return NULL;
}

content_t *getRecipe(InventoryItem *item)
{
	content_t r = item->getContent();
	content_t *recipe;
	for (std::vector<CraftDef>::iterator i=shaped_recipes.begin(); i!=shaped_recipes.end(); i++) {
		CraftDef d = *i;
		if (d.result == r) {
			recipe = new content_t[9];
			for (int j=0; j<9; j++) {
				recipe[j] = d.recipe[j];
			}
			return recipe;
		}
	}
	for (std::vector<CraftDefShapeless>::iterator i=shapeless_recipes.begin(); i!=shapeless_recipes.end(); i++) {
		CraftDefShapeless d = *i;
		if (d.result == r) {
			recipe = new content_t[9];
			for (int j=0; j<9; j++) {
				recipe[j] = d.recipe[j];
			}
			return recipe;
		}
	}
	return NULL;
}

content_t *getRecipe(InventoryItem *item, int index)
{
	content_t r = item->getContent();
	content_t *recipe;
	int count = 0;
	if (index < 0)
		return NULL;
	for (std::vector<CraftDef>::iterator i=shaped_recipes.begin(); i!=shaped_recipes.end(); i++) {
		CraftDef d = *i;
		if (d.result == r) {
			if (index != count++)
				continue;
			recipe = new content_t[9];
			for (int j=0; j<9; j++) {
				recipe[j] = d.recipe[j];
			}
			return recipe;
		}
	}
	for (std::vector<CraftDefShapeless>::iterator i=shapeless_recipes.begin(); i!=shapeless_recipes.end(); i++) {
		CraftDefShapeless d = *i;
		if (d.result == r) {
			if (index != count++)
				continue;
			recipe = new content_t[9];
			for (int j=0; j<9; j++) {
				recipe[j] = d.recipe[j];
			}
			return recipe;
		}
	}
	return NULL;
}

int getResultCount(InventoryItem *item)
{
	content_t r = item->getContent();
	for (std::vector<CraftDef>::iterator i=shaped_recipes.begin(); i!=shaped_recipes.end(); i++) {
		CraftDef d = *i;
		if (d.result == r)
			return d.result_count;
	}
	for (std::vector<CraftDefShapeless>::iterator i=shapeless_recipes.begin(); i!=shapeless_recipes.end(); i++) {
		CraftDefShapeless d = *i;
		if (d.result == r)
			return d.result_count;
	}
	return 0;
}

int getRecipeCount(InventoryItem *item)
{
	content_t r = item->getContent();
	int count = 0;
	for (std::vector<CraftDef>::iterator i=shaped_recipes.begin(); i!=shaped_recipes.end(); i++) {
		CraftDef d = *i;
		if (d.result == r)
			count++;
	}
	for (std::vector<CraftDefShapeless>::iterator i=shapeless_recipes.begin(); i!=shapeless_recipes.end(); i++) {
		CraftDefShapeless d = *i;
		if (d.result == r)
			count++;
	}
	return count;
}

//a little predicate type for determining whether a recipe contains a certain item
struct CraftDefContains {
	content_t item;
	explicit CraftDefContains(content_t item_in) : item(item_in) {}
	template <typename CD>
	bool operator()(const CD& def) const
	{
		return std::find(def.recipe, def.recipe + 9, item) != def.recipe + 9;
	}
};

int getReverseRecipeCount(InventoryItem *item)
{
	using namespace std;

	//make a predicate object to look for recipes containing the right item
	CraftDefContains contains_item (item->getContent());

	//count up the matching recipes
	return count_if(shaped_recipes.begin(), shaped_recipes.end(), contains_item)
	     + count_if(shapeless_recipes.begin(), shapeless_recipes.end(), contains_item);
}

//how to create a FoundReverseRecipe from a CraftDef
template <typename CD>
FoundReverseRecipe FRRFromCD(const CD& def)
{
	using namespace std;
	FoundReverseRecipe recipe;
	recipe.result = def.result;
	recipe.result_count = def.result_count;
	copy(def.recipe, def.recipe + 9, recipe.recipe);
	return recipe;
}

//a helper function for reverse recipe lookup
//count is used to count down to the correct recipe:
//it is decremented by the function each time that it finds a viable recipe
//this makes it possible to use this function for searches through multiple ranges
template <typename It>
FoundReverseRecipe reverseRecipeHelper(It cd_begin, It cd_end, content_t item, int &count)
{
	using namespace std;

	//make a predicate object to aid with the search for recipes containing the item
	CraftDefContains contains_item (item);

	//look through each of the recipes, finding the ones which contain item
	for (It it = cd_begin; it != cd_end; ++it) if (contains_item(*it)) {

		//continue on if the target recipe hasn't been reached yet
		if (count--) continue;

		//if it is the right recipe, make a copy and return it
		return FRRFromCD(*it);
	}

	//if nothing was found yet, return a default FoundReverseRecipe
	return FoundReverseRecipe();
}

FoundReverseRecipe getReverseRecipe(InventoryItem *iitem, int index)
{
	//ignore negative indeces
	if (index < 0)
		return FoundReverseRecipe();

	//find the content id for the item
	content_t item = iitem->getContent();

	//where to store the recipe when found
	FoundReverseRecipe recipe;

	//the recipe counter (counting down)
	int count = index;

	//look in the shaped recipes
	recipe = reverseRecipeHelper(shaped_recipes.begin(), shaped_recipes.end(), item, count);

	//if that fails, look in the shapeless recipes
	if (!recipe)
		recipe = reverseRecipeHelper(shapeless_recipes.begin(), shapeless_recipes.end(), item, count);

	//return the located recipe, if one was found
	return recipe;
}

//how to update an ingredient list given a range of new craft items
template <typename It>
void addToIngredientList(It results_begin, It results_end, std::vector<lists::ListData>& ingredient_list)
{
	using namespace std;

	//make a set to hold the items as the list is compiled
	set<content_t> ingredients (ingredient_list.begin(), ingredient_list.end());

	//go through the result list
	for (It it = results_begin; it != results_end; ++it) {

		//make a temporary inventory item for the result
		InventoryItem *result = InventoryItem::create(*it, 1);

		//go through every recipe for this item
		for (int rec_ind = getRecipeCount(result); rec_ind--;) {

			//get the recipe
			content_t *recipe = getRecipe(result, rec_ind);

			//eliminate duplicates
			sort(recipe, recipe + 9);
			content_t *uniq_end = unique(recipe, recipe + 9);

			//insert into the ingredients list
			ingredients.insert(recipe, uniq_end);

			//clean up
			delete recipe;
		}

		//clean up
		delete result;
	}

	//ignore CONTENT_IGNORE
	ingredients.erase(CONTENT_IGNORE);

	//dump the new ingredients into the ingredient list
	ingredient_list.insert(ingredient_list.end(), ingredients.begin(), ingredients.end());
}

std::vector<content_t>& getCraftGuideIngredientList()
{
	using namespace std;

	//the ingredient list, and the number of items that were in the craftguide list at the last check
	static vector<content_t> ingredient_list;
	static unsigned last_craftguide_count = 0;

	//get the craftguide list
	const vector<lists::ListData>& craft_list = lists::get("craftguide");

	//check if more items need to be added
	if (craft_list.size() > last_craftguide_count) {

		//if so, add the new stuff
		addToIngredientList(craft_list.begin() + last_craftguide_count, craft_list.end(), ingredient_list);

		//and update the craftguide count
		last_craftguide_count = craft_list.size();
	}

	//return the list
	return ingredient_list;
}

void giveCreative(Player *player)
{
	std::vector<lists::ListData> &creativeinv = lists::get("player-creative");

	InventoryList *l = player->inventory.getList("main");

	// if the player doesn't have a creative chest, reset their inventory
	if (!l || l->findItem(CONTENT_CREATIVE_CHEST,NULL) != NULL)
		return;

	// this stops the player being naked when the inventory is reset
	player->setClothesGiven(false);
	player->resetInventory();

	for(u8 i=0; i<creativeinv.size(); i++) {
		player->inventory.addItem("main",InventoryItem::create(creativeinv[i].content,creativeinv[i].count,0,creativeinv[i].data);
	}
}

void giveInitial(Player *player)
{
	player->resetInventory();
	assert(player->inventory.addItem("main", new ToolItem(CONTENT_TOOLITEM_STEELPICK, 0, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new ToolItem(CONTENT_TOOLITEM_STEELAXE, 0, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new ToolItem(CONTENT_TOOLITEM_STEELSHOVEL, 0, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new ToolItem(CONTENT_TOOLITEM_STEELSHEARS, 0, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new MaterialItem(CONTENT_TORCH, 99, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new MaterialItem(CONTENT_ROUGHSTONEBRICK, 99, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new MaterialItem(CONTENT_BORDERSTONE, 5, 0)) == NULL) ;
}

};
