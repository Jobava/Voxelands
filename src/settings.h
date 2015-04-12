/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* settings.h
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

#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

#include "common_irrlicht.h"
#include <string>
#include <jthread.h>
#include <jmutex.h>
#include <jmutexautolock.h>
#include "strfnd.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "debug.h"
#include "utility.h"
#include "log.h"

enum ValueType
{
	VALUETYPE_STRING,
	VALUETYPE_FLAG // Doesn't take any arguments
};

struct ValueSpec
{
	ValueSpec(ValueType a_type, const char *a_help=NULL)
	{
		type = a_type;
		help = a_help;
	}
	ValueType type;
	const char *help;
};

using namespace jthread;

class Settings
{
public:
	Settings()
	{
		m_mutex.Init();
	}

	void writeLines(std::ostream &os)
	{
		JMutexAutoLock lock(m_mutex);

		for(core::map<std::string, std::string>::Iterator
				i = m_settings.getIterator();
				i.atEnd() == false; i++)
		{
			std::string name = i.getNode()->getKey();
			std::string value = i.getNode()->getValue();
			os<<name<<" = "<<value<<"\n";
		}
	}

	bool parseConfigLine(const std::string &line)
	{
		JMutexAutoLock lock(m_mutex);

		std::string trimmedline = trim(line);

		// Ignore comments
		if(trimmedline[0] == '#')
			return true;

		//infostream<<"trimmedline=\""<<trimmedline<<"\""<<std::endl;

		Strfnd sf(trim(line));

		std::string name = sf.next("=");
		name = trim(name);

		if(name == "")
			return true;

		std::string value = sf.next("\n");
		value = trim(value);

		/*infostream<<"Config name=\""<<name<<"\" value=\""
				<<value<<"\""<<std::endl;*/

		m_settings[name] = value;

		return true;
	}

	void parseConfigLines(std::istream &is, const std::string &endstring)
	{
		for(;;){
			if(is.eof())
				break;
			std::string line;
			std::getline(is, line);
			std::string trimmedline = trim(line);
			if(endstring != ""){
				if(trimmedline == endstring)
					break;
			}
			parseConfigLine(line);
		}
	}

	// Returns false on EOF
	bool parseConfigObject(std::istream &is)
	{
		if(is.eof())
			return false;

		/*
			NOTE: This function might be expanded to allow multi-line
			      settings.
		*/
		std::string line;
		std::getline(is, line);
		//infostream<<"got line: \""<<line<<"\""<<std::endl;

		return parseConfigLine(line);
	}

	/*
		Read configuration file

		Returns true on success
	*/
	bool readConfigFile(const char *filename)
	{
		std::ifstream is(filename);
		if(is.good() == false)
		{
			errorstream<<"Error opening configuration file \""
					<<filename<<"\""<<std::endl;
			return false;
		}

		infostream<<"Parsing configuration file: \""
				<<filename<<"\""<<std::endl;

		while(parseConfigObject(is));

		return true;
	}

	/*
		Reads a configuration object from stream (usually a single line)
		and adds it to dst.

		Preserves comments and empty lines.

		Settings that were added to dst are also added to updated.
		key of updated is setting name, value of updated is dummy.

		Returns false on EOF
	*/
	bool getUpdatedConfigObject(std::istream &is,
			core::list<std::string> &dst,
			core::map<std::string, bool> &updated)
	{
		JMutexAutoLock lock(m_mutex);

		if(is.eof())
			return false;

		// NOTE: This function will be expanded to allow multi-line settings
		std::string line;
		std::getline(is, line);

		std::string trimmedline = trim(line);

		std::string line_end = "";
		if(is.eof() == false)
			line_end = "\n";

		// Ignore comments
		if(trimmedline[0] == '#')
		{
			dst.push_back(line+line_end);
			return true;
		}

		Strfnd sf(trim(line));

		std::string name = sf.next("=");
		name = trim(name);

		if(name == "")
		{
			dst.push_back(line+line_end);
			return true;
		}

		std::string value = sf.next("\n");
		value = trim(value);

		if(m_settings.find(name))
		{
			std::string newvalue = m_settings[name];

			if(newvalue != value)
			{
				infostream<<"Changing value of \""<<name<<"\" = \""
						<<value<<"\" -> \""<<newvalue<<"\""
						<<std::endl;
			}

			dst.push_back(name + " = " + newvalue + line_end);

			updated[name] = true;
		}

		return true;
	}

