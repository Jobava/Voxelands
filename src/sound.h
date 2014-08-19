/*
Minetest-Classic
Copyright (C) 2012 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef SOUND_HEADER
#define SOUND_HEADER

#include "common_irrlicht.h"
#include <string>
#include <vector>
#include <set>

class OnDemandSoundFetcher
{
public:
	virtual void getSoundFilenames(const std::string &name,
			std::set<std::string> &dst);
};

class ISoundManager
{
public:
	virtual ~ISoundManager(){}

	// Multiple sounds can be loaded per name; when played, the sound
	// should be chosen randomly from alternatives
	// Return value determines success/failure
	virtual bool loadSound(const std::string &name,
			const std::string &filepath) = 0;
	virtual bool loadSound(const std::string &name,
			const std::vector<char> &filedata) = 0;

	virtual void updateListener(v3f pos, v3f vel, v3f at, v3f up) = 0;
	// playSound functions return -1 on failure, otherwise a handle to the
	// sound
	virtual int playSound(const std::string &name, int loopcount,
			float volume) = 0;
	virtual int playSoundAt(const std::string &name, int loopcount,
			v3f pos, float volume) = 0;
	virtual void stopSound(int sound) = 0;
};

class DummySoundManager: public ISoundManager
{
public:
	virtual bool loadSound(const std::string &name,
			const std::string &filepath) {return true;}
	virtual bool loadSound(const std::string &name,
			const std::vector<char> &filedata) {return true;}
	void updateListener(v3f pos, v3f vel, v3f at, v3f up) {}
	int playSound(const std::string &name, int loopcount,
			float volume) {return 0;}
	int playSoundAt(const std::string &name, int loopcount,
			v3f pos, float volume) {return 0;}
	void stopSound(int sound) {}
};

// Global DummySoundManager singleton
extern DummySoundManager dummySoundManager;

#endif
