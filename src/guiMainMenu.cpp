/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* guiMainMenu.cpp
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

#include "guiMainMenu.h"
#include "settings.h"
#include "main.h"
#include "defaultsettings.h"
#include "guiSettingsMenu.h"
#include "debug.h"
#include "serialization.h"
#include <string>
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIComboBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include "path.h"
#include "porting.h"
#include "gui_colours.h"
#if USE_FREETYPE
#include "intlGUIEditBox.h"
#endif
#include "sound.h"

#include "intl.h"

GUIMainMenu::GUIMainMenu(gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id,
		IMenuManager *menumgr,
		MainMenuData *data,
		IGameCallback *gamecallback,
		ISoundManager *sound
):
	GUIModalMenu(env, parent, id, menumgr),
	m_data(data),
	m_accepted(false),
	m_gamecallback(gamecallback),
	m_sound(sound)
{
	assert(m_data);
	this->env = env;
	this->parent = parent;
	this->id = id;
	this->menumgr = menumgr;
}

GUIMainMenu::~GUIMainMenu()
{
	removeChildren();
}

void GUIMainMenu::removeChildren()
{
	const core::list<gui::IGUIElement*> &children = getChildren();
	core::list<gui::IGUIElement*> children_copy;
	for (core::list<gui::IGUIElement*>::ConstIterator i = children.begin(); i != children.end(); i++) {
		children_copy.push_back(*i);
	}
	for (core::list<gui::IGUIElement*>::Iterator i = children_copy.begin(); i != children_copy.end(); i++) {
		(*i)->remove();
	}
}

