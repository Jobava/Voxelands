#ifndef CONTENT_CLOTHESITEM_HEADER
#define CONTENT_CLOTHESITEM_HEADER

#include "mapnode.h"
#include <map>

#define CONTENT_CLOTHESITEM_MASK 0x1000

enum ClothesType {
	CT_NONE, // used only when checking against an item that isn't clothes
	CT_PANTS,
	CT_SHIRT,
	CT_HAT,
	CT_BOOTS
};

struct ClothesItemFeatures {
	content_t content;
	// used for inventory / wielding etc
	std::string texture;
	// overlaid on the player texture so it can be seen as worn
	std::string overlay_texture;
	// tooltip used in inventory
	std::wstring description;
	// the type of this clothing
	ClothesType type;
	// the strength as armour
	f32 armour;
	// the effectiveness against the cold zone
	f32 warmth;
	// the effectiveness against vacuum / space
	f32 vacuum;
	// the effectiveness against suffocation
	f32 suffocate;
	// this determines how fast the item wears out from use
	u8 durability;

	ClothesItemFeatures():
		content(CONTENT_IGNORE),
		texture("unknown_item.png"),
		overlay_texture(""),
		description(L""),
		type(CT_NONE),
		armour(0.),
		warmth(0.),
		vacuum(0.),
		suffocate(0.),
		durability(10)
	{}
};

extern std::map<content_t,struct ClothesItemFeatures> g_content_clothesitem_features;
// For getting the default properties, set id=CONTENT_IGNORE
void content_clothesitem_init();
ClothesItemFeatures & content_clothesitem_features(content_t i);

#define CONTENT_CLOTHESITEM_FUR_PANTS (CONTENT_CLOTHESITEM_MASK | 0x01)
#define CONTENT_CLOTHESITEM_FUR_SHIRT (CONTENT_CLOTHESITEM_MASK | 0x02)
#define CONTENT_CLOTHESITEM_FUR_HAT (CONTENT_CLOTHESITEM_MASK | 0x03)
#define CONTENT_CLOTHESITEM_FUR_BOOTS (CONTENT_CLOTHESITEM_MASK | 0x04)
#define COTNENT_CLOTHESITEM_SPACESUIT_PANTS (CONTENT_CLOTHESITEM_MASK | 0x05)
#define COTNENT_CLOTHESITEM_SPACESUIT_SHIRT (CONTENT_CLOTHESITEM_MASK | 0x06)
#define COTNENT_CLOTHESITEM_SPACESUIT_HELMET (CONTENT_CLOTHESITEM_MASK | 0x07)
#define COTNENT_CLOTHESITEM_SPACESUIT_BOOTS (CONTENT_CLOTHESITEM_MASK | 0x08)

#endif
