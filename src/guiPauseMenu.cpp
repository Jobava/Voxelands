/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* guiPauseMenu.cpp
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

#include "guiPauseMenu.h"
#include "guiSettingsMenu.h"
#include "debug.h"
#include "serialization.h"
#include "porting.h"
#include "config.h"
#include "main.h"
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>

#include "intl.h"
#include "gui_colours.h"

GUIPauseMenu::GUIPauseMenu(gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id,
		IGameCallback *gamecallback,
		IMenuManager *menumgr):
	GUIModalMenu(env, parent, id, menumgr)
{
	m_gamecallback = gamecallback;
	this->env = env;
	this->parent = parent;
	this->id = id;
	this->menumgr = menumgr;
}

GUIPauseMenu::~GUIPauseMenu()
{
	removeChildren();
}

void GUIPauseMenu::removeChildren()
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

void GUIPauseMenu::regenerateGui(v2u32 screensize)
{
	/*
		Remove stuff
	*/
	removeChildren();

	/*
		Calculate new sizes and positions
	*/
	core::rect<s32> rect(
			screensize.X/2 - 170,
			screensize.Y/2 - 150,
			screensize.X/2 + 170,
			screensize.Y/2 + 150
	);

	DesiredRect = rect;
	recalculateAbsolutePosition(false);

	v2s32 size = rect.getSize();
	{
		core::rect<s32> rect(0, 0, 340, 20);
		rect = rect + v2s32(0 , 10);

		v2u32 max_texture_size;
		{
			video::IVideoDriver* driver = Environment->getVideoDriver();
			max_texture_size = driver->getMaxTextureSize();
		}

		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Voxelands by darkrose and contributors"), rect, false, true, this, 259);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	{
		core::rect<s32> rect(0, 0, 340, 20);
		rect = rect + v2s32(0 , 30);

		v2u32 max_texture_size;
		{
			video::IVideoDriver* driver = Environment->getVideoDriver();
			max_texture_size = driver->getMaxTextureSize();
		}

		gui::IGUIStaticText *t = Environment->addStaticText(L"www.voxelands.com", rect, false, true, this, 259);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	{
		core::rect<s32> rect(0, 0, 340, 20);
		rect = rect + v2s32(0,50);

		v2u32 max_texture_size;
		{
			video::IVideoDriver* driver = Environment->getVideoDriver();
			max_texture_size = driver->getMaxTextureSize();
		}

		gui::IGUIStaticText *t = Environment->addStaticText(L"darkrose@voxelands.com", rect, false, true, this, 259);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}

	/*
		Add stuff
	*/
	const s32 btn_height = 30;
	const s32 btn_gap = 20;
	const s32 btn_num = 4;
	s32 btn_y = (size.Y/2-((btn_num*btn_height+(btn_num-1)*btn_gap))/2)+40;
	{
		core::rect<s32> rect(0, 0, 180, btn_height);
		rect = rect + v2s32(size.X/2-180/2, btn_y);
		Environment->addButton(rect, this, 256, wgettext("Continue"));
	}
	btn_y += btn_height + btn_gap;
	{
		core::rect<s32> rect(0, 0, 180, btn_height);
		rect = rect + v2s32(size.X/2-180/2, btn_y);
		Environment->addButton(rect, this, 261, wgettext("Change Password"));
	}
	btn_y += btn_height + btn_gap;
	{
		core::rect<s32> rect(0, 0, 180, btn_height);
		rect = rect + v2s32(size.X/2-180/2, btn_y);
		Environment->addButton(rect, this, 265, wgettext("Settings"));
	}
	btn_y += btn_height + btn_gap;
	{
		core::rect<s32> rect(0, 0, 180, btn_height);
		rect = rect + v2s32(size.X/2-180/2, btn_y);
		Environment->addButton(rect, this, 260, wgettext("Disconnect"));
	}
}

void GUIPauseMenu::drawMenu()
{
	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;
	video::IVideoDriver* driver = Environment->getVideoDriver();

	driver->draw2DRectangle(AbsoluteRect, GUI_BG_TOP, GUI_BG_TOP, GUI_BG_BTM, GUI_BG_BTM, &AbsoluteClippingRect);
	driver->draw2DRectangleOutline(AbsoluteRect, GUI_BORDER);

	gui::IGUIElement::draw();
}

bool GUIPauseMenu::OnEvent(const SEvent& event)
{

	if(event.EventType==EET_KEY_INPUT_EVENT)
	{
		if(event.KeyInput.PressedDown)
		{
			if(event.KeyInput.Key==KEY_ESCAPE)
			{
				quitMenu();
				return true;
			}
			else if(event.KeyInput.Key==KEY_RETURN)
			{
				quitMenu();
				return true;
			}
		}
	}
	if(event.EventType==EET_GUI_EVENT)
	{
		if(event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST
				&& isVisible())
		{
			if(!canTakeFocus(event.GUIEvent.Element))
			{
				dstream<<"GUIPauseMenu: Not allowing focus change."
						<<std::endl;
				// Returning true disables focus change
				return true;
			}
		}
		if(event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED)
		{
			switch(event.GUIEvent.Caller->getID())
			{
			case 256: // continue
				quitMenu();
				// ALWAYS return immediately after quitMenu()
				return true;
			case 261:
				quitMenu();
				m_gamecallback->changePassword();
				return true;
			case 265: // settings
			{
				GUISettingsMenu *smenu = new GUISettingsMenu(env, parent, -1,menumgr);
				smenu->drop();
				return true;
			}
			case 260: // disconnect
				m_gamecallback->disconnect();
				quitMenu();
				return true;
			}
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}

