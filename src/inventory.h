/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* inventory.h
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


#ifndef INVENTORY_HEADER
#define INVENTORY_HEADER

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "common_irrlicht.h"
#include "debug.h"
#include "main.h" // For g_materials
#include "mapnode.h" // For content_t
// for mapping to old subname
#include "content_craftitem.h"
#include "content_toolitem.h"
#include "content_clothesitem.h"
#include "content_mapnode.h"

#define QUANTITY_ITEM_MAX_COUNT 99

class ServerActiveObject;
class ServerEnvironment;
class Player;

class InventoryItem
{
public:
	InventoryItem(u16 count, u16 data);
	virtual ~InventoryItem();

	static content_t info(std::istream &is, u16 *count, u16 *wear, u16 *data);
	static InventoryItem* deSerialize(std::istream &is);
	static InventoryItem* create(content_t c, u16 count, u16 wear=0, u16 data=0);

	virtual const char* getName() const = 0;
	// Shall write the name and the parameters
	virtual void serialize(std::ostream &os) const = 0;
	// Shall make an exact clone of the item
	virtual InventoryItem* clone() = 0;
#ifndef SERVER
	// Return the name of the image for this item
	virtual std::string getBasename() const { return ""; }
	// Shall return an image of the item (or NULL)
	virtual video::ITexture * getImage() const { return NULL; }
	// Shall return an image of the item without embellishments (or NULL)
	virtual video::ITexture * getImageRaw() const { return getImage(); }
#endif
	// get the content type
	content_t getContent() {return m_content;}
	// this is used for tool tips
	virtual std::wstring getGuiName() { return L""; }
	// this is used for hover data / extended tool tips
	virtual std::wstring getGuiText() { return L""; }
	// Shall return a text to show in the GUI
	virtual std::string getText() { return ""; }
	// Returns the string used for inventory
	virtual std::string getItemString();
	// Creates an object from the item, to be placed in the world.
	virtual ServerActiveObject* createSAO(ServerEnvironment *env, u16 id, v3f pos);
	// Gets amount of items that dropping one SAO will decrement
	virtual u16 getDropCount() const { return getCount(); }

	/*
		Quantity methods
	*/

	// Shall return true if the item can be add()ed to the other
	virtual bool addableTo(const InventoryItem *other) const
	{
		return false;
	}

	u16 getCount() const
	{
		return m_count;
	}
	void setCount(u16 count)
	{
		m_count = count;
	}
	// This should return something else for stackable items
	virtual u16 freeSpace() const
	{
		return 0;
	}
	void add(u16 count)
	{
		if (m_count + count > QUANTITY_ITEM_MAX_COUNT)
			return;
		m_count += count;
	}
	void remove(u16 count)
	{
		if (m_count < count) {
			m_count = 0;
		}else{
			m_count -= count;
		}
	}
	virtual void setWear(u16 wear) {}
	virtual u16 getWear() {return 0;}

	void setData(u16 data) { m_data = data; }
	void addData(u16 data) { m_data |= data; }
	u16 getData() { return m_data; }

	/*
		Other properties
	*/

	// Whether it can be cooked
	virtual bool isCookable() const {return false;}
	// Time of cooking
	virtual float getCookTime() const {return 3.0;}
	// Result of cooking (can randomize)
	virtual InventoryItem *createCookResult() const {return NULL;}
	// Whether it can be used as fuel
	virtual bool isFuel() const {return false;}
	// the fuel time value
	virtual float getFuelTime() const {return 0.0;}

	// Eat, press, activate, whatever.
	// Called when item is right-clicked when lying on ground.
	// If returns true, item shall be deleted.
	virtual bool use(ServerEnvironment *env,
			Player *player){return false;}

protected:
	u16 m_count;
	content_t m_content;
	u16 m_data;
};

class MaterialItem : public InventoryItem
{
public:
	MaterialItem(content_t content, u16 count, u16 data):
		InventoryItem(count,data)
	{
		MapNode n(content);
		n = mapnode_translate_to_internal(n,SER_FMT_VER_HIGHEST);
		m_content = n.getContent();
	}
	/*
		Implementation interface
	*/
	virtual const char* getName() const
	{
		return "MaterialItem";
	}
	virtual void serialize(std::ostream &os) const
	{
		//os.imbue(std::locale("C"));
		os<<"MaterialItem3";
		os<<" ";
		os<<(unsigned int)m_content;
		os<<" ";
		os<<m_count;
		os<<" ";
		os<<m_data;
	}
	virtual InventoryItem* clone()
	{
		return new MaterialItem(m_content, m_count, m_data);
	}
#ifndef SERVER
	video::ITexture * getImage() const;
#endif
	std::wstring getGuiName()
	{
		return content_features(m_content).description;
	}

