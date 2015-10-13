#ifndef CONTENT_TOOLITEM_HEADER
#define CONTENT_TOOLITEM_HEADER

#include "mapnode.h"

#define CONTENT_TOOLITEM_MASK 0x4000

enum ToolType {
	TT_NONE = 0, // used only when checking against an item that isn't a tool
	TT_SPECIAL, // special toolitems cannot dig
	TT_AXE,
	TT_PICK,
	TT_SHOVEL,
	TT_SHEAR,
	TT_BUCKET,
	TT_SWORD,
	TT_SPEAR,
	TT_CLUB
};

struct ToolItemFeatures {
	content_t content;
	std::string texture;
	// the old 'subname'
	std::string name;
	// tooltip used in inventory
	std::wstring description;
	// the result of cooking this item
	std::string cook_result;
	// the fuel value of this item
	float fuel_time;
	// 0 if inedible, otherwise the value it improves hp by
	s16 edible;
	// the number dropped on right click, -1 for all
	s16 drop_count;
	// whether this tool can point at liquid nodes
	bool liquids_pointable;
	// whether this tool should die when trying to pick up damaging nodes
	bool damaging_nodes_diggable;
	// whether this tool has a punch effect, such as open doors
	bool has_punch_effect;
	// whether this tool can lock/unlock nodes
	bool has_unlock_effect;
	// whether this tool can rotate nodes
	bool has_rotate_effect;
	// whether this tool can start fires
	bool has_fire_effect;
	// the type of this tool
	ToolType type;
	// the hardness of this tool
	f32 hardness;
	// the dig time of this tool
	f32 dig_time;
	// the level of the tool, this affects the amount of minerals etc.
	u8 level;
	// the data value of this item
	ContentParamType param_type;
	// used for eg. bows throwing an arrow
	content_t thrown_item;
	// when this item is placed, it should be replaced by this in inventory
	content_t onplace_replace_item;
	// when this item is placed, it should place this node instead
	content_t onplace_node;

	ToolItemFeatures():
		content(CONTENT_IGNORE),
		texture("unknown_item.png"),
		name(""),
		description(L""),
		cook_result(""),
		fuel_time(0.0),
		edible(0),
		drop_count(-1),
		liquids_pointable(false),
		damaging_nodes_diggable(true),
		has_punch_effect(true),
		has_unlock_effect(false),
		has_rotate_effect(false),
		has_fire_effect(false),
		type(TT_NONE),
		hardness(0.),
		dig_time(3.),
		level(0),
		param_type(CPT_NONE),
		thrown_item(CONTENT_IGNORE),
		onplace_replace_item(CONTENT_IGNORE),
		onplace_node(CONTENT_IGNORE)
	{}
};
struct DiggingProperties
{
	DiggingProperties():
		diggable(false),
		time(0.0),
		wear(0)
	{
	}
	DiggingProperties(bool a_diggable, float a_time, u16 a_wear):
		diggable(a_diggable),
		time(a_time),
		wear(a_wear)
	{
	}
	bool diggable;
	// Digging time in seconds
	float time;
	// Caused wear
	u16 wear;
};

// For getting the default properties, set toolid=CONTENT_IGNORE
DiggingProperties getDiggingProperties(content_t material, u8 mineral, content_t toolid, u16 data=0);
std::string toolitem_overlay(content_t content, std::string ol);
void content_toolitem_init();
ToolItemFeatures & content_toolitem_features(content_t i);
ToolItemFeatures & content_toolitem_features(std::string subname);

