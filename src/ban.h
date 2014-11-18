/************************************************************************
* Minetest-c55
* Copyright (C) 2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* ban.h
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

#ifndef BAN_HEADER
#define BAN_HEADER

#include <map>
#include <string>
#include <jthread.h>
#include <jmutex.h>
#include "common_irrlicht.h"
#include "exceptions.h"

using namespace jthread;

class BanManager
{
public:
	BanManager(const std::string &bannfilepath);
	~BanManager();
	void load();
	void save();
	bool isIpBanned(const std::string &ip);
	// Supplying ip_or_name = "" lists all bans.
	std::string getBanDescription(const std::string &ip_or_name);
	std::string getBanName(const std::string &ip);
	void add(const std::string &ip, const std::string &name);
	void remove(const std::string &ip_or_name);
	bool isModified();
private:
	JMutex m_mutex;
	std::string m_banfilepath;
	std::map<std::string, std::string> m_ips;
	bool m_modified;

};

#endif
