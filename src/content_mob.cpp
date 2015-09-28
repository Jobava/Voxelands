/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mob.cpp
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

#include "serverobject.h"
#include "content_sao.h"
#include "content_mob.h"
#include "content_list.h"
#include "content_craftitem.h"
#include "main.h"
#include "settings.h"
#include "environment.h"
#include "map.h"
#include "profiler.h"
#include "intl.h"

MobFeatures g_content_mob_features[CONTENT_MOB_COUNT];

MobFeatures & content_mob_features(content_t c)
{
	static MobFeatures ignore = MobFeatures();
	if ((c&CONTENT_MOB_MASK) != CONTENT_MOB_MASK)
		return ignore;

	u16 i = (c&~CONTENT_MOB_MASK);

	if (i >= CONTENT_MOB_COUNT)
		return ignore;

	if (g_content_mob_features[i].content != c)
		return ignore;

	return g_content_mob_features[i];
}

#ifndef SERVER
void MobFeatures::setBoxTexture(u16 i, std::string name, u8 alpha)
{
	if (g_texturesource)
		tiles[i].texture = g_texturesource->getTexture(name);

	if (alpha != 255) {
		tiles[i].alpha = alpha;
		tiles[i].material_type = MATERIAL_ALPHA_VERTEX;
	}
}
#endif
void MobFeatures::getAnimationFrames(MobAnimation type, int *start, int *end)
{
	*start = 0;
	*end = 0;
	switch (type) {
	case MA_STAND:
		*start = animations[MA_STAND_START];
		*end = animations[MA_STAND_END];
		break;
	case MA_MOVE:
		*start = animations[MA_MOVE_START];
		*end = animations[MA_MOVE_END];
		break;
	case MA_ATTACK:
		*start = animations[MA_ATTACK_START];
		*end = animations[MA_ATTACK_END];
		break;
	default:;
	}
	if (type != MA_STAND && start == end) {
		*start = animations[MA_STAND_START];
		*end = animations[MA_STAND_END];
	}
}
void MobFeatures::setAnimationFrames(MobAnimation type, int start, int end)
{
	switch (type) {
	case MA_STAND:
		animations[MA_STAND_START] = start;
		animations[MA_STAND_END] = end;
		break;
	case MA_MOVE:
		animations[MA_MOVE_START] = start;
		animations[MA_MOVE_END] = end;
		break;
	case MA_ATTACK:
		animations[MA_ATTACK_START] = start;
		animations[MA_ATTACK_END] = end;
		break;
	default:;
	}
}

#define PP(x) "("<<(x).X<<","<<(x).Y<<","<<(x).Z<<")"

bool content_mob_spawn(ServerEnvironment *env, v3s16 pos, u32 active_object_count)
{
	ScopeProfiler sp(g_profiler, "SEnv: content_mob_spawn");
	if (active_object_count > 20)
		return false;
	int rand = myrand();
	if (pos.Y < 0 && rand%2 == 0)
		return false;
	assert(env);
	Map *map = &env->getMap();

	std::vector<content_t> can;
	MapNode n = map->getNodeNoEx(pos);
	MapNode a1 = map->getNodeNoEx(pos+v3s16(0,1,0));
	MapNode a2 = map->getNodeNoEx(pos+v3s16(0,2,0));
	content_t c0 = n.getContent();
	content_t c1 = a1.getContent();
	content_t c2 = a2.getContent();

	if (c0 == CONTENT_IGNORE || c1 == CONTENT_IGNORE || c2 == CONTENT_IGNORE)
		return false;

	if ((c1 != CONTENT_AIR && c1 != CONTENT_WATERSOURCE) && c1 != c2)
		return false;

	for (u16 i=0; i<CONTENT_MOB_COUNT; i++) {
		MobFeatures m = g_content_mob_features[i];
		if (m.spawn_min_height > pos.Y)
			continue;
		if (m.spawn_max_height < pos.Y)
			continue;
		if (m.spawn_chance > 1 && rand%m.spawn_chance != 0)
			continue;
		can.push_back(i);
	}

	if (can.size() == 0)
		return false;

	MobFeatures m;
	u32 index = 0;

	if (can.size() > 1)
		index = myrand_range(0,can.size()-1);

	m = g_content_mob_features[can[index]];

	if (m.content == CONTENT_IGNORE)
		return false;

	v3f p = intToFloat(pos+v3s16(0,1,0), BS);
	actionstream<<"A mob of type "<<m.content<<" spawns at "<<PP(floatToInt(p,BS))<<std::endl;
	ServerActiveObject *obj = new MobSAO(env, 0, p, m.content);
	u16 id = env->addActiveObject(obj);
	if (!id) {
		actionstream<<"A mob of type "<<m.content<<" didn't spawn at "<<PP(floatToInt(p,BS))<<std::endl;
		return false;
	}

	if (m.sound_spawn != "")
		env->addEnvEvent(ENV_EVENT_SOUND,p,m.sound_spawn);

	return true;
}

