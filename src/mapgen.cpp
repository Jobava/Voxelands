/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mapgen.cpp
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

#include "mapgen.h"
#include "voxel.h"
#include "content_mapnode.h"
#include "noise.h"
#include "mapblock.h"
#include "map.h"
#include "mineral.h"
#include "content_sao.h"

namespace mapgen
{

/*
	Some helper functions for the map generator
*/

static void make_tree(ManualMapVoxelManipulator &vmanip, v3s16 p0)
{
	MapNode treenode(CONTENT_TREE);
	MapNode leavesnode(CONTENT_LEAVES);

	s16 trunk_h = myrand_range(5,6);
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		if (vmanip.m_area.contains(p1))
			vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	p1.Y -= 1;

	VoxelArea leaves_a(v3s16(-3,-1,-3), v3s16(3,2,3));
	Buffer<u8> leaves_d(leaves_a.getVolume());
	for (s32 i=0; i<leaves_a.getVolume(); i++) {
		leaves_d[i] = 0;
	}

	// Force leaves at near the end of the trunk
	s16 rad = 3;
	for (s16 y=-1; y<=1; y++) {
	for (s16 z=-rad; z<=rad; z++) {
	for (s16 x=-rad; x<=rad; x++) {
		if (rad < 3 || (z > -rad && z < rad) || (x > -rad && x < rad))
			leaves_d[leaves_a.index(v3s16(x,y,z))] = 1;
	}
	}
		rad--;
	}

	// Add leaves randomly
	for (u32 iii=0; iii<7; iii++) {
		s16 d = 1;

		v3s16 p(
			myrand_range(leaves_a.MinEdge.X, leaves_a.MaxEdge.X-d),
			myrand_range(leaves_a.MinEdge.Y, leaves_a.MaxEdge.Y-d),
			myrand_range(leaves_a.MinEdge.Z, leaves_a.MaxEdge.Z-d)
		);

		for (s16 z=0; z<=d; z++) {
		for (s16 y=0; y<=d; y++) {
		for (s16 x=0; x<=d; x++) {
			leaves_d[leaves_a.index(p+v3s16(x,y,z))] = 1;
		}
		}
		}
	}

	// Blit leaves to vmanip
	for (s16 z=leaves_a.MinEdge.Z; z<=leaves_a.MaxEdge.Z; z++) {
	for (s16 y=leaves_a.MinEdge.Y; y<=leaves_a.MaxEdge.Y; y++) {
	for (s16 x=leaves_a.MinEdge.X; x<=leaves_a.MaxEdge.X; x++) {
		v3s16 p(x,y,z);
		p += p1;
		if (vmanip.m_area.contains(p) == false)
			continue;
		u32 vi = vmanip.m_area.index(p);
		if (vmanip.m_data[vi].getContent() != CONTENT_AIR
				&& vmanip.m_data[vi].getContent() != CONTENT_IGNORE)
			continue;
		u32 i = leaves_a.index(x,y,z);
		if (leaves_d[i] == 1)
			vmanip.m_data[vi] = leavesnode;
	}
	}
	}
}

static void make_appletree(ManualMapVoxelManipulator &vmanip, v3s16 p0)
{
	MapNode treenode(CONTENT_APPLE_TREE);
	MapNode leavesnode(CONTENT_APPLE_LEAVES);
	MapNode applenode(CONTENT_APPLE);

	s16 trunk_h = myrand_range(4, 5);
	v3s16 p1 = p0;
	for(s16 ii=0; ii<trunk_h; ii++)
	{
		if(vmanip.m_area.contains(p1))
			vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	p1.Y -= 1;

	VoxelArea leaves_a(v3s16(-2,-1,-2), v3s16(2,2,2));
	Buffer<u8> leaves_d(leaves_a.getVolume());
	for(s32 i=0; i<leaves_a.getVolume(); i++)
		leaves_d[i] = 0;

	// Force leaves at near the end of the trunk
	{
		s16 d = 1;
		for(s16 z=-d; z<=d; z++)
		for(s16 y=-d; y<=d; y++)
		for(s16 x=-d; x<=d; x++)
		{
			leaves_d[leaves_a.index(v3s16(x,y,z))] = 1;
		}
	}

	// Add leaves randomly
	for(u32 iii=0; iii<7; iii++)
	{
		s16 d = 1;

		v3s16 p(
			myrand_range(leaves_a.MinEdge.X, leaves_a.MaxEdge.X-d),
			myrand_range(leaves_a.MinEdge.Y, leaves_a.MaxEdge.Y-d),
			myrand_range(leaves_a.MinEdge.Z, leaves_a.MaxEdge.Z-d)
		);

		for(s16 z=0; z<=d; z++)
		for(s16 y=0; y<=d; y++)
		for(s16 x=0; x<=d; x++)
		{
			leaves_d[leaves_a.index(p+v3s16(x,y,z))] = 1;
		}
	}

	// not all apple trees have apples
	bool have_fruit = (myrand_range(0,4) == 0);

	// Blit leaves to vmanip
	for(s16 z=leaves_a.MinEdge.Z; z<=leaves_a.MaxEdge.Z; z++)
	for(s16 y=leaves_a.MinEdge.Y; y<=leaves_a.MaxEdge.Y; y++)
	for(s16 x=leaves_a.MinEdge.X; x<=leaves_a.MaxEdge.X; x++)
	{
		v3s16 p(x,y,z);
		p += p1;
		if (vmanip.m_area.contains(p) == false)
			continue;
		u32 vi = vmanip.m_area.index(p);
		if (vmanip.m_data[vi].getContent() != CONTENT_AIR && vmanip.m_data[vi].getContent() != CONTENT_IGNORE)
			continue;
		u32 i = leaves_a.index(x,y,z);
		if (leaves_d[i] == 1) {
			bool is_apple = myrand_range(0,99) < 10;
			if (have_fruit && is_apple) {
				vmanip.m_data[vi] = applenode;
			}else{
				vmanip.m_data[vi] = leavesnode;
			}
		}
	}
}

static void make_conifertree(ManualMapVoxelManipulator &vmanip, v3s16 p0)
{
	MapNode treenode(CONTENT_CONIFER_TREE);
	MapNode leavesnode(CONTENT_CONIFER_LEAVES);

	s16 trunk_h = myrand_range(8, 11);
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		if(vmanip.m_area.contains(p1))
			vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	if (vmanip.m_area.contains(p1+v3s16(0,1,0)))
		vmanip.m_data[vmanip.m_area.index(p1+v3s16(0,1,0))] = leavesnode;
	for(s16 z=-1; z<=1; z++)
	for(s16 y=-2; y<=0; y++)
	for(s16 x=-1; x<=1; x++)
	{
		if (!x && !z)
			continue;
		if (vmanip.m_area.contains(p1+v3s16(x,y,z)))
			vmanip.m_data[vmanip.m_area.index(p1+v3s16(x,y,z))] = leavesnode;
	}
	for(s16 z=-2; z<=2; z++)
	for(s16 y=-5; y<-2; y++)
	for(s16 x=-2; x<=2; x++)
	{
		if (!x && !z)
			continue;
		if (vmanip.m_area.contains(p1+v3s16(x,y,z)))
			vmanip.m_data[vmanip.m_area.index(p1+v3s16(x,y,z))] = leavesnode;
	}

}

static void make_largetree(ManualMapVoxelManipulator &vmanip, v3s16 p0)
{
	MapNode treenode(CONTENT_TREE);
	MapNode leavesnode(CONTENT_LEAVES);

	s16 trunk_h = myrand_range(10, 12);
	v3s16 p1 = p0;
	for(s16 ii=0; ii<trunk_h; ii++)
	{
		if(vmanip.m_area.contains(p1))
			vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	p1.Y -= 1;

	VoxelArea leaves_a(v3s16(-3,-3,-3), v3s16(3,3,3));
	Buffer<u8> leaves_d(leaves_a.getVolume());
	for(s32 i=0; i<leaves_a.getVolume(); i++)
		leaves_d[i] = 0;

	for (s16 k=0; k<5; k++) {
		if (k == 1) {
			p1.Y -= 3;
			for (s16 ki=0; ki<4; ki++) {
				p1.X++;
				vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
			}
			p1.X--;
		}else if (k == 2) {
			p1.X -= 3;
			for (s16 ki=0; ki<4; ki++) {
				p1.X--;
				vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
			}
			p1.X++;
		}else if (k == 3) {
			p1.X += 3;
			for (s16 ki=0; ki<4; ki++) {
				p1.Z++;
				vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
			}
			p1.Z--;
		}else if (k == 4) {
			p1.Z -= 3;
			for (s16 ki=0; ki<4; ki++) {
				p1.Z--;
				vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
			}
			p1.Z++;
		}else{
			vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
		}
		// Force leaves at near the end of the trunk
		{
			s16 d = 1;
			for(s16 z=-d; z<=d; z++)
			for(s16 y=-d; y<=d; y++)
			for(s16 x=-d; x<=d; x++)
			{
				leaves_d[leaves_a.index(v3s16(x,y,z))] = 1;
			}
		}

		if (k) {
			// Add leaves randomly
			for (u32 iii=0; iii<10; iii++) {
				s16 d = 1;

				v3s16 p(
					myrand_range(leaves_a.MinEdge.X, leaves_a.MaxEdge.X-d),
					myrand_range(leaves_a.MinEdge.Y, leaves_a.MaxEdge.Y-d),
					myrand_range(leaves_a.MinEdge.Z, leaves_a.MaxEdge.Z-d)
				);

				for(s16 z=0; z<=d; z++)
				for(s16 y=0; y<=d; y++)
				for(s16 x=0; x<=d; x++)
				{
					leaves_d[leaves_a.index(p+v3s16(x,y,z))] = 1;
				}
			}
		}

		// Blit leaves to vmanip
		for(s16 z=leaves_a.MinEdge.Z; z<=leaves_a.MaxEdge.Z; z++)
		for(s16 y=leaves_a.MinEdge.Y; y<=leaves_a.MaxEdge.Y; y++)
		for(s16 x=leaves_a.MinEdge.X; x<=leaves_a.MaxEdge.X; x++)
		{
			v3s16 p(x,y,z);
			p += p1;
			if(vmanip.m_area.contains(p) == false)
				continue;
			u32 vi = vmanip.m_area.index(p);
			if(vmanip.m_data[vi].getContent() != CONTENT_AIR
					&& vmanip.m_data[vi].getContent() != CONTENT_IGNORE)
				continue;
			u32 i = leaves_a.index(x,y,z);
			if(leaves_d[i] == 1)
				vmanip.m_data[vi] = leavesnode;
		}
	}
}

static void make_jungletree(ManualMapVoxelManipulator &vmanip, v3s16 p0)
{
	MapNode treenode(CONTENT_JUNGLETREE);
	MapNode leavesnode(CONTENT_JUNGLELEAVES);

	for(s16 x=-1; x<=1; x++)
	for(s16 z=-1; z<=1; z++)
	{
		if(myrand_range(0, 2) == 0)
			continue;
		v3s16 p1 = p0 + v3s16(x,0,z);
		v3s16 p2 = p0 + v3s16(x,-1,z);
		if(vmanip.m_area.contains(p2)
				&& vmanip.m_data[vmanip.m_area.index(p2)] == CONTENT_AIR)
			vmanip.m_data[vmanip.m_area.index(p2)] = treenode;
		else if(vmanip.m_area.contains(p1))
			vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
	}

	s16 trunk_h = myrand_range(8, 12);
	v3s16 p1 = p0;
	for(s16 ii=0; ii<trunk_h; ii++)
	{
		if(vmanip.m_area.contains(p1))
			vmanip.m_data[vmanip.m_area.index(p1)] = treenode;
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	p1.Y -= 1;

	VoxelArea leaves_a(v3s16(-3,-2,-3), v3s16(3,2,3));
	//SharedPtr<u8> leaves_d(new u8[leaves_a.getVolume()]);
	Buffer<u8> leaves_d(leaves_a.getVolume());
	for(s32 i=0; i<leaves_a.getVolume(); i++)
		leaves_d[i] = 0;

	// Force leaves at near the end of the trunk
	{
		s16 d = 1;
		for(s16 z=-d; z<=d; z++)
		for(s16 y=-d; y<=d; y++)
		for(s16 x=-d; x<=d; x++)
		{
			leaves_d[leaves_a.index(v3s16(x,y,z))] = 1;
		}
	}

	// Add leaves randomly
	for(u32 iii=0; iii<30; iii++)
	{
		s16 d = 1;

		v3s16 p(
			myrand_range(leaves_a.MinEdge.X, leaves_a.MaxEdge.X-d),
			myrand_range(leaves_a.MinEdge.Y, leaves_a.MaxEdge.Y-d),
			myrand_range(leaves_a.MinEdge.Z, leaves_a.MaxEdge.Z-d)
		);

		for(s16 z=0; z<=d; z++)
		for(s16 y=0; y<=d; y++)
		for(s16 x=0; x<=d; x++)
		{
			leaves_d[leaves_a.index(p+v3s16(x,y,z))] = 1;
		}
	}

	// Blit leaves to vmanip
	for(s16 z=leaves_a.MinEdge.Z; z<=leaves_a.MaxEdge.Z; z++)
	for(s16 y=leaves_a.MinEdge.Y; y<=leaves_a.MaxEdge.Y; y++)
	for(s16 x=leaves_a.MinEdge.X; x<=leaves_a.MaxEdge.X; x++)
	{
		v3s16 p(x,y,z);
		p += p1;
		if(vmanip.m_area.contains(p) == false)
			continue;
		u32 vi = vmanip.m_area.index(p);
		if(vmanip.m_data[vi].getContent() != CONTENT_AIR
				&& vmanip.m_data[vi].getContent() != CONTENT_IGNORE)
			continue;
		u32 i = leaves_a.index(x,y,z);
		if(leaves_d[i] == 1)
			vmanip.m_data[vi] = leavesnode;
	}
}

static void make_papyrus(VoxelManipulator &vmanip, v3s16 p0)
{
	MapNode papyrusnode(CONTENT_PAPYRUS);

	s16 trunk_h = myrand_range(2, 3);
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		if (vmanip.m_area.contains(p1))
			vmanip.m_data[vmanip.m_area.index(p1)] = papyrusnode;
		p1.Y++;
	}
}

static void make_cactus(VoxelManipulator &vmanip, v3s16 p0)
{
	MapNode cactusnode(CONTENT_CACTUS);

	s16 trunk_h = 3;
	if (myrand()%5000 == 0)
		trunk_h = 4;
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		if (vmanip.m_area.contains(p1))
			vmanip.m_data[vmanip.m_area.index(p1)] = cactusnode;
		p1.Y++;
	}
}

/*
	Dungeon making routines
*/

#define VMANIP_FLAG_DUNGEON_INSIDE VOXELFLAG_CHECKED1
#define VMANIP_FLAG_DUNGEON_PRESERVE VOXELFLAG_CHECKED2
#define VMANIP_FLAG_DUNGEON_UNTOUCHABLE (\
		VMANIP_FLAG_DUNGEON_INSIDE|VMANIP_FLAG_DUNGEON_PRESERVE)

static void make_room1(VoxelManipulator &vmanip, v3s16 roomsize, v3s16 roomplace)
{
	// Make +-X walls
	for (s16 z=0; z<roomsize.Z; z++)
	for (s16 y=0; y<roomsize.Y; y++) {
		{
			v3s16 p = roomplace + v3s16(0,y,z);
			if (vmanip.m_area.contains(p) == false)
				continue;
			u32 vi = vmanip.m_area.index(p);
			if (vmanip.m_flags[vi] & VMANIP_FLAG_DUNGEON_UNTOUCHABLE)
				continue;
			vmanip.m_data[vi] = MapNode(CONTENT_COBBLE);
		}
		{
			v3s16 p = roomplace + v3s16(roomsize.X-1,y,z);
			if (vmanip.m_area.contains(p) == false)
				continue;
			u32 vi = vmanip.m_area.index(p);
			if (vmanip.m_flags[vi] & VMANIP_FLAG_DUNGEON_UNTOUCHABLE)
				continue;
			vmanip.m_data[vi] = MapNode(CONTENT_COBBLE);
		}
	}

	// Make +-Z walls
	for (s16 x=0; x<roomsize.X; x++)
	for (s16 y=0; y<roomsize.Y; y++) {
		{
			v3s16 p = roomplace + v3s16(x,y,0);
			if (vmanip.m_area.contains(p) == false)
				continue;
			u32 vi = vmanip.m_area.index(p);
			if (vmanip.m_flags[vi] & VMANIP_FLAG_DUNGEON_UNTOUCHABLE)
				continue;
			vmanip.m_data[vi] = MapNode(CONTENT_COBBLE);
		}
		{
			v3s16 p = roomplace + v3s16(x,y,roomsize.Z-1);
			if (vmanip.m_area.contains(p) == false)
				continue;
			u32 vi = vmanip.m_area.index(p);
			if (vmanip.m_flags[vi] & VMANIP_FLAG_DUNGEON_UNTOUCHABLE)
				continue;
			vmanip.m_data[vi] = MapNode(CONTENT_COBBLE);
		}
	}

	// Make +-Y walls (floor and ceiling)
	for (s16 z=0; z<roomsize.Z; z++)
	for (s16 x=0; x<roomsize.X; x++) {
		{
			v3s16 p = roomplace + v3s16(x,0,z);
			if (vmanip.m_area.contains(p) == false)
				continue;
			u32 vi = vmanip.m_area.index(p);
			if (vmanip.m_flags[vi] & VMANIP_FLAG_DUNGEON_UNTOUCHABLE)
				continue;
			vmanip.m_data[vi] = MapNode(CONTENT_COBBLE);
		}
		{
			v3s16 p = roomplace + v3s16(x,roomsize.Y-1,z);
			if (vmanip.m_area.contains(p) == false)
				continue;
			u32 vi = vmanip.m_area.index(p);
			if (vmanip.m_flags[vi] & VMANIP_FLAG_DUNGEON_UNTOUCHABLE)
				continue;
			vmanip.m_data[vi] = MapNode(CONTENT_COBBLE);
		}
	}

	// Fill with air
	for (s16 z=1; z<roomsize.Z-1; z++)
	for (s16 y=1; y<roomsize.Y-1; y++)
	for (s16 x=1; x<roomsize.X-1; x++) {
		v3s16 p = roomplace + v3s16(x,y,z);
		if (vmanip.m_area.contains(p) == false)
			continue;
		u32 vi = vmanip.m_area.index(p);
		vmanip.m_flags[vi] |= VMANIP_FLAG_DUNGEON_UNTOUCHABLE;
		vmanip.m_data[vi] = MapNode(CONTENT_AIR);
	}
}

static void make_fill(VoxelManipulator &vmanip, v3s16 place, v3s16 size,
		u8 avoid_flags, MapNode n, u8 or_flags)
{
	for (s16 z=0; z<size.Z; z++)
	for (s16 y=0; y<size.Y; y++)
	for (s16 x=0; x<size.X; x++) {
		v3s16 p = place + v3s16(x,y,z);
		if (vmanip.m_area.contains(p) == false)
			continue;
		u32 vi = vmanip.m_area.index(p);
		if (vmanip.m_flags[vi] & avoid_flags)
			continue;
		if (
			vmanip.m_data[vi].getContent() != CONTENT_STONE
			&& vmanip.m_data[vi].getContent() != CONTENT_SAND
			&& vmanip.m_data[vi].getContent() != CONTENT_GRAVEL
			&& vmanip.m_data[vi].getContent() != CONTENT_COBBLE
			&& vmanip.m_data[vi].getContent() != CONTENT_IGNORE
		)
			continue;
		vmanip.m_flags[vi] |= or_flags;
		vmanip.m_data[vi] = n;
	}
}

static void make_hole1(VoxelManipulator &vmanip, v3s16 place)
{
	make_fill(vmanip, place, v3s16(1,2,1), 0, MapNode(CONTENT_AIR), VMANIP_FLAG_DUNGEON_INSIDE);
}

static void make_door1(VoxelManipulator &vmanip, v3s16 doorplace, v3s16 doordir)
{
	make_hole1(vmanip, doorplace);
}

static v3s16 rand_ortho_dir(PseudoRandom &random)
{
	if (random.next()%2==0) {
		return random.next()%2 ? v3s16(-1,0,0) : v3s16(1,0,0);
	}else{
		return random.next()%2 ? v3s16(0,0,-1) : v3s16(0,0,1);
	}
}

static v3s16 turn_xz(v3s16 olddir, int t)
{
	v3s16 dir;
	if (t == 0) {
		// Turn right
		dir.X = olddir.Z;
		dir.Z = -olddir.X;
		dir.Y = olddir.Y;
	}else{
		// Turn left
		dir.X = -olddir.Z;
		dir.Z = olddir.X;
		dir.Y = olddir.Y;
	}
	return dir;
}

static v3s16 random_turn(PseudoRandom &random, v3s16 olddir)
{
	int turn = random.range(0,2);
	v3s16 dir;
	if (turn == 0) {
		// Go straight
		dir = olddir;
	}else if (turn == 1) {
		// Turn right
		dir = turn_xz(olddir, 0);
	}else{
		// Turn left
		dir = turn_xz(olddir, 1);
	}
	return dir;
}

static void make_corridor(VoxelManipulator &vmanip, v3s16 doorplace,
		v3s16 doordir, v3s16 &result_place, v3s16 &result_dir,
		PseudoRandom &random)
{
	make_hole1(vmanip, doorplace);
	v3s16 p0 = doorplace;
	v3s16 dir = doordir;
	u32 length;
	if (random.next()%2) {
		length = random.range(1,13);
	}else{
		length = random.range(1,6);
	}
	length = random.range(1,13);
	u32 partlength = random.range(1,13);
	u32 partcount = 0;
	s16 make_stairs = 0;
	if (random.next()%2 == 0 && partlength >= 3)
		make_stairs = random.next()%2 ? 1 : -1;

	for (u32 i=0; i<length; i++) {
		v3s16 p = p0 + dir;
		if (partcount != 0)
			p.Y += make_stairs;

		if (vmanip.m_area.contains(p) == true && vmanip.m_area.contains(p+v3s16(0,1,0)) == true) {
			if (make_stairs) {
				make_fill(vmanip, p+v3s16(-1,-1,-1), v3s16(3,5,3), VMANIP_FLAG_DUNGEON_UNTOUCHABLE, MapNode(CONTENT_COBBLE), 0);
				make_fill(vmanip, p, v3s16(1,2,1), 0, MapNode(CONTENT_AIR), VMANIP_FLAG_DUNGEON_INSIDE);
				make_fill(vmanip, p-dir, v3s16(1,2,1), 0, MapNode(CONTENT_AIR), VMANIP_FLAG_DUNGEON_INSIDE);
			}else{
				make_fill(vmanip, p+v3s16(-1,-1,-1), v3s16(3,4,3), VMANIP_FLAG_DUNGEON_UNTOUCHABLE, MapNode(CONTENT_COBBLE), 0);
				make_hole1(vmanip, p);
			}

			p0 = p;
		}else{
			// Can't go here, turn away
			dir = turn_xz(dir, random.range(0,1));
			make_stairs = -make_stairs;
			partcount = 0;
			partlength = random.range(1,length);
			continue;
		}

		partcount++;
		if (partcount >= partlength) {
			partcount = 0;

			dir = random_turn(random, dir);

			partlength = random.range(1,length);

			make_stairs = 0;
			if (random.next()%2 == 0 && partlength >= 3)
				make_stairs = random.next()%2 ? 1 : -1;
		}
	}
	result_place = p0;
	result_dir = dir;
}

class RoomWalker
{
public:

