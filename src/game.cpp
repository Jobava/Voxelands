/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* game.cpp
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

#include "game.h"
#include "common_irrlicht.h"
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include "client.h"
#include "server.h"
#include "guiPauseMenu.h"
#include "guiPasswordChange.h"
#include "guiFormSpecMenu.h"
#include "guiTextInputMenu.h"
#include "guiDeathScreen.h"
#include "config.h"
#include "clouds.h"
#include "camera.h"
#include "farmesh.h"
#include "mapblock.h"
#include "settings.h"
#include "profiler.h"
#include "mainmenumanager.h"
#include "intl.h"
#include "log.h"
#include "filesys.h"
#include "path.h"
#include "sound.h"
#ifndef SERVER
#include "main.h"
#endif

/*
	TODO: Move content-aware stuff to separate file by adding properties
	      and virtual interfaces
*/
#include "content_mapnode.h"
#include "content_nodemeta.h"

/* because windows sucks */
#if _WIN32
#define swprintf _snwprintf
#endif

/*
	Setting this to 1 enables a special camera mode that forces
	the renderers to think that the camera statically points from
	the starting place to a static direction.

	This allows one to move around with the player and see what
	is actually drawn behind solid things and behind the player.
*/
#define FIELD_OF_VIEW_TEST 0


// Chat data
struct ChatLine
{
	ChatLine():
		age(0.0)
	{
	}
	ChatLine(const std::wstring &a_text):
		age(0.0),
		text(a_text)
	{
	}
	float age;
	std::wstring text;
};

/*
	Inventory stuff
*/

// Inventory actions from the menu are buffered here before sending
Queue<InventoryAction*> inventory_action_queue;
// This is a copy of the inventory that the client's environment has
Inventory local_inventory;

u16 g_selected_item = 0;

/*
	Text input system
*/

class ChatFormIO : public FormIO
{
public:
	ChatFormIO(Client *client)
	{
		m_client = client;
	}

	void gotText(std::map<std::string, std::wstring> fields)
	{
		// Discard empty line
		if (fields["text"] == L"")
			return;

		// Send to others
		m_client->sendChatMessage(fields["text"]);
		// Show locally
		m_client->addChatMessage(fields["text"]);
	}

	std::string getForm()
	{
		return "";
	}

	Client *m_client;
};

/* Respawn menu callback */

class MainRespawnInitiator: public IRespawnInitiator
{
public:
	MainRespawnInitiator(bool *active, Client *client):
		m_active(active), m_client(client)
	{
		*m_active = true;
	}
	void respawn()
	{
		*m_active = false;
		m_client->sendRespawn();
	}
private:
	bool *m_active;
	Client *m_client;
};

/* Form update callback */

class NodeMetadataFormIO: public FormIO
{
public:
	NodeMetadataFormIO(v3s16 p, Client *client):
		m_client(client),
		m_p(p)
	{
	}

	std::string getForm()
	{
		NodeMetadata *meta = m_client->getEnv().getMap().getNodeMetadata(m_p);
		if (!meta)
			return "";
		return meta->getDrawSpecString();
	}

	NodeMetadata *getMeta()
	{
		return m_client->getEnv().getMap().getNodeMetadata(m_p);
	}

	void gotText(std::map<std::string, std::wstring> fields)
	{
		m_client->sendNodemetaFields(m_p, "", fields);
	}

	Client *m_client;
	v3s16 m_p;
};

class PlayerInventoryFormIO: public FormIO
{
public:
	PlayerInventoryFormIO(Client *client):
		m_show_appearance(false),
		m_client(client)
	{
	}
	std::string getForm()
	{
		if (m_show_appearance) {
			return
				std::string("size[8,9]"
				"list[current_player;main;0,5;8,4;]"
				"button[0.5,3;3,1;show_craft;")+gettext("Show Crafting")+"]"
				"label[1,2;"+gettext("Clothes")+"]"
				"label[4.9,-0.1;"+gettext("Hat/Helmet")+"]"
				"list[current_player;hat;5,0;1,1;]"
				"label[6.6,1.2;"+gettext("Jacket")+"]"
				"list[current_player;jacket;6.3,1.3;1,1;]"
				"label[3.7,0.9;"+gettext("Decorative")+"]"
				"list[current_player;decorative;3.7,1;1,1;]"
				"label[5.4,1.2;"+gettext("Shirt")+"]"
				"list[current_player;shirt;5,1.3;1,1;]"
				"label[4.1,2.2;"+gettext("Belt")+"]"
				"list[current_player;belt;3.7,2.3;1,1;]"
				"label[5.3,2.5;"+gettext("Pants")+"]"
				"list[current_player;pants;5,2.6;1,1;]"
				"label[5.3,3.8;"+gettext("Boots")+"]"
				"list[current_player;boots;5,3.9;1,1;]";
		}
		return
			std::string("size[8,9]"
			"list[current_player;main;0,5;8,4;]"
			"label[1,1.7;")+gettext("Drop to Ground")+"]"
			"list[current_player;discard;1.2,2;1,1;]"
			"button[0.5,3.5;3,1;show_appearance;"+gettext("Change Clothing")+"]"
			"list[current_player;craft;3.5,1;3,3;]"
			"list[current_player;craftresult;7,2;1,1;]";
	}

	void gotText(std::map<std::string, std::wstring> fields)
	{
		if (fields["show_appearance"] != L"") {
			m_show_appearance = true;
		}else{
			m_show_appearance = false;
		}
	}

	bool m_show_appearance;
	Client *m_client;
};

