/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* guiFormSpecMenu.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2011-2014 <lisa@ltmnet.com>
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
#include "inventory.h"
#include "utility.h"
#include "modalMenu.h"
#include "keycode.h"
#include "guiFormSpecMenu.h"
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include "constants.h"
#include "log.h"
#include "tile.h" // ITextureSource
#include "path.h"
#include "gui_colours.h"
#include "intl.h"
#if USE_FREETYPE
#include "intlGUIEditBox.h"
#endif
#include "hud.h"

/*
	GUIFormSpecMenu
*/

GUIFormSpecMenu::GUIFormSpecMenu(gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id,
		IMenuManager *menumgr,
		InventoryManager *invmgr
):
	GUIModalMenu(env, parent, id, menumgr),
	m_invmgr(invmgr),
	m_form_io(NULL),
	m_selected_item(NULL),
	m_selected_amount(0),
	m_selected_dragging(false),
	m_tooltip_element(NULL)
{
}

GUIFormSpecMenu::~GUIFormSpecMenu()
{
	removeChildren();

	delete m_selected_item;
	delete m_form_io;
}

void GUIFormSpecMenu::removeChildren()
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
	/*{
		gui::IGUIElement *e = getElementFromId(256);
		if(e != NULL)
			e->remove();
	}*/
	if(m_tooltip_element)
	{
		m_tooltip_element->remove();
		m_tooltip_element = NULL;
	}
}

