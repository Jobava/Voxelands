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
#include "content_toolitem.h"
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
	MPA_PICKUP,
	MPA_MOVE
};

enum MobMotion
{
	MM_STATIC = 0,
	MM_WANDER,
	MM_SEEKER,
	MM_SENTRY,
	MM_THROWN,
	MM_CONSTANT,
	MM_FOLLOW
};

enum MobMotionType
{
	MMT_WALK = 0,
	MMT_FLY,
	MMT_FLYLOW,
	MMT_SWIM
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

enum MobDrawType
{
	MDT_NOTHING = 0,
	MDT_AUTO,
	MDT_MODEL,
	MDT_BLOCK,
	MDT_SPRITE,
	MDT_EXTRUDED
};


#define CONTENT_MOB_MASK 0x2000

struct MobFeatures {
	content_t content;
	std::wstring description;
	u8 level;
#ifndef SERVER
	TileSpec tiles[6];
#endif
	std::string texture;
	MobDrawType texture_display;
	std::string model;
	std::map<MobAnimationKey,int> animations;
	v3f model_scale;
	v3f model_offset;
	v3f model_rotation;
	std::vector<NodeBox> nodeboxes;
	aabb3f collisionbox;

	MobPunchAction punch_action;
	MobMotion motion;
	MobMotionType motion_type;
	MobMotion angry_motion;
	f32 static_thrown_speed;
	bool notices_player;
	content_t tamed_mob;
	content_t attack_throw_object;
	v3f attack_throw_offset;
	u8 attack_player_damage;
	v3f attack_player_range;
	u8 attack_mob_damage;
	v3f attack_mob_range;
	u8 glow_light;
	u8 attack_glow_light;
	u16 hp;
	std::string dropped_item;
	ToolType special_punch_item;
	content_t special_dropped_item;
	u16 special_dropped_count;
	u16 special_dropped_max;
	f32 lifetime;
	u16 contact_explosion_diameter;
	content_t contact_place_node;
	content_t contact_drop_item;
	bool moves_silently;

	std::string sound_spawn;
	std::string sound_death;
	std::string sound_attack;
	std::string sound_punch;
	std::string sound_random;
	std::string sound_random_extra;

	s16 spawn_min_height;
	s16 spawn_max_height;
	s16 spawn_chance;
	s16 spawn_group;
	bool spawn_water;

	bool spawn_naturally;

	MobFeatures()
	{
		reset();
	}

	/*
		Gets list of node boxes
	*/
	std::vector<NodeBox> getNodeBoxes()
	{
		return nodeboxes;
	}

	void setNodeBox(NodeBox bb)
	{
		model_offset = v3f(0,0.5,0);
		nodeboxes.clear();
		nodeboxes.push_back(bb);
	}

	void addNodeBox(NodeBox bb)
	{
		nodeboxes.push_back(bb);
	}

	void setCollisionBox(aabb3f cb)
	{
		cb.MinEdge.Y -= 0.5*BS;
		cb.MaxEdge.Y -= 0.5*BS;
		collisionbox = cb;
	}

	aabb3f getCollisionBox()
	{
		if (collisionbox.MinEdge != collisionbox.MaxEdge)
			return collisionbox;
		if (!nodeboxes.size())
			return aabb3f(-0.5*BS,0.,-0.5*BS,0.5*BS,BS,0.5*BS);
		aabb3f b = nodeboxes[0].m_box;
		return b;
	}

	v3f getSize()
	{
		aabb3f c = getCollisionBox();
		return v3f((c.MaxEdge.X-c.MinEdge.X)/BS,(c.MaxEdge.Y-c.MinEdge.Y)/BS,(c.MaxEdge.Z-c.MinEdge.Z)/BS);
	}

	v3s16 getSizeBlocks()
	{
		v3f s = getSize();
		return v3s16(MYMAX(s.X+0.5,1.0),MYMAX(s.Y+0.5,1.0),MYMAX(s.Z+0.5,1.0));
	}

