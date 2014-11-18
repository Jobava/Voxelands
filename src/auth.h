/************************************************************************
* Minetest-c55
* Copyright (C) 2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* auth.h
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

#ifndef AUTH_HEADER
#define AUTH_HEADER

#include <string>
#include <jthread.h>
#include <jmutex.h>
#include "common_irrlicht.h"
#include "exceptions.h"

using namespace jthread;

// Player privileges. These form a bitmask stored in the privs field
// of the player, and define things they're allowed to do. See also
// the static methods Player::privsToString and stringToPrivs that
// convert these to human-readable form.
const uint64_t PRIV_BUILD = 1;            // Can build - i.e. modify the world
const uint64_t PRIV_TELEPORT = 2;         // Can teleport
const uint64_t PRIV_SETTIME = 4;          // Can set the time
const uint64_t PRIV_PRIVS = 8;            // Can grant and revoke privileges
const uint64_t PRIV_SERVER = 16;          // Can manage the server (e.g. shutodwn
                                     // ,settings)
const uint64_t PRIV_SHOUT = 32;           // Can broadcast chat messages to all
                                     // players
const uint64_t PRIV_BAN = 64;             // Can ban players

// Default privileges - these can be overriden for new players using the
// config option "default_privs" - however, this value still applies for
// players that existed before the privileges system was added.
const uint64_t PRIV_DEFAULT = PRIV_BUILD|PRIV_SHOUT;
const uint64_t PRIV_ALL = 0x7FFFFFFFFFFFFFFFULL;
const uint64_t PRIV_INVALID = 0x8000000000000000ULL;

// Convert a privileges value into a human-readable string,
// with each component separated by a comma.
std::string privsToString(uint64_t privs);

// Converts a comma-seperated list of privilege values into a
// privileges value. The reverse of privsToString(). Returns
// PRIV_INVALID if there is anything wrong with the input.
uint64_t stringToPrivs(std::string str);

struct AuthData
{
	std::string pwd;
	uint64_t privs;

	AuthData():
		privs(PRIV_DEFAULT)
	{
	}
};

class AuthNotFoundException : public BaseException
{
public:
	AuthNotFoundException(const char *s):
		BaseException(s)
	{}
};

class AuthManager
{
public:
	AuthManager(const std::string &authfilepath);
	~AuthManager();
	void load();
	void save();
	bool exists(const std::string &username);
	void set(const std::string &username, AuthData ad);
	void add(const std::string &username);
	std::string getPassword(const std::string &username);
	void setPassword(const std::string &username,
			const std::string &password);
	uint64_t getPrivs(const std::string &username);
	void setPrivs(const std::string &username, uint64_t privs);
	bool isModified();
private:
	JMutex m_mutex;
	std::string m_authfilepath;
	core::map<std::string, AuthData> m_authdata;
	bool m_modified;
};

#endif

