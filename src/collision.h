/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* collision.h
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

#ifndef COLLISION_HEADER
#define COLLISION_HEADER

#include "common_irrlicht.h"
#include <vector>

class Map;

enum CollisionType
{
	COLLISION_FALL,
	COLLISION_NODE
};

struct CollisionInfo
{
	CollisionType t;
	f32 speed;
	v3s16 node_p; // COLLISION_NODE
	v3f old_speed;
	v3f new_speed;

	CollisionInfo():
		t(COLLISION_NODE),
		node_p(-32768,-32768,-32768),
		old_speed(0,0,0),
		new_speed(0,0,0)
	{}
};

struct collisionMoveResult
{
	bool touching_ground;
	bool in_liquid;
	bool touching_lethal;
	bool collides;
	bool collides_xz;
	bool standing_on_unloaded;
	std::vector<CollisionInfo> collisions;

	collisionMoveResult():
		touching_ground(false),
		in_liquid(false),
		touching_lethal(false),
		collides(false),
		collides_xz(false),
		standing_on_unloaded(false)
	{}
};

// Moves using a single iteration; speed should not exceed pos_max_d/dtime
collisionMoveResult collisionMoveSimple(Map *map,
		f32 pos_max_d, const aabb3f &box_0,
		f32 stepheight, f32 dtime,
		v3f &pos_f, v3f &speed_f, v3f &accel_f);

#endif

