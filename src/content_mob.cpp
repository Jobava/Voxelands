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

}
