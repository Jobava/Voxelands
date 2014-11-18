/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_craft.h
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

#ifndef CONTENT_CRAFT_HEADER
#define CONTENT_CRAFT_HEADER

#include "inventory.h"

class Player;

struct CraftDef {
	content_t recipe[9];
	content_t result;
	u16 result_count;

	bool operator==(InventoryItem * const *items)
	{
		u16 items_min_x = 100;
		u16 items_max_x = 100;
		u16 items_min_y = 100;
		u16 items_max_y = 100;
		for(u16 y=0; y<3; y++)
		for(u16 x=0; x<3; x++)
		{
			if(items[y*3 + x] == NULL)
				continue;
			if(items_min_x == 100 || x < items_min_x)
				items_min_x = x;
			if(items_min_y == 100 || y < items_min_y)
				items_min_y = y;
			if(items_max_x == 100 || x > items_max_x)
				items_max_x = x;
			if(items_max_y == 100 || y > items_max_y)
				items_max_y = y;
		}
		// No items at all, just return false
		if(items_min_x == 100)
			return false;

		u16 items_w = items_max_x - items_min_x + 1;
		u16 items_h = items_max_y - items_min_y + 1;

		u16 specs_min_x = 100;
		u16 specs_max_x = 100;
		u16 specs_min_y = 100;
		u16 specs_max_y = 100;
		for(u16 y=0; y<3; y++)
		for(u16 x=0; x<3; x++)
		{
			if(recipe[y*3 + x] == CONTENT_IGNORE)
				continue;
			if(specs_min_x == 100 || x < specs_min_x)
				specs_min_x = x;
			if(specs_min_y == 100 || y < specs_min_y)
				specs_min_y = y;
			if(specs_max_x == 100 || x > specs_max_x)
				specs_max_x = x;
			if(specs_max_y == 100 || y > specs_max_y)
				specs_max_y = y;
		}
		// No specs at all, just return false
		if(specs_min_x == 100)
			return false;

		u16 specs_w = specs_max_x - specs_min_x + 1;
		u16 specs_h = specs_max_y - specs_min_y + 1;

		// Different sizes
		if(items_w != specs_w || items_h != specs_h)
			return false;

		for(u16 y=0; y<specs_h; y++)
		for(u16 x=0; x<specs_w; x++)
		{
			u16 items_x = items_min_x + x;
			u16 items_y = items_min_y + y;
			u16 specs_x = specs_min_x + x;
			u16 specs_y = specs_min_y + y;

			if (items[items_y * 3 + items_x] == NULL) {
				if (recipe[specs_y * 3 + specs_x] != CONTENT_IGNORE)
					return false;
				continue;
			}
			if (items[items_y * 3 + items_x]->getContent() != recipe[specs_y * 3 + specs_x])
				return false;
		}

		return true;
	}

	bool operator==(content_t check[9])
	{
		for (int i=0; i<9; i++) {
			if (recipe[i] != check[i])
			return false;
		}
		return true;
	}
};

struct CraftDefShapeless {
	content_t recipe[9];
	content_t result;
	u16 result_count;

	bool operator==(InventoryItem * const *items)
	{
		s8 found[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
		s8 found_count = 0;
		s8 search_count = 0;
		s8 match_count = 0;
		for (s8 i=0; i<9; i++) {
			if (items[i] == NULL)
				continue;
			search_count++;
			for (s8 j=0; j<9; j++) {
				if (recipe[j] == CONTENT_IGNORE)
					continue;
				if (found[j] > 0)
					continue;
				found[j] = 0;
				if (recipe[j] == items[i]->getContent()) {
					found[j] = 1;
					match_count++;
					break;
				}
			}
		}
		for (s8 i=0; i<9; i++) {
			if (recipe[i] != CONTENT_IGNORE)
				found_count++;
		}
		return (found_count && search_count && match_count && match_count == search_count && found_count == search_count);
	}

	bool operator==(content_t check[9])
	{
		for (int i=0; i<9; i++) {
			if (recipe[i] != check[i])
			return false;
		}
		return true;
	}
};

namespace crafting {
	void initCrafting();

	// add recipes
	void setRecipe(u16 recipe[9], u16 result, u16 count);
	void setShapelessRecipe(u16 recipe[9], u16 result, u16 count);