void GUIMainMenu::regenerateGui(v2u32 screensize)
{
	std::wstring text_name;
	std::wstring text_address;
	std::wstring text_port;
	std::wstring game_mode;

	std::wstring max_mob_level;
	bool initial_inventory;
	bool infinite_inventory;
	bool droppable_inventory;
	bool death_drops_inventory;
	bool enable_damage;
	bool suffocation;
	bool hunger;
	bool tool_wear;
	bool unsafe_fire;

	bool delete_map;
	bool clear_map;
	bool use_fixed_seed;
	std::wstring fixed_seed;
	std::string map_type;

	m_screensize = screensize;

	// Client options
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_NAME_INPUT);
		if (e != NULL) {
			text_name = e->getText();
		}else{
			text_name = m_data->name;
		}
		if (text_name == L"")
			text_name = narrow_to_wide(porting::getUser());
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_ADDRESS_INPUT);
		if(e != NULL)
			text_address = e->getText();
		else
			text_address = m_data->address;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_PORT_INPUT);
		if(e != NULL)
			text_port = e->getText();
		else
			text_port = m_data->port;
	}

	// Server options
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_GAME_MODE_COMBO);
		if (e != NULL && e->getType() == gui::EGUIET_COMBO_BOX) {
			gui::IGUIComboBox *c = (gui::IGUIComboBox*)e;
			switch (c->getItemData(c->getSelected())) {
			case GUI_ID_GAME_MODE_CREATIVE:
				game_mode = L"creative";
				break;
			case GUI_ID_GAME_MODE_ADVENTURE:
				game_mode = L"adventure";
				break;
			case GUI_ID_GAME_MODE_SURVIVAL:
				game_mode = L"survival";
				break;
			default:
				game_mode = L"adventure";
			}
		}else{
			game_mode = m_data->game_mode;
		}
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MOBS_COMBO);
		if (e != NULL && e->getType() == gui::EGUIET_COMBO_BOX) {
			gui::IGUIComboBox *c = (gui::IGUIComboBox*)e;
			switch (c->getItemData(c->getSelected())) {
			case GUI_ID_MOBS_PASSIVE:
				max_mob_level = L"passive";
				break;
			case GUI_ID_MOBS_AGGRESSIVE:
				max_mob_level = L"aggressive";
				break;
			case GUI_ID_MOBS_DESTRUCTIVE:
				max_mob_level = L"destructive";
				break;
			case GUI_ID_MOBS_NONE:
				max_mob_level = L"none";
				break;
			default:
				max_mob_level = L"aggressive";
			}
		}else{
			max_mob_level = m_data->max_mob_level;
		}
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_DAMAGE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			enable_damage = ((gui::IGUICheckBox*)e)->isChecked();
		else
			enable_damage = m_data->enable_damage;
	}
	if (enable_damage) {
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_SUFFOCATE_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				suffocation = ((gui::IGUICheckBox*)e)->isChecked();
			else
				suffocation = m_data->suffocation;
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_HUNGER_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				hunger = ((gui::IGUICheckBox*)e)->isChecked();
			else
				hunger = m_data->hunger;
		}
	}else{
		suffocation = false;
		m_data->suffocation = false;
		hunger = false;
		m_data->hunger = false;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_INFINITE_INV_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			infinite_inventory = ((gui::IGUICheckBox*)e)->isChecked();
		else
			infinite_inventory = m_data->infinite_inventory;
	}
	if (!infinite_inventory) {
		gui::IGUIElement *e = getElementFromId(GUI_ID_INITIAL_INV_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			initial_inventory = ((gui::IGUICheckBox*)e)->isChecked();
		else
			initial_inventory = m_data->initial_inventory;
	}else{
		initial_inventory = false;
		m_data->initial_inventory = false;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_DROPPABLE_INV_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			droppable_inventory = ((gui::IGUICheckBox*)e)->isChecked();
		else
			droppable_inventory = m_data->droppable_inventory;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_LOSE_INV_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			death_drops_inventory = ((gui::IGUICheckBox*)e)->isChecked();
		else
			death_drops_inventory = m_data->death_drops_inventory;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_TOOL_WEAR_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			tool_wear = ((gui::IGUICheckBox*)e)->isChecked();
		else
			tool_wear = m_data->tool_wear;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_FIRE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			unsafe_fire = ((gui::IGUICheckBox*)e)->isChecked();
		else
			unsafe_fire = m_data->unsafe_fire;
	}

	// Map options
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_DELETE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			delete_map = ((gui::IGUICheckBox*)e)->isChecked();
		else
			delete_map = m_data->delete_map;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_CLEAR_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			clear_map = ((gui::IGUICheckBox*)e)->isChecked();
		else
			clear_map = m_data->clear_map;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_SEED_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			use_fixed_seed = ((gui::IGUICheckBox*)e)->isChecked();
		else
			use_fixed_seed = m_data->use_fixed_seed;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_SEED_INPUT);
		if(e != NULL)
			fixed_seed = e->getText();
		else
			fixed_seed = m_data->fixed_seed;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_TYPE_COMBO);
		if (e != NULL && e->getType() == gui::EGUIET_COMBO_BOX) {
			gui::IGUIComboBox *c = (gui::IGUIComboBox*)e;
			switch (c->getItemData(c->getSelected())) {
			case GUI_ID_MAP_TYPE_FLAT:
				map_type = "flat";
				break;
			case GUI_ID_MAP_TYPE_FLATTER:
				map_type = "flatter";
				break;
			case GUI_ID_MAP_TYPE_SMOOTHER:
				map_type = "smoother";
				break;
			case GUI_ID_MAP_TYPE_HILLY:
				map_type = "hilly";
				break;
			case GUI_ID_MAP_TYPE_MOUNTAINS:
				map_type = "mountains";
				break;
			case GUI_ID_MAP_TYPE_CRAZY:
				map_type = "crazy";
				break;
			case GUI_ID_MAP_TYPE_CRAZYHILLS:
				map_type = "crazyhills";
				break;
			default:
				map_type = "default";
			}
		}else{
			map_type = m_data->map_type;
		}
	}

	/*
		Remove stuff
	*/
	removeChildren();

	/*
		Calculate new sizes and positions
	*/

	v2s32 size(800, 500);

	core::rect<s32> rect(
			screensize.X/2 - size.X/2,
			screensize.Y/2 - size.Y/2,
			screensize.X/2 + size.X/2,
			screensize.Y/2 + size.Y/2
	);

	DesiredRect = rect;
	recalculateAbsolutePosition(false);

	// Character Creator button
	{
		core::rect<s32> rect(0, 0, 200, 40);
		rect += v2s32(25, 200);
		Environment->addButton(rect, this, GUI_ID_CHARACTER_CREATOR, wgettext("Character Creator"));
	}
	// Single Player button
	{
		core::rect<s32> rect(0, 0, 200, 40);
		rect += v2s32(25, 260);
		Environment->addButton(rect, this, GUI_ID_TAB_SINGLEPLAYER, wgettext("Single Player"));
	}
	// Multi Player button
	{
		core::rect<s32> rect(0, 0, 200, 40);
		rect += v2s32(25, 305);
		Environment->addButton(rect, this, GUI_ID_TAB_MULTIPLAYER, wgettext("Multi Player"));
	}
	// Settings button
	{
		core::rect<s32> rect(0, 0, 180, 40);
		rect += v2s32(35, 350);
		Environment->addButton(rect, this, GUI_ID_TAB_SETTINGS, wgettext("Settings"));
	}
	// Credits button
	{
		core::rect<s32> rect(0, 0, 180, 40);
		rect += v2s32(35, 395);
		Environment->addButton(rect, this, GUI_ID_TAB_CREDITS, wgettext("Credits"));
	}
	// Quit button
	{
		core::rect<s32> rect(0, 0, 180, 40);
		rect += v2s32(35, 440);
		Environment->addButton(rect, this, GUI_ID_TAB_QUIT, wgettext("Quit"));
	}

	v2s32 topleft_content(250, 0);

	//v2s32 size = rect.getSize();

	/*
		Add stuff
	*/

	/*
		Client section
	*/
	std::string selected_tab = g_settings->get("mainmenu_tab");
	if (selected_tab == "multiplayer") {
		m_data->selected_tab = TAB_MULTIPLAYER;
	}else if (selected_tab == "settings") {
		m_data->selected_tab = TAB_SETTINGS;
	}else if (selected_tab == "credits") {
		m_data->selected_tab = TAB_CREDITS;
	}else if (selected_tab == "singleadvanced") {
		m_data->selected_tab = TAB_SINGLEPLAYER_ADVANCED;
	}else if (selected_tab == "singlemap") {
		m_data->selected_tab = TAB_SINGLEPLAYER_MAP;
	}else{
		m_data->selected_tab = TAB_SINGLEPLAYER;
	}

	if (m_data->selected_tab == TAB_MULTIPLAYER) {
		{
			core::rect<s32> rect(0, 0, 550, 20);
			rect += topleft_content + v2s32(0, 20);
			gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Multi Player"), rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}

		// Nickname + password
		{
			core::rect<s32> rect(0, 0, 110, 20);
			rect += topleft_content + v2s32(120, 60);
			Environment->addStaticText(wgettext("Name/Password"), rect, false, true, this, -1);
		}
		{
			core::rect<s32> rect(0, 0, 230, 30);
			rect += topleft_content + v2s32(135, 90);
#if USE_FREETYPE
			new gui::intlGUIEditBox(text_name.c_str(), true, Environment, this, GUI_ID_NAME_INPUT, rect);
#else
			Environment->addEditBox(text_name.c_str(), rect, false, this, GUI_ID_NAME_INPUT);
#endif
		}
		{
			core::rect<s32> rect(0, 0, 230, 30);
			rect += topleft_content + v2s32(135, 125);
			gui::IGUIEditBox *e;
#if USE_FREETYPE
			e = (gui::IGUIEditBox *) new gui::intlGUIEditBox(L"", true, Environment, this, GUI_ID_PW_INPUT, rect);
#else
			e = Environment->addEditBox(L"", rect, false, this, GUI_ID_PW_INPUT);
#endif
			e->setPasswordBox(true);
			Environment->setFocus(e);

		}
		// Address + port
		{
			core::rect<s32> rect(0, 0, 110, 20);
			rect += topleft_content + v2s32(120, 170);
			Environment->addStaticText(wgettext("Address/Port"), rect, false, true, this, -1);
		}
		{
			if (text_address == L"")
				text_address = L"servers.voxelands.com";
			core::rect<s32> rect(0, 0, 230, 30);
			rect += topleft_content + v2s32(135, 200);
#if USE_FREETYPE
			new gui::intlGUIEditBox(text_address.c_str(), true, Environment, this, GUI_ID_ADDRESS_INPUT, rect);
#else
			Environment->addEditBox(text_address.c_str(), rect, false, this, GUI_ID_ADDRESS_INPUT);
#endif
		}
		{
			core::rect<s32> rect(0, 0, 120, 30);
			rect += topleft_content + v2s32(245, 240);
#if USE_FREETYPE
			new gui::intlGUIEditBox(text_port.c_str(), true, Environment, this, GUI_ID_PORT_INPUT, rect);
#else
			Environment->addEditBox(text_port.c_str(), rect, false, this, GUI_ID_PORT_INPUT);
#endif
		}
		// Start game button
		{
			core::rect<s32> rect(0, 0, 180, 30);
			rect += topleft_content + v2s32(160, 290);
			Environment->addButton(rect, this, GUI_ID_JOIN_GAME_BUTTON, wgettext("Connect"));
		}
	}else if (m_data->selected_tab == TAB_SINGLEPLAYER) {
		{
			core::rect<s32> rect(0, 0, 550, 20);
			rect += topleft_content + v2s32(0, 20);
			gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Single Player"), rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}

		// Server parameters
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(175, 60);
			gui::IGUIComboBox *c = Environment->addComboBox(rect, this, GUI_ID_GAME_MODE_COMBO);
			u32 cm = c->addItem(wgettext("Creative Mode"),GUI_ID_GAME_MODE_CREATIVE);
			u32 am = c->addItem(wgettext("Adventure Mode"),GUI_ID_GAME_MODE_ADVENTURE);
			u32 sm = c->addItem(wgettext("Survival Mode"),GUI_ID_GAME_MODE_SURVIVAL);
			if (game_mode == L"creative") {
				c->setSelected(cm);
			}else if (game_mode == L"adventure") {
				c->setSelected(am);
			}else if (game_mode == L"survival") {
				c->setSelected(sm);
			}
		}
		// Advanced settings button
		{
			core::rect<s32> rect(0, 0, 220, 30);
			rect += topleft_content + v2s32(50, 100);
			Environment->addButton(rect, this, GUI_ID_GAME_SETTINGS_ADV, wgettext("Advanced Settings"));
		}
		// Map options button
		{
			core::rect<s32> rect(0, 0, 220, 30);
			rect += topleft_content + v2s32(280, 100);
			Environment->addButton(rect, this, GUI_ID_MAP_OPTIONS_BUTTON, wgettext("Map Options"));
		}
		// Start game button
		{
			core::rect<s32> rect(0, 0, 180, 30);
			rect += topleft_content + v2s32(185, 170);
			Environment->addButton(rect, this, GUI_ID_JOIN_GAME_BUTTON, wgettext("Start Game"));
		}
	}else if (m_data->selected_tab == TAB_SINGLEPLAYER_ADVANCED) {
		{
			core::rect<s32> rect(0, 0, 550, 20);
			rect += topleft_content + v2s32(0, 20);
			gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Single Player"), rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}

		// Server parameters
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(175, 60);
			gui::IGUIComboBox *c = Environment->addComboBox(rect, this, GUI_ID_GAME_MODE_COMBO);
			u32 cm = c->addItem(wgettext("Creative Mode"),GUI_ID_GAME_MODE_CREATIVE);
			u32 am = c->addItem(wgettext("Adventure Mode"),GUI_ID_GAME_MODE_ADVENTURE);
			u32 sm = c->addItem(wgettext("Survival Mode"),GUI_ID_GAME_MODE_SURVIVAL);
			if (game_mode == L"creative") {
				c->setSelected(cm);
			}else if (game_mode == L"adventure") {
				c->setSelected(am);
			}else if (game_mode == L"survival") {
				c->setSelected(sm);
			}
		}
		// Basic settings button
		{
			core::rect<s32> rect(0, 0, 220, 30);
			rect += topleft_content + v2s32(50, 100);
			Environment->addButton(rect, this, GUI_ID_GAME_SETTINGS_BASIC, wgettext("Hide Advanced Settings"));
		}
		// Map options button
		{
			core::rect<s32> rect(0, 0, 220, 30);
			rect += topleft_content + v2s32(280, 100);
			Environment->addButton(rect, this, GUI_ID_MAP_OPTIONS_BUTTON, wgettext("Map Options"));
		}
		{
			core::rect<s32> rect(0, 0, 125, 20);
			rect += topleft_content + v2s32(40, 160);
			gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Creatures"), rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
		}
		{
			core::rect<s32> rect(0, 0, 240, 30);
			rect += topleft_content + v2s32(170, 155);
			gui::IGUIComboBox *c = Environment->addComboBox(rect, this, GUI_ID_MOBS_COMBO);
			u32 nm = c->addItem(wgettext("None"),GUI_ID_MOBS_NONE);
			u32 pm = c->addItem(wgettext("Passive"),GUI_ID_MOBS_PASSIVE);
			u32 am = c->addItem(wgettext("Passive & Aggressive"),GUI_ID_MOBS_AGGRESSIVE);
			u32 dm = c->addItem(wgettext("Passive, Aggressive, & Destructive"),GUI_ID_MOBS_DESTRUCTIVE);
			if (max_mob_level == L"passive") {
				c->setSelected(pm);
			}else if (max_mob_level == L"destructive") {
				c->setSelected(dm);
			}else if (max_mob_level == L"none") {
				c->setSelected(nm);
			}else{
				c->setSelected(am);
			}
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(70, 200);
			Environment->addCheckBox(enable_damage, rect, this, GUI_ID_DAMAGE_CB, wgettext("Player Damage"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(80, 230);
			Environment->addCheckBox(suffocation, rect, this, GUI_ID_SUFFOCATE_CB, wgettext("Suffocation/Drowning"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(80, 260);
			Environment->addCheckBox(hunger, rect, this, GUI_ID_HUNGER_CB, wgettext("Hunger"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(70, 290);
			Environment->addCheckBox(tool_wear, rect, this, GUI_ID_TOOL_WEAR_CB, wgettext("Tool Wear"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(70, 320);
			Environment->addCheckBox(unsafe_fire, rect, this, GUI_ID_FIRE_CB, wgettext("Dangerous Fire"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(300, 200);
			Environment->addCheckBox(infinite_inventory, rect, this, GUI_ID_INFINITE_INV_CB, wgettext("Infinite Inventory"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(310, 230);
			Environment->addCheckBox(initial_inventory, rect, this, GUI_ID_INITIAL_INV_CB, wgettext("Initial Inventory"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(300, 260);
			Environment->addCheckBox(droppable_inventory, rect, this, GUI_ID_DROPPABLE_INV_CB, wgettext("Droppable Inventory"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(300, 290);
			Environment->addCheckBox(death_drops_inventory, rect, this, GUI_ID_LOSE_INV_CB, wgettext("Death drops Inventory"));
		}
		// Start game button
		{
			core::rect<s32> rect(0, 0, 180, 30);
			rect += topleft_content + v2s32(185, 370);
			Environment->addButton(rect, this, GUI_ID_JOIN_GAME_BUTTON, wgettext("Start Game"));
		}
	}else if (m_data->selected_tab == TAB_SINGLEPLAYER_MAP) {
		{
			core::rect<s32> rect(0, 0, 550, 20);
			rect += topleft_content + v2s32(0, 20);
			gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Single Player"), rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}

		// Server parameters
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(175, 60);
			gui::IGUIComboBox *c = Environment->addComboBox(rect, this, GUI_ID_GAME_MODE_COMBO);
			u32 cm = c->addItem(wgettext("Creative Mode"),GUI_ID_GAME_MODE_CREATIVE);
			u32 am = c->addItem(wgettext("Adventure Mode"),GUI_ID_GAME_MODE_ADVENTURE);
			u32 sm = c->addItem(wgettext("Survival Mode"),GUI_ID_GAME_MODE_SURVIVAL);
			if (game_mode == L"creative") {
				c->setSelected(cm);
			}else if (game_mode == L"adventure") {
				c->setSelected(am);
			}else if (game_mode == L"survival") {
				c->setSelected(sm);
			}
		}
		// Advanced settings button
		{
			core::rect<s32> rect(0, 0, 220, 30);
			rect += topleft_content + v2s32(50, 100);
			Environment->addButton(rect, this, GUI_ID_GAME_SETTINGS_ADV, wgettext("Advanced Settings"));
		}
		// Basic settings button
		{
			core::rect<s32> rect(0, 0, 220, 30);
			rect += topleft_content + v2s32(280, 100);
			Environment->addButton(rect, this, GUI_ID_GAME_SETTINGS_BASIC, wgettext("Hide Map Options"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(150, 130);
			Environment->addCheckBox(delete_map, rect, this, GUI_ID_MAP_DELETE_CB, wgettext("Create New Map"));
		}
		if (delete_map) {
			{
				core::rect<s32> rect(0, 0, 300, 20);
				rect += topleft_content + v2s32(120, 160);
				Environment->addStaticText(wgettext("Warning! Your old map will be deleted!"),
					rect, false, true, this, -1);
			}
			{
				core::rect<s32> rect(0, 0, 120, 20);
				rect += topleft_content + v2s32(45, 195);
				gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Map Type"), rect, false, true, this, -1);
				t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
			}
			{
				core::rect<s32> rect(0, 0, 240, 30);
				rect += topleft_content + v2s32(170, 190);
				gui::IGUIComboBox *c = Environment->addComboBox(rect, this, GUI_ID_MAP_TYPE_COMBO);
				u32 m1 = c->addItem(wgettext("Flat"),GUI_ID_MAP_TYPE_FLAT);
				u32 m2 = c->addItem(wgettext("Flatter"),GUI_ID_MAP_TYPE_FLATTER);
				u32 m3 = c->addItem(wgettext("Smoother"),GUI_ID_MAP_TYPE_SMOOTHER);
				u32 m4 = c->addItem(wgettext("Default"),GUI_ID_MAP_TYPE_DEFAULT);
				u32 m5 = c->addItem(wgettext("Hilly"),GUI_ID_MAP_TYPE_HILLY);
				u32 m6 = c->addItem(wgettext("Mountains"),GUI_ID_MAP_TYPE_MOUNTAINS);
				u32 m7 = c->addItem(wgettext("Crazy"),GUI_ID_MAP_TYPE_CRAZY);
				u32 m8 = c->addItem(wgettext("Crazy Hills"),GUI_ID_MAP_TYPE_CRAZYHILLS);
				if (map_type == "flat") {
					c->setSelected(m1);
				}else if (map_type == "flatter") {
					c->setSelected(m2);
				}else if (map_type == "smoother") {
					c->setSelected(m3);
				}else if (map_type == "hilly") {
					c->setSelected(m5);
				}else if (map_type == "mountains") {
					c->setSelected(m6);
				}else if (map_type == "crazy") {
					c->setSelected(m7);
				}else if (map_type == "crazyhills") {
					c->setSelected(m8);
				}else{
					c->setSelected(m4);
				}
			}
			{
				core::rect<s32> rect(0, 0, 200, 30);
				rect += topleft_content + v2s32(150, 230);
				Environment->addCheckBox(use_fixed_seed, rect, this, GUI_ID_MAP_SEED_CB, wgettext("Use Fixed Seed"));
			}
			if (use_fixed_seed) {
				{
					core::rect<s32> rect(0, 0, 120, 20);
					rect += topleft_content + v2s32(65, 265);
					gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Map Seed"), rect, false, true, this, -1);
					t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
				}
				{
					core::rect<s32> rect(0, 0, 190, 30);
					rect += topleft_content + v2s32(190, 260);
#if USE_FREETYPE
					new gui::intlGUIEditBox(fixed_seed.c_str(), true, Environment, this, GUI_ID_MAP_SEED_INPUT, rect);
#else
					Environment->addEditBox(fixed_seed.c_str(), rect, false, this, GUI_ID_MAP_SEED_INPUT);
#endif
				}
			}
		}else{
			{
				core::rect<s32> rect(0, 0, 200, 30);
				rect += topleft_content + v2s32(150, 160);
				Environment->addCheckBox(clear_map, rect, this, GUI_ID_MAP_CLEAR_CB, wgettext("Clear Map"));
			}
			if (clear_map) {
				core::rect<s32> rect(0, 0, 300, 40);
				rect += topleft_content + v2s32(120, 190);
				Environment->addStaticText(wgettext("Warning! This will delete all construction from your map!"),
						rect, false, true, this, -1);
			}
		}
		// Start game button
		{
			core::rect<s32> rect(0, 0, 180, 30);
			rect += topleft_content + v2s32(185, 310);
			Environment->addButton(rect, this, GUI_ID_JOIN_GAME_BUTTON, wgettext("Start Game"));
		}
	}else if(m_data->selected_tab == TAB_CREDITS) {
		// CREDITS
		{
			core::rect<s32> rect(0, 0, 550, 480);
			rect += topleft_content + v2s32(0, 10);
			std::string txt("");

			txt += gettext(
				"Voxelands\n"
				"http://www.voxelands.com/\n"
				"By Lisa 'darkrose' Milne <lisa@ltmnet.com> and contributors."
			);

			txt += "\n\n";

			txt += gettext("Programmers");
			txt += "\ndarkrose, sdzen, Pentium44, Menche, MavJS, tiemay, Tapoky, JHeaton, Wayward_One, Stephane, hdastwb, VargaD.\n\n";

			txt += gettext("Artists and Modellers");
			txt += "\ndarkrose, sdzen, Menche, Pentium44, Wuzzy, Carl 'melkior' Kidwell, Jordach, Gwinna, elky.\n\n";

			txt += gettext("Translators");
			txt += "\ndarkrose, Szkodnix, soognarf, Wuzzy, Akien, redhound, Rui, loh, wario, puma_rc, Uniaika.\n\n";

			txt += gettext("Documentation Writters");
			txt += "\ndarkrose, Menche, Pentium44, elky.\n\n";

			txt += gettext("Testers, Packagers, and Builders");
			txt += "\ndarkrose, sdzen, Menche, tiemay, JHeaton, MavJS, mcnalu.\n\n";

			txt += gettext("Music and Sound Effects Composers");
			txt += "\ndarkrose, Jordach, AudioRichter, OwlStorm, DjDust, Taira Komori.\n\n";

			txt += gettext("Other Contributers, and Special Thanks");
			txt += "\nnadnadnad, Honeypaw, tiemay, stormchaser3000, MichaelEh?, NCC74656.\n\n";

			txt += gettext(
				"Based on Minetest-C55 by Perttu Ahola <celeron55@gmail.com>\n"
				"and contributors: PilzAdam, Taoki, tango_, kahrl (kaaaaaahrl?), darkrose, matttpt, erlehmann,"
				" SpeedProg, JacobF, teddydestodes, marktraceur, Jonathan Neuschafer, thexyz, VanessaE, sfan5..."
				" and tens of more random people."
			);
			gui::IGUIStaticText *t = Environment->addStaticText(narrow_to_wide(txt).c_str(),rect,false,true,this,-1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}
	}
}

void GUIMainMenu::drawMenu()
{
	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;
	video::IVideoDriver* driver = Environment->getVideoDriver();

	{
		core::rect<s32> left(
			AbsoluteRect.UpperLeftCorner.X,
			AbsoluteRect.UpperLeftCorner.Y,
			AbsoluteRect.LowerRightCorner.X-550,
			AbsoluteRect.LowerRightCorner.Y
		);
		core::rect<s32> right(
			AbsoluteRect.UpperLeftCorner.X+250,
			AbsoluteRect.UpperLeftCorner.Y,
			AbsoluteRect.LowerRightCorner.X,
			AbsoluteRect.LowerRightCorner.Y
		);
		driver->draw2DRectangle(left, GUI_BG_BTM, GUI_BG_BTM, GUI_BG_BTM, GUI_BG_BTM, &AbsoluteClippingRect);
		driver->draw2DRectangle(right, GUI_BG_TOP, GUI_BG_BTM, GUI_BG_TOP, GUI_BG_BTM, &AbsoluteClippingRect);
		video::ITexture *texture = driver->getTexture(getTexturePath("menulogo.png").c_str());
		if (texture != 0) {
			const core::dimension2d<u32>& img_origsize = texture->getOriginalSize();
			core::rect<s32> logo(
				AbsoluteRect.UpperLeftCorner.X+25,
				AbsoluteRect.UpperLeftCorner.Y,
				AbsoluteRect.UpperLeftCorner.X+225,
				AbsoluteRect.UpperLeftCorner.Y+200
			);
			const video::SColor color(255,255,255,255);
			const video::SColor colors[] = {color,color,color,color};
			driver->draw2DImage(texture, logo, core::rect<s32>(core::position2d<s32>(0,0),img_origsize), NULL, colors, true);
		}
	}

	gui::IGUIElement::draw();
}

void GUIMainMenu::acceptInput()
{
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_NAME_INPUT);
		if (e != NULL) {
			m_data->name = e->getText();
		}else if (m_data->name == L"") {
			m_data->name = std::wstring(L"singleplayer");
		}
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_PW_INPUT);
		if(e != NULL)
			m_data->password = e->getText();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_ADDRESS_INPUT);
		if (e != NULL)
			m_data->address = e->getText();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_PORT_INPUT);
		if (e != NULL)
			m_data->port = e->getText();
	}
	std::wstring o_mode = m_data->game_mode;
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_GAME_MODE_COMBO);
		if (e != NULL && e->getType() == gui::EGUIET_COMBO_BOX) {
			gui::IGUIComboBox *c = (gui::IGUIComboBox*)e;
			switch (c->getItemData(c->getSelected())) {
			case GUI_ID_GAME_MODE_CREATIVE:
				m_data->game_mode = L"creative";
				break;
			case GUI_ID_GAME_MODE_ADVENTURE:
				m_data->game_mode = L"adventure";
				break;
			case GUI_ID_GAME_MODE_SURVIVAL:
				m_data->game_mode = L"survival";
				break;
			default:
				m_data->game_mode = L"adventure";
			}
		}
	}
	if (m_data->selected_tab == TAB_SINGLEPLAYER_ADVANCED && o_mode != m_data->game_mode) {
		GameSettings t_settings;
		if (m_data->game_mode == L"creative") {
			set_creative_defaults(&t_settings);
		}else if (m_data->game_mode == L"survival") {
			set_survival_defaults(&t_settings);
		}else{
			set_adventure_defaults(&t_settings);
		}
		m_data->max_mob_level = narrow_to_wide(t_settings.get("max_mob_level"));
		m_data->initial_inventory = t_settings.getBool("initial_inventory");
		m_data->infinite_inventory = t_settings.getBool("infinite_inventory");
		m_data->droppable_inventory = t_settings.getBool("droppable_inventory");
		m_data->enable_damage = t_settings.getBool("enable_damage");
		m_data->suffocation = t_settings.getBool("enable_suffocation");
		m_data->hunger = t_settings.getBool("enable_hunger");
		m_data->tool_wear = t_settings.getBool("tool_wear");
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_DAMAGE_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				((gui::IGUICheckBox*)e)->setChecked(m_data->enable_damage);
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_SUFFOCATE_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				((gui::IGUICheckBox*)e)->setChecked(m_data->suffocation);
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_HUNGER_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				((gui::IGUICheckBox*)e)->setChecked(m_data->hunger);
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_MOBS_COMBO);
			if(e != NULL && e->getType() == gui::EGUIET_COMBO_BOX) {
				gui::IGUIComboBox *c = (gui::IGUIComboBox*)e;
				s32 i;
				if (m_data->max_mob_level == L"passive") {
					i = c->getIndexForItemData(GUI_ID_MOBS_PASSIVE);
				}else if (m_data->max_mob_level == L"destructive") {
					i = c->getIndexForItemData(GUI_ID_MOBS_DESTRUCTIVE);
				}else{
					i = c->getIndexForItemData(GUI_ID_MOBS_AGGRESSIVE);
				}
				c->setSelected(i);
			}
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_INITIAL_INV_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				((gui::IGUICheckBox*)e)->setChecked(m_data->initial_inventory);
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_INFINITE_INV_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				((gui::IGUICheckBox*)e)->setChecked(m_data->infinite_inventory);
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_DROPPABLE_INV_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				((gui::IGUICheckBox*)e)->setChecked(m_data->droppable_inventory);
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_LOSE_INV_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				((gui::IGUICheckBox*)e)->setChecked(m_data->death_drops_inventory);
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_TOOL_WEAR_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				((gui::IGUICheckBox*)e)->setChecked(m_data->tool_wear);
		}
	}else{
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_MOBS_COMBO);
			if (e != NULL && e->getType() == gui::EGUIET_COMBO_BOX) {
				gui::IGUIComboBox *c = (gui::IGUIComboBox*)e;
				switch (c->getItemData(c->getSelected())) {
				case GUI_ID_MOBS_PASSIVE:
					m_data->max_mob_level = L"passive";
					break;
				case GUI_ID_MOBS_AGGRESSIVE:
					m_data->max_mob_level = L"aggressive";
					break;
				case GUI_ID_MOBS_DESTRUCTIVE:
					m_data->max_mob_level = L"destructive";
					break;
				default:
					m_data->max_mob_level = L"aggressive";
				}
			}
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_DAMAGE_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				m_data->enable_damage = ((gui::IGUICheckBox*)e)->isChecked();
		}
		if (m_data->enable_damage) {
			{
				gui::IGUIElement *e = getElementFromId(GUI_ID_SUFFOCATE_CB);
				if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
					m_data->suffocation = ((gui::IGUICheckBox*)e)->isChecked();
			}
			{
				gui::IGUIElement *e = getElementFromId(GUI_ID_HUNGER_CB);
				if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
					m_data->hunger = ((gui::IGUICheckBox*)e)->isChecked();
			}
		}else{
			{
				m_data->suffocation = false;
				gui::IGUIElement *e = getElementFromId(GUI_ID_SUFFOCATE_CB);
				if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
					((gui::IGUICheckBox*)e)->setChecked(false);
			}
			{
				m_data->hunger = false;
				gui::IGUIElement *e = getElementFromId(GUI_ID_HUNGER_CB);
				if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
					((gui::IGUICheckBox*)e)->setChecked(false);
			}
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_INFINITE_INV_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				m_data->infinite_inventory = ((gui::IGUICheckBox*)e)->isChecked();
		}
		if (!m_data->infinite_inventory) {
			{
				gui::IGUIElement *e = getElementFromId(GUI_ID_INITIAL_INV_CB);
				if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
					m_data->initial_inventory = ((gui::IGUICheckBox*)e)->isChecked();
			}
			{
				gui::IGUIElement *e = getElementFromId(GUI_ID_LOSE_INV_CB);
				if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
					m_data->death_drops_inventory = ((gui::IGUICheckBox*)e)->isChecked();
			}
		}else{
			{
				m_data->initial_inventory = false;
				gui::IGUIElement *e = getElementFromId(GUI_ID_INITIAL_INV_CB);
				if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
					((gui::IGUICheckBox*)e)->setChecked(false);
			}
			{
				m_data->death_drops_inventory = false;
				gui::IGUIElement *e = getElementFromId(GUI_ID_LOSE_INV_CB);
				if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
					((gui::IGUICheckBox*)e)->setChecked(false);
			}
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_DROPPABLE_INV_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				m_data->droppable_inventory = ((gui::IGUICheckBox*)e)->isChecked();
		}
		{
			gui::IGUIElement *e = getElementFromId(GUI_ID_TOOL_WEAR_CB);
			if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
				m_data->tool_wear = ((gui::IGUICheckBox*)e)->isChecked();
		}
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_FIRE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->unsafe_fire = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_DELETE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->delete_map = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_CLEAR_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->clear_map = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_SEED_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->use_fixed_seed = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_SEED_INPUT);
		if(e != NULL)
			m_data->fixed_seed = e->getText();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_MAP_TYPE_COMBO);
		if (e != NULL && e->getType() == gui::EGUIET_COMBO_BOX) {
			gui::IGUIComboBox *c = (gui::IGUIComboBox*)e;
			switch (c->getItemData(c->getSelected())) {
			case GUI_ID_MAP_TYPE_FLAT:
				m_data->map_type = "flat";
				break;
			case GUI_ID_MAP_TYPE_FLATTER:
				m_data->map_type = "flatter";
				break;
			case GUI_ID_MAP_TYPE_SMOOTHER:
				m_data->map_type = "smoother";
				break;
			case GUI_ID_MAP_TYPE_HILLY:
				m_data->map_type = "hilly";
				break;
			case GUI_ID_MAP_TYPE_MOUNTAINS:
				m_data->map_type = "mountains";
				break;
			case GUI_ID_MAP_TYPE_CRAZY:
				m_data->map_type = "crazy";
				break;
			case GUI_ID_MAP_TYPE_CRAZYHILLS:
				m_data->map_type = "crazyhills";
				break;
			default:
				m_data->map_type = "default";
			}
		}
	}

	m_accepted = true;
}

bool GUIMainMenu::OnEvent(const SEvent& event)
{
	if(event.EventType==EET_KEY_INPUT_EVENT)
	{
		if(event.KeyInput.Key==KEY_ESCAPE && event.KeyInput.PressedDown)
		{
			m_gamecallback->exitToOS();
			quitMenu();
			return true;
		}
		if(event.KeyInput.Key==KEY_RETURN && event.KeyInput.PressedDown)
		{
			acceptInput();
			quitMenu();
			return true;
		}
	}
	if (event.EventType==EET_GUI_EVENT) {
		if (event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST && isVisible()) {
			if (!canTakeFocus(event.GUIEvent.Element)) {
				dstream<<"GUIMainMenu: Not allowing focus change."
						<<std::endl;
				// Returning true disables focus change
				return true;
			}
		}
		if (event.GUIEvent.EventType==gui::EGET_CHECKBOX_CHANGED) {
			switch (event.GUIEvent.Caller->getID()) {
			case GUI_ID_SUFFOCATE_CB:
			case GUI_ID_HUNGER_CB:
			case GUI_ID_INITIAL_INV_CB:
			case GUI_ID_DROPPABLE_INV_CB:
			case GUI_ID_TOOL_WEAR_CB:
				acceptInput();
				m_accepted = false;
				break;
			case GUI_ID_INFINITE_INV_CB:
			case GUI_ID_MAP_DELETE_CB: // Delete map
			case GUI_ID_MAP_CLEAR_CB:
			case GUI_ID_MAP_SEED_CB:
			case GUI_ID_DAMAGE_CB:
				acceptInput();
				m_accepted = false;
				regenerateGui(m_screensize);
				break;
			}
		}
		if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
			switch (event.GUIEvent.Caller->getID()) {
			case GUI_ID_JOIN_GAME_BUTTON: // Start game
				acceptInput();
				if (
					m_data->selected_tab == TAB_SINGLEPLAYER
					|| m_data->selected_tab == TAB_SINGLEPLAYER_ADVANCED
					|| m_data->selected_tab == TAB_SINGLEPLAYER_MAP
				)
					m_data->address = std::wstring(L"");
				quitMenu();
				return true;
			case GUI_ID_CHARACTER_CREATOR:
			{
				acceptInput();
				m_data->character_creator = true;
				quitMenu();
				return true;
			}
			case GUI_ID_GAME_SETTINGS_BASIC:
			case GUI_ID_TAB_SINGLEPLAYER:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_SINGLEPLAYER;
				g_settings->set("mainmenu_tab","singleplayer");
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_GAME_SETTINGS_ADV:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_SINGLEPLAYER_ADVANCED;
				g_settings->set("mainmenu_tab","singleadvanced");
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_MAP_OPTIONS_BUTTON:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_SINGLEPLAYER_MAP;
				g_settings->set("mainmenu_tab","singlemap");
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_TAB_MULTIPLAYER:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_MULTIPLAYER;
				g_settings->set("mainmenu_tab","multiplayer");
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_TAB_SETTINGS:
			{
				GUISettingsMenu *smenu = new GUISettingsMenu(env, parent, -1,menumgr, false);
				smenu->drop();
				return true;
			}
			case GUI_ID_TAB_CREDITS:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_CREDITS;
				g_settings->set("mainmenu_tab","credits");
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_TAB_QUIT:
				m_gamecallback->exitToOS();
				quitMenu();
				return true;
			}
		}
		if (event.GUIEvent.EventType == gui::EGET_COMBO_BOX_CHANGED) {
			switch (event.GUIEvent.Caller->getID()) {
			case GUI_ID_GAME_MODE_COMBO:
				acceptInput();
				m_accepted = false;
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_MOBS_COMBO:
				acceptInput();
				m_accepted = false;
				return true;
			}
		}
		if (event.GUIEvent.EventType==gui::EGET_EDITBOX_ENTER) {
			switch (event.GUIEvent.Caller->getID()) {
			case GUI_ID_ADDRESS_INPUT:
			case GUI_ID_PORT_INPUT:
			case GUI_ID_NAME_INPUT:
			case GUI_ID_PW_INPUT:
				acceptInput();
				quitMenu();
				return true;
			}
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}
