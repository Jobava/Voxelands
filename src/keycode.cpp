/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* keycode.cpp
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

#include "keycode.h"
#include "main.h" // For g_settings
#include "exceptions.h"
#include "settings.h"
#include "hex.h"
#include "intl.h"
#include <map>

class UnknownKeycode : public BaseException
{
public:
	UnknownKeycode(const char *s) :
		BaseException(s) {};
};

#define CHECKKEY(x){if(strcmp(name, #x)==0) return irr::x;}

irr::EKEY_CODE keyname_to_keycode(const char *name)
{
	CHECKKEY(KEY_LBUTTON)
	CHECKKEY(KEY_RBUTTON)
	CHECKKEY(KEY_CANCEL)
	CHECKKEY(KEY_MBUTTON)
	CHECKKEY(KEY_XBUTTON1)
	CHECKKEY(KEY_XBUTTON2)
	CHECKKEY(KEY_BACK)
	CHECKKEY(KEY_TAB)
	CHECKKEY(KEY_CLEAR)
	CHECKKEY(KEY_RETURN)
	CHECKKEY(KEY_SHIFT)
	CHECKKEY(KEY_CONTROL)
	CHECKKEY(KEY_MENU)
	CHECKKEY(KEY_PAUSE)
	CHECKKEY(KEY_CAPITAL)
	CHECKKEY(KEY_KANA)
	CHECKKEY(KEY_HANGUEL)
	CHECKKEY(KEY_HANGUL)
	CHECKKEY(KEY_JUNJA)
	CHECKKEY(KEY_FINAL)
	CHECKKEY(KEY_HANJA)
	CHECKKEY(KEY_KANJI)
	CHECKKEY(KEY_ESCAPE)
	CHECKKEY(KEY_CONVERT)
	CHECKKEY(KEY_NONCONVERT)
	CHECKKEY(KEY_ACCEPT)
	CHECKKEY(KEY_MODECHANGE)
	CHECKKEY(KEY_SPACE)
	CHECKKEY(KEY_PRIOR)
	CHECKKEY(KEY_NEXT)
	CHECKKEY(KEY_END)
	CHECKKEY(KEY_HOME)
	CHECKKEY(KEY_LEFT)
	CHECKKEY(KEY_UP)
	CHECKKEY(KEY_RIGHT)
	CHECKKEY(KEY_DOWN)
	CHECKKEY(KEY_SELECT)
	CHECKKEY(KEY_PRINT)
	CHECKKEY(KEY_EXECUT)
	CHECKKEY(KEY_SNAPSHOT)
	CHECKKEY(KEY_INSERT)
	CHECKKEY(KEY_DELETE)
	CHECKKEY(KEY_HELP)
	CHECKKEY(KEY_KEY_0)
	CHECKKEY(KEY_KEY_1)
	CHECKKEY(KEY_KEY_2)
	CHECKKEY(KEY_KEY_3)
	CHECKKEY(KEY_KEY_4)
	CHECKKEY(KEY_KEY_5)
	CHECKKEY(KEY_KEY_6)
	CHECKKEY(KEY_KEY_7)
	CHECKKEY(KEY_KEY_8)
	CHECKKEY(KEY_KEY_9)
	CHECKKEY(KEY_KEY_A)
	CHECKKEY(KEY_KEY_B)
	CHECKKEY(KEY_KEY_C)
	CHECKKEY(KEY_KEY_D)
	CHECKKEY(KEY_KEY_E)
	CHECKKEY(KEY_KEY_F)
	CHECKKEY(KEY_KEY_G)
	CHECKKEY(KEY_KEY_H)
	CHECKKEY(KEY_KEY_I)
	CHECKKEY(KEY_KEY_J)
	CHECKKEY(KEY_KEY_K)
	CHECKKEY(KEY_KEY_L)
	CHECKKEY(KEY_KEY_M)
	CHECKKEY(KEY_KEY_N)
	CHECKKEY(KEY_KEY_O)
	CHECKKEY(KEY_KEY_P)
	CHECKKEY(KEY_KEY_Q)
	CHECKKEY(KEY_KEY_R)
	CHECKKEY(KEY_KEY_S)
	CHECKKEY(KEY_KEY_T)
	CHECKKEY(KEY_KEY_U)
	CHECKKEY(KEY_KEY_V)
	CHECKKEY(KEY_KEY_W)
	CHECKKEY(KEY_KEY_X)
	CHECKKEY(KEY_KEY_Y)
	CHECKKEY(KEY_KEY_Z)
	CHECKKEY(KEY_LWIN)
	CHECKKEY(KEY_RWIN)
	CHECKKEY(KEY_APPS)
	CHECKKEY(KEY_SLEEP)
	CHECKKEY(KEY_NUMPAD0)
	CHECKKEY(KEY_NUMPAD1)
	CHECKKEY(KEY_NUMPAD2)
	CHECKKEY(KEY_NUMPAD3)
	CHECKKEY(KEY_NUMPAD4)
	CHECKKEY(KEY_NUMPAD5)
	CHECKKEY(KEY_NUMPAD6)
	CHECKKEY(KEY_NUMPAD7)
	CHECKKEY(KEY_NUMPAD8)
	CHECKKEY(KEY_NUMPAD9)
	CHECKKEY(KEY_MULTIPLY)
	CHECKKEY(KEY_ADD)
	CHECKKEY(KEY_SEPARATOR)
	CHECKKEY(KEY_SUBTRACT)
	CHECKKEY(KEY_DECIMAL)
	CHECKKEY(KEY_DIVIDE)
	CHECKKEY(KEY_F1)
	CHECKKEY(KEY_F2)
	CHECKKEY(KEY_F3)
	CHECKKEY(KEY_F4)
	CHECKKEY(KEY_F5)
	CHECKKEY(KEY_F6)
	CHECKKEY(KEY_F7)
	CHECKKEY(KEY_F8)
	CHECKKEY(KEY_F9)
	CHECKKEY(KEY_F10)
	CHECKKEY(KEY_F11)
	CHECKKEY(KEY_F12)
	CHECKKEY(KEY_F13)
	CHECKKEY(KEY_F14)
	CHECKKEY(KEY_F15)
	CHECKKEY(KEY_F16)
	CHECKKEY(KEY_F17)
	CHECKKEY(KEY_F18)
	CHECKKEY(KEY_F19)
	CHECKKEY(KEY_F20)
	CHECKKEY(KEY_F21)
	CHECKKEY(KEY_F22)
	CHECKKEY(KEY_F23)
	CHECKKEY(KEY_F24)
	CHECKKEY(KEY_NUMLOCK)
	CHECKKEY(KEY_SCROLL)
	CHECKKEY(KEY_LSHIFT)
	CHECKKEY(KEY_RSHIFT)
	CHECKKEY(KEY_LCONTROL)
	CHECKKEY(KEY_RCONTROL)
	CHECKKEY(KEY_LMENU)
	CHECKKEY(KEY_RMENU)
	CHECKKEY(KEY_PLUS)
	CHECKKEY(KEY_COMMA)
	CHECKKEY(KEY_MINUS)
	CHECKKEY(KEY_PERIOD)
	CHECKKEY(KEY_ATTN)
	CHECKKEY(KEY_CRSEL)
	CHECKKEY(KEY_EXSEL)
	CHECKKEY(KEY_EREOF)
	CHECKKEY(KEY_PLAY)
	CHECKKEY(KEY_ZOOM)
	CHECKKEY(KEY_PA1)
	CHECKKEY(KEY_OEM_CLEAR)

	throw UnknownKeycode(name);
}