void mob_spawn(v3s16 pos, content_t mob, ServerEnvironment *env)
{

	MobFeatures &m = content_mob_features(mob);

	if (m.content == CONTENT_IGNORE)
		return;

	v3f p = intToFloat(pos+v3s16(0,1,0), BS);
	actionstream<<"A mob of type "<<m.content<<" spawns at "<<PP(floatToInt(p,BS))<<std::endl;
	ServerActiveObject *obj = new MobSAO(env, 0, p, m.content);
	u16 id = env->addActiveObject(obj);
	if (!id) {
		actionstream<<"A mob of type "<<m.content<<" didn't spawn at "<<PP(floatToInt(p,BS))<<std::endl;
		return;
	}

	if (id && m.sound_spawn != "")
		env->addEnvEvent(ENV_EVENT_SOUND,p,m.sound_spawn);
}

void mob_spawn_passive(v3s16 pos, bool water, ServerEnvironment *env)
{
	std::vector<content_t> can;
	int rand = myrand();
	for (u16 i=0; i<CONTENT_MOB_COUNT; i++) {
		MobFeatures m = g_content_mob_features[i];
		if (!m.spawn_naturally)
			continue;
		if (m.level > MOB_PASSIVE)
			continue;
		if (m.spawn_water != water)
			continue;
		if (m.spawn_min_height > pos.Y)
			continue;
		if (m.spawn_max_height < pos.Y)
			continue;
		if (m.spawn_chance > 1 && rand%m.spawn_chance != 0)
			continue;
		can.push_back(i);
	}

	if (can.size() == 0)
		return;

	MobFeatures m;
	u32 index = 0;

	if (can.size() > 1)
		index = myrand_range(0,can.size()-1);

	m = g_content_mob_features[can[index]];

	if (m.content == CONTENT_IGNORE)
		return;

	v3s16 spots[9] = {
		v3s16(0,0,0),
		v3s16(-2,0,0),
		v3s16(-2,0,2),
		v3s16(-2,0,-2),
		v3s16(2,0,0),
		v3s16(2,0,-2),
		v3s16(2,0,2),
		v3s16(0,0,-2),
		v3s16(0,0,2)
	};
	u16 cnt = 0;
	for (u16 i=0; i<9 && cnt < m.spawn_group; i++) {
		v3s16 p = pos+spots[i];
		if (water) {
			if (env->getMap().getNodeNoEx(p+v3s16(0,1,0)).getContent() != CONTENT_WATERSOURCE)
				continue;
		}else{
			content_t c = env->getMap().getNodeNoEx(pos).getContent();
			bool have_spot = false;
			for (s16 x=-1; !have_spot && x<2; x++) {
			for (s16 y=-1; !have_spot && y<2; y++) {
			for (s16 z=-1; !have_spot && z<2; z++) {
				v3s16 pp = p+v3s16(x,y,z);
				if (env->getMap().getNodeNoEx(pp).getContent() != c)
					continue;
				pp.Y++;
				if (env->getMap().getNodeNoEx(pp).getContent() != CONTENT_AIR)
					continue;
				pp.Y++;
				if (env->getMap().getNodeNoEx(pp).getContent() != CONTENT_AIR)
					continue;
				p += v3s16(x,y,z);
				have_spot = true;
				break;
			}
			}
			}

			if (!have_spot)
				continue;
		}
		mob_spawn(p,m.content,env);
		cnt++;
	}
}

