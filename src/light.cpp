/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* light.cpp
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

#include "light.h"

#if 1
// This is good
// a_n+1 = a_n * 0.786
// Length of LIGHT_MAX+1 means LIGHT_MAX is the last value.
// LIGHT_SUN is read as LIGHT_MAX from here.
u8 light_decode_table[LIGHT_MAX+1] =
{
8,
11,
14,
18,
22,
29,
37,
47,
60,
76,
97,
123,
157,
200,
255,
};
#else
// Use for debugging in dark
u8 light_decode_table[LIGHT_MAX+1] =
{
58,
64,
72,
80,
88,
98,
109,
121,
135,
150,
167,
185,
206,
229,
255,
};
#endif
