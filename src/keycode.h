/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* keycode.h
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2013-2014 <lisa@ltmnet.com>
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

#ifndef KEYCODE_HEADER
#define KEYCODE_HEADER

#include "common_irrlicht.h"
#include <string>

/* A key press, consisting of either an Irrlicht keycode
or an actual char */

class KeyPress
{
public:
	KeyPress();
	KeyPress(const char *name);
	KeyPress(const irr::SEvent::SKeyInput &in, bool prefer_character=false);

	bool operator==(const KeyPress &o) const
	{
		return (Char > 0 && Char == o.Char) ||
			(valid_kcode(Key) && Key == o.Key);
	}

	void operator=(const KeyPress &o)
	{
		Key = o.Key;
		Char = o.Char;
		m_name = o.m_name;
	}

	const char *sym() const;
	const char *name() const;
    const std::wstring guiName() const;

	std::string debug() const;
protected:
	static bool valid_kcode(irr::EKEY_CODE k)
	{
		return k > 0 && k < irr::KEY_KEY_CODES_COUNT;
	}

	irr::EKEY_CODE Key;
	wchar_t Char;
	std::string m_name;
};

extern const KeyPress EscapeKey;
extern const KeyPress CancelKey;
extern const KeyPress NumberKey[10];

enum KeyCode {
	VLKC_FORWARD = 0,
	VLKC_BACKWARD,
	VLKC_LEFT,
	VLKC_RIGHT,
	VLKC_JUMP,
	VLKC_SNEAK,
	VLKC_INVENTORY,
	VLKC_USE,
	VLKC_CHAT,
	VLKC_COMMAND,
	VLKC_RANGE,
	VLKC_FREEMOVE,
	VLKC_UP,
	VLKC_DOWN,
	VLKC_RUN,
	VLKC_EXAMINE,
	VLKC_SCREENSHOT,
	VLKC_TOGGLE_HUD,
	VLKC_TOGGLE_CHAT,
	VLKC_TOGGLE_FOG,
	VLKC_TOGGLE_CAMERA,
	VLKC_TOGGLE_DEBUG,
	VLKC_TOGGLE_PROFILER,
	VLKC_RANGE_PLUS,
	VLKC_RANGE_MINUS,
	VLKC_PRINT_DEBUG,
	VLKC_SELECT_PREV,
	VLKC_SELECT_NEXT,
	// put new codes above this
	VLKC_MAX
};

// Key configuration getter
KeyPress getKeySetting(KeyCode code);
void saveKeySetting(KeyPress &key, KeyCode code);

// Clear fast lookup cache
void clearKeyCache();

irr::EKEY_CODE keyname_to_keycode(const char *name);

#endif

