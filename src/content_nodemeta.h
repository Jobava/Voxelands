/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_nodemeta.h
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

#ifndef CONTENT_NODEMETA_HEADER
#define CONTENT_NODEMETA_HEADER

#include "nodemetadata.h"
#include "intl.h"

class ServerEnvironment;
class Inventory;

class SignNodeMetadata : public NodeMetadata
{
public:
	SignNodeMetadata(std::string text);
	//~SignNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);

	virtual std::string getText(){ return m_text; }
	void setText(std::string t){ m_text = t; }
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player)
	{
		m_text = fields["text"];
		return true;
	}
	virtual std::string getDrawSpecString(Player *player);

	virtual bool import(NodeMetadata *meta);

private:
	std::string m_text;
};

class LockingSignNodeMetadata : public NodeMetadata
{
public:
	LockingSignNodeMetadata(std::string text);
	//~LockingSignNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();

	virtual std::string getOwner(){ return m_owner; }
	virtual void setOwner(std::string t){ m_owner = t; }
	virtual std::string getInventoryOwner(){ return m_owner; }
	virtual void setInventoryOwner(std::string t){ m_owner = t; }

	virtual std::string getText(){ return m_text; }
	void setText(std::string t){ m_text = t; }
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

	virtual bool import(NodeMetadata *meta);

private:
	std::string m_text;
	std::string m_owner;
};

class FlagNodeMetadata : public NodeMetadata
{
public:
	FlagNodeMetadata();
	//~FlagNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();

	virtual std::string getOwner(){ return m_owner; }
	virtual void setOwner(std::string t){ m_owner = t; }

private:
	std::string m_owner;
};

class BedNodeMetadata : public NodeMetadata
{
public:
	BedNodeMetadata();
	//~BedNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);

	virtual bool nodeRemovalDisabled();

	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

private:
	std::string m_owner;
	std::string m_sleeper;
	bool m_nope;
};

class ChestNodeMetadata : public NodeMetadata
{
public:
	ChestNodeMetadata();
	~ChestNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual std::string getDrawSpecString(Player *player);

	virtual bool import(NodeMetadata *meta);

private:
	Inventory *m_inventory;
};

class LockingChestNodeMetadata : public NodeMetadata
{
public:
	LockingChestNodeMetadata();
	~LockingChestNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual std::string getDrawSpecString(Player *player);

	virtual bool import(NodeMetadata *meta);

	virtual std::string getOwner(){ return m_owner; }
	virtual void setOwner(std::string t){ m_owner = t; }
	virtual std::string getInventoryOwner(){ return m_owner; }
	virtual void setInventoryOwner(std::string t){ m_owner = t; }

private:
	Inventory *m_inventory;
	std::string m_owner;
};

class SafeNodeMetadata : public NodeMetadata
{
public:
	SafeNodeMetadata();
	~SafeNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual std::string getDrawSpecString(Player *player);

	virtual bool import(NodeMetadata *meta);

	virtual std::string getOwner(){ return m_owner; }
	virtual void setOwner(std::string t){ m_owner = t; }
	virtual std::string getInventoryOwner(){ return m_owner; }
	virtual void setInventoryOwner(std::string t){ m_owner = t; }

private:
	Inventory *m_inventory;
	std::string m_owner;
};

class CreativeChestNodeMetadata : public NodeMetadata
{
public:
	CreativeChestNodeMetadata();
	~CreativeChestNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

private:
	u16 m_page;
	Inventory *m_inventory;
};

class ParcelNodeMetadata : public NodeMetadata
{
public:
	ParcelNodeMetadata();
	~ParcelNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual Inventory* getInventory() {return m_inventory;}
	virtual std::string getDrawSpecString(Player *player);

private:
	Inventory *m_inventory;
};

class BorderStoneNodeMetadata : public NodeMetadata
{
public:
	BorderStoneNodeMetadata();
	~BorderStoneNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();

	virtual std::string getOwner(){ return m_text; }
	virtual void setOwner(std::string t){ m_text = t; }

private:
	std::string m_text;
};

class FurnaceNodeMetadata : public NodeMetadata
{
public:
	FurnaceNodeMetadata();
	~FurnaceNodeMetadata();

	virtual u16 typeId() const;
	virtual NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool nodeRemovalDisabled();
	virtual std::string getDrawSpecString(Player *player);
	virtual std::vector<NodeBox> getNodeBoxes(MapNode &n);