void mob_spawn_hostile(v3s16 pos, bool water, ServerEnvironment *env)
{
	std::vector<content_t> can;
	int rand = myrand();
	u8 level = mobLevelI(g_settings->get("max_mob_level"));
	if (level < MOB_AGGRESSIVE)
		return;
	for (u16 i=0; i<CONTENT_MOB_COUNT; i++) {
		MobFeatures m = g_content_mob_features[i];
		if (!m.spawn_naturally)
			continue;
		if (m.level < MOB_AGGRESSIVE)
			continue;
		if (m.level > level)
			continue;
		if (m.spawn_water != water)
			continue;
		if (m.spawn_min_height > pos.Y)
			continue;
		if (m.spawn_max_height < pos.Y)
			continue;
		if (m.spawn_chance > 1 && rand%m.spawn_chance != 0)
			continue;
		can.push_back(i);
	}

	if (can.size() == 0)
		return;

	MobFeatures m;
	u32 index = 0;

	if (can.size() > 1)
		index = myrand_range(0,can.size()-1);

	m = g_content_mob_features[can[index]];

	if (m.content == CONTENT_IGNORE)
		return;

	v3s16 spots[9] = {
		v3s16(0,0,0),
		v3s16(-2,0,0),
		v3s16(-2,0,2),
		v3s16(-2,0,-2),
		v3s16(2,0,0),
		v3s16(2,0,-2),
		v3s16(2,0,2),
		v3s16(0,0,-2),
		v3s16(0,0,2)
	};
	u16 cnt = 0;
	for (u16 i=0; i<9 && cnt < m.spawn_group; i++) {
		v3s16 p = pos+spots[i];
		if (water) {
			if (env->getMap().getNodeNoEx(p+v3s16(0,1,0)).getContent() != CONTENT_WATERSOURCE)
				continue;
		}else{
			content_t c = env->getMap().getNodeNoEx(pos).getContent();
			bool have_spot = false;
			for (s16 x=-1; !have_spot && x<2; x++) {
			for (s16 y=-1; !have_spot && y<2; y++) {
			for (s16 z=-1; !have_spot && z<2; z++) {
				v3s16 pp = p+v3s16(x,y,z);
				if (env->getMap().getNodeNoEx(pp).getContent() != c)
					continue;
				pp.Y++;
				if (env->getMap().getNodeNoEx(pp).getContent() != CONTENT_AIR)
					continue;
				pp.Y++;
				if (env->getMap().getNodeNoEx(pp).getContent() != CONTENT_AIR)
					continue;
				p += v3s16(x,y,z);
				have_spot = true;
				break;
			}
			}
			}

			if (!have_spot)
				continue;
		}
		mob_spawn(p,m.content,env);
		cnt++;
	}
}

