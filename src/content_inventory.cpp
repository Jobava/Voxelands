/*
Minetest-c55
Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "content_inventory.h"
#include "inventory.h"
#include "content_mapnode.h"
//#include "serverobject.h"
#include "content_sao.h"

ServerActiveObject* item_craft_create_object(const std::string &subname,
		ServerEnvironment *env, u16 id, v3f pos)
{
	if(subname == "rat")
	{
		ServerActiveObject *obj = new RatSAO(env, id, pos);
		return obj;
	}
	else if(subname == "firefly")
	{
		ServerActiveObject *obj = new FireflySAO(env, id, pos);
		return obj;
	}

	return NULL;
}


