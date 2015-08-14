/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* guiMainMenu.h
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

#ifndef GUIMAINMENU_HEADER
#define GUIMAINMENU_HEADER

#include "common_irrlicht.h"
#include "modalMenu.h"
#include "utility.h"
#include <string>
// For IGameCallback
#include "guiPauseMenu.h"

enum {
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NAME_INPUT,
	GUI_ID_PW_INPUT,
	GUI_ID_ADDRESS_INPUT,
	GUI_ID_PORT_INPUT,
	GUI_ID_GAME_MODE_COMBO,
	GUI_ID_GAME_MODE_CREATIVE,
	GUI_ID_GAME_MODE_ADVENTURE,
	GUI_ID_GAME_MODE_SURVIVAL,
	GUI_ID_GAME_SETTINGS_ADV,
	GUI_ID_GAME_SETTINGS_BASIC,
	GUI_ID_MOBS_COMBO,
	GUI_ID_MOBS_NONE,
	GUI_ID_MOBS_PASSIVE,
	GUI_ID_MOBS_AGGRESSIVE,
	GUI_ID_MOBS_DESTRUCTIVE,
	GUI_ID_INITIAL_INV_CB,
	GUI_ID_INFINITE_INV_CB,
	GUI_ID_DROPPABLE_INV_CB,
	GUI_ID_DAMAGE_CB,
	GUI_ID_TOOL_WEAR_CB,
	GUI_ID_FIRE_CB,
	GUI_ID_SUFFOCATE_CB,
	GUI_ID_HUNGER_CB,
	GUI_ID_JOIN_GAME_BUTTON,
	GUI_ID_CHANGE_KEYS_BUTTON,
	GUI_ID_MAP_OPTIONS_BUTTON,
	GUI_ID_MAP_DELETE_CB,
	GUI_ID_MAP_CLEAR_CB,
	GUI_ID_MAP_SEED_CB,
	GUI_ID_MAP_SEED_INPUT,
	GUI_ID_MAP_TYPE_COMBO,
	GUI_ID_MAP_TYPE_FLAT,
	GUI_ID_MAP_TYPE_FLATTER,
	GUI_ID_MAP_TYPE_SMOOTHER,
	GUI_ID_MAP_TYPE_DEFAULT,
	GUI_ID_MAP_TYPE_HILLY,
	GUI_ID_MAP_TYPE_MOUNTAINS,
	GUI_ID_MAP_TYPE_CRAZY,
	GUI_ID_MAP_TYPE_CRAZYHILLS,
	GUI_ID_CHARACTER_CREATOR,
	GUI_ID_TAB_SINGLEPLAYER,
	GUI_ID_TAB_MULTIPLAYER,
	GUI_ID_TAB_SETTINGS,
	GUI_ID_TAB_CREDITS,
	GUI_ID_TAB_QUIT
};

enum {
	TAB_SINGLEPLAYER=0,
	TAB_SINGLEPLAYER_ADVANCED,
	TAB_SINGLEPLAYER_MAP,
	TAB_MULTIPLAYER,
	TAB_SETTINGS,
	TAB_CREDITS
};

struct MainMenuData
{
	MainMenuData():
		// Generic
		selected_tab(0),
		// Server opts
		game_mode(L"adventure"),
		max_mob_level(L"aggressive"),
		initial_inventory(true),
		infinite_inventory(false),
		droppable_inventory(true),
		enable_damage(true),
		suffocation(false),
		hunger(false),
		tool_wear(true),
		unsafe_fire(false),
		// Actions
		delete_map(false),
		clear_map(false),
		use_fixed_seed(false),
		fixed_seed(L""),
		map_type("default"),
		character_creator(false)
	{}

	// These are in the native format of the gui elements

	// Generic
	int selected_tab;
	// Client options
	std::wstring address;
	std::wstring port;
	std::wstring name;
	std::wstring password;
	// Server options
	std::wstring game_mode;
	std::wstring max_mob_level;
	bool initial_inventory;
	bool infinite_inventory;
	bool droppable_inventory;
	bool enable_damage;
	bool suffocation;
	bool hunger;
	bool tool_wear;
	bool unsafe_fire;
	// Map options
	bool delete_map;
	bool clear_map;
	bool use_fixed_seed;
	std::wstring fixed_seed;
	std:: string map_type;
	// go to character creator, not the game
	bool character_creator;
};

class ISoundManager;

class GUIMainMenu : public GUIModalMenu
{
public:
	GUIMainMenu(gui::IGUIEnvironment* env,
			gui::IGUIElement* parent, s32 id,
			IMenuManager *menumgr,
			MainMenuData *data,
			IGameCallback *gamecallback,
			ISoundManager *sound);
	~GUIMainMenu();

	void removeChildren();
	/*
		Remove and re-add (or reposition) stuff
	*/
	void regenerateGui(v2u32 screensize);

	void drawMenu();

	void acceptInput();

	bool getStatus()
	{
		return m_accepted;
	}

	bool OnEvent(const SEvent& event);

	int getTab()
	{
		return m_data->selected_tab;
	}

private:
	MainMenuData *m_data;
	bool m_accepted;
	IGameCallback *m_gamecallback;
	ISoundManager *m_sound;

	gui::IGUIEnvironment* env;
	gui::IGUIElement* parent;
	s32 id;
	IMenuManager *menumgr;
	v2u32 m_screensize;
};

#endif
