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

#include "content_mob.h"
#include "main.h"

std::map<content_t,struct MobFeatures> g_content_mob_features;

MobFeatures & content_mob_features(content_t i)
{
	if ((i&CONTENT_MOB_MASK) != CONTENT_MOB_MASK)
		return g_content_mob_features[CONTENT_IGNORE];

	std::map<content_t,struct MobFeatures>::iterator it = g_content_mob_features.find(i);
	if (it == g_content_mob_features.end())
		return g_content_mob_features[CONTENT_IGNORE];
	return it->second;
}

#ifndef SERVER
void MobFeatures::setTexture(std::string name)
{
	if (g_texturesource)
		texture = g_texturesource->getTextureRaw(name);
}
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

void content_mob_init()
{
	g_content_mob_features.clear();

	content_t i;
	MobFeatures *f = NULL;

	i = CONTENT_MOB_RAT;
	f = &g_content_mob_features[i];
	f->content = i;
	f->level = MOB_PASSIVE;
	f->model = "rat.x";
	f->setTexture("mob_rat.png");
	f->punch_action = MPA_PICKUP;
	f->motion = MM_WANDER;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.));

	i = CONTENT_MOB_FIREFLY;
	f = &g_content_mob_features[i];
	f->content = i;
	f->level = MOB_PASSIVE;
	f->setTexture("mob_firefly.png");
	f->punch_action = MPA_PICKUP;
	f->motion = MM_WANDER;
	f->glow_light = LIGHT_MAX-1;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.));

	i = CONTENT_MOB_OERKKI;
	f = &g_content_mob_features[i];
	f->content = i;
	f->level = MOB_AGGRESSIVE;
	f->model = "oerkki.x";
	f->setTexture("mob_oerkki.png");
	f->punch_action = MPA_HARM;
	f->motion = MM_SEEKER;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS*2.,BS/3.));

	i = CONTENT_MOB_DUNGEON_MASTER;
	f = &g_content_mob_features[i];
	f->content = i;
	f->level = MOB_DESTRUCTIVE;
	f->model = "dungeon_master.x";
	f->setTexture("mob_dungeon_master.png");
	f->punch_action = MPA_HARM;
	f->motion = MM_SENTRY;
	f->attack_throw_object = CONTENT_MOB_FIREBALL;
	f->attack_glow_light = LIGHT_MAX-1;
	f->setCollisionBox(aabb3f(-0.4*BS,-0.4*BS,-0.4*BS, 0.4*BS,0.8*BS,0.4*BS));

	i = CONTENT_MOB_FIREBALL;
	f = &g_content_mob_features[i];
	f->content = i;
	f->level = MOB_DESTRUCTIVE;
	f->setTexture("mob_fireball.png");
	f->punch_action = MPA_IGNORE;
	f->motion = MM_CONSTANT;
	f->motion_type = MMT_FLY;
	f->glow_light = LIGHT_MAX-1;
	f->setCollisionBox(aabb3f(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.));

	i = CONTENT_MOB_DEER;
	f = &g_content_mob_features[i];
	f->content = i;
	i = CONTENT_MOB_SHEEP;
	f = &g_content_mob_features[i];
	f->content = i;
	i = CONTENT_MOB_FISH;
	f = &g_content_mob_features[i];
	f->content = i;
	i = CONTENT_MOB_SHARK;
	f = &g_content_mob_features[i];
	f->content = i;
	i = CONTENT_MOB_WOLF;
	f = &g_content_mob_features[i];
	f->content = i;

}
