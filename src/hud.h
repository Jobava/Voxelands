/************************************************************************
* hud.h
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2015 <lisa@ltmnet.com>
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

#ifndef HUD_HEADER
#define HUD_HEADER

#include "common_irrlicht.h"
#include "inventory.h"


void draw_image(
	video::IVideoDriver *driver,
	video::ITexture *texture,
	const video::SColor &colour,
	core::rect<s32> drect,
	const core::rect<s32> *srect,
	const core::rect<s32> *clip
);
void draw_inv_item(
	video::IVideoDriver *driver,
	gui::IGUIFont *font,
	InventoryItem *item,
	core::rect<s32> rect,
	const core::rect<s32> *clip
);
void draw_progress_ring(
	video::IVideoDriver *driver,
	v2s32 screensize,
	core::position2d<s32> pos,
	s32 radius,
	s16 value,
	video::SColor color
);

void hud_draw_old(
	video::IVideoDriver *driver,
	gui::IGUIFont *font,
	v2s32 centrelowerpos,
	s32 imgsize,
	s32 itemcount,
	Inventory *inventory,
	s32 halfheartcount,
	s32 halfbubblecount,
	s32 halfhungercount
);
void hud_draw(
	video::IVideoDriver *driver,
	gui::IGUIFont *font,
	v2s32 screensize,
	s32 imgsize,
	s32 itemcount,
	bool show_index,
	Inventory *inventory,
	bool have_health,
	s32 halfheartcount,
	float cold_boost,
	bool have_suffocation,
	s32 halfbubblecount,
	bool have_hunger,
	s32 halfhungercount,
	float energy,
	float energy_boost,
	int crosshair,
	bool nodeinfo,
	bool selected,
	v3s16 pos,
	MapNode node
);

#endif
