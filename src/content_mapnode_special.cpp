/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mapnode_special.cpp
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
#include "player.h"
#include "intl.h"

void content_mapnode_special(bool repeat)
{
	content_t i;
	ContentFeatures *f = NULL;

	i = CONTENT_FENCE;
	f = &content_features(i);
	f->description = wgettext("Fence");
	f->setAllTextures("fence.png");
	f->setTexture(0,"fence_top.png");
	f->setTexture(1,"fence_top.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_FENCELIKE;
	f->is_ground_content = true;
	f->jumpable = false;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->special_alternate_node = CONTENT_WOOD;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_fence_inv(f);
	content_nodebox_fence(f);
	f->setInventoryTextureNodeBox(i,"fence.png","fence_top.png","fence.png");
	crafting::setWallRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_FENCE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_FENCE;
	f = &content_features(i);
	f->description = wgettext("Steel Fence");
	f->setAllTextures("fence_steel.png");
	f->setTexture(0,"fence_steel_top.png");
	f->setTexture(1,"fence_steel_top.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_FENCELIKE;
	f->is_ground_content = true;
	f->jumpable = false;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->special_alternate_node = CONTENT_STEEL;
	f->type = CMT_STONE;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	f->suffocation_per_second = 0;
	f->destructive_mob_safe = true;
	content_nodebox_fence_inv(f);
	content_nodebox_fence(f);
	f->setInventoryTextureNodeBox(i,"fence_steel.png","fence_steel_top.png","fence_steel.png");
	crafting::setWallRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_STEEL_FENCE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_JUNGLE_FENCE;
	f = &content_features(i);
	f->description = wgettext("Jungle Wood Fence");
	f->setAllTextures("fence_jungle.png");
	f->setTexture(0,"fence_jungle_top.png");
	f->setTexture(1,"fence_jungle_top.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_FENCELIKE;
	f->is_ground_content = true;
	f->jumpable = false;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->special_alternate_node = CONTENT_JUNGLEWOOD;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_fence_inv(f);
	content_nodebox_fence(f);
	f->setInventoryTextureNodeBox(i,"fence_jungle.png","fence_jungle_top.png","fence_jungle.png");
	crafting::setWallRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_JUNGLE_FENCE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_PINE_FENCE;
	f = &content_features(i);
	f->description = wgettext("Pine Fence");
	f->setAllTextures("fence_pine.png");
	f->setTexture(0,"fence_pine_top.png");
	f->setTexture(1,"fence_pine_top.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_FENCELIKE;
	f->is_ground_content = true;
	f->jumpable = false;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->special_alternate_node = CONTENT_WOOD_PINE;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_fence_inv(f);
	content_nodebox_fence(f);
	f->setInventoryTextureNodeBox(i,"fence_pine.png","fence_pine_top.png","fence_pine.png");
	crafting::setWallRecipe(CONTENT_CRAFTITEM_PINE_PLANK,CONTENT_PINE_FENCE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_BARS;
	f = &content_features(i);
	f->description = wgettext("Steel Bars");
	f->setAllTextures("steel_block.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->special_alternate_node = CONTENT_STEEL;
	f->type = CMT_STONE;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	f->suffocation_per_second = 0;
	content_nodebox_bars(f);
	f->setInventoryTextureNodeBox(i,"steel_block.png","steel_block.png","steel_block.png");
	crafting::setHatRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_STEEL_BARS);
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_IGNORE,	CONTENT_CRAFTITEM_STEEL_INGOT
		};
		crafting::setRecipe(r,CONTENT_STEEL_BARS,6);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_RAIL;
	f = &content_features(i);
	f->description = wgettext("Rail");
	f->setAllTextures("rail.png");
	f->setTexture(0,"track_tie.png");
	f->setTexture(1,"track_rail.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_RAILLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	f->suffocation_per_second = 0;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_IGNORE,			CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_CRAFTITEM_STEEL_INGOT
		};
		crafting::setRecipe(r,CONTENT_RAIL,15);
		r[1] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[7] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		crafting::setRecipe(r,CONTENT_RAIL,15);
	}
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,-0.5*BS,-0.5*BS,0.5*BS,-0.375*BS,0.5*BS
	));
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_TERRACOTTA;
	f = &content_features(i);
	f->description = wgettext("Terracotta Roof Tile");
	f->setAllTextures("rooftile_terracotta.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_TERRACOTTA,CONTENT_TERRACOTTA,CONTENT_ROOFTILE_TERRACOTTA);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_WOOD;
	f = &content_features(i);
	f->description = wgettext("Wood Roof Tile");
	f->setAllTextures("rooftile_wood.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_WOOD,CONTENT_WOOD,CONTENT_ROOFTILE_WOOD);
	crafting::set1over4Recipe(CONTENT_JUNGLEWOOD,CONTENT_JUNGLEWOOD,CONTENT_ROOFTILE_WOOD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_ASPHALT;
	f = &content_features(i);
	f->description = wgettext("Asphalt Roof Tile");
	f->setAllTextures("rooftile_asphalt.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	{
		u16 r[9] = {
			CONTENT_SAND,			CONTENT_CRAFTITEM_PAPER,	CONTENT_IGNORE,
			CONTENT_CRAFTITEM_PAPER,	CONTENT_CRAFTITEM_DYE_BLACK,	CONTENT_IGNORE,
			CONTENT_IGNORE,			CONTENT_IGNORE,			CONTENT_IGNORE,
		};
		crafting::setRecipe(r,CONTENT_ROOFTILE_ASPHALT,4);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_STONE;
	f = &content_features(i);
	f->description = wgettext("Stone Roof Tile");
	f->setAllTextures("rooftile_stone.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONE,CONTENT_ROOFTILE_STONE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_GLASS;
	f = &content_features(i);
	f->description = wgettext("Glass Roof Tile");
	f->setAllTextures("glass.png");
	f->setTexture(1,"glass_slab.png"); // special texture for top sections
	f->setAllTextureFlags(0);
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_GLASS;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_GLASS,CONTENT_GLASS,CONTENT_ROOFTILE_GLASS);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_GLASS_BLUE;
	f = &content_features(i);
	f->description = wgettext("Blue Glass Roof Tile");
	f->setAllTextures("glass_blue.png");
	f->setTexture(1,"glass_slab_blue.png"); // special texture for top sections
	f->setAllTextureFlags(0);
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_GLASS;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_GLASS_BLUE,CONTENT_GLASS_BLUE,CONTENT_ROOFTILE_GLASS_BLUE);
	crafting::set1Any2Recipe(CONTENT_ROOFTILE_GLASS,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_ROOFTILE_GLASS_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_GLASS_GREEN;
	f = &content_features(i);
	f->description = wgettext("Green Glass Roof Tile");
	f->setAllTextures("glass_green.png");
	f->setTexture(1,"glass_slab_green.png"); // special texture for top sections
	f->setAllTextureFlags(0);
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_GLASS;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_GLASS_GREEN,CONTENT_GLASS_GREEN,CONTENT_ROOFTILE_GLASS_GREEN);
	crafting::set1Any2Recipe(CONTENT_ROOFTILE_GLASS,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_ROOFTILE_GLASS_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_GLASS_ORANGE;
	f = &content_features(i);
	f->description = wgettext("Orange Glass Roof Tile");
	f->setAllTextures("glass_orange.png");
	f->setTexture(1,"glass_slab_orange.png"); // special texture for top sections
	f->setAllTextureFlags(0);
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_GLASS;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_GLASS_ORANGE,CONTENT_GLASS_ORANGE,CONTENT_ROOFTILE_GLASS_ORANGE);
	crafting::set1Any2Recipe(CONTENT_ROOFTILE_GLASS,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_ROOFTILE_GLASS_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_GLASS_PURPLE;
	f = &content_features(i);
	f->description = wgettext("Purple Glass Roof Tile");
	f->setAllTextures("glass_purple.png");
	f->setTexture(1,"glass_slab_purple.png"); // special texture for top sections
	f->setAllTextureFlags(0);
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_GLASS;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_GLASS_PURPLE,CONTENT_GLASS_PURPLE,CONTENT_ROOFTILE_GLASS_PURPLE);
	crafting::set1Any2Recipe(CONTENT_ROOFTILE_GLASS,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_ROOFTILE_GLASS_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_GLASS_RED;
	f = &content_features(i);
	f->description = wgettext("Red Glass Roof Tile");
	f->setAllTextures("glass_red.png");
	f->setTexture(1,"glass_slab_red.png"); // special texture for top sections
	f->setAllTextureFlags(0);
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_GLASS;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_GLASS_RED,CONTENT_GLASS_RED,CONTENT_ROOFTILE_GLASS_RED);
	crafting::set1Any2Recipe(CONTENT_ROOFTILE_GLASS,CONTENT_CRAFTITEM_DYE_RED,CONTENT_ROOFTILE_GLASS_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_GLASS_YELLOW;
	f = &content_features(i);
	f->description = wgettext("Yellow Glass Roof Tile");
	f->setAllTextures("glass_yellow.png");
	f->setTexture(1,"glass_slab_yellow.png"); // special texture for top sections
	f->setAllTextureFlags(0);
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_GLASS;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_GLASS_YELLOW,CONTENT_GLASS_YELLOW,CONTENT_ROOFTILE_GLASS_YELLOW);
	crafting::set1Any2Recipe(CONTENT_ROOFTILE_GLASS,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_ROOFTILE_GLASS_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_GLASS_BLACK;
	f = &content_features(i);
	f->description = wgettext("Black Glass Roof Tile");
	f->setAllTextures("glass_black.png");
	f->setTexture(1,"glass_slab_black.png"); // special texture for top sections
	f->setAllTextureFlags(0);
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_GLASS;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_GLASS_BLACK,CONTENT_GLASS_BLACK,CONTENT_ROOFTILE_GLASS_BLACK);
	crafting::set1Any2Recipe(CONTENT_ROOFTILE_GLASS,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_ROOFTILE_GLASS_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_THATCH;
	f = &content_features(i);
	f->description = wgettext("Thatch Roof Tile");
	f->setAllTextures("rooftile_thatch.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_roofcollide(f);
	crafting::set1over4Recipe(CONTENT_DEADGRASS,CONTENT_DEADGRASS,CONTENT_ROOFTILE_THATCH);
	crafting::set1over4Recipe(CONTENT_WILDGRASS_SHORT,CONTENT_WILDGRASS_SHORT,CONTENT_ROOFTILE_THATCH);
	crafting::set1over4Recipe(CONTENT_JUNGLEGRASS,CONTENT_JUNGLEGRASS,CONTENT_ROOFTILE_THATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LADDER_WALL;
	f = &content_features(i);
	f->description = wgettext("Ladder");
	f->setAllTextures("ladder.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_WALLMOUNT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem ")+itos(i)+" 1";
	f->solidness = 0;
	f->floormount_alternate_node = CONTENT_LADDER_FLOOR;
	f->roofmount_alternate_node = CONTENT_LADDER_ROOF;
	f->rotate_tile_with_nodebox = true;
	f->climbable = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->type = CMT_WOOD;
	f->hardness = 0.5;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,-0.5*BS,0.3125*BS,-0.3125*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,-0.5*BS,0.3125*BS,0.4375*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,-0.25*BS,0.375*BS,0.3125*BS,-0.1875*BS,0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,0.25*BS,0.375*BS,0.3125*BS,0.3125*BS,0.4375*BS
	));
	f->setInventoryTextureNodeBox(i,"ladder.png","ladder.png","ladder.png");
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_PINE_PLANK,
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_PINE_PLANK,
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_PINE_PLANK
		};
		crafting::setRecipe(r,CONTENT_LADDER_WALL,4);
		r[0] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[2] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[3] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[5] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[6] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[8] = CONTENT_CRAFTITEM_WOOD_PLANK;
		crafting::setRecipe(r,CONTENT_LADDER_WALL,4);
		r[0] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[2] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[3] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[5] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[6] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[8] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		crafting::setRecipe(r,CONTENT_LADDER_WALL,4);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LADDER_FLOOR;
	f = &content_features(i);
	f->description = wgettext("Ladder");
	f->setAllTextures("ladder.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem ")+itos(CONTENT_LADDER_WALL)+" 1";
	f->solidness = 0;
	f->wallmount_alternate_node = CONTENT_LADDER_WALL;
	f->roofmount_alternate_node = CONTENT_LADDER_ROOF;
	f->rotate_tile_with_nodebox = true;
	f->climbable = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->type = CMT_WOOD;
	f->hardness = 0.5;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,-0.5*BS,-0.5*BS,-0.3125*BS,-0.3125*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,-0.5*BS,-0.5*BS,0.4375*BS,-0.3125*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,-0.4375*BS,-0.3125*BS,0.3125*BS,-0.375*BS,-0.25*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,-0.4375*BS,0.1875*BS,0.3125*BS,-0.375*BS,0.25*BS
	));

	i = CONTENT_LADDER_ROOF;
	f = &content_features(i);
	f->description = wgettext("Ladder");
	f->setAllTextures("ladder.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem ")+itos(CONTENT_LADDER_WALL)+" 1";
	f->solidness = 0;
	f->floormount_alternate_node = CONTENT_LADDER_FLOOR;
	f->wallmount_alternate_node = CONTENT_LADDER_WALL;
	f->rotate_tile_with_nodebox = true;
	f->climbable = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->type = CMT_WOOD;
	f->hardness = 0.5;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,0.3125*BS,-0.5*BS,-0.3125*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,0.3125*BS,-0.5*BS,0.4375*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,0.375*BS,-0.3125*BS,0.3125*BS,0.4375*BS,-0.25*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,0.375*BS,0.1875*BS,0.3125*BS,0.4375*BS,0.25*BS
	));

	i = CONTENT_BORDERSTONE;
	f = &content_features(i);
	f->description = wgettext("Border Stone");
	f->setAllTextures("borderstone.png");
	f->setInventoryTextureCube("borderstone.png", "borderstone.png", "borderstone.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new BorderStoneNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 2.0;
	f->pressure_type = CST_SOLID;
	crafting::setFilledRoundRecipe(CONTENT_STONE,CONTENT_CRAFTITEM_MITHRIL_RAW,CONTENT_BORDERSTONE);
	crafting::setFilledRoundRecipe(CONTENT_STONE,CONTENT_CRAFTITEM_MITHRIL_UNBOUND,CONTENT_BORDERSTONE);
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_STONE,			CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_STONE,			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_STONE,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_STONE,			CONTENT_CRAFTITEM_STEEL_INGOT
		};
		crafting::setRecipe(r,CONTENT_BORDERSTONE,1);
	}
	lists::add("craftguide",i);
	lists::add("player-creative",i);
	lists::add("creative",i);

	i = CONTENT_BOOK;
	f = &content_features(i);
	f->description = wgettext("Book");
	f->setTexture(0, "book_cover.png");
	f->setTexture(1, "book_cover.png^[transformFX");
	f->setTexture(2, "book_side.png^[transformFY");
	f->setTexture(3, "book_side.png");
	f->setTexture(4, "book_end.png");
	f->setTexture(5, "book_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_nodebox_book(f);
	f->setFaceText(0,FaceText(0.45,0.3,0.9875,0.7375));
	f->setInventoryTextureNodeBox(i, "book_cover.png", "book_end.png^[transformFX", "book_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::setCol1Recipe(CONTENT_CRAFTITEM_PAPER,i);
	lists::add("craftguide",i);
	lists::add("creative",i);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();

	i = CONTENT_COOK_BOOK;
	f = &content_features(i);
	f->description = wgettext("Cook Book");
	f->setTexture(0, "book_cook_cover.png");
	f->setTexture(1, "book_cook_cover.png^[transformFX");
	f->setTexture(2, "book_cook_side.png^[transformFY");
	f->setTexture(3, "book_cook_side.png");
	f->setTexture(4, "book_cook_end.png");
	f->setTexture(5, "book_cook_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_COOK_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_cook_cover.png", "book_cook_end.png^[transformFX", "book_cook_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_COAL,i);
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_CHARCOAL,i);
	lists::add("craftguide",i);
	lists::add("creative",i);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();

	i = CONTENT_DECRAFT_BOOK;
	f = &content_features(i);
	f->description = wgettext("Decraft Book");
	f->setTexture(0, "book_decraft_cover.png");
	f->setTexture(1, "book_decraft_cover.png^[transformFX");
	f->setTexture(2, "book_decraft_side.png^[transformFY");
	f->setTexture(3, "book_decraft_side.png");
	f->setTexture(4, "book_decraft_end.png");
	f->setTexture(5, "book_decraft_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_DECRAFT_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_decraft_cover.png", "book_decraft_end.png^[transformFX", "book_decraft_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_GUNPOWDER,i);
	lists::add("craftguide",i);
	lists::add("creative",i);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();

	i = CONTENT_DIARY_BOOK;
	f = &content_features(i);
	f->description = wgettext("Diary");
	f->setTexture(0, "book_diary_cover.png");
	f->setTexture(1, "book_diary_cover.png^[transformFX");
	f->setTexture(2, "book_diary_side.png^[transformFY");
	f->setTexture(3, "book_diary_side.png");
	f->setTexture(4, "book_diary_end.png");
	f->setTexture(5, "book_diary_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_DIARY_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_nodebox_book(f);
	f->setFaceText(0,FaceText(0.45,0.3,0.9875,0.7375));
	f->setInventoryTextureNodeBox(i, "book_diary_cover.png", "book_diary_end.png^[transformFX", "book_diary_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_STEEL_INGOT,i);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFT_BOOK;
	f = &content_features(i);
	f->description = wgettext("Craft Book");
	f->setTexture(0, "book_craft_cover.png");
	f->setTexture(1, "book_craft_cover.png^[transformFX");
	f->setTexture(2, "book_craft_side.png^[transformFY");
	f->setTexture(3, "book_craft_side.png");
	f->setTexture(4, "book_craft_end.png");
	f->setTexture(5, "book_craft_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_CRAFT_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_craft_cover.png", "book_craft_end.png^[transformFX", "book_craft_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_MITHRIL_RAW,i);
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_MITHRIL_UNBOUND,i);
	{
		content_t r[9] = {
			CONTENT_CRAFTITEM_STICK, CONTENT_CRAFTITEM_STICK, CONTENT_IGNORE,
			CONTENT_IGNORE,		 CONTENT_CRAFTITEM_STICK, CONTENT_IGNORE,
			CONTENT_CRAFTITEM_STICK, CONTENT_IGNORE,	  CONTENT_IGNORE
		};
		crafting::setRecipe(r,CONTENT_CRAFT_BOOK,1);
	}
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_RCRAFT_BOOK;
	f = &content_features(i);
	f->description = wgettext("Reverse Craft Book");
	f->setTexture(0, "book_rcraft_cover.png");
	f->setTexture(1, "book_rcraft_cover.png^[transformFX");
	f->setTexture(2, "book_rcraft_side.png^[transformFY");
	f->setTexture(3, "book_rcraft_side.png");
	f->setTexture(4, "book_rcraft_end.png");
	f->setTexture(5, "book_rcraft_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_RCRAFT_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_rcraft_cover.png", "book_rcraft_end.png^[transformFX", "book_rcraft_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	{
		content_t r[9] = {
			CONTENT_CRAFTITEM_STICK, CONTENT_IGNORE,          CONTENT_IGNORE,
			CONTENT_IGNORE,          CONTENT_CRAFTITEM_STICK, CONTENT_IGNORE,
			CONTENT_CRAFTITEM_STICK, CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE
		};
		crafting::setRecipe(r,CONTENT_RCRAFT_BOOK,1);
	}
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BOOK_OPEN;
	f = &content_features(i);
	f->description = wgettext("Guide");
	f->setAllTextures("guide_side.png");
	f->setTexture(0, "guide_top.png");
	f->setTexture(1, "guide_bottom.png");
	f->setTexture(4, "guide_end.png");
	f->setTexture(5, "guide_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BOOK)+" 1";
	f->solidness = 0;
	content_nodebox_guide(f);
	f->setFaceText(0,FaceText(0.15,0.35,0.85,0.85,FTT_BOOKCONTENT));
	f->setInventoryTextureNodeBox(i, "guide_top.png", "guide_end.png", "guide_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new BookNodeMetadata();
	f->sound_access = "open-book";

	i = CONTENT_COOK_BOOK_OPEN;
	f = &content_features(i);
	f->description = wgettext("Cooking Guide");
	f->setAllTextures("guide_cook_side.png");
	f->setTexture(0, "guide_cook_top.png");
	f->setTexture(1, "guide_cook_bottom.png");
	f->setTexture(4, "guide_cook_end.png");
	f->setTexture(5, "guide_cook_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_COOK_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_COOK_BOOK)+" 1";
	f->solidness = 0;
	content_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_cook_top.png", "guide_cook_end.png", "guide_cook_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CookBookNodeMetadata();
	f->sound_access = "open-book";

	i = CONTENT_DECRAFT_BOOK_OPEN;
	f = &content_features(i);
	f->description = wgettext("Decrafting Guide");
	f->setAllTextures("guide_decraft_side.png");
	f->setTexture(0, "guide_decraft_top.png");
	f->setTexture(1, "guide_decraft_bottom.png");
	f->setTexture(4, "guide_decraft_end.png");
	f->setTexture(5, "guide_decraft_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_DECRAFT_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_DECRAFT_BOOK)+" 1";
	f->solidness = 0;
	content_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_decraft_top.png", "guide_decraft_end.png", "guide_decraft_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DeCraftNodeMetadata();
	f->sound_access = "open-book";

	i = CONTENT_DIARY_BOOK_OPEN;
	f = &content_features(i);
	f->description = wgettext("Diary");
	f->setAllTextures("guide_diary_side.png");
	f->setTexture(0, "guide_diary_top.png");
	f->setTexture(1, "guide_diary_bottom.png");
	f->setTexture(4, "guide_diary_end.png");
	f->setTexture(5, "guide_diary_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_DIARY_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_DIARY_BOOK)+" 1";
	f->solidness = 0;
	content_nodebox_guide(f);
	f->setFaceText(0,FaceText(0.15,0.35,0.85,0.85,FTT_BOOKCONTENT));
	f->setInventoryTextureNodeBox(i, "guide_diary_top.png", "guide_diary_end.png", "guide_diary_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DiaryNodeMetadata();
	f->sound_access = "open-book";

	i = CONTENT_CRAFT_BOOK_OPEN;
	f = &content_features(i);
	f->description = wgettext("Craft Guide");
	f->setAllTextures("guide_craft_side.png");
	f->setTexture(0, "guide_craft_top.png");
	f->setTexture(1, "guide_craft_bottom.png");
	f->setTexture(4, "guide_craft_end.png");
	f->setTexture(5, "guide_craft_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_CRAFT_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CRAFT_BOOK)+" 1";
	f->solidness = 0;
	content_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_craft_top.png", "guide_craft_end.png", "guide_craft_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CraftGuideNodeMetadata();
	f->sound_access = "open-book";

	i = CONTENT_RCRAFT_BOOK_OPEN;
	f = &content_features(i);
	f->description = wgettext("Reverse Craft Guide");
	f->setAllTextures("guide_rcraft_side.png");
	f->setTexture(0, "guide_rcraft_top.png");
	f->setTexture(1, "guide_rcraft_bottom.png");
	f->setTexture(4, "guide_rcraft_end.png");
	f->setTexture(5, "guide_rcraft_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_RCRAFT_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_RCRAFT_BOOK)+" 1";
	f->solidness = 0;
	content_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_rcraft_top.png", "guide_rcraft_end.png", "guide_rcraft_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ReverseCraftGuideNodeMetadata();
	f->sound_access = "open-book";

	i = CONTENT_FIRE;
	f = &content_features(i);
	f->description = wgettext("Fire");
	f->setAllTextures("fire.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_FIRELIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 8;
	f->sound_ambient = "env-fire";
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif
	f->pressure_type = CST_CRUSHED;

	i = CONTENT_FIRE_SHORTTERM;
	f = &content_features(i);
	f->description = wgettext("Fire");
	f->setAllTextures("fire.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_FIRELIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 8;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif
	f->pressure_type = CST_CRUSHED;

	i = CONTENT_TORCH;
	f = &content_features(i);
	f->description = wgettext("Torch");
	f->setAllTextures("torch.png");
	f->setInventoryTexture("torch_inventory.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_WALLMOUNT;
	f->draw_type = CDT_TORCHLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 0.5;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_source = LIGHT_MAX-1;
	f->type = CMT_WOOD;
	f->hardness = 0.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	crafting::set1over4Recipe(CONTENT_CRAFTITEM_COAL,CONTENT_CRAFTITEM_STICK,CONTENT_TORCH);
	crafting::set1over4Recipe(CONTENT_CRAFTITEM_CHARCOAL,CONTENT_CRAFTITEM_STICK,CONTENT_TORCH);
	lists::add("craftguide",i);
	lists::add("player-creative",i);
	lists::add("creative",i);

	i = CONTENT_SIGN_WALL;
	f = &content_features(i);
	f->description = wgettext("Sign");
	f->setAllTextures("sign_wall.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_WALLMOUNT;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->floormount_alternate_node = CONTENT_SIGN;
	f->roofmount_alternate_node = CONTENT_SIGN_UD;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->alternate_lockstate_node = CONTENT_LOCKABLE_SIGN_WALL;
	content_nodebox_sign_wall(f);
	f->setFaceText(5,FaceText(0.05,0.3,0.95,0.7));

	i = CONTENT_SIGN;
	f = &content_features(i);
	f->description = wgettext("Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back.png");
	f->setTexture(5, "sign_front.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->wallmount_alternate_node = CONTENT_SIGN_WALL;
	f->roofmount_alternate_node = CONTENT_SIGN_UD;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->alternate_lockstate_node = CONTENT_LOCKABLE_SIGN;
	content_nodebox_sign(f);
	f->setFaceText(5,FaceText(0.05,0.0675,0.95,0.55));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_front.png", "sign.png");
	crafting::setSignRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_SIGN);
	crafting::setSignRecipe(CONTENT_CRAFTITEM_PINE_PLANK,CONTENT_SIGN);
	crafting::setSignRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_SIGN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SIGN_UD;
	f = &content_features(i);
	f->description = wgettext("Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back_ud.png");
	f->setTexture(5, "sign_front_ud.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->wallmount_alternate_node = CONTENT_SIGN_WALL;
	f->floormount_alternate_node = CONTENT_SIGN;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->alternate_lockstate_node = CONTENT_LOCKABLE_SIGN_UD;
	content_nodebox_sign_ud(f);
	f->setFaceText(5,FaceText(0.05,0.45,0.95,0.8875));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_front.png", "sign.png");

	i = CONTENT_LOCKABLE_SIGN_WALL;
	f = &content_features(i);
	f->description = wgettext("Locking Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back.png");
	f->setTexture(5, "sign_wall_lock.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_WALLMOUNT;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->floormount_alternate_node = CONTENT_LOCKABLE_SIGN;
	f->roofmount_alternate_node = CONTENT_LOCKABLE_SIGN_UD;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_LOCKABLE_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingSignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->alternate_lockstate_node = CONTENT_SIGN_WALL;
	content_nodebox_sign_wall(f);
	f->setFaceText(4,FaceText(0.05,0.3,0.95,0.7,FTT_OWNER));
	f->setFaceText(5,FaceText(0.05,0.3,0.95,0.7));

	i = CONTENT_LOCKABLE_SIGN;
	f = &content_features(i);
	f->description = wgettext("Locking Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back.png");
	f->setTexture(5, "sign_lock.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->wallmount_alternate_node = CONTENT_LOCKABLE_SIGN_WALL;
	f->roofmount_alternate_node = CONTENT_LOCKABLE_SIGN_UD;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingSignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->alternate_lockstate_node = CONTENT_SIGN;
	content_nodebox_sign(f);
	f->setFaceText(4,FaceText(0.05,0.0675,0.95,0.55,FTT_OWNER));
	f->setFaceText(5,FaceText(0.05,0.0675,0.95,0.55));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_lock.png", "sign.png");
	crafting::set1Any2Recipe(CONTENT_SIGN,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_SIGN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LOCKABLE_SIGN_UD;
	f = &content_features(i);
	f->description = wgettext("Locking Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back_ud.png");
	f->setTexture(5, "sign_lock_ud.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->wallmount_alternate_node = CONTENT_LOCKABLE_SIGN_WALL;
	f->floormount_alternate_node = CONTENT_LOCKABLE_SIGN;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_LOCKABLE_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingSignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	f->alternate_lockstate_node = CONTENT_SIGN_UD;
	content_nodebox_sign_ud(f);
	f->setFaceText(4,FaceText(0.05,0.45,0.95,0.8875,FTT_OWNER));
	f->setFaceText(5,FaceText(0.05,0.45,0.95,0.8875));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_lock.png", "sign.png");

	i = CONTENT_CHEST;
	f = &content_features(i);
	f->description = wgettext("Chest");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("chest_side.png");
	f->setTexture(0, "chest_top.png");
	f->setTexture(1, "chest_top.png");
	f->setTexture(5, "chest_front.png"); // Z-
	f->setInventoryTexture("chest_top.png");
	f->setInventoryTextureCube("chest_top.png", "chest_front.png", "chest_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new ChestNodeMetadata();
	f->sound_access = "open-chest";
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->alternate_lockstate_node = CONTENT_LOCKABLE_CHEST;
	crafting::setRoundRecipe(CONTENT_WOOD,CONTENT_CHEST);
	crafting::setRoundRecipe(CONTENT_JUNGLEWOOD,CONTENT_CHEST);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CREATIVE_CHEST;
	f = &content_features(i);
	f->description = wgettext("Creative Chest");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("chest_side.png");
	f->setTexture(0, "chest_top.png");
	f->setTexture(1, "chest_top.png");
	f->setTexture(5, "chest_creative.png"); // Z-
	f->setInventoryTexture("chest_top.png");
	f->setInventoryTextureCube("chest_top.png", "chest_creative.png", "chest_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new CreativeChestNodeMetadata();
	f->sound_access = "open-chest";
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	lists::add("player-creative",i);
	lists::add("creative",i);

	i = CONTENT_LOCKABLE_CHEST;
	f = &content_features(i);
	f->description = wgettext("Locking Chest");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("chest_side.png");
	f->setTexture(0, "chest_top.png");
	f->setTexture(1, "chest_top.png");
	f->setTexture(5, "chest_lock.png"); // Z-
	f->setInventoryTexture("chest_lock.png");
	f->setInventoryTextureCube("chest_top.png", "chest_lock.png", "chest_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingChestNodeMetadata();
	f->sound_access = "open-chest";
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->alternate_lockstate_node = CONTENT_CHEST;
	crafting::setFilledRoundRecipe(CONTENT_WOOD,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_JUNGLEWOOD,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::set1Any2Recipe(CONTENT_CHEST,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_WOOD,CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_JUNGLEWOOD,CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::set1Any2Recipe(CONTENT_CHEST,CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_WOOD,CONTENT_CRAFTITEM_SILVER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_JUNGLEWOOD,CONTENT_CRAFTITEM_SILVER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::set1Any2Recipe(CONTENT_CHEST,CONTENT_CRAFTITEM_SILVER_INGOT,CONTENT_LOCKABLE_CHEST);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SAFE;
	f = &content_features(i);
	f->description = wgettext("Safe");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("safe_side.png");
	f->setTexture(0, "safe_top.png");
	f->setTexture(1, "safe_top.png");
	f->setTexture(5, "safe_lock.png"); // Z-
	f->setInventoryTexture("safe_lock.png");
	f->setInventoryTextureCube("safe_top.png", "safe_lock.png", "safe_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SafeNodeMetadata();
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->destructive_mob_safe = true;
	crafting::setFilledRoundRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_SAFE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FURNACE;
	f = &content_features(i);
	f->description = wgettext("Furnace");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX_META;
	f->setAllTextures("furnace_side.png");
	f->setTexture(0, "furnace_top.png");
	f->setTexture(1, "furnace_top.png^[transformFY");
	f->setTexture(2, "furnace_side.png^[transformFX");
	f->setTexture(4, "furnace_back.png");
	f->setTexture(5, "furnace_front.png"); // Z-
	f->setAllMetaTextures("fire.png");
	content_nodebox_furnace(f);
	f->setInventoryTextureNodeBox(i,"furnace_top.png^[transformR90", "furnace_front.png", "furnace_side.png^[transformFX");
	f->rotate_tile_with_nodebox = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FurnaceNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 3.0;
	f->pressure_type = CST_SOLID;
	f->alternate_lockstate_node = CONTENT_LOCKABLE_FURNACE;
	crafting::setRoundRecipe(CONTENT_ROUGHSTONE,CONTENT_FURNACE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FURNACE_ACTIVE;
	f = &content_features(i);
	f->description = wgettext("Furnace");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->light_source = 9;
	f->setAllTextures("furnace_side.png");
	f->setTexture(0, "furnace_top.png");
	f->setTexture(1, "furnace_top.png");
	f->setTexture(5, "furnace_front.png"); // Z-
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_FURNACE)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FurnaceNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 3.0;
	f->pressure_type = CST_SOLID;

	i = CONTENT_LOCKABLE_FURNACE;
	f = &content_features(i);
	f->description = wgettext("Locking Furnace");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX_META;
	f->setAllTextures("furnace_side.png");
	f->setTexture(0, "furnace_top.png");
	f->setTexture(1, "furnace_top.png^[transformFY");
	f->setTexture(2, "furnace_side.png^[transformFX");
	f->setTexture(4, "furnace_back.png");
	f->setTexture(5, "furnace_lock.png"); // Z-
	f->setAllMetaTextures("fire.png");
	content_nodebox_lockedfurnace(f);
	f->setInventoryTextureNodeBox(i,"furnace_top.png^[transformR90", "furnace_lock.png", "furnace_side.png^[transformFX");
	f->rotate_tile_with_nodebox = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingFurnaceNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 3.0;
	f->pressure_type = CST_SOLID;
	f->alternate_lockstate_node = CONTENT_FURNACE;
	crafting::setFilledRoundRecipe(CONTENT_ROUGHSTONE,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_FURNACE);
	crafting::set1Any2Recipe(CONTENT_FURNACE,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_FURNACE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LOCKABLE_FURNACE_ACTIVE;
	f = &content_features(i);
	f->description = wgettext("Locking Furnace");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->light_source = 9;
	f->setAllTextures("furnace_side.png");
	f->setTexture(0, "furnace_top.png");
	f->setTexture(1, "furnace_top.png");
	f->setTexture(5, "furnace_lock_active.png"); // Z-
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_LOCKABLE_FURNACE)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingFurnaceNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 3.0;
	f->pressure_type = CST_SOLID;

	i = CONTENT_INCINERATOR;
	f = &content_features(i);
	f->description = wgettext("Incinerator");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX_META;
	f->setAllTextures("incinerator_side.png");
	f->setTexture(0, "incinerator_top.png");
	f->setTexture(1, "incinerator_top.png^[transformFY");
	f->setTexture(2, "incinerator_side.png^[transformFX");
	f->setTexture(4, "incinerator_back.png");
	f->setTexture(5, "incinerator_front.png"); // Z-
	f->setAllMetaTextures("fire.png");
	content_nodebox_incinerator(f);
	f->setInventoryTextureNodeBox(i,"incinerator_top.png^[transformR90", "incinerator_front.png", "incinerator_side.png^[transformFX");
	f->rotate_tile_with_nodebox = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new IncineratorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 0.4;
	f->pressure_type = CST_SOLID;
	crafting::setFilledRoundRecipe(CONTENT_ROUGHSTONE,CONTENT_CRAFTITEM_MITHRIL_UNBOUND,CONTENT_INCINERATOR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_INCINERATOR_ACTIVE;
	f = &content_features(i);
	f->description = wgettext("Incinerator");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->light_source = 9;
	f->setAllTextures("incinerator_side.png");
	f->setTexture(0, "incinerator_top.png"); // Z-
	f->setTexture(1, "incinerator_top.png"); // Z-
	f->setTexture(5, "incinerator_front_active.png"); // Z-
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_INCINERATOR)+" 1";
	if (f->initial_metadata == NULL)
		f->initial_metadata = new IncineratorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 0.4;
	f->pressure_type = CST_SOLID;

	i = CONTENT_NC;
	f = &content_features(i);
	f->description = wgettext("Nyan Cat");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("nc_side.png");
	f->setTexture(5, "nc_front.png"); // Z-
	f->setTexture(4, "nc_back.png"); // Z+
	f->setInventoryTextureCube("nc_side.png", "nc_front.png", "nc_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 3.0;
	lists::add("creative",i);

	i = CONTENT_NC_RB;
	f = &content_features(i);
	f->description = wgettext("Rainbow");
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("nc_rb.png");
	f->setInventoryTextureCube("nc_rb.png", "nc_rb.png", "nc_rb.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 3.0;
	lists::add("creative",i);

	i = CONTENT_FLOWER_POT_RAW;
	f = &content_features(i);
	f->param_type = CPT_LIGHT;
	f->description = wgettext("Unbaked Flower Pot");
	f->setAllTextures("flower_pot_raw.png");
	f->setTexture(0,"flower_pot_raw_top.png");
	f->setTexture(1,"flower_pot_raw_bottom.png");
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_FLOWER_POT)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.75;
	f->pressure_type = CST_CRUSHABLE;
	content_nodebox_flower_pot(f);
	f->setInventoryTextureNodeBox(i,"flower_pot_raw_top.png","flower_pot_raw.png","flower_pot_raw.png");
	crafting::setVRecipe(CONTENT_CRAFTITEM_CLAY,CONTENT_FLOWER_POT_RAW);
	lists::add("craftguide",i);
	lists::add("cooking",i);

	i = CONTENT_FLOWER_POT;
	f = &content_features(i);
	f->param_type = CPT_LIGHT;
	f->description = wgettext("Flower Pot");
	f->setAllTextures("flower_pot.png");
	f->setTexture(0,"flower_pot_top.png");
	f->setTexture(1,"flower_pot_bottom.png");
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.75;
	content_nodebox_flower_pot(f);
	f->setInventoryTextureNodeBox(i,"flower_pot_top.png","flower_pot.png","flower_pot.png");
	lists::add("creative",i);

	// walls
	i = CONTENT_COBBLE_WALL;
	f = &content_features(i);
	f->description = wgettext("Cobblestone Wall");
	f->setAllTextures("cobble.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_wall_inv(f);
	content_nodebox_wall(f);
	f->setInventoryTextureNodeBox(i,"cobble.png","cobble.png","cobble.png");
	f->special_alternate_node = CONTENT_COBBLE;
	f->type = CMT_STONE;
	f->hardness = 0.9;
	f->suffocation_per_second = 0;
	crafting::setWallRecipe(CONTENT_COBBLE,CONTENT_COBBLE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROUGHSTONE_WALL;
	f = &content_features(i);
	f->description = wgettext("Rough Stone Wall");
	f->setAllTextures("roughstone.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_wall_inv(f);
	content_nodebox_wall(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png","roughstone.png","roughstone.png");
	f->special_alternate_node = CONTENT_ROUGHSTONE;
	f->type = CMT_STONE;
	f->hardness = 0.9;
	f->suffocation_per_second = 0;
	crafting::setWallRecipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MOSSYCOBBLE_WALL;
	f = &content_features(i);
	f->description = wgettext("Mossy Cobblestone Wall");
	f->setAllTextures("mossycobble.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_wall_inv(f);
	content_nodebox_wall(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png","mossycobble.png","mossycobble.png");
	f->special_alternate_node = CONTENT_MOSSYCOBBLE;
	f->type = CMT_STONE;
	f->hardness = 0.8;
	f->suffocation_per_second = 0;
	crafting::setWallRecipe(CONTENT_MOSSYCOBBLE,CONTENT_MOSSYCOBBLE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONE_WALL;
	f = &content_features(i);
	f->description = wgettext("Stone Wall");
	f->setAllTextures("stone.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_wall_inv(f);
	content_nodebox_wall(f);
	f->setInventoryTextureNodeBox(i,"stone.png","stone.png","stone.png");
	f->special_alternate_node = CONTENT_STONE;
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	crafting::setWallRecipe(CONTENT_STONE,CONTENT_STONE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SANDSTONE_WALL;
	f = &content_features(i);
	f->description = wgettext("Sand Stone Wall");
	f->setAllTextures("sandstone.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_wall_inv(f);
	content_nodebox_wall(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png","sandstone.png","sandstone.png");
	f->special_alternate_node = CONTENT_SANDSTONE;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	crafting::setWallRecipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LIMESTONE_WALL;
	f = &content_features(i);
	f->description = wgettext("Limestone Wall");
	f->setAllTextures("limestone.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_SPECIAL;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_wall_inv(f);
	content_nodebox_wall(f);
	f->setInventoryTextureNodeBox(i,"limestone.png","limestone.png","limestone.png");
	f->special_alternate_node = CONTENT_LIMESTONE;
	f->type = CMT_STONE;
	f->hardness = 0.9;
	f->suffocation_per_second = 0;
	crafting::setWallRecipe(CONTENT_LIMESTONE,CONTENT_LIMESTONE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TNT;
	f = &content_features(i);
	f->description = wgettext("TNT");
	f->setAllTextures("tnt.png");
	f->setTexture(0, "tnt_top.png");
	f->setTexture(1, "tnt_bottom.png");
	f->setInventoryTextureCube("tnt_top.png", "tnt.png", "tnt.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->energy_type = CET_CONDUCTIVE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new TNTNodeMetadata();
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_TNT,CONTENT_TNT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLASH;
	f = &content_features(i);
	f->description = wgettext("In-Progress explosion - how did you get this???");
	f->setAllTextures("flash.png");
	f->solidness = 0; // drawn separately, makes no faces
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 50;
	f->pressure_type = CST_CRUSHED;
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
	f->post_effect_color = video::SColor(192, 255, 255, 64);
#endif

	i = CONTENT_STEAM;
	f = &content_features(i);
	f->description = wgettext("Steam");
	f->setAllTextures("steam.png");
	f->solidness = 0;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->warmth_per_second = 4;
	f->pressure_type = CST_CRUSHED;
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
	f->post_effect_color = video::SColor(120, 200, 200, 200);
#endif

	// flags
	i = CONTENT_FLAG;
	f = &content_features(i);
	f->description = wgettext("Home Flag");
	f->setAllTextures("flag.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_FLAGLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_flag(f);
	f->home_node = PLAYERFLAG_WHITE;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->suffocation_per_second = 0;
	{
		content_t r[9] = {
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_PAPER,	CONTENT_IGNORE,
			CONTENT_CRAFTITEM_STICK,	CONTENT_IGNORE,			CONTENT_IGNORE,
			CONTENT_IGNORE,			CONTENT_IGNORE,			CONTENT_IGNORE
		};
		crafting::setRecipe(r,CONTENT_FLAG,1);
	}
	crafting::set1Any2Recipe(CONTENT_FLAG_BLUE,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_BLUE,CONTENT_CRAFTITEM_STARCH,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_GREEN,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_GREEN,CONTENT_CRAFTITEM_STARCH,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_ORANGE,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_ORANGE,CONTENT_CRAFTITEM_STARCH,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_PURPLE,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_PURPLE,CONTENT_CRAFTITEM_STARCH,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_RED,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_RED,CONTENT_CRAFTITEM_STARCH,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_YELLOW,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_YELLOW,CONTENT_CRAFTITEM_STARCH,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_BLACK,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_FLAG);
	crafting::set1Any2Recipe(CONTENT_FLAG_BLACK,CONTENT_CRAFTITEM_STARCH,CONTENT_FLAG);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FlagNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLAG_BLUE;
	f = &content_features(i);
	f->description = wgettext("Blue Home Flag");
	f->setAllTextures("flag_blue.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_FLAGLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_flag(f);
	f->home_node = PLAYERFLAG_BLUE;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_FLAG,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_FLAG_BLUE);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FlagNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLAG_GREEN;
	f = &content_features(i);
	f->description = wgettext("Green Home Flag");
	f->setAllTextures("flag_green.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_FLAGLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_flag(f);
	f->home_node = PLAYERFLAG_GREEN;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_FLAG,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_FLAG_GREEN);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FlagNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLAG_ORANGE;
	f = &content_features(i);
	f->description = wgettext("Orange Home Flag");
	f->setAllTextures("flag_orange.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_FLAGLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_flag(f);
	f->home_node = PLAYERFLAG_ORANGE;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_FLAG,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_FLAG_ORANGE);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FlagNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLAG_PURPLE;
	f = &content_features(i);
	f->description = wgettext("Purple Home Flag");
	f->setAllTextures("flag_purple.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_FLAGLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_flag(f);
	f->home_node = PLAYERFLAG_PURPLE;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_FLAG,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_FLAG_PURPLE);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FlagNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLAG_RED;
	f = &content_features(i);
	f->description = wgettext("Red Home Flag");
	f->setAllTextures("flag_red.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_FLAGLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_flag(f);
	f->home_node = PLAYERFLAG_RED;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_FLAG,CONTENT_CRAFTITEM_DYE_RED,CONTENT_FLAG_RED);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FlagNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLAG_YELLOW;
	f = &content_features(i);
	f->description = wgettext("Yellow Home Flag");
	f->setAllTextures("flag_yellow.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_FLAGLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_flag(f);
	f->home_node = PLAYERFLAG_YELLOW;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_FLAG,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_FLAG_YELLOW);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FlagNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLAG_BLACK;
	f = &content_features(i);
	f->description = wgettext("Black Home Flag");
	f->setAllTextures("flag_black.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_FLAGLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	content_nodebox_flag(f);
	f->home_node = PLAYERFLAG_BLACK;
	f->type = CMT_WOOD;
	f->hardness = 0.4;
	f->suffocation_per_second = 0;
	crafting::set1Any2Recipe(CONTENT_FLAG,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_FLAG_BLACK);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FlagNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LIFE_SUPPORT;
	f = &content_features(i);
	f->description = wgettext("Life Support System");
	f->setAllTextures("life_support.png");
	f->setTexture(0, "life_support_top.png");
	f->setTexture(1, "life_support_bottom.png");
	f->setInventoryTextureCube("life_support_top.png", "life_support.png", "life_support.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	{
		content_t r[9] = {
			CONTENT_IGNORE,				CONTENT_SPONGE,				CONTENT_IGNORE,
			CONTENT_CRAFTITEM_MITHRIL_UNBOUND,	CONTENT_IGNORE,				CONTENT_CRAFTITEM_MITHRIL_UNBOUND,
			CONTENT_CRAFTITEM_MITHRIL_UNBOUND,	CONTENT_CRAFTITEM_MITHRIL_UNBOUND,	CONTENT_CRAFTITEM_MITHRIL_UNBOUND
		};
		crafting::setRecipe(r,CONTENT_LIFE_SUPPORT,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_PARCEL;
	f = &content_features(i);
	f->description = wgettext("Parcel");
	f->setAllTextures("parcel.png");
	f->setTexture(0, "parcel_top.png");
	f->setTexture(1, "parcel_bottom.png");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->walkable = false;
	f->param_type = CPT_LIGHT;
	f->rotate_tile_with_nodebox = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->buildable_to = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->borderstone_diggable = true;
	content_nodebox_parcel(f);
	f->setInventoryTextureNodeBox(i,"parcel_top.png", "parcel.png", "parcel.png");
	f->type = CMT_DIRT;
	f->hardness = 0.01;
	f->suffocation_per_second = 0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ParcelNodeMetadata();

	i = CONTENT_CAULDRON;
	f = &content_features(i);
	f->description = wgettext("Cauldron");
	f->setAllTextures("cauldron_outer.png");
	f->setAllMetaTextures("cauldron_inner.png");
	f->setMetaTexture(0,"water.png");
	f->draw_type = CDT_NODEBOX_META;
	f->type = CMT_STONE;
	f->hardness = 3.0;
	f->solidness = 0;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_cauldron(f);
	f->setInventoryTextureNodeBox(i,"cauldron_outer.png", "cauldron_outer.png", "cauldron_outer.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CauldronNodeMetadata();
	crafting::setDeepURecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_CAULDRON);
	f->pressure_type = CST_SOLID;
	lists::add("craftguide",i);
	lists::add("creative",i);
	f->suffocation_per_second = 0;

	i = CONTENT_FORGE;
	f = &content_features(i);
	f->description = wgettext("Forge");
	f->setAllTextures("forge_side.png");
	f->setTexture(0,"forge_top.png");
	f->setTexture(1,"forge_bottom.png");
	f->draw_type = CDT_NODEBOX;
	f->type = CMT_STONE;
	f->hardness = 3.0;
	f->solidness = 0;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_forge(f);
	f->setInventoryTextureNodeBox(i,"forge_top.png", "forge_side.png", "forge_side.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ForgeNodeMetadata();
	{
		content_t r[9] = {
			CONTENT_ROUGHSTONE,	CONTENT_CHARCOAL,	CONTENT_ROUGHSTONE,
			CONTENT_ROUGHSTONE,	CONTENT_SAND,		CONTENT_ROUGHSTONE,
			CONTENT_ROUGHSTONE,	CONTENT_STEEL,		CONTENT_ROUGHSTONE
		};
		crafting::setRecipe(r,CONTENT_FORGE,1);
	}
	f->pressure_type = CST_SOLID;
	lists::add("craftguide",i);
	lists::add("creative",i);
	f->suffocation_per_second = 0;

	i = CONTENT_FORGE_FIRE;
	f = &content_features(i);
	f->description = wgettext("Forge Fire");
	f->setAllTextures("forge_fire.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-4;
	f->solidness = 0; // Drawn separately, makes no faces
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->sound_ambient = "env-fire";
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif
	f->pressure_type = CST_CRUSHED;
}
