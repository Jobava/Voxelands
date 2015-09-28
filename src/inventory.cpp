/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* inventory.cpp
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


#include "inventory.h"
#include "serialization.h"
#include "utility.h"
#include "debug.h"
#include <sstream>
#include "main.h"
#include "serverobject.h"
#include "content_mapnode.h"
#include "content_sao.h"
#include "content_mob.h"
#include "player.h"
#include "log.h"
#include "intl.h"
#include "enchantment.h"

/*
	InventoryItem
*/

InventoryItem::InventoryItem(u16 count, u16 data)
{
	m_content = CONTENT_IGNORE;
	m_count = count;
	m_data = data;
}

InventoryItem::~InventoryItem()
{
}

content_t InventoryItem::info(std::istream &is, u16 *count, u16 *wear, u16 *data)
{
	DSTACK(__FUNCTION_NAME);

	content_t c = CONTENT_IGNORE;
	*count = 0;
	*wear = 0;
	*data = 0;

	//is.imbue(std::locale("C"));
	// Read name
	std::string name;
	std::getline(is, name, ' ');

	if (name == "MaterialItem") {
		// u16 reads directly as a number (u8 doesn't)
		u16 material;
		is>>material;
		is>>(*count);
		if (material > MAX_CONTENT)
			throw SerializationError("Too large material number");
		c = material;
	}else if(name == "MaterialItem2") {
		u16 material;
		is>>material;
		is>>(*count);
		if (material > MAX_CONTENT)
			throw SerializationError("Too large material number");
		c = material;
	}else if(name == "MaterialItem3") {
		u16 material;
		is>>material;
		is>>(*count);
		is>>(*data);
		if (material > MAX_CONTENT)
			throw SerializationError("Too large material number");
		c = material;
	}else if(name == "CraftItem") {
		std::string subname;
		std::getline(is, subname, ' ');
		is>>(*count);
		CraftItem itm(subname, *count, 0);
		c = itm.getContent();
	}else if(name == "CraftItem2") {
		u16 material;
		is>>material;
		is>>(*count);
		c = material;
	}else if(name == "CraftItem3") {
		u16 material;
		is>>material;
		is>>(*count);
		is>>(*data);
		c = material;
	}else if(name == "ToolItem") {
		std::string toolname;
		std::getline(is, toolname, ' ');
		is>>(*wear);
		ToolItem itm(toolname, *wear, 0);
		c = itm.getContent();
	}else if(name == "ToolItem2") {
		u16 material;
		is>>material;
		is>>(*wear);
		c = material;
	}else if(name == "ToolItem3") {
		u16 material;
		is>>material;
		is>>(*wear);
		is>>(*data);
		c = material;
	}else if(name == "ClothesItem") {
		u16 material;
		is>>material;
		is>>(*wear);
		c = material;
	}else if(name == "ClothesItem2") {
		u16 material;
		is>>material;
		is>>(*wear);
		is>>(*data);
		c = material;
	}else if (name != "") {
		infostream<<"Unknown InventoryItem name=\""<<name<<"\""<<std::endl;
		throw SerializationError("Unknown InventoryItem name");
	}

	return c;
}

InventoryItem* InventoryItem::deSerialize(std::istream &is)
{
	content_t c;
	u16 count;
	u16 wear;
	u16 data;

	c = InventoryItem::info(is,&count,&wear,&data);
	if (c == CONTENT_IGNORE)
		return NULL;
	return InventoryItem::create(c,count,wear,data);
}

