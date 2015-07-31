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

#include "enchantment.h"
#include <stdio.h>

/*
1 - Aeterna ---
1 -           +
1 -           +
1 - Velox ---
1 -         +
1 -         +
1 -
1 -
1 -
1 -
1 -
1 -
1 - Amplius --------
1 - Indomitus ---  +
1 -             +  +
1 - Ignis       +
*/

EnchantmentInfo enchantments[ENCHANTMENT_MAX+1];
static int enchantment_isinit = 0;

static void enchantment_init()
{
	EnchantmentInfo *f;
	uint8_t i;

	for (i=0; i<=ENCHANTMENT_MAX; i++) {
		f = &enchantments[i];
		f->type = ENCHANTMENT_NONE;
		f->level = 0;
		f->mask = 0;
		f->overlay = "";
		f->name = L"";
	}

	i = ENCHANTMENT_FLAME;
	f = &enchantments[i];
	f->type = i;
	f->mask = (1<<(i-1));
	f->overlay = "flame";
	f->name = L"Ignis";

	i = ENCHANTMENT_DONTBREAK;
	f = &enchantments[i];
	f->type = i;
	f->mask = (1<<(i-1));
	f->overlay = "dontbreak";
	f->name = L"Indomitus";

	i = ENCHANTMENT_MORE;
	f = &enchantments[i];
	f->type = i;
	f->mask = (1<<(i-1));
	f->overlay = "more";
	f->name = L"Amplius";

	i = ENCHANTMENT_FAST;
	f = &enchantments[i];
	f->type = i;
	f->mask = (1<<(i-1));
	f->overlay = "fast";
	f->name = L"Velox";

	i = ENCHANTMENT_LONGLASTING;
	f = &enchantments[i];
	f->type = i;
	f->mask = (1<<(i-1));
	f->overlay = "longlast";
	f->name = L"Aeterna";

	enchantment_isinit = 1;
}

/* read and remove one enchantment from data,
 * call repeatedly to iterate over all enchantments */
bool enchantment_get(uint16_t *data, EnchantmentInfo *info)
{
	uint16_t d;
	int8_t i;
	uint8_t l = 0;
	if (!data)
		return false;

	printf("enchantment_get( %u , X) - START\n",*data);

	d = *data;
	if (d == ENCHANTMENT_NONE)
		return false;

	if (!enchantment_isinit)
		enchantment_init();

	for (i=15; i>-1; i--) {
		if ((d&(1<<i)) == 0)
			continue;
		if (enchantments[i+1].mask == (1<<i))
			break;
	}
	if (i < 0) {
		*data = 0;
		return false;
	}

	if (i > 2) {
		d = (d>>(i-2));
		d &= 0x0003;
		l = 1+d;
	}else{
		l = 1;
	}

	if (info) {
		*info = enchantments[i+1];
		info->level = l;
	}

	for (d=0; d<3 && i>-1; d++,i--) {
		(*data) &= ~(1<<i);
		printf("enchantment_get( %u , X) - TICK %u\n",*data,i);
	}
	printf("enchantment_get( %u , X) - END\n",*data);

	return true;
}