static const char *KeyNames[] =
{ "-", "KEY_LBUTTON", "KEY_RBUTTON", "KEY_CANCEL", "KEY_MBUTTON", "KEY_XBUTTON1",
		"KEY_XBUTTON2", "-", "KEY_BACK", "KEY_TAB", "-", "-", "KEY_CLEAR", "KEY_RETURN", "-",
		"-", "KEY_SHIFT", "KEY_CONTROL", "KEY_MENU", "KEY_PAUSE", "KEY_CAPITAL", "KEY_KANA", "-",
		"KEY_JUNJA", "KEY_FINAL", "KEY_KANJI", "-", "KEY_ESCAPE", "KEY_CONVERT", "KEY_NONCONVERT",
		"KEY_ACCEPT", "KEY_MODECHANGE", "KEY_SPACE", "KEY_PRIOR", "KEY_NEXT", "KEY_END",
		"KEY_HOME", "KEY_LEFT", "KEY_UP", "KEY_RIGHT", "KEY_DOWN", "KEY_SELECT", "KEY_PRINT",
		"KEY_EXECUTE", "KEY_SNAPSHOT", "KEY_INSERT", "KEY_DELETE", "KEY_HELP", "KEY_KEY_0",
		"KEY_KEY_1", "KEY_KEY_2", "KEY_KEY_3", "KEY_KEY_4", "KEY_KEY_5",
		"KEY_KEY_6", "KEY_KEY_7", "KEY_KEY_8", "KEY_KEY_9", "-", "-", "-", "-",
		"-", "-", "-", "KEY_KEY_A", "KEY_KEY_B", "KEY_KEY_C", "KEY_KEY_D",
		"KEY_KEY_E", "KEY_KEY_F", "KEY_KEY_G", "KEY_KEY_H", "KEY_KEY_I",
		"KEY_KEY_J", "KEY_KEY_K", "KEY_KEY_L", "KEY_KEY_M", "KEY_KEY_N",
		"KEY_KEY_O", "KEY_KEY_P", "KEY_KEY_Q", "KEY_KEY_R", "KEY_KEY_S",
		"KEY_KEY_T", "KEY_KEY_U", "KEY_KEY_V", "KEY_KEY_W", "KEY_KEY_X",
		"KEY_KEY_Y", "KEY_KEY_Z", "KEY_LWIN", "KEY_RWIN", "KEY_APPS", "-",
		"KEY_SLEEP", "KEY_NUMPAD0", "KEY_NUMPAD1", "KEY_NUMPAD2", "KEY_NUMPAD3",
		"KEY_NUMPAD4", "KEY_NUMPAD5", "KEY_NUMPAD6", "KEY_NUMPAD7",
		"KEY_NUMPAD8", "KEY_NUMPAD9", "KEY_MULTIPLY", "KEY_ADD", "KEY_SEPERATOR",
		"KEY_SUBTRACT", "KEY_DECIMAL", "KEY_DIVIDE", "KEY_F1", "KEY_F2", "KEY_F3",
		"KEY_F4", "KEY_F5", "KEY_F6", "KEY_F7", "KEY_F8", "KEY_F9", "KEY_F10",
		"KEY_F11", "KEY_F12", "KEY_F13", "KEY_F14", "KEY_F15", "KEY_F16",
		"KEY_F17", "KEY_F18", "KEY_F19", "KEY_F20", "KEY_F21", "KEY_F22",
		"KEY_F23", "KEY_F24", "-", "-", "-", "-", "-", "-", "-", "-",
		"KEY_NUMLOCK", "KEY_SCROLL", "-", "-", "-", "-", "-", "-", "-", "-", "-",
		"-", "-", "-", "-", "-", "KEY_LSHIFT", "KEY_RSHIFT", "KEY_LCONTROL",
		"KEY_RCONTROL", "KEY_LMENU", "KEY_RMENU", "-", "-", "-", "-", "-",
		"-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-",
		"-", "-", "KEY_PLUS", "KEY_COMMA", "KEY_MINUS", "KEY_PERIOD", "-", "-", "-", "-", "-",
		"-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-",
		"-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-",
		"-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-", "-",
		"-", "-", "-", "-", "-", "-", "-", "-", "KEY_ATTN", "KEY_CRSEL", "KEY_EXSEL",
		"KEY_EREOF", "KEY_PLAY", "KEY_ZOOM", "KEY_PA1", "KEY_OEM_CLEAR", "-" };
		
