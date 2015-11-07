/************************************************************************
* plantgrowth.cpp
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

#include "environment.h"
#include "content_mapnode.h"
#include "mapblock.h"
#include "map.h"


void plantgrowth_tree(ServerEnvironment *env, v3s16 p0)
{
	MapNode treenode(CONTENT_TREE);
	MapNode leavesnode(CONTENT_LEAVES);
	Map *map = &env->getMap();

	s16 trunk_h = myrand_range(5,6);
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		map->addNodeWithEvent(p1,treenode);
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
		v3s16 p(
			myrand_range(leaves_a.MinEdge.X, leaves_a.MaxEdge.X-1),
			myrand_range(leaves_a.MinEdge.Y, leaves_a.MaxEdge.Y-1),
			myrand_range(leaves_a.MinEdge.Z, leaves_a.MaxEdge.Z-1)
		);

		for (s16 z=0; z<=1; z++) {
		for (s16 y=0; y<=1; y++) {
		for (s16 x=0; x<=1; x++) {
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
		u32 i = leaves_a.index(x,y,z);
		if (leaves_d[i] != 1)
			continue;
		MapNode n = map->getNodeNoEx(p);
		if (n.getContent() != CONTENT_AIR)
			continue;
		map->addNodeWithEvent(p,leavesnode);
	}
	}
	}
}

void plantgrowth_appletree(ServerEnvironment *env, v3s16 p0)
{
	MapNode treenode(CONTENT_APPLE_TREE);
	MapNode leavesnode(CONTENT_APPLE_LEAVES);
	MapNode applenode(CONTENT_APPLE);
	Map *map = &env->getMap();

	s16 trunk_h = myrand_range(4, 5);
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		map->addNodeWithEvent(p1,treenode);
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	p1.Y -= 1;

	VoxelArea leaves_a(v3s16(-2,-1,-2), v3s16(2,2,2));
	Buffer<u8> leaves_d(leaves_a.getVolume());
	for (s32 i=0; i<leaves_a.getVolume(); i++) {
		leaves_d[i] = 0;
	}

	// Force leaves at near the end of the trunk
	for (s16 z=-1; z<=1; z++) {
	for (s16 y=-1; y<=1; y++) {
	for (s16 x=-1; x<=1; x++) {
		leaves_d[leaves_a.index(v3s16(x,y,z))] = 1;
	}
	}
	}

	// Add leaves randomly
	for (u32 iii=0; iii<7; iii++) {
		v3s16 p(
			myrand_range(leaves_a.MinEdge.X, leaves_a.MaxEdge.X-1),
			myrand_range(leaves_a.MinEdge.Y, leaves_a.MaxEdge.Y-1),
			myrand_range(leaves_a.MinEdge.Z, leaves_a.MaxEdge.Z-1)
		);

		for (s16 z=0; z<=1; z++) {
		for (s16 y=0; y<=1; y++) {
		for (s16 x=0; x<=1; x++) {
			leaves_d[leaves_a.index(p+v3s16(x,y,z))] = 1;
		}
		}
		}
	}

	// not all apple trees have apples
	bool have_fruit = (myrand_range(0,4) == 0);

	// Blit leaves to vmanip
	for (s16 z=leaves_a.MinEdge.Z; z<=leaves_a.MaxEdge.Z; z++) {
	for (s16 y=leaves_a.MinEdge.Y; y<=leaves_a.MaxEdge.Y; y++) {
	for (s16 x=leaves_a.MinEdge.X; x<=leaves_a.MaxEdge.X; x++) {
		v3s16 p(x,y,z);
		p += p1;
		u32 i = leaves_a.index(x,y,z);
		if (leaves_d[i] != 1)
			continue;
		MapNode n = map->getNodeNoEx(p);
		if (n.getContent() != CONTENT_AIR)
			continue;
		if (have_fruit && myrand_range(0,99) < 10) {
			map->addNodeWithEvent(p,applenode);
		}else{
			map->addNodeWithEvent(p,leavesnode);
		}
	}
	}
	}
}

void plantgrowth_conifertree(ServerEnvironment *env, v3s16 p0)
{
	MapNode treenode(CONTENT_CONIFER_TREE);
	MapNode leavesnode(CONTENT_CONIFER_LEAVES);
	Map *map = &env->getMap();

	s16 trunk_h = myrand_range(8, 11);
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		map->addNodeWithEvent(p1,treenode);
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	map->addNodeWithEvent(p1+v3s16(0,1,0),leavesnode);
	for (s16 z=-1; z<=1; z++) {
	for (s16 y=-2; y<=0; y++) {
	for (s16 x=-1; x<=1; x++) {
		if (!x && !z)
			continue;
		v3s16 p = p1+v3s16(x,y,z);
		MapNode n = map->getNodeNoEx(p);
		if (n.getContent() != CONTENT_AIR)
			continue;
		map->addNodeWithEvent(p,leavesnode);
	}
	}
	}
	for (s16 z=-2; z<=2; z++) {
	for (s16 y=-5; y<-2; y++) {
	for (s16 x=-2; x<=2; x++) {
		if (!x && !z)
			continue;
		v3s16 p = p1+v3s16(x,y,z);
		MapNode n = map->getNodeNoEx(p);
		if (n.getContent() != CONTENT_AIR)
			continue;
		map->addNodeWithEvent(p,leavesnode);
	}
	}
	}

}

void plantgrowth_largetree(ServerEnvironment *env, v3s16 p0)
{
	MapNode treenode(CONTENT_TREE);
	MapNode leavesnode(CONTENT_LEAVES);
	Map *map = &env->getMap();

	s16 trunk_h = myrand_range(10, 12);
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		map->addNodeWithEvent(p1,treenode);
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	p1.Y -= 1;

	VoxelArea leaves_a(v3s16(-3,-3,-3), v3s16(3,3,3));
	Buffer<u8> leaves_d(leaves_a.getVolume());
	for (s32 i=0; i<leaves_a.getVolume(); i++) {
		leaves_d[i] = 0;
	}

	for (s16 k=0; k<5; k++) {
		if (k == 1) {
			p1.Y -= 3;
			for (s16 ki=0; ki<4; ki++) {
				p1.X++;
				map->addNodeWithEvent(p1,treenode);
			}
			p1.X--;
		}else if (k == 2) {
			p1.X -= 3;
			for (s16 ki=0; ki<4; ki++) {
				p1.X--;
				map->addNodeWithEvent(p1,treenode);
			}
			p1.X++;
		}else if (k == 3) {
			p1.X += 3;
			for (s16 ki=0; ki<4; ki++) {
				p1.Z++;
				map->addNodeWithEvent(p1,treenode);
			}
			p1.Z--;
		}else if (k == 4) {
			p1.Z -= 3;
			for (s16 ki=0; ki<4; ki++) {
				p1.Z--;
				map->addNodeWithEvent(p1,treenode);
			}
			p1.Z++;
		}else{
			map->addNodeWithEvent(p1,treenode);
		}
		// Force leaves at near the end of the trunk
		for (s16 z=-1; z<=1; z++) {
		for (s16 y=-1; y<=1; y++) {
		for (s16 x=-1; x<=1; x++) {
			leaves_d[leaves_a.index(v3s16(x,y,z))] = 1;
		}
		}
		}

		if (k) {
			// Add leaves randomly
			for (u32 iii=0; iii<10; iii++) {
				v3s16 p(
					myrand_range(leaves_a.MinEdge.X, leaves_a.MaxEdge.X-1),
					myrand_range(leaves_a.MinEdge.Y, leaves_a.MaxEdge.Y-1),
					myrand_range(leaves_a.MinEdge.Z, leaves_a.MaxEdge.Z-1)
				);

				for (s16 z=0; z<=1; z++) {
				for (s16 y=0; y<=1; y++) {
				for (s16 x=0; x<=1; x++) {
					leaves_d[leaves_a.index(p+v3s16(x,y,z))] = 1;
				}
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
			u32 i = leaves_a.index(x,y,z);
			if (leaves_d[i] != 1)
				continue;
			MapNode n = map->getNodeNoEx(p);
			if (n.getContent() != CONTENT_AIR)
				continue;
			map->addNodeWithEvent(p,leavesnode);
		}
		}
		}
	}
}

void plantgrowth_jungletree(ServerEnvironment *env, v3s16 p0)
{
	MapNode treenode(CONTENT_JUNGLETREE);
	MapNode leavesnode(CONTENT_JUNGLELEAVES);
	Map *map = &env->getMap();

	for (s16 x=-1; x<=1; x++) {
	for (s16 z=-1; z<=1; z++) {
		if (myrand_range(0, 2) == 0)
			continue;
		v3s16 p1 = p0 + v3s16(x,0,z);
		v3s16 p2 = p0 + v3s16(x,-1,z);
		MapNode n1 = map->getNodeNoEx(p1);
		MapNode n2 = map->getNodeNoEx(p2);
		if (n2.getContent() == CONTENT_AIR) {
			map->addNodeWithEvent(p2,treenode);
		}else if (n1.getContent() == CONTENT_AIR) {
			map->addNodeWithEvent(p1,treenode);
		}
	}
	}

	s16 trunk_h = myrand_range(8, 12);
	v3s16 p1 = p0;
	for (s16 ii=0; ii<trunk_h; ii++) {
		map->addNodeWithEvent(p1,treenode);
		p1.Y++;
	}

	// p1 is now the last piece of the trunk
	p1.Y -= 1;

	VoxelArea leaves_a(v3s16(-3,-2,-3), v3s16(3,2,3));
	//SharedPtr<u8> leaves_d(new u8[leaves_a.getVolume()]);
	Buffer<u8> leaves_d(leaves_a.getVolume());
	for (s32 i=0; i<leaves_a.getVolume(); i++) {
		leaves_d[i] = 0;
	}

	// Force leaves at near the end of the trunk
	for (s16 z=-1; z<=1; z++) {
	for (s16 y=-1; y<=1; y++) {
	for (s16 x=-1; x<=1; x++) {
		leaves_d[leaves_a.index(v3s16(x,y,z))] = 1;
	}
	}
	}

	// Add leaves randomly
	for (u32 iii=0; iii<30; iii++) {
		v3s16 p(
			myrand_range(leaves_a.MinEdge.X, leaves_a.MaxEdge.X-1),
			myrand_range(leaves_a.MinEdge.Y, leaves_a.MaxEdge.Y-1),
			myrand_range(leaves_a.MinEdge.Z, leaves_a.MaxEdge.Z-1)
		);

		for (s16 z=0; z<=1; z++) {
		for (s16 y=0; y<=1; y++) {
		for (s16 x=0; x<=1; x++) {
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
		u32 i = leaves_a.index(x,y,z);
		if (leaves_d[i] != 1)
			continue;
		MapNode n = map->getNodeNoEx(p);
		if (n.getContent() != CONTENT_AIR)
			continue;
		map->addNodeWithEvent(p,leavesnode);
	}
	}
	}
}

void plantgrowth_fertilizer(ServerEnvironment *env, v3s16 p0)
{
	content_t types[8] = {
		CONTENT_SEEDS_WHEAT,
		CONTENT_SEEDS_MELON,
		CONTENT_SEEDS_PUMPKIN,
		CONTENT_SEEDS_POTATO,
		CONTENT_SEEDS_CARROT,
		CONTENT_SEEDS_BEETROOT,
		CONTENT_SEEDS_GRAPE,
		CONTENT_SEEDS_COTTON
	};
	u8 seed = myrand()%10;
	if (seed > 7)
		return;
	MapNode n(types[seed]);
	env->getMap().addNodeWithEvent(p0,n);
}

void plantgrowth_seed(ServerEnvironment *env, v3s16 p0)
{
	MapNode n = env->getMap().getNodeNoEx(p0);
	content_t c = content_features(n).special_alternate_node;
	if (c == CONTENT_IGNORE)
		return;
	n.setContent(c);
	n.param2 = 1;
	env->getMap().addNodeWithEvent(p0,n);
}

void plantgrowth_plant(ServerEnvironment *env, v3s16 p0, s16 height)
{
	u32 tod = env->getTimeOfDay();
	MapNode n = env->getMap().getNodeNoEx(p0);
	u8 light = n.getLightBlend(env->getDayNightRatio());
	content_t c = n.getContent();
	// only grow during the day or with good light
	if (light < LIGHT_MAX-3 && (tod < 6000 || tod > 18000))
		return;
	if (n.param2 == 0 && content_features(n).plantgrowth_trellis_node != CONTENT_IGNORE) {
		c = content_features(n).plantgrowth_trellis_node;
		v3s16 p;
		std::vector<content_t> search;
		search.push_back(c);
		search.push_back(CONTENT_TRELLIS);
		search.push_back(CONTENT_TRELLIS_DEAD_VINE);
		if (!env->searchNear(p0,v3s16(-1,0,-1),v3s16(1,0,1),search,&p))
			return;
		p0 = p;
		n = env->getMap().getNodeNoEx(p0);
		if (n.getContent() != c) {
			n.setContent(c);
			n.param2 = 1;
			env->getMap().addNodeWithEvent(p0,n);
			return;
		}
	}
	if (n.param2 == 0) {
		bool grow = false;
		if (!height)
			height = content_features(c).plantgrowth_max_height;
		if (content_features(n).plantgrowth_on_trellis) {
			for (s16 h=1; !grow && h<height; h++) {
				p0.Y++;
				n = env->getMap().getNodeNoEx(p0);
				if (n.getContent() == CONTENT_TRELLIS || n.getContent() == CONTENT_TRELLIS_DEAD_VINE) {
					n.setContent(c);
					n.param2 = 0;
					grow = true;
					break;
				}else if (n.getContent() == c) {
					if (n.param2 == 0)
						continue;
					grow = true;
					break;
				}
			}
		}else{
			for (s16 h=1; !grow && h<height; h++) {
				p0.Y++;
				n = env->getMap().getNodeNoEx(p0);
				if (n.getContent() == CONTENT_AIR) {
					n.setContent(c);
					n.param2 = 0;
					grow = true;
					break;
				}else if (n.getContent() == c) {
					if (n.param2 == 0)
						continue;
					grow = true;
					break;
				}
			}
		}
		if (!grow)
			return;
	}

	if (n.param2 == 15) {
		n.param2 = 0;
	}else{
		n.param2++;
	}
	env->getMap().addNodeWithEvent(p0,n);
}

void plantgrowth_grass(ServerEnvironment *env, v3s16 p0)
{
	u32 tod = env->getTimeOfDay();
	MapNode n = env->getMap().getNodeNoEx(p0);
	u8 light = n.getLightBlend(env->getDayNightRatio());
	content_t c = content_features(n.getContent()).special_alternate_node;
	if (c == CONTENT_IGNORE)
		return;
	bool add = false;
	{
		u8 p = n.param2&0xF0;
		if ((p&(1<<7)) == 0) {
			MapNode nn = env->getMap().getNodeNoEx(p0+v3s16(0,0,-1));
			if (nn.getContent() == c)
				p |= 1<<7;
		}
		if ((p&(1<<6)) == 0) {
			MapNode nn = env->getMap().getNodeNoEx(p0+v3s16(0,0,1));
			if (nn.getContent() == c)
				p |= 1<<6;
		}
		if ((p&(1<<5)) == 0) {
			MapNode nn = env->getMap().getNodeNoEx(p0+v3s16(-1,0,0));
			if (nn.getContent() == c)
				p |= 1<<5;
		}
		if ((p&(1<<4)) == 0) {
			MapNode nn = env->getMap().getNodeNoEx(p0+v3s16(1,0,0));
			if (nn.getContent() == c)
				p |= 1<<4;
		}
		if (!n.param2 && p == 0 && myrand_range(0,20) != 0)
			return;
		if (!n.param2 || p != (n.param2&0xF0)) {
			n.param2 &= 0x0F;
			n.param2 |= p;
			add = true;
		}
	}
	// only grow during the day or with good light
	if (light > LIGHT_MAX-4 || (tod > 6000 && tod < 18000)) {
		u8 p = n.param2&0xF0;
		u8 g = n.param2&0x0F;
		g++;
		if (g > 15) {
			n.setContent(c);
		}else{
			n.param2 = (p|g);
		}
		add = true;
	}

	if (add)
		env->getMap().addNodeWithEvent(p0,n);
}

void plantgrowth_cactus(ServerEnvironment *env, v3s16 p0)
{
	int height = 1;
	for (;; height++) {
		MapNode nn = env->getMap().getNodeNoEx(p0+v3s16(0,height,0));
		if (nn.getContent() == CONTENT_AIR) {
			break;
		}else if (nn.getContent() != CONTENT_CACTUS || nn.envticks < 5) {
			return;
		}
	}

	if (height > 4 || myrand_range(0,3) != 0)
		return;

	MapNode n(CONTENT_CACTUS);
	env->getMap().addNodeWithEvent(p0+v3s16(0,height,0),n);
}