#define CONTENT_TOOLITEM_SMALL_PICK (CONTENT_TOOLITEM_MASK | 0x01)
#define CONTENT_TOOLITEM_STPICK (CONTENT_TOOLITEM_MASK | 0x02)
#define CONTENT_TOOLITEM_STEELPICK (CONTENT_TOOLITEM_MASK | 0x03)
#define CONTENT_TOOLITEM_CREATIVEPICK (CONTENT_TOOLITEM_MASK | 0x04)
#define CONTENT_TOOLITEM_TROWEL (CONTENT_TOOLITEM_MASK | 0x05)
#define CONTENT_TOOLITEM_STSHOVEL (CONTENT_TOOLITEM_MASK | 0x06)
#define CONTENT_TOOLITEM_STEELSHOVEL (CONTENT_TOOLITEM_MASK | 0x07)
#define CONTENT_TOOLITEM_SMALL_AXE (CONTENT_TOOLITEM_MASK | 0x08)
#define CONTENT_TOOLITEM_STAXE (CONTENT_TOOLITEM_MASK | 0x09)
#define CONTENT_TOOLITEM_STEELAXE (CONTENT_TOOLITEM_MASK | 0x0A)
#define CONTENT_TOOLITEM_CLUB (CONTENT_TOOLITEM_MASK | 0x0B)
#define CONTENT_TOOLITEM_STSWORD (CONTENT_TOOLITEM_MASK | 0x0C)
#define CONTENT_TOOLITEM_STEELSWORD (CONTENT_TOOLITEM_MASK | 0x0D)
#define CONTENT_TOOLITEM_STEELSHEARS (CONTENT_TOOLITEM_MASK | 0x0E)
#define CONTENT_TOOLITEM_WBUCKET (CONTENT_TOOLITEM_MASK | 0x0F)
#define CONTENT_TOOLITEM_TINBUCKET (CONTENT_TOOLITEM_MASK | 0x10)
#define CONTENT_TOOLITEM_WBUCKET_WATER (CONTENT_TOOLITEM_MASK | 0x11)
#define CONTENT_TOOLITEM_TINBUCKET_WATER (CONTENT_TOOLITEM_MASK | 0x12)
#define CONTENT_TOOLITEM_STEELBUCKET (CONTENT_TOOLITEM_MASK | 0x13)
#define CONTENT_TOOLITEM_STEELBUCKET_WATER (CONTENT_TOOLITEM_MASK | 0x14)
#define CONTENT_TOOLITEM_STEELBUCKET_LAVA (CONTENT_TOOLITEM_MASK | 0x15)
#define CONTENT_TOOLITEM_FIRESTARTER (CONTENT_TOOLITEM_MASK | 0x16)
#define CONTENT_TOOLITEM_CROWBAR (CONTENT_TOOLITEM_MASK | 0x17)
#define CONTENT_TOOLITEM_FLINTPICK (CONTENT_TOOLITEM_MASK | 0x18)
#define CONTENT_TOOLITEM_FLINTSHOVEL (CONTENT_TOOLITEM_MASK | 0x19)
#define CONTENT_TOOLITEM_FLINTAXE (CONTENT_TOOLITEM_MASK | 0x1A)
#define CONTENT_TOOLITEM_FLINTSHEARS (CONTENT_TOOLITEM_MASK | 0x1B)
#define CONTENT_TOOLITEM_KEY (CONTENT_TOOLITEM_MASK | 0x1C)
#define CONTENT_TOOLITEM_STONESPEAR (CONTENT_TOOLITEM_MASK | 0x1D)
#define CONTENT_TOOLITEM_FLINTSPEAR (CONTENT_TOOLITEM_MASK | 0x1E)
#define CONTENT_TOOLITEM_STEELSPEAR (CONTENT_TOOLITEM_MASK | 0x1F)
#define CONTENT_TOOLITEM_BOW (CONTENT_TOOLITEM_MASK | 0x20)
#define CONTENT_TOOLITEM_MITHRIL_RAW_PICK (CONTENT_TOOLITEM_MASK | 0x21)
#define CONTENT_TOOLITEM_MITHRIL_RAW_SHOVEL (CONTENT_TOOLITEM_MASK | 0x22)
#define CONTENT_TOOLITEM_MITHRIL_RAW_AXE (CONTENT_TOOLITEM_MASK | 0x23)
#define CONTENT_TOOLITEM_MITHRIL_RAW_SWORD (CONTENT_TOOLITEM_MASK | 0x24)
#define CONTENT_TOOLITEM_MITHRIL_RAW_SPEAR (CONTENT_TOOLITEM_MASK | 0x25)
#define CONTENT_TOOLITEM_MITHRIL_UNBOUND_PICK (CONTENT_TOOLITEM_MASK | 0x26)
#define CONTENT_TOOLITEM_MITHRIL_UNBOUND_SHOVEL (CONTENT_TOOLITEM_MASK | 0x27)
#define CONTENT_TOOLITEM_MITHRIL_UNBOUND_AXE (CONTENT_TOOLITEM_MASK | 0x28)
#define CONTENT_TOOLITEM_MITHRIL_UNBOUND_SWORD (CONTENT_TOOLITEM_MASK | 0x29)
#define CONTENT_TOOLITEM_MITHRIL_UNBOUND_SPEAR (CONTENT_TOOLITEM_MASK | 0x2A)
#define CONTENT_TOOLITEM_MITHRIL_PICK (CONTENT_TOOLITEM_MASK | 0x2B)
#define CONTENT_TOOLITEM_MITHRIL_SHOVEL (CONTENT_TOOLITEM_MASK | 0x2C)
#define CONTENT_TOOLITEM_MITHRIL_AXE (CONTENT_TOOLITEM_MASK | 0x2D)
#define CONTENT_TOOLITEM_MITHRIL_SWORD (CONTENT_TOOLITEM_MASK | 0x2E)
#define CONTENT_TOOLITEM_MITHRIL_SPEAR (CONTENT_TOOLITEM_MASK | 0x2F)
#define CONTENT_TOOLITEM_MOB_SPAWNER (CONTENT_TOOLITEM_MASK | 0x30)

#endif
