/*
Minetest-c55
Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>

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

#include "content_nodemeta.h"
#include "inventory.h"
#include "content_mapnode.h"
#include "content_craft.h"
#include "content_list.h"
#include "log.h"
#include "player.h"

/*
	SignNodeMetadata
*/

// Prototype
SignNodeMetadata proto_SignNodeMetadata("");

SignNodeMetadata::SignNodeMetadata(std::string text):
	m_text(text)
{
	NodeMetadata::registerType(typeId(), create);
}
u16 SignNodeMetadata::typeId() const
{
	return CONTENT_SIGN_WALL;
}
NodeMetadata* SignNodeMetadata::create(std::istream &is)
{
	std::string text = deSerializeString(is);
	return new SignNodeMetadata(text);
}
NodeMetadata* SignNodeMetadata::clone()
{
	return new SignNodeMetadata(m_text);
}
void SignNodeMetadata::serializeBody(std::ostream &os)
{
	os<<serializeString(m_text);
}
std::string SignNodeMetadata::infoText()
{
	return std::string("\"")+m_text+"\"";
}
std::string SignNodeMetadata::getDrawSpecString()
{
	return std::string("field[text;;") + m_text + "]";
}

/*
	LockingSignNodeMetadata
*/

// Prototype
LockingSignNodeMetadata proto_LockingSignNodeMetadata("");

LockingSignNodeMetadata::LockingSignNodeMetadata(std::string text):
	m_text(text)
{
	NodeMetadata::registerType(typeId(), create);
}
u16 LockingSignNodeMetadata::typeId() const
{
	return CONTENT_LOCKABLE_SIGN_WALL;
}
NodeMetadata* LockingSignNodeMetadata::create(std::istream &is)
{
	std::string text = deSerializeString(is);
	LockingSignNodeMetadata *d = new LockingSignNodeMetadata(text);
	d->setOwner(deSerializeString(is));
	return d;
}
NodeMetadata* LockingSignNodeMetadata::clone()
{
	return new LockingSignNodeMetadata(m_text);
}
void LockingSignNodeMetadata::serializeBody(std::ostream &os)
{
	os<<serializeString(m_text);
	os<<serializeString(m_owner);
}
std::string LockingSignNodeMetadata::infoText()
{
	return std::string("(")+m_owner+") \""+m_text+"\"";
}
bool LockingSignNodeMetadata::receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player)
{
	if (getOwner() != player->getName())
		return false;
	m_text = fields["text"];
	return true;
}
std::string LockingSignNodeMetadata::getDrawSpecString()
{
	return std::string("field[text;;") + m_text + "]";
}

/*
	ChestNodeMetadata
*/

// Prototype
ChestNodeMetadata proto_ChestNodeMetadata;

ChestNodeMetadata::ChestNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_inventory = new Inventory();
	m_inventory->addList("0", 8*4);
}
ChestNodeMetadata::~ChestNodeMetadata()
{
	delete m_inventory;
}
u16 ChestNodeMetadata::typeId() const
{
	return CONTENT_CHEST;
}
NodeMetadata* ChestNodeMetadata::create(std::istream &is)
{
	ChestNodeMetadata *d = new ChestNodeMetadata();
	d->m_inventory->deSerialize(is);
	return d;
}
NodeMetadata* ChestNodeMetadata::clone()
{
	ChestNodeMetadata *d = new ChestNodeMetadata();
	*d->m_inventory = *m_inventory;
	return d;
}
void ChestNodeMetadata::serializeBody(std::ostream &os)
{
	m_inventory->serialize(os);
}
std::string ChestNodeMetadata::infoText()
{
	return "Chest";
}
bool ChestNodeMetadata::nodeRemovalDisabled()
{
	/*
		Disable removal if chest contains something
	*/
	InventoryList *list = m_inventory->getList("0");
	if(list == NULL)
		return false;
	if(list->getUsedSlots() == 0)
		return false;
	return true;
}
std::string ChestNodeMetadata::getDrawSpecString()
{
	return
		"size[8,9]"
		"list[current_name;0;0,0;8,4;]"
		"list[current_player;main;0,5;8,4;]";
}

/*
	LockingChestNodeMetadata
*/

// Prototype
LockingChestNodeMetadata proto_LockingChestNodeMetadata;

LockingChestNodeMetadata::LockingChestNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_inventory = new Inventory();
	m_inventory->addList("0", 8*4);
}
LockingChestNodeMetadata::~LockingChestNodeMetadata()
{
	delete m_inventory;
}
u16 LockingChestNodeMetadata::typeId() const
{
	return CONTENT_LOCKABLE_CHEST;
}
NodeMetadata* LockingChestNodeMetadata::create(std::istream &is)
{
	LockingChestNodeMetadata *d = new LockingChestNodeMetadata();
	d->setOwner(deSerializeString(is));
	d->m_inventory->deSerialize(is);
	return d;
}
NodeMetadata* LockingChestNodeMetadata::clone()
{
	LockingChestNodeMetadata *d = new LockingChestNodeMetadata();
	*d->m_inventory = *m_inventory;
	return d;
}
void LockingChestNodeMetadata::serializeBody(std::ostream &os)
{
	os<<serializeString(m_owner);
	m_inventory->serialize(os);
}
std::string LockingChestNodeMetadata::infoText()
{
	return std::string("Locking Chest owned by '")+m_owner+"'";
}
bool LockingChestNodeMetadata::nodeRemovalDisabled()
{
	/*
		Disable removal if chest contains something
	*/
	InventoryList *list = m_inventory->getList("0");
	if(list == NULL)
		return false;
	if(list->getUsedSlots() == 0)
		return false;
	return true;
}
std::string LockingChestNodeMetadata::getDrawSpecString()
{
	return
		"size[8,9]"
		"list[current_name;0;0,0;8,4;]"
		"list[current_player;main;0,5;8,4;]";
}

