/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* staticobject.h
* voxelands - 3d voxel world sandbox game
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
*
* License updated from GPLv2 or later to GPLv3 or later by Lisa Milne
* for Voxelands.
************************************************************************/

#ifndef STATICOBJECT_HEADER
#define STATICOBJECT_HEADER

#include "common_irrlicht.h"
#include <string>
#include <sstream>
#include "utility.h"
#include "settings.h"
#include "main.h"
#include <list>

struct StaticObject
{
	u8 type;
	v3f pos;
	std::string data;

	StaticObject():
		type(0),
		pos(0,0,0)
	{
	}
	StaticObject(u8 type_, v3f pos_, const std::string &data_):
		type(type_),
		pos(pos_),
		data(data_)
	{
	}

	void serialize(std::ostream &os)
	{
		char buf[12];
		// type
		buf[0] = type;
		os.write(buf, 1);
		// pos
		writeV3S32((u8*)buf, v3s32(pos.X*1000,pos.Y*1000,pos.Z*1000));
		os.write(buf, 12);
		// data
		os<<serializeString(data);
	}
	void deSerialize(std::istream &is, u8 version)
	{
		char buf[12];
		// type
		is.read(buf, 1);
		type = buf[0];
		// pos
		is.read(buf, 12);
		v3s32 intp = readV3S32((u8*)buf);
		pos.X = (f32)intp.X/1000;
		pos.Y = (f32)intp.Y/1000;
		pos.Z = (f32)intp.Z/1000;
		// data
		data = deSerializeString(is);
	}
};

class StaticObjectList
{
public:
	void serialize(std::ostream &os)
	{
		char buf[12];
		// version
		buf[0] = 0;
		os.write(buf, 1);
		// count
		u16 count = m_objects.size();
		writeU16((u8*)buf, count);
		os.write(buf, 2);
		for (std::list<StaticObject>::iterator i = m_objects.begin(); i != m_objects.end(); i++) {
			StaticObject &s_obj = *i;
			s_obj.serialize(os);
		}
	}
	void deSerialize(std::istream &is)
	{
		bool drop = g_settings->getBool("onload_ignore_objects");
		char buf[12];
		// version
		is.read(buf, 1);
		u8 version = buf[0];
		// count
		is.read(buf, 2);
		u16 count = readU16((u8*)buf);
		for (u16 i=0; i<count; i++) {
			StaticObject s_obj;
			s_obj.deSerialize(is, version);
			if (!drop)
				m_objects.push_back(s_obj);
		}
	}

	std::list<StaticObject> m_objects;

private:
};

#endif