static const char *KeyNamesLang[255];

void init_KeyNamesLang()
{
	for (int i = 0; i < 255; i++)
		KeyNamesLang[i] = gettext("-");

	KeyNamesLang[1] = gettext("Left Button");
	KeyNamesLang[2] = gettext("Right Button");
	KeyNamesLang[3] = gettext("Cancel");
	KeyNamesLang[4] = gettext("Middle Button");
	KeyNamesLang[5] = gettext("X Button 1");
	KeyNamesLang[6] = gettext("X Button 2");
	KeyNamesLang[8] = gettext("Back");
	KeyNamesLang[9] = gettext("Tab");
	KeyNamesLang[12] = gettext("Clear");
	KeyNamesLang[13] = gettext("Return");
	KeyNamesLang[16] = gettext("Shift");
	KeyNamesLang[17] = gettext("Control");
	KeyNamesLang[18] = gettext("Menu");
	KeyNamesLang[19] = gettext("Pause");
	KeyNamesLang[20] = gettext("Capital");
	KeyNamesLang[21] = gettext("Kana");
	KeyNamesLang[23] = gettext("Junja");
	KeyNamesLang[24] = gettext("Final");
	KeyNamesLang[25] = gettext("Kanji");
	KeyNamesLang[27] = gettext("Escape");
	KeyNamesLang[28] = gettext("Convert");
	KeyNamesLang[29] = gettext("Nonconvert");
	KeyNamesLang[30] = gettext("Accept");
	KeyNamesLang[31] = gettext("Mode Change");
	KeyNamesLang[32] = gettext("Space");
	KeyNamesLang[33] = gettext("Prior");
	KeyNamesLang[34] = gettext("Next");
	KeyNamesLang[35] = gettext("End");
	KeyNamesLang[36] = gettext("Home");
	KeyNamesLang[37] = gettext("Left");
	KeyNamesLang[38] = gettext("Up");
	KeyNamesLang[39] = gettext("Right");
	KeyNamesLang[40] = gettext("Down");
	KeyNamesLang[41] = gettext("Select");
	KeyNamesLang[42] = gettext("Print");
	KeyNamesLang[43] = gettext("Execute");
	KeyNamesLang[44] = gettext("Snapshot");
	KeyNamesLang[45] = gettext("Insert");
	KeyNamesLang[46] = gettext("Delete");
	KeyNamesLang[47] = gettext("Help");
	KeyNamesLang[48] = gettext("0");
	KeyNamesLang[49] = gettext("1");
	KeyNamesLang[50] = gettext("2");
	KeyNamesLang[51] = gettext("3");
	KeyNamesLang[52] = gettext("4");
	KeyNamesLang[53] = gettext("5");
	KeyNamesLang[54] = gettext("6");
	KeyNamesLang[55] = gettext("7");
	KeyNamesLang[56] = gettext("8");
	KeyNamesLang[57] = gettext("9");
	KeyNamesLang[65] = gettext("A");
	KeyNamesLang[66] = gettext("B");
	KeyNamesLang[67] = gettext("C");
	KeyNamesLang[68] = gettext("D");
	KeyNamesLang[69] = gettext("E");
	KeyNamesLang[70] = gettext("F");
	KeyNamesLang[71] = gettext("G");
	KeyNamesLang[72] = gettext("H");
	KeyNamesLang[73] = gettext("I");
	KeyNamesLang[74] = gettext("J");
	KeyNamesLang[75] = gettext("K");
	KeyNamesLang[76] = gettext("L");
	KeyNamesLang[77] = gettext("M");
	KeyNamesLang[78] = gettext("N");
	KeyNamesLang[79] = gettext("O");
	KeyNamesLang[80] = gettext("P");
	KeyNamesLang[81] = gettext("Q");
	KeyNamesLang[82] = gettext("R");
	KeyNamesLang[83] = gettext("S");
	KeyNamesLang[84] = gettext("T");
	KeyNamesLang[85] = gettext("U");
	KeyNamesLang[86] = gettext("V");
	KeyNamesLang[87] = gettext("W");
	KeyNamesLang[88] = gettext("X");
	KeyNamesLang[89] = gettext("Y");
	KeyNamesLang[90] = gettext("Z");
	KeyNamesLang[91] = gettext("Left Windows");
	KeyNamesLang[92] = gettext("Right Windows");
	KeyNamesLang[93] = gettext("Apps");
	KeyNamesLang[95] = gettext("Sleep");
	KeyNamesLang[96] = gettext("Numpad 0");
	KeyNamesLang[97] = gettext("Numpad 1");
	KeyNamesLang[98] = gettext("Numpad 2");
	KeyNamesLang[99] = gettext("Numpad 3");
	KeyNamesLang[100] = gettext("Numpad 4");
	KeyNamesLang[101] = gettext("Numpad 5");
	KeyNamesLang[102] = gettext("Numpad 6");
	KeyNamesLang[103] = gettext("Numpad 7");
	KeyNamesLang[104] = gettext("Numpad 8");
	KeyNamesLang[105] = gettext("Numpad 9");
	KeyNamesLang[106] = gettext("Numpad *");
	KeyNamesLang[107] = gettext("Numpad +");
	KeyNamesLang[108] = gettext("Numpad /");
	KeyNamesLang[109] = gettext("Numpad -");
	KeyNamesLang[110] = gettext("Numpad .");
	KeyNamesLang[111] = gettext("Numpad /");
	KeyNamesLang[121] = gettext("F10");
	KeyNamesLang[122] = gettext("F11");
	KeyNamesLang[123] = gettext("F12");
	KeyNamesLang[124] = gettext("F13");
	KeyNamesLang[125] = gettext("F14");
	KeyNamesLang[126] = gettext("F15");
	KeyNamesLang[127] = gettext("F16");
	KeyNamesLang[128] = gettext("F17");
	KeyNamesLang[129] = gettext("F18");
	KeyNamesLang[130] = gettext("F19");
	KeyNamesLang[131] = gettext("F20");
	KeyNamesLang[132] = gettext("F21");
	KeyNamesLang[133] = gettext("F22");
	KeyNamesLang[134] = gettext("F23");
	KeyNamesLang[135] = gettext("F24");
	KeyNamesLang[144] = gettext("Num Lock");
	KeyNamesLang[145] = gettext("Scroll Lock");
	KeyNamesLang[160] = gettext("Left Shift");
	KeyNamesLang[161] = gettext("Right Shift");
	KeyNamesLang[162] = gettext("Left Control");
	KeyNamesLang[163] = gettext("Right Control");
	KeyNamesLang[164] = gettext("Left Menu");
	KeyNamesLang[165] = gettext("Right Menu");
	KeyNamesLang[187] = gettext("Plus");
	KeyNamesLang[188] = gettext("Comma");
	KeyNamesLang[189] = gettext("Minus");
	KeyNamesLang[190] = gettext("Period");
	KeyNamesLang[246] = gettext("Attn");
	KeyNamesLang[247] = gettext("CrSel");
	KeyNamesLang[248] = gettext("ExSel");
	KeyNamesLang[249] = gettext("Erase OEF");
	KeyNamesLang[250] = gettext("Play");
	KeyNamesLang[251] = gettext("Zoom");
	KeyNamesLang[252] = gettext("PA1");
	KeyNamesLang[253] = gettext("OEM Clear");
}