void GUIFormSpecMenu::regenerateGui(v2u32 screensize)
{
	// Remove children
	removeChildren();

	v2s32 size(100,100);
	s32 helptext_h = 15;
	core::rect<s32> rect;

	// Base position of contents of form
	v2s32 basepos = getBasePos();
	// State of basepos, 0 = not set, 1= set by formspec, 2 = set by size[] element
	// Used to adjust form size automatically if needed
	// A proceed button is added if there is no size[] element
	int bp_set = 0;

	/* Convert m_init_draw_spec to m_inventorylists */

	m_inventorylists.clear();
	m_rings.clear();
	m_images.clear();
	m_fields.clear();

	Strfnd f(m_formspec_string);
	while (f.atend() == false) {
		std::string type = trim(f.next("["));
		if (type == "invsize" || type == "size") {
			v2f invsize;
			invsize.X = mystof(f.next(","));
			if (type == "size") {
				invsize.Y = mystof(f.next("]"));
			}else{
				invsize.Y = mystof(f.next(";"));
				errorstream<<"WARNING: invsize is deprecated, use size"<<std::endl;
				f.next("]");
			}
			infostream<<"size ("<<invsize.X<<","<<invsize.Y<<")"<<std::endl;

			padding = v2s32(screensize.Y/40, screensize.Y/40);
			spacing = v2s32(screensize.Y/12, screensize.Y/13);
			imgsize = v2s32(screensize.Y/15, screensize.Y/15);
			size = v2s32(
				padding.X*2+spacing.X*(invsize.X-1.0)+imgsize.X,
				padding.Y*2+spacing.Y*(invsize.Y-1.0)+imgsize.Y + (helptext_h-5)
			);
			rect = core::rect<s32>(
				screensize.X/2 - size.X/2,
				screensize.Y/2 - size.Y/2,
				screensize.X/2 + size.X/2,
				screensize.Y/2 + size.Y/2
			);
			DesiredRect = rect;
			recalculateAbsolutePosition(false);
			basepos = getBasePos();
			bp_set = 2;
		}else if (type == "list") {
			std::string name = f.next(";");
			InventoryLocation loc;
			if (name == "context" || name == "current_name") {
				loc = m_current_inventory_location;
			}else{
				loc.deSerialize(name);
			}
			std::string listname = f.next(";");
			v2s32 pos = basepos;
			pos.X += mystof(f.next(",")) * (float)spacing.X;
			pos.Y += mystof(f.next(";")) * (float)spacing.Y;
			v2s32 geom;
			geom.X = mystoi(f.next(","));
			geom.Y = mystoi(f.next(";"));
			int i_start = 0;
			int i_end = -1;
			std::string end = f.next("]");
			std::string bg = "";
			if (end != "") {
				Strfnd fend(end);
				std::string s_start = fend.next(",");
				std::string s_end = fend.next(";");
				if (s_start != "") {
					if (s_end != "") {
						i_start = mystoi(s_start);
						i_end = mystoi(s_end);
						end = fend.end();
					}else{
						end = s_start;
					}
				}
				if (end != "")
					bg = end;
			}
			infostream<<"list inv="<<name<<", listname="<<listname
					<<", pos=("<<pos.X<<","<<pos.Y<<")"
					<<", geom=("<<geom.X<<","<<geom.Y<<")"
					<<std::endl;
			if (bp_set != 2)
				errorstream<<"WARNING: invalid use of list without a size[] element"<<std::endl;
			m_inventorylists.push_back(ListDrawSpec(loc, listname, bg, pos, geom, i_start, i_end));
		}else if (type == "image") {
			v2s32 pos = basepos;
			pos.X += mystof(f.next(",")) * (float)spacing.X;
			pos.Y += mystof(f.next(";")) * (float)spacing.Y;
			v2s32 geom;
			geom.X = mystof(f.next(",")) * (float)imgsize.X;
			geom.Y = mystof(f.next(";")) * (float)imgsize.Y;
			std::string name = f.next("]");
			infostream<<"image name="<<name
					<<", pos=("<<pos.X<<","<<pos.Y<<")"
					<<", geom=("<<geom.X<<","<<geom.Y<<")"
					<<std::endl;
			if (bp_set != 2)
				errorstream<<"WARNING: invalid use of image without a size[] element"<<std::endl;
			m_images.push_back(ImageDrawSpec(name, pos, geom));
		}else if (type == "ring") {
			v2s32 pos = basepos;
			pos.X += mystof(f.next(",")) * (float)spacing.X;
			pos.Y += mystof(f.next(";")) * (float)spacing.Y;
			int rad = mystof(f.next(";")) * (float)imgsize.Y;
			pos.X += rad/2;
			pos.Y += rad/2;
			std::string c = f.next(";");
			int val = mystoi(f.next("]"));
			video::SColor col;
			if (!parseColorString(c,col,true))
				col = video::SColor(255,255,0,0);
			m_rings.push_back(RingDrawSpec(col,val,pos,rad));
		}else if (type == "field") {
			std::string fname = f.next(";");
			std::string flabel = f.next(";");
			bool multi = false;

			if (fname.find(",") == std::string::npos && flabel.find(",") == std::string::npos) {
				if (!bp_set) {
					rect = core::rect<s32>(
						screensize.X/2 - 160,
						screensize.Y/2 - 60,
						screensize.X/2 + 160,
						screensize.Y/2 + 60
					);
					DesiredRect = rect;
					recalculateAbsolutePosition(false);
					basepos = getBasePos();
					bp_set = 1;
				}else if(bp_set == 2) {
					errorstream<<"WARNING: invalid use of unpositioned field in inventory"<<std::endl;
				}

				v2s32 pos = basepos;
				pos.Y = (m_fields.size()+1*50);
				v2s32 size = DesiredRect.getSize();
				rect = core::rect<s32>(size.X/2-150, pos.Y, (size.X/2-150)+300, pos.Y+30);
			}else{
				v2s32 pos;
				pos.X = mystof(fname.substr(0,fname.find(","))) * (float)spacing.X;
				pos.Y = mystof(fname.substr(fname.find(",")+1)) * (float)spacing.Y;
				v2s32 geom;
				geom.X = (mystof(flabel.substr(0,flabel.find(","))) * (float)spacing.X)-(spacing.X-imgsize.X);
				if (mystof(flabel.substr(flabel.find(",")+1)) > 1.0) {
					geom.Y = (mystof(flabel.substr(flabel.find(",")+1)) * (float)spacing.Y)-(spacing.Y-imgsize.Y);
					multi = true;
				}else{
					geom.Y = 30;
				}
				pos.Y += ((mystof(flabel.substr(flabel.find(",")+1)) * (float)imgsize.Y)/2)-15;

				rect = core::rect<s32>(pos.X, pos.Y, pos.X+geom.X, pos.Y+geom.Y);

				fname = f.next(";");
				flabel = f.next(";");
				if(bp_set != 2)
					errorstream<<"WARNING: invalid use of positioned field without a size[] element"<<std::endl;

			}

			std::string odefault = f.next("]");
			std::string fdefault;

			// fdefault may contain a variable reference, which
			// needs to be resolved from the node metadata
			if (m_form_io) {
				fdefault = m_form_io->resolveText(odefault);
			}else{
				fdefault = odefault;
			}

			FieldSpec spec = FieldSpec(
				fname.c_str(),
				narrow_to_wide(flabel.c_str()),
				narrow_to_wide(fdefault.c_str()),
				258+m_fields.size()
			);

			// three cases: field and no label, label and no field, label and field
			if (flabel == "") {
				spec.send = true;
				gui::IGUIEditBox *e;
#if USE_FREETYPE
				e = (gui::IGUIEditBox *) new gui::intlGUIEditBox(spec.fdefault.c_str(), true, Environment, this, spec.fid, rect);
#else
				e = Environment->addEditBox(spec.fdefault.c_str(), rect, false, this, spec.fid);
#endif
				if (multi) {
					e->setMultiLine(true);
					e->setTextAlignment(gui::EGUIA_UPPERLEFT, gui::EGUIA_UPPERLEFT);
				}else{
					irr::SEvent evt;
					evt.EventType = EET_KEY_INPUT_EVENT;
					evt.KeyInput.Key = KEY_END;
					evt.KeyInput.PressedDown = true;
					e->OnEvent(evt);
					Environment->setFocus(e);
				}
			}else if (fname == "") {
				// set spec field id to 0, this stops submit searching for a value that isn't there
				Environment->addStaticText(spec.flabel.c_str(), rect, false, true, this, spec.fid);
			}else{
				spec.send = true;
				gui::IGUIEditBox *e;
#if USE_FREETYPE
				e = (gui::IGUIEditBox *) new gui::intlGUIEditBox(spec.fdefault.c_str(), true, Environment, this, spec.fid, rect);
#else
				e = Environment->addEditBox(spec.fdefault.c_str(), rect, false, this, spec.fid);
#endif
				if (multi) {
					e->setMultiLine(true);
					e->setWordWrap(true);
					e->setTextAlignment(gui::EGUIA_UPPERLEFT, gui::EGUIA_UPPERLEFT);
				}else{
					Environment->setFocus(e);
					irr::SEvent evt;
					evt.EventType = EET_KEY_INPUT_EVENT;
					evt.KeyInput.Key = KEY_END;
					evt.KeyInput.Char = 0;
					evt.KeyInput.Control = 0;
					evt.KeyInput.Shift = 0;
					evt.KeyInput.PressedDown = true;
					e->OnEvent(evt);
				}
				rect.UpperLeftCorner.Y -= 20;
				rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y+15;
				Environment->addStaticText(spec.flabel.c_str(), rect, false, true, this, 0);
			}

			m_fields.push_back(spec);
		}else if (type == "label") {
			v2s32 pos;
			pos.X = mystof(f.next(",")) * (float)spacing.X;
			pos.Y = mystof(f.next(";")) * (float)spacing.Y;

			rect = core::rect<s32>(pos.X, pos.Y+((imgsize.Y/2)-15), pos.X+300, pos.Y+((imgsize.Y/2)+15));

			std::string flabel = f.next("]");
			if (bp_set != 2)
				errorstream<<"WARNING: invalid use of label without a size[] element"<<std::endl;

			FieldSpec spec = FieldSpec(
				"",
				narrow_to_wide(flabel.c_str()),
				narrow_to_wide(""),
				258+m_fields.size()
			);
			Environment->addStaticText(spec.flabel.c_str(), rect, false, true, this, spec.fid);
			m_fields.push_back(spec);
		}else if (type == "button" || type == "button_exit") {
			v2s32 pos;
			pos.X = mystof(f.next(",")) * (float)spacing.X;
			pos.Y = mystof(f.next(";")) * (float)spacing.Y;
			v2s32 geom;
			geom.X = (mystof(f.next(",")) * (float)spacing.X)-(spacing.X-imgsize.X);
			pos.Y += (mystof(f.next(";")) * (float)imgsize.Y)/2;

			rect = core::rect<s32>(pos.X, pos.Y-15, pos.X+geom.X, pos.Y+15);

			std::string fname = f.next(";");
			std::string flabel = f.next("]");
			if (bp_set != 2)
				errorstream<<"WARNING: invalid use of button without a size[] element"<<std::endl;

			FieldSpec spec = FieldSpec(
				fname.c_str(),
				narrow_to_wide(flabel.c_str()),
				narrow_to_wide(""),
				258+m_fields.size()
			);
			spec.is_button = true;
			if (type == "button_exit")
				spec.is_exit = true;
			Environment->addButton(rect, this, spec.fid, spec.flabel.c_str());
			m_fields.push_back(spec);
		}else if (type == "image_button" || type == "image_button_exit") {
			v2s32 pos;
			pos.X = mystof(f.next(",")) * (float)spacing.X;
			pos.Y = mystof(f.next(";")) * (float)spacing.Y;
			v2s32 geom;
			geom.X = (mystof(f.next(",")) * (float)spacing.X)-(spacing.X-imgsize.X);
			geom.Y = (mystof(f.next(";")) * (float)spacing.Y)-(spacing.Y-imgsize.Y);

			rect = core::rect<s32>(pos.X, pos.Y, pos.X+geom.X, pos.Y+geom.Y);

			std::string fimage = f.next(";");
			std::string fname = f.next(";");
			std::string flabel = f.next("]");
			if (bp_set != 2)
				errorstream<<"WARNING: invalid use of image_button without a size[] element"<<std::endl;

			FieldSpec spec = FieldSpec(
				fname.c_str(),
				narrow_to_wide(flabel.c_str()),
				narrow_to_wide(fimage.c_str()),
				258+m_fields.size()
			);
			spec.is_button = true;
			if (type == "image_button_exit")
				spec.is_exit = true;

			video::ITexture *texture = Environment->getVideoDriver()->getTexture(getTexturePath(fimage).c_str());
			gui::IGUIButton *e = Environment->addButton(rect, this, spec.fid, spec.flabel.c_str());
			e->setImage(texture);
			e->setPressedImage(texture);
			e->setScaleImage(true);

			m_fields.push_back(spec);
		}else{
			// Ignore others
			std::string ts = f.next("]");
			infostream<<"Unknown DrawSpec: type="<<type<<", data=\""<<ts<<"\""
					<<std::endl;
		}
	}

	// If there's inventory, put the usage string at the bottom
	if (m_inventorylists.size()) {
		core::rect<s32> rect(0, 0, size.X-padding.X*2, helptext_h);
		rect = rect + v2s32(size.X/2 - rect.getWidth()/2,
				size.Y-rect.getHeight()-5);
		const wchar_t *text = wgettext("Left click: Move all items, Right click: Move single item");
		Environment->addStaticText(text, rect, false, true, this, 256);
	}
	// If there's fields, add a Proceed button
	if (m_fields.size() && bp_set != 2) {
		// if the size wasn't set by an invsize[] or size[] adjust it now to fit all the fields
		rect = core::rect<s32>(
			screensize.X/2 - 160,
			screensize.Y/2 - 60,
			screensize.X/2 + 160,
			screensize.Y/2 + 50+(m_fields.size()*50)
		);
		DesiredRect = rect;
		recalculateAbsolutePosition(false);
		basepos = getBasePos();

		{
			v2s32 pos = basepos;
			pos.Y = ((m_fields.size()+1)*50);

			v2s32 size = DesiredRect.getSize();
			rect = core::rect<s32>(size.X/2-70, pos.Y, (size.X/2-70)+140, pos.Y+25);
			Environment->addButton(rect, this, 257, wgettext("Write It"));
		}
	}
	// Add tooltip
	{
		// Note: parent != this so that the tooltip isn't clipped by the menu rectangle
		m_tooltip_element = Environment->addStaticText(L"",core::rect<s32>(0,0,1000,18));
		m_tooltip_element->enableOverrideColor(true);
		m_tooltip_element->setBackgroundColor(GUI_BG_TT);
		m_tooltip_element->setDrawBackground(true);
		m_tooltip_element->setDrawBorder(true);
		m_tooltip_element->setOverrideColor(GUI_TT_TEXT);
		m_tooltip_element->setTextAlignment(gui::EGUIA_UPPERLEFT, gui::EGUIA_CENTER);
		m_tooltip_element->setWordWrap(true);
	}
}