	/*
		Updates configuration file

		Returns true on success
	*/
	bool updateConfigFile(const char *filename)
	{
		infostream<<"Updating configuration file: \""
				<<filename<<"\""<<std::endl;

		core::list<std::string> objects;
		core::map<std::string, bool> updated;

		// Read and modify stuff
		{
			std::ifstream is(filename);
			if(is.good() == false)
			{
				infostream<<"updateConfigFile():"
						" Error opening configuration file"
						" for reading: \""
						<<filename<<"\""<<std::endl;
			}
			else
			{
				while(getUpdatedConfigObject(is, objects, updated));
			}
		}

		JMutexAutoLock lock(m_mutex);

		// Write stuff back
		{
			std::ofstream os(filename);
			if(os.good() == false)
			{
				errorstream<<"Error opening configuration file"
						" for writing: \""
						<<filename<<"\""<<std::endl;
				return false;
			}

			/*
				Write updated stuff
			*/
			for(core::list<std::string>::Iterator
					i = objects.begin();
					i != objects.end(); i++)
			{
				os<<(*i);
			}

			/*
				Write stuff that was not already in the file
			*/
			for(core::map<std::string, std::string>::Iterator
					i = m_settings.getIterator();
					i.atEnd() == false; i++)
			{
				if(updated.find(i.getNode()->getKey()))
					continue;
				std::string name = i.getNode()->getKey();
				std::string value = i.getNode()->getValue();
				infostream<<"Adding \""<<name<<"\" = \""<<value<<"\""
						<<std::endl;
				os<<name<<" = "<<value<<"\n";
			}
		}

		return true;
	}

	/*
		NOTE: Types of allowed_options are ignored

		returns true on success
	*/
	bool parseCommandLine(int argc, char *argv[],
			core::map<std::string, ValueSpec> &allowed_options)
	{
		int i=1;
		for(;;)
		{
			if(i >= argc)
				break;
			std::string argname = argv[i];
			if(argname.substr(0, 2) != "--")
			{
				errorstream<<"Invalid command-line parameter \""
						<<argname<<"\": --<option> expected."<<std::endl;
				return false;
			}
			i++;

			std::string name = argname.substr(2);

			core::map<std::string, ValueSpec>::Node *n;
			n = allowed_options.find(name);
			if(n == NULL)
			{
				errorstream<<"Unknown command-line parameter \""
						<<argname<<"\""<<std::endl;
				return false;
			}

			ValueType type = n->getValue().type;

			std::string value = "";

			if(type == VALUETYPE_FLAG)
			{
				value = "true";
			}
			else
			{
				if(i >= argc)
				{
					errorstream<<"Invalid command-line parameter \""
							<<name<<"\": missing value"<<std::endl;
					return false;
				}
				value = argv[i];
				i++;
			}


			infostream<<"Valid command-line parameter: \""
					<<name<<"\" = \""<<value<<"\""
					<<std::endl;
			set(name, value);
		}

		return true;
	}

	virtual void set(std::string name, std::string value)
	{
		JMutexAutoLock lock(m_mutex);

		m_settings[name] = value;
	}

	virtual void set(std::string name, const char *value)
	{
		JMutexAutoLock lock(m_mutex);

		m_settings[name] = value;
	}


	void setDefault(std::string name, std::string value)
	{
		JMutexAutoLock lock(m_mutex);

		m_defaults[name] = value;
	}

	bool exists(std::string name)
	{
		JMutexAutoLock lock(m_mutex);

		return (m_settings.find(name) || m_defaults.find(name));
	}

	std::string get(std::string name)
	{
		JMutexAutoLock lock(m_mutex);

		core::map<std::string, std::string>::Node *n;
		n = m_settings.find(name);
		if(n == NULL)
		{
			n = m_defaults.find(name);
			if(n == NULL)
			{
				printf("Not Found: %s\n",name.c_str());
				infostream<<"Settings: Setting not found: \""
						<<name<<"\""<<std::endl;
				throw SettingNotFoundException("Setting not found");
			}
		}

		return n->getValue();
	}

	bool getBool(std::string name)
	{
		return is_yes(get(name));
	}

	bool getFlag(std::string name)
	{
		try
		{
			return getBool(name);
		}
		catch(SettingNotFoundException &e)
		{
			return false;
		}
	}

	// Asks if empty
	bool getBoolAsk(std::string name, std::string question, bool def)
	{
		// If it is in settings
		if(exists(name))
			return getBool(name);

		std::string s;
		char templine[10];
		std::cout<<question<<" [y/N]: ";
		std::cin.getline(templine, 10);
		s = templine;

		if(s == "")
			return def;

		return is_yes(s);
	}

	float getFloat(std::string name)
	{
		return mystof(get(name));
	}

	u16 getU16(std::string name)
	{
		return mystoi(get(name), 0, 65535);
	}

