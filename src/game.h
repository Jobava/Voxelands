/************************************************************************
* Minetest-c55
* Copyright (C) 2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* game.h
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

#ifndef GAME_HEADER
#define GAME_HEADER

#include "common_irrlicht.h"
#include <string>

#include "keycode.h"

void drawLoadingScreen(video::IVideoDriver* driver, const std::wstring);

class KeyList : protected core::list<KeyPress>
{
	typedef core::list<KeyPress> super;
	typedef super::Iterator Iterator;
	typedef super::ConstIterator ConstIterator;

	virtual ConstIterator find(const KeyPress &key) const
	{
		ConstIterator f(begin());
		ConstIterator e(end());
		while (f!=e) {
			if (*f == key)
				return f;
			++f;
		}
		return e;
	}

	virtual Iterator find(const KeyPress &key)
	{
		Iterator f(begin());
		Iterator e(end());
		while (f!=e) {
			if (*f == key)
				return f;
			++f;
		}
		return e;
	}

public:
	void clear() { super::clear(); }

	void set(const KeyPress &key)
	{
		if (find(key) == end())
			push_back(key);
	}

	void unset(const KeyPress &key)
	{
		Iterator p(find(key));
		if (p != end())
			erase(p);
	}

	void toggle(const KeyPress &key)
	{
		Iterator p(this->find(key));
		if (p != end())
			erase(p);
		else
			push_back(key);
	}

	bool operator[](const KeyPress &key) const
	{
		return find(key) != end();
	}
};

class InputHandler
{
public:
	InputHandler()
	{
	}
	virtual ~InputHandler()
	{
	}

	virtual bool isKeyDown(const KeyPress &keyCode) = 0;
	virtual bool wasKeyDown(const KeyPress &keyCode) = 0;

	virtual v2s32 getMousePos() = 0;
	virtual void setMousePos(s32 x, s32 y) = 0;

	virtual bool getLeftState() = 0;
	virtual bool getRightState() = 0;

	virtual bool getLeftClicked() = 0;
	virtual bool getRightClicked() = 0;
	virtual void resetLeftClicked() = 0;
	virtual void resetRightClicked() = 0;

	virtual bool getLeftReleased() = 0;
	virtual bool getRightReleased() = 0;
	virtual void resetLeftReleased() = 0;
	virtual void resetRightReleased() = 0;

	virtual s32 getMouseWheel() = 0;

	virtual void step(float dtime) {};

	virtual void clear() {};
};

class ISoundManager;

void the_game(
	bool &kill,
	bool random_input,
	InputHandler *input,
	IrrlichtDevice *device,
	gui::IGUIFont* font,
	std::string map_dir,
	std::string playername,
	std::string password,
	std::string address,
	u16 port,
	std::wstring &error_message,
	std::string configpath,
	ISoundManager *sound
);

#endif