GUIFormSpecMenu::ItemSpec GUIFormSpecMenu::getItemAtPos(v2s32 p) const
{
	core::rect<s32> imgrect(0,0,imgsize.X,imgsize.Y);

	for (u32 i=0; i<m_inventorylists.size(); i++) {
		const ListDrawSpec &s = m_inventorylists[i];
		int end = s.i_end;
		if (end < 0)
			end = s.i_start+(s.geom.X*s.geom.Y);

		for (s32 i=s.i_start; i<end; i++) {
			s32 x = ((i-s.i_start)%s.geom.X) * spacing.X;
			s32 y = ((i-s.i_start)/s.geom.X) * spacing.Y;
			v2s32 p0(x,y);
			core::rect<s32> rect = imgrect + s.pos + p0;
			rect.UpperLeftCorner -= 2;
			rect.LowerRightCorner += 2;
			if (rect.isPointInside(p)) {
				return ItemSpec(s.inventoryloc, s.listname, i);
			}
		}
	}

	return ItemSpec(InventoryLocation(), "", -1);
}

void GUIFormSpecMenu::drawList(const ListDrawSpec &s, int phase)
{
	video::IVideoDriver* driver = Environment->getVideoDriver();

	// Get font
	gui::IGUIFont *font = NULL;
	gui::IGUISkin* skin = Environment->getSkin();
	if (skin)
		font = skin->getFont();

	Inventory *inv = m_invmgr->getInventory(&s.inventoryloc);
	if (!inv)
		return;
	InventoryList *ilist = inv->getList(s.listname);

	video::ITexture *bg_texture = NULL;
	if (s.background != "")
		bg_texture = driver->getTexture(getTexturePath(s.background).c_str());

	core::rect<s32> imgrect(0,0,imgsize.X,imgsize.Y);

	int end = s.i_end;
	if (end < 0)
		end = s.i_start+(s.geom.X*s.geom.Y);

	for (s32 i=s.i_start; i<end; i++) {
		s32 x = ((i-s.i_start)%s.geom.X) * spacing.X;
		s32 y = ((i-s.i_start)/s.geom.X) * spacing.Y;
		v2s32 p(x,y);
		core::rect<s32> rect = imgrect + s.pos + p;
		core::rect<s32> brect = rect;
		brect.UpperLeftCorner -= 2;
		brect.LowerRightCorner += 2;
		InventoryItem *item = NULL;
		if (ilist)
			item = ilist->getItem(i);

		driver->draw2DRectangle(GUI_INV_BG, brect, &AbsoluteClippingRect);

		if (m_selected_item != NULL && m_selected_item->listname == s.listname && m_selected_item->i == i) {
			driver->draw2DRectangleOutline(brect, GUI_INV_HIGHLIGHT);

		}else{
			driver->draw2DRectangleOutline(brect, GUI_INV_BORDER);
		}

		if (item) {
			draw_inv_item(driver, font, item, rect, &AbsoluteClippingRect);
			if (rect.isPointInside(m_pointer)) {
				std::wstring name = item->getGuiText();
				if (name != L"") {
					m_tooltip_element->setVisible(true);
					this->bringToFront(m_tooltip_element);
					m_tooltip_element->setText(name.c_str());
					s32 tooltip_x = m_pointer.X + 15;
					s32 tooltip_y = m_pointer.Y + 15;
					// this prevents forced word-wrap when we don't want it
					m_tooltip_element->setRelativePosition(
						core::rect<s32>(
							core::position2d<s32>(tooltip_x, tooltip_y),
							core::dimension2d<s32>(1000,18)
						)
					);
					s32 tooltip_width = m_tooltip_element->getTextWidth() + 14;
					s32 tooltip_height = m_tooltip_element->getTextHeight() + 4;
					m_tooltip_element->setRelativePosition(
						core::rect<s32>(
							core::position2d<s32>(tooltip_x, tooltip_y),
							core::dimension2d<s32>(tooltip_width, tooltip_height)
						)
					);
				}
			}
		}else if (bg_texture != NULL) {
			const video::SColor color(255,255,255,255);
			draw_image(driver, bg_texture, color, rect,NULL,&AbsoluteClippingRect);
		}
	}
}