	virtual bool import(NodeMetadata *meta);

private:
	Inventory *m_inventory;
	float m_step_accumulator;
	float m_fuel_totaltime;
	float m_fuel_time;
	float m_src_totaltime;
	float m_src_time;
};

class LockingFurnaceNodeMetadata : public NodeMetadata
{
public:
	LockingFurnaceNodeMetadata();
	~LockingFurnaceNodeMetadata();

	virtual u16 typeId() const;
	virtual NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool nodeRemovalDisabled();
	virtual std::string getDrawSpecString(Player *player);
	virtual std::vector<NodeBox> getNodeBoxes(MapNode &n);

	virtual bool import(NodeMetadata *meta);

	virtual std::string getOwner(){ return m_owner; }
	virtual void setOwner(std::string t){ m_owner = t; }
	virtual std::string getInventoryOwner(){ return m_inv_owner; }
	virtual void setInventoryOwner(std::string t){ m_inv_owner = t; }

private:
	Inventory *m_inventory;
	float m_step_accumulator;
	float m_fuel_totaltime;
	float m_fuel_time;
	float m_src_totaltime;
	float m_src_time;
	std::string m_owner;
	std::string m_inv_owner;
	float m_lock;
};

class TNTNodeMetadata : public NodeMetadata
{
public:
	TNTNodeMetadata();
	~TNTNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);

	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
	virtual u8 getEnergy();

private:
	bool m_armed;
	float m_time;
};

class IncineratorNodeMetadata : public NodeMetadata
{
public:
	IncineratorNodeMetadata();
	~IncineratorNodeMetadata();

	virtual u16 typeId() const;
	virtual NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool nodeRemovalDisabled();
	virtual std::string getDrawSpecString(Player *player);
	virtual std::vector<NodeBox> getNodeBoxes(MapNode &n);

	bool m_should_fire;
	float m_fuel_totaltime;
	float m_fuel_time;

private:
	Inventory *m_inventory;
	float m_step_accumulator;
};


class CraftGuideNodeMetadata : public NodeMetadata
{
public:
	CraftGuideNodeMetadata();
	~CraftGuideNodeMetadata();

	virtual u16 typeId() const;
	NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText() {return wgettext("Craft Guide");}
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool import(NodeMetadata *meta);
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

	u16 getPage() {return m_page;}

private:
	Inventory *m_inventory;
	u16 m_page;
	u16 m_recipe;
};

class ReverseCraftGuideNodeMetadata : public NodeMetadata
{
public:
	ReverseCraftGuideNodeMetadata();
	~ReverseCraftGuideNodeMetadata();

	virtual u16 typeId() const;
	NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText() {return wgettext("Reverse Craft Guide");}
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool import(NodeMetadata *meta);
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

	u16 getPage() {return m_page;}

private:
	Inventory *m_inventory;
	u16 m_page;
	u16 m_recipe;

	//a helper function to reload the current page, to reduce repeated code
	//this will also automatically wrap around the page number, casting it to a signed type and using modulus
	void reloadPage();
};

class CookBookNodeMetadata : public NodeMetadata
{
public:
	CookBookNodeMetadata();
	~CookBookNodeMetadata();

	virtual u16 typeId() const;
	NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText() {return wgettext("Cooking Guide");}
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool import(NodeMetadata *meta);
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

	u16 getPage() {return m_page;}

private:
	Inventory *m_inventory;
	u16 m_page;
};

class DeCraftNodeMetadata : public NodeMetadata
{
public:
	DeCraftNodeMetadata();
	~DeCraftNodeMetadata();

	virtual u16 typeId() const;
	NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText() {return wgettext("Decrafting Guide");}
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool import(NodeMetadata *meta);
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

	u16 getPage() {return m_page;}

private:
	Inventory *m_inventory;
	u16 m_page;
};

class BookNodeMetadata : public NodeMetadata
{
public:
	BookNodeMetadata();

	virtual u16 typeId() const;
	NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText() {return narrow_to_wide(m_title);}
	virtual bool nodeRemovalDisabled();
	virtual bool import(NodeMetadata *meta);
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

	std::string getContent() { return m_content; }

private:
	std::string m_title;
	std::string m_content;
};

class DiaryNodeMetadata : public NodeMetadata
{
public:
	DiaryNodeMetadata();

	virtual u16 typeId() const;
	NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText() {return narrow_to_wide(m_title);}
	virtual bool nodeRemovalDisabled();
	virtual bool import(NodeMetadata *meta);
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

