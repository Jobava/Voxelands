/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mapnode.h
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

#ifndef MAPNODE_HEADER
#define MAPNODE_HEADER

#include <iostream>
#include "common_irrlicht.h"
#include "light.h"
#include "exceptions.h"
#include "serialization.h"
#ifndef SERVER
#include "tile.h"
#endif

/*
	Naming scheme:
	- Material = irrlicht's Material class
	- Content = (content_t) content of a node
	- Tile = TileSpec at some side of a node of some content type

	Content ranges:
		0x000...0x07f: param2 is fully usable
		0x800...0xfff: param2 lower 4 bytes are free
*/
typedef u16 content_t;
#define MAX_CONTENT 0xfff

/*
	Initializes all kind of stuff in here.
	Many things depend on this.

	This accesses g_texturesource; if it is non-NULL, textures are set.

	Client first calls this with g_texturesource=NULL to run some
	unit tests and stuff, then it runs this again with g_texturesource
	defined to get the textures.

	Server only calls this once with g_texturesource=NULL.
*/
#ifndef SERVER
#include "common_irrlicht.h"
void init_mapnode(irr::IrrlichtDevice* device);
#else
void init_mapnode();
#endif

/*
	Ignored node.

	Anything that stores MapNodes doesn't have to preserve parameters
	associated with this material.

	Doesn't create faces with anything and is considered being
	out-of-map in the game map.
*/
//#define CONTENT_IGNORE 255
#define CONTENT_IGNORE 127
#define CONTENT_IGNORE_DEFAULT_PARAM 0

/*
	The common material above y=1000 through which the player can
	walk and which is transparent to light, but kills fast without
	a pressure suit
*/
#define CONTENT_VACUUM 125
/*
	The common material through which the player can walk and which
	is transparent to light
*/
#define CONTENT_AIR 126

/*
	Draw types
*/
enum ContentDrawType
{
	CDT_AIRLIKE,
	CDT_CUBELIKE,
	CDT_RAILLIKE,
	CDT_PLANTLIKE,
	CDT_PLANTLIKE_SML,
	CDT_PLANTLIKE_LGE,
	CDT_MELONLIKE,
	CDT_LIQUID,
	CDT_LIQUID_SOURCE,
	CDT_NODEBOX,
	CDT_GLASSLIKE,
	CDT_TORCHLIKE,
	CDT_FENCELIKE,
	CDT_FIRELIKE,
	CDT_WALLLIKE,
	CDT_ROOFLIKE,
	CDT_LEAFLIKE,
	CDT_NODEBOX_META,
	CDT_WIRELIKE,
	CDT_3DWIRELIKE,
	CDT_STAIRLIKE,
	CDT_SLABLIKE,
	CDT_TRUNKLIKE,
	CDT_FLAGLIKE
};

/*
	Content feature list
*/

enum ContentParamType
{
	CPT_NONE,
	CPT_LIGHT,
	CPT_MINERAL,
	// Direction for chests and furnaces and such
	CPT_FACEDIR_SIMPLE,
	CPT_FACEDIR_WALLMOUNT,
	CPT_LIQUID,
	CPT_PLANTGROWTH,
	CPT_ENCHANTMENT,
	CPT_DROP,
	CPT_SPECIAL
};

enum LiquidType
{
	LIQUID_NONE,
	LIQUID_FLOWING,
	LIQUID_SOURCE
};

/*
	Material Type
*/
enum ContentMaterialType {
	CMT_AIR,
	CMT_WOOD,
	CMT_TREE,
	CMT_STONE,
	CMT_LIQUID,
	CMT_PLANT,
	CMT_DIRT,
	CMT_GLASS
};

/*
	Circuits Type
*/
enum ContentEnergyType {
	CET_NONE,
	CET_CONDUCTIVE,
	CET_SOURCE,
	CET_SWITCH,
	CET_GATE
};

/*
	PressureType
	This determines the behaviour when pushed by a piston
*/
enum ContentPressureType {
	CST_MOVABLE,
	CST_DROPABLE,	// node may drop when above an upward-facing piston
	CST_SOLID,
	CST_CRUSHABLE,	// When pushed, this node will be obliterated
			// if pushed against another node
	CST_CRUSHED	// node is always obliterated when pushed
};

