/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
* Copyright (C) 2011 Ciaran Gultnieks <ciaran@ciarang.com>
* Copyright (C) 2011 teddydestodes <derkomtur@schattengang.net>
*
* guiKeyChangeMenu.cpp
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

#include "guiKeyChangeMenu.h"
#include "debug.h"
#include "serialization.h"
#include "main.h"
#include <string>
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include "settings.h"
#include "gui_colours.h"

GUIKeyChangeMenu::GUIKeyChangeMenu(gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id, IMenuManager *menumgr) :
	GUIModalMenu(env, parent, id, menumgr)
{
	activeKey = -1;
	init_keys();
}

GUIKeyChangeMenu::~GUIKeyChangeMenu()
{
	removeChildren();
}

void GUIKeyChangeMenu::removeChildren()
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

void GUIKeyChangeMenu::regenerateGui(v2u32 screensize)
{
	/*
	 Remove stuff
	 */
	removeChildren();

	/*
	 Calculate new sizes and positions
	 */

	v2s32 size(620, 430);

	core::rect < s32 > rect(screensize.X / 2 - size.X / 2,
			screensize.Y / 2 - size.Y / 2, screensize.X / 2 + size.X / 2,
			screensize.Y / 2 + size.Y / 2);

	DesiredRect = rect;
	recalculateAbsolutePosition(false);

	v2s32 topleft(0, 0);
	{
		core::rect < s32 > rect(0, 0, 620, 20);
		rect += topleft + v2s32(0, 10);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Controls"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	v2s32 offset(25, 40);
	// buttons

	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Forward"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->forward = Environment->addButton(rect, this,
				GUI_ID_KEY_FORWARD_BUTTON,
				wgettext(key_forward.name()));
	}

	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Backward"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->backward = Environment->addButton(rect, this,
				GUI_ID_KEY_BACKWARD_BUTTON,
				wgettext(key_backward.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Left"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->left = Environment->addButton(rect, this, GUI_ID_KEY_LEFT_BUTTON,
				wgettext(key_left.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Right"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->right = Environment->addButton(rect, this,
				GUI_ID_KEY_RIGHT_BUTTON,
				wgettext(key_right.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Use"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->use = Environment->addButton(rect, this, GUI_ID_KEY_USE_BUTTON,
				wgettext(key_use.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Sneak"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->sneak = Environment->addButton(rect, this,
				GUI_ID_KEY_SNEAK_BUTTON,
				wgettext(key_sneak.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Jump"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->jump = Environment->addButton(rect, this, GUI_ID_KEY_JUMP_BUTTON,
				wgettext(key_jump.name()));
	}

	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Inventory"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->inventory = Environment->addButton(rect, this,
				GUI_ID_KEY_INVENTORY_BUTTON,
				wgettext(key_inventory.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Chat"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->chat = Environment->addButton(rect, this, GUI_ID_KEY_CHAT_BUTTON,
				wgettext(key_chat.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Command"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->cmd = Environment->addButton(rect, this, GUI_ID_KEY_CMD_BUTTON,
				wgettext(key_cmd.name()));
	}


	//next col
	offset = v2s32(350, 40);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Toggle fly"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->fly = Environment->addButton(rect, this, GUI_ID_KEY_FLY_BUTTON,
				wgettext(key_fly.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Toggle fast"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->fast = Environment->addButton(rect, this, GUI_ID_KEY_FAST_BUTTON,
				wgettext(key_fast.name()));
	}
	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Range select"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->range = Environment->addButton(rect, this,
				GUI_ID_KEY_RANGE_BUTTON,
				wgettext(key_range.name()));
	}

	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Print stacks"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->dump = Environment->addButton(rect, this, GUI_ID_KEY_DUMP_BUTTON,
				wgettext(key_dump.name()));
	}

	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Next item"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}
	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft	+ v2s32(offset.X + 135, offset.Y - 5);
		this->next_item = Environment->addButton(rect, this, GUI_ID_KEY_NEXT_ITEM_BUTTON,
				wgettext(key_next_item.name()));
	}

	offset += v2s32(0, 33);
	{
		core::rect < s32 > rect(0, 0, 130, 20);
		rect += topleft + v2s32(offset.X, offset.Y);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Previous item"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT);
	}
	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(offset.X + 135, offset.Y - 5);
		this->prev_item = Environment->addButton(rect, this, GUI_ID_KEY_PREV_ITEM_BUTTON,
				wgettext(key_prev_item.name()));
	}

	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(size.X - 120 - 20, size.Y - 40);
		Environment->addButton(rect, this, GUI_ID_BACK_BUTTON,
		wgettext("Save"));
	}
	{
		core::rect < s32 > rect(0, 0, 120, 30);
		rect += topleft + v2s32(size.X - 120 - 20 - 120 - 20, size.Y - 40);
		Environment->addButton(rect, this, GUI_ID_ABORT_BUTTON,
		wgettext("Cancel"));
	}
}