InventoryItem* InventoryItem::create(content_t c, u16 count, u16 wear, u16 data)
{
	if ((c&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
		return new CraftItem(c,count,data);
	}else if ((c&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
		return new ToolItem(c,wear,data);
	}else if ((c&CONTENT_CLOTHESITEM_MASK) == CONTENT_CLOTHESITEM_MASK) {
		return new ClothesItem(c,wear,data);
	}

	return new MaterialItem(c,count,data);
}

std::string InventoryItem::getItemString() {
	// Get item string
	std::ostringstream os(std::ios_base::binary);
	serialize(os);
	return os.str();
}

ServerActiveObject* InventoryItem::createSAO(ServerEnvironment *env, u16 id, v3f pos)
{
	/*
		Create an ItemSAO
	*/
	// Create object
	ServerActiveObject *obj = new ItemSAO(env, 0, pos, getItemString());
	return obj;
}

/*
	MaterialItem
*/
#ifndef SERVER
video::ITexture * MaterialItem::getImage() const
{
	//if (m_data == 0 || content_features(m_content).param_type != CPT_MINERAL)
		return content_features(m_content).inventory_texture;


	//std::string name = content_features(m_content).inventory_texture;

	//std::ostringstream os;
	//os<<name;

	//u8 mineral = m_data;
	//std::string mineral_texture_name = mineral_features(mineral).texture;
	//if (mineral_texture_name != "") {
		//u32 orig_id = spec.texture.id;
		//std::string texture_name = g_texturesource->getTextureName(orig_id);
		////texture_name += "^blit:";
		//texture_name += "^";
		//texture_name += mineral_texture_name;
		//u32 new_id = g_texturesource->getTextureId(texture_name);
		//spec.texture = g_texturesource->getTexture(new_id);
	//}

	//// Get such a texture
	//return g_texturesource->getTextureRaw(os.str());
}
#endif
std::wstring MaterialItem::getGuiText()
{
	std::wstring txt(L"  ");
	ContentFeatures *f = &content_features(m_content);
	txt += f->description;
	if (f->cook_result != "" || f->fuel_time != 0.0)
		txt += L"\n";
	if (f->cook_result != "") {
		txt += L"\n";
		txt += wgettext("Cookable: Yes");
	}
	if (f->fuel_time != 0.0) {
		char buff[20];
		txt += L"\n";
		txt += wgettext("Fuel Burn Time: ");
		txt += itows((int)f->fuel_time/60);
		txt += L":";
		sprintf(buff,"%02d",(int)f->fuel_time%60);
		txt += narrow_to_wide(buff);
	}

	return txt;
}

bool MaterialItem::isCookable() const
{
	return (content_features(m_content).cook_result != "");
}

InventoryItem *MaterialItem::createCookResult() const
{
	std::istringstream is(content_features(m_content).cook_result, std::ios::binary);
	return InventoryItem::deSerialize(is);
}

bool MaterialItem::isFuel() const
{
	return (content_features(m_content).fuel_time != 0.0);
}

float MaterialItem::getFuelTime() const
{
	return content_features(m_content).fuel_time;
}

bool MaterialItem::use(ServerEnvironment *env, Player *player)
{
	content_t n = content_features(m_content).onuse_replace_node;
	if (n != CONTENT_IGNORE)
		m_content = n;

	return false;
}

/*
	CraftItem
*/

#ifndef SERVER
video::ITexture * CraftItem::getImage() const
{
	if(g_texturesource == NULL)
		return NULL;

	std::string name = content_craftitem_features(m_content).texture;
	std::string base = content_craftitem_features(m_content).overlay_base;

	std::ostringstream os;
	os<<name;

	if (base != "") {
		if (content_craftitem_features(m_content).param_type == CPT_ENCHANTMENT) {
			EnchantmentInfo info;
			u16 data = m_data;
			// TODO: adding more than 2 overlays messes up alpha
			for (int i=0; i<2 && enchantment_get(&data,&info); i++) {
				if (info.overlay != "")
					os<<"^"<<base<<"_"<<info.overlay<<".png";
			}
		}
	}

	// Get such a texture
	return g_texturesource->getTextureRaw(os.str());
}
#endif
std::wstring CraftItem::getGuiName()
{
	return content_craftitem_features(m_content).description;
}
std::wstring CraftItem::getGuiText()
{
	std::wstring txt(L"  ");
	CraftItemFeatures *f = &content_craftitem_features(m_content);
	txt += f->description;
	if (f->consumable || f->cook_result != CONTENT_IGNORE || f->fuel_time != 0.0)
		txt += L"\n";
	if (f->consumable) {
		if (f->hunger_effect) {
			txt += L"\n";
			txt += wgettext("Hunger: ");
			txt += itows(f->hunger_effect*5);
			txt += L"%";
		}
		if (f->health_effect) {
			txt += L"\n";
			txt += wgettext("Health: ");
			txt += itows(f->health_effect*5);
			txt += L"%";
		}
		if (f->cold_effect) {
			char buff[20];
			txt += L"\n";
			txt += wgettext("Cold Protection: ");
			txt += itows(f->cold_effect/60);
			txt += L":";
			sprintf(buff,"%02d",f->cold_effect%60);
			txt += narrow_to_wide(buff);
		}
		if (f->energy_effect) {
			char buff[20];
			txt += L"\n";
			txt += wgettext("Energy Boost: ");
			txt += itows(f->energy_effect/60);
			txt += L":";
			sprintf(buff,"%02d",f->energy_effect%60);
			txt += narrow_to_wide(buff);
		}
	}
	if (f->cook_result != CONTENT_IGNORE) {
		txt += L"\n";
		txt += wgettext("Cookable: Yes");
	}
	if (f->fuel_time != 0.0) {
		char buff[20];
		txt += L"\n";
		txt += wgettext("Fuel Burn Time: ");
		txt += itows((int)f->fuel_time/60);
		txt += L":";
		sprintf(buff,"%02d",(int)f->fuel_time%60);
		txt += narrow_to_wide(buff);
	}
	if (m_data > 0) {
		if (content_craftitem_features(m_content).param_type == CPT_ENCHANTMENT) {
			EnchantmentInfo info;
			u16 data = m_data;
			txt += L"\n";
			while (enchantment_get(&data,&info)) {
				txt += L"\n";
				txt += info.name;
				txt += L" ";
				txt += itows(info.level);
			}
		}
	}

	return txt;
}

ServerActiveObject* CraftItem::createSAO(ServerEnvironment *env, u16 id, v3f pos)
{
	content_t drop = content_craftitem_features(m_content).drop_item;
	if (content_craftitem_features(m_content).param_type == CPT_DROP && m_data != 0)
		drop = m_data;
	// Special cases
	if ((drop&CONTENT_MOB_MASK) == CONTENT_MOB_MASK) {
		v3f p = pos;
		p.Y += 0.5*BS;
		ServerActiveObject *obj = new MobSAO(env,id,p,drop);
		if (obj)
			return obj;
	}
	// Default
	return InventoryItem::createSAO(env, id, pos);
}

u16 CraftItem::getDropCount() const
{
	// Special cases
	s16 dc = content_craftitem_features(m_content).drop_count;
	if(dc != -1)
		return dc;
	// Default
	return InventoryItem::getDropCount();
}

bool CraftItem::isCookable() const
{
	return content_craftitem_features(m_content).cook_result != CONTENT_IGNORE;
}

InventoryItem *CraftItem::createCookResult() const
{
	return InventoryItem::create(content_craftitem_features(m_content).cook_result,1,1,0);
}

bool CraftItem::isFuel() const
{
	return (content_craftitem_features(m_content).fuel_time != 0.0);
}

float CraftItem::getFuelTime() const
{
	return content_craftitem_features(m_content).fuel_time;
}

bool CraftItem::use(ServerEnvironment *env, Player *player)
{
	u16 count = getCount();
	bool used = false;
	CraftItemFeatures f = content_craftitem_features(m_content);
	if (f.consumable) {
		if (f.hunger_effect && (f.health_effect < 1 || player->hunger < 20)) {
			if (player->hunger + f.hunger_effect > 20) {
				player->hunger = 20;
			}else{
				player->hunger += f.hunger_effect;
			}
			used = true;
		}
		if (f.health_effect < 0 || (!used && f.health_effect > 0)) {
			if (player->hp + f.health_effect > 20) {
				player->hp = 20;
			}else{
				player->hp += f.health_effect;
			}
			used = true;
		}
		if (f.cold_effect) {
			player->cold_effect = f.cold_effect;
			used = true;
		}
		if (f.energy_effect) {
			player->energy_effect = f.energy_effect;
			used = true;
		}
	}
	if (f.onuse_replace_item != CONTENT_IGNORE) {
		m_content = f.onuse_replace_item;
	}else if (used) {
		count--;
		if (count < 1)
			return true;
		setCount(count);
	}
	return false;
}

/*
	ToolItem
*/

#ifndef SERVER
std::string ToolItem::getBasename() const
{
	std::ostringstream os;
	os<<content_toolitem_features(m_content).texture;

	if (content_toolitem_features(m_content).param_type == CPT_ENCHANTMENT) {
		EnchantmentInfo info;
		u16 data = m_data;
		// TODO: adding more than 2 overlays messes up alpha
		for (int i=0; i<2 && enchantment_get(&data,&info); i++) {
			std::string ol = toolitem_overlay(m_content,info.overlay);
			if (ol != "")
				os<<"^"<<ol;
		}
	}

	return os.str();
}

video::ITexture *ToolItem::getImage() const
{
	if(g_texturesource == NULL)
		return NULL;

	std::string basename = getBasename();

	/*
		Calculate a progress value with sane amount of
		maximum states
	*/
	u32 maxprogress = 30;
	u32 toolprogress = (65535-m_wear)/(65535/maxprogress);

	float value_f = (float)toolprogress / (float)maxprogress;
	std::ostringstream os;
	os<<basename<<"^[progressbar"<<value_f;

	return g_texturesource->getTextureRaw(os.str());
}
#endif

std::wstring ToolItem::getGuiText()
{
	std::wstring txt(L"  ");
	ToolItemFeatures *f = &content_toolitem_features(m_content);
	txt += f->description;
	txt += L"\n\n";
	txt += wgettext("Strength: ");
	txt += ftows(f->hardness);
	txt += L"\n";
	txt += wgettext("Speed: ");
	txt += ftows(f->dig_time);
	txt += L"\n";
	txt += wgettext("Level: ");
	txt += itows(f->level);
	if (f->fuel_time != 0.0) {
		char buff[20];
		txt += L"\n";
		txt += wgettext("Fuel Burn Time: ");
		txt += itows((int)f->fuel_time/60);
		txt += L":";
		sprintf(buff,"%02d",(int)f->fuel_time%60);
		txt += narrow_to_wide(buff);
	}
	if (m_data > 0) {
		switch (content_toolitem_features(m_content).param_type) {
		case CPT_ENCHANTMENT:
		{
			EnchantmentInfo info;
			u16 data = m_data;
			txt += L"\n";
			while (enchantment_get(&data,&info)) {
				txt += L"\n";
				txt += info.name;
				txt += L" ";
				txt += itows(info.level);
			}
			break;
		}
		case CPT_DROP:
		{
			if ((m_data&CONTENT_MOB_MASK) == CONTENT_MOB_MASK) {
				txt += L"\n";
				txt += wgettext("Contains: ");
				txt += content_mob_features(m_data).description;
			}
			break;
		}
		default:;
		}
	}

	return txt;
}

bool ToolItem::isCookable() const
{
	return content_toolitem_features(m_content).cook_result != "";
}

InventoryItem *ToolItem::createCookResult() const
{
	std::istringstream is(content_toolitem_features(m_content).cook_result, std::ios::binary);
	return InventoryItem::deSerialize(is);
}

bool ToolItem::isFuel() const
{
	return (content_toolitem_features(m_content).fuel_time != 0.0);
}

float ToolItem::getFuelTime() const
{
	return content_toolitem_features(m_content).fuel_time;
}

ServerActiveObject* ToolItem::createSAO(ServerEnvironment *env, u16 id, v3f pos)
{
	content_t drop = CONTENT_IGNORE;
	if (content_toolitem_features(m_content).param_type == CPT_DROP && m_data != 0)
		drop = m_data;
	// Special cases
	if ((drop&CONTENT_MOB_MASK) == CONTENT_MOB_MASK) {
		v3f p = pos;
		p.Y += 0.5*BS;
		ServerActiveObject *obj = new MobSAO(env,id,p,drop);
		if (obj)
			return obj;
	}
	// Default
	return InventoryItem::createSAO(env, id, pos);
}

/*
	ClothesItem
*/

#ifndef SERVER
video::ITexture *ClothesItem::getImage() const
{
	if (g_texturesource == NULL)
		return NULL;

	std::string basename = getBasename();

	/*
		Calculate a progress value with sane amount of
		maximum states
	*/
	u32 maxprogress = 30;
	u32 toolprogress = (65535-m_wear)/(65535/maxprogress);

	float value_f = (float)toolprogress / (float)maxprogress;
	std::ostringstream os;
	os<<basename<<"^[progressbar"<<value_f;

	return g_texturesource->getTextureRaw(os.str());
}
#endif

std::wstring ClothesItem::getGuiText()
{
	std::wstring txt(L"  ");
	ClothesItemFeatures *f = &content_clothesitem_features(m_content);
	txt += f->description;
	if (f->armour > 0.0 || f->warmth > 0.0 || f->vacuum > 0.0 || f->suffocate > 0.0 || f->durability > 0.0 || f->effect > 1.0)
		txt += L"\n";
	if (f->armour > 0.0) {
		txt += L"\n";
		txt += wgettext("Armour: ");
		txt += itows(f->armour*100.0);
		txt += L"%";
	}
	if (f->warmth > 0.0) {
		txt += L"\n";
		txt += wgettext("Warmth: ");
		txt += itows(f->warmth*100.0);
		txt += L"%";
	}
	if (f->vacuum > 0.0) {
		txt += L"\n";
		txt += wgettext("Pressure: ");
		txt += itows(f->vacuum*100.0);
		txt += L"%";
	}
	if (f->suffocate > 0.0) {
		txt += L"\n";
		txt += wgettext("Suffocation: ");
		txt += itows(f->suffocate*100.0);
		txt += L"%";
	}
	if (f->durability > 0.0) {
		txt += L"\n";
		txt += wgettext("Durability: ");
		txt += itows(f->durability);
	}
	if (f->effect != 1.0) {
		txt += L"\n";
		txt += wgettext("Effect Boost: ");
		txt += itows(f->effect*100.0);
		txt += L"%";
	}

	return txt;
}

/*
	Inventory
*/

InventoryList::InventoryList(std::string name, u32 size)
{
	m_name = name;
	m_size = size;
	clearItems();
	clearAllowed();
	clearDenied();
	setStackable();
}

InventoryList::~InventoryList()
{
	for(u32 i=0; i<m_items.size(); i++)
	{
		if(m_items[i])
			delete m_items[i];
	}
}

void InventoryList::clearItems()
{
	for (u32 i=0; i<m_items.size(); i++) {
		if (m_items[i]) {
			delete m_items[i];
			m_diff.add(m_name,i,NULL);
		}
	}

	m_items.clear();

	for (u32 i=0; i<m_size; i++) {
		m_items.push_back(NULL);
	}
}

void InventoryList::serialize(std::ostream &os) const
{
	writeU8(os,1); // version

	for (u32 i=0; i<m_items.size(); i++) {
		InventoryItem *item = m_items[i];
		if (item != NULL) {
			content_t c = item->getContent();
			writeU16(os,c);
			if ((c&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK || (c&CONTENT_CLOTHESITEM_MASK) == CONTENT_CLOTHESITEM_MASK) {
				writeU16(os,item->getWear());
			}else{
				writeU16(os,item->getCount());
			}
			writeU16(os,item->getData());
		}else{
			writeU16(os,CONTENT_IGNORE);
		}
	}
}

void InventoryList::deSerialize(std::istream &is)
{
	u32 item_i = 0;
	char f;
	is.get(f);
	if (f == '\1') {
		char v;
		is.get(v);
		switch (v) {
		case 1:
		{
			for (item_i=0; item_i<getSize(); item_i++) {
				content_t c = readU16(is);
				if (c == CONTENT_IGNORE) {
					if (m_items[item_i] != NULL)
						delete m_items[item_i];
					m_items[item_i] = NULL;
					continue;
				}
				if (c == CONTENT_MESE)
					c = CONTENT_CRAFTITEM_MITHRIL_UNBOUND;
				u16 wc = readU16(is);
				u16 d = readU16(is);
				if (m_items[item_i] != NULL) {
					if (m_items[item_i]->getContent() == c) {
						m_items[item_i]->setWear(wc);
						m_items[item_i]->setCount(wc);
						m_items[item_i]->setData(d);
						continue;
					}
					delete m_items[item_i];
				}
				m_items[item_i] = InventoryItem::create(c,wc,wc,d);
			}
		}
			break;
		default:
			throw SerializationError("Unknown inventory format");
		}

		if (readU8(is) != 3)
			throw SerializationError("Unknown inventory identifier");

		return;
	}
	if (f == '\2') {
		for (item_i=0; item_i<getSize(); item_i++) {
			content_t c = readU16(is);
			if (c == CONTENT_IGNORE) {
				if (m_items[item_i] != NULL)
					delete m_items[item_i];
				m_items[item_i] = NULL;
				continue;
			}
			u16 wc = readU16(is);
			if (m_items[item_i] != NULL) {
				if (m_items[item_i]->getContent() == c) {
					m_items[item_i]->setWear(wc);
					m_items[item_i]->setCount(wc);
					continue;
				}
				delete m_items[item_i];
			}
			m_items[item_i] = InventoryItem::create(c,wc,wc);
		}
		if (readU8(is) != 3)
			throw SerializationError("Unknown inventory identifier");
		return;
	}

	is.unget();

	clearAllowed();
	clearDenied();
	m_stackable = true;

	for (;;) {
		std::string line;
		std::getline(is, line, '\n');

		std::istringstream iss(line);

		std::string name;
		std::getline(iss, name, ' ');

		if (name == "EndInventoryList") {
			break;
		}else if (name == "Unstackable") {
			m_stackable = false;
		}else if (name == "Allowed") {
			u16 c;
			iss >> c;
			m_allowed[c] = true;
		}else if (name == "Denied") {
			u16 c;
			iss >> c;
			m_denied[c] = true;
		}else if (name == "Item") {
			if(item_i >= getSize())
				throw SerializationError("too many items");
			content_t c;
			u16 count;
			u16 wear;
			u16 data;

			c = InventoryItem::info(iss,&count,&wear,&data);
			if (c == CONTENT_IGNORE) {
				if (m_items[item_i] != NULL)
					delete m_items[item_i];
				m_items[item_i++] = NULL;
				continue;
			}
			if (m_items[item_i] != NULL) {
				if (m_items[item_i]->getContent() == c) {
					m_items[item_i]->setWear(wear);
					m_items[item_i++]->setCount(count);
					m_items[item_i++]->setData(data);
					continue;
				}
				delete m_items[item_i];
			}
			m_items[item_i++] = InventoryItem::create(c,count,wear);
		}else if (name == "Empty") {
			if (item_i >= getSize())
				throw SerializationError("too many items");
			if (m_items[item_i] != NULL)
				delete m_items[item_i];
			m_items[item_i++] = NULL;
		}else{
			throw SerializationError("Unknown inventory identifier");
		}
	}
}

InventoryList::InventoryList(const InventoryList &other)
{
	/*
		Do this so that the items get cloned. Otherwise the pointers
		in the array will just get copied.
	*/
	*this = other;
}

InventoryList & InventoryList::operator = (const InventoryList &other)
{
	m_name = other.m_name;
	m_size = other.m_size;
	clearItems();
	for (u32 i=0; i<other.m_items.size(); i++) {
		InventoryItem *item = other.m_items[i];
		if (item != NULL)
			m_items[i] = item->clone();
	}
	m_stackable = other.m_stackable;
	m_allowed = other.m_allowed;
	m_denied = other.m_denied;

	return *this;
}

const std::string &InventoryList::getName() const
{
	return m_name;
}

u32 InventoryList::getSize()
{
	return m_items.size();
}

u32 InventoryList::getUsedSlots()
{
	u32 num = 0;
	for (u32 i=0; i<m_items.size(); i++) {
		InventoryItem *item = m_items[i];
		if (item != NULL)
			num++;
	}
	return num;
}

u32 InventoryList::getFreeSlots()
{
	return getSize() - getUsedSlots();
}

const InventoryItem * InventoryList::getItem(u32 i) const
{
	if (i >= m_items.size())
		return NULL;
	return m_items[i];
}

InventoryItem * InventoryList::getItem(u32 i)
{
	if (i >= m_items.size())
		return NULL;
	return m_items[i];
}

InventoryItem * InventoryList::changeItem(u32 i, InventoryItem *newitem)
{
	assert(i < m_items.size());
	if (newitem != NULL && !isAllowed(newitem))
		return newitem;

	InventoryItem *olditem = m_items[i];
	m_items[i] = newitem;
	m_diff.add(m_name,i,m_items[i]);
	return olditem;
}

void InventoryList::deleteItem(u32 i)
{
	assert(i < m_items.size());
	InventoryItem *item = changeItem(i, NULL);
	if (item)
		delete item;
	m_diff.add(m_name,i,m_items[i]);
}

InventoryItem * InventoryList::addItem(InventoryItem *newitem)
{
	if (newitem == NULL)
		return NULL;

	if (!isAllowed(newitem))
		return newitem;

	/*
		First try to find if it could be added to some existing items
	*/
	if (m_stackable) {
		for (u32 i=0; i<m_items.size(); i++) {
			// Ignore empty slots
			if (m_items[i] == NULL)
				continue;
			// Try adding
			newitem = addItem(i, newitem);
			if (newitem == NULL)
				return NULL; // All was eaten
		}
	}

	/*
		Then try to add it to empty slots
	*/
	for(u32 i=0; i<m_items.size(); i++) {
		// Ignore unempty slots
		if (m_items[i] != NULL)
			continue;
		// Try adding
		newitem = addItem(i, newitem);
		if (newitem == NULL)
			return NULL; // All was eaten
	}

	// Return leftover
	return newitem;
}

InventoryItem * InventoryList::addItem(u32 i, InventoryItem *newitem)
{
	if (newitem == NULL)
		return NULL;

	if (!isAllowed(newitem))
		return newitem;

	InventoryItem *to_item = getItem(i);

	if (m_stackable == false) {
		if (newitem->getCount() > 1) {
			if (to_item != NULL)
				return newitem;
			newitem->remove(1);
			m_items[i] = newitem->clone();
			m_items[i]->setCount(1);
			m_diff.add(m_name,i,m_items[i]);
			return newitem;
		}
		m_items[i] = newitem;
		m_diff.add(m_name,i,m_items[i]);
		return to_item;
	}

	// If it is an empty position, it's an easy job.
	if (to_item == NULL) {
		m_items[i] = newitem;
		m_diff.add(m_name,i,m_items[i]);
		return NULL;
	}

	// If not addable, return the item
	if (newitem->addableTo(to_item) == false)
		return newitem;

	// If the item fits fully in the slot, add counter and delete it
	if (newitem->getCount() <= to_item->freeSpace()) {
		to_item->add(newitem->getCount());
		m_diff.add(m_name,i,m_items[i]);
		delete newitem;
		return NULL;
	}
	// Else the item does not fit fully. Add all that fits and return
	// the rest.
	else{
		u16 freespace = to_item->freeSpace();
		if (!freespace)
			return newitem;
		to_item->add(freespace);
		m_diff.add(m_name,i,m_items[i]);
		newitem->remove(freespace);
		return newitem;
	}
}

void InventoryList::updateItem(u32 i, content_t type, u16 wear_count, u16 data)
{
	if (type == CONTENT_IGNORE) {
		if (m_items[i] != NULL)
			delete m_items[i];
		m_items[i] = NULL;
		return;
	}
	if (m_items[i] != NULL) {
		if (m_items[i]->getContent() == type) {
			if (
				(type&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK
				|| (type&CONTENT_CLOTHESITEM_MASK) == CONTENT_CLOTHESITEM_MASK
			) {
				m_items[i]->setWear(wear_count);
			}else{
				m_items[i]->setCount(wear_count);
			}
			m_items[i]->setData(data);
			return;
		}
		delete m_items[i];
	}
	m_items[i] = InventoryItem::create(type,wear_count,wear_count,data);
}

bool InventoryList::itemFits(const u32 i, const InventoryItem *newitem)
{
	// If it is an empty position, it's an easy job.
	const InventoryItem *to_item = getItem(i);
	if (to_item == NULL)
		return true;

	// If not addable, fail
	if (newitem->addableTo(to_item) == false)
		return false;

	// If the item fits fully in the slot, pass
	if (newitem->getCount() <= to_item->freeSpace())
		return true;

	return false;
}

bool InventoryList::roomForItem(const InventoryItem *item)
{
	for (u32 i=0; i<m_items.size(); i++) {
		if (itemFits(i, item))
			return true;
	}
	return false;
}

bool InventoryList::roomForCookedItem(const InventoryItem *item)
{
	if (!item)
		return false;
	const InventoryItem *cook = item->createCookResult();
	if (!cook)
		return false;
	bool room = roomForItem(cook);
	delete cook;
	return room;
}

InventoryItem * InventoryList::takeItem(u32 i, u32 count)
{
	if (count == 0)
		return NULL;

	InventoryItem *item = getItem(i);
	// If it is an empty position, return NULL
	if (item == NULL)
		return NULL;

	if (count >= item->getCount()) {
		// Get the item by swapping NULL to its place
		InventoryItem *item = changeItem(i, NULL);
		m_diff.add(m_name,i,m_items[i]);
		return item;
	}else{
		InventoryItem *item2 = item->clone();
		item->remove(count);
		item2->setCount(count);
		m_diff.add(m_name,i,m_items[i]);
		return item2;
	}

	return NULL;
}

InventoryItem * InventoryList::findItem(content_t c, u16 *item_i)
{
	for (u32 i=0; i<m_items.size(); i++) {
		InventoryItem *item = getItem(i);
		if (item && item->getContent() == c) {
			if (item_i)
				*item_i = i;
			return item;
		}
	}
	return NULL;
}

void InventoryList::decrementMaterials(u16 count)
{
	for (u32 i=0; i<m_items.size(); i++) {
		InventoryItem *item = takeItem(i, count);
		if (item)
			delete item;
		m_diff.add(m_name,i,m_items[i]);
	}
}

void InventoryList::print(std::ostream &o)
{
	o<<"InventoryList:"<<std::endl;
	for( u32 i=0; i<m_items.size(); i++) {
		InventoryItem *item = m_items[i];
		if (item == NULL)
			continue;
		o<<i<<": ";
		item->serialize(o);
		o<<"\n";
	}
}

/*
	Inventory
*/

Inventory::~Inventory()
{
	clear();
}

void Inventory::clear()
{
	for (u32 i=0; i<m_lists.size(); i++) {
		delete m_lists[i];
	}
	m_lists.clear();
}

Inventory::Inventory()
{
}

Inventory::Inventory(const Inventory &other)
{
	*this = other;
}

Inventory & Inventory::operator = (const Inventory &other)
{
	clear();
	for (u32 i=0; i<other.m_lists.size(); i++) {
		m_lists.push_back(new InventoryList(*other.m_lists[i]));
	}
	return *this;
}

void Inventory::serialize(std::ostream &os) const
{
	// start of header
	writeU8(os,1);
	for (u32 i=0; i<m_lists.size(); i++) {
		InventoryList *list = m_lists[i];
		os<<list->getName()<<" ";
		writeU16(os,list->getSize());
		// start header
		writeU8(os,1);
		list->serialize(os);
		// end text
		writeU8(os,3);
	}

	// end transmission
	writeU8(os,4);
}

void Inventory::deSerialize(std::istream &is)
{
	//clear();
	char c;
	is.get(c);
	if (c == '\1') {
		while (readU8(is) != 4) {
			is.unget();
			std::string name;
			u16 size;
			std::getline(is,name,' ');
			size = readU16(is);
			InventoryList *list = addList(name,size);
			list->deSerialize(is);
		}
		return;
	}

	is.unget();

	for (;;) {
		std::string line;
		std::getline(is, line, '\n');

		std::istringstream iss(line);

		std::string name;
		std::getline(iss, name, ' ');

		if (name == "EndInventory") {
			break;
		}else if(name == "List") {
			std::string listname;
			u32 listsize;

			std::getline(iss, listname, ' ');
			iss>>listsize;

			InventoryList *list = addList(listname,listsize);

			list->deSerialize(is);
		}else{
			throw SerializationError("Unknown inventory identifier");
		}
	}
}

InventoryList * Inventory::addList(const std::string &name, u32 size)
{
	s32 i = getListIndex(name);
	if (i != -1) {
		if (m_lists[i]->getSize() != size) {
			delete m_lists[i];
			m_lists[i] = new InventoryList(name, size);
		}
		return m_lists[i];
	}else{
		m_lists.push_back(new InventoryList(name, size));
		return m_lists.getLast();
	}
}

InventoryList * Inventory::getList(const std::string &name)
{
	s32 i = getListIndex(name);
	if (i == -1)
		return NULL;
	return m_lists[i];
}

const InventoryList * Inventory::getList(const std::string &name) const
{
	s32 i = getListIndex(name);
	if (i == -1)
		return NULL;
	return m_lists[i];
}

const s32 Inventory::getListIndex(const std::string &name) const
{
	for (u32 i=0; i<m_lists.size(); i++) {
		if (m_lists[i]->getName() == name)
			return i;
	}
	return -1;
}

/*
	InventoryAction
*/

InventoryAction * InventoryAction::deSerialize(std::istream &is)
{
	std::string type;
	std::getline(is, type, ' ');

	if (type == "Move")
		return new IMoveAction(is);

	return NULL;
}

static std::string describeC(const struct InventoryContext *c)
{
	if (c->current_player == NULL) {
		return "current_player=NULL";
	}else{
		return std::string("current_player=") + c->current_player->getName();
	}
}

void IMoveAction::apply(InventoryContext *c, InventoryManager *mgr)
{
	Inventory *inv_from = mgr->getInventory(c, from_inv);
	Inventory *inv_to = mgr->getInventory(c, to_inv);

	if (!inv_from) {
		infostream<<"IMoveAction::apply(): FAIL: source inventory not found: "
				<<"context=["<<describeC(c)<<"], from_inv=\""<<from_inv<<"\""
				<<", to_inv=\""<<to_inv<<"\""<<std::endl;
		return;
	}
	if (!inv_to) {
		infostream<<"IMoveAction::apply(): FAIL: destination inventory not found: "
				"context=["<<describeC(c)<<"], from_inv=\""<<from_inv<<"\""
				<<", to_inv=\""<<to_inv<<"\""<<std::endl;
		return;
	}

	InventoryList *list_from = inv_from->getList(from_list);
	InventoryList *list_to = inv_to->getList(to_list);

	/*
		If a list doesn't exist or the source item doesn't exist
	*/
	if (!list_from) {
		infostream<<"IMoveAction::apply(): FAIL: source list not found: "
				<<"context=["<<describeC(c)<<"], from_inv=\""<<from_inv<<"\""
				<<", from_list=\""<<from_list<<"\""<<std::endl;
		return;
	}
	if (!list_to) {
		infostream<<"IMoveAction::apply(): FAIL: destination list not found: "
				<<"context=["<<describeC(c)<<"], to_inv=\""<<to_inv<<"\""
				<<", to_list=\""<<to_list<<"\""<<std::endl;
		return;
	}
	if (list_from->getItem(from_i) == NULL) {
		infostream<<"IMoveAction::apply(): FAIL: source item not found: "
				<<"context=["<<describeC(c)<<"], from_inv=\""<<from_inv<<"\""
				<<", from_list=\""<<from_list<<"\""
				<<" from_i="<<from_i<<std::endl;
		return;
	}
	/*
		If the source and the destination slots are the same
	*/
	if (inv_from == inv_to && list_from == list_to && from_i == to_i) {
		infostream<<"IMoveAction::apply(): FAIL: source and destination slots "
				<<"are the same: inv=\""<<from_inv<<"\" list=\""<<from_list
				<<"\" i="<<from_i<<std::endl;
		return;
	}

	if (count == 0 && list_to->getStackable() == false)
		count = 1;

	// Take item from source list
	InventoryItem *item1 = NULL;
	if (count == 0) {
		item1 = list_from->changeItem(from_i, NULL);
		if (item1 != NULL)
			count = item1->getCount();
	}else{
		item1 = list_from->takeItem(from_i, count);
	}

	// Try to add the item to destination list
	InventoryItem *olditem = item1;
	item1 = list_to->addItem(to_i, item1);

	// If something is returned, the item was not fully added
	if (item1 != NULL) {
		// If olditem is returned, nothing was added.
		bool nothing_added = (item1 == olditem && item1->getCount() == count);

		// If something else is returned, part of the item was left unadded.
		// Add the other part back to the source item
		list_from->addItem(from_i, item1);

		// If olditem is returned, nothing was added.
		// Swap the items
		if (nothing_added) {
			// Take item from source list
			item1 = list_from->changeItem(from_i, NULL);
			// Adding was not possible, swap the items.
			InventoryItem *item2 = list_to->changeItem(to_i, item1);
			// Put item from destination list to the source list
			list_from->changeItem(from_i, item2);
		}
	}

	mgr->inventoryModified(c, from_inv);
	if (from_inv != to_inv)
		mgr->inventoryModified(c, to_inv);

	infostream<<"IMoveAction::apply(): moved at "
			<<"["<<describeC(c)<<"]"
			<<" from inv=\""<<from_inv<<"\""
			<<" list=\""<<from_list<<"\""
			<<" i="<<from_i
			<<" to inv=\""<<to_inv<<"\""
			<<" list=\""<<to_list<<"\""
			<<" i="<<to_i
			<<std::endl;
}

/*
	InventoryLocation
*/

std::string InventoryLocation::dump() const
{
	std::ostringstream os(std::ios::binary);
	serialize(os);
	return os.str();
}

void InventoryLocation::serialize(std::ostream &os) const
{
	switch (type) {
	case InventoryLocation::UNDEFINED:
		os<<"undefined";
		break;
	case InventoryLocation::CURRENT_PLAYER:
		os<<"current_player";
		break;
	case InventoryLocation::PLAYER:
		os<<"player:"<<name;
		break;
	case InventoryLocation::NODEMETA:
		os<<"nodemeta:"<<p.X<<","<<p.Y<<","<<p.Z;
		break;
	default:
		os<<"undefined";
	}
}

void InventoryLocation::deSerialize(std::istream &is)
{
	std::string tname;
	std::getline(is, tname, ':');
	if (tname == "undefined") {
		type = InventoryLocation::UNDEFINED;
	}else if (tname == "current_player") {
		type = InventoryLocation::CURRENT_PLAYER;
	}else if (tname == "player") {
		type = InventoryLocation::PLAYER;
		std::getline(is, name, '\n');
	}else if (tname == "nodemeta") {
		type = InventoryLocation::NODEMETA;
		std::string pos;
		std::getline(is, pos, '\n');
		Strfnd fn(pos);
		p.X = mystoi(fn.next(","));
		p.Y = mystoi(fn.next(","));
		p.Z = mystoi(fn.next(","));
	}else{
		infostream<<"Unknown InventoryLocation type=\""<<tname<<"\""<<std::endl;
		throw SerializationError("Unknown InventoryLocation type");
	}
}

void InventoryLocation::deSerialize(std::string s)
{
	std::istringstream is(s, std::ios::binary);
	deSerialize(is);
}

//END
