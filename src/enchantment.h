/************************************************************************
* enchantment.cpp
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
************************************************************************/

#ifndef ENCHANTMENT_HEADER
#define ENCHANTMENT_HEADER

#include <stdint.h>
#include <string>

#define ENCHANTMENT_NONE 0
#define ENCHANTMENT_FLAME 2
#define ENCHANTMENT_DONTBREAK 3
#define ENCHANTMENT_MORE 4
#define ENCHANTMENT_FAST 13
#define ENCHANTMENT_LONGLASTING 16
#define ENCHANTMENT_MAX 16

struct EnchantmentInfo {
	uint16_t type;
	uint8_t level;
	uint16_t mask;
	std::string overlay;
	std::wstring name;
};

bool enchantment_get(uint16_t *data, EnchantmentInfo *info);
bool enchantment_have(uint16_t data, uint16_t enchantment);

#endif
