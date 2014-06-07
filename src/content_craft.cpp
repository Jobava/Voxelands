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

#include "content_craft.h"
#include "inventory.h"
#include "content_mapnode.h"
#include "content_craftitem.h"
#include "content_toolitem.h"
#include "content_list.h"
#include "player.h"
#include "mapnode.h" // For content_t
#include "settings.h" // for g_settings

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
	setRecipe(r,result,1);
}
// shovel recipe, input is blade yields one result
void setShovelRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
	u16 r1[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,1);
	u16 r2[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r2,result,1);
}
// axe recipe, input is blade yields one result
void setAxeRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,				CONTENT_IGNORE,
		input,		CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
	u16 r1[9] = {
		CONTENT_IGNORE,	input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,1);
	u16 r2[9] = {
		input,		input,				CONTENT_IGNORE,
		input,		CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r2,result,1);
	u16 r3[9] = {
		CONTENT_IGNORE,	input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r3,result,1);
	u16 r4[9] = {
		input,		input,				CONTENT_IGNORE,
		input,		CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r4,result,1);
	u16 r5[9] = {
		CONTENT_IGNORE,	input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r5,result,1);
}
// pick recipe, input is blade yields one result
void setPickRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
	u16 r1[9] = {
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,1);
	u16 r2[9] = {
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r2,result,1);
}
// sword recipe, input is blade yields one result
void setSwordRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
	u16 r1[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,1);
	u16 r2[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_JUNGLE_PLANK,	CONTENT_IGNORE
	};
	setRecipe(r2,result,1);
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

InventoryItem *getResult(InventoryItem **items)
{
	for (std::vector<CraftDef>::iterator i=shaped_recipes.begin(); i!=shaped_recipes.end(); i++) {
		CraftDef d = *i;
		if (d == items) {
			if ((d.result&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
				return new CraftItem(d.result,d.result_count);
			}else if ((d.result&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
				return new ToolItem(d.result,d.result_count);
			}else{
				return new MaterialItem(d.result,d.result_count);
			}
		}
	}
	for (std::vector<CraftDefShapeless>::iterator i=shapeless_recipes.begin(); i!=shapeless_recipes.end(); i++) {
		CraftDefShapeless d = *i;
		if (d == items) {
			if ((d.result&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
				return new CraftItem(d.result,d.result_count);
			}else if ((d.result&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
				return new ToolItem(d.result,d.result_count);
			}else{
				return new MaterialItem(d.result,d.result_count);
			}
		}
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

void giveCreative(Player *player)
{
	std::vector<content_t> &creativeinv = lists::get("player-creative");

	player->resetInventory();

	for(u8 i=0; i<creativeinv.size(); i++) {
		if ((creativeinv[(int)i]&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
			assert(player->inventory.addItem("main", new CraftItem(creativeinv[i], 1)) == NULL) ;
		}else if ((creativeinv.at(i)&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
			assert(player->inventory.addItem("main", new ToolItem(creativeinv[i], 0)) == NULL) ;
		}else{
			assert(player->inventory.addItem("main", new MaterialItem(creativeinv[i], 1)) == NULL) ;
		}
	}
}

void giveInitial(Player *player)
{
	player->resetInventory();
	assert(player->inventory.addItem("main", new ToolItem(CONTENT_TOOLITEM_STEELPICK, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new ToolItem(CONTENT_TOOLITEM_STEELAXE, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new ToolItem(CONTENT_TOOLITEM_STEELSHOVEL, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new ToolItem(CONTENT_TOOLITEM_STEELSHEARS, 0)) == NULL) ;
	assert(player->inventory.addItem("main", new MaterialItem(CONTENT_TORCH, 99)) == NULL) ;
	assert(player->inventory.addItem("main", new MaterialItem(CONTENT_ROUGHSTONEBRICK, 99)) == NULL) ;
	assert(player->inventory.addItem("main", new MaterialItem(CONTENT_BORDERSTONE, 5)) == NULL) ;
}

};
