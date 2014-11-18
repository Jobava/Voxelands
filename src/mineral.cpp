/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mineral.cpp
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

#include "mineral.h"

struct MineralFeatures g_mineral_features[MINERAL_MAX+1];

MineralFeatures & mineral_features(u8 i)
{
	return g_mineral_features[i];
}

void init_mineral()
{
	u8 i;
	MineralFeatures *f = NULL;

	i = MINERAL_COAL;
	f = &mineral_features(i);
	f->texture = "mineral_coal.png";
	f->dug_item = CONTENT_CRAFTITEM_COAL;
	f->dug_count = 2;

	i = MINERAL_IRON;
	f = &mineral_features(i);
	f->texture = "mineral_iron.png";
	f->dug_item = CONTENT_CRAFTITEM_IRON;
	f->dug_count = 2;

	i = MINERAL_TIN;
	f = &mineral_features(i);
	f->texture = "mineral_tin.png";
	f->dug_item = CONTENT_CRAFTITEM_TIN;
	f->dug_count = 2;

	i = MINERAL_COPPER;
	f = &mineral_features(i);
	f->texture = "mineral_copper.png";
	f->dug_item = CONTENT_CRAFTITEM_COPPER;
	f->dug_count = 2;

	i = MINERAL_SILVER;
	f = &mineral_features(i);
	f->texture = "mineral_silver.png";
	f->dug_item = CONTENT_CRAFTITEM_SILVER;

	i = MINERAL_GOLD;
	f = &mineral_features(i);
	f->texture = "mineral_gold.png";
	f->dug_item = CONTENT_CRAFTITEM_GOLD;

	i = MINERAL_QUARTZ;
	f = &mineral_features(i);
	f->texture = "mineral_quartz.png";
	f->dug_item = CONTENT_CRAFTITEM_QUARTZ;
}
