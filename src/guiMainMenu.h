/*
Minetest-c55
Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
	GUI_ID_ADDRESS_INPUT,
	GUI_ID_PORT_INPUT,
	GUI_ID_FANCYTREE_CB,
	GUI_ID_SMOOTH_LIGHTING_CB,
	GUI_ID_3D_CLOUDS_CB,
	GUI_ID_OPAQUE_WATER_CB,
	GUI_ID_MIPMAP_CB,
	GUI_ID_BILINEAR_CB,
	GUI_ID_TRILINEAR_CB,
	GUI_ID_ANISOTROPIC_CB,
	GUI_ID_PARTICLES_CB,
	GUI_ID_FULLSCREEN_CB,
	GUI_ID_GAME_MODE_COMBO,
	GUI_ID_GAME_MODE_CREATIVE,
	GUI_ID_GAME_MODE_ADVENTURE,
	GUI_ID_GAME_MODE_SURVIVAL,
	GUI_ID_JOIN_GAME_BUTTON,
	GUI_ID_CHANGE_KEYS_BUTTON,
	GUI_ID_DELETE_MAP_BUTTON,
	GUI_ID_TAB_SINGLEPLAYER,
	GUI_ID_TAB_MULTIPLAYER,
	GUI_ID_TAB_SETTINGS,
	GUI_ID_TAB_CREDITS,
	GUI_ID_TAB_QUIT
};

enum {
	TAB_SINGLEPLAYER=0,
	TAB_MULTIPLAYER,
	TAB_SETTINGS,
	TAB_CREDITS
};

struct MainMenuData
{
	MainMenuData():
		// Generic
		selected_tab(0),
		// Client opts
		fancy_trees(false),
		smooth_lighting(false),
		particles(true),
		fullscreen(false),
		// Server opts
		game_mode(L"adventure"),
		// Actions
		delete_map(false)
	{}

	// These are in the native format of the gui elements

	// Generic
	int selected_tab;
	// Client options
	std::wstring address;
	std::wstring port;
	std::wstring name;
	std::wstring password;
	bool fancy_trees;
	bool smooth_lighting;
	bool clouds_3d;
	bool opaque_water;
	bool mip_map;
	bool anisotropic_filter;
	bool bilinear_filter;
	bool trilinear_filter;
	//int enable_shaders;
	bool particles;
	bool fullscreen;
	// Server options
	std::wstring game_mode;
	// If map deletion is requested, this is set to true
	bool delete_map;
};

class GUIMainMenu : public GUIModalMenu
{
public:
	GUIMainMenu(gui::IGUIEnvironment* env,
			gui::IGUIElement* parent, s32 id,
			IMenuManager *menumgr,
			MainMenuData *data,
			IGameCallback *gamecallback);
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

	gui::IGUIEnvironment* env;
	gui::IGUIElement* parent;
	s32 id;
	IMenuManager *menumgr;
	v2u32 m_screensize;
};

#endif