void content_mob_init()
{
	content_t i;
	MobFeatures *f = NULL;

	i = CONTENT_MOB_RAT;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Rat");
	f->level = MOB_PASSIVE;
	f->model = "rat.x";
	f->setTexture("mob_rat.png");
	f->setAnimationFrames(MA_STAND,0,79);
	f->punch_action = MPA_PICKUP;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_RAT)+" 1";
	f->motion = MM_WANDER;
	f->moves_silently = true;
	f->spawn_max_height = -10;
	f->spawn_group = 3;
	f->spawn_naturally = false;
	f->lifetime = 900.0;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_FIREFLY;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Firefly");
	f->level = MOB_PASSIVE;
	f->model_scale = v3f(0.5,0.5,0.5);
	f->setTexture("mob_firefly.png");
	f->punch_action = MPA_PICKUP;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_FIREFLY)+" 1";
	f->motion = MM_WANDER;
	f->motion_type = MMT_FLYLOW;
	f->glow_light = LIGHT_MAX-1;
	f->moves_silently = true;
	f->spawn_min_height = -5;
	f->spawn_max_height = 50;
	f->spawn_naturally = false;
	f->lifetime = 1200.0;
	f->setCollisionBox(aabb3f(-BS/4.,-BS/6.,-BS/4., BS/4.,BS/6.,BS/4.));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_OERKKI;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Oerkki");
	f->level = MOB_AGGRESSIVE;
	if (g_settings->getBool("enable_supernatural")) {
		f->model = "oerkki.x";
		f->model_scale = v3f(4,4,4);
		f->setTexture("mob_oerkki.png");
		f->setAnimationFrames(MA_STAND,24,36);
		f->setAnimationFrames(MA_ATTACK,37,49);
	}else{
		f->setNodeBox(NodeBox(
			-0.3125*BS,-0.4375*BS,-0.4375*BS,0.3125*BS,-0.25*BS,0.4375*BS
		));
		f->addNodeBox(NodeBox(
			0.3125*BS,-0.5*BS,0.1875*BS,0.5*BS,-0.1875*BS,0.5*BS
		));
		f->addNodeBox(NodeBox(
			0.3125*BS,-0.5*BS,-0.5*BS,0.5*BS,-0.1875*BS,-0.1875*BS
		));
		f->addNodeBox(NodeBox(
			-0.5*BS,-0.5*BS,0.1875*BS,-0.3125*BS,-0.1875*BS,0.5*BS
		));
		f->addNodeBox(NodeBox(
			-0.5*BS,-0.5*BS,-0.5*BS,-0.3125*BS,-0.1875*BS,-0.1875*BS
		));
		f->addNodeBox(NodeBox(
			-0.125*BS,-0.25*BS,-0.125*BS,0.125*BS,1.*BS,0.125*BS
		));
		f->addNodeBox(NodeBox(
			-0.375*BS,0.6875*BS,-0.5*BS,-0.125*BS,0.9375*BS,0.5*BS
		));
		f->addNodeBox(NodeBox(
			0.125*BS,0.6875*BS,-0.5*BS,0.375*BS,0.9375*BS,0.5*BS
		));
		f->setAllBoxTextures("copper_block.png");
	}
	f->punch_action = MPA_HARM;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_OERKKI_DUST)+" 2";
	f->motion = MM_SEEKER;
	f->moves_silently = true;
	f->spawn_max_height = 2;
	f->sound_spawn = "mob-oerkki-spawn";
	f->notices_player = true;
	f->attack_player_damage = 3;
	f->attack_player_range = v3f(1,1,1);
	f->lifetime = 600.0;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS*2.,BS/3.));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_DUNGEON_MASTER;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Dungeon Master");
	f->level = MOB_DESTRUCTIVE;
	if (g_settings->getBool("enable_supernatural")) {
		f->model = "dungeon_master.b3d";
		f->model_rotation = v3f(0,-90,0);
		f->model_offset = v3f(0,1.0,0);
		f->setTexture("mob_dungeon_master.png");
		f->setAnimationFrames(MA_STAND,1,30);
		f->setAnimationFrames(MA_MOVE,31,60);
		f->setAnimationFrames(MA_ATTACK,61,90);
	}else{
		f->setNodeBox(NodeBox(
			-0.3125*BS,-0.4375*BS,-0.4375*BS,0.3125*BS,-0.25*BS,0.4375*BS
		));
		f->addNodeBox(NodeBox(
			0.3125*BS,-0.5*BS,0.1875*BS,0.5*BS,-0.1875*BS,0.5*BS
		));
		f->addNodeBox(NodeBox(
			0.3125*BS,-0.5*BS,-0.5*BS,0.5*BS,-0.1875*BS,-0.1875*BS
		));
		f->addNodeBox(NodeBox(
			-0.5*BS,-0.5*BS,0.1875*BS,-0.3125*BS,-0.1875*BS,0.5*BS
		));
		f->addNodeBox(NodeBox(
			-0.5*BS,-0.5*BS,-0.5*BS,-0.3125*BS,-0.1875*BS,-0.1875*BS
		));
		f->addNodeBox(NodeBox(
			-0.125*BS,-0.25*BS,-0.125*BS,0.125*BS,1.*BS,0.125*BS
		));
		f->addNodeBox(NodeBox(
			-0.375*BS,0.6875*BS,-0.5*BS,-0.125*BS,0.9375*BS,0.5*BS
		));
		f->addNodeBox(NodeBox(
			0.125*BS,0.6875*BS,-0.5*BS,0.375*BS,0.9375*BS,0.5*BS
		));
		f->setAllBoxTextures("steel_block.png");
	}
	f->punch_action = MPA_HARM;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_GUNPOWDER)+" 4";
	f->motion = MM_SENTRY;
	f->notices_player = true;
	f->attack_throw_object = CONTENT_MOB_FIREBALL;
	f->attack_glow_light = LIGHT_MAX-1;
	f->attack_throw_offset = v3f(0,1.4,-1.0);
	f->lifetime = 600.0;
	f->setCollisionBox(aabb3f(-0.75*BS, 0.*BS, -0.75*BS, 0.75*BS, 2.0*BS, 0.75*BS));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_FIREBALL;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Fireball");
	f->level = MOB_DESTRUCTIVE;
	f->setTexture("mob_fireball.png");
	f->punch_action = MPA_IGNORE;
	f->motion = MM_CONSTANT;
	f->motion_type = MMT_FLY;
	f->glow_light = LIGHT_MAX-1;
	f->notices_player = true;
	f->moves_silently = true;
	f->attack_player_damage = 3;
	f->attack_player_range = v3f(2,2,2);
	f->contact_explosion_diameter = 3;
	f->spawn_naturally = false;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.));

	i = CONTENT_MOB_DOE;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Doe");
	f->level = MOB_PASSIVE;
	f->hp = 30;
	f->model = "doe.x";
	f->model_scale = v3f(1.0,1.0,1.0);
	f->model_rotation = v3f(0,180,0);
	f->model_offset = v3f(0,0,0);
	f->setTexture("mob_doe.png");
	f->setAnimationFrames(MA_STAND,0,240);
	f->setAnimationFrames(MA_MOVE,241,300);
	f->setAnimationFrames(MA_ATTACK,241,300);
	f->punch_action = MPA_HARM;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_FUR)+" 2";
	f->motion = MM_SEEKER;
	f->motion_type = MMT_WALK;
	f->sound_random = "mob-deer-env";
	f->spawn_min_height = -5;
	f->spawn_max_height = 40;
	f->spawn_group = 3;
	f->lifetime = 1200.0;
	f->setCollisionBox(aabb3f(-0.6*BS, 0., -0.6*BS, 0.6*BS, 1.25*BS, 0.6*BS));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_STAG;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Stag");
	f->level = MOB_AGGRESSIVE;
	f->hp = 40;
	f->model = "stag.x";
	f->model_scale = v3f(1.1,1.1,1.1);
	f->model_rotation = v3f(0,180,0);
	f->model_offset = v3f(0,0,0);
	f->setTexture("mob_doe.png");
	f->setAnimationFrames(MA_STAND,0,240);
	f->setAnimationFrames(MA_MOVE,241,300);
	f->setAnimationFrames(MA_ATTACK,301,324);
	f->punch_action = MPA_HARM;
	f->tamed_mob = CONTENT_MOB_TAMESTAG;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_MEAT)+" 2";
	f->motion = MM_WANDER;
	f->motion_type = MMT_WALK;
	f->angry_motion = MM_SEEKER;
	f->sound_random = "mob-deer-env";
	f->spawn_min_height = -5;
	f->spawn_max_height = 40;
	f->spawn_chance = 2;
	f->notices_player = true;
	f->attack_player_damage = 3;
	f->attack_player_range = v3f(1,1,1);
	f->lifetime = 900.0;
	f->setCollisionBox(aabb3f(-0.7*BS, 0., -0.7*BS, 0.7*BS, 1.5*BS, 0.7*BS));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_TAMESTAG;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Stag");
	f->level = MOB_PASSIVE;
	f->hp = 40;
	f->model = "stag.b3d";
	f->model_scale = v3f(1,1,1);
	f->model_rotation = v3f(0,-90,0);
	f->model_offset = v3f(0,1.1,0);
	f->setTexture("mob_tamestag.png");
	f->setAnimationFrames(MA_STAND,61,120);
	f->setAnimationFrames(MA_MOVE,0,60);
	f->setAnimationFrames(MA_ATTACK,0,60);
	f->punch_action = MPA_HARM;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_FUR)+" 2";
	f->motion = MM_SEEKER;
	f->motion_type = MMT_WALK;
	f->sound_random = "mob-deer-env";
	f->notices_player = true;
	f->lifetime = 1800.0;
	f->spawn_naturally = false;
	f->setCollisionBox(aabb3f(-0.7*BS, 0., -0.7*BS, 0.7*BS, 1.5*BS, 0.7*BS));

	i = CONTENT_MOB_FISH;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Fish");
	f->level = MOB_PASSIVE;
	f->model = "fish.b3d";
	f->model_rotation = v3f(0,-90,0);
	f->model_offset = v3f(0,0.5,0);
	f->setTexture("mob_fish.png");
	f->setAnimationFrames(MA_STAND,1,80);
	f->setAnimationFrames(MA_MOVE,81,155);
	f->punch_action = MPA_HARM;
	f->special_punch_item = TT_SPEAR;
	f->special_dropped_item = CONTENT_CRAFTITEM_FISH;
	f->special_dropped_count = 1;
	f->special_dropped_max = 0;
	f->motion = MM_WANDER;
	f->motion_type = MMT_SWIM;
	f->moves_silently = true;
	f->spawn_group = 3;
	f->spawn_water = true;
	f->hp = 5;
	f->lifetime = 1200.0;
	f->setCollisionBox(aabb3f(-0.25*BS, 0.25*BS, -0.25*BS, 0.25*BS, 0.75*BS, 0.25*BS));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_SHARK;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Shark");
	f->level = MOB_AGGRESSIVE;
	f->hp = 40;
	f->model = "shark.b3d";
	f->model_scale = v3f(1,1,1);
	f->model_rotation = v3f(0,-90,0);
	f->model_offset = v3f(0,0.5,0);
	f->setTexture("mob_shark.png");
	f->setAnimationFrames(MA_STAND,1,80);
	f->setAnimationFrames(MA_MOVE,80,160);
	f->setAnimationFrames(MA_ATTACK,80,160);
	f->punch_action = MPA_HARM;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_MEAT)+" 2";
	f->motion = MM_SEEKER;
	f->motion_type = MMT_SWIM;
	f->moves_silently = true;
	f->spawn_water = true;
	f->notices_player = true;
	f->attack_player_damage = 3;
	f->attack_player_range = v3f(1,1,1);
	f->lifetime = 600.0;
	f->setCollisionBox(aabb3f(-0.75*BS, 0., -0.75*BS, 0.75*BS, 1.*BS, 0.75*BS));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_WOLF;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Wolf");
	f->level = MOB_AGGRESSIVE;
	f->hp = 40;
	f->model = "wolf.b3d";
	f->model_scale = v3f(1,1,1);
	f->model_rotation = v3f(0,-90,0);
	f->model_offset = v3f(0,0.5,0);
	f->setTexture("mob_wolf.png");
	f->setAnimationFrames(MA_STAND,1,60);
	f->setAnimationFrames(MA_MOVE,61,120);
	f->setAnimationFrames(MA_ATTACK,61,120);
	f->punch_action = MPA_HARM;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_MEAT)+" 2";
	f->tamed_mob = CONTENT_MOB_TAMEWOLF;
	f->motion = MM_SEEKER;
	f->motion_type = MMT_WALK;
	f->spawn_min_height = 0;
	f->spawn_max_height = 40;
	f->sound_punch = "mob-wolf-hit";
	f->sound_spawn = "mob-wolf-spawn";
	f->notices_player = true;
	f->attack_player_damage = 3;
	f->attack_player_range = v3f(1,1,1);
	f->lifetime = 900.0;
	f->setCollisionBox(aabb3f(-0.5*BS, 0., -0.5*BS, 0.5*BS, 1.*BS, 0.5*BS));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_TAMEWOLF;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Tame Wolf");
	f->level = MOB_PASSIVE;
	f->hp = 40;
	f->model = "wolf.b3d";
	f->model_scale = v3f(1,1,1);
	f->model_rotation = v3f(0,-90,0);
	f->model_offset = v3f(0,0.5,0);
	f->setTexture("mob_tamewolf.png");
	f->setAnimationFrames(MA_STAND,1,60);
	f->setAnimationFrames(MA_MOVE,61,120);
	f->setAnimationFrames(MA_ATTACK,61,120);
	f->punch_action = MPA_HARM;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_FUR)+" 2";
	f->motion = MM_SEEKER;
	f->motion_type = MMT_WALK;
	f->sound_punch = "mob-wolf-hit";
	f->notices_player = true;
	f->attack_mob_damage = 5;
	f->attack_mob_range = v3f(1,1,1);
	f->lifetime = 1800.0;
	f->spawn_naturally = false;
	f->setCollisionBox(aabb3f(-0.5*BS, 0., -0.5*BS, 0.5*BS, 1.*BS, 0.5*BS));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_SHEEP;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Sheep");
	f->level = MOB_PASSIVE;
	f->hp = 30;
	f->model = "sheep.b3d";
	f->model_scale = v3f(0.8,0.8,0.8);
	f->model_rotation = v3f(0,-90,0);
	f->model_offset = v3f(0,0.6,0);
	f->setTexture("mob_sheep.png");
	f->setAnimationFrames(MA_STAND,61,120);
	f->setAnimationFrames(MA_MOVE,0,60);
	f->setAnimationFrames(MA_ATTACK,0,60);
	f->punch_action = MPA_HARM;
	f->dropped_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_MEAT)+" 2";
	f->special_punch_item = TT_SHEAR;
	f->special_dropped_item = CONTENT_CRAFTITEM_STRING;
	f->special_dropped_count = 4;
	f->special_dropped_max = 8;
	f->motion = MM_SEEKER;
	f->motion_type = MMT_WALK;
	f->sound_random = "mob-sheep-env";
	f->sound_random_extra = "mob-ducksheep-env";
	f->spawn_min_height = 2;
	f->spawn_max_height = 50;
	f->spawn_group = 4;
	f->lifetime = 1800.0;
	f->setCollisionBox(aabb3f(-0.4*BS, 0., -0.4*BS, 0.4*BS, 1.*BS, 0.4*BS));
	lists::add("creative",CONTENT_TOOLITEM_MOB_SPAWNER,1,i);

	i = CONTENT_MOB_SNOWBALL;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Snowball");
	f->level = MOB_AGGRESSIVE;
	f->setTexture("snow_ball.png");
	f->model_offset = v3f(0,0.2,0);
	f->punch_action = MPA_IGNORE;
	f->motion = MM_THROWN;
	f->motion_type = MMT_FLY;
	f->moves_silently = true;
	f->notices_player = true;
	f->attack_mob_damage = 5;
	f->attack_mob_range = v3f(1,1,1);
	f->lifetime = 10.0;
	f->contact_place_node = CONTENT_SNOW;
	f->contact_drop_item = CONTENT_CRAFTITEM_SNOW_BALL;
	f->spawn_naturally = false;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.));

	i = CONTENT_MOB_ARROW;
	f = &g_content_mob_features[i&~CONTENT_MOB_MASK];
	f->content = i;
	f->description = wgettext("Arrow");
	f->level = MOB_AGGRESSIVE;
	f->setTexture("mob_arrow.png");
	f->texture_display = MDT_EXTRUDED;
	f->model_offset = v3f(0,0.2,0);
	f->punch_action = MPA_IGNORE;
	f->motion = MM_THROWN;
	f->motion_type = MMT_FLY;
	f->moves_silently = true;
	f->notices_player = true;
	f->attack_mob_damage = 20;
	f->attack_mob_range = v3f(1,1,1);
	f->lifetime = 20.0;
	f->contact_drop_item = CONTENT_CRAFTITEM_ARROW;
	f->spawn_naturally = false;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.));
}
