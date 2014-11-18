/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mineral.h
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

#ifndef MINERAL_HEADER
#define MINERAL_HEADER

#include "inventory.h"

#define MINERAL_NONE 0
#define MINERAL_COAL 1
#define MINERAL_IRON 2
#define MINERAL_TIN 3
#define MINERAL_COPPER 4
#define MINERAL_SILVER 5
#define MINERAL_GOLD 6
#define MINERAL_QUARTZ 7

struct MineralFeatures {
	std::string texture;
	// the dug item
	content_t dug_item;
	// the number of dug_items received when dug
	s16 dug_count;

	MineralFeatures():
		texture(""),
		dug_item(CONTENT_IGNORE),
		dug_count(1)
	{}
};

/*
	Minerals

	Value is stored in the lowest 5 bits of a MapNode's CPT_MINERAL
	type param.
*/

// Caches textures
void init_mineral();
MineralFeatures & mineral_features(u8 i);

#define MINERAL_MAX 255

inline CraftItem * getDiggedMineralItem(u8 mineral)
{
	MineralFeatures m = mineral_features(mineral);
	if (m.dug_item == CONTENT_IGNORE)
		return NULL;

	return new CraftItem(content_craftitem_features(m.dug_item).name,m.dug_count);
}

#endif

