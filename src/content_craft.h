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

			if (items[items_y * 3 + items_x]->getContent() != recipe[specs_y * 3 + specs_x])
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
		for (s8 i=0; i<9; i++) {
			if (items[i] == NULL)
				continue;
			search_count++;
			for (s8 j=0; j<9; j++) {
				if (recipe[j] == CONTENT_IGNORE)
					continue;
				if (found[j] > -1)
					continue;
				if (recipe[j] == items[i]->getContent()) {
					found[j] = i;
					found_count++;
					break;
				}
			}
		}
		return (found_count == search_count);
	}
};

namespace crafting {
	void initCrafting();

	void setRecipe(u16 recipe[9], u16 result, u16 count);
	void setShapelessRecipe(u16 recipe[9], u16 result, u16 count);

	InventoryItem *getResult(InventoryItem **items);
	InventoryItem **getRecipe(InventoryItem *item);

	void giveCreative(Player *player);
	void giveInitial(Player *player);
};


#endif