void GUIKeyChangeMenu::drawMenu()
{
	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;
	video::IVideoDriver* driver = Environment->getVideoDriver();

	video::SColor bgcolor(140, 0, 0, 0);

	driver->draw2DRectangle(AbsoluteRect, GUI_BG_TOP, GUI_BG_TOP, GUI_BG_BTM, GUI_BG_BTM, &AbsoluteClippingRect);
	driver->draw2DRectangleOutline(AbsoluteRect, GUI_BORDER);

	gui::IGUIElement::draw();
}

bool GUIKeyChangeMenu::acceptInput()
{
	g_settings->set("keymap_forward", key_forward.sym());
	g_settings->set("keymap_backward", key_backward.sym());
	g_settings->set("keymap_left", key_left.sym());
	g_settings->set("keymap_right", key_right.sym());
	g_settings->set("keymap_jump", key_jump.sym());
	g_settings->set("keymap_sneak", key_sneak.sym());
	g_settings->set("keymap_inventory", key_inventory.sym());
	g_settings->set("keymap_chat", key_chat.sym());
	g_settings->set("keymap_cmd", key_cmd.sym());
	g_settings->set("keymap_rangeselect", key_range.sym());
	g_settings->set("keymap_freemove", key_fly.sym());
	g_settings->set("keymap_fastmove", key_fast.sym());
	g_settings->set("keymap_special1", key_use.sym());
	g_settings->set("keymap_print_debug_stacks", key_dump.sym());
	g_settings->set("keymap_select_next", key_next_item.sym());
	g_settings->set("keymap_select_prev", key_prev_item.sym());
	clearKeyCache();
	return true;
}
void GUIKeyChangeMenu::init_keys()
{
	key_forward = getKeySetting(VLKC_FORWARD);
	key_backward = getKeySetting(VLKC_BACKWARD);
	key_left = getKeySetting(VLKC_LEFT);
	key_right = getKeySetting(VLKC_RIGHT);
	key_jump = getKeySetting(VLKC_JUMP);
	key_sneak = getKeySetting(VLKC_SNEAK);
	key_inventory = getKeySetting(VLKC_INVENTORY);
	key_chat = getKeySetting(VLKC_CHAT);
	key_cmd = getKeySetting(VLKC_COMMAND);
	key_range = getKeySetting(VLKC_RANGE);
	key_fly = getKeySetting(VLKC_FREEMOVE);
	key_fast = getKeySetting(VLKC_FASTMOVE);
	key_use = getKeySetting(VLKC_USE);
	key_dump = getKeySetting(VLKC_PRINT_DEBUG);
	key_next_item = getKeySetting(VLKC_SELECT_NEXT);
	key_prev_item = getKeySetting(VLKC_SELECT_PREV);
}

