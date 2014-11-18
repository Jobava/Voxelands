/************************************************************************
* Minetest-c55
* Copyright (C) 2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* auth.cpp
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

#include "auth.h"
#include <fstream>
#include <jmutexautolock.h>
//#include "main.h" // for g_settings
#include <sstream>
#include "strfnd.h"
#include "debug.h"

// Convert a privileges value into a human-readable string,
// with each component separated by a comma.
std::string privsToString(uint64_t privs)
{
	std::ostringstream os(std::ios_base::binary);
	if(privs & PRIV_BUILD)
		os<<"build,";
	if(privs & PRIV_TELEPORT)
		os<<"teleport,";
	if(privs & PRIV_SETTIME)
		os<<"settime,";
	if(privs & PRIV_PRIVS)
		os<<"privs,";
	if(privs & PRIV_SHOUT)
		os<<"shout,";
	if(privs & PRIV_BAN)
		os<<"ban,";
	if(os.tellp())
	{
		// Drop the trailing comma. (Why on earth can't
		// you truncate a C++ stream anyway???)
		std::string tmp = os.str();
		return tmp.substr(0, tmp.length() -1);
	}
	return os.str();
}

// Converts a comma-seperated list of privilege values into a
// privileges value. The reverse of privsToString(). Returns
// PRIV_INVALID if there is anything wrong with the input.
uint64_t stringToPrivs(std::string str)
{
	uint64_t privs=0;
	Strfnd f(str);
	while(f.atend() == false)
	{
		std::string s = trim(f.next(","));
		if(s == "build")
			privs |= PRIV_BUILD;
		else if(s == "teleport")
			privs |= PRIV_TELEPORT;
		else if(s == "settime")
			privs |= PRIV_SETTIME;
		else if(s == "privs")
			privs |= PRIV_PRIVS;
		else if(s == "shout")
			privs |= PRIV_SHOUT;
		else if(s == "ban")
			privs |= PRIV_BAN;
		else
			return PRIV_INVALID;
	}
	return privs;
}

AuthManager::AuthManager(const std::string &authfilepath):
		m_authfilepath(authfilepath),
		m_modified(false)
{
	m_mutex.Init();

	try{
		load();
	}
	catch(SerializationError &e)
	{
		dstream<<"WARNING: AuthManager: creating "
				<<m_authfilepath<<std::endl;
	}
}

AuthManager::~AuthManager()
{
	save();
}

void AuthManager::load()
{
	JMutexAutoLock lock(m_mutex);

	dstream<<"AuthManager: loading from "<<m_authfilepath<<std::endl;
	std::ifstream is(m_authfilepath.c_str(), std::ios::binary);
	if(is.good() == false)
	{
		dstream<<"AuthManager: failed loading from "<<m_authfilepath<<std::endl;
		throw SerializationError("AuthManager::load(): Couldn't open file");
	}

	for(;;)
	{
		if(is.eof() || is.good() == false)
			break;

		// Read a line
		std::string line;
		std::getline(is, line, '\n');

		std::istringstream iss(line);

		// Read name
		std::string name;
		std::getline(iss, name, ':');

		// Read password
		std::string pwd;
		std::getline(iss, pwd, ':');

		// Read privileges
		std::string stringprivs;
		std::getline(iss, stringprivs, ':');
		uint64_t privs = stringToPrivs(stringprivs);

		// Store it
		AuthData ad;
		ad.pwd = pwd;
		ad.privs = privs;
		m_authdata[name] = ad;
	}

	m_modified = false;
}

void AuthManager::save()
{
	JMutexAutoLock lock(m_mutex);

	dstream<<"AuthManager: saving to "<<m_authfilepath<<std::endl;
	std::ofstream os(m_authfilepath.c_str(), std::ios::binary);
	if(os.good() == false)
	{
		dstream<<"AuthManager: failed saving to "<<m_authfilepath<<std::endl;
		throw SerializationError("AuthManager::save(): Couldn't open file");
	}

	for(core::map<std::string, AuthData>::Iterator
			i = m_authdata.getIterator();
			i.atEnd()==false; i++)
	{
		std::string name = i.getNode()->getKey();
		if(name == "")
			continue;
		AuthData ad = i.getNode()->getValue();
		os<<name<<":"<<ad.pwd<<":"<<privsToString(ad.privs)<<"\n";
	}

	m_modified = false;
}

bool AuthManager::exists(const std::string &username)
{
	JMutexAutoLock lock(m_mutex);

	core::map<std::string, AuthData>::Node *n;
	n = m_authdata.find(username);
	if(n == NULL)
		return false;
	return true;
}

void AuthManager::set(const std::string &username, AuthData ad)
{
	JMutexAutoLock lock(m_mutex);

	m_authdata[username] = ad;

	m_modified = true;
}

void AuthManager::add(const std::string &username)
{
	JMutexAutoLock lock(m_mutex);

	m_authdata[username] = AuthData();

	m_modified = true;
}

std::string AuthManager::getPassword(const std::string &username)
{
	JMutexAutoLock lock(m_mutex);

	core::map<std::string, AuthData>::Node *n;
	n = m_authdata.find(username);
	if(n == NULL)
		throw AuthNotFoundException("");

	return n->getValue().pwd;
}

void AuthManager::setPassword(const std::string &username,
		const std::string &password)
{
	JMutexAutoLock lock(m_mutex);

	core::map<std::string, AuthData>::Node *n;
	n = m_authdata.find(username);
	if(n == NULL)
		throw AuthNotFoundException("");

	AuthData ad = n->getValue();
	ad.pwd = password;
	n->setValue(ad);

	m_modified = true;
}

uint64_t AuthManager::getPrivs(const std::string &username)
{
	JMutexAutoLock lock(m_mutex);

	core::map<std::string, AuthData>::Node *n;
	n = m_authdata.find(username);
	if(n == NULL)
		throw AuthNotFoundException("");

	return n->getValue().privs;
}

void AuthManager::setPrivs(const std::string &username, uint64_t privs)
{
	JMutexAutoLock lock(m_mutex);

	core::map<std::string, AuthData>::Node *n;
	n = m_authdata.find(username);
	if(n == NULL)
		throw AuthNotFoundException("");

	AuthData ad = n->getValue();
	ad.privs = privs;
	n->setValue(ad);

	m_modified = true;
}

bool AuthManager::isModified()
{
	JMutexAutoLock lock(m_mutex);
	return m_modified;
}