	RoomWalker(VoxelManipulator &vmanip_, v3s16 pos, PseudoRandom &random):
			vmanip(vmanip_),
			m_pos(pos),
			m_random(random)
	{
		randomizeDir();
	}

	void randomizeDir()
	{
		m_dir = rand_ortho_dir(m_random);
	}

	void setPos(v3s16 pos)
	{
		m_pos = pos;
	}

	void setDir(v3s16 dir)
	{
		m_dir = dir;
	}

	bool findPlaceForDoor(v3s16 &result_place, v3s16 &result_dir)
	{
		for (u32 i=0; i<100; i++) {
			v3s16 p = m_pos + m_dir;
			v3s16 p1 = p + v3s16(0,1,0);
			if (
				vmanip.m_area.contains(p) == false
				|| vmanip.m_area.contains(p1) == false
				|| i % 4 == 0
			) {
				randomizeDir();
				continue;
			}
			if (
				vmanip.getNodeNoExNoEmerge(p).getContent() == CONTENT_COBBLE
				&& vmanip.getNodeNoExNoEmerge(p1).getContent() == CONTENT_COBBLE
			) {
				// Found wall, this is a good place!
				result_place = p;
				result_dir = m_dir;
				// Randomize next direction
				randomizeDir();
				return true;
			}
			/*
				Determine where to move next
			*/
			// Jump one up if the actual space is there
			if (
				vmanip.getNodeNoExNoEmerge(p+v3s16(0,0,0)).getContent() == CONTENT_COBBLE
				&& vmanip.getNodeNoExNoEmerge(p+v3s16(0,1,0)).getContent() == CONTENT_AIR
				&& vmanip.getNodeNoExNoEmerge(p+v3s16(0,2,0)).getContent() == CONTENT_AIR
			)
				p += v3s16(0,1,0);
			// Jump one down if the actual space is there
			if (
				vmanip.getNodeNoExNoEmerge(p+v3s16(0,1,0)).getContent() == CONTENT_COBBLE
				&& vmanip.getNodeNoExNoEmerge(p+v3s16(0,0,0)).getContent() == CONTENT_AIR
				&& vmanip.getNodeNoExNoEmerge(p+v3s16(0,-1,0)).getContent() == CONTENT_AIR
			)
				p += v3s16(0,-1,0);
			// Check if walking is now possible
			if (
				vmanip.getNodeNoExNoEmerge(p).getContent() != CONTENT_AIR
				|| vmanip.getNodeNoExNoEmerge(p+v3s16(0,1,0)).getContent() != CONTENT_AIR
			) {
				// Cannot continue walking here
				randomizeDir();
				continue;
			}
			// Move there
			m_pos = p;
		}
		return false;
	}

