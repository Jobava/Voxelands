/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* guiTextInputMenu.cpp
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

#include "guiTextInputMenu.h"
#include "debug.h"
#include "serialization.h"
#include <string>
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include "intl.h"
#include "gui_colours.h"
#if USE_FREETYPE
#include "intlGUIEditBox.h"
#endif

GUITextInputMenu::GUITextInputMenu(gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id,
		IMenuManager *menumgr,
		FormIO *io,
		std::wstring initial_text
):
	GUIModalMenu(env, parent, id, menumgr),
	m_io(io),
	m_initial_text(initial_text)
{
}

GUITextInputMenu::~GUITextInputMenu()
{
	if (m_io)
		delete m_io;
	removeChildren();
}

void GUITextInputMenu::removeChildren()
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

void GUITextInputMenu::regenerateGui(v2u32 screensize)
{
	std::wstring text;

	{
		gui::IGUIElement *e = getElementFromId(256);
		if (e != NULL) {
			text = e->getText();
		}else{
			text = m_initial_text;
		}
	}

	/*
		Remove stuff
	*/
	removeChildren();

	/*
		Calculate new sizes and positions
	*/
	core::rect<s32> rect(
			150,
			screensize.Y - 100,
			450,
			screensize.Y - 70
	);

	DesiredRect = rect;
	recalculateAbsolutePosition(false);

	/*
		Add stuff
	*/
	{
		core::rect<s32> rect(5, 0, 290, 30);
		gui::IGUIEditBox *e;
#if USE_FREETYPE
		e = (gui::IGUIEditBox *) new gui::intlGUIEditBox(text.c_str(), true, Environment, this, 256, rect);
#else
		e = Environment->addEditBox(text.c_str(), rect, false, this, 256);
#endif
		Environment->setFocus(e);


		irr::SEvent evt;
		evt.EventType = EET_KEY_INPUT_EVENT;
		evt.KeyInput.Key = KEY_END;
		evt.KeyInput.PressedDown = true;
		e->OnEvent(evt);
	}
}

void GUITextInputMenu::drawMenu()
{
	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;
	video::IVideoDriver* driver = Environment->getVideoDriver();

	video::SColor c(64,0,0,0);

	driver->draw2DRectangle(AbsoluteRect, c, c, c, c, &AbsoluteClippingRect);

#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
	// Irrlicht 1.8 input colours
	skin->setColor(gui::EGDC_FOCUSED_EDITABLE, video::SColor(0,0,0,0));
	skin->setColor(gui::EGDC_EDITABLE, video::SColor(0,0,0,0));
#endif

	gui::IGUIElement::draw();

#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
	// Irrlicht 1.8 input colours
	skin->setColor(gui::EGDC_EDITABLE, GUI_EDITABLE);
	skin->setColor(gui::EGDC_FOCUSED_EDITABLE, GUI_FOCUSED_EDITABLE);
#endif
}

void GUITextInputMenu::acceptInput()
{
	if (m_io) {
		gui::IGUIElement *e = getElementFromId(256);
		if (e != NULL) {
			std::map<std::string,std::wstring> fields;
			fields["text"] = e->getText();
			m_io->gotText(fields);
		}
		delete m_io;
		m_io = NULL;
	}
}

bool GUITextInputMenu::OnEvent(const SEvent& event)
{
	if (event.EventType==EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.Key==KEY_ESCAPE && event.KeyInput.PressedDown) {
			quitMenu();
			return true;
		}
		if (event.KeyInput.Key==KEY_RETURN && event.KeyInput.PressedDown) {
			acceptInput();
			quitMenu();
			return true;
		}
	}
	if (event.EventType==EET_GUI_EVENT) {
		if (event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST && isVisible()) {
			if (!canTakeFocus(event.GUIEvent.Element)) {
				dstream<<"GUITextInputMenu: Not allowing focus change."
						<<std::endl;
				// Returning true disables focus change
				return true;
			}
		}
		if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
			switch (event.GUIEvent.Caller->getID()) {
			case 257:
				acceptInput();
				quitMenu();
				// quitMenu deallocates menu
				return true;
			}
		}
		if (event.GUIEvent.EventType==gui::EGET_EDITBOX_ENTER) {
			switch (event.GUIEvent.Caller->getID()) {
			case 256:
				acceptInput();
				quitMenu();
				// quitMenu deallocates menu
				return true;
			}
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}

