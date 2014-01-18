#include "materials.h"
#include "mapnode.h"
#include "content_toolitem.h"

DiggingProperties getDiggingProperties(content_t content, content_t tool)
{
	ToolItemFeatures t_features = content_toolitem_features(tool);
	ContentFeatures &c_features = content_features(content);
	f32 time = t_features.dig_time*c_features.hardness;
	f32 wear = 0;
	if (t_features.hardness && c_features.hardness)
		wear = 65535/t_features.hardness*c_features.hardness;
	f32 diggable = true;
	if (c_features.type == CMT_STONE && t_features.type != TT_PICK) {
		diggable = false;
	}else{
		switch (t_features.type) {
		case TT_SPECIAL:
			diggable = false;
			break;
		case TT_AXE:
			if (c_features.type != CMT_WOOD && c_features.type != CMT_PLANT)
				time *= 10.;
			break;
		case TT_PICK:
			if (c_features.type != CMT_STONE)
				time *= 2.;
			break;
		case TT_SHOVEL:
			if (c_features.type != CMT_DIRT)
				time *= 10.;
			break;
		case TT_SWORD:
		case TT_SHEAR:
			if (c_features.type != CMT_PLANT)
				time *= 10.;
			break;
		case TT_BUCKET:
			if (c_features.type != CMT_LIQUID)
				time = 10.;
			break;
		case TT_NONE:
		default:
			break;
		}
	}


	return DiggingProperties(diggable,time,wear);
}

