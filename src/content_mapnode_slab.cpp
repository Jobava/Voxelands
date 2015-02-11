/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mapnode_slab.cpp
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
************************************************************************/

#include "content_mapnode.h"
#include "content_nodebox.h"
#include "content_list.h"
#include "content_craft.h"
#include "content_nodemeta.h"
#include "gettext.h"

void content_mapnode_slab(bool repeat)
{
	content_t i;
	ContentFeatures *f = NULL;

	// slabs
	i = CONTENT_ROUGHSTONE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Rough Stone Slab");
	f->setAllTextures("roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_ROUGHSTONE;
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COBBLE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Cobble Stone Slab");
	f->setAllTextures("cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_COBBLE;
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_COBBLE,CONTENT_COBBLE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MOSSYCOBBLE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Mossy Cobble Stone Slab");
	f->setAllTextures("mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_MOSSYCOBBLE;
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.8;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_MOSSYCOBBLE,CONTENT_MOSSYCOBBLE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Stone Slab");
	f->setAllTextures("stone.png");
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_STONE;
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_STONE,CONTENT_STONE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_SLAB;
	f = &content_features(i);
	f->description = wgettext("Wood Slab");
	f->setAllTextures("wood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_WOOD;
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_WOOD,CONTENT_WOOD_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_JUNGLE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Jungle Wood Slab");
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_JUNGLEWOOD;
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_JUNGLEWOOD,CONTENT_JUNGLE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BRICK_SLAB;
	f = &content_features(i);
	f->description = wgettext("Brick Slab");
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	f->special_alternate_node = CONTENT_BRICK;
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_BRICK,CONTENT_BRICK_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SANDSTONE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Sand Stone Slab");
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->special_alternate_node = CONTENT_SANDSTONE;
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_SLAB;
	f = &content_features(i);
	f->description = wgettext("Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab.png");
	f->setTexture(0,"glass.png");
	f->setTexture(1,"glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass.png", "glass_slab.png", "glass_slab.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_BLUE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Blue Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_BLUE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_blue.png");
	f->setTexture(0,"glass_blue.png");
	f->setTexture(1,"glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_blue.png", "glass_slab_blue.png", "glass_slab_blue.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_BLUE_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_GLASS_BLUE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_GREEN_SLAB;
	f = &content_features(i);
	f->description = wgettext("Green Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_GREEN;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_green.png");
	f->setTexture(0,"glass_green.png");
	f->setTexture(1,"glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_green.png", "glass_slab_green.png", "glass_slab_green.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_GREEN_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_GLASS_GREEN_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_ORANGE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Orange Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_ORANGE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_orange.png");
	f->setTexture(0,"glass_orange.png");
	f->setTexture(1,"glass_oange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_orange.png", "glass_slab_orange.png", "glass_slab_orange.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_ORANGE_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_GLASS_ORANGE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PURPLE_SLAB;
	f = &content_features(i);
	f->description = wgettext("Purple Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_PURPLE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_purple.png");
	f->setTexture(0,"glass_purple.png");
	f->setTexture(1,"glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_purple.png", "glass_slab_purple.png", "glass_slab_purple.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_PURPLE_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_GLASS_PURPLE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_RED_SLAB;
	f = &content_features(i);
	f->description = wgettext("Red Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_RED;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_red.png");
	f->setTexture(0,"glass_red.png");
	f->setTexture(1,"glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_red.png", "glass_slab_red.png", "glass_slab_red.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_RED_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_RED,CONTENT_GLASS_RED_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_YELLOW_SLAB;
	f = &content_features(i);
	f->description = wgettext("Yellow Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_YELLOW;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_yellow.png");
	f->setTexture(0,"glass_yellow.png");
	f->setTexture(1,"glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_yellow.png", "glass_slab_yellow.png", "glass_slab_yellow.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_YELLOW_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_GLASS_YELLOW_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_BLACK_SLAB;
	f = &content_features(i);
	f->description = wgettext("Black Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_BLACK;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_black.png");
	f->setTexture(0,"glass_black.png");
	f->setTexture(1,"glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_black.png", "glass_slab_black.png", "glass_slab_black.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_BLACK_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_GLASS_BLACK_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// upside down slabs
	i = CONTENT_ROUGHSTONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE_SLAB)+" 1";
	f->special_alternate_node = CONTENT_ROUGHSTONE;
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	f->suffocation_per_second = 0;

	i = CONTENT_COBBLE_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Cobble Stone Slab");
	f->setAllTextures("cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_COBBLE;
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	f->suffocation_per_second = 0;

	i = CONTENT_MOSSYCOBBLE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MOSSYCOBBLE_SLAB)+" 1";
	f->special_alternate_node = CONTENT_MOSSYCOBBLE;
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.8;
	f->suffocation_per_second = 0;

	i = CONTENT_STONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("stone.png");
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STONE_SLAB)+" 1";
	f->special_alternate_node = CONTENT_STONE;
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;

	i = CONTENT_WOOD_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("wood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_SLAB)+" 1";
	f->special_alternate_node = CONTENT_WOOD;
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->suffocation_per_second = 0;

	i = CONTENT_JUNGLE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_JUNGLE_SLAB)+" 1";
	f->special_alternate_node = CONTENT_JUNGLEWOOD;
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;

	i = CONTENT_BRICK_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	f->special_alternate_node = CONTENT_BRICK;
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;

	i = CONTENT_SANDSTONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_SLABLIKE;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->special_alternate_node = CONTENT_SANDSTONE;
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;

	i = CONTENT_GLASS_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab.png");
	f->setTexture(0,"glass.png");
	f->setTexture(1,"glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass.png", "glass_slab.png", "glass_slab.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;

	i = CONTENT_GLASS_BLUE_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Blue Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_BLUE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_blue.png");
	f->setTexture(0,"glass_blue.png");
	f->setTexture(1,"glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_blue.png", "glass_slab_blue.png", "glass_slab_blue.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;

	i = CONTENT_GLASS_GREEN_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Green Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_GREEN;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_green.png");
	f->setTexture(0,"glass_green.png");
	f->setTexture(1,"glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_green.png", "glass_slab_green.png", "glass_slab_green.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;

	i = CONTENT_GLASS_ORANGE_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Orange Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_ORANGE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_orange.png");
	f->setTexture(0,"glass_orange.png");
	f->setTexture(1,"glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_orange.png", "glass_slab_orange.png", "glass_slab_orange.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;

	i = CONTENT_GLASS_PURPLE_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Purple Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_PURPLE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_purple.png");
	f->setTexture(0,"glass_purple.png");
	f->setTexture(1,"glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_purple.png", "glass_slab_purple.png", "glass_slab_purple.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;

	i = CONTENT_GLASS_RED_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Red Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_RED;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_red.png");
	f->setTexture(0,"glass_red.png");
	f->setTexture(1,"glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_red.png", "glass_slab_red.png", "glass_slab_red.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;

	i = CONTENT_GLASS_YELLOW_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Yellow Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_YELLOW;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_yellow.png");
	f->setTexture(0,"glass_yellow.png");
	f->setTexture(1,"glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_yellow.png", "glass_slab_yellow.png", "glass_slab_yellow.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;

	i = CONTENT_GLASS_BLACK_SLAB_UD;
	f = &content_features(i);
	f->description = wgettext("Black Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->special_alternate_node = CONTENT_GLASS_BLACK;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_slab_black.png");
	f->setTexture(0,"glass_black.png");
	f->setTexture(1,"glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_black.png", "glass_slab_black.png", "glass_slab_black.png");
	f->type = CMT_GLASS;
	f->hardness = 0.15;
	f->suffocation_per_second = 0;
}
