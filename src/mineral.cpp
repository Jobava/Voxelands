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

CraftItem *getDiggedMineralItem(u8 mineral, Player *player, InventoryItem *tool)
{
	MineralFeatures m = mineral_features(mineral);
	if (m.dug_item == CONTENT_IGNORE)
		return NULL;

	if (content_craftitem_features(m.dug_item).content == CONTENT_IGNORE)
		return NULL;

	if (!tool && m.min_level > 0)
		return NULL;

	ToolItemFeatures *t = &content_toolitem_features(tool->getContent());
	if (t->content == CONTENT_IGNORE && m.min_level > 0)
		return NULL;

	if (t->level < m.min_level)
		return NULL;

	u16 count = m.dug_count_min;
	if (m.dug_count_min != m.dug_count_max && t->level > count) {
		count = myrand_range(m.dug_count_min,t->level);
		if (count > m.dug_count_max)
			count = m.dug_count_max;
	}

	return new CraftItem(m.dug_item, count, 0);
}

void init_mineral()
{
	u8 i;
	MineralFeatures *f = NULL;

	i = MINERAL_COAL;
	f = &mineral_features(i);
	f->texture = "mineral_coal.png";
	f->dug_item = CONTENT_CRAFTITEM_COAL;

	i = MINERAL_IRON;
	f = &mineral_features(i);
	f->texture = "mineral_iron.png";
	f->dug_item = CONTENT_CRAFTITEM_IRON;
	f->min_level = 2;

	i = MINERAL_TIN;
	f = &mineral_features(i);
	f->texture = "mineral_tin.png";
	f->dug_item = CONTENT_CRAFTITEM_TIN;
	f->min_level = 2;

	i = MINERAL_COPPER;
	f = &mineral_features(i);
	f->texture = "mineral_copper.png";
	f->dug_item = CONTENT_CRAFTITEM_COPPER;
	f->dug_count_max = 4;
	f->min_level = 2;

	i = MINERAL_SILVER;
	f = &mineral_features(i);
	f->texture = "mineral_silver.png";
	f->dug_item = CONTENT_CRAFTITEM_SILVER;
	f->min_level = 3;

	i = MINERAL_GOLD;
	f = &mineral_features(i);
	f->texture = "mineral_gold.png";
	f->dug_item = CONTENT_CRAFTITEM_GOLD;
	f->min_level = 3;

	i = MINERAL_QUARTZ;
	f = &mineral_features(i);
	f->texture = "mineral_quartz.png";
	f->dug_item = CONTENT_CRAFTITEM_QUARTZ;
	f->min_level = 3;
}