	std::wstring getGuiText();

	std::string getText()
	{
		std::ostringstream os;
		os<<m_count;
		return os.str();
	}

	virtual bool addableTo(const InventoryItem *other) const
	{
		content_t c = ((InventoryItem*)other)->getContent();
		u16 d = ((InventoryItem*)other)->getData();
		if (c != m_content)
			return false;
		if (m_data != d)
			return false;
		return true;
	}
	u16 freeSpace() const
	{
		if (m_count > QUANTITY_ITEM_MAX_COUNT)
			return 0;
		return QUANTITY_ITEM_MAX_COUNT - m_count;
	}
	/*
		Other properties
	*/
	bool isCookable() const;
	InventoryItem *createCookResult() const;
	virtual bool isFuel() const;
	virtual float getFuelTime() const;

	bool use(ServerEnvironment *env, Player *player);

	/*
		Special methods
	*/
	content_t getMaterial()
	{
		return m_content;
	}
private:
};

/*
	An item that is used as a mid-product when crafting.
	Subnames:
	- Stick
*/
class CraftItem : public InventoryItem
{
public:
	CraftItem(std::string subname, u16 count, u16 data):
		InventoryItem(count,data)
	{
		m_subname = content_craftitem_features(subname).name;
		m_content = content_craftitem_features(subname).content;
	}
	CraftItem(content_t content, u16 count, u16 data):
		InventoryItem(count,data)
	{
		m_subname = content_craftitem_features(content).name;
		m_content = content_craftitem_features(content).content;
	}
	/*
		Implementation interface
	*/
	virtual const char* getName() const
	{
		return "CraftItem";
	}
	virtual void serialize(std::ostream &os) const
	{
		os<<"CraftItem3";
		os<<" ";
		os<<(unsigned int)m_content;
		os<<" ";
		os<<m_count;
		os<<" ";
		os<<m_data;
	}
	virtual InventoryItem* clone()
	{
		return new CraftItem(m_content, m_count, m_data);
	}
#ifndef SERVER
	video::ITexture * getImage() const;
#endif

	std::wstring getGuiName();
	std::wstring getGuiText();

	std::string getText()
	{
		std::ostringstream os;
		os<<m_count;
		return os.str();
	}

	ServerActiveObject* createSAO(ServerEnvironment *env, u16 id, v3f pos);
	u16 getDropCount() const;

	virtual bool addableTo(const InventoryItem *other) const
	{
		content_t c = ((InventoryItem*)other)->getContent();
		u16 d = ((InventoryItem*)other)->getData();
		if (c != m_content)
			return false;
		if (m_data != d)
			return false;
		return true;
	}
	u16 freeSpace() const
	{
		if (!content_craftitem_features(m_content).stackable)
			return 0;
		if (m_count > QUANTITY_ITEM_MAX_COUNT)
			return 0;
		return QUANTITY_ITEM_MAX_COUNT - m_count;
	}

	/*
		Other properties
	*/

	bool isCookable() const;
	InventoryItem *createCookResult() const;
	bool isFuel() const;
	float getFuelTime() const;

	bool use(ServerEnvironment *env, Player *player);

	/*
		Special methods
	*/
	std::string getSubName()
	{
		return m_subname;
	}
private:
	std::string m_subname;
};

class ToolItem : public InventoryItem
{
public:
	ToolItem(std::string toolname, u16 wear, u16 data):
		InventoryItem(1,data)
	{
		m_wear = wear;
		m_toolname = content_toolitem_features(toolname).name;
		m_content = content_toolitem_features(toolname).content;
	}
	ToolItem(content_t content, u16 wear, u16 data):
		InventoryItem(1,data)
	{
		m_wear = wear;
		m_toolname = content_toolitem_features(content).name;
		m_content = content_toolitem_features(content).content;
	}
	/*
		Implementation interface
	*/
	virtual const char* getName() const
	{
		return "ToolItem";
	}
	virtual void serialize(std::ostream &os) const
	{
		os<<"ToolItem3";
		os<<" ";
		os<<(unsigned int)m_content;
		os<<" ";
		os<<m_wear;
		os<<" ";
		os<<m_data;
	}
	virtual InventoryItem* clone()
	{
		return new ToolItem(m_content, m_wear, m_data);
	}
#ifndef SERVER
	std::string getBasename() const;