enum NodeTextureIndex {
	NODETEXTURE_TOP = 0,
	NODETEXTURE_BOTTOM,
	NODETEXTURE_RIGHT,
	NODETEXTURE_LEFT,
	NODETEXTURE_BACK,
	NODETEXTURE_FRONT
};

struct MapNode;
class NodeMetadata;

class NodeBox
{
public:
	NodeBox():
		m_angle(0,0,0),
		m_centre(0,0,0),
		m_box(0.0,0.0,0.0,0.0,0.0,0.0)
	{
	}
	NodeBox(v3s16 angle, aabb3f box):
		m_angle(angle),
		m_centre(0,0,0),
		m_box(box)
	{
	}
	NodeBox(v3s16 angle, v3f centre, aabb3f box):
		m_angle(angle),
		m_centre(centre),
		m_box(box)
	{
	}
	NodeBox(v3s16 angle, v3f centre, f32 tlx, f32 tly, f32 tlz, f32 brx, f32 bry, f32 brz):
		m_angle(angle),
		m_centre(centre),
		m_box(tlx,tly,tlz,brx,bry,brz)
	{
	}
	NodeBox(aabb3f box):
		m_angle(0,0,0),
		m_centre(0,0,0),
		m_box(box)
	{
	}
	NodeBox(f32 tlx, f32 tly, f32 tlz, f32 brx, f32 bry, f32 brz):
		m_angle(0,0,0),
		m_centre(0,0,0),
		m_box(tlx,tly,tlz,brx,bry,brz)
	{
	}
	v3s16 m_angle;
	v3f m_centre;
	aabb3f m_box;
};

enum FaceTextType {
	FTT_INFO,
	FTT_BOOKCONTENT,
	FTT_OWNER,
	FTT_INVOWNER
};

class FaceText
{
public:
	FaceText():
		m_hastext(false)
	{
	}
	FaceText(f32 tlx, f32 tly, f32 brx, f32 bry):
		m_type(FTT_INFO),
		m_hastext(true)
	{
#ifndef SERVER
		m_pos = core::rect<f32>(tlx,tly,brx,bry);
#endif
	}
	FaceText(f32 tlx, f32 tly, f32 brx, f32 bry, FaceTextType type):
		m_type(type),
		m_hastext(true)
	{
#ifndef SERVER
		m_pos = core::rect<f32>(tlx,tly,brx,bry);
#endif
	}

#ifndef SERVER
	core::rect<f32> m_pos;
#endif
	FaceTextType m_type;
	bool m_hastext;
};

std::vector<NodeBox> transformNodeBox(MapNode &n,
		const std::vector<NodeBox> &nodebox);

struct ContentFeatures
{
#ifndef SERVER
	/*
		0: up
		1: down
		2: right
		3: left
		4: back
		5: front
	*/
	TileSpec tiles[6];
	TileSpec meta_tiles[6];

	video::ITexture *inventory_texture;

	// Used currently for flowing liquids
	u8 vertex_alpha;
	// Post effect color, drawn when the camera is inside the node.
	video::SColor post_effect_color;
#endif

	bool rotate_tile_with_nodebox;
	bool wield_nodebox;
	std::wstring description;
	std::vector<NodeBox> nodeboxes;
	std::vector<NodeBox> wield_nodeboxes;

	// positions for text on faces
	FaceText facetexts[6];

	// List of all block textures that have been used (value is dummy)
	// Exists on server too for cleaner code in content_mapnode.cpp
	core::map<std::string, bool> used_texturenames;

