/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
* Copyright (C) 2011 Ciaran Gultnieks <ciaran@ciarang.com>
*
* guiPasswordChange.cpp
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

#include "guiPasswordChange.h"
#include "debug.h"
#include "serialization.h"
#include <string>
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include "gettext.h"
#include "gui_colours.h"
#if USE_FREETYPE
#include "intlGUIEditBox.h"
#endif

const int ID_oldPassword = 256;
const int ID_newPassword1 = 257;
const int ID_newPassword2 = 258;
const int ID_change = 259;
const int ID_message = 260;

GUIPasswordChange::GUIPasswordChange(gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id,
		IMenuManager *menumgr,
		Client* client
):
	GUIModalMenu(env, parent, id, menumgr),
	m_client(client)
{
}

GUIPasswordChange::~GUIPasswordChange()
{
	removeChildren();
}

void GUIPasswordChange::removeChildren()
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

void GUIPasswordChange::regenerateGui(v2u32 screensize)
{
	/*
		Remove stuff
	*/
	removeChildren();

	/*
		Calculate new sizes and positions
	*/
	core::rect<s32> rect(
			screensize.X/2 - 230,
			screensize.Y/2 - 120,
			screensize.X/2 + 230,
			screensize.Y/2 + 120
	);

	DesiredRect = rect;
	recalculateAbsolutePosition(false);

	v2s32 size = rect.getSize();
	v2s32 topleft_client(40, 0);
	v2s32 size_client = size - v2s32(40, 0);

	/*
		Add stuff
	*/
	s32 ypos = 30;
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += topleft_client + v2s32(35, ypos+6);
		Environment->addStaticText(wgettext("Old Password"), rect, false, true, this, -1);
	}
	{
		core::rect<s32> rect(0, 0, 230, 30);
		rect += topleft_client + v2s32(160, ypos);
		gui::IGUIEditBox *e;
#if USE_FREETYPE
		e = (gui::IGUIEditBox *) new gui::intlGUIEditBox(L"", true, Environment, this, ID_oldPassword, rect);
#else
		e = Environment->addEditBox(L"", rect, false, this, ID_oldPassword);
#endif
		Environment->setFocus(e);
		e->setPasswordBox(true);
	}
	ypos += 50;
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += topleft_client + v2s32(35, ypos+6);
		Environment->addStaticText(wgettext("New Password"), rect, false, true, this, -1);
	}
	{
		core::rect<s32> rect(0, 0, 230, 30);
		rect += topleft_client + v2s32(160, ypos);
		gui::IGUIEditBox *e;
#if USE_FREETYPE
		e = (gui::IGUIEditBox *) new gui::intlGUIEditBox(L"", true, Environment, this, ID_newPassword1, rect);
#else
		e = Environment->addEditBox(L"", rect, false, this, ID_newPassword1);
#endif
		e->setPasswordBox(true);
	}
	ypos += 50;
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += topleft_client + v2s32(35, ypos+6);
		Environment->addStaticText(wgettext("Confirm Password"), rect, false, true, this, -1);
	}
	{
		core::rect<s32> rect(0, 0, 230, 30);
		rect += topleft_client + v2s32(160, ypos);
		gui::IGUIEditBox *e;
#if USE_FREETYPE
		e = (gui::IGUIEditBox *) new gui::intlGUIEditBox(L"", true, Environment, this, ID_newPassword2, rect);
#else
		e = Environment->addEditBox(L"", rect, false, this, ID_newPassword2);
#endif
		e->setPasswordBox(true);
	}

	ypos += 50;
	{
		core::rect<s32> rect(0, 0, 140, 30);
		rect = rect + v2s32(size.X/2-140/2, ypos);
		Environment->addButton(rect, this, ID_change, wgettext("Change"));
	}

	ypos += 50;
	{
		core::rect<s32> rect(0, 0, 300, 20);
		rect += topleft_client + v2s32(35, ypos);
		IGUIElement *e =
		Environment->addStaticText(wgettext("Passwords do not match!"), rect, false, true, this, ID_message);
		e->setVisible(false);
	}

}

void GUIPasswordChange::drawMenu()
{
	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;
	video::IVideoDriver* driver = Environment->getVideoDriver();

	driver->draw2DRectangle(AbsoluteRect, GUI_BG_TOP, GUI_BG_TOP, GUI_BG_BTM, GUI_BG_BTM, &AbsoluteClippingRect);
	driver->draw2DRectangleOutline(AbsoluteRect, GUI_BORDER);

	gui::IGUIElement::draw();
}

bool GUIPasswordChange::acceptInput()
{
		std::wstring oldpass;
		std::wstring newpass;
		gui::IGUIElement *e;
		e = getElementFromId(ID_oldPassword);
		if(e != NULL)
			oldpass = e->getText();
		e = getElementFromId(ID_newPassword1);
		if(e != NULL)
			newpass = e->getText();
		e = getElementFromId(ID_newPassword2);
		if(e != NULL && newpass != e->getText())
		{
			e = getElementFromId(ID_message);
			if(e != NULL)
				e->setVisible(true);
			return false;
		}
		m_client->sendChangePassword(oldpass, newpass);
		return true;
}

bool GUIPasswordChange::OnEvent(const SEvent& event)
{
	if(event.EventType==EET_KEY_INPUT_EVENT)
	{
		if(event.KeyInput.Key==KEY_ESCAPE && event.KeyInput.PressedDown)
		{
			quitMenu();
			return true;
		}
		if(event.KeyInput.Key==KEY_RETURN && event.KeyInput.PressedDown)
		{
			if(acceptInput())
				quitMenu();
			return true;
		}
	}
	if(event.EventType==EET_GUI_EVENT)
	{
		if(event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST
				&& isVisible())
		{
			if(!canTakeFocus(event.GUIEvent.Element))
			{
				dstream<<"GUIPasswordChange: Not allowing focus change."
						<<std::endl;
				// Returning true disables focus change
				return true;
			}
		}
		if(event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED)
		{
			switch(event.GUIEvent.Caller->getID())
			{
			case ID_change:
				if(acceptInput())
					quitMenu();
				return true;
			}
		}
		if(event.GUIEvent.EventType==gui::EGET_EDITBOX_ENTER)
		{
			switch(event.GUIEvent.Caller->getID())
			{
			case ID_oldPassword:
			case ID_newPassword1:
			case ID_newPassword2:
				if(acceptInput())
					quitMenu();
				return true;
			}
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}

