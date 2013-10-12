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

#include "guiMainMenu.h"
#include "guiKeyChangeMenu.h"
#include "debug.h"
#include "serialization.h"
#include <string>
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include <IGUITabControl.h>


#include "gettext.h"

GUIMainMenu::GUIMainMenu(gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id,
		IMenuManager *menumgr,
		MainMenuData *data,
		IGameCallback *gamecallback
):
	GUIModalMenu(env, parent, id, menumgr),
	m_data(data),
	m_accepted(false),
	m_gamecallback(gamecallback)
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
	for(core::list<gui::IGUIElement*>::ConstIterator
			i = children.begin(); i != children.end(); i++)
	{
		children_copy.push_back(*i);
	}
	for(core::list<gui::IGUIElement*>::Iterator
			i = children_copy.begin();
			i != children_copy.end(); i++)
	{
		(*i)->remove();
	}
}

void GUIMainMenu::regenerateGui(v2u32 screensize)
{
	std::wstring text_name;
	std::wstring text_address;
	std::wstring text_port;
	bool creative_mode;
	bool enable_damage;
	bool fancy_trees;
	bool smooth_lighting;
	bool clouds_3d;
	bool opaque_water;

	m_screensize = screensize;

	// Client options
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_NAME_INPUT);
		if(e != NULL)
			text_name = e->getText();
		else
			text_name = m_data->name;
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
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_FANCYTREE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			fancy_trees = ((gui::IGUICheckBox*)e)->isChecked();
		else
			fancy_trees = m_data->fancy_trees;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_SMOOTH_LIGHTING_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			smooth_lighting = ((gui::IGUICheckBox*)e)->isChecked();
		else
			smooth_lighting = m_data->smooth_lighting;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_3D_CLOUDS_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			clouds_3d = ((gui::IGUICheckBox*)e)->isChecked();
		else
			clouds_3d = m_data->clouds_3d;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_OPAQUE_WATER_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			opaque_water = ((gui::IGUICheckBox*)e)->isChecked();
		else
			opaque_water = m_data->opaque_water;
	}

	// Server options
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_CREATIVE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			creative_mode = ((gui::IGUICheckBox*)e)->isChecked();
		else
			creative_mode = m_data->creative_mode;
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_DAMAGE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			enable_damage = ((gui::IGUICheckBox*)e)->isChecked();
		else
			enable_damage = m_data->enable_damage;
	}

	/*
		Remove stuff
	*/
	removeChildren();

	/*
		Calculate new sizes and positions
	*/

	v2s32 size(500, 430);

	core::rect<s32> rect(
			screensize.X/2 - size.X/2,
			screensize.Y/2 - size.Y/2,
			screensize.X/2 + size.X/2,
			screensize.Y/2 + size.Y/2
	);

	DesiredRect = rect;
	recalculateAbsolutePosition(false);

	// Single Player button
	{
		core::rect<s32> rect(0, 0, 180, 30);
		rect += v2s32(10, 80);
		Environment->addButton(rect, this, GUI_ID_TAB_SINGLEPLAYER, wgettext("Single Player"));
	}
	// Multi Player button
	{
		core::rect<s32> rect(0, 0, 180, 30);
		rect += v2s32(10, 120);
		Environment->addButton(rect, this, GUI_ID_TAB_MULTIPLAYER, wgettext("Multi Player"));
	}
	// Settings button
	{
		core::rect<s32> rect(0, 0, 180, 30);
		rect += v2s32(10, 160);
		Environment->addButton(rect, this, GUI_ID_TAB_SETTINGS, wgettext("Settings"));
	}
	// Credits button
	{
		core::rect<s32> rect(0, 0, 180, 30);
		rect += v2s32(10, 200);
		Environment->addButton(rect, this, GUI_ID_TAB_CREDITS, wgettext("Credits"));
	}
	// Quit button
	{
		core::rect<s32> rect(0, 0, 180, 30);
		rect += v2s32(10, 240);
		Environment->addButton(rect, this, GUI_ID_TAB_QUIT, wgettext("Quit"));
	}

	v2s32 topleft_content(200, 0);
	v2s32 size_content = size - v2s32(300, 0);

	//v2s32 size = rect.getSize();

	/*
		Add stuff
	*/

	/*
		Client section
	*/

	if (m_data->selected_tab == TAB_MULTIPLAYER) {
		changeCtype("");
		{
			core::rect<s32> rect(0, 0, 310, 20);
			rect += topleft_content + v2s32(0, 20);
			const wchar_t *text = L"Multi Player";
			gui::IGUIStaticText *t = Environment->addStaticText(text, rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}

		// Nickname + password
		{
			core::rect<s32> rect(0, 0, 110, 20);
			rect += topleft_content + v2s32(20, 60);
			Environment->addStaticText(wgettext("Name/Password"), rect, false, true, this, -1);
		}
		changeCtype("C");
		{
			core::rect<s32> rect(0, 0, 230, 30);
			rect += topleft_content + v2s32(35, 90);
			gui::IGUIElement *e =
			Environment->addEditBox(text_name.c_str(), rect, false, this, GUI_ID_NAME_INPUT);
			if(text_name == L"")
				Environment->setFocus(e);
		}
		{
			core::rect<s32> rect(0, 0, 230, 30);
			rect += topleft_content + v2s32(35, 125);
			gui::IGUIEditBox *e =
			Environment->addEditBox(L"", rect, false, this, 264);
			e->setPasswordBox(true);
			if(text_name != L"" && text_address != L"")
				Environment->setFocus(e);

		}
		changeCtype("");
		// Address + port
		{
			core::rect<s32> rect(0, 0, 110, 20);
			rect += topleft_content + v2s32(20, 170);
			Environment->addStaticText(wgettext("Address/Port"),
				rect, false, true, this, -1);
		}
		changeCtype("C");
		{
			core::rect<s32> rect(0, 0, 230, 30);
			rect += topleft_content + v2s32(35, 200);
			gui::IGUIElement *e =
			Environment->addEditBox(text_address.c_str(), rect, false, this, GUI_ID_ADDRESS_INPUT);
			if(text_name != L"" && text_address == L"")
				Environment->setFocus(e);
		}
		{
			core::rect<s32> rect(0, 0, 120, 30);
			rect += topleft_content + v2s32(145, 240);
			Environment->addEditBox(text_port.c_str(), rect, false, this, GUI_ID_PORT_INPUT);
		}
		changeCtype("");
		// Start game button
		{
			core::rect<s32> rect(0, 0, 180, 30);
			rect += topleft_content + v2s32(60, 290);
			Environment->addButton(rect, this, GUI_ID_JOIN_GAME_BUTTON, wgettext("Connect"));
		}
	}else if (m_data->selected_tab == TAB_SETTINGS) {
		changeCtype("");
		{
			core::rect<s32> rect(0, 0, 310, 20);
			rect += topleft_content + v2s32(0, 20);
			const wchar_t *text = L"Settings";
			gui::IGUIStaticText *t = Environment->addStaticText(text, rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(85, 60);
			Environment->addCheckBox(fancy_trees, rect, this, GUI_ID_FANCYTREE_CB,
				wgettext("Fancy trees"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(85, 90);
			Environment->addCheckBox(smooth_lighting, rect, this, GUI_ID_SMOOTH_LIGHTING_CB,
					wgettext("Smooth Lighting"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(85, 120);
			Environment->addCheckBox(clouds_3d, rect, this, GUI_ID_3D_CLOUDS_CB,
					wgettext("3D Clouds"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(85, 150);
			Environment->addCheckBox(opaque_water, rect, this, GUI_ID_OPAQUE_WATER_CB,
					wgettext("Opaque water"));
		}

		// Key change button
		{
			core::rect<s32> rect(0, 0, 130, 30);
			rect += topleft_content + v2s32(90, 200);
			Environment->addButton(rect, this, GUI_ID_CHANGE_KEYS_BUTTON,
				wgettext("Change keys"));
		}
	}else if (m_data->selected_tab == TAB_SINGLEPLAYER) {
		changeCtype("");
		{
			core::rect<s32> rect(0, 0, 300, 20);
			rect += topleft_content + v2s32(0, 20);
			const wchar_t *text = L"Single Player";
			gui::IGUIStaticText *t = Environment->addStaticText(text, rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}

		// Server parameters
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(85, 60);
			Environment->addCheckBox(creative_mode, rect, this, GUI_ID_CREATIVE_CB, wgettext("Creative Mode"));
		}
		{
			core::rect<s32> rect(0, 0, 200, 30);
			rect += topleft_content + v2s32(85, 90);
			Environment->addCheckBox(enable_damage, rect, this, GUI_ID_DAMAGE_CB, wgettext("Enable Damage"));
		}
		// Map delete button
		{
			core::rect<s32> rect(0, 0, 130, 30);
			rect += topleft_content + v2s32(90, 130);
			Environment->addButton(rect, this, GUI_ID_DELETE_MAP_BUTTON, wgettext("Delete map"));
		}
		// Start game button
		{
			core::rect<s32> rect(0, 0, 180, 30);
			rect += topleft_content + v2s32(60, 200);
			Environment->addButton(rect, this, GUI_ID_JOIN_GAME_BUTTON, wgettext("Start Game"));
		}
	}else if(m_data->selected_tab == TAB_CREDITS) {
		// CREDITS
		{
			core::rect<s32> rect(0, 0, 310, 20);
			rect += topleft_content + v2s32(0, 20);
			const wchar_t *text = L"Credits";
			gui::IGUIStaticText *t = Environment->addStaticText(text, rect, false, true, this, -1);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}
		{
			core::rect<s32> rect(0, 0, 300, 350);
			rect += topleft_content + v2s32(0, 50);
			gui::IGUIStaticText *t = Environment->addStaticText(
				narrow_to_wide(
					"Minetest-Classic\n"
					"http://www.minetest-classic.com/\n"
					"\n"
					"By Lisa 'darkrose' Milne <lisa@ltmnet.com>\n"
					"and contributers: sdzen, MichaelEh?, Pentium44, Jordach, Menche\n"
					"\n"
					"Based on Minetest-C55\n"
					"by Perttu Ahola <celeron55@gmail.com>\n"
					"and contributors: PilzAdam, Taoki, tango_, kahrl (kaaaaaahrl?), darkrose, matttpt, erlehmann, SpeedProg, JacobF, teddydestodes, marktraceur, Jonathan Neuschafer, thexyz, VanessaE, sfan5... and tens of more random people."
				).c_str(),
				rect,
				false,
				true,
				this,
				-1
			);
			t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
		}
	}
	changeCtype("C");
}

void GUIMainMenu::drawMenu()
{
	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;
	video::IVideoDriver* driver = Environment->getVideoDriver();

	{
		video::SColor bgtcolor(240,50,50,70);
		video::SColor bgbcolor(240,30,30,50);
		core::rect<s32> left(
			AbsoluteRect.UpperLeftCorner.X,
			AbsoluteRect.UpperLeftCorner.Y,
			AbsoluteRect.LowerRightCorner.X-300,
			AbsoluteRect.LowerRightCorner.Y
		);
		core::rect<s32> right(
			AbsoluteRect.UpperLeftCorner.X+200,
			AbsoluteRect.UpperLeftCorner.Y,
			AbsoluteRect.LowerRightCorner.X,
			AbsoluteRect.LowerRightCorner.Y
		);
		driver->draw2DRectangle(left, bgtcolor, bgtcolor, bgbcolor, bgbcolor, &AbsoluteClippingRect);
		driver->draw2DRectangle(right, bgbcolor, bgbcolor, bgtcolor, bgtcolor, &AbsoluteClippingRect);
		video::SColor bdcolor(245,60,60,80);
		driver->draw2DRectangleOutline(AbsoluteRect, bdcolor);
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
		gui::IGUIElement *e = getElementFromId(264);
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
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_CREATIVE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->creative_mode = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_DAMAGE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->enable_damage = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_FANCYTREE_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->fancy_trees = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_SMOOTH_LIGHTING_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->smooth_lighting = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_3D_CLOUDS_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->clouds_3d = ((gui::IGUICheckBox*)e)->isChecked();
	}
	{
		gui::IGUIElement *e = getElementFromId(GUI_ID_OPAQUE_WATER_CB);
		if(e != NULL && e->getType() == gui::EGUIET_CHECK_BOX)
			m_data->opaque_water = ((gui::IGUICheckBox*)e)->isChecked();
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
	if(event.EventType==EET_GUI_EVENT)
	{
		if(event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST
				&& isVisible())
		{
			if(!canTakeFocus(event.GUIEvent.Element))
			{
				dstream<<"GUIMainMenu: Not allowing focus change."
						<<std::endl;
				// Returning true disables focus change
				return true;
			}
		}
		if(event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED)
		{
			switch(event.GUIEvent.Caller->getID())
			{
			case GUI_ID_JOIN_GAME_BUTTON: // Start game
				acceptInput();
				if (m_data->selected_tab == TAB_SINGLEPLAYER)
					m_data->address = std::wstring(L"");
				quitMenu();
				return true;
			case GUI_ID_CHANGE_KEYS_BUTTON: {
				GUIKeyChangeMenu *kmenu = new GUIKeyChangeMenu(env, parent, -1,menumgr);
				kmenu->drop();
				return true;
			}
			case GUI_ID_DELETE_MAP_BUTTON: // Delete map
				// Don't accept input data, just set deletion request
				m_data->delete_map = true;
				m_accepted = true;
				quitMenu();
				return true;
			case GUI_ID_TAB_SINGLEPLAYER:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_SINGLEPLAYER;
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_TAB_MULTIPLAYER:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_MULTIPLAYER;
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_TAB_SETTINGS:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_SETTINGS;
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_TAB_CREDITS:
				if (m_data->selected_tab == TAB_SETTINGS)
					acceptInput();
				m_accepted = false;
				m_data->selected_tab = TAB_CREDITS;
				regenerateGui(m_screensize);
				return true;
			case GUI_ID_TAB_QUIT:
				m_gamecallback->exitToOS();
				quitMenu();
				return true;
			}
		}
		if(event.GUIEvent.EventType==gui::EGET_EDITBOX_ENTER)
		{
			switch(event.GUIEvent.Caller->getID())
			{
				case GUI_ID_ADDRESS_INPUT: case GUI_ID_PORT_INPUT: case GUI_ID_NAME_INPUT: case 264:
				acceptInput();
				quitMenu();
				return true;
			}
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}
