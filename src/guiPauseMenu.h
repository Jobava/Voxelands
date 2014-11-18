/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* guiPauseMenu.h
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

#ifndef GUIPAUSEMENU_HEADER
#define GUIPAUSEMENU_HEADER

#include "common_irrlicht.h"
#include "modalMenu.h"

class IGameCallback
{
public:
	virtual void exitToOS() = 0;
	virtual void disconnect() = 0;
	virtual void changePassword() = 0;
};

class GUIPauseMenu : public GUIModalMenu
{
public:
	GUIPauseMenu(gui::IGUIEnvironment* env,
			gui::IGUIElement* parent, s32 id,
			IGameCallback *gamecallback,
			IMenuManager *menumgr);
	~GUIPauseMenu();

	void removeChildren();
	/*
		Remove and re-add (or reposition) stuff
	*/
	void regenerateGui(v2u32 screensize);

	void drawMenu();

	bool OnEvent(const SEvent& event);

private:
	IGameCallback *m_gamecallback;
};

#endif

