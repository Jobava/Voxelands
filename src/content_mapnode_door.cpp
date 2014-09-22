/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mapnode_door.cpp
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

void content_mapnode_door(bool repeat)
{
	content_t i;
	ContentFeatures *f = NULL;

	// doors
	i = CONTENT_WOOD_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(2,"door_wood_b.png^[transformFX");
	f->setTexture(3,"door_wood_b.png^[transformFX");
	f->setTexture(4,"door_wood_b.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	i = CONTENT_WOOD_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(2,"door_wood_t.png^[transformFX");
	f->setTexture(3,"door_wood_t.png^[transformFX");
	f->setTexture(4,"door_wood_t.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	crafting::set1over1Recipe(CONTENT_WOOD_HATCH,CONTENT_WOOD_HATCH,CONTENT_WOOD_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_WOOD_DOOR_RT,CONTENT_WOOD_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(2,"door_steel_b.png^[transformFX");
	f->setTexture(3,"door_steel_b.png^[transformFX");
	f->setTexture(4,"door_steel_b.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_nodebox_door(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	i = CONTENT_STEEL_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_t.png");
	f->setTexture(2,"door_steel_t.png^[transformFX");
	f->setTexture(3,"door_steel_t.png^[transformFX");
	f->setTexture(4,"door_steel_t.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_door(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	crafting::set1over1Recipe(CONTENT_STEEL_HATCH,CONTENT_STEEL_HATCH,CONTENT_STEEL_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_STEEL_DOOR_RT,CONTENT_STEEL_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(2,"door_glass_b.png^[transformFX");
	f->setTexture(3,"door_glass_b.png^[transformFX");
	f->setTexture(4,"door_glass_b.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->pressure_type = CST_SOLID;
	i = CONTENT_GLASS_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(2,"door_glass_t.png^[transformFX");
	f->setTexture(3,"door_glass_t.png^[transformFX");
	f->setTexture(4,"door_glass_t.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->pressure_type = CST_SOLID;
	crafting::set1over1Recipe(CONTENT_GLASS_PANE,CONTENT_GLASS_PANE,CONTENT_GLASS_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_GLASS_DOOR_RT,CONTENT_GLASS_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_W_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(2,"door_wood_wb.png^[transformFX");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setTexture(4,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	i = CONTENT_WOOD_W_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(2,"door_wood_wt.png^[transformFX");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setTexture(4,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorw(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	crafting::set1over1Recipe(CONTENT_WOOD_W_HATCH,CONTENT_WOOD_HATCH,CONTENT_WOOD_W_DOOR_LT);
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_WOOD_DOOR_LT,CONTENT_WOOD_W_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_WOOD_W_DOOR_RT,CONTENT_WOOD_W_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_W_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(2,"door_steel_wb.png^[transformFX");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setTexture(4,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_nodebox_door(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	i = CONTENT_STEEL_W_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(2,"door_steel_wt.png^[transformFX");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setTexture(4,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_doorw(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	crafting::set1over1Recipe(CONTENT_STEEL_W_HATCH,CONTENT_STEEL_HATCH,CONTENT_STEEL_W_DOOR_LT);
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_STEEL_DOOR_LT,CONTENT_STEEL_W_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_STEEL_W_DOOR_RT,CONTENT_STEEL_W_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// right doors
	i = CONTENT_WOOD_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_b.png^[transformFX");
	f->setTexture(2,"door_wood_b.png");
	f->setTexture(3,"door_wood_b.png");
	f->setTexture(4,"door_wood_b.png");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	i = CONTENT_WOOD_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_t.png^[transformFX");
	f->setTexture(2,"door_wood_t.png");
	f->setTexture(3,"door_wood_t.png");
	f->setTexture(4,"door_wood_t.png");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	crafting::set1To1Recipe(CONTENT_WOOD_DOOR_LT,CONTENT_WOOD_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_b.png^[transformFX");
	f->setTexture(2,"door_glass_b.png");
	f->setTexture(3,"door_glass_b.png");
	f->setTexture(4,"door_glass_b.png");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->pressure_type = CST_SOLID;
	i = CONTENT_GLASS_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_t.png^[transformFX");
	f->setTexture(2,"door_glass_t.png");
	f->setTexture(3,"door_glass_t.png");
	f->setTexture(4,"door_glass_t.png");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->pressure_type = CST_SOLID;
	crafting::set1To1Recipe(CONTENT_GLASS_DOOR_LT,CONTENT_GLASS_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png^[transformFX");
	f->setTexture(2,"door_steel_b.png");
	f->setTexture(3,"door_steel_b.png");
	f->setTexture(4,"door_steel_b.png");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_nodebox_door(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	i = CONTENT_STEEL_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_t.png^[transformFX");
	f->setTexture(2,"door_steel_t.png");
	f->setTexture(3,"door_steel_t.png");
	f->setTexture(4,"door_steel_t.png");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_door(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	crafting::set1To1Recipe(CONTENT_GLASS_DOOR_LT,CONTENT_GLASS_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_W_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png^[transformFX");
	f->setTexture(2,"door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png");
	f->setTexture(4,"door_wood_wb.png");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	i = CONTENT_WOOD_W_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png^[transformFX");
	f->setTexture(2,"door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png");
	f->setTexture(4,"door_wood_wt.png");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorw(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	crafting::set1To1Recipe(CONTENT_WOOD_W_DOOR_LT,CONTENT_WOOD_W_DOOR_RT);
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_WOOD_DOOR_RT,CONTENT_WOOD_W_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_W_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png^[transformFX");
	f->setTexture(2,"door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png");
	f->setTexture(4,"door_steel_wb.png");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_nodebox_door(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	i = CONTENT_STEEL_W_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png^[transformFX");
	f->setTexture(2,"door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png");
	f->setTexture(4,"door_steel_wt.png");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_doorw(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	crafting::set1To1Recipe(CONTENT_STEEL_W_DOOR_LT,CONTENT_STEEL_W_DOOR_RT);
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_STEEL_DOOR_RT,CONTENT_STEEL_W_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// open doors
	i = CONTENT_WOOD_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(3,"door_wood_b.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	i = CONTENT_WOOD_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(3,"door_wood_t.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;

	i = CONTENT_GLASS_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(3,"door_glass_b.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->pressure_type = CST_SOLID;
	i = CONTENT_GLASS_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(3,"door_glass_t.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->pressure_type = CST_SOLID;

	i = CONTENT_STEEL_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_b.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_LT)+" 1";
	content_nodebox_doorol(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	i = CONTENT_STEEL_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_t.png");
	f->setTexture(3,"door_steel_t.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_LT)+" 1";
	content_nodebox_doorol(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();

	i = CONTENT_WOOD_W_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	i = CONTENT_WOOD_W_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorwol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;

	i = CONTENT_STEEL_W_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_LT)+" 1";
	content_nodebox_doorol(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	i = CONTENT_STEEL_W_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_LT)+" 1";
	content_nodebox_doorwol(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();

	// right open doors
	i = CONTENT_WOOD_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(3,"door_wood_b.png^[transformFX");
	f->setTexture(4,"door_wood_b.png^[transformFX");
	f->setTexture(5,"door_wood_b.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	i = CONTENT_WOOD_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(3,"door_wood_t.png^[transformFX");
	f->setTexture(4,"door_wood_t.png^[transformFX");
	f->setTexture(5,"door_wood_t.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;

	i = CONTENT_GLASS_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(2,"door_glass_b.png^[transformFX");
	f->setTexture(3,"door_glass_b.png^[transformFX");
	f->setTexture(4,"door_glass_b.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->pressure_type = CST_SOLID;
	i = CONTENT_GLASS_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(2,"door_glass_t.png^[transformFX");
	f->setTexture(3,"door_glass_t.png^[transformFX");
	f->setTexture(4,"door_glass_t.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->pressure_type = CST_SOLID;

	i = CONTENT_STEEL_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_b.png^[transformFX");
	f->setTexture(4,"door_steel_b.png^[transformFX");
	f->setTexture(5,"door_steel_b.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_RT)+" 1";
	content_nodebox_dooror(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	i = CONTENT_STEEL_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_t.png^[transformFX");
	f->setTexture(4,"door_steel_t.png^[transformFX");
	f->setTexture(5,"door_steel_t.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_RT)+" 1";
	content_nodebox_dooror(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();

	i = CONTENT_WOOD_W_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setTexture(4,"door_wood_wb.png^[transformFX");
	f->setTexture(5,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	i = CONTENT_WOOD_W_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setTexture(4,"door_wood_wt.png^[transformFX");
	f->setTexture(5,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_doorwor(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;

	i = CONTENT_STEEL_W_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setTexture(4,"door_steel_wb.png^[transformFX");
	f->setTexture(5,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_RT)+" 1";
	content_nodebox_dooror(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	i = CONTENT_STEEL_W_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setTexture(4,"door_steel_wt.png^[transformFX");
	f->setTexture(5,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_RT)+" 1";
	content_nodebox_doorwor(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();

	// hatches
	i = CONTENT_WOOD_HATCH;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_inv.png");
	f->wield_nodebox = false;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_hatch(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_WOOD_HATCH);
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_WOOD_HATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_HATCH;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_inv.png");
	f->wield_nodebox = false;
	f->air_equivalent = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_hatch(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_STEEL_HATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_W_HATCH;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Hatch");
	f->setAllTextures("hatch_wood_w.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_w_inv.png");
	f->wield_nodebox = false;
	f->air_equivalent = true;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_hatchw(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_WOOD_HATCH,CONTENT_WOOD_W_HATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_W_HATCH;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Hatch");
	f->setAllTextures("hatch_steel_w.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_w_inv.png");
	f->wield_nodebox = false;
	f->air_equivalent = true;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_hatchw(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_STEEL_HATCH,CONTENT_STEEL_W_HATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// gates
	i = CONTENT_WOOD_GATE;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Gate");
	f->setAllTextures("gate_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_wood_inv.png");
	f->wield_nodebox = false;
	f->air_equivalent = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_gate(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;
	crafting::setGateRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_WOOD,CONTENT_WOOD_GATE);
	crafting::setGateRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_JUNGLEWOOD,CONTENT_WOOD_GATE);
	crafting::setGateRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_JUNGLEWOOD,CONTENT_WOOD_GATE);
	crafting::setGateRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_WOOD,CONTENT_WOOD_GATE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_GATE;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Gate");
	f->setAllTextures("gate_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_steel_inv.png");
	f->wield_nodebox = false;
	f->air_equivalent = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_nodebox_gate(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
	crafting::setGateRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_STEEL,CONTENT_STEEL_GATE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// open hatches
	i = CONTENT_WOOD_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood.png");
	f->setTexture(2,"hatch_wood.png^[transformR90");
	f->setTexture(3,"hatch_wood.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_inv.png");
	f->wield_nodebox = false;
	f->air_equivalent = true;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_HATCH)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_hatcho(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;

	i = CONTENT_STEEL_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel.png");
	f->setTexture(2,"hatch_steel.png^[transformR90");
	f->setTexture(3,"hatch_steel.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_inv.png");
	f->wield_nodebox = false;
	f->air_equivalent = true;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_HATCH)+" 1";
	content_nodebox_hatcho(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();

	i = CONTENT_WOOD_W_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood_w.png");
	f->setTexture(2,"hatch_wood.png^[transformR90");
	f->setTexture(3,"hatch_wood.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->air_equivalent = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_HATCH)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_hatchwo(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;

	i = CONTENT_STEEL_W_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel_w.png");
	f->setTexture(2,"hatch_steel.png^[transformR90");
	f->setTexture(3,"hatch_steel.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_HATCH)+" 1";
	content_nodebox_hatchwo(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();

	// open gates
	i = CONTENT_WOOD_GATE_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Gate");
	f->setAllTextures("gate_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_GATE)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_nodebox_gateo(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	f->pressure_type = CST_SOLID;

	i = CONTENT_STEEL_GATE_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Gate");
	f->setAllTextures("gate_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_GATE)+" 1";
	content_nodebox_gateo(f);
	f->type = CMT_STONE;
	f->hardness = 5.0;
	f->pressure_type = CST_SOLID;
	f->energy_type = CET_CONDUCTIVE;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DoorNodeMetadata();
}