	virtual std::string getOwner(){ return m_owner; }
	virtual void setOwner(std::string t){ m_owner = t; }
	std::string getContent() { return m_content; }

private:
	std::string m_owner;
	std::string m_title;
	std::string m_content;
};

class ClosedBookNodeMetadata : public NodeMetadata
{
public:
	ClosedBookNodeMetadata();

	virtual u16 typeId() const;
	NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::string getText() {return m_title;}
	virtual std::wstring infoText() {return narrow_to_wide(m_title);}
	virtual bool nodeRemovalDisabled();
	virtual bool import(NodeMetadata *meta);

	virtual std::string getOwner(){ return m_owner; }
	std::string getContent() { return m_content; }
	u16 getPage() {return m_page;}

private:
	std::string m_owner;
	std::string m_title;
	std::string m_content;
	u16 m_page;
};

class BookShelfNodeMetadata : public NodeMetadata
{
public:
	BookShelfNodeMetadata();
	~BookShelfNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual Inventory* getInventory() {return m_inventory;}
	virtual bool nodeRemovalDisabled();
	virtual std::string getDrawSpecString(Player *player);
	virtual std::vector<NodeBox> getNodeBoxes(MapNode &n);

private:
	Inventory *m_inventory;
};

class ClockNodeMetadata : public NodeMetadata
{
public:
	ClockNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual std::vector<NodeBox> getNodeBoxes(MapNode &n);
private:
	u16 m_time;
};

class CauldronNodeMetadata : public NodeMetadata
{
public:
	CauldronNodeMetadata();
	~CauldronNodeMetadata();

	virtual u16 typeId() const;
	virtual NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool nodeRemovalDisabled();
	virtual std::string getDrawSpecString(Player *player);
	virtual std::vector<NodeBox> getNodeBoxes(MapNode &n);

	u8 m_water_level;
	bool m_water_heated;
	bool m_water_hot;

private:
	Inventory *m_inventory;
	float m_fuel_time;
	float m_src_time;
	float m_cool_time;
};

class ForgeNodeMetadata : public NodeMetadata
{
public:
	ForgeNodeMetadata();
	~ForgeNodeMetadata();

	virtual u16 typeId() const;
	virtual NodeMetadata* clone();
	static NodeMetadata* create(std::istream &is);
	virtual void serializeBody(std::ostream &os);
	virtual std::wstring infoText();
	virtual Inventory* getInventory() {return m_inventory;}
	virtual void inventoryModified();
	virtual bool step(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool nodeRemovalDisabled();
	virtual bool receiveFields(std::string formname, std::map<std::string, std::string> fields, Player *player);
	virtual std::string getDrawSpecString(Player *player);

private:
	bool m_show_craft;
	Inventory *m_inventory;
};

class CircuitNodeMetadata : public NodeMetadata
{
public:
	CircuitNodeMetadata();
	~CircuitNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
	virtual u8 getEnergy()
	{
		return m_energy;
	}
	virtual std::map<v3s16, u8> *getSources() {return &m_sources;}
protected:
	u8 m_energy;
	std::map<v3s16, u8> m_sources;
	f32 m_ptime;
};

class SwitchNodeMetadata : public CircuitNodeMetadata
{
public:
	SwitchNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
	virtual std::vector<NodeBox> getNodeBoxes(MapNode &n);
};

class SourceNodeMetadata : public SwitchNodeMetadata
{
public:
	SourceNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
};

class ButtonNodeMetadata : public CircuitNodeMetadata
{
public:
	ButtonNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
};

class SolarPanelNodeMetadata : public CircuitNodeMetadata
{
public:
	SolarPanelNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
};

class WaterWheelNodeMetadata : public CircuitNodeMetadata
{
public:
	WaterWheelNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
};

class RepeaterNodeMetadata : public CircuitNodeMetadata
{
public:
	RepeaterNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
private:
	u8 m_ticks;
};

class NotGateNodeMetadata : public CircuitNodeMetadata
{
public:
	NotGateNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
};

class DoorNodeMetadata : public CircuitNodeMetadata
{
public:
	DoorNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
private:
	f32 m_otime;
};

class PistonNodeMetadata : public CircuitNodeMetadata
{
public:
	PistonNodeMetadata();
	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual bool stepCircuit(float dtime, v3s16 pos, ServerEnvironment *env);
	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
private:
	bool extend(v3s16 pos, v3s16 dir, content_t arm, MapNode piston, ServerEnvironment *env);
	bool contract(v3s16 pos, v3s16 dir, bool sticky, MapNode piston, ServerEnvironment *env);
	f32 m_otime;
};

#endif