	video::ITexture * getImage() const;

	video::ITexture * getImageRaw() const
	{
		if(g_texturesource == NULL)
			return NULL;

		return g_texturesource->getTextureRaw(getBasename());
	}
#endif
	std::wstring getGuiName()
	{
		return content_toolitem_features(m_content).description;
	}
	std::wstring getGuiText();
	std::string getText()
	{
		return "";
	}

	ServerActiveObject* createSAO(ServerEnvironment *env, u16 id, v3f pos);

	bool isCookable() const;
	InventoryItem *createCookResult() const;
	bool isFuel() const;
	float getFuelTime() const;
	/*
		Special methods
	*/
	std::string getToolName()
	{
		return m_toolname;
	}
	virtual u16 getWear()
	{
		return m_wear;
	}
	// Returns true if weared out
	bool addWear(u16 add)
	{
		if(m_wear >= 65535 - add)
		{
			m_wear = 65535;
			return true;
		}
		else
		{
			m_wear += add;
			return false;
		}
	}
	virtual void setWear(u16 wear) {m_wear = wear;}
private:
	std::string m_toolname;
	u16 m_wear;
};

class ClothesItem : public InventoryItem
{
public:
	ClothesItem(content_t content, u16 wear, u16 data):
		InventoryItem(1,data)
	{
		m_wear = wear;
		m_content = content_clothesitem_features(content).content;
	}
	/*
		Implementation interface
	*/
	virtual const char* getName() const
	{
		return "ClothesItem";
	}
	virtual void serialize(std::ostream &os) const
	{
		os<<"ClothesItem2";
		os<<" ";
		os<<(unsigned int)m_content;
		os<<" ";
		os<<m_wear;
		os<<" ";
		os<<m_data;
	}
	virtual InventoryItem* clone()
	{
		return new ClothesItem(m_content, m_wear, m_data);
	}
#ifndef SERVER
	std::string getBasename() const {
		return content_clothesitem_features(m_content).texture;
	}

	video::ITexture * getImage() const;

	video::ITexture * getImageRaw() const
	{
		if (g_texturesource == NULL)
			return NULL;

		return g_texturesource->getTextureRaw(getBasename());
	}
#endif
	std::wstring getGuiName()
	{
		return content_clothesitem_features(m_content).description;
	}
	std::wstring getGuiText();
	std::string getText()
	{
		return "";
	}
	/*
		Special methods
	*/
	virtual u16 getWear()
	{
		return m_wear;
	}
	// Returns true if weared out
	bool addWear(u16 add)
	{
		if (m_wear >= 65535 - add) {
			m_wear = 65535;
			return true;
		}else{
			m_wear += add;
			return false;
		}
	}
	virtual void setWear(u16 wear) {m_wear = wear;}
private:
	u16 m_wear;
};

class InventoryDiffData
{
public:
	InventoryDiffData(u32 i, content_t t, u16 c, u16 d):
		index(i),
		type(t),
		wear_count(c),
		data(d)
	{
	}
	InventoryDiffData():
		index(0),
		type(CONTENT_IGNORE),
		wear_count(0),
		data(0)
	{
	}

	u32 index;
	content_t type;
	u16 wear_count;
	u16 data;
};

class InventoryDiff
{
public:
	InventoryDiff()
	{
		clear();
	}
	~InventoryDiff()
	{
		clear();
	}

	void clear()
	{
		m_data.clear();
	}

	void add(std::string list, u32 index, content_t type, u16 count_wear, u16 data)
	{
		m_data[list][index] = InventoryDiffData(index,type,count_wear,data);
	}

	void add(std::string list, u32 index, InventoryItem *item)
	{
		if (item == NULL) {
			add(list,index,CONTENT_IGNORE,0,0);
		}else if (
			(item->getContent()&CONTENT_TOOLITEM_MASK) == CONTENT_TOOLITEM_MASK
			|| (item->getContent()&CONTENT_CLOTHESITEM_MASK) == CONTENT_CLOTHESITEM_MASK
		) {
			add(list,index,item->getContent(),item->getWear(),item->getData());
		}else{
			add(list,index,item->getContent(),item->getCount(),item->getData());
		}
	}

