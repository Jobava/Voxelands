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

/*
(c) 2010 Perttu Ahola <celeron55@gmail.com>
*/

#include "inventory.h"
#include "serialization.h"
#include "utility.h"
#include "debug.h"
#include <sstream>
#include "main.h"
#include "serverobject.h"
#include "content_mapnode.h"
#include "content_inventory.h"
#include "content_sao.h"
#include "player.h"
#include "log.h"

/*
	InventoryItem
*/

InventoryItem::InventoryItem(u16 count)
{
	m_content = CONTENT_IGNORE;
	m_count = count;
}

InventoryItem::~InventoryItem()
{
}

content_t content_translate_from_19_to_internal(content_t c_from)
{
	for(u32 i=0; i<sizeof(trans_table_19)/sizeof(trans_table_19[0]); i++)
	{
		if(trans_table_19[i][1] == c_from)
		{
			return trans_table_19[i][0];
		}
	}
	return c_from;
}

InventoryItem* InventoryItem::deSerialize(std::istream &is)
{
	DSTACK(__FUNCTION_NAME);

	//is.imbue(std::locale("C"));
	// Read name
	std::string name;
	std::getline(is, name, ' ');

	if(name == "MaterialItem")
	{
		// u16 reads directly as a number (u8 doesn't)
		u16 material;
		is>>material;
		u16 count;
		is>>count;
		// Convert old materials
		if(material <= 0xff)
		{
			material = content_translate_from_19_to_internal(material);
		}
		if(material > MAX_CONTENT)
			throw SerializationError("Too large material number");
		return new MaterialItem(material, count);
	}
	else if(name == "MaterialItem2")
	{
		u16 material;
		is>>material;
		u16 count;
		is>>count;
		if(material > MAX_CONTENT)
			throw SerializationError("Too large material number");
		return new MaterialItem(material, count);
	}
	else if(name == "MBOItem")
	{
		std::string inventorystring;
		std::getline(is, inventorystring, '|');
		throw SerializationError("MBOItem not supported anymore");
	}
	else if(name == "CraftItem")
	{
		std::string subname;
		std::getline(is, subname, ' ');
		u16 count;
		is>>count;
		return new CraftItem(subname, count);
	}
	else if(name == "CraftItem2")
	{
		u16 material;
		is>>material;
		u16 count;
		is>>count;
		return new CraftItem(material, count);
	}
	else if(name == "ToolItem")
	{
		std::string toolname;
		std::getline(is, toolname, ' ');
		u16 wear;
		is>>wear;
		return new ToolItem(toolname, wear);
	}
	else if(name == "ToolItem2")
	{
		u16 material;
		is>>material;
		u16 wear;
		is>>wear;
		return new ToolItem(material, wear);
	}
	else
	{
		infostream<<"Unknown InventoryItem name=\""<<name<<"\""<<std::endl;
		throw SerializationError("Unknown InventoryItem name");
	}
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

/*
	CraftItem
*/

#ifndef SERVER
video::ITexture * CraftItem::getImage() const
{
	if(g_texturesource == NULL)
		return NULL;

	std::string name = content_craftitem_features(m_content).texture;

	// Get such a texture
	return g_texturesource->getTextureRaw(name);
}
#endif
std::string CraftItem::getGuiName()
{
	return content_craftitem_features(m_content).description;
}

ServerActiveObject* CraftItem::createSAO(ServerEnvironment *env, u16 id, v3f pos)
{
	// Special cases
	ServerActiveObject *obj = item_craft_create_object(m_subname, env, id, pos);
	if(obj)
		return obj;
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
	return content_craftitem_features(m_content).cook_result != "";
}

InventoryItem *CraftItem::createCookResult() const
{
	std::istringstream is(content_craftitem_features(m_content).cook_result, std::ios::binary);
	return InventoryItem::deSerialize(is);
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
	if (content_craftitem_features(m_content).edible) {
		u16 result_count = getCount() - 1; // Eat one at a time
		s16 hp_change = content_craftitem_features(m_content).edible;
		if (hp_change) {
			if (player->hunger < 20) {
				if (player->hunger + hp_change > 20) {
					hp_change -= 20-player->hunger;
					player->hunger = 20;
				}else{
					player->hunger += hp_change;
					hp_change = 0;
				}
			}
			if (player->hp + hp_change > 20) {
				player->hp = 20;
			}else{
				player->hp += hp_change;
			}
		}

		if(result_count < 1)
			return true;
		else
			setCount(result_count);
	}
	return false;
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
	for(u32 i=0; i<m_items.size(); i++)
	{
		if(m_items[i])
			delete m_items[i];
	}

	m_items.clear();

	for(u32 i=0; i<m_size; i++)
	{
		m_items.push_back(NULL);
	}

	//setDirty(true);
}

void InventoryList::serialize(std::ostream &os) const
{
	//os.imbue(std::locale("C"));

	for(u32 i=0; i<m_items.size(); i++)
	{
		InventoryItem *item = m_items[i];
		if(item != NULL)
		{
			os<<"Item ";
			item->serialize(os);
		}
		else
		{
			os<<"Empty";
		}
		os<<"\n";
	}

	if (!m_stackable)
		os<<"Unstackable\n";
	for (std::map<content_t, bool>::const_iterator i = m_allowed.begin(); i != m_allowed.end(); ++i) {
		content_t c = i->first;
		bool s = i->second;
		if (!s)
			continue;
		os<<"Allowed ";
		os<<itos(c);
		os<<"\n";
	}
	for (std::map<content_t, bool>::const_iterator i = m_denied.begin(); i != m_denied.end(); ++i) {
		content_t c = i->first;
		bool s = i->second;
		if (!s)
			continue;
		os<<"Denied ";
		os<<itos(c);
		os<<"\n";
	}

	os<<"EndInventoryList\n";
}

void InventoryList::deSerialize(std::istream &is)
{
	clearItems();
	clearAllowed();
	clearDenied();
	m_stackable = true;
	u32 item_i = 0;

	for(;;)
	{
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
			if(item_i > getSize() - 1)
				throw SerializationError("too many items");
			InventoryItem *item = InventoryItem::deSerialize(iss);
			m_items[item_i++] = item;
		}else if (name == "Empty") {
			if(item_i > getSize() - 1)
				throw SerializationError("too many items");
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
	for(u32 i=0; i<other.m_items.size(); i++)
	{
		InventoryItem *item = other.m_items[i];
		if(item != NULL)
		{
			m_items[i] = item->clone();
		}
	}
	m_stackable = other.m_stackable;
	m_allowed = other.m_allowed;
	m_denied = other.m_denied;
	//setDirty(true);

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
	for(u32 i=0; i<m_items.size(); i++)
	{
		InventoryItem *item = m_items[i];
		if(item != NULL)
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
	if(i > m_items.size() - 1)
		return NULL;
	return m_items[i];
}

InventoryItem * InventoryList::getItem(u32 i)
{
	if(i > m_items.size() - 1)
		return NULL;
	return m_items[i];
}

InventoryItem * InventoryList::changeItem(u32 i, InventoryItem *newitem)
{
	assert(i < m_items.size());

	InventoryItem *olditem = m_items[i];
	m_items[i] = newitem;
	//setDirty(true);
	return olditem;
}

void InventoryList::deleteItem(u32 i)
{
	assert(i < m_items.size());
	InventoryItem *item = changeItem(i, NULL);
	if(item)
		delete item;
}

InventoryItem * InventoryList::addItem(InventoryItem *newitem)
{
	if(newitem == NULL)
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
			return newitem;
		}
		m_items[i] = newitem;
		return to_item;
	}

	// If it is an empty position, it's an easy job.
	if (to_item == NULL) {
		m_items[i] = newitem;
		return NULL;
	}

	// If not addable, return the item
	if (newitem->addableTo(to_item) == false)
		return newitem;

	// If the item fits fully in the slot, add counter and delete it
	if (newitem->getCount() <= to_item->freeSpace()) {
		to_item->add(newitem->getCount());
		delete newitem;
		return NULL;
	}
	// Else the item does not fit fully. Add all that fits and return
	// the rest.
	else{
		u16 freespace = to_item->freeSpace();
		to_item->add(freespace);
		newitem->remove(freespace);
		return newitem;
	}
}

bool InventoryList::itemFits(const u32 i, const InventoryItem *newitem)
{
	// If it is an empty position, it's an easy job.
	const InventoryItem *to_item = getItem(i);
	if(to_item == NULL)
	{
		return true;
	}

	// If not addable, fail
	if(newitem->addableTo(to_item) == false)
		return false;

	// If the item fits fully in the slot, pass
	if(newitem->getCount() <= to_item->freeSpace())
	{
		return true;
	}

	return false;
}

bool InventoryList::roomForItem(const InventoryItem *item)
{
	for(u32 i=0; i<m_items.size(); i++)
		if(itemFits(i, item))
			return true;
	return false;
}

bool InventoryList::roomForCookedItem(const InventoryItem *item)
{
	if(!item)
		return false;
	const InventoryItem *cook = item->createCookResult();
	if(!cook)
		return false;
	bool room = roomForItem(cook);
	delete cook;
	return room;
}

InventoryItem * InventoryList::takeItem(u32 i, u32 count)
{
	if(count == 0)
		return NULL;

	//setDirty(true);

	InventoryItem *item = getItem(i);
	// If it is an empty position, return NULL
	if(item == NULL)
		return NULL;

	if(count >= item->getCount())
	{
		// Get the item by swapping NULL to its place
		return changeItem(i, NULL);
	}
	else
	{
		InventoryItem *item2 = item->clone();
		item->remove(count);
		item2->setCount(count);
		return item2;
	}

	return NULL;
}

void InventoryList::decrementMaterials(u16 count)
{
	for(u32 i=0; i<m_items.size(); i++)
	{
		InventoryItem *item = takeItem(i, count);
		if(item)
			delete item;
	}
}

void InventoryList::print(std::ostream &o)
{
	o<<"InventoryList:"<<std::endl;
	for(u32 i=0; i<m_items.size(); i++)
	{
		InventoryItem *item = m_items[i];
		if(item != NULL)
		{
			o<<i<<": ";
			item->serialize(o);
			o<<"\n";
		}
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
	for(u32 i=0; i<m_lists.size(); i++)
	{
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
	for(u32 i=0; i<other.m_lists.size(); i++)
	{
		m_lists.push_back(new InventoryList(*other.m_lists[i]));
	}
	return *this;
}

void Inventory::serialize(std::ostream &os) const
{
	for(u32 i=0; i<m_lists.size(); i++)
	{
		InventoryList *list = m_lists[i];
		os<<"List "<<list->getName()<<" "<<list->getSize()<<"\n";
		list->serialize(os);
	}

	os<<"EndInventory\n";
}

void Inventory::deSerialize(std::istream &is)
{
	clear();

	for(;;)
	{
		std::string line;
		std::getline(is, line, '\n');

		std::istringstream iss(line);

		std::string name;
		std::getline(iss, name, ' ');

		if(name == "EndInventory")
		{
			break;
		}
		// This is a temporary backwards compatibility fix
		else if(name == "end")
		{
			break;
		}
		else if(name == "List")
		{
			std::string listname;
			u32 listsize;

			std::getline(iss, listname, ' ');
			iss>>listsize;

			InventoryList *list = new InventoryList(listname, listsize);
			list->deSerialize(is);

			m_lists.push_back(list);
		}
		else
		{
			throw SerializationError("Unknown inventory identifier");
		}
	}
}

InventoryList * Inventory::addList(const std::string &name, u32 size)
{
	s32 i = getListIndex(name);
	if(i != -1)
	{
		if(m_lists[i]->getSize() != size)
		{
			delete m_lists[i];
			m_lists[i] = new InventoryList(name, size);
		}
		return m_lists[i];
	}
	else
	{
		m_lists.push_back(new InventoryList(name, size));
		return m_lists.getLast();
	}
}

InventoryList * Inventory::getList(const std::string &name)
{
	s32 i = getListIndex(name);
	if(i == -1)
		return NULL;
	return m_lists[i];
}

const InventoryList * Inventory::getList(const std::string &name) const
{
	s32 i = getListIndex(name);
	if(i == -1)
		return NULL;
	return m_lists[i];
}

const s32 Inventory::getListIndex(const std::string &name) const
{
	for(u32 i=0; i<m_lists.size(); i++) {
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

	if(type == "Move")
		return new IMoveAction(is);

	return NULL;
}

static std::string describeC(const struct InventoryContext *c)
{
	if(c->current_player == NULL) {
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
	if(from_inv != to_inv)
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
        switch(type) {
        case InventoryLocation::UNDEFINED:
        {
		os<<"undefined";
	}
        break;
        case InventoryLocation::CURRENT_PLAYER:
        {
		os<<"current_player";
	}
        break;
        case InventoryLocation::PLAYER:
        {
		os<<"player:"<<name;
        }
        break;
        case InventoryLocation::NODEMETA:
        {
		os<<"nodemeta:"<<p.X<<","<<p.Y<<","<<p.Z;
        }
        break;
        default:
                assert(0);
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

/*
	Craft checking system
*/

bool ItemSpec::checkItem(const InventoryItem *item) const
{
	if(type == ITEM_NONE)
	{
		// Has to be no item
		if(item != NULL)
			return false;
		return true;
	}

	// There should be an item
	if(item == NULL)
		return false;

	std::string itemname = item->getName();

	if(type == ITEM_MATERIAL)
	{
		if(itemname != "MaterialItem")
			return false;
		MaterialItem *mitem = (MaterialItem*)item;
		if(mitem->getMaterial() != num)
			return false;
	}
	else if(type == ITEM_CRAFT)
	{
		if(itemname != "CraftItem")
			return false;
		CraftItem *mitem = (CraftItem*)item;
		if(mitem->getSubName() != name)
			return false;
	}
	else if(type == ITEM_TOOL)
	{
		// Not supported yet
		assert(0);
	}
	else if(type == ITEM_MBO)
	{
		// Not supported yet
		assert(0);
	}
	else
	{
		// Not supported yet
		assert(0);
	}
	return true;
}

//END