/*
	ChestNodeMetadata
*/

// Prototype
CreativeChestNodeMetadata proto_CreativeChestNodeMetadata;

CreativeChestNodeMetadata::CreativeChestNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_page = 0;

	m_inventory = new Inventory();
	m_inventory->addList("0", 8*4);
}
CreativeChestNodeMetadata::~CreativeChestNodeMetadata()
{
	delete m_inventory;
}
u16 CreativeChestNodeMetadata::typeId() const
{
	return CONTENT_CREATIVE_CHEST;
}
NodeMetadata* CreativeChestNodeMetadata::create(std::istream &is)
{
	CreativeChestNodeMetadata *d = new CreativeChestNodeMetadata();
	d->m_inventory->deSerialize(is);
	is>>d->m_page;
	return d;
}
NodeMetadata* CreativeChestNodeMetadata::clone()
{
	CreativeChestNodeMetadata *d = new CreativeChestNodeMetadata();
	*d->m_inventory = *m_inventory;
	InventoryList *l = d->m_inventory->getList("0");
	InventoryItem *t;
	l->clearItems();
	std::vector<content_t> &list = lists::get("creative");
	for (u16 i=0; i<list.size() && i < 32; i++) {
		if ((list[i]&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
			t = new CraftItem(list[i],1);
		}else if ((list[i]&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
			t = new ToolItem(list[i],1);
		}else{
			t = new MaterialItem(list[i],1);
		}
		l->addItem(t);
	}
	return d;
}
void CreativeChestNodeMetadata::serializeBody(std::ostream &os)
{
	m_inventory->serialize(os);
	os<<itos(m_page) << " ";
}
std::string CreativeChestNodeMetadata::infoText()
{
	return "Creative Chest";
}
bool CreativeChestNodeMetadata::nodeRemovalDisabled()
{
	return false;
}
bool CreativeChestNodeMetadata::receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player)
{
	if (fields["prev"] == "" && fields["next"] == "")
		return false;
	if (fields["prev"] != "")
		m_page--;
	if (fields["next"] != "")
		m_page++;
	if (m_page < 0)
		m_page = 0;
	std::vector<content_t> &list = lists::get("creative");
	if (m_page > (list.size()/32))
		m_page = list.size()/32;
	InventoryList *l = m_inventory->getList("0");
	InventoryItem *t;
	l->clearItems();
	u16 start = m_page*32;
	u16 end = start+32;
	if (end > list.size())
		end = list.size();
	for (u16 i=start; i<end; i++) {
		if ((list[i]&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
			t = new CraftItem(list[i],1);
		}else if ((list[i]&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
			t = new ToolItem(list[i],1);
		}else{
			t = new MaterialItem(list[i],1);
		}
		l->addItem(t);
	}
	return true;
}
std::string CreativeChestNodeMetadata::getDrawSpecString()
{
	std::vector<content_t> &list = lists::get("creative");
	std::string spec("size[8,10]");
		spec += "list[current_name;0;0,0.5;8,4;]";
		spec += "button[0.25,5;2.5,0.75;prev;<< Previous Page]";
		spec += "label[3.5,5;Page ";
		spec += itos(m_page+1);
		spec +=" of ";
		spec += itos((list.size()/32)+1);
		spec += "]";
		spec += "button[6,5;2.5,0.75;next;Next Page >>]";
		spec += "list[current_player;main;0,6;8,4;]";

	return spec;
}

/*
	BorderStoneNodeMetadata
*/

// Prototype
BorderStoneNodeMetadata proto_BorderStoneNodeMetadata;

BorderStoneNodeMetadata::BorderStoneNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);
}
BorderStoneNodeMetadata::~BorderStoneNodeMetadata()
{
}
u16 BorderStoneNodeMetadata::typeId() const
{
	return CONTENT_BORDERSTONE;
}
NodeMetadata* BorderStoneNodeMetadata::create(std::istream &is)
{
	BorderStoneNodeMetadata *d = new BorderStoneNodeMetadata();
	d->setOwner(deSerializeString(is));
	return d;
}
NodeMetadata* BorderStoneNodeMetadata::clone()
{
	BorderStoneNodeMetadata *d = new BorderStoneNodeMetadata();
	return d;
}
void BorderStoneNodeMetadata::serializeBody(std::ostream &os)
{
	os<<serializeString(m_text);
}
std::string BorderStoneNodeMetadata::infoText()
{
	return std::string("Border Stone owned by '")+m_text+"'";
}

/*
	LockedDoorNodeMetadata
*/

// Prototype
LockedDoorNodeMetadata proto_LockedDoorNodeMetadata;

LockedDoorNodeMetadata::LockedDoorNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);
}
LockedDoorNodeMetadata::~LockedDoorNodeMetadata()
{
}
u16 LockedDoorNodeMetadata::typeId() const
{
	return CONTENT_WOOD_DOOR_LT;
}
NodeMetadata* LockedDoorNodeMetadata::create(std::istream &is)
{
	LockedDoorNodeMetadata *d = new LockedDoorNodeMetadata();
	d->setOwner(deSerializeString(is));
	return d;
}
NodeMetadata* LockedDoorNodeMetadata::clone()
{
	LockedDoorNodeMetadata *d = new LockedDoorNodeMetadata();
	return d;
}
void LockedDoorNodeMetadata::serializeBody(std::ostream &os)
{
	os<<serializeString(m_text);
}
std::string LockedDoorNodeMetadata::infoText()
{
	return std::string("Locked Door owned by '")+m_text+"'";
}