	void getAnimationFrames(MobAnimation type, int *start, int *end);
	void setAnimationFrames(MobAnimation type, int start, int end);
	void setTexture(std::string name) {texture = name;}

#ifdef SERVER
	void setBoxTexture(u16 i, std::string name, u8 alpha=255)
	{}
	void setAllBoxTextures(std::string name, u8 alpha=255)
	{}
#else
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
		description = L"";
		texture = "";
		texture_display = MDT_AUTO;
		model = "";
		model_scale = v3f(1.0,1.0,1.0);
		model_offset = v3f(0,0,0);
		model_rotation = v3f(0,0,0);
		nodeboxes.clear();
		punch_action = MPA_DIE;
		motion = MM_STATIC;
		motion_type = MMT_WALK;
		angry_motion = MM_STATIC;
		static_thrown_speed = 20.0;
		notices_player = false;
		tamed_mob = CONTENT_IGNORE;
		attack_throw_object = CONTENT_IGNORE;
		attack_throw_offset = v3f(0,0,0);
		attack_player_damage = 0;
		attack_player_range = v3f(0,0,0);
		attack_mob_damage = 0;
		attack_mob_range = v3f(0,0,0);
		glow_light = 0;
		attack_glow_light = 0;
		hp = 20;
		dropped_item = "";
		special_punch_item = TT_NONE;
		special_dropped_item = CONTENT_IGNORE;
		special_dropped_count = 0;
		special_dropped_max = 0;
		lifetime = 0.0;
		contact_explosion_diameter = 0;
		contact_place_node = CONTENT_IGNORE;
		contact_drop_item = CONTENT_IGNORE;
		moves_silently = false;
		sound_spawn = "";
		sound_death = "";
		sound_attack = "";
		sound_punch = "mob-dig";
		sound_random = "";
		sound_random_extra = "";
		spawn_min_height = -20000;
		spawn_max_height = 100;
		spawn_chance = 1;
		spawn_group = 1;
		spawn_water = false;
		spawn_naturally = true;
	}
};

inline u8 mobLevelI(std::string level)
{
	if (level == "destructive")
		return MOB_DESTRUCTIVE;
	if (level == "aggressive")
		return MOB_AGGRESSIVE;
	if (level == "none")
		return MOB_NONE;
	return MOB_PASSIVE;
}

inline std::string mobLevelS(u8 level)
{
	if (level == MOB_DESTRUCTIVE)
		return std::string("destructive");
	if (level == MOB_AGGRESSIVE)
		return std::string("aggressive");
	if (level == MOB_NONE)
		return std::string("none");
	return std::string("passive");
}

class ServerEnvironment;
bool content_mob_spawn(ServerEnvironment *env, v3s16 pos, u32 active_object_count);
void mob_spawn(v3s16 pos, content_t mob, ServerEnvironment *env);
void mob_spawn_passive(v3s16 pos, bool water, ServerEnvironment *env);
void mob_spawn_hostile(v3s16 pos, bool water, ServerEnvironment *env);

MobFeatures & content_mob_features(content_t i);
void content_mob_init();

#define CONTENT_MOB_RAT (CONTENT_MOB_MASK | 0x01)
#define CONTENT_MOB_FIREFLY (CONTENT_MOB_MASK | 0x02)
#define CONTENT_MOB_OERKKI (CONTENT_MOB_MASK | 0x03)
#define CONTENT_MOB_DUNGEON_MASTER (CONTENT_MOB_MASK | 0x04)
#define CONTENT_MOB_FIREBALL (CONTENT_MOB_MASK | 0x05)
#define CONTENT_MOB_DOE (CONTENT_MOB_MASK | 0x06)
#define CONTENT_MOB_STAG (CONTENT_MOB_MASK | 0x07)
#define CONTENT_MOB_TAMESTAG (CONTENT_MOB_MASK | 0x08)
#define CONTENT_MOB_FISH (CONTENT_MOB_MASK | 0x09)
#define CONTENT_MOB_SHARK (CONTENT_MOB_MASK | 0x0A)
#define CONTENT_MOB_WOLF (CONTENT_MOB_MASK | 0x0B)
#define CONTENT_MOB_TAMEWOLF (CONTENT_MOB_MASK | 0x0C)
#define CONTENT_MOB_SHEEP (CONTENT_MOB_MASK | 0x0D)
#define CONTENT_MOB_SNOWBALL (CONTENT_MOB_MASK | 0x0E)
#define CONTENT_MOB_ARROW (CONTENT_MOB_MASK | 0x0F)
#define CONTENT_MOB_GREY_KITTY (CONTENT_MOB_MASK | 0x10)
#define CONTENT_MOB_WHITE_KITTY (CONTENT_MOB_MASK | 0x11)
#define CONTENT_MOB_SIAMESE_KITTY (CONTENT_MOB_MASK | 0x12)
#define CONTENT_MOB_GINGER_KITTY (CONTENT_MOB_MASK | 0x13)
#define CONTENT_MOB_SHEARED_SHEEP (CONTENT_MOB_MASK | 0x14)

// increment me if you add a mob!
#define CONTENT_MOB_COUNT 21

#endif
