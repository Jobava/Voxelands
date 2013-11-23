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

void setRecipe(content_t recipe[9], content_t result, u16 count)
{
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
// three input in a v yields one result
void setVRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		input,		CONTENT_IGNORE,	input,
		CONTENT_IGNORE,	input,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
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
// six input in two vertical columns yields one result
void setDoorRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		CONTENT_IGNORE,
		input,		input,		CONTENT_IGNORE,
		input,		input,		CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// six input in two horizontal rows yields one result
void setHatchRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,		input,
		input,		input,		input,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
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
	setRecipe(r,result,1);
}
// one each of inputs shapeless yields 1 result
void set1Any2(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		input2,		CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,1);
}
// one each of inputs shapeless yields 2 result
void set2Any2(u16 input1, u16 input2, u16 result)
{
	u16 r[9] = {
		input1,		input2,		CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,2);
}
// one each of inputs shapeless yields 3 result
void set1Any3(u16 input1, u16 input2, u16 input3, u16 result)
{
	u16 r[9] = {
		input1,		input2,		input3,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,1);
}
// one each of inputs shapeless yields 2 result
void set2Any3(u16 input1, u16 input2, u16 input3, u16 result)
{
	u16 r[9] = {
		input1,		input2,		input3,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_IGNORE,	CONTENT_IGNORE
	};
	setShapelessRecipe(r,result,2);
}
// one each of inputs shapeless yields 3 result
void set3Any3(u16 input1, u16 input2, u16 input3, u16 result)
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
		CONTENT_CRAFTITEM_STICK,	CONTENT_COTTON,	CONTENT_CRAFTITEM_STICK,
		CONTENT_WOOD,			input,		CONTENT_WOOD,
		CONTENT_CRAFTITEM_STICK,	input,		CONTENT_CRAFTITEM_STICK
	};
	setRecipe(r,result,1);
	u16 r1[9] = {
		CONTENT_CRAFTITEM_STICK,	CONTENT_COTTON,	CONTENT_CRAFTITEM_STICK,
		CONTENT_JUNGLEWOOD,		input,		CONTENT_JUNGLEWOOD,
		CONTENT_CRAFTITEM_STICK,	input,		CONTENT_CRAFTITEM_STICK
	};
	setRecipe(r1,result,1);
}
// sign recipe, input is top six wood yields one result
void setSignRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,				input,
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE
	};
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
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// axe recipe, input is blade yields one result
void setAxeRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,				CONTENT_IGNORE,
		input,		CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
	u16 r1[9] = {
		input,		input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE
	};
	setRecipe(r1,result,1);
}
// pick recipe, input is blade yields one result
void setPickRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		input,		input,				input,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// sword recipe, input is blade yields one result
void setSwordRecipe(u16 input, u16 result)
{
	u16 r[9] = {
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	input,				CONTENT_IGNORE,
		CONTENT_IGNORE,	CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE
	};
	setRecipe(r,result,1);
}
// gate recipe 1 2 1 in two rows yields one result
void setGateRecipt(u16 input1, u16 input2, u16 result)
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

// TODO: return recipe from result
InventoryItem **getRecipe(InventoryItem *item)
{
	return NULL;
}

