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
	MPA_HARM,
	MPA_PICKUP
};

enum MobMotion
{
	MM_STATIC = 0,
	MM_WANDER,
	MM_SEEKER,
	MM_SENTRY,
	MM_THROWN,
	MM_CONSTANT
};

enum MobMotionType
{
	MMT_WALK = 0,
	MMT_FLY,
	MMT_FLYLOW,
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

enum MobAnimationKey
{
	MA_STAND_START = 0,
	MA_STAND_END,
	MA_MOVE_START,
	MA_MOVE_END,
	MA_ATTACK_START,
	MA_ATTACK_END
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
	std::map<MobAnimationKey,int> animations;
	v3f model_scale;
	v3f model_offset;
	std::vector<aabb3f> nodeboxes;
	aabb3f collisionbox;

	MobPunchAction punch_action;
	MobMotion motion;
	MobMotionType motion_type;
	MobState spawn_state;
	bool is_tamable;
	bool notices_player;
	content_t tamed_mob;
	content_t attack_throw_object;
	v3f attack_throw_offset;
	u8 attack_player_damage;
	v3f attack_player_range;
	u8 glow_light;
	u8 attack_glow_light;
	u16 hp;
	std::string dropped_item;
	f32 lifetime;
	u16 contact_explosion_diameter;

	content_t spawn_on;
	content_t spawn_in;
	u8 spawn_min_light;
	u8 spawn_max_light;

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
		aabb3f b = nodeboxes[0];
		b.MinEdge /= BS;
		b.MaxEdge /= BS;
		return b;
	}

	v3f getSize()
	{
		aabb3f c = getCollisionBox();
		return v3f(c.MaxEdge.X-c.MinEdge.X,c.MaxEdge.Y-c.MinEdge.Y,c.MaxEdge.Z-c.MinEdge.Z);
	}

	v3s16 getSizeBlocks()
	{
		v3f s = getSize();
		return v3s16(s.X+0.5,s.Y+0.5,s.Z+0.5);
	}

	void getAnimationFrames(MobAnimation type, int *start, int *end);
	void setAnimationFrames(MobAnimation type, int start, int end);

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
		model_scale = v3f(0,0,0);
		model_offset = v3f(0,0,0);
		nodeboxes.clear();
		punch_action = MPA_DIE;
		motion = MM_STATIC;
		motion_type = MMT_WALK;
		spawn_state = MS_WILD;
		is_tamable = false;
		notices_player = false;
		tamed_mob = CONTENT_IGNORE;
		attack_throw_object = CONTENT_IGNORE;
		attack_throw_offset = v3f(0,0,0);
		attack_player_damage = 0;
		attack_player_range = v3f(0,0,0);
		glow_light = 0;
		attack_glow_light = 0;
		hp = 20;
		dropped_item = "";
		lifetime = 0.0;
		contact_explosion_diameter = 0;
		spawn_on = CONTENT_IGNORE;
		spawn_in = CONTENT_IGNORE;
		spawn_min_light = 0;
		spawn_max_light = LIGHT_MAX;
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

class ServerEnvironment;
bool content_mob_spawn(ServerEnvironment *env, v3s16 pos);

MobFeatures & content_mob_features(content_t i);
void content_mob_init();

#define CONTENT_MOB_RAT (CONTENT_MOB_MASK | 0x01)
#define CONTENT_MOB_FIREFLY (CONTENT_MOB_MASK | 0x02)
#define CONTENT_MOB_OERKKI (CONTENT_MOB_MASK | 0x03)
#define CONTENT_MOB_DUNGEON_MASTER (CONTENT_MOB_MASK | 0x04)
#define CONTENT_MOB_FIREBALL (CONTENT_MOB_MASK | 0x05)
#define CONTENT_MOB_DEER (CONTENT_MOB_MASK | 0x06)
#define CONTENT_MOB_SHEEP (CONTENT_MOB_MASK | 0x07)
#define CONTENT_MOB_FISH (CONTENT_MOB_MASK | 0x08)
#define CONTENT_MOB_SHARK (CONTENT_MOB_MASK | 0x09)
#define CONTENT_MOB_WOLF (CONTENT_MOB_MASK | 0x0A)

#endif