	u16 getU16Ask(std::string name, std::string question, u16 def)
	{
		// If it is in settings
		if(exists(name))
			return getU16(name);

		std::string s;
		char templine[10];
		std::cout<<question<<" ["<<def<<"]: ";
		std::cin.getline(templine, 10);
		s = templine;

		if(s == "")
			return def;

		return mystoi(s, 0, 65535);
	}

	s16 getS16(std::string name)
	{
		return mystoi(get(name), -32768, 32767);
	}

	s32 getS32(std::string name)
	{
		return mystoi(get(name));
	}

	v3f getV3F(std::string name)
	{
		v3f value;
		Strfnd f(get(name));
		f.next("(");
		value.X = mystof(f.next(","));
		value.Y = mystof(f.next(","));
		value.Z = mystof(f.next(")"));
		return value;
	}

	v2f getV2F(std::string name)
	{
		v2f value;
		Strfnd f(get(name));
		f.next("(");
		value.X = mystof(f.next(","));
		value.Y = mystof(f.next(")"));
		return value;
	}

	uint64_t getU64(std::string name)
	{
		uint64_t value = 0;
		std::string s = get(name);
		std::istringstream ss(s);
		ss>>value;
		return value;
	}

	void setBool(std::string name, bool value)
	{
		if(value)
			set(name, "true");
		else
			set(name, "false");
	}

	void setS32(std::string name, s32 value)
	{
		set(name, itos(value));
	}

	void setFloat(std::string name, float value)
	{
		set(name, ftos(value));
	}

	void setV3F(std::string name, v3f value)
	{
		std::ostringstream os;
		os<<"("<<value.X<<","<<value.Y<<","<<value.Z<<")";
		set(name, os.str());
	}

	void setV2F(std::string name, v2f value)
	{
		std::ostringstream os;
		os<<"("<<value.X<<","<<value.Y<<")";
		set(name, os.str());
	}

	void setU64(std::string name, uint64_t value)
	{
		std::ostringstream os;
		os<<value;
		set(name, os.str());
	}

	void clear()
	{
		JMutexAutoLock lock(m_mutex);

		m_settings.clear();
		m_defaults.clear();
	}

	void updateValue(Settings &other, const std::string &name)
	{
		JMutexAutoLock lock(m_mutex);

		if(&other == this)
			return;

		try{
			std::string val = other.get(name);
			m_settings[name] = val;
		} catch(SettingNotFoundException &e){
		}

		return;
	}

	void update(Settings &other)
	{
		JMutexAutoLock lock(m_mutex);
		JMutexAutoLock lock2(other.m_mutex);

		if(&other == this)
			return;

		for(core::map<std::string, std::string>::Iterator
				i = other.m_settings.getIterator();
				i.atEnd() == false; i++)
		{
			m_settings[i.getNode()->getKey()] = i.getNode()->getValue();
		}

		for(core::map<std::string, std::string>::Iterator
				i = other.m_defaults.getIterator();
				i.atEnd() == false; i++)
		{
			m_defaults[i.getNode()->getKey()] = i.getNode()->getValue();
		}

		return;
	}

	Settings & operator+=(Settings &other)
	{
		JMutexAutoLock lock(m_mutex);
		JMutexAutoLock lock2(other.m_mutex);

		if(&other == this)
			return *this;

		for(core::map<std::string, std::string>::Iterator
				i = other.m_settings.getIterator();
				i.atEnd() == false; i++)
		{
			m_settings.insert(i.getNode()->getKey(),
					i.getNode()->getValue());
		}

		for(core::map<std::string, std::string>::Iterator
				i = other.m_defaults.getIterator();
				i.atEnd() == false; i++)
		{
			m_defaults.insert(i.getNode()->getKey(),
					i.getNode()->getValue());
		}

		return *this;

	}

	Settings & operator=(Settings &other)
	{
		JMutexAutoLock lock(m_mutex);
		JMutexAutoLock lock2(other.m_mutex);

		if(&other == this)
			return *this;

		clear();
		(*this) += other;

		return *this;
	}
protected:
	core::map<std::string, std::string> m_settings;
	// All methods that access m_settings/m_defaults directly should lock this.
	JMutex m_mutex;

private:
	core::map<std::string, std::string> m_defaults;
};

class GameSettings : public Settings
{
public:
	GameSettings()
	{
		Settings();
	}

	// you'll find this in defaultsettings.cpp
	void setGameDefaults(std::string mode);

	virtual void set(std::string name, std::string value)
	{
		{
			JMutexAutoLock lock(m_mutex);
			m_settings[name] = value;
		}
		if (name == "game_mode")
			setGameDefaults(value);
	}

	virtual void set(std::string name, const char *value)
	{
		{
			JMutexAutoLock lock(m_mutex);
			m_settings[name] = value;
		}
		if (name == "game_mode")
			setGameDefaults(value);
	}
};

#endif