void GUIFormSpecMenu::drawMenu()
{
	if (m_form_io) {
		std::string newform = m_form_io->getForm();
		if (newform != m_formspec_string) {
			m_formspec_string = newform;
			regenerateGui(m_screensize_old);
		}
	}

	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;
	video::IVideoDriver* driver = Environment->getVideoDriver();

	driver->draw2DRectangle(AbsoluteRect,GUI_BG_TOP, GUI_BG_TOP, GUI_BG_BTM, GUI_BG_BTM, &AbsoluteClippingRect);
	driver->draw2DRectangleOutline(AbsoluteRect, GUI_BORDER);

	m_tooltip_element->setVisible(false);

	/*
		Draw items
		Phase 0: Item slot rectangles
		Phase 1: Item images; prepare tooltip
	*/

	for (u32 i=0; i<m_rings.size(); i++) {
		draw_progress_ring(
			driver,
			v2s32(m_screensize_old.X,m_screensize_old.Y),
			m_rings[i].pos,
			m_rings[i].rad,
			m_rings[i].value,
			m_rings[i].colour
		);
	}

	for (u32 i=0; i<m_inventorylists.size(); i++) {
		drawList(m_inventorylists[i], 1);
	}

	for (u32 i=0; i<m_images.size(); i++) {
		const ImageDrawSpec &spec = m_images[i];
		video::ITexture *texture = driver->getTexture(getTexturePath(spec.name).c_str());
		// Image size on screen
		core::rect<s32> imgrect(0, 0, spec.geom.X, spec.geom.Y);
		// Image rectangle on screen
		core::rect<s32> rect = imgrect + spec.pos;
		const video::SColor color(255,255,255,255);
		const video::SColor colors[] = {color,color,color,color};
		driver->draw2DImage(
			texture,
			rect,
			core::rect<s32>(
				core::position2d<s32>(0,0),
				core::dimension2di(texture->getOriginalSize())
			),
			NULL,
			colors,
			true
		);
	}

	/*
		Call base class
	*/
	gui::IGUIElement::draw();
}