KeyPress::KeyPress() :
	Key(irr::KEY_KEY_CODES_COUNT),
	Char(L'\0')
{}

KeyPress::KeyPress(const char *name)
{
	if (strlen(name) > 4) {
		try {
			Key = keyname_to_keycode(name);
			m_name = name;
			if (strlen(name) > 8 && strncmp(name, "KEY_KEY_", 8) == 0) {
				int chars_read = mbtowc(&Char, name + 8, 1);
				assert (chars_read == 1 && "unexpected multibyte character");
			} else
				Char = L'\0';
			return;
		} catch (UnknownKeycode &e) {};
	} else {
		// see if we can set it up as a KEY_KEY_something
		m_name = "KEY_KEY_";
		m_name += name;
		try {
			Key = keyname_to_keycode(m_name.c_str());
			int chars_read = mbtowc(&Char, name, 1);
			assert (chars_read == 1 && "unexpected multibyte character");
			return;
		} catch (UnknownKeycode &e) {};
	}

	// it's not a (known) key, just take the first char and use that

	Key = irr::KEY_KEY_CODES_COUNT;

	int mbtowc_ret = mbtowc(&Char, name, 1);
	assert (mbtowc_ret == 1 && "unexpected multibyte character");
	m_name = name[0];
}

KeyPress::KeyPress(const irr::SEvent::SKeyInput &in, bool prefer_character)
{
	Key = in.Key;
	Char = in.Char;

	if(prefer_character){
		m_name.resize(MB_CUR_MAX+1, '\0');
		int written = wctomb(&m_name[0], Char);
		if(written > 0){
			infostream<<"KeyPress: Preferring character for "<<m_name<<std::endl;
			Key = irr::KEY_KEY_CODES_COUNT;
			return;
		}
	}

	if (valid_kcode(Key)) {
		m_name = KeyNames[Key];
	} else {
		m_name.resize(MB_CUR_MAX+1, '\0');
		int written = wctomb(&m_name[0], Char);
		if(written < 0){
			std::string hexstr = hex_encode((const char*)&Char, sizeof(Char));
			errorstream<<"KeyPress: Unexpected multibyte character "<<hexstr<<std::endl;
		}
	}
}

