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

#ifndef NODEMETADATA_HEADER
#define NODEMETADATA_HEADER

#include "common_irrlicht.h"
#include <string>
#include <iostream>
#include <map>

#define ENERGY_MAX 16
#define ENERGY_MIN 0

/*
	Used for storing:

	Oven:
		- Item that is being burned
		- Burning time
		- Item stack that is being heated
		- Result item stack

	Sign:
		- Text
*/

class Inventory;

class NodeMetadata
{
public:
	typedef NodeMetadata* (*Factory)(std::istream&);

	NodeMetadata();
	virtual ~NodeMetadata();

	static NodeMetadata* deSerialize(std::istream &is);
	void serialize(std::ostream &os);

	// This usually is the CONTENT_ value
	virtual u16 typeId() const = 0;
	virtual NodeMetadata* clone() = 0;
	virtual void serializeBody(std::ostream &os) = 0;
	virtual std::string infoText() {return "";}
	virtual Inventory* getInventory() {return NULL;}
	// This is called always after the inventory is modified, before
	// the changes are copied elsewhere
	virtual void inventoryModified(){}
	// A step in time. Returns true if metadata changed.
	virtual bool step(float dtime) {return false;}
	virtual bool nodeRemovalDisabled(){return false;}
	// Used to make custom inventory menus.
	// See format in guiInventoryMenu.cpp.
	virtual std::string getInventoryDrawSpecString(){return "";}
	// primarily used for locking chests, but others can play too
	virtual std::string getOwner(){ return std::string(""); }
	virtual void setOwner(std::string t){  }
	// used by tnt to arm it, but also for future circuitry
	// level is the amount of power
	// powersrc is the generator or such that created the power
	// signalsrc is the node that sent this node the energise signal
	// pos is the position of this node
	// returns false if propogation was stopped
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos) {return false;}
	bool energise(u8 level, v3f powersrc, v3f signalsrc, v3s16 pos)
	{
		v3s16 p;
		v3s16 s;
		p.X = powersrc.X;
		p.Y = powersrc.Y;
		p.Z = powersrc.Z;
		s.X = signalsrc.X;
		s.Y = signalsrc.Y;
		s.Z = signalsrc.Z;
		return energise(level,p,s,pos);
	}
	virtual u8 getEnergy() {return ENERGY_MIN;}
	virtual std::map<v3s16, u8> *getSources() {return NULL;}

protected:
	static void registerType(u16 id, Factory f);
private:
	static core::map<u16, Factory> m_types;
};

/*
	List of metadata of all the nodes of a block
*/

class NodeMetadataList
{
public:
	~NodeMetadataList();

	void serialize(std::ostream &os);
	void deSerialize(std::istream &is);

	// Get pointer to data
	NodeMetadata* get(v3s16 p);
	// Deletes data
	void remove(v3s16 p);
	// Deletes old data and sets a new one
	void set(v3s16 p, NodeMetadata *d);

	// A step in time. Returns true if something changed.
	bool step(float dtime);

private:
	core::map<v3s16, NodeMetadata*> m_data;
};

#endif

