/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* nodemetadata.h
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2014 <lisa@ltmnet.com>
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

#ifndef NODEMETADATA_HEADER
#define NODEMETADATA_HEADER

#include "common_irrlicht.h"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "mapnode.h"

class Player;

#define ENERGY_MAX 16

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
class ServerEnvironment;

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
	virtual std::wstring infoText() {return L"";}
	virtual std::string getText() {return "";}
	virtual Inventory* getInventory() {return NULL;}
	// This is called always after the inventory is modified, before
	// the changes are copied elsewhere
	virtual void inventoryModified(){}
	// A step in time. Returns true if metadata changed.
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env) {return false;}
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env) {return false;}
	virtual bool nodeRemovalDisabled(){return false;}
	// Used to make custom inventory menus.
	// See format in guiInventoryMenu.cpp.
	virtual std::string getDrawSpecString(Player *player){return "";}
	// the node owner - if not "" then only the owner can dig the node
	virtual std::string getOwner(){ return std::string(""); }
	virtual void setOwner(std::string t){  }
	// the inventory owner - if not "" then only the owner can modify
	virtual std::string getInventoryOwner(){ return std::string(""); }
	virtual void setInventoryOwner(std::string t){  }
	// receive data from the client. Returns true if metadata changed.
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player) {return false;}
	// import data from another nodemetadata. Returns true if metadata changed.
	virtual bool import(NodeMetadata *meta) {return false;}
	// get nodeboxes for CDT_NODEBOX_META
	virtual std::vector<NodeBox> getNodeBoxes(MapNode &n) {return std::vector<NodeBox>();}
	// used by tnt to arm it, but also for future circuitry
	// level is the amount of power
	// powersrc is the generator or such that created the power
	// signalsrc is the node that sent this node the energise signal
	// pos is the position of this node
	// returns false if propogation was stopped
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos) {return false;}
	virtual u8 getEnergy() {return 0;}
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
	NodeMetadataList();
	~NodeMetadataList();

	void serialize(std::ostream &os);
	void deSerialize(std::istream &is);

	// Get pointer to data
	NodeMetadata *get(v3s16 p);
	NodeMetadata *getClone(v3s16 p);
	// Deletes data
	void remove(v3s16 p);
	// Deletes old data and sets a new one
	void set(v3s16 p, NodeMetadata *d);

	// A step in time. Returns true if something changed.
	bool step(float dtime, v3s16 blockpos_nodes, ServerEnvironment *env);
	bool stepCircuit(float dtime, v3s16 blockpos_nodes, ServerEnvironment *env);

private:
	core::map<v3s16, NodeMetadata*> m_data;
	JMutex m_mutex;
};

#endif

