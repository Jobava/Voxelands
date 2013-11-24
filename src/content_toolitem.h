#ifndef CONTENT_TOOLITEM_HEADER
#define CONTENT_TOOLITEM_HEADER

#include "mapnode.h"

#define CONTENT_TOOLITEM_MASK 0x4000

struct ToolItemFeatures {
	content_t content;
	std::string texture;
	// the old 'subname'
	std::string name;
	// tooltip used in inventory
	std::string gui_name;
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

	ToolItemFeatures():
		content(CONTENT_IGNORE),
		texture("unknown_item.png"),
		name(""),
		gui_name(""),
		cook_result(""),
		fuel_time(0.0),
		edible(0),
		drop_count(-1),
		liquids_pointable(false)
	{}
};

void content_toolitem_init();
ToolItemFeatures & content_toolitem_features(content_t i);
ToolItemFeatures & content_toolitem_features(std::string subname);

#define CONTENT_TOOLITEM_WPICK (CONTENT_TOOLITEM_MASK | 0x01)
#define CONTENT_TOOLITEM_STPICK (CONTENT_TOOLITEM_MASK | 0x02)
#define CONTENT_TOOLITEM_STEELPICK (CONTENT_TOOLITEM_MASK | 0x03)
#define CONTENT_TOOLITEM_MESEPICK (CONTENT_TOOLITEM_MASK | 0x04)
#define CONTENT_TOOLITEM_WSHOVEL (CONTENT_TOOLITEM_MASK | 0x05)
#define CONTENT_TOOLITEM_STSHOVEL (CONTENT_TOOLITEM_MASK | 0x06)
#define CONTENT_TOOLITEM_STEELSHOVEL (CONTENT_TOOLITEM_MASK | 0x07)
#define CONTENT_TOOLITEM_WAXE (CONTENT_TOOLITEM_MASK | 0x08)
#define CONTENT_TOOLITEM_STAXE (CONTENT_TOOLITEM_MASK | 0x09)
#define CONTENT_TOOLITEM_STEELAXE (CONTENT_TOOLITEM_MASK | 0x0A)
#define CONTENT_TOOLITEM_WSWORD (CONTENT_TOOLITEM_MASK | 0x0B)
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

#endif