	bool findPlaceForRoomDoor(v3s16 roomsize, v3s16 &result_doorplace, v3s16 &result_doordir, v3s16 &result_roomplace)
	{
		for (s16 trycount=0; trycount<30; trycount++) {
			v3s16 doorplace;
			v3s16 doordir;
			bool r = findPlaceForDoor(doorplace, doordir);
			if (r == false)
				continue;
			v3s16 roomplace;
			// X east, Z north, Y up
			if (doordir == v3s16(1,0,0)) // X+
				roomplace = doorplace + v3s16(0,-1,m_random.range(-roomsize.Z+2,-2));
			if (doordir == v3s16(-1,0,0)) // X-
				roomplace = doorplace + v3s16(-roomsize.X+1,-1,m_random.range(-roomsize.Z+2,-2));
			if (doordir == v3s16(0,0,1)) // Z+
				roomplace = doorplace + v3s16(m_random.range(-roomsize.X+2,-2),-1,0);
			if (doordir == v3s16(0,0,-1)) // Z-
				roomplace = doorplace + v3s16(m_random.range(-roomsize.X+2,-2),-1,-roomsize.Z+1);

			// Check fit
			bool fits = true;
			for (s16 z=1; z<roomsize.Z-1; z++)
			for (s16 y=1; y<roomsize.Y-1; y++)
			for (s16 x=1; x<roomsize.X-1; x++) {
				v3s16 p = roomplace + v3s16(x,y,z);
				if (vmanip.m_area.contains(p) == false) {
					fits = false;
					break;
				}
				if (vmanip.m_flags[vmanip.m_area.index(p)] & VMANIP_FLAG_DUNGEON_INSIDE) {
					fits = false;
					break;
				}
			}
			// Find new place
			if (fits == false)
				continue;
			result_doorplace = doorplace;
			result_doordir = doordir;
			result_roomplace = roomplace;
			return true;
		}
		return false;
	}

private:
	VoxelManipulator &vmanip;
	v3s16 m_pos;
	v3s16 m_dir;
	PseudoRandom &m_random;
};

static void make_dungeon1(VoxelManipulator &vmanip, PseudoRandom &random)
{
	v3s16 areasize = vmanip.m_area.getExtent();
	v3s16 roomsize;
	v3s16 roomplace;

	/*
		Find place for first room
	*/
	bool fits = false;
	for (u32 i=0; i<100; i++) {
		roomsize = v3s16(random.range(4,8),random.range(4,6),random.range(4,8));
		roomplace = vmanip.m_area.MinEdge + v3s16(
			random.range(0,areasize.X-roomsize.X-1),
			random.range(0,areasize.Y-roomsize.Y-1),
			random.range(0,areasize.Z-roomsize.Z-1)
		);
		/*
			Check that we're not putting the room to an unknown place,
			otherwise it might end up floating in the air
		*/
		fits = true;
		for (s16 z=1; z<roomsize.Z-1; z++)
		for (s16 y=1; y<roomsize.Y-1; y++)
		for (s16 x=1; x<roomsize.X-1; x++) {
			v3s16 p = roomplace + v3s16(x,y,z);
			u32 vi = vmanip.m_area.index(p);
			if (vmanip.m_flags[vi] & VMANIP_FLAG_DUNGEON_INSIDE) {
				fits = false;
				break;
			}
			if (vmanip.m_data[vi].getContent() == CONTENT_IGNORE) {
				fits = false;
				break;
			}
		}
		if (fits)
			break;
	}
	// No place found
	if (fits == false)
		return;

	/*
		Stores the center position of the last room made, so that
		a new corridor can be started from the last room instead of
		the new room, if chosen so.
	*/
	v3s16 last_room_center = roomplace+v3s16(roomsize.X/2,1,roomsize.Z/2);

	u32 room_count = random.range(2,7);
	for (u32 i=0; i<room_count; i++) {

		v3s16 room_center = roomplace + v3s16(roomsize.X/2,1,roomsize.Z/2);

		// dungeons should only be made in stone, so if room_center isn't stone
		// stop making a dungeon
		if (vmanip.m_data[vmanip.m_area.index(room_center)].getContent() != CONTENT_STONE)
			break;

		// Make a room to the determined place
		make_room1(vmanip, roomsize, roomplace);

		// Place torch at room center (for testing)
		//vmanip.m_data[vmanip.m_area.index(room_center)] = MapNode(CONTENT_TORCH);

		// Quit if last room
		if (i == room_count-1)
			break;

		// Determine walker start position

		bool start_in_last_room = (random.range(0,2)!=0);
		//bool start_in_last_room = true;

		v3s16 walker_start_place;

		if (start_in_last_room) {
			walker_start_place = last_room_center;
		}else{
			walker_start_place = room_center;
			// Store center of current room as the last one
			last_room_center = room_center;
		}

		// Create walker and find a place for a door
		RoomWalker walker(vmanip, walker_start_place, random);
		v3s16 doorplace;
		v3s16 doordir;
		bool r = walker.findPlaceForDoor(doorplace, doordir);
		if (r == false)
			return;

		if (random.range(0,1)==0) {
			// Make the door
			make_door1(vmanip, doorplace, doordir);
		}else{
			// Don't actually make a door
			doorplace -= doordir;
		}

		// Make a random corridor starting from the door
		v3s16 corridor_end;
		v3s16 corridor_end_dir;
		make_corridor(vmanip, doorplace, doordir, corridor_end, corridor_end_dir, random);

		// Find a place for a random sized room
		roomsize = v3s16(random.range(4,8),random.range(4,6),random.range(4,8));
		walker.setPos(corridor_end);
		walker.setDir(corridor_end_dir);
		r = walker.findPlaceForRoomDoor(roomsize, doorplace, doordir, roomplace);
		if (r == false)
			return;

		if (random.range(0,1)==0) {
			// Make the door
			make_door1(vmanip, doorplace, doordir);
		}else{
			// Don't actually make a door
			roomplace -= doordir;
		}
	}
}

static void make_nc(VoxelManipulator &vmanip, PseudoRandom &random)
{
	v3s16 dir;
	u8 facedir_i = 0;
	s32 r = random.range(0, 3);
	if (r == 0) {
		dir = v3s16( 1, 0, 0);
		facedir_i = 3;
	}
	if (r == 1) {
		dir = v3s16(-1, 0, 0);
		facedir_i = 1;
	}
	if (r == 2) {
		dir = v3s16( 0, 0, 1);
		facedir_i = 2;
	}
	if (r == 3) {
		dir = v3s16( 0, 0,-1);
		facedir_i = 0;
	}
	v3s16 p = vmanip.m_area.MinEdge + v3s16(
			16+random.range(0,15),
			16+random.range(0,15),
			16+random.range(0,15));
	vmanip.m_data[vmanip.m_area.index(p)] = MapNode(CONTENT_NC, facedir_i);
	u32 length = random.range(3,15);
	for (u32 j=0; j<length; j++) {
		p -= dir;
		vmanip.m_data[vmanip.m_area.index(p)] = MapNode(CONTENT_NC_RB);
	}
}

/*
	Noise functions. Make sure seed is mangled differently in each one.
*/

/*
	Scaling the output of the noise function affects the overdrive of the
	contour function, which affects the shape of the output considerably.
*/
#define CAVE_NOISE_SCALE 12.0
#define CAVE_NOISE_THRESHOLD (1.5/CAVE_NOISE_SCALE)

NoiseParams get_cave_noise1_params(uint64_t seed)
{
	return NoiseParams(NOISE_PERLIN_CONTOUR, seed+52534, 4, 0.5,
			50, CAVE_NOISE_SCALE);
}

NoiseParams get_cave_noise2_params(uint64_t seed)
{
	return NoiseParams(NOISE_PERLIN_CONTOUR_FLIP_YZ, seed+10325, 4, 0.5,
			50, CAVE_NOISE_SCALE);
}

NoiseParams get_ground_noise1_params(uint64_t seed)
{
	return NoiseParams(NOISE_PERLIN, seed+983240, 4,
			0.55, 80.0, 40.0);
}

NoiseParams get_ground_crumbleness_params(uint64_t seed)
{
	return NoiseParams(NOISE_PERLIN, seed+34413, 3,
			1.3, 20.0, 1.0);
}

NoiseParams get_ground_wetness_params(uint64_t seed)
{
	return NoiseParams(NOISE_PERLIN, seed+32474, 4,
			1.1, 40.0, 1.0);
}

bool is_cave(uint64_t seed, v3s16 p)
{
	double d1 = noise3d_param(get_cave_noise1_params(seed), p.X,p.Y,p.Z);
	double d2 = noise3d_param(get_cave_noise2_params(seed), p.X,p.Y,p.Z);
	return d1*d2 > CAVE_NOISE_THRESHOLD;
}

/*
	Ground density noise shall be interpreted by using this.

	TODO: No perlin noises here, they should be outsourced
	      and buffered
		  NOTE: The speed of these actually isn't terrible
*/
bool val_is_ground_flat(double ground_noise1_val, v3s16 p, uint64_t seed)
{
	return (p.Y < 2); // flat
}
bool val_is_ground_flatter(double ground_noise1_val, v3s16 p, uint64_t seed)
{
	double f = 0.55 + noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+920381, 3, 0.45);
	if (f < 0.01) {
		f = 0.1;
	}else if(f > 1.0) {
		f = 1.0;
	}
	double h = WATER_LEVEL + 1 * noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+84174, 4, 0.5);
	return ((double)p.Y - h < ground_noise1_val * f);
}
bool val_is_ground_smoother(double ground_noise1_val, v3s16 p, uint64_t seed)
{
	double f = 0.55 + noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+920381, 3, 0.45);
	//return (p.Y < 1); // flat
	if (f < 0.01) {
		f = 0.1;
	}else if(f > 1.0) {
		f = 1.0;
	}
	double h = WATER_LEVEL + 10 * noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+84174, 4, 0.5);
	return ((double)p.Y - h < ground_noise1_val * f);
}
bool val_is_ground_default(double ground_noise1_val, v3s16 p, uint64_t seed)
{
	double f = 0.55 + noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+920381, 3, 0.45);
	//return (p.Y < 1); // flat
	if (f < 0.01) {
		f = 0.1;
	}else if(f >= 1.0) {
		f *= 1.6; // set to = 1.0 for less crazy maps
	}
	double height_affect = 10; // set to 100 for awesome hills or 1 for flat maps
	double h = WATER_LEVEL + height_affect * noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+84174, 4, 0.5);
	return ((double)p.Y - h < ground_noise1_val * f);
}
bool val_is_ground_hilly(double ground_noise1_val, v3s16 p, uint64_t seed)
{
	double f = 0.55 + noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+920381, 3, 0.45);
	if (f < 0.01) {
		f = 0.1;
	}else if(f > 1.0) {
		f = 1.0;
	}
	double h = WATER_LEVEL + 30 * noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+84174, 4, 0.5);
	return ((double)p.Y - h < ground_noise1_val * f);
}
bool val_is_ground_mountains(double ground_noise1_val, v3s16 p, uint64_t seed)
{
	double f = 0.55 + noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+920381, 3, 0.45);
	if (f < 0.01) {
		f = 0.1;
	}else if(f > 1.0) {
		f = 1.0;
	}
	double h = WATER_LEVEL + 100 * noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+84174, 4, 0.5);
	return ((double)p.Y - h < ground_noise1_val * f);
}
bool val_is_ground_crazy(double ground_noise1_val, v3s16 p, uint64_t seed)
{
	double f = 0.55 + noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+920381, 3, 0.45);
	if (f < 0.01) {
		f = 0.1;
	}else if(f >= 1.0) {
		f = 6.0;
	}
	double h = WATER_LEVEL + 10 * noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+84174, 4, 0.5);
	return ((double)p.Y - h < ground_noise1_val * f);
}
bool val_is_ground_crazyhills(double ground_noise1_val, v3s16 p, uint64_t seed)
{
	double f = 0.55 + noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+920381, 3, 0.45);
	if (f < 0.01) {
		f = 0.1;
	}else if(f > 1.0) {
		f = 6.0;
	}
	double h = WATER_LEVEL + 100 * noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Z/250,
			seed+84174, 4, 0.5);
	return ((double)p.Y - h < ground_noise1_val * f);
}

