/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* serverobject.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2013-2014 <lisa@ltmnet.com>
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

#include "serverobject.h"
#include "content_object.h"
#include "content_sao.h"
#include <fstream>
#include "inventory.h"

ServerActiveObject::ServerActiveObject(ServerEnvironment *env, u16 id, v3f pos):
	ActiveObject(id),
	m_known_by_count(0),
	m_removed(false),
	m_pending_deactivation(false),
	m_static_exists(false),
	m_static_block(1337,1337,1337),
	m_env(env),
	m_base_position(pos)
{
}

ServerActiveObject::~ServerActiveObject()
{
}

ServerActiveObject* ServerActiveObject::create(u8 type,
		ServerEnvironment *env, u16 id, v3f pos,
		const std::string &data)
{
	// Find factory function
	core::map<u16, Factory>::Node *n;
	n = m_types.find(type);
	if (n == NULL) {
		// If factory is not found, just return.
		dstream<<"WARNING: ServerActiveObject: No factory for type="
				<<type<<std::endl;
		return NULL;
	}

	Factory f = n->getValue();
	ServerActiveObject *object = (*f)(env, id, pos, data);
	return object;
}

void ServerActiveObject::registerType(u16 type, Factory f)
{
	core::map<u16, Factory>::Node *n;
	n = m_types.find(type);
	if(n)
		return;
	m_types.insert(type, f);
}



