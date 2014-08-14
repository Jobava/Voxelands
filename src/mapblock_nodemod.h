/*
Minetest-c55
Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>

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

#ifndef MAPBLOCK_NODEMOD_HEADER
#define MAPBLOCK_NODEMOD_HEADER

#include <map>

enum NodeModType
{
	NODEMOD_NONE = 0x0,
	NODEMOD_CHANGECONTENT = 0x01, //param is content id
	NODEMOD_CRACK = 0x02, // param is crack progression
	NODEMOD_SELECTION = 0x04 // param is ignored
};

struct NodeMod
{
	NodeMod(enum NodeModType a_type=NODEMOD_NONE, u16 a_param=0)
	{
		type = a_type;
		param = a_param;
	}
	bool operator==(enum NodeModType c_type)
	{
		if (type)
printf("%X\n",type);
		return ((type&c_type)==c_type);
	}
	bool operator==(const NodeMod &other)
	{
		return (type == other.type && param == other.param);
	}
	void operator|=(const NodeMod &other)
	{
		type |= other.type;
		if (other.type != NODEMOD_SELECTION)
			param = other.param;
	}
	u16 type;
	u16 param;
};

class NodeModMap
{
public:
	/*
		returns true if the mod was different last time
	*/
	bool set(v3s16 p, const NodeMod &mod)
	{
		// See if old is different, cancel if it is not different.
		std::map<v3s16, NodeMod>::iterator n = m_mods.find(p);
		if (n != m_mods.end()) {
			NodeMod old = n->second;
			if(old == mod)
				return false;

			old |= mod;
			m_mods[p] = old;
		}else{
			m_mods[p] = mod;
		}

		return true;
	}
	// Returns true if there was one
	bool get(v3s16 p, NodeMod *mod)
	{
		std::map<v3s16, NodeMod>::iterator n = m_mods.find(p);
		if (n == m_mods.end())
			return false;
		if(mod)
			*mod = n->second;
		return true;
	}
	bool clear(v3s16 p)
	{
		if (m_mods.find(p) != m_mods.end()) {
			m_mods.erase(p);
			return true;
		}
		return false;
	}
	bool clear()
	{
		if(m_mods.size() == 0)
			return false;
		m_mods.clear();
		return true;
	}
	void copy(NodeModMap &dest)
	{
		dest.m_mods.clear();

		for(std::map<v3s16, NodeMod>::iterator i = m_mods.begin();
				i != m_mods.end(); i++)
		{
			dest.m_mods[i->first] = i->second;
		}
	}

private:
	std::map<v3s16, NodeMod> m_mods;
};

#endif

