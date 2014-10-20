/************************************************************************
* content_toolitem.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa Milne 2014 <lisa@ltmnet.com>
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

#include "content_clothesitem.h"
#include "content_craftitem.h"
#include "content_craft.h"
#include "content_list.h"
#include "content_mapnode.h"
#include <map>
#include "gettext.h"

std::map<content_t,struct ClothesItemFeatures> g_content_clothesitem_features;

ClothesItemFeatures & content_clothesitem_features(content_t i)
{
	if ((i&CONTENT_CLOTHESITEM_MASK) != CONTENT_CLOTHESITEM_MASK)
		return g_content_clothesitem_features[CONTENT_IGNORE];

	std::map<content_t,struct ClothesItemFeatures>::iterator it = g_content_clothesitem_features.find(i);
	if (it == g_content_clothesitem_features.end())
		return g_content_clothesitem_features[CONTENT_IGNORE];
	return it->second;
}

void content_clothesitem_init()
{
	g_content_clothesitem_features.clear();

	content_t i;
	ClothesItemFeatures *f = NULL;

	i = CONTENT_CLOTHESITEM_FUR_PANTS;
	f = &g_content_clothesitem_features[i];
	f->content = i;
	f->texture = "clothes_furpants.png";
	f->overlay_texture = "clothes_player_furpants.png";
	f->description = wgettext("Fur Pants");
	f->type = CT_PANTS;
	f->armour = 0.10;
	f->warmth = 0.30;
	f->vacuum = 0.05;
	{
		content_t r[9] = {
			CONTENT_CRAFTITEM_FUR,	CONTENT_CRAFTITEM_FUR,	CONTENT_CRAFTITEM_FUR,
			CONTENT_CRAFTITEM_FUR,	CONTENT_IGNORE,		CONTENT_CRAFTITEM_FUR,
			CONTENT_CRAFTITEM_FUR,	CONTENT_IGNORE,		CONTENT_CRAFTITEM_FUR
		};
		crafting::setRecipe(r,i,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CLOTHESITEM_FUR_SHIRT;
	f = &g_content_clothesitem_features[i];
	f->content = i;
	f->texture = "clothes_furshirt.png";
	f->overlay_texture = "clothes_player_furshirt.png";
	f->description = wgettext("Fur Shirt");
	f->type = CT_SHIRT;
	f->armour = 0.10;
	f->warmth = 0.30;
	f->vacuum = 0.05;
	{
		content_t r[9] = {
			CONTENT_CRAFTITEM_FUR,	CONTENT_CRAFTITEM_FUR,	CONTENT_CRAFTITEM_FUR,
			CONTENT_IGNORE,		CONTENT_CRAFTITEM_FUR,	CONTENT_IGNORE,
			CONTENT_IGNORE,		CONTENT_CRAFTITEM_FUR,	CONTENT_IGNORE
		};
		crafting::setRecipe(r,i,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CLOTHESITEM_FUR_HAT;
	f = &g_content_clothesitem_features[i];
	f->content = i;
	f->texture = "clothes_furhat.png";
	f->overlay_texture = "clothes_player_furhat.png";
	f->description = wgettext("Fur Hat");
	f->type = CT_HAT;
	f->armour = 0.05;
	f->warmth = 0.20;
	f->vacuum = 0.05;
	{
		content_t r[9] = {
			CONTENT_CRAFTITEM_FUR,	CONTENT_CRAFTITEM_FUR,	CONTENT_CRAFTITEM_FUR,
			CONTENT_CRAFTITEM_FUR,	CONTENT_IGNORE,		CONTENT_CRAFTITEM_FUR,
			CONTENT_IGNORE,		CONTENT_IGNORE,		CONTENT_IGNORE
		};
		crafting::setRecipe(r,i,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CLOTHESITEM_FUR_BOOTS;
	f = &g_content_clothesitem_features[i];
	f->content = i;
	f->texture = "clothes_furboots.png";
	f->overlay_texture = "clothes_player_furboots.png";
	f->description = wgettext("Fur Boots");
	f->type = CT_BOOTS;
	f->armour = 0.05;
	f->warmth = 0.20;
	f->vacuum = 0.05;
	{
		content_t r[9] = {
			CONTENT_CRAFTITEM_FUR,	CONTENT_IGNORE,		CONTENT_CRAFTITEM_FUR,
			CONTENT_CRAFTITEM_FUR,	CONTENT_IGNORE,		CONTENT_CRAFTITEM_FUR,
			CONTENT_IGNORE,		CONTENT_IGNORE,		CONTENT_IGNORE
		};
		crafting::setRecipe(r,i,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);
}