	// shortcuts
	// one input yields one result
	void set1To1Recipe(u16 input, u16 result);
	// one input yields two result
	void set1To2Recipe(u16 input, u16 result);
	// one input yields four result
	void set1To4Recipe(u16 input, u16 result);
	// one input1 over one input2 yields one result
	void set1over1Recipe(u16 input1, u16 input2, u16 result);
	// one input1 over one input2 yields two result
	void set1over2Recipe(u16 input1, u16 input2, u16 result);
	// one input1 over one input2 yields four result
	void set1over4Recipe(u16 input1, u16 input2, u16 result);
	// four input in a square yields one result
	void setSoftBlockRecipe(u16 input, u16 result);
	// four input in a square yields four result
	void setBlockRecipe(u16 input, u16 result);
	// nine input yields one result
	void setHardBlockRecipe(u16 input, u16 result);
	// one input yields four result
	void setUncraftBlockRecipe(u16 input, u16 result);
	// one input yields nine result
	void setUncraftHardBlockRecipe(u16 input, u16 result);
	// four input in a Z yields four result
	void setBrickRecipe(u16 input, u16 result);
	// four input1 surround one input2 yields four result
	void setSurroundRecipe(u16 input1, u16 input2, u16 result);
	// five input in an X yields five result
	void set5Recipe(u16 input, u16 result);
	// three input in a v yields count result
	void setVRecipe(u16 input, u16 result, u16 count=1);
	// five input in a u yields one result
	void setURecipe(u16 input, u16 result);
	// three input in a horizontal row yields one result
	void setRow1Recipe(u16 input, u16 result);
	// three input in a horizontal row yields two result
	void setRow2Recipe(u16 input, u16 result);
	// three input in a horizontal row yields three result
	void setRow3Recipe(u16 input, u16 result);
	// three input in a vertical row yields one result
	void setCol1Recipe(u16 input, u16 result);
	// three input in a vertical row yields two result
	void setCol2Recipe(u16 input, u16 result);
	// three input in a vertical row yields three result
	void setCol3Recipe(u16 input, u16 result);
	// six input as a stair yields six result
	void setStairRecipe(u16 input, u16 result);
	// two input in a horizontal row yields three result
	void setTileRecipe(u16 input, u16 result);
	// eight input in a circle yields one result
	void setRoundRecipe(u16 input, u16 result);
	// eight input1 surrounding one input2 yields one result
	void setFilledRoundRecipe(u16 input1, u16 input2, u16 result);
	// six input in two horizontal rows yields six result
	void setWallRecipe(u16 input, u16 result);
	// shapeless shortcuts
	// one each of inputs shapeless yields 1 result
	void set1Any2Recipe(u16 input1, u16 input2, u16 result);
	// one each of inputs shapeless yields 2 result
	void set2Any2Recipe(u16 input1, u16 input2, u16 result);
	// one each of inputs shapeless yields 1 result
	void set1Any3Recipe(u16 input1, u16 input2, u16 input3, u16 result);
	// one each of inputs shapeless yields 2 result
	void set2Any3Recipe(u16 input1, u16 input2, u16 input3, u16 result);
	// one each of inputs shapeless yields 3 result
	void set3Any3Recipe(u16 input1, u16 input2, u16 input3, u16 result);
	// special shortcuts
	// bed recipe, input is bottom two cotton yields one result
	void setBedRecipe(u16 input, u16 result);
	// sign recipe, input is top six wood yields one result
	void setSignRecipe(u16 input, u16 result);
	// shears recipe, input is blade yields one result
	void setShearsRecipe(u16 input, u16 result);
	// spear recipe, input is blade yields one result
	void setSpearRecipe(u16 input, u16 result);
	// shovel recipe, input is blade yields one result
	void setShovelRecipe(u16 input, u16 result);
	// axe recipe, input is blade yields one result
	void setAxeRecipe(u16 input, u16 result);
	// pick recipe, input is blade yields one result
	void setPickRecipe(u16 input, u16 result);
	// sword recipe, input is blade yields one result
	void setSwordRecipe(u16 input, u16 result);
	// gate recipe 1 2 1 in two rows yields one result
	void setGateRecipe(u16 input1, u16 input2, u16 result);
	// pants recipe 7 input in an upside-down U yields one result
	void setPantsRecipe(u16 input, u16 result);
	// shirt recipe 5 input in a T yields one result
	void setShirtRecipe(u16 input, u16 result);
	// shirt recipe 5 input in a t yields one result
	void setTShirtRecipe(u16 input, u16 result);
	// boots recipe 4 input in 2 separated columns yields one result
	void setBootsRecipe(u16 input, u16 result);
	// hat recipe, 5 input in an upside down u yields one result
	void setHatRecipe(u16 input, u16 result);
	// helmet recipe, 5 input in an upside down u around a glass pane yields one result
	void setHelmetRecipe(u16 input, u16 result);
	// like boots, but the top 2 input are replaced with string
	void setShoesRecipe(u16 input, u16 result);
	// pants recipe 5 input in an upside-down V yields one result
	void setShortsRecipe(u16 input, u16 result);

	InventoryItem *getResult(InventoryItem **items);
	content_t *getRecipe(InventoryItem *item);
	content_t *getRecipe(InventoryItem *item, int i);
	int getResultCount(InventoryItem *item);
	int getRecipeCount(InventoryItem *item);

	void giveCreative(Player *player);
	void giveInitial(Player *player);
};


#endif