void GUIFormSpecMenu::acceptInput()
{
	if (m_form_io) {
		std::map<std::string, std::wstring> fields;
		gui::IGUIElement *e;
		for (u32 i=0; i<m_fields.size(); i++) {
			const FieldSpec &s = m_fields[i];
			if (s.send) {
				if (s.is_button) {
					fields[s.fname.c_str()] = L"true";
				}else{
					e = getElementFromId(s.fid);
					if (e != NULL)
						fields[s.fname.c_str()] = e->getText();
				}
			}
		}
		m_form_io->gotText(fields);
	}
}

bool GUIFormSpecMenu::OnEvent(const SEvent& event)
{
	if (event.EventType==EET_KEY_INPUT_EVENT) {
		KeyPress kp(event.KeyInput);
		if (event.KeyInput.PressedDown && (kp == EscapeKey || kp == getKeySetting(VLKC_INVENTORY))) {
			m_tooltip_element->setVisible(false);
			acceptInput();
			quitMenu();
			return true;
		}
		if (event.KeyInput.Key==KEY_RETURN && event.KeyInput.PressedDown) {
			m_tooltip_element->setVisible(false);
			acceptInput();
			quitMenu();
			return true;
		}
	}
	if (event.EventType==EET_MOUSE_INPUT_EVENT) {
		char amount = -1;

		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
			amount = 0;
		}else if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) {
			amount = 1;
		}else if (event.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN) {
			amount = 10;
		}

		m_pointer.X = event.MouseInput.X;
		m_pointer.Y = event.MouseInput.Y;

		if (amount >= 0) {
			v2s32 p(event.MouseInput.X, event.MouseInput.Y);
			//infostream<<"Mouse down at p=("<<p.X<<","<<p.Y<<")"<<std::endl;
			ItemSpec s = getItemAtPos(p);
			if (s.isValid()) {
				if (m_selected_item) {
					Inventory *inv_from = m_invmgr->getInventory(&m_selected_item->inventoryloc);
					Inventory *inv_to = m_invmgr->getInventory(&s.inventoryloc);
					assert(inv_from);
					assert(inv_to);
					InventoryList *list_from =
							inv_from->getList(m_selected_item->listname);
					InventoryList *list_to =
							inv_to->getList(s.listname);
					if (list_from == NULL)
						infostream<<"from list doesn't exist"<<std::endl;
					if (list_to == NULL)
						infostream<<"to list doesn't exist"<<std::endl;
					// Indicates whether source slot completely empties
					bool source_empties = false;
					if (list_from && list_to && list_from->getItem(m_selected_item->i) != NULL) {
						infostream<<"Handing IACTION_MOVE to manager"<<std::endl;
						IMoveAction *a = new IMoveAction();
						a->count = amount;
						a->from_inv = m_selected_item->inventoryloc.getName();
						a->from_list = m_selected_item->listname;
						a->from_i = m_selected_item->i;
						a->to_inv = s.inventoryloc.getName();
						a->to_list = s.listname;
						a->to_i = s.i;
						//ispec.actions->push_back(a);
						m_invmgr->inventoryAction(a);

						if (list_from->getItem(m_selected_item->i)->getCount()==1)
							source_empties = true;
					}
					// Remove selection if target was left-clicked or source
					// slot was emptied
					if (amount == 0 || source_empties) {
						delete m_selected_item;
						m_selected_item = NULL;
					}
				}else{
					/*
						Select if non-NULL
					*/
					Inventory *inv = m_invmgr->getInventory(&s.inventoryloc);
					assert(inv);
					InventoryList *list = inv->getList(s.listname);
					if (list && list->getItem(s.i) != NULL)
						m_selected_item = new ItemSpec(s);
				}
			}else{
				if (m_selected_item) {
					delete m_selected_item;
					m_selected_item = NULL;
				}
			}
		}
	}
	if (event.EventType==EET_GUI_EVENT) {
		if (event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST && isVisible()) {
			if (!canTakeFocus(event.GUIEvent.Element)) {
				infostream<<"GUIFormSpecMenu: Not allowing focus change."
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
			// find the element that was clicked
			for (u32 i=0; i<m_fields.size(); i++) {
				FieldSpec &s = m_fields[i];
				// if its a button, set the send field so
				// receiveFields knows which button was pressed
				if (s.is_button && s.fid == event.GUIEvent.Caller->getID()) {
					s.send = true;
					acceptInput();
					if (s.is_exit) {
						quitMenu();
					}else{
						s.send = false;
						// Restore focus to the full form
						Environment->setFocus(this);
					}
					return true;
				}
			}
		}
		if (event.GUIEvent.EventType==gui::EGET_EDITBOX_ENTER) {
			if (event.GUIEvent.Caller->getID() > 257) {
				acceptInput();
				quitMenu();
				// quitMenu deallocates menu
				return true;
			}
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}