bool GUIKeyChangeMenu::resetMenu()
{
	if (activeKey >= 0)
	{
		switch (activeKey)
		{
		case GUI_ID_KEY_FORWARD_BUTTON:
			this->forward->setText(
					wgettext(key_forward.name()));
			break;
		case GUI_ID_KEY_BACKWARD_BUTTON:
			this->backward->setText(
					wgettext(key_backward.name()));
			break;
		case GUI_ID_KEY_LEFT_BUTTON:
			this->left->setText(wgettext(key_left.name()));
			break;
		case GUI_ID_KEY_RIGHT_BUTTON:
			this->right->setText(wgettext(key_right.name()));
			break;
		case GUI_ID_KEY_JUMP_BUTTON:
			this->jump->setText(wgettext(key_jump.name()));
			break;
		case GUI_ID_KEY_SNEAK_BUTTON:
			this->sneak->setText(wgettext(key_sneak.name()));
			break;
		case GUI_ID_KEY_INVENTORY_BUTTON:
			this->inventory->setText(
					wgettext(key_inventory.name()));
			break;
		case GUI_ID_KEY_CHAT_BUTTON:
			this->chat->setText(wgettext(key_chat.name()));
			break;
		case GUI_ID_KEY_CMD_BUTTON:
			this->cmd->setText(wgettext(key_cmd.name()));
			break;
		case GUI_ID_KEY_RANGE_BUTTON:
			this->range->setText(wgettext(key_range.name()));
			break;
		case GUI_ID_KEY_FLY_BUTTON:
			this->fly->setText(wgettext(key_fly.name()));
			break;
		case GUI_ID_KEY_FAST_BUTTON:
			this->fast->setText(wgettext(key_fast.name()));
			break;
		case GUI_ID_KEY_USE_BUTTON:
			this->use->setText(wgettext(key_use.name()));
			break;
		case GUI_ID_KEY_DUMP_BUTTON:
			this->dump->setText(wgettext(key_dump.name()));
			break;
		case GUI_ID_KEY_NEXT_ITEM_BUTTON:
			this->next_item->setText(wgettext(key_next_item.name()));
			break;
		case GUI_ID_KEY_PREV_ITEM_BUTTON:
			this->prev_item->setText(wgettext(key_prev_item.name()));
			break;
		}
		activeKey = -1;
		return false;
	}
	return true;
}
bool GUIKeyChangeMenu::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_KEY_INPUT_EVENT && activeKey >= 0
			&& event.KeyInput.PressedDown)
	{
		KeyPress kp(event.KeyInput);

		if (activeKey == GUI_ID_KEY_FORWARD_BUTTON)
		{
			this->forward->setText(wgettext(kp.name()));
			this->key_forward = kp;
		}
		else if (activeKey == GUI_ID_KEY_BACKWARD_BUTTON)
		{
			this->backward->setText(wgettext(kp.name()));
			this->key_backward = kp;
		}
		else if (activeKey == GUI_ID_KEY_LEFT_BUTTON)
		{
			this->left->setText(wgettext(kp.name()));
			this->key_left = kp;
		}
		else if (activeKey == GUI_ID_KEY_RIGHT_BUTTON)
		{
			this->right->setText(wgettext(kp.name()));
			this->key_right = kp;
		}
		else if (activeKey == GUI_ID_KEY_JUMP_BUTTON)
		{
			this->jump->setText(wgettext(kp.name()));
			this->key_jump = kp;
		}
		else if (activeKey == GUI_ID_KEY_SNEAK_BUTTON)
		{
			this->sneak->setText(wgettext(kp.name()));
			this->key_sneak = kp;
		}
		else if (activeKey == GUI_ID_KEY_INVENTORY_BUTTON)
		{
			this->inventory->setText(wgettext(kp.name()));
			this->key_inventory = kp;
		}
		else if (activeKey == GUI_ID_KEY_CHAT_BUTTON)
		{
			this->chat->setText(wgettext(kp.name()));
			this->key_chat = kp;
		}
		else if (activeKey == GUI_ID_KEY_CMD_BUTTON)
		{
			this->cmd->setText(wgettext(kp.name()));
			this->key_cmd = kp;
		}
		else if (activeKey == GUI_ID_KEY_RANGE_BUTTON)
		{
			this->range->setText(wgettext(kp.name()));
			this->key_range = kp;
		}
		else if (activeKey == GUI_ID_KEY_FLY_BUTTON)
		{
			this->fly->setText(wgettext(kp.name()));
			this->key_fly = kp;
		}
		else if (activeKey == GUI_ID_KEY_FAST_BUTTON)
		{
			this->fast->setText(wgettext(kp.name()));
			this->key_fast = kp;
		}
		else if (activeKey == GUI_ID_KEY_USE_BUTTON)
		{
			this->use->setText(wgettext(kp.name()));
			this->key_use = kp;
		}
		else if (activeKey == GUI_ID_KEY_DUMP_BUTTON)
		{
			this->dump->setText(wgettext(kp.name()));
			this->key_dump = kp;
		}
		else if (activeKey == GUI_ID_KEY_NEXT_ITEM_BUTTON)
		{
			this->next_item->setText(wgettext(kp.name()));
			this->key_next_item = kp;
		}
		else if (activeKey == GUI_ID_KEY_PREV_ITEM_BUTTON)
		{
			this->prev_item->setText(wgettext(kp.name()));
			this->key_prev_item = kp;
		}
		activeKey = -1;
		return true;
	}
	if (event.EventType == EET_GUI_EVENT)
	{
		if (event.GUIEvent.EventType == gui::EGET_ELEMENT_FOCUS_LOST
				&& isVisible())
		{
			if (!canTakeFocus(event.GUIEvent.Element))
			{
				dstream << "GUIMainMenu: Not allowing focus change."
						<< std::endl;
				// Returning true disables focus change
				return true;
			}
		}
		if (event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
		{

			switch (event.GUIEvent.Caller->getID())
			{
			case GUI_ID_BACK_BUTTON: //back
				acceptInput();
				quitMenu();
				return true;
			case GUI_ID_ABORT_BUTTON: //abort
				quitMenu();
				return true;
			case GUI_ID_KEY_FORWARD_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->forward->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_BACKWARD_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->backward->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_LEFT_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->left->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_RIGHT_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->right->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_USE_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->use->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_FLY_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->fly->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_FAST_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->fast->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_JUMP_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->jump->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_CHAT_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->chat->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_CMD_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->cmd->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_SNEAK_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->sneak->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_INVENTORY_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->inventory->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_DUMP_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->dump->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_RANGE_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->range->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_NEXT_ITEM_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->next_item->setText(wgettext("press Key"));
				break;
			case GUI_ID_KEY_PREV_ITEM_BUTTON:
				resetMenu();
				activeKey = event.GUIEvent.Caller->getID();
				this->prev_item->setText(wgettext("press Key"));
				break;
			}

		}
	}
	return Parent ? Parent->OnEvent(event) : false;
}

