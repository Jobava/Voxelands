/************************************************************************
* Minetest-c55
* Copyright (C) 2012 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* sound.h
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

#ifndef SOUND_HEADER
#define SOUND_HEADER

#include "common_irrlicht.h"
#include <string>
#include <vector>
#include <set>
#include "mapnode.h"

class ISoundManager
{
public:
	virtual ~ISoundManager(){}

	// Multiple sounds can be loaded per name; when played, the sound
	// should be chosen randomly from alternatives
	// Return value determines success/failure
	virtual bool loadSound(const std::string &name, const std::string &filepath, float gain=1.0) = 0;

	virtual void updateListener(v3f pos, v3f vel, v3f at, v3f up) = 0;
	virtual void setListenerGain(float gain) = 0;

	// playSound functions return -1 on failure, otherwise a handle to the
	// sound. If name=="", call should be ignored without error.
	virtual int playSound(const std::string &name, bool loop) = 0;
	virtual int playSoundAt(const std::string &name, bool loop, v3f pos, float gain=1.0) = 0;
	virtual void stopSound(int sound) = 0;
	virtual bool soundExists(int sound) = 0;

	virtual bool playMusic(const std::string &name, bool loop) = 0;
	virtual void stopMusic() = 0;

	virtual void updateSoundPosition(int sound, v3f pos) = 0;

	virtual void maintain(float dtime) = 0;
};

class DummySoundManager: public ISoundManager
{
public:
	virtual bool loadSound(const std::string &name, const std::string &filepath, float gain) {return true;}

	void updateListener(v3f pos, v3f vel, v3f at, v3f up) {}
	void setListenerGain(float gain) {}

	int playSound(const std::string &name, bool loop) {return 0;}
	int playSoundAt(const std::string &name, bool loop, v3f pos, float gain) {return 0;}
	void stopSound(int sound) {}
	bool soundExists(int sound) {return false;}

	bool playMusic(const std::string &name, bool loop) {return false;}
	void stopMusic() {}

	void updateSoundPosition(int sound, v3f pos) {}

	void maintain(float dtime) {}
};

ISoundManager *createSoundManager();
void init_sounds(ISoundManager *sound);

class Map;

// Global DummySoundManager singleton
extern DummySoundManager dummySoundManager;
extern ISoundManager *g_sound;

void sound_playStep(Map *map, v3f pos, int foot, float gain=1.0);
void sound_playDig(content_t c, v3f pos);

#endif