const char *KeyPress::sym() const
{
	if (Key && Key < irr::KEY_KEY_CODES_COUNT) {
		return KeyNames[Key];
	}else{
		return m_name.c_str();
	}
}

const char *KeyPress::name() const
{
	if (Key && Key < irr::KEY_KEY_CODES_COUNT) {
		return KeyNamesLang[Key];
	}else{
		return m_name.c_str();
	}
}

const std::wstring KeyPress::guiName() const
{
	const char* n = name();
	return narrow_to_wide(n);
}

const KeyPress EscapeKey("KEY_ESCAPE");
const KeyPress CancelKey("KEY_CANCEL");
const KeyPress NumberKey[] = {
	KeyPress("KEY_KEY_0"), KeyPress("KEY_KEY_1"), KeyPress("KEY_KEY_2"),
	KeyPress("KEY_KEY_3"), KeyPress("KEY_KEY_4"), KeyPress("KEY_KEY_5"),
	KeyPress("KEY_KEY_6"), KeyPress("KEY_KEY_7"), KeyPress("KEY_KEY_8"),
	KeyPress("KEY_KEY_9")};

/*
	Key config
*/

static const char* keymap_strings[] = {
	"keymap_forward",
	"keymap_backward",
	"keymap_left",
	"keymap_right",
	"keymap_jump",
	"keymap_sneak",
	"keymap_inventory",
	"keymap_use",
	"keymap_chat",
	"keymap_cmd",
	"keymap_rangeselect",
	"keymap_freemove",
	"keymap_up",
	"keymap_down",
	"keymap_run",
	"keymap_examine",
	"keymap_screenshot",
	"keymap_toggle_hud",
	"keymap_toggle_chat",
	"keymap_toggle_force_fog_off",
	"keymap_toggle_update_camera",
	"keymap_toggle_debug",
	"keymap_toggle_profiler",
	"keymap_increase_viewing_range_min",
	"keymap_decrease_viewing_range_min",
	"keymap_print_debug_stacks",
	"keymap_select_prev",
	"keymap_select_next",
	NULL
};

KeyPress *g_key_setting_cache[256];
static bool key_setting_cache_init = false;

KeyPress getKeySetting(KeyCode code)
{
	if (!key_setting_cache_init)
		clearKeyCache();
	if (g_key_setting_cache[code])
		return *g_key_setting_cache[code];

	g_key_setting_cache[code] = new KeyPress(g_settings->get(keymap_strings[code]).c_str());

	return *g_key_setting_cache[code];
}

void saveKeySetting(KeyPress &key, KeyCode code)
{
	if (!key_setting_cache_init)
		clearKeyCache();

	*g_key_setting_cache[code] = key;
	g_settings->set(keymap_strings[code],key.sym());
}

void clearKeyCache()
{
	if (key_setting_cache_init) {
		for (int i=0; i<VLKC_MAX; i++) {
			if (g_key_setting_cache[i] == NULL)
				continue;
			delete g_key_setting_cache[i];
			g_key_setting_cache[i] = NULL;
		}
	}else{
		for (int i=0; i<VLKC_MAX; i++) {
			g_key_setting_cache[i] = NULL;
		}
	}
	key_setting_cache_init = true;
}