	// Type of MapNode::param1
	ContentParamType param_type;
	ContentParamType param2_type;
	// drawtype
	ContentDrawType draw_type;
	// True for all ground-like things like stone and mud, false for eg. trees
	bool is_ground_content;
	bool light_propagates;
	bool sunlight_propagates;
	u8 solidness; // Used when choosing which face is drawn
	u8 visual_solidness; // When solidness=0, this tells how it looks like
	// This is used for collision detection.
	// Also for general solidness queries.
	bool walkable;
	// Player can point to these
	bool pointable;
	// Player can point to these while holding a material (node)
	bool material_pointable;
	// Player can dig these
	bool diggable;
	// Player can climb these
	bool climbable;
	// Player can build on these
	bool buildable_to;
	// Whether fire can be started on top of it
	u8 flammable;
	// Whether a player or mob can jump over this node
	bool jumpable;
	// Whether the node conducts circuit current
	ContentEnergyType energy_type;
	// How the current drops by on this conductive node
	u8 energy_drop;
	// Whether the node has no liquid, source liquid or flowing liquid
	enum LiquidType liquid_type;
	// If true, node is equivalent to air. Torches are, air is. Water is not.
	// Is used for example to check whether a mud block can have grass on.
	bool air_equivalent;
	// Whether this content type often contains mineral.
	// Used for texture atlas creation.
	// Currently only enabled for CONTENT_STONE.
	bool often_contains_mineral;
	// Whether destructive mobs can destroy this node
	bool destructive_mob_safe;
	// Whether punching with fertilizer advances the growth rate of the node
	bool fertilizer_affects;
	// when dug with a shovel near water, turns to farm dirt
	bool farm_ploughable;
	// if true, this node can be dug even in a borderstone protected area
	bool borderstone_diggable;

	// Inventory item string as which the node appears in inventory when dug.
	// Mineral overrides this.
	std::string dug_item;
	// Extra dug item and its rarity
	std::string extra_dug_item;
	s32 extra_dug_item_rarity;
	// the minimum tool level required to get the extra dug item
	u8 extra_dug_item_min_level;
	// the maximum tool level required to get the extra dug item
	u8 extra_dug_item_max_level;

	// when dug, this node will replace the dug node
	content_t ondig_replace_node;
	// the above replacement will only occur if this node is nearby (3 node radius)
	content_t ondig_replace_node_requires;
	// drop this instead of dug_item when the special tooltype is used
	content_t ondig_special_drop;
	u16 ondig_special_drop_count;
	u16 ondig_special_tool;
	// for above, when dug the special tool's name has this appended to it
	// used for sponge filling buckets
	// if set, ondig_special_drop replaces the node, instead of being picked up
	std::string ondig_special_tool_append;
	// when punched, this node will replace the punched node
	content_t onpunch_replace_node;

	// when placed against a wall, this node should be placed instead
	content_t wallmount_alternate_node;
	// when placed on the floor, this node should be placed instead
	content_t floormount_alternate_node;
	// when placed on the roof, this node should be placed instead
	content_t roofmount_alternate_node;
	// special node for things like slabs combining into cubes,
	// walls connecting to blocks, or seeds growing to plants
	content_t special_alternate_node;

	// if this node is locked/unlocked, replace with this
	content_t alternate_lockstate_node;

	// if the item is wielded and Use is pressed, the node is swapped with this
	content_t onuse_replace_node;
	// when dug/punched also affects this
	v3s16 onact_also_affects;
	// when dug, the node's inventory is given to the player
	bool ondig_gives_inventory;
	// if the inventory contents won't all fit in the player's
	// if true then node remains undug, player gets nothing
	// if false then player gets as much as possible, rest stays in undug node
	bool ondig_gives_inventory_all_or_none;

	// when energised, replace with this node
	content_t powered_node;
	// when unenergised, replace with this node
	content_t unpowered_node;

	// the result of cooking this node
	std::string cook_result;
	// the fuel value of this node
	float fuel_time;

	// maximum height a plant can grow to
	s16 plantgrowth_max_height;
	// when CPT_PLANTGROWTH < 8 digging gives this
	content_t plantgrowth_small_dug_node;
	// when CPT_PLANTGROWTH > 7 digging gives this
	content_t plantgrowth_large_dug_node;
	// the maximum number of large items given
	u16 plantgrowth_large_count;
	// whether to also give small when large is given
	bool plantgrowth_large_gives_small;
	// whether this node grows on trellis
	bool plantgrowth_on_trellis;
	// if this node spreads to trellis to continue growing
	// then this is the node that grows on the trellis
	content_t plantgrowth_trellis_node;

	// Initial metadata is cloned from this
	NodeMetadata *initial_metadata;
	// Sound played when the node's formspec is accessed
	std::string sound_access;
	// Sound played when the node is stepped on or in
	std::string sound_step;
	// Sound played when the node is dug
	std::string sound_dig;
	// Sound played when the node is placed
	std::string sound_place;
	// Sound played when the node is punched
	std::string sound_punch;
	// Sound played by the node always
	std::string sound_ambient;