	void merge(InventoryDiff &other)
	{
		for (std::map<std::string,std::map<u32,InventoryDiffData> >::iterator i = other.m_data.begin(); i != other.m_data.end(); i++) {
			m_data[i->first].swap(i->second);
			i->second.clear();
		}
	}

	std::map<std::string,std::map<u32,InventoryDiffData> > m_data;
};

class InventoryList
{
public:
	InventoryList(std::string name, u32 size);
	~InventoryList();
	void clearItems();
	void serialize(std::ostream &os) const;
	void deSerialize(std::istream &is);

	InventoryList(const InventoryList &other);
	InventoryList & operator = (const InventoryList &other);

	const std::string &getName() const;
	u32 getSize();
	// Count used slots
	u32 getUsedSlots();
	u32 getFreeSlots();

	// set specific nodes only allowed in inventory
	void addAllowed(content_t c) {m_allowed[c] = true;}
	void clearAllowed() {m_allowed.clear();}

	// set specific nodes not allowed in inventory
	void addDenied(content_t c) {m_denied[c] = true;}
	void clearDenied() {m_denied.clear();}

	// whether an item is allowed in inventory
	bool isAllowed(content_t c)
	{
		if (m_allowed.size() > 0)
			return m_allowed[c];
		return !m_denied[c];
	}
	bool isAllowed(InventoryItem *item) {return isAllowed(item->getContent());}

	// set whether items can be stacked (more than one per slot)
	void setStackable(bool s=true) {m_stackable = s;}
	bool getStackable() {return m_stackable;}

	/*bool getDirty(){ return m_dirty; }
	void setDirty(bool dirty=true){ m_dirty = dirty; }*/

	// Get pointer to item
	const InventoryItem * getItem(u32 i) const;
	InventoryItem * getItem(u32 i);
	// Returns old item (or NULL). Parameter can be NULL.
	InventoryItem * changeItem(u32 i, InventoryItem *newitem);
	// Delete item
	void deleteItem(u32 i);

	// Adds an item to a suitable place. Returns leftover item.
	// If all went into the list, returns NULL.
	InventoryItem * addItem(InventoryItem *newitem);

	// If possible, adds item to given slot.
	// If cannot be added at all, returns the item back.
	// If can be added partly, decremented item is returned back.
	// If can be added fully, NULL is returned.
	InventoryItem * addItem(u32 i, InventoryItem *newitem);

	// Updates item type/count/wear
	void updateItem(u32 i, content_t type, u16 wear_count, u16 data);

	// Checks whether the item could be added to the given slot
	bool itemFits(const u32 i, const InventoryItem *newitem);

	// Checks whether there is room for a given item
	bool roomForItem(const InventoryItem *item);

	// Checks whether there is room for a given item aftr it has been cooked
	bool roomForCookedItem(const InventoryItem *item);

	// Takes some items from a slot.
	// If there are not enough, takes as many as it can.
	// Returns NULL if couldn't take any.
	InventoryItem * takeItem(u32 i, u32 count);

	// find a stack containing an item
	InventoryItem *findItem(content_t item, u16 *item_i = NULL);

	// Decrements amount of every material item
	void decrementMaterials(u16 count);

	void print(std::ostream &o);

	void addDiff(u32 index, InventoryItem *item) {m_diff.add(m_name,index,item);}
	InventoryDiff &getDiff() {return m_diff;}

private:
	core::array<InventoryItem*> m_items;
	u32 m_size;
	std::string m_name;
	std::map<content_t,bool> m_allowed;
	std::map<content_t,bool> m_denied;
	bool m_stackable;
	InventoryDiff m_diff;
};

class Inventory
{
public:
	~Inventory();

	void clear();

	Inventory();
	Inventory(const Inventory &other);
	Inventory & operator = (const Inventory &other);

	void serialize(std::ostream &os) const;
	void deSerialize(std::istream &is);

