/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mapgen.h
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

#ifndef MAPGEN_HEADER
#define MAPGEN_HEADER

#include "common_irrlicht.h"
#include "utility.h" // UniqueQueue

class MapBlock;
class ManualMapVoxelManipulator;

enum MapGenType {
	MGT_FLAT = 0,
	MGT_FLATTER,
	MGT_SMOOTHER,
	MGT_DEFAULT,
	MGT_HILLY,
	MGT_MOUNTAINS,
	MGT_CRAZY,
	MGT_CRAZYHILLS
};

namespace mapgen
{
	struct BlockMakeData
	{
		bool no_op;
		ManualMapVoxelManipulator *vmanip;
		uint64_t seed;
		MapGenType type;
		v3s16 blockpos;
		UniqueQueue<v3s16> transforming_liquid;

		BlockMakeData();
		~BlockMakeData();
	};

	// Finds precise ground level at any position
	s16 find_ground_level_from_noise(BlockMakeData *data, v2s16 p2d, s16 precision);

	// Find out if block is completely underground
	bool block_is_underground(BlockMakeData *data, v3s16 blockpos);

	// Main map generation routine
	void make_block(BlockMakeData *data);

	// Add a tree
	void make_tree(ManualMapVoxelManipulator &vmanip, v3s16 p0);
	// Add an apple tree
	void make_appletree(ManualMapVoxelManipulator &vmanip, v3s16 p0);
	// Add a large tree
	void make_largetree(ManualMapVoxelManipulator &vmanip, v3s16 p0);
	// Add a jungle tree
	void make_jungletree(ManualMapVoxelManipulator &vmanip, v3s16 p0);
	// Add a conifer tree
	void make_conifertree(ManualMapVoxelManipulator &vmanip, v3s16 p0);

	/*
		These are used by FarMesh
	*/
	bool get_have_sand(uint64_t seed, v2s16 p2d);
	double tree_amount_2d(uint64_t seed, v2s16 p);

}; // namespace mapgen

#endif

