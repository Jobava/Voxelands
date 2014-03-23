/*
Minetest-c55
Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef GUIINVENTORYMENU_HEADER
#define GUIINVENTORYMENU_HEADER

#include "common_irrlicht.h"
#include <IGUIFont.h>
#include "inventory.h"
#include "modalMenu.h"
#include "utility.h"
#include <map>

class InventoryManager;

struct TextDest
{
	virtual ~TextDest() {};
	// This is deprecated I guess? -celeron55
	virtual void gotText(std::wstring text) = 0;
	virtual void gotText(std::map<std::string, std::string> fields) = 0;
};

class IFormSource
{
public:
	virtual ~IFormSource(){}
	virtual std::string getForm() = 0;
	// Fill in variables in field text
	virtual std::string resolveText(std::string str){ return str; }
};

void drawInventoryItem(video::IVideoDriver *driver,
		gui::IGUIFont *font,
		InventoryItem *item,
		core::rect<s32> rect,
		const core::rect<s32> *clip);

class GUIFormSpecMenu : public GUIModalMenu
{
	struct ItemSpec
	{
		ItemSpec()
		{
			i = -1;
		}
		ItemSpec(const InventoryLocation &a_inventoryloc,
			const std::string &a_listname,
			s32 a_i)
		{
			inventoryloc = a_inventoryloc;
			listname = a_listname;
			i = a_i;
		}
		bool isValid() const
		{
			return i != -1;
		}

		InventoryLocation inventoryloc;
		std::string listname;
		s32 i;
	};

	struct ListDrawSpec
	{
		ListDrawSpec()
		{
		}
		ListDrawSpec(const InventoryLocation &a_inventoryloc,
				const std::string &a_listname,
				v2s32 a_pos, v2s32 a_geom):
			inventoryloc(a_inventoryloc),
			listname(a_listname),
			pos(a_pos),
			geom(a_geom)
		{
		}

		InventoryLocation inventoryloc;
		std::string listname;
		v2s32 pos;
		v2s32 geom;
	};

	struct ImageDrawSpec
	{
		ImageDrawSpec()
		{
		}
		ImageDrawSpec(const std::string &a_name,
				v2s32 a_pos, v2s32 a_geom):
			name(a_name),
			pos(a_pos),
			geom(a_geom)
		{
		}
		std::string name;
		v2s32 pos;
		v2s32 geom;
	};

	struct FieldSpec
	{
		FieldSpec()
		{
		}
		FieldSpec(const std::wstring name, const std::wstring label, const std::wstring fdeflt, int id):
			fname(name),
			flabel(label),
			fdefault(fdeflt),
			fid(id)
		{
			send = false;
			is_button = false;
			is_exit = false;
		}
		std::wstring fname;
		std::wstring flabel;
		std::wstring fdefault;
		int fid;
		bool send;
		bool is_button;
		bool is_exit;
	};

public:
	GUIFormSpecMenu(gui::IGUIEnvironment* env,
			gui::IGUIElement* parent, s32 id,
			IMenuManager *menumgr,
			InventoryManager *invmgr
			);
	~GUIFormSpecMenu();

	void setFormSpec(const std::string &formspec_string,
			InventoryLocation current_inventory_location)
	{
		m_formspec_string = formspec_string;
		m_current_inventory_location = current_inventory_location;
		regenerateGui(m_screensize_old);
	}

	// form_src is deleted by this GUIFormSpecMenu
	void setFormSource(IFormSource *form_src)
	{
		m_form_src = form_src;
	}

	// text_dst is deleted by this GUIFormSpecMenu
	void setTextDest(TextDest *text_dst)
	{
		m_text_dst = text_dst;
	}

	void removeChildren();
	/*
		Remove and re-add (or reposition) stuff
	*/
	void regenerateGui(v2u32 screensize);

	ItemSpec getItemAtPos(v2s32 p) const;
	void drawList(const ListDrawSpec &s, int phase);
	void drawMenu();

	void acceptInput();
	bool OnEvent(const SEvent& event);

protected:
	v2s32 getBasePos() const
	{
		return padding + AbsoluteRect.UpperLeftCorner;
	}

	v2s32 padding;
	v2s32 spacing;
	v2s32 imgsize;

	InventoryManager *m_invmgr;

	std::string m_formspec_string;
	InventoryLocation m_current_inventory_location;
	IFormSource *m_form_src;
	TextDest *m_text_dst;

	core::array<ListDrawSpec> m_inventorylists;
	core::array<ImageDrawSpec> m_images;
	core::array<FieldSpec> m_fields;

	ItemSpec *m_selected_item;
	u32 m_selected_amount;
	bool m_selected_dragging;

	v2s32 m_pointer;
	gui::IGUIStaticText *m_tooltip_element;
};

#endif

