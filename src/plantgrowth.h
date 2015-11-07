/************************************************************************
* plantgrowth.h
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

#ifndef PLANTGROWTH_HEADER
#define PLANTGROWTH_HEADER

void plantgrowth_tree(ServerEnvironment *env, v3s16 p0);
void plantgrowth_appletree(ServerEnvironment *env, v3s16 p0);
void plantgrowth_conifertree(ServerEnvironment *env, v3s16 p0);
void plantgrowth_largetree(ServerEnvironment *env, v3s16 p0);
void plantgrowth_jungletree(ServerEnvironment *env, v3s16 p0);
void plantgrowth_fertilizer(ServerEnvironment *env, v3s16 p0);
void plantgrowth_seed(ServerEnvironment *env, v3s16 p0);
void plantgrowth_plant(ServerEnvironment *env, v3s16 p0, s16 height=0);
void plantgrowth_grass(ServerEnvironment *env, v3s16 p0);
void plantgrowth_cactus(ServerEnvironment *env, v3s16 p0);

#endif