	// If the content is liquid, this is the flowing version of the liquid.
	// If content is liquid, this is the same content.
	content_t liquid_alternative_flowing;
	// If the content is liquid, this is the source version of the liquid.
	content_t liquid_alternative_source;
	// Viscosity for fluid flow, ranging from 1 to 7, with
	// 1 giving almost instantaneous propagation and 7 being
	// the slowest possible
	u8 liquid_viscosity;

	// Amount of light the node emits
	u8 light_source;

	// the material type (stone, wood, etc)
	ContentMaterialType type;
	// the hardness of the node
	f32 hardness;
	// for pistons
	ContentPressureType pressure_type;

	// damage player with no protection
	u32 damage_per_second;
	// damage player with suffocation
	u32 suffocation_per_second;
	// damage player with cold/heat
	u32 warmth_per_second;
	// damage player with pressure (vacuum)
	u32 pressure_per_second;

	// can be used to set the player's respawn position
	s8 home_node;

	// NOTE: Move relevant properties to here from elsewhere

	void reset()
	{
#ifndef SERVER
		inventory_texture = NULL;

		vertex_alpha = 255;
		post_effect_color = video::SColor(0, 0, 0, 0);
#endif
		rotate_tile_with_nodebox = false;
		wield_nodebox = true;
		description = std::wstring(L"");
		nodeboxes.clear();
		nodeboxes.push_back(NodeBox(
			-0.5*BS,
			-0.5*BS,
			-0.5*BS,
			0.5*BS,
			0.5*BS,
			0.5*BS
		));
		wield_nodeboxes.clear();
		setAllFaceTexts(FaceText());
		param_type = CPT_NONE;
		param2_type = CPT_NONE;
		draw_type = CDT_AIRLIKE;
		is_ground_content = false;
		light_propagates = false;
		sunlight_propagates = false;
		solidness = 2;
		visual_solidness = 0;
		walkable = true;
		pointable = true;
		material_pointable = true;
		diggable = true;
		climbable = false;
		buildable_to = false;
		flammable = 0;
		jumpable = true;
		energy_type = CET_NONE;
		energy_drop = 1;
		liquid_type = LIQUID_NONE;
		air_equivalent = false;
		often_contains_mineral = false;
		destructive_mob_safe = false;
		fertilizer_affects = false;
		farm_ploughable = false;
		borderstone_diggable = false;
		dug_item = "";
		extra_dug_item = "";
		extra_dug_item_rarity = 2;
		extra_dug_item_min_level = 0;
		extra_dug_item_max_level = 100;
		ondig_replace_node = CONTENT_IGNORE;
		ondig_replace_node_requires = CONTENT_IGNORE;
		ondig_special_drop = CONTENT_IGNORE;
		ondig_special_drop_count = 1;
		ondig_special_tool = 0;
		ondig_special_tool_append = "";
		onpunch_replace_node = CONTENT_IGNORE;
		wallmount_alternate_node = CONTENT_IGNORE;
		floormount_alternate_node = CONTENT_IGNORE;
		roofmount_alternate_node = CONTENT_IGNORE;
		special_alternate_node = CONTENT_IGNORE;
		alternate_lockstate_node = CONTENT_IGNORE;
		onuse_replace_node = CONTENT_IGNORE;
		onact_also_affects = v3s16(0,0,0);
		ondig_gives_inventory = false;
		ondig_gives_inventory_all_or_none = false;
		powered_node = CONTENT_IGNORE;
		unpowered_node = CONTENT_IGNORE;
		cook_result = "";
		fuel_time = 0.0;
		plantgrowth_max_height = 1;
		plantgrowth_small_dug_node = CONTENT_IGNORE;
		plantgrowth_large_dug_node = CONTENT_IGNORE;
		plantgrowth_large_count = 3;
		plantgrowth_large_gives_small = false;
		plantgrowth_on_trellis = false;
		plantgrowth_trellis_node = CONTENT_IGNORE;
		initial_metadata = NULL;
		sound_access = "";
		sound_step = "";
		sound_dig = "";
		sound_place = "";
		sound_punch = "";
		sound_ambient = "";
		liquid_alternative_flowing = CONTENT_IGNORE;
		liquid_alternative_source = CONTENT_IGNORE;
		liquid_viscosity = 0;
		light_source = 0;
		type = CMT_AIR;
		hardness = 1.0;
		pressure_type = CST_MOVABLE;
		damage_per_second = 0;
		suffocation_per_second = 4;
		warmth_per_second = 0;
		pressure_per_second = 0;
		home_node = -1;
	}