	InventoryList * addList(const std::string &name, u32 size);
	InventoryList * getList(const std::string &name);
	const InventoryList * getList(const std::string &name) const;
	bool deleteList(const std::string &name);
	// A shorthand for adding items.
	// Returns NULL if the item was fully added, leftover otherwise.
	InventoryItem * addItem(const std::string &listname, InventoryItem *newitem)
	{
		InventoryList *list = getList(listname);
		if (list == NULL)
			return newitem;
		return list->addItem(newitem);
	}
	InventoryDiff &getDiff()
	{
		m_diff.clear();
		for (u32 i=0; i<m_lists.size(); i++) {
			InventoryDiff &diff = m_lists[i]->getDiff();
			m_diff.merge(diff);
		}
		return m_diff;
	}

private:
	// -1 if not found
	const s32 getListIndex(const std::string &name) const;

	core::array<InventoryList*> m_lists;
	InventoryDiff m_diff;
};

class Player;

struct InventoryContext
{
	Player *current_player;

	InventoryContext():
		current_player(NULL)
	{}
};

struct InventoryAction;
struct InventoryLocation;

class InventoryManager
{
public:
	InventoryManager(){}
	virtual ~InventoryManager(){}

	/*
		Get a pointer to an inventory specified by id.
		id can be:
		- "current_player"
		- "nodemeta:X,Y,Z"
	*/
	virtual Inventory* getInventory(InventoryContext *c, std::string id)
		{return NULL;}
	virtual Inventory* getInventory(const InventoryLocation *loc)
		{return NULL;}
	// Used on the server by InventoryAction::apply and other stuff
	virtual void inventoryModified(InventoryContext *c, std::string id)
		{}
	// Used on the client
	virtual void inventoryAction(InventoryAction *a)
		{}
};

#define IACTION_MOVE 0

struct InventoryAction
{
	virtual ~InventoryAction() {}
	static InventoryAction * deSerialize(std::istream &is);

	virtual u16 getType() const = 0;
	virtual void serialize(std::ostream &os) const = 0;
	virtual void apply(InventoryContext *c, InventoryManager *mgr) = 0;
};

struct IMoveAction : public InventoryAction
{
	// count=0 means "everything"
	u16 count;
	std::string from_inv;
	std::string from_list;
	s16 from_i;
	std::string to_inv;
	std::string to_list;
	s16 to_i;

	IMoveAction()
	{
		count = 0;
		from_i = -1;
		to_i = -1;
	}
	IMoveAction(std::istream &is)
	{
		std::string ts;

		std::getline(is, ts, ' ');
		count = mystoi(ts);

		std::getline(is, from_inv, ' ');

		std::getline(is, from_list, ' ');

		std::getline(is, ts, ' ');
		from_i = mystoi(ts);

		std::getline(is, to_inv, ' ');

		std::getline(is, to_list, ' ');

		std::getline(is, ts, ' ');
		to_i = mystoi(ts);
	}

	u16 getType() const
	{
		return IACTION_MOVE;
	}

	void serialize(std::ostream &os) const
	{
		os<<"Move ";
		os<<count<<" ";
		os<<from_inv<<" ";
		os<<from_list<<" ";
		os<<from_i<<" ";
		os<<to_inv<<" ";
		os<<to_list<<" ";
		os<<to_i;
	}

	void apply(InventoryContext *c, InventoryManager *mgr);
};

struct InventoryLocation
{
	enum Type{
		UNDEFINED,
		CURRENT_PLAYER,
		PLAYER,
		NODEMETA,
	} type;

	std::string name; // PLAYER
	v3s16 p; // NODEMETA

	InventoryLocation()
	{
		setUndefined();
	}
	void setUndefined()
	{
		type = UNDEFINED;
	}
	void setCurrentPlayer()
	{
		type = CURRENT_PLAYER;
	}
	void setPlayer(const std::string &name_)
	{
		type = PLAYER;
		name = name_;
	}
	void setNodeMeta(v3s16 p_)
	{
		type = NODEMETA;
		p = p_;
	}

	void applyCurrentPlayer(const std::string &name_)
	{
		if (type == CURRENT_PLAYER)
			setPlayer(name_);
	}
	std::string getName()
	{
		std::string l_name("");
		if (type == PLAYER) {
			l_name += "player:";
			l_name += name;
		}else if (type == NODEMETA) {
			l_name += "nodemeta:";
			l_name += itos(p.X);
			l_name += ",";
			l_name += itos(p.Y);
			l_name += ",";
			l_name += itos(p.Z);
		}else if (type == CURRENT_PLAYER) {
			l_name += "current_player";
		}
		return l_name;
	}

	std::string dump() const;
	void serialize(std::ostream &os) const;
	void deSerialize(std::istream &is);
	void deSerialize(std::string s);
};

#endif

