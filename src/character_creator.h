/************************************************************************
* character_creator.h
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

#include "common_irrlicht.h"
#include "modalMenu.h"
#include "utility.h"
#include <string>
#include <map>
#include "main.h"
#include "mesh.h"
// For IGameCallback
#include "guiPauseMenu.h"
#include "player.h"

using namespace irr;

enum {
	GUI_ID_CD_QUIT_BUTTON = 101,
	GUI_ID_CD_SAVE_BUTTON,
	GUI_ID_CD_MALE_BUTTON,
	GUI_ID_CD_FEMALE_BUTTON,
	GUI_ID_CD_TALLER_BUTTON,
	GUI_ID_CD_SHORTER_BUTTON,
	GUI_ID_CD_WIDER_BUTTON,
	GUI_ID_CD_THINNER_BUTTON,
	GUI_ID_CD_SKINTONE_PREV_BUTTON,
	GUI_ID_CD_SKINTONE_NEXT_BUTTON,
	GUI_ID_CD_FACE_PREV_BUTTON,
	GUI_ID_CD_FACE_NEXT_BUTTON,
	GUI_ID_CD_HAIRTONE_PREV_BUTTON,
	GUI_ID_CD_HAIRTONE_NEXT_BUTTON,
	GUI_ID_CD_HAIR_PREV_BUTTON,
	GUI_ID_CD_HAIR_NEXT_BUTTON,
	GUI_ID_CD_EYES_PREV_BUTTON,
	GUI_ID_CD_EYES_NEXT_BUTTON,
	GUI_ID_CD_SHIRT_PREV_BUTTON,
	GUI_ID_CD_SHIRT_NEXT_BUTTON,
	GUI_ID_CD_PANTS_PREV_BUTTON,
	GUI_ID_CD_PANTS_NEXT_BUTTON,
	GUI_ID_CD_SHOES_PREV_BUTTON,
	GUI_ID_CD_SHOES_NEXT_BUTTON,
};

class GUICharDefMenu : public GUIModalMenu
{
public:
	GUICharDefMenu(
		IrrlichtDevice* device,
		gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id,
		IMenuManager *menumgr
	);
	~GUICharDefMenu();

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

	std::string getPlayerSkin();
	video::ITexture* getPlayerTexture()
	{
		std::string tex = getPlayerSkin();
		return g_texturesource->getTextureRaw(tex);
	}
	void fetchPlayerSkin();
	void savePlayerSkin();

	void printCharDef()
	{
		std::string chardef = m_parts["gender"];
		chardef += std::string(":")+m_parts["Yscale"];
		chardef += std::string(":")+m_parts["XZscale"];
		chardef += std::string(":")+m_parts["skintone"];
		chardef += std::string(":")+m_parts["eyes"];
		chardef += std::string(":")+m_parts["hairtone"];
		chardef += std::string(":")+m_parts["hair"];
		chardef += std::string(":")+m_parts["face"];
	}

private:
	bool m_accepted;

	gui::IGUIEnvironment* env;
	gui::IGUIElement* parent;
	s32 id;
	IMenuManager *menumgr;
	v2u32 m_screensize;

	scene::IAnimatedMeshSceneNode *m_model;
	scene::ICameraSceneNode* m_cameranode;
	f32 m_model_rotation;

	std::map<std::string,std::string> m_parts;
	f32 m_y_scale;
	f32 m_xz_scale;

	std::map<std::string,std::wstring> m_skintone_types;
	std::map<std::string,std::wstring> m_face_types;
	std::map<std::string,std::wstring> m_hairtone_types;
	std::map<std::string,std::wstring> m_hair_types;
	std::map<std::string,std::wstring> m_eyes_types;

	std::map<std::string,std::wstring> m_shirt_types;
	std::map<std::string,std::wstring> m_pants_types;
	std::map<std::string,std::wstring> m_shoes_types;
};