	ContentFeatures()
	{
		reset();
	}

	~ContentFeatures();

	/*
		Bounding Box
	*/

	/*
		Gets list of node boxes (used for collision)
	*/
	std::vector<NodeBox> getNodeBoxes(MapNode &n) const;

	void setNodeBox(NodeBox nb)
	{
		nodeboxes.clear();
		nodeboxes.push_back(nb);
	}

	void addNodeBox(NodeBox nb)
	{
		nodeboxes.push_back(nb);
	}

	std::vector<NodeBox> getWieldNodeBoxes() const;

	void setWieldNodeBox(NodeBox nb)
	{
		wield_nodeboxes.clear();
		wield_nodeboxes.push_back(nb);
	}

	void addWieldNodeBox(NodeBox nb)
	{
		wield_nodeboxes.push_back(nb);
	}

	void setFaceText(u16 i, FaceText ft)
	{
		facetexts[i] = ft;
	}

	void setAllFaceTexts(FaceText ft)
	{
		for (u16 i=0; i<6; i++) {
			setFaceText(i,ft);
		}
	}

	/*
		Quickhands for simple materials
	*/

#ifdef SERVER
	void setTexture(u16 i, std::string name, u8 alpha=255)
	{}
	void setAllTextures(std::string name, u8 alpha=255)
	{}
	void setTextureFlags(u16 i, u8 flags)
	{}
	void setAllTextureFlags(u8 flags)
	{}
	void setTextureType(u16 i, u8 type)
	{}
	void setAllTextureTypes(u8 type)
	{}
	void setMetaTexture(u16 i, std::string name, u8 alpha=255)
	{}
	void setAllMetaTextures(std::string name, u8 alpha=255)
	{}
	void setMetaTextureFlags(u16 i, u8 flags)
	{}
	void setAllMetaTextureFlags(u8 flags)
	{}
	void setMetaTextureType(u16 i, u8 type)
	{}
	void setAllMetaTextureTypes(u8 type)
	{}
#else
	void setTexture(u16 i, std::string name, u8 alpha=255);

	void setAllTextures(std::string name, u8 alpha=255)
	{
		for (u16 i=0; i<6; i++) {
			setTexture(i, name, alpha);
		}
		// Force inventory texture too
		setInventoryTexture(name);
	}
	void setTextureFlags(u16 i, u8 flags)
	{
		tiles[i].material_flags = flags;
	}
	void setAllTextureFlags(u8 flags)
	{
		for (u16 i=0; i<6; i++) {
			setTextureFlags(i, flags);
		}
	}
	void setTextureType(u16 i, u8 type)
	{
		tiles[i].material_type = type;
	}
	void setAllTextureTypes(u8 type)
	{
		for (u16 i=0; i<6; i++) {
			setTextureType(i, type);
		}
	}

	void setMetaTexture(u16 i, std::string name, u8 alpha=255);

	void setAllMetaTextures(std::string name, u8 alpha=255)
	{
		for (u16 i=0; i<6; i++) {
			setMetaTexture(i, name, alpha);
		}
	}
	void setMetaTextureFlags(u16 i, u8 flags)
	{
		meta_tiles[i].material_flags = flags;
	}
	void setAllMetaTextureFlags(u8 flags)
	{
		for (u16 i=0; i<6; i++) {
			setMetaTextureFlags(i, flags);
		}
	}
	void setMetaTextureType(u16 i, u8 type)
	{
		meta_tiles[i].material_type = type;
	}
	void setAllMetaTextureTypes(u8 type)
	{
		for (u16 i=0; i<6; i++) {
			setMetaTextureType(i, type);
		}
	}

#endif

#ifndef SERVER
	void setTile(u16 i, const TileSpec &tile)
	{
		tiles[i] = tile;
	}
	void setAllTiles(const TileSpec &tile)
	{
		for(u16 i=0; i<6; i++)
		{
			setTile(i, tile);
		}
	}
#endif

#ifdef SERVER
	void setInventoryTexture(std::string imgname)
	{}
	void setInventoryTextureCube(std::string top,
			std::string left, std::string right)
	{}
	void setInventoryTextureNodeBox(content_t c, std::string top,
			std::string left, std::string right)
	{}
#else
	void setInventoryTexture(std::string imgname);