/*
	Hotbar draw routine
*/
void draw_old_hotbar(video::IVideoDriver *driver, gui::IGUIFont *font,
		v2s32 centrelowerpos, s32 imgsize, s32 itemcount,
		Inventory *inventory, s32 halfheartcount, s32 halfbubblecount, s32 halfhungercount)
{
	InventoryList *mainlist = inventory->getList("main");
	if (mainlist == NULL) {
		errorstream<<"draw_hotbar(): mainlist == NULL"<<std::endl;
		return;
	}

	s32 padding = imgsize/12;
	s32 width = itemcount*(imgsize+padding*2);

	// Position of upper left corner of bar
	v2s32 pos = centrelowerpos - v2s32(width/2, imgsize+padding*2);

	core::rect<s32> imgrect(0,0,imgsize,imgsize);

	std::wstring selected = L"";

	for(s32 i=0; i<itemcount; i++)
	{
		InventoryItem *item = mainlist->getItem(i);

		core::rect<s32> rect = imgrect + pos + v2s32(padding+i*(imgsize+padding*2), padding);

		if (g_selected_item == i) {
			video::SColor c_outside(255,255,0,0);
			s32 xo = (rect.LowerRightCorner.X-rect.UpperLeftCorner.X) / 4;
			s32 yo = (rect.LowerRightCorner.Y-rect.UpperLeftCorner.Y) / 4;
			rect.LowerRightCorner.X += xo;
			rect.UpperLeftCorner.Y -= yo;
			pos.X += xo;
			video::SColor bgcolor2(128,0,0,0);
			driver->draw2DRectangle(bgcolor2, rect, NULL);
			driver->draw2DRectangleOutline(rect, c_outside);

			if (item != NULL) {
				drawInventoryItem(driver, font, item, rect, NULL);
				std::wstring name = item->getGuiName();
				if (name != L"")
					selected = name;
			}
		}else{
			video::SColor bgcolor2(128,0,0,0);
			driver->draw2DRectangle(bgcolor2, rect, NULL);

			if (item != NULL)
				drawInventoryItem(driver, font, item, rect, NULL);
		}
	}

	/*
		Draw hearts
	*/
	struct {
		s32 count;
		s32 halfcount;
		const char* texture;
		bool show_full;
	} barData[3] = {
		{halfheartcount/2,halfheartcount,"heart.png",true},
		{halfbubblecount/2,halfbubblecount,"bubble.png",false},
		{halfhungercount/2,halfhungercount,"harvested_carrot.png",true},
	};
	v2s32 bar_base(0,-25);
	for (s32 k=0; k<3; k++) {
		if (barData[k].count == 10 && !barData[k].show_full)
			continue;
		video::ITexture *texture = driver->getTexture(getTexturePath(barData[k].texture).c_str());
		v2s32 p = pos + bar_base;
		for (s32 i=0; i<barData[k].count; i++) {
			const video::SColor color(255,255,255,255);
			const video::SColor colors[] = {color,color,color,color};
			core::rect<s32> rect(0,0,16,16);
			rect += p;
			driver->draw2DImage(texture, rect,
				core::rect<s32>(core::position2d<s32>(0,0),
				core::dimension2di(texture->getOriginalSize())),
				NULL, colors, true);
			p += v2s32(16,0);
		}
		if (barData[k].halfcount % 2 == 1) {
			const video::SColor color(255,255,255,255);
			const video::SColor colors[] = {color,color,color,color};
			core::rect<s32> rect(0,0,16/2,16);
			rect += p;
			core::dimension2di srcd(texture->getOriginalSize());
			srcd.Width /= 2;
			driver->draw2DImage(texture, rect,
				core::rect<s32>(core::position2d<s32>(0,0), srcd),
				NULL, colors, true);
			p += v2s32(16,0);
		}
		bar_base.Y -= 20;
	}
	if (selected != L"") {
		v2u32 dim = font->getDimension(selected.c_str());
		v2s32 sdim(dim.X,dim.Y);
		v2s32 p = pos + v2s32(170, -(24+(sdim.Y-16)));

		core::rect<s32> rect2(
			p,
			sdim
		);
		font->draw(selected.c_str(), rect2, video::SColor(255,255,255,255), false, false, NULL);
	}
}
void draw_hud(
	video::IVideoDriver *driver,
	gui::IGUIFont *font,
	v2s32 screensize,
	s32 imgsize,
	s32 itemcount,
	Inventory *inventory,
	bool have_health,
	s32 halfheartcount,
	bool have_suffocation,
	s32 halfbubblecount,
	bool have_hunger,
	s32 halfhungercount,
	int crosshair
)
{
	InventoryList *mainlist = inventory->getList("main");
	if (mainlist == NULL) {
		errorstream<<"draw_hotbar(): mainlist == NULL"<<std::endl;
		return;
	}

	// background
	{
		const video::SColor color(255,255,255,255);
		const video::SColor colors[] = {color,color,color,color};
		video::ITexture *texture = driver->getTexture(getTexturePath("ringbg.png").c_str());
		core::rect<s32> rect(screensize.X-165,screensize.Y-165,screensize.X-19,screensize.Y-19);

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

	// wielded item
	{
		InventoryItem *item = mainlist->getItem(g_selected_item);
		const video::SColor color(255,255,255,255);
		const video::SColor colors[] = {color,color,color,color};
		core::rect<s32> rect(screensize.X-124,screensize.Y-124,screensize.X-60,screensize.Y-60);
		video::ITexture *texture = NULL;

		std::wstring selected = L"";
		std::wstring txt = L"";

		if (item != NULL) {
			texture = item->getImageRaw();
			if (texture == NULL)
				texture = item->getImage();
			std::wstring name = item->getGuiName();
			if (name != L"")
				selected = name;
			content_t type = item->getContent();
			if ((type&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK || (type&CONTENT_CLOTHESITEM_MASK) == CONTENT_CLOTHESITEM_MASK) {
				float w = item->getWear();
				w = (100.0/65535.0)*w;
				txt = itows(100-w);
				txt += L"%";
			}else{
				txt = itows(item->getCount());
			}
		}else{
			texture = driver->getTexture(getTexturePath("wieldhand.png").c_str());
		}

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

		// item count/wear
		if (txt != L"") {
			v2u32 dim = font->getDimension(txt.c_str());
			v2s32 sdim(dim.X,dim.Y);
			v2s32 p(screensize.X-92,screensize.Y-44);
			p -= v2s32(sdim.X/2, sdim.Y/2);
			core::rect<s32> rect2(
				p,
				sdim
			);
			font->draw(txt.c_str(), rect2, video::SColor(255,255,255,255), false, false, NULL);
		}
		// tool name
		if (selected != L"") {
			v2u32 dim = font->getDimension(selected.c_str());
			v2s32 sdim(dim.X,dim.Y);
			v2s32 p = v2s32(screensize.X-92,screensize.Y-30);
			p -= v2s32(sdim.X/2, sdim.Y/2);
			if (sdim.X > 175)
				p.X -= (sdim.X-175)/2;

			core::rect<s32> rect2(
				p,
				sdim
			);
			font->draw(selected.c_str(), rect2, video::SColor(255,255,255,255), false, false, NULL);
		}
	}

	// rest of the wieldring
	{
		const video::SColor color(255,255,255,255);
		const video::SColor colors[] = {color,color,color,color};

		v2s32 pos[8] = {
			v2s32(0,-56),
			v2s32(39,-39),
			v2s32(56,0),
			v2s32(39,39),
			v2s32(0,56),
			v2s32(-39,39),
			v2s32(-56,0),
			v2s32(-39,-39)
		};

		core::rect<s32> base_rect(screensize.X-104,screensize.Y-104,screensize.X-80,screensize.Y-80);
		for (s32 i=g_selected_item+1, p=3; ; i++,p--) {
			if (i >= itemcount)
				i = 0;
			if (i == g_selected_item)
				break;
			if (p < 0)
				p = 7;
			InventoryItem *item = mainlist->getItem(i);

			core::rect<s32> rect = base_rect + pos[p];
			video::ITexture *texture = NULL;

			if (item == NULL)
				continue;

			texture = item->getImageRaw();
			if (texture == NULL)
				texture = item->getImage();

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
	}

	// inventory status
	{
		const char* states[3] = {
			"inventory.png",
			"inventory_full.png",
			"inventory_max.png"
		};
		int state = 2;
		{
			u32 count = mainlist->getSize();
			for (u32 i=0; i<count; i++) {
				InventoryItem *item = mainlist->getItem(i);
				if (!item) {
					state = 0;
					break;
				}
				if (state == 2 && item->freeSpace())
					state = 1;
			}
		}
		const video::SColor color(255,255,255,255);
		const video::SColor colors[] = {color,color,color,color};
		video::ITexture *texture = driver->getTexture(getTexturePath(states[state]).c_str());
		core::rect<s32> rect(screensize.X-51,screensize.Y-186,screensize.X-19,screensize.Y-154);

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

	// health
	if (have_health) {
		int c = 55+(halfheartcount*10);
		const video::SColor color(220,c,c,c);
		const video::SColor colors[] = {color,color,color,color};
		video::ITexture *texture = driver->getTexture(getTexturePath("heart.png").c_str());
		core::rect<s32> rect(60,screensize.Y-108,108,screensize.Y-60);
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

		std::wstring txt = itows(halfheartcount*5);
		txt += L"%";

		v2u32 dim = font->getDimension(txt.c_str());
		v2s32 sdim(dim.X,dim.Y);
		v2s32 p(84,screensize.Y-84);
		p -= v2s32(sdim.X/2, sdim.Y/2);
		core::rect<s32> rect2(
			p,
			sdim
		);
		font->draw(txt.c_str(), rect2, video::SColor(255,255,255,255), false, false, NULL);
	}
	// air
	if (have_suffocation && halfbubblecount<20) {
		int c = 55+(halfbubblecount*10);
		const video::SColor color(255,255,c,c);
		const video::SColor colors[] = {color,color,color,color};
		video::ITexture *texture = driver->getTexture(getTexturePath("bubble.png").c_str());
		core::rect<s32> rect(100,screensize.Y-68,132,screensize.Y-36);
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

		std::wstring txt = itows(halfbubblecount*5);
		txt += L"%";

		v2u32 dim = font->getDimension(txt.c_str());
		v2s32 sdim(dim.X,dim.Y);
		v2s32 p(116,screensize.Y-52);
		p -= v2s32(sdim.X/2, sdim.Y/2);
		core::rect<s32> rect2(
			p,
			sdim
		);
		font->draw(txt.c_str(), rect2, video::SColor(255,255,255,255), false, false, NULL);
	}
	// hunger
	if (have_hunger) {
		int c = 55+(halfhungercount*10);
		const video::SColor color(255,255,c,c);
		const video::SColor colors[] = {color,color,color,color};
		video::ITexture *texture = driver->getTexture(getTexturePath("harvested_carrot.png").c_str());
		core::rect<s32> rect(36,screensize.Y-68,68,screensize.Y-36);
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

		std::wstring txt = itows(halfhungercount*5);
		txt += L"%";

		v2u32 dim = font->getDimension(txt.c_str());
		v2s32 sdim(dim.X,dim.Y);
		v2s32 p(52,screensize.Y-52);
		p -= v2s32(sdim.X/2, sdim.Y/2);
		core::rect<s32> rect2(
			p,
			sdim
		);
		font->draw(txt.c_str(), rect2, video::SColor(255,255,255,255), false, false, NULL);
	}

	// crosshair
	if (crosshair) {
		const video::SColor color(220,255,255,255);
		const video::SColor colors[] = {color,color,color,color};
		std::string tex("");
		if (crosshair == 1) {
			tex = getTexturePath("crosshair_unfocused.png");
		}else{
			tex = getTexturePath("crosshair_focused.png");
		}
		video::ITexture *texture = driver->getTexture(tex.c_str());
		core::rect<s32> rect((screensize.X/2)-16,(screensize.Y/2)-16,(screensize.X/2)+16,(screensize.Y/2)+16);
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
}

/*
	Find what the player is pointing at
*/
void getPointedNode(Client *client, v3f player_position,
		v3f camera_direction, v3f camera_position,
		bool &nodefound, core::line3d<f32> shootline,
		v3s16 &nodepos, v3s16 &neighbourpos, v3s16 camera_offset,
		core::aabbox3d<f32> &nodehilightbox,
		f32 d)
{
	f32 mindistance = BS * 1001;

	v3s16 pos_i = floatToInt(player_position, BS);

	/*infostream<<"pos_i=("<<pos_i.X<<","<<pos_i.Y<<","<<pos_i.Z<<")"
			<<std::endl;*/
//printf("%f,%f,%f - %f,%f,%f\n",shootline.start.X,shootline.start.Y,shootline.start.Z,shootline.end.X,shootline.end.Y,shootline.end.Z);

	s16 a = d;
	s16 ystart = pos_i.Y + 0 - (camera_direction.Y<0 ? a : 1);
	s16 zstart = pos_i.Z - (camera_direction.Z<0 ? a : 1);
	s16 xstart = pos_i.X - (camera_direction.X<0 ? a : 1);
	s16 yend = pos_i.Y + 1 + (camera_direction.Y>0 ? a : 1);
	s16 zend = pos_i.Z + (camera_direction.Z>0 ? a : 1);
	s16 xend = pos_i.X + (camera_direction.X>0 ? a : 1);
	InventoryItem *wield = (InventoryItem*)client->getLocalPlayer()->getWieldItem();
	bool wield_is_hand = (wield == NULL);
	bool wield_is_tool = (wield && wield->getContent()&CONTENT_TOOLITEM_MASK);
	bool wield_is_craft = (wield && wield->getContent()&CONTENT_CRAFTITEM_MASK);
	bool wield_is_material = (!wield_is_hand && !wield_is_tool && !wield_is_craft);

	content_t content = CONTENT_IGNORE;

	for(s16 y = ystart; y <= yend; y++)
	for(s16 z = zstart; z <= zend; z++)
	for(s16 x = xstart; x <= xend; x++)
	{
//printf("%d,%d,%d\n",x,y,z);
		MapNode n;
		try
		{
			n = client->getNode(v3s16(x,y,z));
			if (content_features(n.getContent()).pointable == false) {
				if (content_features(n.getContent()).liquid_type != LIQUID_SOURCE)
					continue;
				if (!wield || content_toolitem_features(wield->getContent()).liquids_pointable == false)
					continue;
			}else if (content_features(n.getContent()).material_pointable == false && wield_is_material) {
					continue;
			}
		}
		catch(InvalidPositionException &e)
		{
			continue;
		}

		v3s16 np(x,y,z);
		v3f npf = intToFloat(np, BS);

		f32 d = 0.01;

		v3s16 dirs[6] = {
			v3s16(0,0,1), // back
			v3s16(0,1,0), // top
			v3s16(1,0,0), // right
			v3s16(0,0,-1), // front
			v3s16(0,-1,0), // bottom
			v3s16(-1,0,0), // left
		};

		/*
			Meta-objects
		*/
		if(n.getContent() == CONTENT_TORCH) {
			v3s16 dir = unpackDir(n.param2);
			v3f dir_f = v3f(dir.X, dir.Y, dir.Z);
			dir_f *= BS/2 - BS/6 - BS/20;
			v3f cpf = npf + dir_f;
			f32 distance = (cpf - camera_position).getLength();

			core::aabbox3d<f32> box;

			// bottom
			if(dir == v3s16(0,-1,0))
			{
				box = core::aabbox3d<f32>(
					npf - v3f(BS/6, BS/2, BS/6),
					npf + v3f(BS/6, -BS/2+BS/3*2, BS/6)
				);
			}
			// top
			else if(dir == v3s16(0,1,0))
			{
				box = core::aabbox3d<f32>(
					npf - v3f(BS/6, -BS/2+BS/3*2, BS/6),
					npf + v3f(BS/6, BS/2, BS/6)
				);
			}
			// side
			else
			{
				box = core::aabbox3d<f32>(
					cpf - v3f(BS/6, BS/3, BS/6),
					cpf + v3f(BS/6, BS/3, BS/6)
				);
			}

			if(distance < mindistance)
			{
				if(box.intersectsWithLine(shootline))
				{
					nodefound = true;
					nodepos = np;
					content = n.getContent();
					neighbourpos = np;
					mindistance = distance;
					box.MinEdge -= intToFloat(camera_offset,BS);
					box.MaxEdge -= intToFloat(camera_offset,BS);
					nodehilightbox = box;
				}
			}
		}else if(n.getContent() == CONTENT_RAIL) {
			v3s16 dir = unpackDir(n.param0);
			v3f dir_f = v3f(dir.X, dir.Y, dir.Z);
			dir_f *= BS/2 - BS/6 - BS/20;
			v3f cpf = npf + dir_f;
			f32 distance = (cpf - camera_position).getLength();

			float d = (float)BS/8;
			v3f vertices[4] =
			{
				v3f(BS/2, -BS/2+d, -BS/2),
				v3f(-BS/2, -BS/2, BS/2),
			};

			for(s32 i=0; i<2; i++)
			{
				vertices[i] += npf;
			}

			core::aabbox3d<f32> box;

			box = core::aabbox3d<f32>(vertices[0]);
			box.addInternalPoint(vertices[1]);

			if(distance < mindistance)
			{
				if(box.intersectsWithLine(shootline))
				{
					nodefound = true;
					nodepos = np;
					content = n.getContent();
					neighbourpos = np;
					mindistance = distance;
					box.MinEdge -= intToFloat(camera_offset,BS);
					box.MaxEdge -= intToFloat(camera_offset,BS);
					nodehilightbox = box;
				}
			}
		/*
			Roofs and Node boxes
		*/
		}else if (
			content_features(n).draw_type == CDT_NODEBOX
			|| content_features(n).draw_type == CDT_NODEBOX_META
			|| content_features(n).draw_type == CDT_WIRELIKE
			|| content_features(n).draw_type == CDT_3DWIRELIKE
			|| content_features(n).draw_type == CDT_FENCELIKE
			|| content_features(n).draw_type == CDT_WALLLIKE
			|| content_features(n).draw_type == CDT_STAIRLIKE
			|| content_features(n).draw_type == CDT_SLABLIKE
		) {
			f32 distance = (npf - camera_position).getLength();

			if (distance < mindistance) {
				aabb3f box;
				aabb3f nhbox(0.5*BS,0.5*BS,0.5*BS,-0.5*BS,-0.5*BS,-0.5*BS);
				bool hit = false;
				std::vector<NodeBox> boxes = content_features(n).getNodeBoxes(n);
				for (std::vector<NodeBox>::iterator b = boxes.begin(); b != boxes.end(); b++) {
					box = b->m_box;

					if (nhbox.MinEdge.X > box.MinEdge.X)
						nhbox.MinEdge.X = box.MinEdge.X;
					if (nhbox.MinEdge.Y > box.MinEdge.Y)
						nhbox.MinEdge.Y = box.MinEdge.Y;
					if (nhbox.MinEdge.Z > box.MinEdge.Z)
						nhbox.MinEdge.Z = box.MinEdge.Z;
					if (nhbox.MaxEdge.X < box.MaxEdge.X)
						nhbox.MaxEdge.X = box.MaxEdge.X;
					if (nhbox.MaxEdge.Y < box.MaxEdge.Y)
						nhbox.MaxEdge.Y = box.MaxEdge.Y;
					if (nhbox.MaxEdge.Z < box.MaxEdge.Z)
						nhbox.MaxEdge.Z = box.MaxEdge.Z;

					box.MinEdge += npf;
					box.MaxEdge += npf;

					if (box.intersectsWithLine(shootline)) {
						for(u16 i=0; i<6; i++) {
							v3f dir_f = v3f(dirs[i].X,
									dirs[i].Y, dirs[i].Z);
							v3f centerpoint = npf + dir_f * BS/2;
							f32 distance =
									(centerpoint - camera_position).getLength();

							if(distance < mindistance)
							{
								core::CMatrix4<f32> m;
								m.buildRotateFromTo(v3f(0,0,1), dir_f);

								// This is the back face
								v3f corners[2] = {
									v3f(BS/2, BS/2, BS/2),
									v3f(-BS/2, -BS/2, BS/2+d)
								};

								for(u16 j=0; j<2; j++)
								{
									m.rotateVect(corners[j]);
									corners[j] += npf;
								}

								core::aabbox3d<f32> facebox(corners[0]);
								facebox.addInternalPoint(corners[1]);

								if(facebox.intersectsWithLine(shootline))
								{
									nodefound = true;
									nodepos = np;
									content = n.getContent();
									neighbourpos = np + dirs[i];
									mindistance = distance;

									hit = true;
								}
							} // if distance < mindistance
						} // for dirs
					}
				}
				if (hit) {
					nhbox.MinEdge -= 0.002;
					nhbox.MaxEdge += 0.002;
					v3f nodepos_f = intToFloat(nodepos-camera_offset, BS);
					nhbox.MinEdge += nodepos_f;
					nhbox.MaxEdge += nodepos_f;
					nodehilightbox = nhbox;
				}
				boxes.clear();
			}
		/*
			Regular blocks
		*/
		}else{
			for(u16 i=0; i<6; i++)
			{
				v3f dir_f = v3f(dirs[i].X,
						dirs[i].Y, dirs[i].Z);
				v3f centerpoint = npf + dir_f * BS/2;
				f32 distance =
						(centerpoint - camera_position).getLength();
//printf("%f %f - %d,%d,%d\n",distance, mindistance,x,y,z);

				if(distance < mindistance)
				{
					core::CMatrix4<f32> m;
					m.buildRotateFromTo(v3f(0,0,1), dir_f);

					// This is the back face
					v3f corners[2] = {
						v3f(BS/2, BS/2, BS/2),
						v3f(-BS/2, -BS/2, BS/2+d)
					};

					for(u16 j=0; j<2; j++)
					{
						m.rotateVect(corners[j]);
						corners[j] += npf;
					}

					core::aabbox3d<f32> facebox(corners[0]);
					facebox.addInternalPoint(corners[1]);

					if(facebox.intersectsWithLine(shootline))
					{
						nodefound = true;
						nodepos = np;
						content = n.getContent();
						neighbourpos = np + dirs[i];
						mindistance = distance;

						//nodehilightbox = facebox;

						const float d = 0.502;
						core::aabbox3d<f32> nodebox
								(-BS*d, -BS*d, -BS*d, BS*d, BS*d, BS*d);
						v3f nodepos_f = intToFloat(nodepos-camera_offset, BS);
						nodebox.MinEdge += nodepos_f;
						nodebox.MaxEdge += nodepos_f;
						nodehilightbox = nodebox;
					}
				} // if distance < mindistance
			} // for dirs
		} // regular block
	} // for coords
	if (nodefound) {
		client->setPointedNode(nodepos);
		client->setPointedContent(content);
	}else{
		client->setPointedContent(CONTENT_IGNORE);
	}
}

void update_skybox(video::IVideoDriver* driver,
		scene::ISceneManager* smgr, scene::ISceneNode* &skybox,
		float brightness)
{
	if (skybox)
		skybox->remove();

	/*// Disable skybox if FarMesh is enabled
	if(g_settings->getBool("enable_farmesh"))
		return;*/

	if (brightness >= 0.5) {
		skybox = smgr->addSkyBoxSceneNode(
			driver->getTexture(getTexturePath("skybox2.png").c_str()),
			driver->getTexture(getTexturePath("skybox3.png").c_str()),
			driver->getTexture(getTexturePath("skybox1.png").c_str()),
			driver->getTexture(getTexturePath("skybox1.png").c_str()),
			driver->getTexture(getTexturePath("skybox1.png").c_str()),
			driver->getTexture(getTexturePath("skybox1.png").c_str()));
	}else if(brightness >= 0.2) {
		skybox = smgr->addSkyBoxSceneNode(
			driver->getTexture(getTexturePath("skybox2_dawn.png").c_str()),
			driver->getTexture(getTexturePath("skybox3_dawn.png").c_str()),
			driver->getTexture(getTexturePath("skybox1_dawn.png").c_str()),
			driver->getTexture(getTexturePath("skybox1_dawn.png").c_str()),
			driver->getTexture(getTexturePath("skybox1_dawn.png").c_str()),
			driver->getTexture(getTexturePath("skybox1_dawn.png").c_str()));
	}else{
		skybox = smgr->addSkyBoxSceneNode(
			driver->getTexture(getTexturePath("skybox2_night.png").c_str()),
			driver->getTexture(getTexturePath("skybox3_night.png").c_str()),
			driver->getTexture(getTexturePath("skybox1_night.png").c_str()),
			driver->getTexture(getTexturePath("skybox1_night.png").c_str()),
			driver->getTexture(getTexturePath("skybox1_night.png").c_str()),
			driver->getTexture(getTexturePath("skybox1_night.png").c_str()));
	}
}

/*
	Draws a screen with logo and text on it.
	Text will be removed when the screen is drawn the next time.
*/
void drawLoadingScreen(irr::IrrlichtDevice* device, const std::wstring msg)
{
	if (device == NULL)
		return;
	device->run();
	video::IVideoDriver* driver = device->getVideoDriver();
	if (driver == NULL)
		return;
	core::dimension2d<u32> screensize = driver->getScreenSize();
	s32 x = (screensize.Width/2);
	s32 y = (screensize.Height/2);

	driver->beginScene(true, true, video::SColor(255,0,0,0));

	video::ITexture *logotexture = driver->getTexture(getTexturePath("menulogo.png").c_str());
	if (logotexture) {
		core::rect<s32> rect(x-100,y-150,x+100,y+50);
		driver->draw2DImage(logotexture, rect,
			core::rect<s32>(core::position2d<s32>(0,0),
			core::dimension2di(logotexture->getSize())),
			NULL, NULL, true);
	}
	if (guienv) {
		std::wstring m;
		if (msg != L"") {
			m = msg;
		}else{
			m = wgettext("Loading");
		}
		core::dimension2d<u32> textsize = guienv->getSkin()->getFont()->getDimension(m.c_str());
		core::rect<s32> rect(x-(textsize.Width/2), y+50, x+textsize.Width, y+50+textsize.Height);
		gui::IGUIStaticText *guitext = guienv->addStaticText(m.c_str(),rect);
		guienv->drawAll();
		guitext->remove();
	}

	driver->endScene();
}

/* Profiler display */

void update_profiler_gui(gui::IGUIStaticText *guitext_profiler,
		gui::IGUIFont *font, u32 text_height,
		u32 show_profiler, u32 show_profiler_max)
{
	if (show_profiler == 0) {
		guitext_profiler->setVisible(false);
	}else{

		std::ostringstream os(std::ios_base::binary);
		g_profiler->printPage(os, show_profiler, show_profiler_max);
		std::wstring text = narrow_to_wide(os.str());
		guitext_profiler->setText(text.c_str());
		guitext_profiler->setVisible(true);

		s32 w = font->getDimension(text.c_str()).Width;
		if(w < 400)
			w = 400;
		core::rect<s32> rect(6, 4+(text_height+5)*2, 12+w,
				8+(text_height+5)*2 +
				font->getDimension(text.c_str()).Height);
		guitext_profiler->setRelativePosition(rect);
		guitext_profiler->setVisible(true);
	}
}

void the_game(
	bool &kill,
	bool random_input,
	InputHandler *input,
	IrrlichtDevice *device,
	gui::IGUIFont* font,
	std::string map_dir,
	std::string playername,
	std::string password,
	std::string address,
	u16 port,
	std::wstring &error_message,
	std::string configpath,
	ISoundManager *sound
)
{
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// Calculate text height using the font
	u32 text_height = font->getDimension(L"Random test string").Height;

	v2u32 screensize(0,0);
	v2u32 last_screensize(0,0);
	screensize = driver->getScreenSize();

	const s32 hotbar_itemcount = 8;
	s32 hotbar_imagesize = 48;

	// The color of the sky
	video::SColor bgcolor_bright = video::SColor(255,167,167,200);
	/*
		Draw "Loading" screen
	*/
	//draw_load_screen(L"Loading...", driver, font);
	drawLoadingScreen(device,wgettext("Loading..."));

	/*
		Create server.
		SharedPtr will delete it when it goes out of scope.
	*/
	SharedPtr<Server> server;
	if(address == ""){
		//draw_load_screen(L"Creating server...", driver, font);
		drawLoadingScreen(device,wgettext("Creating server..."));
		infostream<<"Creating server"<<std::endl;
		server = new Server(map_dir, configpath);
		server->start(port);
	}

	/*
		Create client
	*/

	//draw_load_screen(L"Creating client...", driver, font);
	drawLoadingScreen(device,wgettext("Creating client..."));
	infostream<<"Creating client"<<std::endl;
	MapDrawControl draw_control;
	Client client(device, playername.c_str(), password, draw_control, sound);

	drawLoadingScreen(device,wgettext("Resolving address..."));
	Address connect_address(0,0,0,0, port);
	try{
		if(address == "")
			connect_address.setAddress(127,0,0,1);
		else
			connect_address.Resolve(address.c_str());
	}
	catch(ResolveError &e)
	{
		errorstream<<"Couldn't resolve address"<<std::endl;
		error_message = wgettext("Couldn't resolve address");
		return;
	}

	/*
		Attempt to connect to the server
	*/

	infostream<<"Connecting to server at ";
	connect_address.print(&infostream);
	infostream<<std::endl;
	client.connect(connect_address);

	bool could_connect = false;

	try{
		float time_counter = 0.0;
		for(;;)
		{
			if(client.connectedAndInitialized())
			{
				could_connect = true;
				break;
			}
			if(client.accessDenied())
			{
				break;
			}
			// Wait for 10 seconds
			if(time_counter >= 10.0)
			{
				break;
			}

			char buff[512];
			int tot = (10.0 - time_counter + 1.0);
			snprintf(
				buff,
				512,
				ngettext(
					"Connecting to server... (timeout in %d second)",
					"Connecting to server... (timeout in %d seconds)",
					tot
				),
				tot
			);
			//draw_load_screen(ss.str(), driver, font);
			drawLoadingScreen(device,narrow_to_wide(buff));
			// Update client and server
			client.step(0.1);

			if (server != NULL) {
				if (!server->step(0.1)) {
					could_connect = false;
					break;
				}
			}

			// Delay a bit
			sleep_ms(100);
			time_counter += 0.1;
		}
	}
	catch(con::PeerNotFoundException &e)
	{}

	if (could_connect == false) {
		if (client.accessDenied()) {
			wchar_t buff[512];
			swprintf(buff,512,wgettext("Access denied. Reason: %ls"),client.accessDeniedReason().c_str());
			error_message = std::wstring(buff);
			errorstream<<wide_to_narrow(error_message)<<std::endl;
		}else if (server != NULL) {
			error_message = wgettext("Unable to Connect (port already in use?).");
			errorstream<<"Timed out."<<std::endl;
		}else{
			error_message = wgettext("Connection timed out.");
			errorstream<<"Timed out."<<std::endl;
		}
		return;
	}

	/*
		Create skybox
	*/
	float old_brightness = 1.0;
	scene::ISceneNode* skybox = NULL;
	update_skybox(driver, smgr, skybox, 1.0);

	/*
		Create the camera node
	*/
	Camera camera(smgr, draw_control, &client);
	if (!camera.successfullyCreated(error_message))
		return;

	f32 camera_yaw = 0; // "right/left"
	f32 camera_pitch = 0; // "up/down"

	/*
		Clouds
	*/

	float cloud_height = BS*100;
	Clouds *clouds = NULL;
	if (g_settings->getBool("enable_clouds"))
		clouds = new Clouds(smgr->getRootSceneNode(), smgr, -1, cloud_height, time(0));

	/*
		FarMesh
	*/

	FarMesh *farmesh = NULL;
	if (g_settings->getBool("enable_farmesh"))
		farmesh = new FarMesh(smgr->getRootSceneNode(), smgr, -1, client.getMapSeed(), client.getMapType(), &client);

	/*
		Move into game
	*/

	/*
		Add some gui stuff
	*/

	// First line of debug text
	gui::IGUIStaticText *guitext = guienv->addStaticText(
			L"Voxelands",
			core::rect<s32>(5, 5, 795, 5+text_height),
			false, false);
	// Second line of debug text
	gui::IGUIStaticText *guitext2 = guienv->addStaticText(
			L"",
			core::rect<s32>(5, 3+(text_height)*1, 795, (5+text_height)*2),
			false, false);
	// At the middle of the screen
	// Object infos are shown in this
	gui::IGUIStaticText *guitext_info = guienv->addStaticText(
			L"",
			core::rect<s32>(0,0,500,text_height+5) + v2s32(100,200),
			false, false);

	// Status text (displays info when showing and hiding GUI stuff, etc.)
	gui::IGUIStaticText *guitext_status = guienv->addStaticText(
			L"<Status>",
			core::rect<s32>(0,0,0,0),
			false, false);
	guitext_status->setVisible(false);

	std::wstring statustext;
	float statustext_time = 0;

	// Chat text
	gui::IGUIStaticText *guitext_chat = guienv->addStaticText(
			L"",
			core::rect<s32>(0,0,0,0),
			//false, false); // Disable word wrap as of now
			false, true);
	core::list<ChatLine> chat_lines;

	// Profiler text (size is updated when text is updated)
	gui::IGUIStaticText *guitext_profiler = guienv->addStaticText(
			L"<Profiler>",
			core::rect<s32>(0,0,0,0),
			false, false);
	guitext_profiler->setBackgroundColor(video::SColor(80,0,0,0));
	guitext_profiler->setVisible(false);

	// Launch pause menu
	(new GUIPauseMenu(guienv, guiroot, -1, g_gamecallback,
			&g_menumgr))->drop();

	/*
		Some statistics are collected in these
	*/
	u32 drawtime = 0;

	core::list<float> frametime_log;

	float nodig_delay_counter = 0.0;
	float dig_time = 0.0;
	u16 dig_index = 0;
	v3s16 nodepos_old(-32768,-32768,-32768);

	float damage_flash_timer = 0;
	s16 farmesh_range = 20*MAP_BLOCKSIZE;

	const float object_hit_delay = 0.5;
	float object_hit_delay_timer = 0.0;

	bool invert_mouse = g_settings->getBool("invert_mouse");

	bool respawn_menu_active = false;

	bool show_hud = true;
	bool show_chat = true;
	bool force_fog_off = false;
	bool disable_camera_update = false;
	bool show_debug = g_settings->getBool("show_debug");
	bool show_debug_frametime = false;
	u32 show_profiler = 0;
	u32 show_profiler_max = 3;  // Number of pages
	float fps_max = g_settings->getFloat("fps_max");
	float profiler_print_interval = g_settings->getFloat("profiler_print_interval");

	bool free_move = g_settings->getBool("free_move");
	bool fast_move = g_settings->getBool("fast_move");
	f32 mouse_sensitivity = g_settings->getFloat("mouse_sensitivity");
	bool highlight_selected_node = true;
	if (g_settings->exists("selected_node") && g_settings->get("selected_node") == "outline")
		highlight_selected_node = false;
	bool enable_particles = g_settings->getBool("enable_particles");
	bool enable_fog = g_settings->getBool("enable_fog");
	bool old_hotbar = g_settings->getBool("old_hotbar");

	/*
		Main loop
	*/

	bool first_loop_after_window_activation = true;

	// TODO: Convert the static interval timers to these
	// Interval limiter for profiler
	IntervalLimiter m_profiler_interval;

	// Time is in milliseconds
	// NOTE: getRealTime() causes strange problems in wine (imprecision?)
	// NOTE: So we have to use getTime() and call run()s between them
	u32 lasttime = device->getTimer()->getTime();
	v3s16 lastpointed(0,0,0);

	while (device->run() && kill == false) {
		//std::cerr<<"frame"<<std::endl;

		if (client.accessDenied()) {
			error_message = wgettext("Access denied. Reason: ")
					+client.accessDeniedReason();
			errorstream<<wide_to_narrow(error_message)<<std::endl;
			break;
		}

		if (g_gamecallback->disconnect_requested) {
			g_gamecallback->disconnect_requested = false;
			break;
		}

		if (g_gamecallback->changepassword_requested) {
			(new GUIPasswordChange(guienv, guiroot, -1,
				&g_menumgr, &client))->drop();
			g_gamecallback->changepassword_requested = false;
		}

		/*
			Process TextureSource's queue
		*/
		((TextureSource*)g_texturesource)->processQueue();

		/*
			Random calculations
		*/
		last_screensize = screensize;
		screensize = driver->getScreenSize();
		v2s32 displaycenter(screensize.X/2,screensize.Y/2);

		// Resize hotbar
		if (screensize.Y <= 800) {
			hotbar_imagesize = 32;
		}else if (screensize.Y <= 1280) {
			hotbar_imagesize = 48;
		}else{
			hotbar_imagesize = 64;
		}

		// Hilight boxes collected during the loop and displayed
		core::list< core::aabbox3d<f32> > hilightboxes;

		// Info text
		std::wstring infotext;

		// Time of frame without fps limit
		float busytime;
		u32 busytime_u32;
		{
			// not using getRealTime is necessary for wine
			u32 time = device->getTimer()->getTime();
			if (time > lasttime) {
				busytime_u32 = time - lasttime;
			}else{
				busytime_u32 = 0;
			}
			busytime = busytime_u32 / 1000.0;
		}

		// Necessary for device->getTimer()->getTime()
		device->run();

		/*
			FPS limiter
		*/

		{
			u32 frametime_min = 1000./fps_max;

			if (busytime_u32 < frametime_min) {
				u32 sleeptime = frametime_min - busytime_u32;
				device->sleep(sleeptime);
			}
		}

		// Necessary for device->getTimer()->getTime()
		device->run();

		/*
			Time difference calculation
		*/
		f32 dtime; // in seconds

		u32 time = device->getTimer()->getTime();
		if (time > lasttime) {
			dtime = (time - lasttime) / 1000.0;
		}else{
			dtime = 0;
		}
		lasttime = time;

		/* Run timers */

		object_hit_delay_timer -= dtime;

		g_profiler->add("Elapsed time", dtime);
		g_profiler->avg("FPS", 1./dtime);

		/*
			Log frametime for visualization
		*/
		frametime_log.push_back(dtime);
		if (frametime_log.size() > 100) {
			core::list<float>::Iterator i = frametime_log.begin();
			frametime_log.erase(i);
		}

		/*
			Time average and jitter calculation
		*/

		static f32 dtime_avg1 = 0.0;
		dtime_avg1 = dtime_avg1 * 0.96 + dtime * 0.04;
		f32 dtime_jitter1 = dtime - dtime_avg1;

		static f32 dtime_jitter1_max_sample = 0.0;
		static f32 dtime_jitter1_max_fraction = 0.0;
		{
			static f32 jitter1_max = 0.0;
			static f32 counter = 0.0;
			if (dtime_jitter1 > jitter1_max)
				jitter1_max = dtime_jitter1;
			counter += dtime;
			if (counter > 0.0) {
				counter -= 3.0;
				dtime_jitter1_max_sample = jitter1_max;
				dtime_jitter1_max_fraction = dtime_jitter1_max_sample / (dtime_avg1+0.001);
				jitter1_max = 0.0;
			}
		}

		/*
			Busytime average and jitter calculation
		*/

		static f32 busytime_avg1 = 0.0;
		busytime_avg1 = busytime_avg1 * 0.98 + busytime * 0.02;
		f32 busytime_jitter1 = busytime - busytime_avg1;

		{
			static f32 jitter1_max = 0.0;
			static f32 jitter1_min = 0.0;
			static f32 counter = 0.0;
			if (busytime_jitter1 > jitter1_max)
				jitter1_max = busytime_jitter1;
			if (busytime_jitter1 < jitter1_min)
				jitter1_min = busytime_jitter1;
			counter += dtime;
			if (counter > 0.0) {
				counter -= 3.0;
				jitter1_max = 0.0;
				jitter1_min = 0.0;
			}
		}

		/*
			Debug info for client
		*/
		{
			static float counter = 0.0;
			counter -= dtime;
			if (counter < 0) {
				counter = 30.0;
				client.printDebugInfo(infostream);
			}
		}

		/*
			Profiler
		*/
		bool print_to_log = true;
		if (profiler_print_interval == 0) {
			print_to_log = false;
			profiler_print_interval = 5;
		}
		if (m_profiler_interval.step(dtime, profiler_print_interval)) {
			if (print_to_log) {
				infostream<<"Profiler:"<<std::endl;
				g_profiler->print(infostream);
			}

			update_profiler_gui(guitext_profiler, font, text_height,
					show_profiler, show_profiler_max);

			g_profiler->clear();
		}

		/*
			Direct handling of user input
		*/

		// Reset input if window not active or some menu is active
		if (device->isWindowActive() == false || noMenuActive() == false)
			input->clear();

		// Input handler step() (used by the random input generator)
		input->step(dtime);

		/*
			Launch menus according to keys
		*/
		if (input->wasKeyDown(getKeySetting(VLKC_INVENTORY))) {
			infostream<<"the_game: Launching inventory"<<std::endl;

			GUIFormSpecMenu *menu = new GUIFormSpecMenu(guienv, guiroot, -1, &g_menumgr, &client);

			InventoryLocation inventoryloc;
			inventoryloc.setCurrentPlayer();

			PlayerInventoryFormIO *fio = new PlayerInventoryFormIO(&client);
			assert(fio);
			menu->setFormSpec(fio->getForm(), inventoryloc);
			menu->setFormIO(fio);
			menu->drop();
		}else if (input->wasKeyDown(EscapeKey)) {
			infostream<<"the_game: Launching pause menu"<<std::endl;
			// It will delete itself by itself
			(new GUIPauseMenu(guienv, guiroot, -1, g_gamecallback, &g_menumgr))->drop();

			// Move mouse cursor on top of the disconnect button
			input->setMousePos(displaycenter.X, displaycenter.Y+25);
		}else if (input->wasKeyDown(getKeySetting(VLKC_CHAT))) {
			FormIO *fio = new ChatFormIO(&client);

			(new GUITextInputMenu(guienv, guiroot, -1, &g_menumgr, fio, L""))->drop();
		}else if (input->wasKeyDown(getKeySetting(VLKC_COMMAND))) {
			FormIO *fio = new ChatFormIO(&client);

			(new GUITextInputMenu(guienv, guiroot, -1, &g_menumgr, fio, L"/"))->drop();
		}else if(input->wasKeyDown(getKeySetting(VLKC_FREEMOVE))) {
			if (free_move) {
				free_move = false;
				statustext = wgettext("free_move disabled");
				statustext_time = 0;
			}else{
				free_move = true;
				statustext = wgettext("free_move enabled");
				statustext_time = 0;
			}
		}else if(input->wasKeyDown(getKeySetting(VLKC_FASTMOVE))) {
			if (fast_move) {
				fast_move = false;
				statustext = wgettext("fast_move disabled");
				statustext_time = 0;
			}else{
				fast_move = true;
				statustext = wgettext("fast_move enabled");
				statustext_time = 0;
			}
		}else if(input->wasKeyDown(getKeySetting(VLKC_SCREENSHOT))) {
			irr::video::IImage* const image = driver->createScreenShot();
			if (image) {
				irr::c8 filename[256];
				snprintf(filename, 256, "%s" DIR_DELIM "screenshot_%u.png",
						 g_settings->get("screenshot_path").c_str(),
						 device->getTimer()->getRealTime());
				if (driver->writeImageToFile(image, filename)) {
					wchar_t buff[512];
					swprintf(buff,512,wgettext("Saved screenshot to '%s'"),filename);
					infostream<<"Saved screenshot to '"<<filename<<"'"<<std::endl;
					statustext = std::wstring(buff);
					statustext_time = 0;
				}else{
					infostream<<"Failed to save screenshot '"<<filename<<"'"<<std::endl;
				}
				image->drop();
			}
		}else if (input->wasKeyDown(getKeySetting(VLKC_TOGGLE_HUD))) {
			show_hud = !show_hud;
			if (show_hud) {
				statustext = wgettext("HUD shown");
			}else{
				statustext = wgettext("HUD hidden");
			}
			statustext_time = 0;
		}else if (input->wasKeyDown(getKeySetting(VLKC_TOGGLE_CHAT))) {
			show_chat = !show_chat;
			if (show_chat) {
				statustext = wgettext("Chat shown");
			}else{
				statustext = wgettext("Chat hidden");
			}
			statustext_time = 0;
		}else if (input->wasKeyDown(getKeySetting(VLKC_TOGGLE_FOG))) {
			force_fog_off = !force_fog_off;
			if (force_fog_off) {
				statustext = wgettext("Fog disabled");
			}else{
				statustext = wgettext("Fog enabled");
			}
			statustext_time = 0;
		}else if (input->wasKeyDown(getKeySetting(VLKC_TOGGLE_CAMERA))) {
			disable_camera_update = !disable_camera_update;
			if (disable_camera_update) {
				statustext = wgettext("Camera update disabled");
			}else{
				statustext = wgettext("Camera update enabled");
			}
			statustext_time = 0;
		}else if (input->wasKeyDown(getKeySetting(VLKC_TOGGLE_DEBUG))) {
			// Initial / 3x toggle: Chat only
			// 1x toggle: Debug text with chat
			// 2x toggle: Debug text with frametime
			if (!show_debug) {
				show_debug = true;
				show_debug_frametime = false;
				statustext = wgettext("Debug info shown");
				statustext_time = 0;
			}else if (show_debug_frametime) {
				show_debug = false;
				show_debug_frametime = false;
				statustext = wgettext("Debug info and frametime graph hidden");
				statustext_time = 0;
			}else{
				show_debug_frametime = true;
				statustext = wgettext("Frametime graph shown");
				statustext_time = 0;
			}
		}else if (input->wasKeyDown(getKeySetting(VLKC_TOGGLE_PROFILER))) {
			show_profiler = (show_profiler + 1) % (show_profiler_max + 1);

			// FIXME: This updates the profiler with incomplete values
			update_profiler_gui(guitext_profiler, font, text_height,
					show_profiler, show_profiler_max);

			if (show_profiler != 0) {
				wchar_t buff[512];
				swprintf(buff,512,wgettext("Profiler shown (page %d of %d)"),show_profiler,show_profiler_max);
				statustext = std::wstring(buff);
				statustext_time = 0;
			}else{
				statustext = wgettext("Profiler hidden");
				statustext_time = 0;
			}
		}else if (input->wasKeyDown(getKeySetting(VLKC_RANGE_PLUS))) {
			s16 range = g_settings->getS16("viewing_range_nodes_min");
			s16 range_new = range + 10;
			g_settings->set("viewing_range_nodes_min", itos(range_new));
			wchar_t buff[512];
			swprintf(buff,512,wgettext("Minimum viewing range changed to %d"),range_new);
			statustext = std::wstring(buff);
			statustext_time = 0;
		}else if (input->wasKeyDown(getKeySetting(VLKC_RANGE_MINUS))) {
			s16 range = g_settings->getS16("viewing_range_nodes_min");
			s16 range_new = range - 10;
			if (range_new < 0)
				range_new = range;
			g_settings->set("viewing_range_nodes_min",
					itos(range_new));
			wchar_t buff[512];
			swprintf(buff,512,wgettext("Minimum viewing range changed to %d"),range_new);
			statustext = std::wstring(buff);
			statustext_time = 0;
		}

		// Item selection with mouse wheel
		{
			s32 wheel = input->getMouseWheel();
			u16 max_item = MYMIN(PLAYER_INVENTORY_SIZE-1,
					hotbar_itemcount-1);

			std::string wield_sound = "wield";

			if (wheel < 0) {
				if (g_selected_item < max_item) {
					g_selected_item++;
				}else{
					g_selected_item = 0;
				}
				client.playSound(wield_sound,false);
			}else if (wheel > 0) {
				if (g_selected_item > 0) {
					g_selected_item--;
				}else{
					g_selected_item = max_item;
				}
				client.playSound(wield_sound,false);
			}
		}

		// Item selection
		for (u16 i=0; i<10; i++) {
			const KeyPress *kp = NumberKey + (i + 1) % 10;
			if (input->wasKeyDown(*kp)) {
				if (i < PLAYER_INVENTORY_SIZE && i < hotbar_itemcount) {
					g_selected_item = i;

					infostream<<"Selected item: "<<g_selected_item<<std::endl;
					std::string wield_sound = "wield";
					client.playSound(wield_sound,false);
				}
			}
		}
		if (input->wasKeyDown(getKeySetting(VLKC_SELECT_PREV))) {
			u16 max_item = MYMIN(PLAYER_INVENTORY_SIZE-1, hotbar_itemcount-1);
			if (g_selected_item > 0) {
				g_selected_item--;
			}else{
				g_selected_item = max_item;
			}
			std::string wield_sound = "wield";
			client.playSound(wield_sound,false);
		}
		if (input->wasKeyDown(getKeySetting(VLKC_SELECT_NEXT))) {
			u16 max_item = MYMIN(PLAYER_INVENTORY_SIZE-1, hotbar_itemcount-1);
			if (g_selected_item < max_item) {
				g_selected_item++;
			}else{
				g_selected_item = 0;
			}
			std::string wield_sound = "wield";
			client.playSound(wield_sound,false);
		}

		// Viewing range selection
		if (input->wasKeyDown(getKeySetting(VLKC_RANGE))) {
			draw_control.range_all = !draw_control.range_all;
			if (draw_control.range_all) {
				infostream<<"Enabled full viewing range"<<std::endl;
				statustext = wgettext("Enabled full viewing range");
				statustext_time = 0;
			}else{
				infostream<<"Disabled full viewing range"<<std::endl;
				statustext = wgettext("Disabled full viewing range");
				statustext_time = 0;
			}
		}

		// Print debug stacks
		if (input->wasKeyDown(getKeySetting(VLKC_PRINT_DEBUG))) {
			dstream<<"-----------------------------------------"
					<<std::endl;
			dstream<<DTIME<<"Printing debug stacks:"<<std::endl;
			dstream<<"-----------------------------------------"
					<<std::endl;
			debug_stacks_print();
		}

		/*
			Mouse and camera control
			NOTE: Do this before client.setPlayerControl() to not cause a camera lag of one frame
		*/

		if ((device->isWindowActive() && noMenuActive()) || random_input) {
			if (!random_input) {
				// Mac OSX gets upset if this is set every frame
				if (device->getCursorControl()->isVisible())
					device->getCursorControl()->setVisible(false);
			}

			if (first_loop_after_window_activation) {
				//infostream<<"window active, first loop"<<std::endl;
				first_loop_after_window_activation = false;
			}else{
				s32 dx = input->getMousePos().X - displaycenter.X;
				s32 dy = input->getMousePos().Y - displaycenter.Y;
				if (invert_mouse)
					dy = -dy;

				f32 d = rangelim(mouse_sensitivity, 0.01, 100.0);

				camera_yaw -= dx*d;
				camera_pitch += dy*d;
				if (camera_pitch < -89.5) camera_pitch = -89.5;
				if (camera_pitch > 89.5) camera_pitch = 89.5;
			}
			input->setMousePos(displaycenter.X, displaycenter.Y);
		}else{
			// Mac OSX gets upset if this is set every frame
			if (device->getCursorControl()->isVisible() == false)
				device->getCursorControl()->setVisible(true);

			//infostream<<"window inactive"<<std::endl;
			first_loop_after_window_activation = true;
		}

		/*
			Player speed control
		*/

		if (!noMenuActive() || !device->isWindowActive()) {
			PlayerControl control(
				false,
				false,
				false,
				false,
				false,
				false,
				false,
				fast_move,
				free_move,
				camera_pitch,
				camera_yaw
			);
			client.setPlayerControl(control);
		}else{
			/*bool a_up,
			bool a_down,
			bool a_left,
			bool a_right,
			bool a_jump,
			bool a_superspeed,
			bool a_sneak,
			float a_pitch,
			float a_yaw*/
			PlayerControl control(
				input->isKeyDown(getKeySetting(VLKC_FORWARD)),
				input->isKeyDown(getKeySetting(VLKC_BACKWARD)),
				input->isKeyDown(getKeySetting(VLKC_LEFT)),
				input->isKeyDown(getKeySetting(VLKC_RIGHT)),
				input->isKeyDown(getKeySetting(VLKC_JUMP)),
				input->isKeyDown(getKeySetting(VLKC_USE)),
				input->isKeyDown(getKeySetting(VLKC_SNEAK)),
				fast_move,
				free_move,
				camera_pitch,
				camera_yaw
			);
			client.setPlayerControl(control);
		}

		/*
			Run server
		*/

		if (server != NULL)
			server->step(dtime);

		/*
			Process environment
		*/

		{
			client.step(dtime);
		}

#if USE_AUDIO == 1
		{
			sound->maintain(dtime);
		}
#endif

		{
			// Read client events
			while (1) {
				ClientEvent event = client.getClientEvent();
				if (event.type == CE_NONE) {
					break;
				}else if (event.type == CE_PLAYER_DAMAGE) {
					damage_flash_timer = 0.05;
					if(event.player_damage.amount >= 2){
						damage_flash_timer += 0.05 * event.player_damage.amount;
					}
				}else if (event.type == CE_PLAYER_FORCE_MOVE) {
					camera_yaw = event.player_force_move.yaw;
					camera_pitch = event.player_force_move.pitch;
				}else if (event.type == CE_DEATHSCREEN) {
					if (respawn_menu_active)
						continue;

					MainRespawnInitiator *respawner =
							new MainRespawnInitiator(
									&respawn_menu_active, &client);
					GUIDeathScreen *menu =
							new GUIDeathScreen(guienv, guiroot, -1,
								&g_menumgr, respawner);
					menu->drop();

					/* Handle visualization */
					damage_flash_timer = 0;
				}
			}
		}

		//v3s16 old_camera_offset = camera.getOffset();

		LocalPlayer* player = client.getLocalPlayer();
		camera.update(player, busytime, screensize);
		camera.step(dtime);

		v3f player_position = player->getPosition();
		v3f camera_position = camera.getPosition();
		v3f camera_direction = camera.getDirection();
		f32 camera_fov = camera.getFovMax();
		v3s16 camera_offset = camera.getOffset();

		{
			ISoundManager *snd = client.getSoundManager();
			if (snd)
				snd->updateListener(camera_position,v3f(0,0,0),camera_direction,camera.getCameraNode()->getUpVector());
		}

		//bool camera_offset_changed = (camera_offset != old_camera_offset);

		if (!disable_camera_update) {
			client.updateCamera(camera_position, camera_direction, camera_fov, camera_offset);
			client.updateCameraOffset(camera_offset);
			client.getEnv().updateObjectsCameraOffset(camera_offset);
			update_particles_camera_offset(camera_offset);
			if (clouds)
				clouds->updateCameraOffset(camera_offset);
		}

		bool left_punch = false;
		bool left_punch_muted = false;

		InventoryItem *wield = (InventoryItem*)client.getLocalPlayer()->getWieldItem();
		InventoryList *ilist;
		if (
			wield
			&& (
				content_craftitem_features(wield->getContent()).thrown_item != CONTENT_IGNORE
				|| (
					content_toolitem_features(wield->getContent()).thrown_item != CONTENT_IGNORE
					&& (ilist = client.getLocalPlayer()->inventory.getList("main")) != NULL
					&& ilist->findItem(content_toolitem_features(wield->getContent()).thrown_item) != NULL
				)
			) && input->getLeftClicked()
		) {
			client.throwItem(camera_direction,g_selected_item);
		}else{
			/*
				Calculate what block is the crosshair pointing to
			*/

			f32 d = 4; // max. distance
			core::line3d<f32> shootline(camera_position, camera_position + camera_direction * BS * (d+1));

			ClientActiveObject *selected_active_object = client.getSelectedActiveObject(d*BS, camera_position, shootline);

			if (selected_active_object != NULL) {
				client.setPointedContent(selected_active_object->getContent());
				/* Clear possible cracking animation */
				if (nodepos_old != v3s16(-32768,-32768,-32768)) {
					client.clearTempMod(nodepos_old);
					dig_time = 0.0;
					nodepos_old = v3s16(-32768,-32768,-32768);
				}

				core::aabbox3d<f32> *selection_box
						= selected_active_object->getSelectionBox();
				// Box should exist because object was returned in the
				// first place
				assert(selection_box);

				v3f pos = selected_active_object->getPosition()-intToFloat(camera_offset,BS);

				core::aabbox3d<f32> box_on_map(
						selection_box->MinEdge + pos,
						selection_box->MaxEdge + pos
				);

				if (selected_active_object->doShowSelectionBox())
					hilightboxes.push_back(box_on_map);

				infotext = narrow_to_wide(selected_active_object->infoText());

				if (input->getLeftState()) {
					bool do_punch = false;
					bool do_punch_damage = false;
					if (object_hit_delay_timer <= 0.0){
						do_punch = true;
						do_punch_damage = true;
						object_hit_delay_timer = object_hit_delay;
					}
					if (input->getLeftClicked()) {
						do_punch = true;
					}
					if (do_punch) {
						infostream<<"Left-clicked object"<<std::endl;
						left_punch = true;
					}
					if (do_punch_damage) {
						client.clickActiveObject(0, selected_active_object->getId(), g_selected_item);
					}
				}else if (input->getRightClicked()) {
					infostream<<"Right-clicked object"<<std::endl;
					client.clickActiveObject(1, selected_active_object->getId(), g_selected_item);
				}
			}else{ // selected_object == NULL
				/*
					Find out which node we are pointing at
				*/

				bool nodefound = false;
				v3s16 nodepos;
				v3s16 neighbourpos;
				core::aabbox3d<f32> nodehilightbox;

				getPointedNode(&client, player_position,
						camera_direction, camera_position,
						nodefound, shootline,
						nodepos, neighbourpos, camera_offset,
						nodehilightbox, d);

				if (!nodefound) {
					if (nodepos_old != v3s16(-32768,-32768,-32768)) {
						client.clearTempMod(nodepos_old);
						dig_time = 0.0;
						nodepos_old = v3s16(-32768,-32768,-32768);
					}
				}else{

					/*
						Check information text of node
					*/

					if (nodepos != nodepos_old && nodepos_old != v3s16(-32768,-32768,-32768))
						client.clearTempMod(nodepos_old);
					NodeMetadata *meta = client.getNodeMetadata(nodepos);
					if (meta)
						infotext = meta->infoText();

					/*
						Handle digging
					*/

					if (input->getLeftReleased()) {
						client.clearTempMod(nodepos);
						dig_time = 0.0;
					}
					/*
						Visualize selection
					*/

					if (highlight_selected_node) {
						client.setTempMod(nodepos, NodeMod(NODEMOD_SELECTION));
					}else{
						hilightboxes.push_back(nodehilightbox);
					}

					if (nodig_delay_counter > 0.0) {
						nodig_delay_counter -= dtime;
					}else{
						if (nodepos != nodepos_old) {
							infostream<<"Pointing at ("<<nodepos.X<<","
									<<nodepos.Y<<","<<nodepos.Z<<")"<<std::endl;

							if (nodepos_old != v3s16(-32768,-32768,-32768)) {
								client.clearTempMod(nodepos_old);
								dig_time = 0.0;
								nodepos_old = v3s16(-32768,-32768,-32768);
							}
						}

						if (input->getLeftClicked() || (input->getLeftState() && nodepos != nodepos_old)) {
							infostream<<"Started digging"<<std::endl;
							client.groundAction(0, nodepos, neighbourpos, g_selected_item);
						}
						if (input->getLeftClicked())
							client.setTempMod(nodepos, NodeMod(NODEMOD_CRACK, 0));
						if (input->getLeftState()) {
							MapNode n = client.getNode(nodepos);

							// Get tool name. Default is "" = bare hands
							content_t toolid = CONTENT_IGNORE;
							InventoryList *mlist = local_inventory.getList("main");
							if (mlist != NULL) {
								InventoryItem *item = mlist->getItem(g_selected_item);
								if (item && (std::string)item->getName() == "ToolItem") {
									ToolItem *titem = (ToolItem*)item;
									toolid = titem->getContent();
								}
							}

							// Get digging properties for material and tool
							content_t material = n.getContent();
							DiggingProperties prop = getDiggingProperties(material, toolid);

							float dig_time_complete = 0.0;

							if (prop.diggable == false) {
								dig_time_complete = 10000000.0;
								client.clearTempMod(nodepos);
							}else{
								dig_time_complete = prop.time;
								if (enable_particles)
									addPunchingParticles(smgr, player, nodepos, content_features(n).tiles);

								if (dig_time_complete >= 0.001) {
									dig_index = (u16)((float)CRACK_ANIMATION_LENGTH
											* dig_time/dig_time_complete);
								}else {
									// This is for torches
									dig_index = CRACK_ANIMATION_LENGTH;
								}

								if (dig_index < CRACK_ANIMATION_LENGTH) {
									client.setTempMod(nodepos, NodeMod(NODEMOD_CRACK, dig_index));
								}else{
									infostream<<"Digging completed"<<std::endl;
									client.groundAction(3, nodepos, neighbourpos, g_selected_item);
									client.clearTempMod(nodepos);
									client.removeNode(nodepos);

									if (enable_particles)
										addDiggingParticles(smgr, player, nodepos, content_features(n).tiles);

									dig_time = 0;

									nodig_delay_counter = dig_time_complete
											/ (float)CRACK_ANIMATION_LENGTH;

									// We don't want a corresponding delay to
									// very time consuming nodes
									if (nodig_delay_counter > 0.5)
										nodig_delay_counter = 0.5;
									// We want a slight delay to very little
									// time consuming nodes
									float mindelay = 0.15;
									if (nodig_delay_counter < mindelay)
										nodig_delay_counter = mindelay;
								}
							}

							dig_time += dtime;

							camera.setDigging(0);  // left click animation
						}
					}


					if (input->getRightClicked()) {
						infostream<<"Ground right-clicked"<<std::endl;

						// If metadata provides an inventory view, activate it
						if (meta && meta->getDrawSpecString() != "" && !random_input) {
							infostream<<"Launching custom inventory view"<<std::endl;

							InventoryLocation inventoryloc;
							inventoryloc.setNodeMeta(nodepos);

							/* Create menu */

							GUIFormSpecMenu *menu = new GUIFormSpecMenu(guienv, guiroot, -1, &g_menumgr, &client);
							menu->setFormSpec(meta->getDrawSpecString(), inventoryloc);
							menu->setFormIO(new NodeMetadataFormIO(nodepos, &client));
							menu->drop();
							{
								std::string sound = content_features(client.getEnv().getMap().getNodeNoEx(nodepos).getContent()).sound_access;
								if (sound == "")
									sound = "open-menu";
								client.playSound(sound,0);
							}
						}else{
							client.groundAction(1, nodepos, neighbourpos, g_selected_item);
							camera.setDigging(1);  // right click animation
						}
					}

					nodepos_old = nodepos;
				}

			} // selected_object == NULL
		}

		if (left_punch || (input->getLeftClicked() && !left_punch_muted))
			camera.setDigging(0); // left click animation

		input->resetLeftClicked();
		input->resetRightClicked();

		if (input->getLeftReleased()) {
			infostream<<"Left button released (stopped digging)"
					<<std::endl;
			client.groundAction(2, v3s16(0,0,0), v3s16(0,0,0), 0);
		}
		if (input->getRightReleased()) {
			// Nothing here
		}

		input->resetLeftReleased();
		input->resetRightReleased();

		/*
			Calculate stuff for drawing
		*/

		u32 daynight_ratio = client.getDayNightRatio();
		u8 l = decode_light((daynight_ratio * LIGHT_SUN) / 1000);
		video::SColor bgcolor = video::SColor(
			255,
			bgcolor_bright.getRed() * l / 255,
			bgcolor_bright.getGreen() * l / 255,
			bgcolor_bright.getBlue() * l / 255
		);

		float brightness = (float)l/255.0;

		/*
			Update skybox
		*/
		if (fabs(brightness - old_brightness) > 0.01)
			update_skybox(driver, smgr, skybox, brightness);

		/*
			Update clouds
		*/
		if (clouds) {
			clouds->step(dtime);
			clouds->update(v2f(player_position.X, player_position.Z), 0.05+brightness*0.95);
		}

		/*
			Update farmesh
		*/
		if (farmesh) {
			farmesh_range = draw_control.wanted_range * 10;
			if (draw_control.range_all && farmesh_range < 500)
				farmesh_range = 500;
			if (farmesh_range > 1000)
				farmesh_range = 1000;

			farmesh->step(dtime);
			farmesh->update(v2f(player_position.X, player_position.Z), 0.05+brightness*0.95, farmesh_range);
		}

		/*
			Update particles
		*/
		allparticles_step(dtime, client.getEnv());
		allparticlespawners_step(dtime, client.getEnv());

		// Store brightness value
		old_brightness = brightness;

		/*
			Fog
		*/

		if (enable_fog && !force_fog_off) {
			f32 range;
			if (farmesh) {
				range = BS*farmesh_range;
			}else{
				range = draw_control.wanted_range*BS + MAP_BLOCKSIZE*BS*1.5;
				range *= 0.9;
				if (draw_control.range_all)
					range = 100000*BS;
			}

			driver->setFog(
				bgcolor,
				video::EFT_FOG_LINEAR,
				range*0.4,
				range*1.0,
				0.01,
				false, // pixel fog
				false // range fog
			);
		}else{
			driver->setFog(
				bgcolor,
				video::EFT_FOG_LINEAR,
				100000*BS,
				110000*BS,
				0.01,
				false, // pixel fog
				false // range fog
			);
		}

		float client_rtt = client.getRTT();
		if (client_rtt < -1000) {
			error_message = wgettext("Disconnected (Network Timeout)");
			break;
		}

		/*
			Update gui stuff (0ms)
		*/
		const char program_name_and_version[] = "Voxelands " VERSION_STRING;
		if (show_debug) {
			static float drawtime_avg = 0;
			drawtime_avg = drawtime_avg * 0.95 + (float)drawtime*0.05;

			{
				char temptext[300];
				snprintf(temptext, 300, "%s ("
						"R: range_all=%i"
						")"
						" drawtime=%.0f, dtime_jitter = % .1f %%"
						", v_range = %.1f, RTT = %.3f",
						program_name_and_version,
						draw_control.range_all,
						drawtime_avg,
						dtime_jitter1_max_fraction * 100.0,
						draw_control.wanted_range,
						client_rtt
						);

				guitext->setText(narrow_to_wide(temptext).c_str());
				guitext->setVisible(true);
			}
			{
				char temptext[300];
				snprintf(temptext, 300,
						"(% .1f, % .1f, % .1f)"
						" (yaw = %.1f)",
						player_position.X/BS,
						player_position.Y/BS,
						player_position.Z/BS,
						wrapDegrees_0_360(camera_yaw));

				guitext2->setText(narrow_to_wide(temptext).c_str());
				guitext2->setVisible(true);
			}
		}else if (show_hud || show_chat) {
			char temptext[300];
			snprintf(temptext, 300,
					"(% .1f, % .1f, % .1f)"
					" (yaw = %.1f)",
					player_position.X/BS,
					player_position.Y/BS,
					player_position.Z/BS,
					wrapDegrees_0_360(camera_yaw));

			guitext2->setText(narrow_to_wide(temptext).c_str());
			guitext2->setVisible(true);
			guitext->setText(narrow_to_wide(program_name_and_version).c_str());
			guitext->setVisible(true);
		}else{
			guitext->setVisible(false);
			guitext2->setVisible(false);
		}

		if (!show_debug && g_menumgr.menuCount() == 0) {
			guitext_info->setText(infotext.c_str());
			guitext_info->setVisible(show_hud);
		}else{
			guitext_info->setVisible(false);
		}

		{
			float statustext_time_max = 3.0;
			if (!statustext.empty()) {
				statustext_time += dtime;
				if (statustext_time >= statustext_time_max) {
					statustext = L"";
					statustext_time = 0;
				}
			}
			guitext_status->setText(statustext.c_str());
			guitext_status->setVisible(!statustext.empty());

			if (!statustext.empty()) {
				s32 status_y = screensize.Y - 130;
				core::rect<s32> rect(
					10,
					status_y - guitext_status->getTextHeight(),
					screensize.X - 10,
					status_y
				);
				guitext_status->setRelativePosition(rect);

				// Fade out
				video::SColor initial_color(255,0,0,0);
				if (guienv->getSkin())
					initial_color = guienv->getSkin()->getColor(gui::EGDC_BUTTON_TEXT);
				video::SColor final_color = initial_color;
				final_color.setAlpha(0);
				video::SColor fade_color = initial_color.getInterpolated_quadratic(
					initial_color,
					final_color,
					statustext_time / (float) statustext_time_max
				);
				guitext_status->setOverrideColor(fade_color);
				guitext_status->enableOverrideColor(true);
			}
		}

		/*
			Get chat messages from client
		*/
		{
			// Get new messages
			std::wstring message;
			while (client.getChatMessage(message)) {
				chat_lines.push_back(ChatLine(message));
			}
			// Append them to form the whole static text and throw
			// it to the gui element
			std::wstring whole;
			// This will correspond to the line number counted from
			// top to bottom, from size-1 to 0
			s16 line_number = chat_lines.size();
			// Count of messages to be removed from the top
			u16 to_be_removed_count = 0;
			for (core::list<ChatLine>::Iterator i = chat_lines.begin(); i != chat_lines.end(); i++) {
				// After this, line number is valid for this loop
				line_number--;
				// Increment age
				(*i).age += dtime;
				/*
					This results in a maximum age of 60*6 to the
					lowermost line and a maximum of 6 lines
				*/
				float allowed_age = (6-line_number) * 60.0;

				if ((*i).age > allowed_age) {
					to_be_removed_count++;
					continue;
				}
				whole += (*i).text + L'\n';
			}
			for (u16 i=0; i<to_be_removed_count; i++) {
				core::list<ChatLine>::Iterator it = chat_lines.begin();
				chat_lines.erase(it);
			}
			guitext_chat->setText(whole.c_str());

			// Update gui element size and position

			s32 chat_y = 5+(2*text_height);
			core::rect<s32> rect(
				10,
				chat_y,
				screensize.X - 10,
				chat_y + guitext_chat->getTextHeight()
			);

			guitext_chat->setRelativePosition(rect);

			// Don't show chat if empty or profiler or debug is enabled
			guitext_chat->setVisible(chat_lines.size() != 0
					&& show_chat && show_profiler == 0);
		}

		/*
			Inventory
		*/

		static u16 old_selected_item = 65535;
		if (client.getLocalInventoryUpdated() || g_selected_item != old_selected_item) {
			client.selectPlayerItem(g_selected_item);
			old_selected_item = g_selected_item;
			client.getLocalInventory(local_inventory);

			// Update wielded tool
			InventoryList *mlist = local_inventory.getList("main");
			InventoryItem *item = NULL;
			if (mlist != NULL)
				item = mlist->getItem(g_selected_item);
			camera.wield(item);
		}

		/*
			Send actions returned by the inventory menu
		*/
		while (inventory_action_queue.size() != 0) {
			InventoryAction *a = inventory_action_queue.pop_front();

			client.sendInventoryAction(a);
			// Eat it
			delete a;
		}

		/*
			Drawing begins
		*/

		TimeTaker drawtimer("Drawing");


		{
			TimeTaker timer("beginScene");
			driver->beginScene(false, true, bgcolor);
			timer.stop(true);
		}

		{
			TimeTaker timer("smgr");
			smgr->drawAll();
			timer.stop(true);
		}

		{

		video::SMaterial m;
		//m.Thickness = 10;
		m.Thickness = 3;
		m.Lighting = false;
		driver->setMaterial(m);

		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

		if (show_hud) {
			for (core::list<aabb3f>::Iterator i=hilightboxes.begin(); i != hilightboxes.end(); i++) {
				driver->draw3DBox(*i, video::SColor(255,0,0,0));
			}
		}

		/*
			Wielded tool
		*/
		if (show_hud) {
			// Warning: This clears the Z buffer.
			camera.drawWieldedTool();
		}

		/*
			Post effects
		*/
		{
			client.renderPostFx();
		}

		/*
			Frametime log
		*/
		if (show_debug_frametime) {
			s32 x = 10;
			s32 y = screensize.Y - 10;
			for (core::list<float>::Iterator i = frametime_log.begin(); i != frametime_log.end(); i++) {
				driver->draw2DLine(
					v2s32(x,y),
					v2s32(x,y+(*i)*1000),
					video::SColor(255,255,255,255)
				);
				x++;
			}
		}

		} // timer

		/*
			Draw gui
		*/
		// 0-1ms
		guienv->drawAll();

		/*
			Draw hotbar
		*/
		if (show_hud) {
			s32 hunger = 0;
			if (client.getServerHunger())
				hunger = client.getHunger();
			if (old_hotbar) {

				/*
					Draw crosshair
				*/
				driver->draw2DLine(displaycenter - core::vector2d<s32>(10,0),
							displaycenter + core::vector2d<s32>(10,0),
							video::SColor(255,255,255,255));
				driver->draw2DLine(displaycenter - core::vector2d<s32>(0,10),
							displaycenter + core::vector2d<s32>(0,10),
							video::SColor(255,255,255,255));
				draw_old_hotbar(
					driver,
					font,
					v2s32(screensize.X/2,screensize.Y),
					hotbar_imagesize,
					hotbar_itemcount,
					&local_inventory,
					client.getHP(),
					client.getAir(),
					hunger
				);
			}else{
				int crosshair = 1;
				if (g_menumgr.menuCount() > 0) {
					crosshair = 0;
				}else if (client.getPointedContent() != CONTENT_IGNORE) {
					crosshair = 2;
				}
				draw_hud(
					driver,
					font,
					v2s32(screensize.X,screensize.Y),
					hotbar_imagesize,
					hotbar_itemcount,
					&local_inventory,
					client.getServerDamage(),
					client.getHP(),
					client.getServerSuffocation(),
					client.getAir(),
					client.getServerHunger(),
					hunger,
					crosshair
				);
			}
		}

		/*
			Damage flash
		*/
		if (damage_flash_timer > 0.0) {
			damage_flash_timer -= dtime;

			video::SColor color(128,255,0,0);
			driver->draw2DRectangle(color,
					core::rect<s32>(0,0,screensize.X,screensize.Y),
					NULL);
		}

		/*
			End scene
		*/
		{
			TimeTaker timer("endScene");
			endSceneX(driver);
			timer.stop(true);
		}

		drawtime = drawtimer.stop(true);

		/*
			End of drawing
		*/

		static s16 lastFPS = 0;
		u16 fps = (1.0/dtime_avg1);

		if (lastFPS != fps) {
			core::stringw str = L"Voxelands [";
			str += driver->getName();
			str += "] FPS=";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	// save some settings
	if (free_move) {
		g_settings->set("free_move","true");
	}else{
		g_settings->set("free_move","false");
	}
	if (fast_move) {
		g_settings->set("fast_move","true");
	}else{
		g_settings->set("fast_move","false");
	}

	/*
		Drop stuff
	*/
	if (clouds)
		clouds->drop();

	clear_particles();

	/*
		Draw a "shutting down" screen, which will be shown while the map
		generator and other stuff quits
	*/
	{
		drawLoadingScreen(device,wgettext("Shutting down..."));
	}
}


