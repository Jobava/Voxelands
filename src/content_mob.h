/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mob.h
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
************************************************************************/

#ifndef MOB_HEADER
#define MOB_HEADER

#include "mapnode.h"
#include "serverobject.h"
#ifndef SERVER
#include "common_irrlicht.h"
#endif
#include <vector>
#include <map>

enum MobPunchAction
{
	MPA_IGNORE = 0,
	MPA_DIE,
	MPA_PICKUP
};

enum MobMotion
{
	MM_STATIC = 0,
	MM_WANDER,
	MM_SEEKER,
	MM_SENTRY
};

enum MobMotionType
{
	MMT_WALK = 0,
	MMT_FLY,
	MMT_SWIM
};

enum MobState
{
	MS_WILD = 0,
	MS_TAME
};

enum MobAnimation
{
	MA_STAND = 0,
	MA_MOVE,
	MA_ATTACK
};

#define CONTENT_MOB_MASK 0x2000

struct MobFeatures {
	content_t content;
	u8 level;
#ifndef SERVER
	TileSpec tiles[6];
	video::ITexture *texture;
#endif
	std::string model;
	std::map<MobAnimation,int[2]> animations;
	std::vector<aabb3f> nodeboxes;
	aabb3f collisionbox;

	MobPunchAction punch_action;
	MobMotion motion;
	MobMotionType motion_type;
	MobState spawn_state;
	bool is_tamable;
	content_t tamed_mob;
	u16 hp;
	std::string dropped_item;

	MobFeatures()
	{
		reset();
	}

	/*
		Gets list of node boxes
	*/
        std::vector<aabb3f> getNodeBoxes()
        {
		return nodeboxes;
	}

	void setNodeBox(aabb3f bb)
	{
		nodeboxes.clear();
		nodeboxes.push_back(bb);
	}

	void addNodeBox(aabb3f bb)
	{
		nodeboxes.push_back(bb);
	}

	void setCollisionBox(aabb3f cb)
	{
		collisionbox = cb;
	}

	aabb3f getCollisionBox()
	{
		if (collisionbox.MinEdge != collisionbox.MaxEdge)
			return collisionbox;
		if (!nodeboxes.size())
			return aabb3f(-0.5,-0.5,-0.5,0.5,0.5,0.5);
		return nodeboxes[0];
	}

#ifdef SERVER
	void setTexture(std::string name)
	{}
	void setBoxTexture(u16 i, std::string name, u8 alpha=255)
	{}
	void setAllBoxTextures(std::string name, u8 alpha=255)
	{}
#else
	void setTexture(std::string name);
	void setBoxTexture(u16 i, std::string name, u8 alpha=255);

	void setAllBoxTextures(std::string name, u8 alpha=255)
	{
		for (u16 i=0; i<6; i++) {
			setBoxTexture(i, name, alpha);
		}
	}
#endif

	void reset()
	{
		content = CONTENT_IGNORE;
		model = "";
		nodeboxes.clear();
		punch_action = MPA_DIE;
		motion = MM_STATIC;
		motion_type = MMT_WALK;
		spawn_state = MS_WILD;
		is_tamable = false;
		tamed_mob = CONTENT_IGNORE;
		hp = 20;
		dropped_item = "";
	}
};

inline u8 mobLevelI(std::string level)
{
	if (level == "destructive")
		return MOB_DESTRUCTIVE;
	if (level == "aggressive")
		return MOB_AGGRESSIVE;
	return MOB_PASSIVE;
}

inline std::string mobLevelS(u8 level)
{
	if (level == MOB_DESTRUCTIVE)
		return std::string("destructive");
	if (level == MOB_AGGRESSIVE)
		return std::string("aggressive");
	return std::string("passive");
}

void content_mob_init();

#endif