	void setInventoryTextureCube(std::string top,
			std::string left, std::string right);
	void setInventoryTextureNodeBox(content_t c, std::string top,
			std::string left, std::string right);
#endif
};

/*
	Call this to access the ContentFeature list
*/
ContentFeatures & content_features(content_t i);
ContentFeatures & content_features(MapNode &n);

struct SelectedNode
{
	v3s16 pos;
	u16 crack;
	bool has_crack;
	bool is_coloured;
	content_t content;

	SelectedNode()
	{
		pos = v3s16(0,0,0);
		has_crack = false;
		is_coloured = false;
		content = CONTENT_IGNORE;
	}

	SelectedNode(v3s16 p, u16 c, bool h, content_t cnt)
	{
		pos = p;
		if (c) {
			crack = c-1;
			has_crack = true;
		}else{
			has_crack = false;
		}
		is_coloured = h;
		content = cnt;
	}
};

/*
	Packs directions like (1,0,0), (1,-1,0)
*/
inline u8 packDir(v3s16 dir)
{
	u8 b = 0;
	if (dir.Y > 0)
		return 4;
	if (dir.Y < 0)
		return 5;
	if (dir.Z > 0)
		return 0;
	if (dir.X > 0)
		return 1;
	if (dir.Z < 0)
		return 2;
	if (dir.X < 0)
		return 3;

	return b;
}
inline v3s16 unpackDir(u8 b)
{
	v3s16 d(0,0,0);

	switch (b) {
	case 0:
		d.Z = 1;
		break;
	case 1:
		d.X = 1;
		break;
	case 2:
		d.Z = -1;
		break;
	case 3:
		d.X = -1;
		break;
	case 4:
		d.Y = 1;
		break;
	case 5:
		d.Y = -1;
		break;
	default:;
	}

	return d;
}

/*
	facedir: CPT_FACEDIR_SIMPLE param value
	dir: The face for which stuff is wanted
	return value: The face from which the stuff is actually found

	NOTE: Currently this uses 2 bits for Z-,X-,Z+,X+, should there be Y+
	      and Y- too?
*/
v3s16 facedir_rotate(u8 facedir, v3s16 dir);

enum LightBank
{
	LIGHTBANK_DAY,
	LIGHTBANK_NIGHT
};

/*
	Masks for MapNode.param2 of flowing liquids
*/
#define LIQUID_LEVEL_MASK 0x07
#define LIQUID_FLOW_DOWN_MASK 0x08

/* maximum amount of liquid in a block */
#define LIQUID_LEVEL_MAX LIQUID_LEVEL_MASK
#define LIQUID_LEVEL_SOURCE (LIQUID_LEVEL_MAX+1)

/*
	This is the stuff what the whole world consists of.
*/


struct MapNode
{
	/*
		Main content
	*/
	content_t content;

	/*
		Misc parameter. Initialized to 0.
		- For light_propagates() blocks, this is light intensity,
		  stored logarithmically from 0 to LIGHT_MAX.
		  Sunlight is LIGHT_SUN, which is LIGHT_MAX+1.
		  - Contains 2 values, day- and night lighting. Each takes 4 bits.
		- Mineral content (should be removed from here)
		- Uhh... well, most blocks have light or nothing in here.
	*/
	u8 param1;

	/*
		The second parameter. Initialized to 0.
		E.g. direction for torches and flowing water.
	*/
	u8 param2;

	u32 envticks;

	MapNode(const MapNode & n)
	{
		*this = n;
	}

	MapNode(content_t a_content=CONTENT_AIR, u8 a_param1=0, u8 a_param2=0)
	{
		content = a_content;
		param1 = a_param1;
		param2 = a_param2;
		envticks = 0;
	}