typedef bool (*val_is_ground_fn)(double,v3s16,uint64_t);

static val_is_ground_fn val_is_ground[8] = {
	val_is_ground_flat,
	val_is_ground_flatter,
	val_is_ground_smoother,
	val_is_ground_default,
	val_is_ground_hilly,
	val_is_ground_mountains,
	val_is_ground_crazy,
	val_is_ground_crazyhills
};

/*
	Queries whether a position is ground or not.
*/
bool is_ground(BlockMakeData *data, v3s16 p)
{
	double val1 = noise3d_param(get_ground_noise1_params(data->seed), p.X,p.Y,p.Z);
	return val_is_ground[data->type](val1, p, data->seed);
}

// Amount of trees per area in nodes
double tree_amount_2d(uint64_t seed, v2s16 p)
{
	double noise = noise2d_perlin(
			0.5+(float)p.X/125, 0.5+(float)p.Y/125,
			seed+2, 4, 0.66);
	double zeroval = -0.39;
	if(noise < zeroval)
		return 0;
	else
		return 0.04 * (noise-zeroval) / (1.0-zeroval);
}

double surface_humidity_2d(uint64_t seed, v2s16 p)
{
	double noise = noise2d_perlin(
			0.5+(float)p.X/500, 0.5+(float)p.Y/500,
			seed+72384, 4, 0.66);
	noise = (noise + 1.0)/2.0;
	if(noise < 0.0)
		noise = 0.0;
	if(noise > 1.0)
		noise = 1.0;
	return noise;
}

