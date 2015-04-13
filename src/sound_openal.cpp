/************************************************************************
* Minetest-c55
* Copyright (C) 2012 celeron55, Perttu Ahola <celeron55@gmail.com>
* OpenAL support based on work by:
* Copyright (C) 2011 Sebastian 'Bahamada' Rühl
* Copyright (C) 2011 Cyriaque 'Cisoun' Skrapits <cysoun@gmail.com>
* Copyright (C) 2011 Giuseppe Bilotta <giuseppe.bilotta@gmail.com>
*
* sound_openal.cpp
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

#ifndef SERVER

#include "sound_openal.h"

#if defined(_MSC_VER)
	#include <al.h>
	#include <alc.h>
	#include <alext.h>
#elif defined(__APPLE__)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <AL/alext.h>
#endif
#include <vorbis/vorbisfile.h>
#include "log.h"
#include <map>
#include <vector>
#include "utility.h" // myrand()
#include "path.h"
#include "settings.h"
#include "main.h"

#define BUFFER_SIZE 30000

static const char *alcErrorString(ALCenum err)
{
	switch (err) {
	case ALC_NO_ERROR:
		return "no error";
	case ALC_INVALID_DEVICE:
		return "invalid device";
	case ALC_INVALID_CONTEXT:
		return "invalid context";
	case ALC_INVALID_ENUM:
		return "invalid enum";
	case ALC_INVALID_VALUE:
		return "invalid value";
	case ALC_OUT_OF_MEMORY:
		return "out of memory";
	default:
		return "<unknown OpenAL error>";
	}
}

static const char *alErrorString(ALenum err)
{
	switch (err) {
	case AL_NO_ERROR:
		return "no error";
	case AL_INVALID_NAME:
		return "invalid name";
	case AL_INVALID_ENUM:
		return "invalid enum";
	case AL_INVALID_VALUE:
		return "invalid value";
	case AL_INVALID_OPERATION:
		return "invalid operation";
	case AL_OUT_OF_MEMORY:
		return "out of memory";
	default:
		return "<unknown OpenAL error>";
	}
}

void f3_set(ALfloat *f3, v3f v)
{
	f3[0] = v.X;
	f3[1] = v.Y;
	f3[2] = v.Z;
}

struct SoundBuffer
{
	ALenum format;
	ALsizei freq;
	ALuint buffer_id;
	std::vector<char> buffer;
	float gain;
};

SoundBuffer* loadOggFile(const std::string &filepath)
{
	int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE]; // Local fixed size array
	vorbis_info *pInfo;
	OggVorbis_File oggFile;

	// Try opening the given file
	if (ov_fopen((char*)filepath.c_str(), &oggFile) != 0) {
		infostream<<"Audio: Error opening "<<filepath<<" for decoding"<<std::endl;
		return NULL;
	}

	SoundBuffer *snd = new SoundBuffer;

	// Get some information about the OGG file
	pInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1) {
		snd->format = AL_FORMAT_MONO16;
	}else{
		snd->format = AL_FORMAT_STEREO16;
	}

	// The frequency of the sampling rate
	snd->freq = pInfo->rate;

	// Keep reading until all is read
	do {
		// Read up to a buffer's worth of decoded sound data
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

		if (bytes < 0) {
			ov_clear(&oggFile);
			infostream<<"Audio: Error decoding "<<filepath<<std::endl;
			return NULL;
		}

		// Append to end of buffer
		snd->buffer.insert(snd->buffer.end(), array, array + bytes);
	} while (bytes > 0);

	alGenBuffers(1, &snd->buffer_id);
	alBufferData(snd->buffer_id, snd->format,
			&(snd->buffer[0]), snd->buffer.size(),
			snd->freq);

	ALenum error = alGetError();

	if (error != AL_NO_ERROR) {
		infostream<<"Audio: OpenAL error: "<<alErrorString(error)
				<<"preparing sound buffer"<<std::endl;
	}

	infostream<<"Audio file "<<filepath<<" loaded"<<std::endl;

	// Clean up!
	ov_clear(&oggFile);

	return snd;
}

struct PlayingSound
{
	SoundBuffer *buf;
	ALuint source_id;
	bool loop;
	bool should_delete;
	bool has_position;
	v3f pos;
	bool disabled;
	float gain;
};

class OpenALSoundManager: public ISoundManager
{
private:
	ALCdevice *m_device;
	ALCcontext *m_context;
	bool m_can_vorbis;
	int m_next_id;
	int m_music_id;
	int m_music_last_id;
	std::map<std::string, std::vector<SoundBuffer*> > m_buffers;
	std::map<int, PlayingSound*> m_sounds_playing;
	std::map<std::string, int> m_indexes;
	v3f m_listener_pos;
	JMutex m_mutex;
public:
	OpenALSoundManager():
		m_device(NULL),
		m_context(NULL),
		m_can_vorbis(false),
		m_next_id(1),
		m_music_id(0),
		m_music_last_id(0)
	{
		ALCenum error = ALC_NO_ERROR;

		infostream<<"Audio: Initializing..."<<std::endl;

		m_mutex.Init();

		m_device = alcOpenDevice(NULL);
		if (!m_device) {
			infostream<<"Audio: No audio device available, audio system "
				<<"not initialized"<<std::endl;
			return;
		}

		if (alcIsExtensionPresent(m_device, "EXT_vorbis")) {
			infostream<<"Audio: Vorbis extension present"<<std::endl;
			m_can_vorbis = true;
		}else{
			infostream<<"Audio: Vorbis extension NOT present"<<std::endl;
			m_can_vorbis = false;
		}

		m_context = alcCreateContext(m_device, NULL);
		if (!m_context) {
			error = alcGetError(m_device);
			infostream<<"Audio: Unable to initialize audio context, "
					<<"aborting audio initialization ("<<alcErrorString(error)
					<<")"<<std::endl;
			alcCloseDevice(m_device);
			m_device = NULL;
			return;
		}

		if (
			!alcMakeContextCurrent(m_context)
			|| (error = alcGetError(m_device) != ALC_NO_ERROR)
		) {
			infostream<<"Audio: Error setting audio context, aborting audio "
					<<"initialization ("<<alcErrorString(error)<<")"<<std::endl;
			alcDestroyContext(m_context);
			m_context = NULL;
			alcCloseDevice(m_device);
			m_device = NULL;
			return;
		}

		alDistanceModel(AL_EXPONENT_DISTANCE);

		{
			f32 volume = g_settings->getFloat("sound_volume");
			if (volume < 1.0)
				return;
			if (volume > 100.0)
				volume = 100.0;
			volume /= 100.0;
			alListenerf(AL_GAIN, volume);
		}

		infostream<<"Audio: Initialized: OpenAL "<<alGetString(AL_VERSION)
				<<", using "<<alcGetString(m_device, ALC_DEVICE_SPECIFIER)
				<<std::endl;
	}

	~OpenALSoundManager()
	{
		infostream<<"Audio: Deinitializing..."<<std::endl;
		g_sound = NULL;
		// KABOOM!
		// TODO: Clear SoundBuffers
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_context);
		m_context = NULL;
		alcCloseDevice(m_device);
		m_device = NULL;

		for (std::map<std::string, std::vector<SoundBuffer*> >::iterator i = m_buffers.begin();
				i != m_buffers.end(); i++) {
			for (std::vector<SoundBuffer*>::iterator iter = (*i).second.begin();
					iter != (*i).second.end(); iter++) {
				delete *iter;
			}
			(*i).second.clear();
		}
		m_buffers.clear();
		infostream<<"Audio: Deinitialized."<<std::endl;
	}

	void addBuffer(const std::string &name, SoundBuffer *buf)
	{
		std::map<std::string, std::vector<SoundBuffer*> >::iterator i =
				m_buffers.find(name);
		if (i != m_buffers.end()) {
			i->second.push_back(buf);
			return;
		}
		std::vector<SoundBuffer*> bufs;
		bufs.push_back(buf);
		m_buffers[name] = bufs;
		return;
	}

	SoundBuffer* getBuffer(const std::string &name)
	{
		std::map<std::string, std::vector<SoundBuffer*> >::iterator i =
				m_buffers.find(name);
		if (i == m_buffers.end())
			return NULL;
		std::vector<SoundBuffer*> &bufs = i->second;
		u32 j = m_indexes[name];
		if (j+1 >= bufs.size()) {
			m_indexes[name] = 0;
		}else{
			m_indexes[name] = j+1;
		}
		return bufs[j];
	}

	PlayingSound* createPlayingSound(SoundBuffer *buf, bool loop)
	{
		infostream<<"OpenALSoundManager: Creating playing sound"<<std::endl;
		assert(buf);
		PlayingSound *sound = new PlayingSound;
		assert(sound);
		alGenSources(1, &sound->source_id);
		alSourcei(sound->source_id, AL_BUFFER, buf->buffer_id);
		alSourcei(sound->source_id, AL_SOURCE_RELATIVE, true);
		alSource3f(sound->source_id, AL_POSITION, 0, 0, 0);
		alSource3f(sound->source_id, AL_VELOCITY, 0, 0, 0);
		alSourcei(sound->source_id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
		sound->gain = 1.0;
		alSourcef(sound->source_id, AL_GAIN, buf->gain);
		alSourcePlay(sound->source_id);
		sound->should_delete = false;
		sound->loop = loop;
		sound->has_position = false;
		sound->disabled = false;
		sound->buf = buf;
		return sound;
	}

	PlayingSound* createPlayingSoundAt(SoundBuffer *buf, bool loop, v3f pos, float gain, bool queue)
	{
		infostream<<"OpenALSoundManager: Creating positional playing sound"
				<<std::endl;
		assert(buf);
		if (buf->format != AL_FORMAT_MONO16)
			errorstream<<"Attempting to play non-mono sound as positional sound"<<std::endl;
		PlayingSound *sound = new PlayingSound;
		assert(sound);
		float distance = m_listener_pos.getDistanceFrom(pos);
		if (!queue && (!loop || distance < 160.0)) {
			alGenSources(1, &sound->source_id);
			alSourcei(sound->source_id, AL_BUFFER, buf->buffer_id);
			alSourcei(sound->source_id, AL_SOURCE_RELATIVE, false);
			alSource3f(sound->source_id, AL_POSITION, pos.X, pos.Y, pos.Z);
			alSource3f(sound->source_id, AL_VELOCITY, 0, 0, 0);
			alSourcef(sound->source_id, AL_REFERENCE_DISTANCE, 30.0);
			alSourcei(sound->source_id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
			float t_gain = MYMAX(0.0, buf->gain*gain);
			alSourcef(sound->source_id, AL_GAIN, t_gain);
			alSourcePlay(sound->source_id);
			sound->disabled = false;
		}else{
			sound->source_id = 0;
			sound->disabled = true;
		}
		sound->gain = gain;
		sound->should_delete = false;
		sound->loop = loop;
		sound->has_position = true;
		sound->pos = pos;
		sound->buf = buf;
		return sound;
	}

	int playSoundRaw(SoundBuffer *buf, bool loop)
	{
		assert(buf);
		PlayingSound *sound = createPlayingSound(buf, loop);
		if (!sound)
			return -1;
		int id = -1;
		{
			JMutexAutoLock lock(m_mutex);
			id = m_next_id++;
			m_sounds_playing[id] = sound;
		}
		return id;
	}

	int playSoundRawAt(SoundBuffer *buf, bool loop, v3f pos, float gain, bool queue)
	{
		assert(buf);
		PlayingSound *sound = createPlayingSoundAt(buf, loop, pos, gain, queue);
		if (!sound)
			return -1;
		int id = -1;
		{
			JMutexAutoLock lock(m_mutex);
			id = m_next_id++;
			m_sounds_playing[id] = sound;
		}
		return id;
	}

	void deleteSound(int id)
	{
		std::map<int, PlayingSound*>::iterator i = m_sounds_playing.find(id);
		if (id == m_music_id)
			m_music_id = 0;
		if (id == m_music_last_id)
			m_music_last_id = 0;
		if (i == m_sounds_playing.end())
			return;

		PlayingSound *sound = i->second;

		alSourceStop(sound->source_id);
		alDeleteSources(1, &sound->source_id);

		delete sound;

		m_sounds_playing.erase(id);

	}

	// Remove stopped sounds
	void maintain(float dtime)
	{
		JMutexAutoLock lock(m_mutex);
		verbosestream<<"OpenALSoundManager::maintain(): "
				<<m_sounds_playing.size()<<" playing sounds, "
				<<m_buffers.size()<<" sound names loaded"<<std::endl;
		std::set<int> del_list;
		for (std::map<int, PlayingSound*>::iterator i = m_sounds_playing.begin(); i != m_sounds_playing.end(); i++) {
			int id = i->first;
			PlayingSound *sound = i->second;
			if (sound->has_position && sound->loop && !sound->should_delete) {
				float distance = m_listener_pos.getDistanceFrom(sound->pos);
				if (distance > 320.0 && !sound->disabled) {
					alDeleteSources(1, &sound->source_id);
					sound->disabled = true;
					sound->source_id = 0;
				}else if (distance < 160.0 && sound->disabled) {
					alGenSources(1, &sound->source_id);
					alSourcei(sound->source_id, AL_BUFFER, sound->buf->buffer_id);
					alSourcei(sound->source_id, AL_SOURCE_RELATIVE, false);
					alSource3f(sound->source_id, AL_POSITION, sound->pos.X, sound->pos.Y, sound->pos.Z);
					alSource3f(sound->source_id, AL_VELOCITY, 0, 0, 0);
					alSourcef(sound->source_id, AL_REFERENCE_DISTANCE, 30.0);
					alSourcei(sound->source_id, AL_LOOPING, AL_TRUE);
					float t_gain = MYMAX(0.0, sound->buf->gain*sound->gain);
					alSourcef(sound->source_id, AL_GAIN, t_gain);
					alSourcePlay(sound->source_id);
					sound->disabled = false;
				}
				continue;
			}
			if (sound->should_delete) {
				del_list.insert(id);
			}else{ // If not playing, remove it
				ALint state;
				alGetSourcei(sound->source_id, AL_SOURCE_STATE, &state);
				if (state != AL_PLAYING) {
					del_list.insert(id);
					if (id == m_music_last_id)
						m_music_last_id = 0;
				}
			}
			if (id == m_music_last_id) {
				ALfloat volume;
				alSourcei(sound->source_id, AL_LOOPING, AL_FALSE);
				alGetSourcef(sound->source_id, AL_GAIN, &volume);
				if (dtime > 0.05)
					dtime = 0.05;
				while (volume > 1.) {
					volume /= 10.;
				}
				volume -= dtime;
				if (volume < 0.01) {
					volume = 0.0;
					m_music_last_id = 0;
					sound->should_delete = true;
				}
				alSourcef(sound->source_id, AL_GAIN, volume);
			}
		}
		if (del_list.size() == 0)
			return;

		verbosestream<<"OpenALSoundManager::maintain(): deleting "
					<<del_list.size()<<" playing sounds"<<std::endl;
		for (std::set<int>::iterator i = del_list.begin(); i != del_list.end(); i++) {
			int id = *i;
			deleteSound(id);
		}
	}

	bool loadSound(const std::string &name, const std::string &filepath, float gain)
	{
		std::string path = getPath("sound",filepath,true);
		if (path == "")
			return false;
		SoundBuffer *buf = loadOggFile(path);
		if (buf == NULL)
			return false;
		buf->gain = gain;
		addBuffer(name, buf);
		return true;
	}

	void updateListener(v3f pos, v3f vel, v3f at, v3f up)
	{
		m_listener_pos = pos;
		alListener3f(AL_POSITION, pos.X, pos.Y, pos.Z);
		alListener3f(AL_VELOCITY, vel.X, vel.Y, vel.Z);
		ALfloat f[6];
		f3_set(f, at);
		f3_set(f+3, -up);
		alListenerfv(AL_ORIENTATION, f);
	}

	void setListenerGain(float gain)
	{
		alListenerf(AL_GAIN, gain);
	}

	int playSound(const std::string &name, bool loop)
	{
		if (name == "")
			return 0;
		SoundBuffer *buf = getBuffer(name);
		if (!buf) {
			infostream<<"OpenALSoundManager: \""<<name<<"\" not found."
					<<std::endl;
			return -1;
		}
		return playSoundRaw(buf, loop);
	}
	int playSoundAt(const std::string &name, bool loop, v3f pos, float gain, bool queue=false)
	{
		if (name == "")
			return 0;
		SoundBuffer *buf = getBuffer(name);
		if (!buf) {
			infostream<<"OpenALSoundManager: \""<<name<<"\" not found."
					<<std::endl;
			return -1;
		}
		return playSoundRawAt(buf, loop, pos, gain, queue);
	}
	void stopSound(int id)
	{
		JMutexAutoLock lock(m_mutex);
		std::map<int, PlayingSound*>::iterator i = m_sounds_playing.find(id);
		if (id == m_music_id)
			m_music_id = 0;
		if (id == m_music_last_id)
			m_music_last_id = 0;
		if (i == m_sounds_playing.end())
			return;
		PlayingSound *sound = i->second;
		sound->should_delete = true;
	}
	bool soundExists(int sound)
	{
		JMutexAutoLock lock(m_mutex);
		return (m_sounds_playing.count(sound) != 0);
	}

	bool playMusic(const std::string &name, bool loop)
	{
		stopMusic();

		m_music_id = playSound(name,loop);
		if (m_music_id > 0)
			return true;
		m_music_id = 0;
		return false;
	}
	void stopMusic()
	{
		if (m_music_id != 0) {
			if (m_music_last_id != 0)
				stopSound(m_music_last_id);
			m_music_last_id = m_music_id;
		}
		m_music_id = 0;
	}

	void updateSoundPosition(int id, v3f pos)
	{
		std::map<int, PlayingSound*>::iterator i =
				m_sounds_playing.find(id);
		if (i == m_sounds_playing.end())
			return;
		PlayingSound *sound = i->second;

		alSourcei(sound->source_id, AL_SOURCE_RELATIVE, false);
		alSource3f(sound->source_id, AL_POSITION, pos.X, pos.Y, pos.Z);
		alSource3f(sound->source_id, AL_VELOCITY, 0, 0, 0);
		alSourcef(sound->source_id, AL_REFERENCE_DISTANCE, 30.0);
	}
};

ISoundManager *createSoundManager()
{
	ISoundManager *sound = new OpenALSoundManager();
	g_sound = sound;
	init_sounds(sound);
	return sound;
};

#endif