	bool operator==(const MapNode &other)
	{
		return (content == other.content
				&& param1 == other.param1
				&& param2 == other.param2);
	}

	// To be used everywhere
	content_t getContent()
	{
		return content;
	}
	void setContent(content_t c)
	{
		content = c;
		envticks = 0;
	}

	u8 getLightBanksWithSource()
	{
		// Select the brightest of [light source, propagated light]
		u8 lightday = 0;
		u8 lightnight = 0;
		ContentFeatures &f = content_features(content);
		if (f.param_type == CPT_LIGHT) {
			lightday = param1 & 0x0f;
			lightnight = (param1>>4)&0x0f;
		}
		if (f.light_source > lightday)
			lightday = f.light_source;
		if (f.light_source > lightnight)
			lightnight = f.light_source;
		return (lightday&0x0f) | ((lightnight<<4)&0xf0);
	}

	u8 getLight(enum LightBank bank)
	{
		// Select the brightest of [light source, propagated light]
		u8 light = 0;
		ContentFeatures &f = content_features(content);
		if (f.param_type == CPT_LIGHT) {
			if (bank == LIGHTBANK_DAY) {
				light = param1 & 0x0f;
			}else if (bank == LIGHTBANK_NIGHT) {
				light = (param1>>4)&0x0f;
			}
		}
		if (f.light_source > light)
			light = f.light_source;
		return light;
	}

	// 0 <= daylight_factor <= 1000
	// 0 <= return value <= LIGHT_SUN
	u8 getLightBlend(u32 daylight_factor)
	{
		u8 l = ((daylight_factor * getLight(LIGHTBANK_DAY)
			+ (1000-daylight_factor) * getLight(LIGHTBANK_NIGHT))
			)/1000;
		u8 max = LIGHT_MAX;
		if(getLight(LIGHTBANK_DAY) == LIGHT_SUN)
			max = LIGHT_SUN;
		if(l > max)
			l = max;
		return l;
	}

	void setLight(enum LightBank bank, u8 a_light)
	{
		// If node doesn't contain light data, ignore this
		if (content_features(content).param_type != CPT_LIGHT)
			return;
		if (bank == LIGHTBANK_DAY) {
			param1 &= 0xf0;
			param1 |= a_light & 0x0f;
		}else if(bank == LIGHTBANK_NIGHT) {
			param1 &= 0x0f;
			param1 |= (a_light & 0x0f)<<4;
		}
	}
	v3s16 getRotation(v3s16 dir = v3s16(1,1,1));
	s16 getRotationAngle();
	v3s16 getEffectedRotation();

	// In mapnode.cpp
#ifndef SERVER
	/*
		Get tile of a face of the node.
		dir: direction of face
		Returns: TileSpec. Can contain miscellaneous texture coordinates,
		         which must be obeyed so that the texture atlas can be used.
	*/
	TileSpec getTile(v3s16 dir, bool rotate = true) {return getTileFrom(dir,content_features(*this).tiles,rotate); }
	TileSpec getMetaTile(v3s16 dir, bool rotate = true) {return getTileFrom(dir,content_features(*this).meta_tiles,rotate); }
	TileSpec getTileFrom(v3s16 dir, TileSpec raw_spec[6], bool rotate = true);
	std::string getTileRotationString(v3s16 dir);
#endif

	FaceText getFaceText(v3s16 dir);

	/*
		Gets mineral content of node, if there is any.
		MINERAL_NONE if doesn't contain or isn't able to contain mineral.
	*/
	u8 getMineral();

	/*
		Serialization functions
	*/

	static u32 serializedLength(u8 version);
	void serialize(u8 *dest, u8 version);
	void deSerialize(u8 *source, u8 version);

};

/*
	Gets lighting value at face of node

	Parameters must consist of air and !air.
	Order doesn't matter.

	If either of the nodes doesn't exist, light is 0.

	parameters:
		daynight_ratio: 0...1000
		n: getNodeParent(p)
		n2: getNodeParent(p + face_dir)
		face_dir: axis oriented unit vector from p to p2

	returns encoded light value.
*/
u8 getFaceLight(u32 daynight_ratio, MapNode n, MapNode n2, v3s16 face_dir);
u8 face_light(MapNode n, MapNode n2, v3s16 face_dir);

#endif