double largestone_amount_2d(uint64_t seed, v2s16 p)
{
	double noise = noise2d_perlin(
			0.5+(float)p.X/250, 0.5+(float)p.Y/250,
			seed+14143242, 5, 0.66);
	double zeroval = 0.3;
	if(noise < zeroval)
		return 0;
	else
		return 0.005 * (noise-zeroval) / (1.0-zeroval);
}

/*
	Incrementally find ground level from 3d noise
*/
s16 find_ground_level_from_noise(BlockMakeData *data, v2s16 p2d, s16 precision)
{
	// Start a bit fuzzy to make averaging lower precision values
	// more useful
	s16 level = myrand_range(-precision/2, precision/2);
	s16 dec[] = {31000, 100, 20, 4, 1, 0};
	s16 i;
	for (i = 1; dec[i] != 0 && precision <= dec[i]; i++) {
		// First find non-ground by going upwards
		// Don't stop in caves.
		{
			s16 max = level+dec[i-1]*2;
			v3s16 p(p2d.X, level, p2d.Y);
			for (; p.Y < max; p.Y += dec[i]) {
				if (!is_ground(data, p)) {
					level = p.Y;
					break;
				}
			}
		}
		// Then find ground by going downwards from there.
		// Go in caves, too, when precision is 1.
		{
			s16 min = level-dec[i-1]*2;
			v3s16 p(p2d.X, level, p2d.Y);
			for (; p.Y>min; p.Y-=dec[i]) {
				bool ground = is_ground(data, p);
				/*if(dec[i] == 1 && is_cave(seed, p))
					ground = false;*/
				if (ground) {
					level = p.Y;
					break;
				}
			}
		}
	}

	// This is more like the actual ground level
	level += dec[i-1]/2;

	return level;
}

double get_sector_average_ground_level(BlockMakeData *data, v2s16 sectorpos, double p=4);

double get_sector_average_ground_level(BlockMakeData *data, v2s16 sectorpos, double p)
{
	v2s16 node_min = sectorpos*MAP_BLOCKSIZE;
	v2s16 node_max = (sectorpos+v2s16(1,1))*MAP_BLOCKSIZE-v2s16(1,1);
	double a = 0;
	a += find_ground_level_from_noise(data, v2s16(node_min.X, node_min.Y), p);
	a += find_ground_level_from_noise(data, v2s16(node_min.X, node_max.Y), p);
	a += find_ground_level_from_noise(data, v2s16(node_max.X, node_max.Y), p);
	a += find_ground_level_from_noise(data, v2s16(node_max.X, node_min.Y), p);
	a += find_ground_level_from_noise(data, v2s16(node_min.X+MAP_BLOCKSIZE/2, node_min.Y+MAP_BLOCKSIZE/2), p);
	a /= 5;
	return a;
}

double get_sector_maximum_ground_level(BlockMakeData *data, v2s16 sectorpos, double p=4);

double get_sector_maximum_ground_level(BlockMakeData *data, v2s16 sectorpos, double p)
{
	v2s16 node_min = sectorpos*MAP_BLOCKSIZE;
	v2s16 node_max = (sectorpos+v2s16(1,1))*MAP_BLOCKSIZE-v2s16(1,1);
	double a = -31000;
	// Corners
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_min.X, node_min.Y), p));
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_min.X, node_max.Y), p));
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_max.X, node_max.Y), p));
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_min.X, node_min.Y), p));
	// Center
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_min.X+MAP_BLOCKSIZE/2, node_min.Y+MAP_BLOCKSIZE/2), p));
	// Side middle points
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_min.X+MAP_BLOCKSIZE/2, node_min.Y), p));
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_min.X+MAP_BLOCKSIZE/2, node_max.Y), p));
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_min.X, node_min.Y+MAP_BLOCKSIZE/2), p));
	a = MYMAX(a, find_ground_level_from_noise(data, v2s16(node_max.X, node_min.Y+MAP_BLOCKSIZE/2), p));
	return a;
}

double get_sector_minimum_ground_level(BlockMakeData *data, v2s16 sectorpos, double p=4);

double get_sector_minimum_ground_level(BlockMakeData *data, v2s16 sectorpos, double p)
{
	v2s16 node_min = sectorpos*MAP_BLOCKSIZE;
	v2s16 node_max = (sectorpos+v2s16(1,1))*MAP_BLOCKSIZE-v2s16(1,1);
	double a = 31000;
	// Corners
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_min.X, node_min.Y), p));
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_min.X, node_max.Y), p));
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_max.X, node_max.Y), p));
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_min.X, node_min.Y), p));
	// Center
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_min.X+MAP_BLOCKSIZE/2, node_min.Y+MAP_BLOCKSIZE/2), p));
	// Side middle points
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_min.X+MAP_BLOCKSIZE/2, node_min.Y), p));
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_min.X+MAP_BLOCKSIZE/2, node_max.Y), p));
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_min.X, node_min.Y+MAP_BLOCKSIZE/2), p));
	a = MYMIN(a, find_ground_level_from_noise(data, v2s16(node_max.X, node_min.Y+MAP_BLOCKSIZE/2), p));
	return a;
}

bool block_is_underground(BlockMakeData *data, v3s16 blockpos)
{
	s16 minimum_groundlevel = (s16)get_sector_minimum_ground_level(data, v2s16(blockpos.X, blockpos.Z));

	if(blockpos.Y*MAP_BLOCKSIZE + MAP_BLOCKSIZE <= minimum_groundlevel)
		return true;
	else
		return false;
}

bool get_have_sand(uint64_t seed, v2s16 p2d)
{
	// Determine whether to have sand here
	double sandnoise = noise2d_perlin(
			0.5+(float)p2d.X/500, 0.5+(float)p2d.Y/500,
			seed+59420, 3, 0.50);

	return (sandnoise > -0.15);
}