// TODO: creative inventory needs redoing
void giveCreative(Player *player)
{
	player->resetInventory();

	// Give some good tools
	{
		InventoryItem *item = new ToolItem("MesePick", 0);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
	{
		InventoryItem *item = new ToolItem("SteelPick", 0);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
	{
		InventoryItem *item = new ToolItem("SteelAxe", 0);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
	{
		InventoryItem *item = new ToolItem("SteelShovel", 0);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}

	/*
		Give materials
	*/

	// CONTENT_IGNORE-terminated list
	content_t material_items[] = {
		CONTENT_TORCH,
		CONTENT_ROUGHSTONE,
		CONTENT_MUD,
		CONTENT_STONE,
		CONTENT_SAND,
		CONTENT_SANDSTONE,
		CONTENT_CLAY,
		CONTENT_BRICK,
		CONTENT_TREE,
		CONTENT_LEAVES,
		CONTENT_CACTUS,
		CONTENT_PAPYRUS,
		CONTENT_BOOKSHELF,
		CONTENT_GLASS,
		CONTENT_FENCE,
		CONTENT_RAIL,
		CONTENT_MESE,
		CONTENT_WATERSOURCE,
		CONTENT_COTTON,
		CONTENT_CHEST,
		CONTENT_FURNACE,
		CONTENT_SIGN,
		CONTENT_LAVASOURCE,
		CONTENT_WOOD,
		CONTENT_LADDER,
		CONTENT_IGNORE
	};

	content_t *mip = material_items;
	for(u16 i=0; i<PLAYER_INVENTORY_SIZE; i++)
	{
		if(*mip == CONTENT_IGNORE)
			break;

		InventoryItem *item = new MaterialItem(*mip, 1);
		player->inventory.addItem("main", item);

		mip++;
	}
}

void giveInitial(Player *player)
{
	{
		InventoryItem *item = new ToolItem("SteelPick", 0);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
	{
		InventoryItem *item = new MaterialItem(CONTENT_TORCH, 99);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
	{
		InventoryItem *item = new ToolItem("SteelAxe", 0);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
	{
		InventoryItem *item = new ToolItem("SteelShovel", 0);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
	{
		InventoryItem *item = new ToolItem("Shears", 0);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
	{
		InventoryItem *item = new MaterialItem(CONTENT_ROUGHSTONE, 99);
		void* r = player->inventory.addItem("main", item);
		assert(r == NULL);
	}
}

};

#if 0
/*
	items: actually *items[9]
	return value: allocates a new item, or returns NULL.
*/
InventoryItem *craft_get_result(InventoryItem **items)
{
	static CraftDef defs[234];
	static int defs_init = 0;

	// only initialise (and hence allocate) these once
	if (!defs_init) {
		// Wood
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_TREE);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD, 4);
			defs_init++;
		}

		// Junglewood
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLETREE);
			defs[defs_init].item = new MaterialItem(CONTENT_JUNGLEWOOD, 4);
			defs_init++;
		}

		// Stick from wood
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new CraftItem("Stick", 4);
			defs_init++;
		}

		// Stick from jungle wood
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].item = new CraftItem("Stick", 4);
			defs_init++;
		}

		// Fence
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_FENCE, 2);
			defs_init++;
		}

		// Sign from wood
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_SIGN, 1);
			defs_init++;
		}

		// Sign from jungle wood
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_SIGN, 1);
			defs_init++;
		}

		// Torch
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_TORCH, 4);
			defs_init++;
		}

		// Wooden pick
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("WPick", 0);
			defs_init++;
		}

		// Stone pick
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("STPick", 0);
			defs_init++;
		}

		// Steel pick
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("SteelPick", 0);
			defs_init++;
		}

		// Mese pick
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_MESE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_MESE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_MESE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("MesePick", 0);
			defs_init++;
		}

		// Wooden shovel
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("WShovel", 0);
			defs_init++;
		}

		// Stone shovel
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("STShovel", 0);
			defs_init++;
		}

		// Steel shovel
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("SteelShovel", 0);
			defs_init++;
		}

		// Wooden axe
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("WAxe", 0);
			defs_init++;
		}

		// Stone axe
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("STAxe", 0);
			defs_init++;
		}

		// Steel axe
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("SteelAxe", 0);
			defs_init++;
		}

		// Shears
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("Shears", 0);
			defs_init++;
		}

		// Wooden sword
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("WSword", 0);
			defs_init++;
		}

		// Stone sword
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("STSword", 0);
			defs_init++;
		}

		// Steel sword
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("SteelSword", 0);
			defs_init++;
		}

		// Rail
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_RAIL, 15);
			defs_init++;
		}

		// Chest
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_CHEST, 1);
			defs_init++;
		}

		// Locking Chest
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_LOCKABLE_CHEST, 1);
			defs_init++;
		}

		// Converted Locking Chest
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CHEST);
			defs[defs_init].item = new MaterialItem(CONTENT_LOCKABLE_CHEST, 1);
			defs_init++;
		}

		// Border Stone
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_MESE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_BORDERSTONE, 1);
			defs_init++;
		}

		// Furnace
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_FURNACE, 1);
			defs_init++;
		}

		// Incinerator
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_MESE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_INCINERATOR, 1);
			defs_init++;
		}

		// Cobble
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_COBBLE, 5);
			defs_init++;
		}

		// Steel block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL, 1);
			defs_init++;
		}

		// Uncrafted Steel block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL);
			defs[defs_init].item = new CraftItem("steel_ingot", 9);
			defs_init++;
		}

		// copper block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "copper_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_COPPER, 1);
			defs_init++;
		}

		// Uncrafted Copper block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COPPER);
			defs[defs_init].item = new CraftItem("copper_ingot", 9);
			defs_init++;
		}

		// gold block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "gold_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_GOLD, 1);
			defs_init++;
		}

		// Uncrafted gold block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GOLD);
			defs[defs_init].item = new CraftItem("gold_ingot", 9);
			defs_init++;
		}

		// silver block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "silver_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_SILVER, 1);
			defs_init++;
		}

		// Uncrafted silver block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_SILVER);
			defs[defs_init].item = new CraftItem("silver_ingot", 9);
			defs_init++;
		}

		// tin block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_TIN, 1);
			defs_init++;
		}

		// Uncrafted tin block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_TIN);
			defs[defs_init].item = new CraftItem("tin_ingot", 9);
			defs_init++;
		}

		// quartz block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].item = new MaterialItem(CONTENT_QUARTZ, 1);
			defs_init++;
		}

		// Uncrafted quartz block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_QUARTZ);
			defs[defs_init].item = new CraftItem("lump_of_quartz", 9);
			defs_init++;
		}

		// Sandstone
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_SAND);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_SAND);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_SAND);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_SAND);
			defs[defs_init].item = new MaterialItem(CONTENT_SANDSTONE, 1);
			defs_init++;
		}

		// Clay
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "lump_of_clay");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "lump_of_clay");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "lump_of_clay");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "lump_of_clay");
			defs[defs_init].item = new MaterialItem(CONTENT_CLAY, 1);
			defs_init++;
		}

		// Brick
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "clay_brick");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "clay_brick");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "clay_brick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "clay_brick");
			defs[defs_init].item = new MaterialItem(CONTENT_BRICK, 1);
			defs_init++;
		}

		// Cotton
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEGRASS);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEGRASS);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEGRASS);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEGRASS);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON, 1);
			defs_init++;
		}

		// Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLUE);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON, 1);
			defs_init++;
		}

		// Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_GREEN);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON, 1);
			defs_init++;
		}

		// Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_ORANGE);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON, 1);
			defs_init++;
		}

		// Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_PURPLE);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON, 1);
			defs_init++;
		}

		// Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_RED);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON, 1);
			defs_init++;
		}

		// Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_YELLOW);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON, 1);
			defs_init++;
		}

		// Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLACK);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON, 1);
			defs_init++;
		}

		// blue Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON_BLUE, 1);
			defs_init++;
		}

		// green Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_green");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON_GREEN, 1);
			defs_init++;
		}

		// orange Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_orange");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON_ORANGE, 1);
			defs_init++;
		}

		// purple Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_purple");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON_PURPLE, 1);
			defs_init++;
		}

		// red Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON_RED, 1);
			defs_init++;
		}

		// yellow Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON_YELLOW, 1);
			defs_init++;
		}

		// black Cotton
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_black");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].item = new MaterialItem(CONTENT_COTTON_BLACK, 1);
			defs_init++;
		}

		// Paper
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_PAPYRUS);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_PAPYRUS);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_PAPYRUS);
			defs[defs_init].item = new CraftItem("paper", 1);
			defs_init++;
		}

		// Book
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "paper");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "paper");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "paper");
			defs[defs_init].item = new CraftItem("book", 1);
			defs_init++;
		}

		// Book shelf
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "book");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "book");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "book");
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_BOOKSHELF, 1);
			defs_init++;
		}

		// Ladder
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_LADDER, 1);
			defs_init++;
		}

		// Iron Apple
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "apple");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new CraftItem("apple_iron", 1);
			defs_init++;
		}

		// roughstone slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_ROUGHSTONE_SLAB, 3);
			defs_init++;
		}

		// cobble slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].item = new MaterialItem(CONTENT_COBBLE_SLAB, 3);
			defs_init++;
		}

		// mossy cobble slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].item = new MaterialItem(CONTENT_MOSSYCOBBLE_SLAB, 3);
			defs_init++;
		}

		// stone slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONE_SLAB, 3);
			defs_init++;
		}

		// wood slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_SLAB, 3);
			defs_init++;
		}

		// jungle slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_JUNGLE_SLAB, 3);
			defs_init++;
		}

		// brick slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].item = new MaterialItem(CONTENT_BRICK_SLAB, 3);
			defs_init++;
		}

		// sandstone slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_SANDSTONE_SLAB, 3);
			defs_init++;
		}

		// glass slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_SLAB, 3);
			defs_init++;
		}

		// blue glass slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLUE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLUE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLUE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_BLUE_SLAB, 3);
			defs_init++;
		}

		// green glass slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_GREEN);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_GREEN);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_GREEN);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_GREEN_SLAB, 3);
			defs_init++;
		}

		// orange glass slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_ORANGE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_ORANGE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_ORANGE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_ORANGE_SLAB, 3);
			defs_init++;
		}

		// purple glass slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PURPLE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PURPLE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PURPLE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PURPLE_SLAB, 3);
			defs_init++;
		}

		// red glass slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_RED);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_RED);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_RED);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_RED_SLAB, 3);
			defs_init++;
		}

		// yellow glass slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_YELLOW);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_YELLOW);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_YELLOW);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_YELLOW_SLAB, 3);
			defs_init++;
		}

		// black glass slabs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLACK);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLACK);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLACK);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_BLACK_SLAB, 3);
			defs_init++;
		}

		// roughstone stairs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_ROUGHSTONE_STAIR, 6);
			defs_init++;
		}

		// roughstone stairs
		{
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_ROUGHSTONE_STAIR, 6);
			defs_init++;
		}

		// cobble stairs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].item = new MaterialItem(CONTENT_COBBLE_STAIR, 6);
			defs_init++;
		}

		// cobble stairs
		{
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].item = new MaterialItem(CONTENT_COBBLE_STAIR, 6);
			defs_init++;
		}

		// mossy cobble stairs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].item = new MaterialItem(CONTENT_MOSSYCOBBLE_STAIR, 6);
			defs_init++;
		}

		// mossy cobble stairs
		{
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].item = new MaterialItem(CONTENT_MOSSYCOBBLE_STAIR, 6);
			defs_init++;
		}

		// stone stairs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONE_STAIR, 6);
			defs_init++;
		}

		// stone stairs
		{
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONE_STAIR, 6);
			defs_init++;
		}

		// wood stairs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_STAIR, 6);
			defs_init++;
		}

		// wood stairs
		{
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_STAIR, 6);
			defs_init++;
		}

		// jungle stairs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_JUNGLE_STAIR, 6);
			defs_init++;
		}

		// jungle stairs
		{
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_JUNGLE_STAIR, 6);
			defs_init++;
		}

		// brick stairs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].item = new MaterialItem(CONTENT_BRICK_STAIR, 6);
			defs_init++;
		}

		// brick stairs
		{
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_BRICK);
			defs[defs_init].item = new MaterialItem(CONTENT_BRICK_STAIR, 6);
			defs_init++;
		}

		// sandstone stairs
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_SANDSTONE_STAIR, 6);
			defs_init++;
		}

		// sandstone stairs
		{
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_SANDSTONE_STAIR, 6);
			defs_init++;
		}

		// glass light
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_TORCH);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASSLIGHT, 4);
			defs_init++;
		}

		// stone brick
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONEBRICK, 4);
			defs_init++;
		}

		// stone brick
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONEBRICK, 4);
			defs_init++;
		}

		// stone block
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONEBLOCK, 4);
			defs_init++;
		}

		// stone brick
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONEBRICK, 4);
			defs_init++;
		}

		// stone brick
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONEBRICK, 4);
			defs_init++;
		}

		// roughstone brick
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_ROUGHSTONEBRICK, 4);
			defs_init++;
		}

		// roughstone brick
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_ROUGHSTONEBRICK, 4);
			defs_init++;
		}

		// roughstone block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_ROUGHSTONEBLOCK, 4);
			defs_init++;
		}

		// wood door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_DOOR_LT, 1);
			defs_init++;
		}

		// wood door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_DOOR_LT, 1);
			defs_init++;
		}

		// glass door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_DOOR_LT, 1);
			defs_init++;
		}

		// steel door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_DOOR_LT, 1);
			defs_init++;
		}

		// wood windowed door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_DOOR_LT);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_W_DOOR_LT, 1);
			defs_init++;
		}

		// steel windowed door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL_DOOR_LT);
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_W_DOOR_LT, 1);
			defs_init++;
		}

		// wood windowed door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_DOOR_RT);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_W_DOOR_RT, 1);
			defs_init++;
		}

		// steel windowed door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL_DOOR_RT);
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_W_DOOR_RT, 1);
			defs_init++;
		}

		// wood opposite door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_DOOR_LT);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_DOOR_RT, 1);
			defs_init++;
		}

		// glass opposite door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_DOOR_LT);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_DOOR_RT, 1);
			defs_init++;
		}

		// steel opposite door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL_DOOR_LT);
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_DOOR_RT, 1);
			defs_init++;
		}

		// wood !opposite door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_DOOR_RT);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_DOOR_LT, 1);
			defs_init++;
		}

		// glass !opposite door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_DOOR_RT);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_DOOR_LT, 1);
			defs_init++;
		}

		// steel !opposite door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL_DOOR_RT);
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_DOOR_LT, 1);
			defs_init++;
		}

		// wood opposite windowed door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_W_DOOR_LT);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_W_DOOR_RT, 1);
			defs_init++;
		}

		// steel opposite windowed door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL_W_DOOR_LT);
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_W_DOOR_RT, 1);
			defs_init++;
		}

		// wood !opposite windowed door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_W_DOOR_RT);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_W_DOOR_LT, 1);
			defs_init++;
		}

		// steel !opposite windowed door
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL_W_DOOR_RT);
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_W_DOOR_LT, 1);
			defs_init++;
		}

		// wood hatch
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_HATCH, 1);
			defs_init++;
		}

		// wood hatch
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_HATCH, 1);
			defs_init++;
		}

		// steel hatch
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_HATCH, 1);
			defs_init++;
		}

		// wood windowed hatch
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_HATCH);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_W_HATCH, 1);
			defs_init++;
		}

		// steel windowed hatch
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL_HATCH);
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_W_HATCH, 1);
			defs_init++;
		}

		// wood gate
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_GATE, 1);
			defs_init++;
		}

		// wood gate
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_GATE, 1);
			defs_init++;
		}

		// steel gate
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL);
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_STEEL);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new MaterialItem(CONTENT_STEEL_GATE, 1);
			defs_init++;
		}

		// flower pot
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "lump_of_clay");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "lump_of_clay");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "lump_of_clay");
			defs[defs_init].item = new MaterialItem(CONTENT_FLOWER_POT_RAW, 1);
			defs_init++;
		}

		// yellow dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_FLOWER_DAFFODIL);
			defs[defs_init].item = new CraftItem("dye_yellow", 2);
			defs_init++;
		}

		// red dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_FLOWER_ROSE);
			defs[defs_init].item = new CraftItem("dye_red", 2);
			defs_init++;
		}

		// blue dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_FLOWER_TULIP);
			defs[defs_init].item = new CraftItem("dye_blue", 2);
			defs_init++;
		}

		// green dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].item = new CraftItem("dye_green", 2);
			defs_init++;
		}

		// green dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].item = new CraftItem("dye_green", 2);
			defs_init++;
		}

		// orange dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].item = new CraftItem("dye_orange", 2);
			defs_init++;
		}

		// orange dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].item = new CraftItem("dye_orange", 2);
			defs_init++;
		}

		// purple dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].item = new CraftItem("dye_purple", 2);
			defs_init++;
		}

		// purple dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].item = new CraftItem("dye_purple", 2);
			defs_init++;
		}

		// white dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].item = new CraftItem("dye_white", 2);
			defs_init++;
		}

		// black dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].item = new CraftItem("dye_black", 2);
			defs_init++;
		}

		// black dye
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "dye_white");
			defs[defs_init].item = new CraftItem("dye_black", 2);
			defs_init++;
		}

		// Wooden bucket
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new ToolItem("WBucket", 0);
			defs_init++;
		}

		// Steel bucket
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new ToolItem("SteelBucket", 0);
			defs_init++;
		}

		// Tin bucket
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "tin_ingot");
			defs[defs_init].item = new ToolItem("TinBucket", 0);
			defs_init++;
		}

		// Roughstone wall
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_ROUGHSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_ROUGHSTONE_WALL, 6);
			defs_init++;
		}

		// Cobble wall
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_COBBLE);
			defs[defs_init].item = new MaterialItem(CONTENT_COBBLE_WALL, 6);
			defs_init++;
		}

		// MossyCobble wall
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_MOSSYCOBBLE);
			defs[defs_init].item = new MaterialItem(CONTENT_MOSSYCOBBLE_WALL, 6);
			defs_init++;
		}

		// Stone wall
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_STONE);
			defs[defs_init].item = new MaterialItem(CONTENT_STONE_WALL, 6);
			defs_init++;
		}

		// Sandstone wall
		{
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_SANDSTONE_WALL, 6);
			defs_init++;
		}

		// hay bale
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_DEADGRASS);
			defs[defs_init].item = new MaterialItem(CONTENT_HAY, 1);
			defs_init++;
		}

		// sandstone block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_SANDSTONE_BLOCK, 4);
			defs_init++;
		}

		// sandstone brick
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_SANDSTONE_BRICK, 4);
			defs_init++;
		}

		// sandstone brick
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_SANDSTONE);
			defs[defs_init].item = new MaterialItem(CONTENT_SANDSTONE_BRICK, 4);
			defs_init++;
		}

		// terracotta block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].item = new MaterialItem(CONTENT_TERRACOTTA_BLOCK, 4);
			defs_init++;
		}

		// terracotta brick
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].item = new MaterialItem(CONTENT_TERRACOTTA_BRICK, 4);
			defs_init++;
		}

		// terracotta brick
		{
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].item = new MaterialItem(CONTENT_TERRACOTTA_BRICK, 4);
			defs_init++;
		}

		// terracotta tile
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].specs[8] = ItemSpec(ITEM_MATERIAL, CONTENT_TERRACOTTA);
			defs[defs_init].item = new MaterialItem(CONTENT_TERRACOTTA_TILE, 5);
			defs_init++;
		}

		// blue clay
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CLAY);
			defs[defs_init].item = new MaterialItem(CONTENT_CLAY_BLUE, 1);
			defs_init++;
		}

		// green clay
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_green");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CLAY);
			defs[defs_init].item = new MaterialItem(CONTENT_CLAY_GREEN, 1);
			defs_init++;
		}

		// orange clay
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_orange");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CLAY);
			defs[defs_init].item = new MaterialItem(CONTENT_CLAY_ORANGE, 1);
			defs_init++;
		}

		// purple clay
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_purple");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CLAY);
			defs[defs_init].item = new MaterialItem(CONTENT_CLAY_PURPLE, 1);
			defs_init++;
		}

		// red clay
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CLAY);
			defs[defs_init].item = new MaterialItem(CONTENT_CLAY_RED, 1);
			defs_init++;
		}

		// yellow clay
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CLAY);
			defs[defs_init].item = new MaterialItem(CONTENT_CLAY_YELLOW, 1);
			defs_init++;
		}

		// black clay
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_black");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CLAY);
			defs[defs_init].item = new MaterialItem(CONTENT_CLAY_BLACK, 1);
			defs_init++;
		}

		// blue glass
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_BLUE, 1);
			defs_init++;
		}

		// green glass
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_green");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_GREEN, 1);
			defs_init++;
		}

		// orange glass
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_orange");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_ORANGE, 1);
			defs_init++;
		}

		// purple glass
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_purple");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PURPLE, 1);
			defs_init++;
		}

		// red glass
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_RED, 1);
			defs_init++;
		}

		// yellow glass
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_YELLOW, 1);
			defs_init++;
		}

		// black glass
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_black");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_BLACK, 1);
			defs_init++;
		}

		// Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET, 3);
			defs_init++;
		}

		// blue Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLUE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLUE);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_BLUE, 3);
			defs_init++;
		}

		// blue Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CARPET);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_BLUE, 1);
			defs_init++;
		}

		// green Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_GREEN);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_GREEN);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_GREEN, 3);
			defs_init++;
		}

		// green Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_green");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CARPET);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_GREEN, 1);
			defs_init++;
		}

		// orange Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_ORANGE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_ORANGE);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_ORANGE, 3);
			defs_init++;
		}

		// orange Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_orange");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CARPET);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_ORANGE, 1);
			defs_init++;
		}

		// purple Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_PURPLE);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_PURPLE);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_PURPLE, 3);
			defs_init++;
		}

		// purple Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_purple");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CARPET);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_PURPLE, 1);
			defs_init++;
		}

		// red Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_RED);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_RED);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_RED, 3);
			defs_init++;
		}

		// red Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CARPET);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_RED, 1);
			defs_init++;
		}

		// yellow Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_YELLOW);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_YELLOW);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_YELLOW, 3);
			defs_init++;
		}

		// yellow Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CARPET);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_YELLOW, 1);
			defs_init++;
		}

		// black Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLACK);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLACK);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_BLACK, 3);
			defs_init++;
		}

		// black Carpet
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_black");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_CARPET);
			defs[defs_init].item = new MaterialItem(CONTENT_CARPET_BLACK, 1);
			defs_init++;
		}

		// Coal block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "lump_of_coal");
			defs[defs_init].item = new MaterialItem(CONTENT_COAL, 1);
			defs_init++;
		}

		// Coal block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_COAL);
			defs[defs_init].item = new CraftItem("lump_of_coal", 9);
			defs_init++;
		}

		// Charoal block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].item = new MaterialItem(CONTENT_CHARCOAL, 1);
			defs_init++;
		}

		// Charcoal block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_CHARCOAL);
			defs[defs_init].item = new CraftItem("lump_of_charcoal", 9);
			defs_init++;
		}

		// fire starter
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "lump_of_flint");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new ToolItem("FireStarter", 0);
			defs_init++;
		}

		// quartz dust
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "lump_of_quartz");
			defs[defs_init].item = new CraftItem("quartz_dust", 1);
			defs_init++;
		}

		// gun powder
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "lump_of_charcoal");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "saltpeter");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "lump_of_flint");
			defs[defs_init].item = new CraftItem("gunpowder", 2);
			defs_init++;
		}

		// tnt
		if (g_settings->getBool("enable_tnt")) {
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "gunpowder");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_SAND);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "gunpowder");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_SAND);
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "gunpowder");
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_SAND);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "gunpowder");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_SAND);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "gunpowder");
			defs[defs_init].item = new MaterialItem(CONTENT_TNT, 1);
			defs_init++;
		}

		// glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE, 3);
			defs_init++;
		}

		// blue glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLUE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLUE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLUE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_BLUE, 3);
			defs_init++;
		}

		// blue glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "blue_dye");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PANE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_BLUE, 1);
			defs_init++;
		}

		// green glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_GREEN);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_GREEN);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_GREEN);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_GREEN, 3);
			defs_init++;
		}

		// green glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "green_dye");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PANE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_GREEN, 1);
			defs_init++;
		}

		// orange glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_ORANGE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_ORANGE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_ORANGE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_ORANGE, 3);
			defs_init++;
		}

		// orange glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "orange_dye");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PANE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_ORANGE, 1);
			defs_init++;
		}

		// purple glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PURPLE);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PURPLE);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PURPLE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_PURPLE, 3);
			defs_init++;
		}

		// purple glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "purple_dye");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PANE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_PURPLE, 1);
			defs_init++;
		}

		// red glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_RED);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_RED);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_RED);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_RED, 3);
			defs_init++;
		}

		// red glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "red_dye");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PANE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_RED, 1);
			defs_init++;
		}

		// yellow glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_YELLOW);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_YELLOW);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_YELLOW);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_YELLOW, 3);
			defs_init++;
		}

		// yellow glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "yellow_dye");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PANE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_YELLOW, 1);
			defs_init++;
		}

		// black glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLACK);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLACK);
			defs[defs_init].specs[6] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_BLACK);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_BLACK, 3);
			defs_init++;
		}

		// black glass pane
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "black_dye");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_GLASS_PANE);
			defs[defs_init].item = new MaterialItem(CONTENT_GLASS_PANE_BLACK, 1);
			defs_init++;
		}

		// Snow block
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].specs[1] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].specs[4] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "snow_ball");
			defs[defs_init].item = new MaterialItem(CONTENT_SNOW_BLOCK, 1);
			defs_init++;
		}

		// finished wood
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_FINISHED, 4);
			defs_init++;
		}

		// finished wood
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_JUNGLEWOOD);
			defs[defs_init].item = new MaterialItem(CONTENT_WOOD_FINISHED, 4);
			defs_init++;
		}

		// chair
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_FINISHED);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_FINISHED);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_CHAIR, 2);
			defs_init++;
		}

		// table
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_FINISHED);
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_FINISHED);
			defs[defs_init].specs[2] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD_FINISHED);
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[5] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_TABLE, 1);
			defs_init++;
		}

		// bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_BED_HEAD, 1);
			defs_init++;
		}

		// blue bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLUE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLUE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_BED_BLUE_HEAD, 1);
			defs_init++;
		}

		// red bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_blue");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_BED_HEAD);
			defs[defs_init].item = new MaterialItem(CONTENT_BED_BLUE_HEAD, 1);
			defs_init++;
		}

		// green bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_GREEN);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_GREEN);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_BED_GREEN_HEAD, 1);
			defs_init++;
		}

		// green bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_green");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_BED_HEAD);
			defs[defs_init].item = new MaterialItem(CONTENT_BED_GREEN_HEAD, 1);
			defs_init++;
		}

		// orange bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_ORANGE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_ORANGE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_BED_ORANGE_HEAD, 1);
			defs_init++;
		}

		// orange bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_orange");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_BED_HEAD);
			defs[defs_init].item = new MaterialItem(CONTENT_BED_ORANGE_HEAD, 1);
			defs_init++;
		}

		// purple bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_PURPLE);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_PURPLE);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_BED_PURPLE_HEAD, 1);
			defs_init++;
		}

		// purple bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_purple");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_BED_HEAD);
			defs[defs_init].item = new MaterialItem(CONTENT_BED_PURPLE_HEAD, 1);
			defs_init++;
		}

		// red bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_RED);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_RED);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_BED_RED_HEAD, 1);
			defs_init++;
		}

		// red bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_red");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_BED_HEAD);
			defs[defs_init].item = new MaterialItem(CONTENT_BED_RED_HEAD, 1);
			defs_init++;
		}

		// yellow bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_YELLOW);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_YELLOW);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_BED_YELLOW_HEAD, 1);
			defs_init++;
		}

		// yellow bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_yellow");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_BED_HEAD);
			defs[defs_init].item = new MaterialItem(CONTENT_BED_YELLOW_HEAD, 1);
			defs_init++;
		}

		// black bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[1] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON);
			defs[defs_init].specs[2] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[4] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLACK);
			defs[defs_init].specs[5] = ItemSpec(ITEM_MATERIAL, CONTENT_WOOD);
			defs[defs_init].specs[6] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].specs[7] = ItemSpec(ITEM_MATERIAL, CONTENT_COTTON_BLACK);
			defs[defs_init].specs[8] = ItemSpec(ITEM_CRAFT, "Stick");
			defs[defs_init].item = new MaterialItem(CONTENT_BED_BLACK_HEAD, 1);
			defs_init++;
		}

		// black bed
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "dye_black");
			defs[defs_init].specs[3] = ItemSpec(ITEM_MATERIAL, CONTENT_BED_HEAD);
			defs[defs_init].item = new MaterialItem(CONTENT_BED_BLACK_HEAD, 1);
			defs_init++;
		}

		// crowbar
		{
			defs[defs_init].specs[0] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].specs[3] = ItemSpec(ITEM_CRAFT, "steel_ingot");
			defs[defs_init].item = new ToolItem("crowbar", 0);
			defs_init++;
		}
	}

	for (int i=0; i<defs_init; i++) {
		if (defs[i] == items)
			return defs[i].item->clone();
	}

	return NULL;
}
#endif