/*
	FurnaceNodeMetadata
*/

// Prototype
FurnaceNodeMetadata proto_FurnaceNodeMetadata;

FurnaceNodeMetadata::FurnaceNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_inventory = new Inventory();
	m_inventory->addList("fuel", 1);
	m_inventory->addList("src", 1);
	m_inventory->addList("dst", 4);

	m_step_accumulator = 0;
	m_fuel_totaltime = 0;
	m_fuel_time = 0;
	m_src_totaltime = 0;
	m_src_time = 0;
}
FurnaceNodeMetadata::~FurnaceNodeMetadata()
{
	delete m_inventory;
}
u16 FurnaceNodeMetadata::typeId() const
{
	return CONTENT_FURNACE;
}
NodeMetadata* FurnaceNodeMetadata::clone()
{
	FurnaceNodeMetadata *d = new FurnaceNodeMetadata();
	*d->m_inventory = *m_inventory;
	return d;
}
NodeMetadata* FurnaceNodeMetadata::create(std::istream &is)
{
	FurnaceNodeMetadata *d = new FurnaceNodeMetadata();

	d->m_inventory->deSerialize(is);

	int temp;
	is>>temp;
	d->m_fuel_totaltime = (float)temp/10;
	is>>temp;
	d->m_fuel_time = (float)temp/10;

	return d;
}
void FurnaceNodeMetadata::serializeBody(std::ostream &os)
{
	m_inventory->serialize(os);
	os<<itos(m_fuel_totaltime*10)<<" ";
	os<<itos(m_fuel_time*10)<<" ";
}
std::string FurnaceNodeMetadata::infoText()
{
	//return "Furnace";
	if(m_fuel_time >= m_fuel_totaltime)
	{
		const InventoryList *src_list = m_inventory->getList("src");
		assert(src_list);
		const InventoryItem *src_item = src_list->getItem(0);

		if(src_item && src_item->isCookable()) {
			InventoryList *dst_list = m_inventory->getList("dst");
			if(!dst_list->roomForCookedItem(src_item))
				return "Furnace is overloaded";
			return "Furnace is out of fuel";
		}
		else
			return "Furnace is inactive";
	}
	else
	{
		std::string s = "Furnace is active";
		// Do this so it doesn't always show (0%) for weak fuel
		if(m_fuel_totaltime > 3) {
			s += " (";
			s += itos(m_fuel_time/m_fuel_totaltime*100);
			s += "%)";
		}
		return s;
	}
}
bool FurnaceNodeMetadata::nodeRemovalDisabled()
{
	/*
		Disable removal if furnace is not empty
	*/
	InventoryList *list[3] = {m_inventory->getList("src"),
	m_inventory->getList("dst"), m_inventory->getList("fuel")};

	for(int i = 0; i < 3; i++) {
		if(list[i] == NULL)
			continue;
		if(list[i]->getUsedSlots() == 0)
			continue;
		return true;
	}
	return false;

}
void FurnaceNodeMetadata::inventoryModified()
{
	infostream<<"Furnace inventory modification callback"<<std::endl;
}
bool FurnaceNodeMetadata::step(float dtime, v3s16 pos, ServerEnvironment *env)
{
	if(dtime > 60.0)
		infostream<<"Furnace stepping a long time ("<<dtime<<")"<<std::endl;
	// Update at a fixed frequency
	const float interval = 2.0;
	m_step_accumulator += dtime;
	bool changed = false;
	while(m_step_accumulator > interval)
	{
		m_step_accumulator -= interval;
		dtime = interval;

		//infostream<<"Furnace step dtime="<<dtime<<std::endl;

		InventoryList *dst_list = m_inventory->getList("dst");
		assert(dst_list);

		InventoryList *src_list = m_inventory->getList("src");
		assert(src_list);
		InventoryItem *src_item = src_list->getItem(0);

		bool room_available = false;

		if (src_item && src_item->isCookable())
			room_available = dst_list->roomForCookedItem(src_item);

		// Start only if there are free slots in dst, so that it can
		// accomodate any result item
		if (room_available) {
			m_src_totaltime = 3;
		}else{
			m_src_time = 0;
			m_src_totaltime = 0;
		}

		/*
			If fuel is burning, increment the burn counters.
			If item finishes cooking, move it to result.
		*/
		if (m_fuel_time < m_fuel_totaltime) {
			//infostream<<"Furnace is active"<<std::endl;
			m_fuel_time += dtime;
			m_src_time += dtime;
			if (m_src_time >= m_src_totaltime && m_src_totaltime > 0.001 && src_item) {
				InventoryItem *cookresult = src_item->createCookResult();
				dst_list->addItem(cookresult);
				src_list->decrementMaterials(1);
				m_src_time = 0;
				m_src_totaltime = 0;
			}
			changed = true;

			// If the fuel was not used up this step, just keep burning it
			if (m_fuel_time < m_fuel_totaltime)
				continue;
		}

		/*
			Get the source again in case it has all burned
		*/
		src_item = src_list->getItem(0);

		/*
			If there is no source item, or the source item is not cookable,
			or the furnace is still cooking, or the furnace became overloaded, stop loop.
		*/
		if(src_item == NULL || !room_available || m_fuel_time < m_fuel_totaltime ||
			dst_list->roomForCookedItem(src_item) == false)
		{
			m_step_accumulator = 0;
			break;
		}

		//infostream<<"Furnace is out of fuel"<<std::endl;

		InventoryList *fuel_list = m_inventory->getList("fuel");
		assert(fuel_list);
		InventoryItem *fuel_item = fuel_list->getItem(0);
		if (fuel_item && fuel_item->isFuel()) {
			if ((fuel_item->getContent()&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
				m_fuel_totaltime = ((CraftItem*)fuel_item)->getFuelTime();
			}else if ((fuel_item->getContent()&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
				m_fuel_totaltime = ((ToolItem*)fuel_item)->getFuelTime();
			}else{
				m_fuel_totaltime = ((MaterialItem*)fuel_item)->getFuelTime();
			}
			m_fuel_time = 0;
			content_t c = fuel_item->getContent();
			fuel_list->decrementMaterials(1);
			if (c == CONTENT_TOOLITEM_STEELBUCKET_LAVA) {
				fuel_list->addItem(0,new ToolItem(CONTENT_TOOLITEM_STEELBUCKET,0));
			}
			changed = true;
		}else{
			m_step_accumulator = 0;
		}
	}
	return changed;
}
std::string FurnaceNodeMetadata::getDrawSpecString()
{
	return
		"size[8,9]"
		"list[current_name;fuel;2,3;1,1;]"
		"list[current_name;src;2,1;1,1;]"
		"list[current_name;dst;5,1;2,2;]"
		"list[current_player;main;0,5;8,4;]";
}

/*
	LockingFurnaceNodeMetadata
*/

// Prototype
LockingFurnaceNodeMetadata proto_LockingFurnaceNodeMetadata;

LockingFurnaceNodeMetadata::LockingFurnaceNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_inventory = new Inventory();
	m_inventory->addList("fuel", 1);
	m_inventory->addList("src", 1);
	m_inventory->addList("dst", 4);

	m_step_accumulator = 0;
	m_fuel_totaltime = 0;
	m_fuel_time = 0;
	m_src_totaltime = 0;
	m_src_time = 0;
	m_lock = 0;
}
LockingFurnaceNodeMetadata::~LockingFurnaceNodeMetadata()
{
	delete m_inventory;
}
u16 LockingFurnaceNodeMetadata::typeId() const
{
	return CONTENT_LOCKABLE_FURNACE;
}
NodeMetadata* LockingFurnaceNodeMetadata::clone()
{
	LockingFurnaceNodeMetadata *d = new LockingFurnaceNodeMetadata();
	*d->m_inventory = *m_inventory;
	return d;
}
NodeMetadata* LockingFurnaceNodeMetadata::create(std::istream &is)
{
	LockingFurnaceNodeMetadata *d = new LockingFurnaceNodeMetadata();

	d->m_inventory->deSerialize(is);
	d->setOwner(deSerializeString(is));
	d->setInventoryOwner(deSerializeString(is));

	int temp;
	is>>temp;
	d->m_fuel_totaltime = (float)temp/10;
	is>>temp;
	d->m_fuel_time = (float)temp/10;
	is>>temp;
	d->m_lock = (float)temp/10;

	return d;
}
void LockingFurnaceNodeMetadata::serializeBody(std::ostream &os)
{
	m_inventory->serialize(os);
	os<<serializeString(m_owner);
	os<<serializeString(m_inv_owner);
	os<<itos(m_fuel_totaltime*10)<<" ";
	os<<itos(m_fuel_time*10)<<" ";
	os<<itos(m_lock*10)<<" ";
}
std::string LockingFurnaceNodeMetadata::infoText()
{
	//return "Furnace";
	std::string ostr = m_owner;
	if (m_inv_owner != "")
		ostr += ","+m_inv_owner;
	if(m_fuel_time >= m_fuel_totaltime)
	{
		const InventoryList *src_list = m_inventory->getList("src");
		assert(src_list);
		const InventoryItem *src_item = src_list->getItem(0);

		if(src_item && src_item->isCookable()) {
			InventoryList *dst_list = m_inventory->getList("dst");
			if(!dst_list->roomForCookedItem(src_item))
				return std::string("Locking Furnace (")+ostr+") is overloaded";
			return std::string("Locking Furnace (")+ostr+") is out of fuel";
		}
		else
			return std::string("Locking Furnace (")+ostr+") is inactive";
	}
	else
	{
		std::string s = std::string("Locking Furnace (")+ostr+") is active";
		// Do this so it doesn't always show (0%) for weak fuel
		if(m_fuel_totaltime > 3) {
			s += " (";
			s += itos(m_fuel_time/m_fuel_totaltime*100);
			s += "%)";
		}
		return s;
	}
}
bool LockingFurnaceNodeMetadata::nodeRemovalDisabled()
{
	/*
		Disable removal if furnace is not empty
	*/
	InventoryList *list[3] = {m_inventory->getList("src"),
	m_inventory->getList("dst"), m_inventory->getList("fuel")};

	for(int i = 0; i < 3; i++) {
		if(list[i] == NULL)
			continue;
		if(list[i]->getUsedSlots() == 0)
			continue;
		return true;
	}
	return false;

}
void LockingFurnaceNodeMetadata::inventoryModified()
{
	infostream<<"LockingFurnace inventory modification callback"<<std::endl;
}
bool LockingFurnaceNodeMetadata::step(float dtime, v3s16 pos, ServerEnvironment *env)
{
	if(dtime > 60.0)
		infostream<<"LockingFurnace stepping a long time ("<<dtime<<")"<<std::endl;
	// Update at a fixed frequency
	const float interval = 2.0;
	m_step_accumulator += dtime;
	bool changed = false;
	while(m_step_accumulator > interval)
	{
		m_step_accumulator -= interval;
		dtime = interval;

		//infostream<<"Furnace step dtime="<<dtime<<std::endl;

		InventoryList *dst_list = m_inventory->getList("dst");
		assert(dst_list);

		InventoryList *src_list = m_inventory->getList("src");
		assert(src_list);
		InventoryItem *src_item = src_list->getItem(0);

		bool room_available = false;

		if (src_item && src_item->isCookable()) {
			room_available = dst_list->roomForCookedItem(src_item);
			m_lock = 300.0;
			changed = true;
		}else if (m_lock < 0.0) {
			setInventoryOwner("");
			changed = true;
		}else{
			m_lock -= dtime;
			changed = true;
		}

		// Start only if there are free slots in dst, so that it can
		// accomodate any result item
		if (room_available) {
			m_src_totaltime = 3;
		}else{
			m_src_time = 0;
			m_src_totaltime = 0;
		}

		/*
			If fuel is burning, increment the burn counters.
			If item finishes cooking, move it to result.
		*/
		if (m_fuel_time < m_fuel_totaltime) {
			//infostream<<"Furnace is active"<<std::endl;
			m_fuel_time += dtime;
			m_src_time += dtime;
			if (m_src_time >= m_src_totaltime && m_src_totaltime > 0.001 && src_item) {
				InventoryItem *cookresult = src_item->createCookResult();
				dst_list->addItem(cookresult);
				src_list->decrementMaterials(1);
				m_src_time = 0;
				m_src_totaltime = 0;
			}
			changed = true;

			// If the fuel was not used up this step, just keep burning it
			if (m_fuel_time < m_fuel_totaltime)
				continue;
		}

		/*
			Get the source again in case it has all burned
		*/
		src_item = src_list->getItem(0);

		/*
			If there is no source item, or the source item is not cookable,
			or the furnace is still cooking, or the furnace became overloaded, stop loop.
		*/
		if(src_item == NULL || !room_available || m_fuel_time < m_fuel_totaltime ||
			dst_list->roomForCookedItem(src_item) == false)
		{
			m_step_accumulator = 0;
			break;
		}

		//infostream<<"Furnace is out of fuel"<<std::endl;

		InventoryList *fuel_list = m_inventory->getList("fuel");
		assert(fuel_list);
		InventoryItem *fuel_item = fuel_list->getItem(0);
		if (fuel_item && fuel_item->isFuel()) {
			if ((fuel_item->getContent()&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
				m_fuel_totaltime = ((CraftItem*)fuel_item)->getFuelTime();
			}else if ((fuel_item->getContent()&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
				m_fuel_totaltime = ((ToolItem*)fuel_item)->getFuelTime();
			}else{
				m_fuel_totaltime = ((MaterialItem*)fuel_item)->getFuelTime();
			}
			m_fuel_time = 0;
			content_t c = fuel_item->getContent();
			fuel_list->decrementMaterials(1);
			if (c == CONTENT_TOOLITEM_STEELBUCKET_LAVA) {
				fuel_list->addItem(0,new ToolItem(CONTENT_TOOLITEM_STEELBUCKET,0));
			}
			changed = true;
		}else{
			m_step_accumulator = 0;
		}
	}
	return changed;
}
std::string LockingFurnaceNodeMetadata::getDrawSpecString()
{
	return
		"size[8,9]"
		"list[current_name;fuel;2,3;1,1;]"
		"list[current_name;src;2,1;1,1;]"
		"list[current_name;dst;5,1;2,2;]"
		"list[current_player;main;0,5;8,4;]";
}

/*
	LockedDoorNodeMetadata
*/

// Prototype
TNTNodeMetadata proto_TNTNodeMetadata;

TNTNodeMetadata::TNTNodeMetadata():
	m_armed(false),
	m_time(0)
{
	NodeMetadata::registerType(typeId(), create);
}
TNTNodeMetadata::~TNTNodeMetadata()
{
}
u16 TNTNodeMetadata::typeId() const
{
	return CONTENT_TNT;
}
NodeMetadata* TNTNodeMetadata::create(std::istream &is)
{
	TNTNodeMetadata *d = new TNTNodeMetadata();
	int temp;
	is>>temp;
	d->m_time = (float)temp/10;
	is>>temp;
	d->m_armed = (bool)temp;
	return d;
}
NodeMetadata* TNTNodeMetadata::clone()
{
	TNTNodeMetadata *d = new TNTNodeMetadata();
	return d;
}
bool TNTNodeMetadata::step(float dtime, v3s16 pos, ServerEnvironment *env)
{
	if (!m_armed)
		return false;
	m_time -= dtime;
	if (m_time < 0.0)
		m_time = 0.0;
	return true;
}
void TNTNodeMetadata::serializeBody(std::ostream &os)
{
	os<<itos(m_time*10) << " ";
	os<<itos(m_armed) << " ";
}
std::string TNTNodeMetadata::infoText()
{
	if (!m_armed)
		return std::string("");

	int s = (int)ceil(m_time);
	if (s < 1)
		return std::string("Armed Explosive: about to detonate");

	return std::string("Armed Explosive: ")+itos(s)+" seconds till detonation";
}

/*
	IncineratorNodeMetadata
*/

// Prototype
IncineratorNodeMetadata proto_IncineratorNodeMetadata;

IncineratorNodeMetadata::IncineratorNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_inventory = new Inventory();
	m_inventory->addList("fuel", 1);
}
IncineratorNodeMetadata::~IncineratorNodeMetadata()
{
	delete m_inventory;
}
u16 IncineratorNodeMetadata::typeId() const
{
	return CONTENT_INCINERATOR;
}
NodeMetadata* IncineratorNodeMetadata::clone()
{
	IncineratorNodeMetadata *d = new IncineratorNodeMetadata();
	*d->m_inventory = *m_inventory;
	return d;
}
NodeMetadata* IncineratorNodeMetadata::create(std::istream &is)
{
	IncineratorNodeMetadata *d = new IncineratorNodeMetadata();

	d->m_inventory->deSerialize(is);

	return d;
}
void IncineratorNodeMetadata::serializeBody(std::ostream &os)
{
	m_inventory->serialize(os);
}
std::string IncineratorNodeMetadata::infoText()
{
	InventoryList *list = m_inventory->getList("fuel");
	InventoryItem *fitem;

	if (list && list->getUsedSlots() > 0 && (fitem = list->getItem(0)) != NULL && fitem->isFuel())
		return "Incinerator is active";
	return "Incinerator is inactive";
}
bool IncineratorNodeMetadata::nodeRemovalDisabled()
{
	/*
		Disable removal if not empty
	*/
	InventoryList *list = m_inventory->getList("fuel");

	if (list && list->getUsedSlots() > 0)
		return true;
	return false;

}
void IncineratorNodeMetadata::inventoryModified()
{
	infostream<<"Incinerator inventory modification callback"<<std::endl;
}
std::string IncineratorNodeMetadata::getDrawSpecString()
{
	return
		"size[8,7]"
		"label[1,0.5;Add fuel, then punch to incinerate wielded item]"
		"label[3.5,1.5;Fuel]"
		"list[current_name;fuel;4,1;1,1;]"
		"list[current_player;main;0,3;8,4;]";
}

/*
	CraftGuideNodeMetadata
*/

// Prototype
CraftGuideNodeMetadata proto_CraftGuideNodeMetadata;

CraftGuideNodeMetadata::CraftGuideNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_page = 0;

	m_inventory = new Inventory();
	m_inventory->addList("list", 300);
	m_inventory->addList("recipe", 9);
	m_inventory->addList("result", 1);
}
CraftGuideNodeMetadata::~CraftGuideNodeMetadata()
{
	delete m_inventory;
}
u16 CraftGuideNodeMetadata::typeId() const
{
	return CONTENT_CRAFT_BOOK_OPEN;
}
NodeMetadata* CraftGuideNodeMetadata::clone()
{
	CraftGuideNodeMetadata *d = new CraftGuideNodeMetadata();
	*d->m_inventory = *m_inventory;
	d->m_page = m_page;
	InventoryList *l = d->m_inventory->getList("list");
	InventoryItem *t;
	content_t *r;
	l->clearItems();
	std::vector<content_t> &list = lists::get("craftguide");
	u16 start = m_page*40;
	u16 end = start+40;
	if (end > list.size())
		end = list.size();
	for (int i=start; i<end; i++) {
		if ((list[i]&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
			t = new CraftItem(list[i],1);
		}else if ((list[i]&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
			t = new ToolItem(list[i],1);
		}else{
			t = new MaterialItem(list[i],1);
		}
		r = crafting::getRecipe(t);
		if (!r) {
			delete t;
			continue;
		}
		l->addItem(t);
	}
	return d;
}
NodeMetadata* CraftGuideNodeMetadata::create(std::istream &is)
{
	CraftGuideNodeMetadata *d = new CraftGuideNodeMetadata();

	d->m_inventory->deSerialize(is);
	is>>d->m_page;

	return d;
}
void CraftGuideNodeMetadata::serializeBody(std::ostream &os)
{
	m_inventory->serialize(os);
	os<<itos(m_page) << " ";
}
bool CraftGuideNodeMetadata::nodeRemovalDisabled()
{
	return false;
}
void CraftGuideNodeMetadata::inventoryModified()
{
	infostream<<"CraftGuide inventory modification callback"<<std::endl;
}
bool CraftGuideNodeMetadata::step(float dtime, v3s16 pos, ServerEnvironment *env)
{
	InventoryList *l = m_inventory->getList("result");
	InventoryItem *t = l->getItem(0);
	if (!t || t->getContent() == CONTENT_IGNORE)
		return false;
	content_t *r = crafting::getRecipe(t);
	if (!r)
		return false;
	l = m_inventory->getList("recipe");
	l->clearItems();
	for (int i=0; i<9; i++) {
		if (r[i] == CONTENT_IGNORE)
			continue;
		if ((r[i]&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
			t = new CraftItem(r[i],1);
		}else if ((r[i]&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
			t = new ToolItem(r[i],1);
		}else{
			t = new MaterialItem(r[i],1);
		}
		l->addItem(i,t);
	}

	delete[] r;

	return true;
}
bool CraftGuideNodeMetadata::import(NodeMetadata *meta)
{
	if (meta->typeId() == CONTENT_BOOK)
		m_page = ((ClosedBookNodeMetadata*)meta)->getPage();

	if (m_page < 0)
		m_page = 0;
	std::vector<content_t> &list = lists::get("craftguide");
	if (m_page > (list.size()/40))
		m_page = list.size()/40;
	InventoryList *l = m_inventory->getList("list");
	InventoryItem *t;
	content_t *r;
	l->clearItems();
	u16 start = m_page*40;
	u16 end = start+40;
	if (end > list.size())
		end = list.size();
	for (int i=start; i<end; i++) {
		if ((list[i]&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
			t = new CraftItem(list[i],1);
		}else if ((list[i]&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
			t = new ToolItem(list[i],1);
		}else{
			t = new MaterialItem(list[i],1);
		}
		r = crafting::getRecipe(t);
		if (!r) {
			delete t;
			continue;
		}
		l->addItem(t);
	}
	return true;
}
bool CraftGuideNodeMetadata::receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player)
{
	if (fields["prev"] == "" && fields["next"] == "")
		return false;
	if (fields["prev"] != "")
		m_page--;
	if (fields["next"] != "")
		m_page++;
	if (m_page < 0)
		m_page = 0;
	std::vector<content_t> &list = lists::get("craftguide");
	if (m_page > (list.size()/40))
		m_page = list.size()/40;
	InventoryList *l = m_inventory->getList("list");
	InventoryItem *t;
	content_t *r;
	l->clearItems();
	u16 start = m_page*40;
	u16 end = start+40;
	if (end > list.size())
		end = list.size();
	for (int i=start; i<end; i++) {
		if ((list[i]&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK) {
			t = new CraftItem(list[i],1);
		}else if ((list[i]&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK) {
			t = new ToolItem(list[i],1);
		}else{
			t = new MaterialItem(list[i],1);
		}
		r = crafting::getRecipe(t);
		if (!r) {
			delete t;
			continue;
		}
		l->addItem(t);
	}
	return true;
}
std::string CraftGuideNodeMetadata::getDrawSpecString()
{
	InventoryList *l = m_inventory->getList("result");
	InventoryItem *q = l->getItem(0);
	int tr = 0;
	std::vector<content_t> &list = lists::get("craftguide");
	if (q && q->getContent() != CONTENT_IGNORE)
		tr = crafting::getResultCount(q);

	std::string spec("size[8,9]");
	spec +=	"label[0.5,0.75;Add item here to see recipe]";
	spec +=	"list[current_name;result;2,1;1,1;]";
	// this overflows into the craft grid... but could be cool
	//if (q && tr) {
		//spec += "label[0.5,2.5;Gives ";
		//spec += itos(tr);
		//spec += " ";
		//spec += q->getGuiName();
		//spec += "]";
	//}
	spec +=	"list[current_name;recipe;4,0;3,3;]";
	spec +=	"button[0.25,3.5;2.5,0.75;prev;<< Previous Page]";
	spec +=	"label[3.5,3.5;Page ";
	spec += itos(m_page+1);
	spec +=" of ";
	spec += itos((list.size()/40)+1);
	spec += "]";
	spec +=	"button[6,3.5;2.5,0.75;next;Next Page >>]";
	spec +=	"list[current_name;list;0,4;8,5;]";
	return spec;
}

/*
	BookNodeMetadata
*/

// Prototype
BookNodeMetadata proto_BookNodeMetadata;

BookNodeMetadata::BookNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_title = "Book";
	m_content = "";
}

u16 BookNodeMetadata::typeId() const
{
	return CONTENT_BOOK_OPEN;
}
NodeMetadata* BookNodeMetadata::clone()
{
	BookNodeMetadata *d = new BookNodeMetadata();
	d->m_title = m_title;
	d->m_content = m_content;
	return d;
}
NodeMetadata* BookNodeMetadata::create(std::istream &is)
{
	BookNodeMetadata *d = new BookNodeMetadata();
	d->m_title = deSerializeString(is);
	d->m_content = deSerializeString(is);
	return d;
}
void BookNodeMetadata::serializeBody(std::ostream &os)
{
	os<<serializeString(m_title);
	os<<serializeString(m_content);
}
bool BookNodeMetadata::nodeRemovalDisabled()
{
	if (m_content != "")
		return true;
	return false;
}
bool BookNodeMetadata::import(NodeMetadata *meta)
{
	if (meta->typeId() != CONTENT_BOOK)
		return false;

	ClosedBookNodeMetadata *m = (ClosedBookNodeMetadata*)meta;
	m_title = m->infoText();
	m_content = m->getContent();
	return true;
}
bool BookNodeMetadata::receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player)
{
	m_title = fields["title"];
	m_content = fields["content"];

	std::string::size_type pos = 0;
	while ((pos = m_content.find("]",pos)) != std::string::npos) {
		m_content.replace(pos,1,")");
		pos++;
	}
	pos = 0;
	while ((pos = m_content.find("[",pos)) != std::string::npos) {
		m_content.replace(pos,1,"(");
		pos++;
	}
	return true;
}

std::string BookNodeMetadata::getDrawSpecString()
{
	std::string spec("size[6,6]");
	spec += "field[1,1;5,1;title;Title;";
	spec += m_title;
	spec += "]";
	spec += "field[1,2;5,2;content;Content;";
	spec += m_content;
	spec += "]";
	spec += "button_exit[2,5;3,1;submit;Save]";
	return spec;
}

/*
	DiaryNodeMetadata
*/

// Prototype
DiaryNodeMetadata proto_DiaryNodeMetadata;

DiaryNodeMetadata::DiaryNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_title = "Diary";
	m_content = "";
}

u16 DiaryNodeMetadata::typeId() const
{
	return CONTENT_DIARY_BOOK_OPEN;
}
NodeMetadata* DiaryNodeMetadata::clone()
{
	DiaryNodeMetadata *d = new DiaryNodeMetadata();
	d->m_owner = m_owner;
	d->m_title = m_title;
	d->m_content = m_content;
	return d;
}
NodeMetadata* DiaryNodeMetadata::create(std::istream &is)
{
	DiaryNodeMetadata *d = new DiaryNodeMetadata();
	d->m_owner = deSerializeString(is);
	d->m_title = deSerializeString(is);
	d->m_content = deSerializeString(is);
	return d;
}
void DiaryNodeMetadata::serializeBody(std::ostream &os)
{
	os<<serializeString(m_owner);
	os<<serializeString(m_title);
	os<<serializeString(m_content);
}
bool DiaryNodeMetadata::nodeRemovalDisabled()
{
	if (m_content != "")
		return true;
	return false;
}
bool DiaryNodeMetadata::import(NodeMetadata *meta)
{
	if (meta->typeId() != CONTENT_BOOK)
		return false;

	ClosedBookNodeMetadata *m = (ClosedBookNodeMetadata*)meta;
	m_owner = m->getOwner();
	m_title = m->infoText();
	m_content = m->getContent();
	return true;
}
bool DiaryNodeMetadata::receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player)
{
	if (m_owner != "" && player->getName() != m_owner)
		return false;
	m_title = fields["title"];
	m_content = fields["content"];

	std::string::size_type pos = 0;
	while ((pos = m_content.find("]",pos)) != std::string::npos) {
		m_content.replace(pos,1,")");
		pos++;
	}
	pos = 0;
	while ((pos = m_content.find("[",pos)) != std::string::npos) {
		m_content.replace(pos,1,"(");
		pos++;
	}
	return true;
}

std::string DiaryNodeMetadata::getDrawSpecString()
{
	std::string spec("size[6,6]");
	spec += "field[1,1;5,1;title;Title;";
	spec += m_title;
	spec += "]";
	spec += "field[1,2;5,2;content;Content;";
	spec += m_content;
	spec += "]";
	spec += "button_exit[2,5;3,1;submit;Save]";
	return spec;
}

/*
	ClosedBookNodeMetadata
*/

// Prototype
ClosedBookNodeMetadata proto_ClosedBookNodeMetadata;

ClosedBookNodeMetadata::ClosedBookNodeMetadata()
{
	NodeMetadata::registerType(typeId(), create);

	m_page = 0;
	m_owner = "";
	m_title = "";
	m_content = "";
}
u16 ClosedBookNodeMetadata::typeId() const
{
	return CONTENT_BOOK;
}
NodeMetadata* ClosedBookNodeMetadata::clone()
{
	ClosedBookNodeMetadata *d = new ClosedBookNodeMetadata();
	d->m_page = m_page;
	d->m_owner = m_owner;
	d->m_title = m_title;
	d->m_content = m_content;
	return d;
}
NodeMetadata* ClosedBookNodeMetadata::create(std::istream &is)
{
	ClosedBookNodeMetadata *d = new ClosedBookNodeMetadata();

	d->m_owner = deSerializeString(is);
	d->m_title = deSerializeString(is);
	d->m_content = deSerializeString(is);
	is>>d->m_page;

	return d;
}
void ClosedBookNodeMetadata::serializeBody(std::ostream &os)
{
	os<<serializeString(m_owner);
	os<<serializeString(m_title);
	os<<serializeString(m_content);
	os<<itos(m_page) << " ";
}
bool ClosedBookNodeMetadata::nodeRemovalDisabled()
{
	return false;
}
bool ClosedBookNodeMetadata::import(NodeMetadata *meta)
{
	switch (meta->typeId()) {
	case CONTENT_CRAFT_BOOK_OPEN:
	{
		CraftGuideNodeMetadata *cm = (CraftGuideNodeMetadata*)meta;
		m_page = cm->getPage();
		m_title = cm->infoText();
		break;
	}
	case CONTENT_BOOK_OPEN:
	{
		BookNodeMetadata *bm = (BookNodeMetadata*)meta;
		m_title = bm->infoText();
		m_content = bm->getContent();
		break;
	}
	case CONTENT_DIARY_BOOK_OPEN:
	{
		DiaryNodeMetadata *dm = (DiaryNodeMetadata*)meta;
		m_owner = dm->getOwner();
		m_title = dm->infoText();
		m_content = dm->getContent();
		break;
	}
	default:;
	}
	return false;
}