void make_block(BlockMakeData *data)
{
	if (data->no_op)
		return;

	v3s16 blockpos = data->blockpos;

	ManualMapVoxelManipulator &vmanip = *(data->vmanip);
	// Area of center block
	v3s16 node_min = blockpos*MAP_BLOCKSIZE;
	v3s16 node_max = (blockpos+v3s16(1,1,1))*MAP_BLOCKSIZE-v3s16(1,1,1);
	// Full allocated area
	v3s16 full_node_min = (blockpos-1)*MAP_BLOCKSIZE;
	v3s16 full_node_max = (blockpos+2)*MAP_BLOCKSIZE-v3s16(1,1,1);
	// Area of a block
	double block_area_nodes = MAP_BLOCKSIZE*MAP_BLOCKSIZE;

	v2s16 p2d_center(node_min.X+MAP_BLOCKSIZE/2, node_min.Z+MAP_BLOCKSIZE/2);

	/*
		Get average ground level from noise
	*/

	s16 approx_groundlevel = (s16)get_sector_average_ground_level(data, v2s16(blockpos.X, blockpos.Z));

	s16 approx_ground_depth = approx_groundlevel - (node_min.Y+MAP_BLOCKSIZE/2);

	s16 minimum_groundlevel = (s16)get_sector_minimum_ground_level(data, v2s16(blockpos.X, blockpos.Z));
	// Minimum amount of ground above the top of the central block
	s16 minimum_ground_depth = minimum_groundlevel - node_max.Y;

	s16 maximum_groundlevel = (s16)get_sector_maximum_ground_level(data, v2s16(blockpos.X, blockpos.Z), 1);
	// Maximum amount of ground above the bottom of the central block
	s16 maximum_ground_depth = maximum_groundlevel - node_min.Y;

	/*
		If block is deep underground, this is set to true and ground
		density noise is not generated, for speed optimization.
	*/
	bool all_is_ground_except_caves = (minimum_ground_depth > 40);

	/*
		Create a block-specific seed
	*/
	u32 blockseed = (u32)(data->seed%0x100000000ULL) + full_node_min.Z*38134234
			+ full_node_min.Y*42123 + full_node_min.X*23;

	/*
		Make some 3D noise
	*/

	//NoiseBuffer noisebuf1;
	//NoiseBuffer noisebuf2;
	NoiseBuffer noisebuf_cave;
	NoiseBuffer noisebuf_ground;
	NoiseBuffer noisebuf_ground_crumbleness;
	NoiseBuffer noisebuf_ground_wetness;
	{
		v3f minpos_f(node_min.X, node_min.Y, node_min.Z);
		v3f maxpos_f(node_max.X, node_max.Y, node_max.Z);

		//TimeTaker timer("noisebuf.create");

		/*
			Cave noise
		*/
#if 1
		noisebuf_cave.create(get_cave_noise1_params(data->seed),
				minpos_f.X, minpos_f.Y, minpos_f.Z,
				maxpos_f.X, maxpos_f.Y, maxpos_f.Z,
				2, 2, 2);
		noisebuf_cave.multiply(get_cave_noise2_params(data->seed));
#endif

		/*
			Ground noise
		*/

		// Sample length
		v3f sl = v3f(4.0, 4.0, 4.0);

		/*
			Density noise
		*/
		if (all_is_ground_except_caves == false)
			noisebuf_ground.create(get_ground_noise1_params(data->seed),
					minpos_f.X, minpos_f.Y, minpos_f.Z,
					maxpos_f.X, maxpos_f.Y, maxpos_f.Z,
					sl.X, sl.Y, sl.Z);

		/*
			Ground property noise
		*/
		sl = v3f(2.5, 2.5, 2.5);
		noisebuf_ground_crumbleness.create(
				get_ground_crumbleness_params(data->seed),
				minpos_f.X, minpos_f.Y, minpos_f.Z,
				maxpos_f.X, maxpos_f.Y+5, maxpos_f.Z,
				sl.X, sl.Y, sl.Z);
		noisebuf_ground_wetness.create(
				get_ground_wetness_params(data->seed),
				minpos_f.X, minpos_f.Y, minpos_f.Z,
				maxpos_f.X, maxpos_f.Y+5, maxpos_f.Z,
				sl.X, sl.Y, sl.Z);
	}


	bool limestone = (noisebuf_ground_wetness.get(node_min.X+8,node_min.Y+8,node_min.Z+8) > 0.5);
	content_t base_content = CONTENT_STONE;
	if (limestone)
		base_content = CONTENT_LIMESTONE;

	/*
		Make base ground level
	*/

	for (s16 x=node_min.X; x<=node_max.X; x++)
	for (s16 z=node_min.Z; z<=node_max.Z; z++) {
		// Node position
		v2s16 p2d(x,z);
		{
			// Use fast index incrementing
			v3s16 em = vmanip.m_area.getExtent();
			u32 i = vmanip.m_area.index(v3s16(p2d.X, node_min.Y, p2d.Y));
			for (s16 y=node_min.Y; y<=node_max.Y; y++) {
				// Only modify places that have no content
				if (vmanip.m_data[i].getContent() == CONTENT_IGNORE) {
					// First priority: make air and water.
					// This avoids caves inside water.
					if (
						all_is_ground_except_caves == false
						&& val_is_ground[data->type](noisebuf_ground.get(x,y,z), v3s16(x,y,z), data->seed) == false
					) {
						if (y <= WATER_LEVEL) {
							vmanip.m_data[i] = MapNode(CONTENT_WATERSOURCE);
						}else if (y>=1024) {
							vmanip.m_data[i] = MapNode(CONTENT_VACUUM);
						}else{
							vmanip.m_data[i] = MapNode(CONTENT_AIR);
						}
					}else if (noisebuf_cave.get(x,y,z) > CAVE_NOISE_THRESHOLD) {
						vmanip.m_data[i] = MapNode(CONTENT_AIR);
					}else{
						vmanip.m_data[i] = MapNode(base_content);
					}
				}

				data->vmanip->m_area.add_y(em, i, 1);
			}
		}
	}

	/*
		Add minerals
	*/

	{
		PseudoRandom mineralrandom(blockseed);

		/*
			Add mithril blocks
		*/
		for (s16 i=0; i<approx_ground_depth/4; i++) {
			if (mineralrandom.next()%50 == 0) {
				s16 x = mineralrandom.range(node_min.X+1, node_max.X-1);
				s16 y = mineralrandom.range(node_min.Y+1, node_max.Y-1);
				s16 z = mineralrandom.range(node_min.Z+1, node_max.Z-1);
				// TODO: at random, some should be gems
				u8 type = mineralrandom.next()%12;
				switch (type) {
				case 0:
					type = MINERAL_RUBY;
					break;
				case 1:
					type = MINERAL_TURQUOISE;
					break;
				case 2:
					type = MINERAL_AMETHYST;
					break;
				case 3:
					type = MINERAL_SAPPHIRE;
					break;
				case 4:
					type = MINERAL_SUNSTONE;
					break;
				default:
					type = MINERAL_MITHRIL;
				}
				for (u16 i=0; i<27; i++) {
					v3s16 p = v3s16(x,y,z) + g_27dirs[i];
					u32 vi = vmanip.m_area.index(p);
					if (vmanip.m_data[vi].getContent() == base_content && mineralrandom.next()%8 == 0)
						vmanip.m_data[vi] = MapNode(base_content,type);
				}

			}
		}
		/*
			Add others
		*/
		if (limestone) {
			/*
				Add tin/quartz (and rare coal)
			*/
			u16 a = mineralrandom.range(0,15);
			a = a*a*a;
			u16 amount = 20;
			u16 rareness = 40 / amount;
			if (rareness == 0)
				rareness = 1;
			if (mineralrandom.next()%rareness == 0) {
				u16 a = mineralrandom.next() % 16;
				u16 amnt = amount * a*a*a / 1000;
				for (s16 i=0; i<amnt; i++) {
					s16 x = mineralrandom.range(node_min.X+1, node_max.X-1);
					s16 y = mineralrandom.range(node_min.Y+1, node_max.Y-1);
					s16 z = mineralrandom.range(node_min.Z+1, node_max.Z-1);
					u8 type = mineralrandom.next()%12;
					if (type == 0) {
						type = MINERAL_COAL;
					}else if (type > 8) {
						type =  MINERAL_QUARTZ;
					}else if (type > 4) {
						type =  MINERAL_COPPER;
					}else{
						type = MINERAL_TIN;
					}
					for (u16 i=0; i<27; i++) {
						v3s16 p = v3s16(x,y,z) + g_27dirs[i];
						u32 vi = vmanip.m_area.index(p);
						if (vmanip.m_data[vi].getContent() == CONTENT_LIMESTONE && mineralrandom.next()%8 == 0)
							vmanip.m_data[vi] = MapNode(CONTENT_LIMESTONE, type);
					}
				}
			}
		}else{
			{
				u16 a = mineralrandom.range(0,15);
				a = a*a*a;
				u16 amount = 20 * a/1000;
				for (s16 i=0; i<amount; i++) {
					s16 x = mineralrandom.range(node_min.X+1, node_max.X-1);
					s16 y = mineralrandom.range(node_min.Y+1, node_max.Y-1);
					s16 z = mineralrandom.range(node_min.Z+1, node_max.Z-1);

					u8 base_content = CONTENT_STONE;
					MapNode new_content(CONTENT_IGNORE);
					u32 sparseness = 6;

					if (noisebuf_ground_crumbleness.get(x,y+5,z) < -0.1) {
						new_content = MapNode(CONTENT_STONE, MINERAL_COAL);
					}else if (noisebuf_ground_wetness.get(x,y+5,z) > 0.1) {
						new_content = MapNode(CONTENT_STONE, MINERAL_IRON);
					}else if (noisebuf_ground_crumbleness.get(x,y,z) > 0.4) {
						new_content = MapNode(CONTENT_STONE, MINERAL_SILVER);
					}else if (noisebuf_ground_crumbleness.get(x,y,z) > 0.3) {
						new_content = MapNode(CONTENT_STONE, MINERAL_QUARTZ);
					}else if (noisebuf_ground_crumbleness.get(x,y,z) > 0.2) {
						new_content = MapNode(CONTENT_STONE, MINERAL_COPPER);
					}else if (noisebuf_ground_crumbleness.get(x,y,z) > 0.1) {
						new_content = MapNode(CONTENT_STONE, MINERAL_TIN);
					}else if (noisebuf_ground_wetness.get(x,y+5,z) > 0.0) {
						new_content = MapNode(CONTENT_STONE, MINERAL_GOLD);
					}

					if (new_content.getContent() != CONTENT_IGNORE) {
						for (u16 i=0; i<27; i++) {
							v3s16 p = v3s16(x,y,z) + g_27dirs[i];
							u32 vi = vmanip.m_area.index(p);
							if (vmanip.m_data[vi].getContent() == base_content && mineralrandom.next()%sparseness == 0)
								vmanip.m_data[vi] = new_content;
						}
					}
				}
			}
			/*
				Add coal
			*/
			u16 coal_amount = 30;
			u16 coal_rareness = 60 / coal_amount;
			if (coal_rareness == 0)
				coal_rareness = 1;
			if (mineralrandom.next()%coal_rareness == 0) {
				u16 a = mineralrandom.next() % 16;
				u16 amount = coal_amount * a*a*a / 1000;
				for (s16 i=0; i<amount; i++) {
					s16 x = mineralrandom.range(node_min.X+1, node_max.X-1);
					s16 y = mineralrandom.range(node_min.Y+1, node_max.Y-1);
					s16 z = mineralrandom.range(node_min.Z+1, node_max.Z-1);
					for (u16 i=0; i<27; i++) {
						v3s16 p = v3s16(x,y,z) + g_27dirs[i];
						u32 vi = vmanip.m_area.index(p);
						if (vmanip.m_data[vi].getContent() == CONTENT_STONE && mineralrandom.next()%8 == 0)
							vmanip.m_data[vi] = MapNode(CONTENT_STONE, MINERAL_COAL);
					}
				}
			}
			/*
				Add iron
			*/
			u16 iron_amount = 8;
			u16 iron_rareness = 60 / iron_amount;
			if (iron_rareness == 0)
				iron_rareness = 1;
			if (mineralrandom.next()%iron_rareness == 0) {
				u16 a = mineralrandom.next() % 16;
				u16 amount = iron_amount * a*a*a / 1000;
				for (s16 i=0; i<amount; i++) {
					s16 x = mineralrandom.range(node_min.X+1, node_max.X-1);
					s16 y = mineralrandom.range(node_min.Y+1, node_max.Y-1);
					s16 z = mineralrandom.range(node_min.Z+1, node_max.Z-1);
					for (u16 i=0; i<27; i++) {
						v3s16 p = v3s16(x,y,z) + g_27dirs[i];
						u32 vi = vmanip.m_area.index(p);
						if (vmanip.m_data[vi].getContent() == CONTENT_STONE && mineralrandom.next()%8 == 0)
							vmanip.m_data[vi] = MapNode(CONTENT_STONE, MINERAL_IRON);
					}
				}
			}
		}
	}

	/*
		Add mud and sand and others underground (in place of stone)
	*/
	content_t liquid_type = CONTENT_LAVASOURCE;
	if (limestone || blockpos.Y > -1 || ((blockpos.X + blockpos.Z)/blockpos.Y+1)%16 == 0)
		liquid_type = CONTENT_WATERSOURCE;

	for (s16 x=node_min.X; x<=node_max.X; x++)
	for (s16 z=node_min.Z; z<=node_max.Z; z++) {
		// Node position
		v2s16 p2d(x,z);
		{
			// Use fast index incrementing
			v3s16 em = vmanip.m_area.getExtent();
			u32 i = vmanip.m_area.index(v3s16(p2d.X, node_max.Y, p2d.Y));
			for (s16 y=node_max.Y; y>=node_min.Y; y--) {
				if (vmanip.m_data[i].getContent() == base_content) {
					if (noisebuf_ground_crumbleness.get(x,y,z) > 1.3) {
						if (noisebuf_ground_wetness.get(x,y,z) > 0.0) {
							vmanip.m_data[i] = MapNode(CONTENT_MUD);
						}else{
							vmanip.m_data[i] = MapNode(CONTENT_SAND);
						}
					}else if (noisebuf_ground_crumbleness.get(x,y,z) > 0.7) {
						if (noisebuf_ground_wetness.get(x,y,z) < -0.6)
							vmanip.m_data[i] = MapNode(CONTENT_GRAVEL);
					}else if (noisebuf_ground_crumbleness.get(x,y,z) < -3.0 + MYMIN(0.1 * sqrt((float)MYMAX(0, -y)), 1.5)) {
						vmanip.m_data[i] = MapNode(liquid_type);
						for (s16 x1=-1; x1<=1; x1++)
						for (s16 y1=-1; y1<=1; y1++)
						for (s16 z1=-1; z1<=1; z1++) {
							data->transforming_liquid.push_back(v3s16(p2d.X+x1, y+y1, p2d.Y+z1));
						}
					}
				}

				data->vmanip->m_area.add_y(em, i, -1);
			}
		}
	}

	if (!limestone) {
		/*
			Add dungeons
		*/

		float dungeon_rarity = 0.02;
		if (
			((noise3d(blockpos.X,blockpos.Y,blockpos.Z,data->seed)+1.0)/2.0) < dungeon_rarity
			&& node_min.Y < approx_groundlevel
		) {
			// Dungeon generator doesn't modify places which have this set
			data->vmanip->clearFlag(VMANIP_FLAG_DUNGEON_INSIDE | VMANIP_FLAG_DUNGEON_PRESERVE);

			// Set all air and water to be untouchable to make dungeons open
			// to caves and open air
			for (s16 x=full_node_min.X; x<=full_node_max.X; x++)
			for (s16 z=full_node_min.Z; z<=full_node_max.Z; z++) {
				// Node position
				v2s16 p2d(x,z);
				{
					// Use fast index incrementing
					v3s16 em = vmanip.m_area.getExtent();
					u32 i = vmanip.m_area.index(v3s16(p2d.X, full_node_max.Y, p2d.Y));
					for (s16 y=full_node_max.Y; y>=full_node_min.Y; y--) {
						if (vmanip.m_data[i].getContent() == CONTENT_AIR) {
							vmanip.m_flags[i] |= VMANIP_FLAG_DUNGEON_PRESERVE;
						}else if (vmanip.m_data[i].getContent() == CONTENT_WATERSOURCE) {
							vmanip.m_flags[i] |= VMANIP_FLAG_DUNGEON_PRESERVE;
						}
						data->vmanip->m_area.add_y(em, i, -1);
					}
				}
			}

			PseudoRandom random(blockseed+2);

			// Add it
			make_dungeon1(vmanip, random);

			// Convert some cobble to mossy cobble
			for (s16 x=full_node_min.X; x<=full_node_max.X; x++)
			for (s16 z=full_node_min.Z; z<=full_node_max.Z; z++) {
				// Node position
				v2s16 p2d(x,z);
				{
					// Use fast index incrementing
					v3s16 em = vmanip.m_area.getExtent();
					u32 i = vmanip.m_area.index(v3s16(p2d.X, full_node_max.Y, p2d.Y));
					for (s16 y=full_node_max.Y; y>=full_node_min.Y; y--) {
						// (noisebuf not used because it doesn't contain the
						//  full area)
						double wetness = noise3d_param(get_ground_wetness_params(data->seed), x,y,z);
						double d = noise3d_perlin((float)x/2.5, (float)y/2.5,(float)z/2.5, blockseed, 2, 1.4);
						if (vmanip.m_data[i].getContent() == CONTENT_COBBLE && d < wetness/3.0)
							vmanip.m_data[i].setContent(CONTENT_MOSSYCOBBLE);
						data->vmanip->m_area.add_y(em, i, -1);
					}
				}
			}
		}

		/*
			Add NC
		*/
		{
			PseudoRandom ncrandom(blockseed+9324342);
			if (ncrandom.range(0, 1000) == 0 && blockpos.Y <= -3)
				make_nc(vmanip, ncrandom);
		}
	}

	/*
		Add top and bottom side of water to transforming_liquid queue
	*/

	for (s16 x=node_min.X; x<=node_max.X; x++)
	for (s16 z=node_min.Z; z<=node_max.Z; z++) {
		// Node position
		v2s16 p2d(x,z);
		{
			bool water_found = false;
			// Use fast index incrementing
			v3s16 em = vmanip.m_area.getExtent();
			u32 i = vmanip.m_area.index(v3s16(p2d.X, node_max.Y, p2d.Y));
			for (s16 y=node_max.Y; y>=node_min.Y; y--) {
				if (!water_found) {
					if (vmanip.m_data[i].getContent() == CONTENT_WATERSOURCE) {
						v3s16 p = v3s16(p2d.X, y, p2d.Y);
						data->transforming_liquid.push_back(p);
						water_found = true;
					}
				}else{
					// This can be done because water_found can only
					// turn to true and end up here after going through
					// a single block.
					if (vmanip.m_data[i+1].getContent() != CONTENT_WATERSOURCE) {
						v3s16 p = v3s16(p2d.X, y+1, p2d.Y);
						data->transforming_liquid.push_back(p);
						water_found = false;
					}
				}

				data->vmanip->m_area.add_y(em, i, -1);
			}
		}
	}

	/*
		If close to ground level
	*/

	if (minimum_ground_depth < 5 && maximum_ground_depth > -5) {
		/*
			Add grass and mud
		*/

		for (s16 x=node_min.X; x<=node_max.X; x++)
		for (s16 z=node_min.Z; z<=node_max.Z; z++) {
			// Node position
			v2s16 p2d(x,z);
			{
				bool possibly_have_sand = get_have_sand(data->seed, p2d);
				bool have_sand = false;
				u32 current_depth = 0;
				bool air_detected = false;
				bool water_detected = false;
				bool have_clay = false;

				// Use fast index incrementing
				s16 start_y = node_max.Y+2;
				v3s16 em = vmanip.m_area.getExtent();
				u32 i = vmanip.m_area.index(v3s16(p2d.X, start_y, p2d.Y));
				for (s16 y=start_y; y>=node_min.Y-3; y--) {
					if (vmanip.m_data[i].getContent() == CONTENT_WATERSOURCE)
						water_detected = true;
					if (vmanip.m_data[i].getContent() == CONTENT_AIR)
						air_detected = true;

					if (
						(
							vmanip.m_data[i].getContent() == base_content
							|| vmanip.m_data[i].getContent() == CONTENT_GRASS
							|| vmanip.m_data[i].getContent() == CONTENT_MUD
							|| vmanip.m_data[i].getContent() == CONTENT_SAND
							|| vmanip.m_data[i].getContent() == CONTENT_GRAVEL
						) && (
							air_detected || water_detected
						)
					) {
						if (current_depth == 0 && y <= WATER_LEVEL+2 && possibly_have_sand)
							have_sand = true;

						if (current_depth < 4) {
							if (have_sand) {
								// Determine whether to have clay in the sand here
								double claynoise = noise2d_perlin(
										0.5+(float)p2d.X/500, 0.5+(float)p2d.Y/500,
										data->seed+4321, 6, 0.95) + 0.5;

								have_clay = (y <= WATER_LEVEL) && (y >= WATER_LEVEL-2) && (
									((claynoise > 0) && (claynoise < 0.04) && (current_depth == 0)) ||
									((claynoise > 0) && (claynoise < 0.12) && (current_depth == 1))
									);
								if (have_clay) {
									vmanip.m_data[i] = MapNode(CONTENT_CLAY);
								}else{
									vmanip.m_data[i] = MapNode(CONTENT_SAND);
								}
							}else if (current_depth==0 && !water_detected && y >= WATER_LEVEL && air_detected) {
								if (y > 50) {
									vmanip.m_data[i] = MapNode(CONTENT_MUDSNOW);
								}else if (y > 45 && myrand()%5 == 0) {
									vmanip.m_data[i] = MapNode(CONTENT_MUDSNOW);
								}else{
									vmanip.m_data[i] = MapNode(CONTENT_GRASS);
								}
							}else{
								vmanip.m_data[i] = MapNode(CONTENT_MUD);
							}
						}else{
							if(vmanip.m_data[i].getContent() == CONTENT_MUD
								|| vmanip.m_data[i].getContent() == CONTENT_GRASS)
								vmanip.m_data[i] = MapNode(base_content);
						}

						current_depth++;

						if (current_depth >= 8)
							break;
					}else if (current_depth != 0) {
						break;
					}

					data->vmanip->m_area.add_y(em, i, -1);
				}
			}
		}

		/*
			Calculate some stuff
		*/

		float surface_humidity = surface_humidity_2d(data->seed, p2d_center);
		bool is_jungle = surface_humidity > 0.75;
		// Amount of trees
		u32 tree_count = block_area_nodes * tree_amount_2d(data->seed, p2d_center);
		if (is_jungle)
			tree_count *= 5;

		/*
			Add trees
		*/
		PseudoRandom treerandom(blockseed);
		// Put trees in random places on part of division
		for (u32 i=0; i<tree_count; i++) {
			s16 x = treerandom.range(node_min.X, node_max.X);
			s16 z = treerandom.range(node_min.Z, node_max.Z);
			s16 y = find_ground_level_from_noise(data, v2s16(x,z), 4);
			// Don't make a tree under water level
			if (y < WATER_LEVEL)
				continue;
			// Make sure tree fits (only trees whose starting point is
			// at this block are added)
			if (y < node_min.Y || y > node_max.Y)
				continue;
			/*
				Find exact ground level
			*/
			v3s16 p(x,y+6,z);
			bool found = false;
			for (; p.Y >= y-6; p.Y--) {
				u32 i = data->vmanip->m_area.index(p);
				MapNode *n = &data->vmanip->m_data[i];
				if (n->getContent() != CONTENT_AIR && n->getContent() != CONTENT_WATERSOURCE && n->getContent() != CONTENT_IGNORE) {
					found = true;
					break;
				}
			}
			// If not found, handle next one
			if (found == false)
				continue;

			{
				u32 i = data->vmanip->m_area.index(p);
				MapNode *n = &data->vmanip->m_data[i];

				if (
					n->getContent() != CONTENT_MUD
					&& n->getContent() != CONTENT_GRASS
					&& n->getContent() != CONTENT_SAND
					&& n->getContent() != CONTENT_MUDSNOW
				)
						continue;

				// Papyrus grows only on mud and in water
				if (n->getContent() == CONTENT_MUD && y <= WATER_LEVEL) {
					p.Y++;
					make_papyrus(vmanip, p);
				}
				// Trees grow only on mud and grass, on land
				else if ((n->getContent() == CONTENT_MUD || n->getContent() == CONTENT_GRASS) && y > WATER_LEVEL + 2) {
					p.Y++;
					if (is_jungle == false || y > 30) {
						// connifers
						if (y > 45) {
							make_conifertree(vmanip, p);
						// regular trees
						}else{
							if (myrand_range(0,10) != 0) {
								if (
									noise2d_perlin(
										0.5+(float)p.X/100,
										0.5+(float)p.Z/100,
										data->seed+342902,
										3,
										0.45
									) > 0.2
								) {
									make_appletree(vmanip, p);
								}else{
									make_tree(vmanip, p);
								}
							}else{
								make_largetree(vmanip, p);
							}
						}
					}else{
						make_jungletree(vmanip, p);
					}
				}
				// connifers
				else if (n->getContent() == CONTENT_MUDSNOW) {
					p.Y++;
					make_conifertree(vmanip, p);
				}
				// Cactii grow only on sand, on land
				else if (n->getContent() == CONTENT_SAND && y > WATER_LEVEL + 2) {
					p.Y++;
					make_cactus(vmanip, p);
				}
			}
		}

		/*
			Add jungle grass
		*/
		if (is_jungle) {
			PseudoRandom grassrandom(blockseed);
			for (u32 i=0; i<surface_humidity*5*tree_count; i++) {
				s16 x = grassrandom.range(node_min.X, node_max.X);
				s16 z = grassrandom.range(node_min.Z, node_max.Z);
				s16 y = find_ground_level_from_noise(data, v2s16(x,z), 4);
				if (y < WATER_LEVEL)
					continue;
				if (y < node_min.Y || y > node_max.Y)
					continue;
				/*
					Find exact ground level
				*/
				v3s16 p(x,y+6,z);
				bool found = false;
				for (; p.Y >= y-6; p.Y--) {
					u32 i = data->vmanip->m_area.index(p);
					MapNode *n = &data->vmanip->m_data[i];
					if (content_features(*n).is_ground_content || n->getContent() == CONTENT_JUNGLETREE) {
						found = true;
						break;
					}
				}
				// If not found, handle next one
				if (found == false)
					continue;
				p.Y++;
				if (vmanip.m_area.contains(p) == false)
					continue;
				if (vmanip.m_data[vmanip.m_area.index(p)].getContent() != CONTENT_AIR)
					continue;
				if (vmanip.m_area.contains(p)) {
					if (y > 20 || y < 10) {
						if (myrand_range(0,20) == 0) {
							if (y > 20) {
								vmanip.m_data[vmanip.m_area.index(p)] = CONTENT_TEA;
							}else{
								vmanip.m_data[vmanip.m_area.index(p)] = CONTENT_COFFEE;
							}
						}else{
							vmanip.m_data[vmanip.m_area.index(p)] = CONTENT_JUNGLEGRASS;
						}
					}else{
						vmanip.m_data[vmanip.m_area.index(p)] = CONTENT_JUNGLEGRASS;
					}
				}
			}
		}
	}
}

BlockMakeData::BlockMakeData():
	no_op(false),
	vmanip(NULL),
	seed(0),
	type(MGT_DEFAULT)
{}

BlockMakeData::~BlockMakeData()
{
	delete vmanip;
}

}; // namespace mapgen


