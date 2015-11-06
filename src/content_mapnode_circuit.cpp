/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_mapnode_circuit.cpp
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
#include "intl.h"

void content_mapnode_circuit(bool repeat)
{
	content_t i;
	ContentFeatures *f = NULL;

	i = CONTENT_CIRCUIT_MITHRILWIRE;
	f = &content_features(i);
	f->description = wgettext("Mithril Wire");
	f->setAllTextures("mithril_wire.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WIRELIKE;
	f->energy_type = CET_CONDUCTIVE;
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true; // grass grows underneath
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("CraftItem2 ")+itos(CONTENT_CRAFTITEM_MITHRILDUST)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_carpet(f);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CircuitNodeMetadata();
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CIRCUIT_COPPERWIRE;
	f = &content_features(i);
	f->description = wgettext("Copper Wire");
	f->setAllTextures("copper_wire.png");
	f->setInventoryTexture("copper_wire_inv.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_3DWIRELIKE;
	f->energy_type = CET_CONDUCTIVE;
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->air_equivalent = true; // grass grows underneath
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ") + itos(i)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	f->pressure_type = CST_CRUSHABLE;
	f->suffocation_per_second = 0;
	content_nodebox_carpet(f);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CircuitNodeMetadata();
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_RESIN,	CONTENT_CRAFTITEM_COPPER_INGOT,	CONTENT_IGNORE,
			CONTENT_IGNORE,			CONTENT_IGNORE,			CONTENT_IGNORE,
			CONTENT_IGNORE,			CONTENT_IGNORE,			CONTENT_IGNORE
		};
		crafting::setShapelessRecipe(r,CONTENT_CIRCUIT_COPPERWIRE,10);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_REACTOR;
	f = &content_features(i);
	f->description = wgettext("Reactor");
	f->setAllTextures("circuit_reactor.png");
	f->setTexture(0,"circuit_reactor_top.png");
	f->setTexture(1,"circuit_reactor_bottom.png");
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_SOURCE;
	f->energy_drop = 0;
	f->light_source = 10;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	f->suffocation_per_second = 0;
	content_nodebox_battery(f);
	f->setInventoryTextureNodeBox(i,"circuit_reactor_top.png","circuit_reactor.png","circuit_reactor.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new SourceNodeMetadata();
	crafting::setFilledRoundRecipe(CONTENT_CRAFTITEM_MITHRIL_UNBOUND,CONTENT_CRAFTITEM_QUARTZ,CONTENT_CIRCUIT_REACTOR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_SOLARPANEL;
	f = &content_features(i);
	f->description = wgettext("Solar Panel");
	f->setAllTextures("wood.png");
	f->setTexture(0,"circuit_solarpanel_top.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_SOURCE;
	f->energy_drop = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	f->suffocation_per_second = 0;
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"circuit_solarpanel_top.png","wood.png","wood.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new SolarPanelNodeMetadata();
	{
		u16 r[9] = {
			CONTENT_GLASS,			CONTENT_GLASS,			CONTENT_GLASS,
			CONTENT_CRAFTITEM_QUARTZ_DUST,	CONTENT_CRAFTITEM_MITHRILDUST,	CONTENT_CRAFTITEM_QUARTZ_DUST,
			CONTENT_WOOD_SLAB,		CONTENT_WOOD_SLAB,		CONTENT_WOOD_SLAB
		};
		crafting::setRecipe(r,CONTENT_CIRCUIT_SOLARPANEL,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_WATERWHEEL;
	f = &content_features(i);
	f->description = wgettext("Water Wheel");
	f->setAllTextures("circuit_waterwheel.png");
	f->setTexture(2,"circuit_waterwheel_side.png");
	f->setTexture(3,"circuit_waterwheel_side.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_SWITCH;
	f->energy_drop = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->rotate_tile_with_nodebox = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	content_nodebox_waterwheel(f);
	f->setInventoryTextureNodeBox(i,"circuit_waterwheel.png","circuit_waterwheel.png","circuit_waterwheel_side.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new WaterWheelNodeMetadata();
	{
		u16 r[9] = {
			CONTENT_STONE,			CONTENT_WOOD_SLAB,		CONTENT_STONE,
			CONTENT_CRAFTITEM_MITHRILDUST,	CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_CRAFTITEM_QUARTZ_DUST,
			CONTENT_STONE,			CONTENT_WOOD_SLAB,		CONTENT_STONE
		};
		crafting::setRecipe(r,CONTENT_CIRCUIT_WATERWHEEL,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_SWITCH;
	f = &content_features(i);
	f->description = wgettext("Switch");
	f->setAllTextures("stone.png");
	f->setTexture(5,"circuit_switch_front.png");
	f->setAllMetaTextures("stone.png");
	f->setMetaTexture(5,"circuit_switch_active_front.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_WALLMOUNT;
	f->draw_type = CDT_NODEBOX_META;
	f->rotate_tile_with_nodebox = true;
	f->air_equivalent = true;
	f->light_propagates = true;
	f->is_ground_content = true;
	f->energy_type = CET_SWITCH;
	f->energy_drop = 0;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	content_nodebox_switch(f);
	f->setInventoryTextureNodeBox(i,"stone.png","circuit_switch_front.png","stone.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new SwitchNodeMetadata();
	{
		u16 recipe[9] = {
			CONTENT_IGNORE, CONTENT_CRAFTITEM_MITHRILDUST, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_STONE, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_CRAFTITEM_QUARTZ_DUST, CONTENT_IGNORE
		};
		crafting::setRecipe(recipe,CONTENT_CIRCUIT_SWITCH,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_BUTTON;
	f = &content_features(i);
	f->description = wgettext("Button");
	f->setAllTextures("stone.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_WALLMOUNT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->air_equivalent = true;
	f->light_propagates = true;
	f->energy_type = CET_SWITCH;
	f->energy_drop = 0;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	content_nodebox_button(f);
	f->setInventoryTextureNodeBox(i,"stone.png","stone.png","stone.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ButtonNodeMetadata();
	{
		u16 recipe[9] = {
			CONTENT_IGNORE, CONTENT_CRAFTITEM_MITHRILDUST, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_STONE_KNOB, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_CRAFTITEM_QUARTZ_DUST, CONTENT_IGNORE
		};
		crafting::setRecipe(recipe,CONTENT_CIRCUIT_BUTTON,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_PRESSUREPLATE_STONE;
	f = &content_features(i);
	f->description = wgettext("Stone Pressure Plate");
	f->setAllTextures("stone.png");
	f->setTexture(0,"pressureplate_stone_top.png");
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->energy_type = CET_SWITCH;
	f->energy_drop = 0;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"pressureplate_stone_top.png","stone.png","stone.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ButtonNodeMetadata();
	{
		u16 recipe[9] = {
			CONTENT_IGNORE, CONTENT_CRAFTITEM_MITHRILDUST, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_STONE_SLAB, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_CRAFTITEM_QUARTZ_DUST, CONTENT_IGNORE
		};
		crafting::setRecipe(recipe,CONTENT_CIRCUIT_PRESSUREPLATE_STONE,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_PRESSUREPLATE_WOOD;
	f = &content_features(i);
	f->description = wgettext("Wood Pressure Plate");
	f->setAllTextures("stone.png");
	f->setTexture(0,"pressureplate_wood_top.png");
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->energy_type = CET_SWITCH;
	f->energy_drop = 0;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	content_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"pressureplate_wood_top.png","stone.png","stone.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ButtonNodeMetadata();
	{
		u16 recipe[9] = {
			CONTENT_IGNORE, CONTENT_CRAFTITEM_MITHRILDUST, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_WOOD_SLAB, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_CRAFTITEM_QUARTZ_DUST, CONTENT_IGNORE
		};
		crafting::setRecipe(recipe,CONTENT_CIRCUIT_PRESSUREPLATE_WOOD,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_NOTGATE;
	f = &content_features(i);
	f->description = wgettext("Not Gate");
	f->setAllTextures("circuit_gate.png");
	f->setTexture(0,"circuit_gate_top.png");
	f->rotate_tile_with_nodebox = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_GATE;
	f->energy_drop = 0;
	f->light_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	content_nodebox_logicgate(f);
	f->setInventoryTextureNodeBox(i,"circuit_gate_top.png","circuit_gate.png","circuit_gate.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new NotGateNodeMetadata();
	{
		u16 recipe[9] = {
			CONTENT_IGNORE, CONTENT_CRAFTITEM_MITHRILDUST, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_STONE, CONTENT_IGNORE,
			CONTENT_IGNORE, CONTENT_IGNORE, CONTENT_IGNORE
		};
		crafting::setRecipe(recipe,CONTENT_CIRCUIT_NOTGATE,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_REPEATER;
	f = &content_features(i);
	f->description = wgettext("Repeater");
	f->setAllTextures("circuit_repeater.png");
	f->setTexture(0,"circuit_repeater_top.png");
	f->rotate_tile_with_nodebox = true;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_GATE;
	f->energy_drop = 0;
	f->light_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->suffocation_per_second = 0;
	content_nodebox_logicgate(f);
	f->setInventoryTextureNodeBox(i,"circuit_repeater_top.png","circuit_repeater.png","circuit_repeater.png");
	if (f->initial_metadata == NULL)
		f->initial_metadata = new RepeaterNodeMetadata();
	{
		u16 recipe[9] = {
			CONTENT_IGNORE,			CONTENT_IGNORE,	CONTENT_IGNORE,
			CONTENT_CRAFTITEM_MITHRILDUST,	CONTENT_STONE,	CONTENT_CRAFTITEM_MITHRILDUST,
			CONTENT_IGNORE,			CONTENT_IGNORE,	CONTENT_IGNORE
		};
		crafting::setRecipe(recipe,CONTENT_CIRCUIT_REPEATER,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_LAMP;
	f = &content_features(i);
	f->description = wgettext("Electric Lamp");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_CUBELIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->energy_type = CET_CONDUCTIVE;
	f->light_source = LIGHT_MAX-1;
	f->unpowered_node = CONTENT_CIRCUIT_LAMP_OFF;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_LAMP_OFF)+" 1";
	f->setAllTextures("circuit_lamp_on.png");
	f->setInventoryTextureCube("circuit_lamp.png", "circuit_lamp.png", "circuit_lamp.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CircuitNodeMetadata();

	i = CONTENT_CIRCUIT_LAMP_OFF;
	f = &content_features(i);
	f->description = wgettext("Electric Lamp");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->energy_type = CET_CONDUCTIVE;
	f->powered_node = CONTENT_CIRCUIT_LAMP;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->setAllTextures("circuit_lamp.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("circuit_lamp.png", "circuit_lamp.png", "circuit_lamp.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CircuitNodeMetadata();
	{
		u16 recipe[9] = {
			CONTENT_GLASS, CONTENT_IGNORE, CONTENT_GLASS,
			CONTENT_IGNORE, CONTENT_CRAFTITEM_QUARTZ, CONTENT_IGNORE,
			CONTENT_GLASS, CONTENT_IGNORE, CONTENT_GLASS
		};
		crafting::setRecipe(recipe,CONTENT_CIRCUIT_LAMP_OFF,4);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	// regular piston
	i = CONTENT_CIRCUIT_PISTON_OFF;
	f = &content_features(i);
	f->description = wgettext("Piston");
	f->is_ground_content = true;
	f->visual_solidness = 1;
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->energy_type = CET_CONDUCTIVE;
	f->floormount_alternate_node = CONTENT_CIRCUIT_PISTON_UP_OFF;
	f->roofmount_alternate_node = CONTENT_CIRCUIT_PISTON_DOWN_OFF;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->setAllTextures("circuit_piston.png");
	f->setTexture(0,"circuit_piston.png^[transformR90");
	f->setTexture(1,"circuit_piston.png^[transformR270");
	f->setTexture(3,"circuit_piston.png^[transformFX");
	f->setTexture(4,"circuit_piston_bottom.png");
	f->setTexture(5,"circuit_piston_arm_top.png");
	f->setInventoryTextureCube("circuit_piston.png^[transformR90", "circuit_piston_arm_top.png", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();
	{
		u16 recipe[9] = {
			CONTENT_WOOD,	CONTENT_ROUGHSTONE,		CONTENT_ROUGHSTONE,
			CONTENT_WOOD,	CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_CRAFTITEM_MITHRILDUST,
			CONTENT_WOOD,	CONTENT_ROUGHSTONE,		CONTENT_ROUGHSTONE
		};
		crafting::setRecipe(recipe,CONTENT_CIRCUIT_PISTON_OFF,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_PISTON;
	f = &content_features(i);
	f->description = wgettext("Piston");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_CONDUCTIVE;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_PISTON_OFF)+" 1";
	f->onact_also_affects = v3s16(0,0,1);
	f->setAllTextures("circuit_piston_active.png");
	f->setTexture(0,"circuit_piston_active.png^[transformR90");
	f->setTexture(1,"circuit_piston_active.png^[transformR270");
	f->setTexture(3,"circuit_piston_active.png^[transformFX");
	f->setTexture(4,"circuit_piston_bottom.png");
	f->setTexture(5,"circuit_piston_top.png");
	content_nodebox_piston(f);
	f->setInventoryTextureCube("circuit_piston.png^[transformR90", "circuit_piston_top.png", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_PISTON_ARM;
	f = &content_features(i);
	f->description = wgettext("Piston Arm");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->air_equivalent = true;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_PISTON_OFF)+" 1";
	f->onact_also_affects = v3s16(0,0,-1);
	f->setAllTextures("circuit_piston_arm.png");
	f->setTexture(0,"circuit_piston_arm.png^[transformR90");
	f->setTexture(1,"circuit_piston_arm.png^[transformR270");
	f->setTexture(3,"circuit_piston_arm.png^[transformFX");
	f->setTexture(4,"circuit_piston_arm_bottom.png");
	f->setTexture(5,"circuit_piston_arm_top.png");
	content_nodebox_pistonarm(f);
	f->setInventoryTextureCube("circuit_piston_arm.png", "circuit_piston_arm_top.png", "circuit_piston_arm.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->suffocation_per_second = 0;

	// push up
	i = CONTENT_CIRCUIT_PISTON_UP_OFF;
	f = &content_features(i);
	f->description = wgettext("Piston");
	f->is_ground_content = true;
	f->visual_solidness = 1;
	f->draw_type = CDT_CUBELIKE;
	f->energy_type = CET_CONDUCTIVE;
	f->wallmount_alternate_node = CONTENT_CIRCUIT_PISTON_OFF;
	f->roofmount_alternate_node = CONTENT_CIRCUIT_PISTON_DOWN_OFF;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON_UP_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_PISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston.png^[transformR270");
	f->setTexture(0,"circuit_piston_arm_top.png");
	f->setTexture(1,"circuit_piston_bottom.png");
	f->setInventoryTextureCube("circuit_piston_arm_top.png", "circuit_piston.png^[transformR90", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_PISTON_UP;
	f = &content_features(i);
	f->description = wgettext("Piston");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_CONDUCTIVE;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON_UP_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_PISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston_active.png^[transformR270");
	f->setTexture(0,"circuit_piston_top.png");
	f->setTexture(1,"circuit_piston_bottom.png");
	f->onact_also_affects = v3s16(0,1,0);
	content_nodebox_piston_up(f);
	f->setInventoryTextureCube("circuit_piston_top.png", "circuit_piston.png^[transformR90", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_PISTON_UP_ARM;
	f = &content_features(i);
	f->description = wgettext("Piston Arm");
	f->is_ground_content = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->draw_type = CDT_NODEBOX;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON_UP;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_PISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston_arm.png^[transformR270");
	f->setTexture(0,"circuit_piston_arm_top.png");
	f->setTexture(1,"circuit_piston_arm_bottom.png");
	f->onact_also_affects = v3s16(0,-1,0);
	content_nodebox_pistonarm_up(f);
	f->setInventoryTextureCube("circuit_piston_arm_top.png", "circuit_piston_arm.png", "circuit_piston_arm.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->suffocation_per_second = 0;

	// push down
	i = CONTENT_CIRCUIT_PISTON_DOWN_OFF;
	f = &content_features(i);
	f->description = wgettext("Piston");
	f->is_ground_content = true;
	f->visual_solidness = 1;
	f->draw_type = CDT_CUBELIKE;
	f->energy_type = CET_CONDUCTIVE;
	f->floormount_alternate_node = CONTENT_CIRCUIT_PISTON_UP_OFF;
	f->wallmount_alternate_node = CONTENT_CIRCUIT_PISTON_OFF;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON_DOWN_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_PISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston.png^[transformR90");
	f->setTexture(0,"circuit_piston_bottom.png");
	f->setTexture(1,"circuit_piston_arm_top.png");
	f->setInventoryTextureCube("circuit_piston_arm_bottom.png", "circuit_piston.png^[transformR90", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_PISTON_DOWN;
	f = &content_features(i);
	f->description = wgettext("Piston");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_CONDUCTIVE;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON_DOWN_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_PISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston_active.png^[transformR90");
	f->setTexture(0,"circuit_piston_bottom.png");
	f->setTexture(1,"circuit_piston_top.png");
	f->onact_also_affects = v3s16(0,-1,0);
	content_nodebox_piston_down(f);
	f->setInventoryTextureCube("circuit_piston_top.png", "circuit_piston.png^[transformR90", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_PISTON_DOWN_ARM;
	f = &content_features(i);
	f->description = wgettext("Piston Arm");
	f->is_ground_content = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->draw_type = CDT_NODEBOX;
	f->special_alternate_node = CONTENT_CIRCUIT_PISTON_DOWN;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_PISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston_arm.png^[transformR90");
	f->setTexture(0,"circuit_piston_arm_bottom.png");
	f->setTexture(1,"circuit_piston_arm_top.png");
	f->onact_also_affects = v3s16(0,1,0);
	content_nodebox_pistonarm_down(f);
	f->setInventoryTextureCube("circuit_piston_arm.png", "circuit_piston_arm_top.png", "circuit_piston_arm.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->suffocation_per_second = 0;

	// sticky piston
	i = CONTENT_CIRCUIT_STICKYPISTON_OFF;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston");
	f->is_ground_content = true;
	f->visual_solidness = 1;
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->energy_type = CET_CONDUCTIVE;
	f->floormount_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_UP_OFF;
	f->roofmount_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_DOWN_OFF;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->setAllTextures("circuit_piston.png");
	f->setTexture(0,"circuit_piston.png^[transformR90");
	f->setTexture(1,"circuit_piston.png^[transformR270");
	f->setTexture(3,"circuit_piston.png^[transformFX");
	f->setTexture(4,"circuit_piston_bottom.png");
	f->setTexture(5,"circuit_stickypiston_arm_top.png");
	f->setInventoryTextureCube("circuit_piston.png^[transformR90", "circuit_stickypiston_arm_top.png", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_RESIN,CONTENT_CIRCUIT_PISTON_OFF,CONTENT_CIRCUIT_STICKYPISTON_OFF);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CIRCUIT_STICKYPISTON;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_CONDUCTIVE;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_STICKYPISTON_OFF)+" 1";
	f->onact_also_affects = v3s16(0,0,1);
	f->setAllTextures("circuit_piston_active.png");
	f->setTexture(0,"circuit_piston_active.png^[transformR90");
	f->setTexture(1,"circuit_piston_active.png^[transformR270");
	f->setTexture(3,"circuit_piston_active.png^[transformFX");
	f->setTexture(4,"circuit_piston_bottom.png");
	f->setTexture(5,"circuit_piston_top.png");
	content_nodebox_piston(f);
	f->setInventoryTextureCube("circuit_piston.png^[transformR90", "circuit_piston_top.png", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_STICKYPISTON_ARM;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston Arm");
	f->is_ground_content = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_STICKYPISTON_OFF)+" 1";
	f->onact_also_affects = v3s16(0,0,-1);
	f->setAllTextures("circuit_piston_arm.png");
	f->setTexture(0,"circuit_piston_arm.png^[transformR90");
	f->setTexture(1,"circuit_piston_arm.png^[transformR270");
	f->setTexture(3,"circuit_piston_arm.png^[transformFX");
	f->setTexture(4,"circuit_piston_arm_bottom.png");
	f->setTexture(5,"circuit_stickypiston_arm_top.png");
	content_nodebox_pistonarm(f);
	f->setInventoryTextureCube("circuit_piston_arm.png", "circuit_stickypiston_arm_top.png", "circuit_piston_arm.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->suffocation_per_second = 0;

	// push up
	i = CONTENT_CIRCUIT_STICKYPISTON_UP_OFF;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston");
	f->is_ground_content = true;
	f->visual_solidness = 1;
	f->draw_type = CDT_CUBELIKE;
	f->energy_type = CET_CONDUCTIVE;
	f->wallmount_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_OFF;
	f->roofmount_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_DOWN_OFF;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_UP_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_STICKYPISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston.png^[transformR270");
	f->setTexture(0,"circuit_stickypiston_arm_top.png");
	f->setTexture(1,"circuit_piston_bottom.png");
	f->setInventoryTextureCube("circuit_stickypiston_arm_top.png", "circuit_piston.png^[transformR90", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_STICKYPISTON_UP;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_CONDUCTIVE;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_UP_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_STICKYPISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston_active.png^[transformR270");
	f->setTexture(0,"circuit_piston_top.png");
	f->setTexture(1,"circuit_piston_bottom.png");
	f->onact_also_affects = v3s16(0,1,0);
	content_nodebox_piston_up(f);
	f->setInventoryTextureCube("circuit_piston_top.png", "circuit_piston.png^[transformR90", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_STICKYPISTON_UP_ARM;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston Arm");
	f->is_ground_content = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->draw_type = CDT_NODEBOX;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_UP;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_STICKYPISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston_arm.png^[transformR270");
	f->setTexture(0,"circuit_stickypiston_arm_top.png");
	f->setTexture(1,"circuit_piston_arm_bottom.png");
	f->onact_also_affects = v3s16(0,-1,0);
	content_nodebox_pistonarm_up(f);
	f->setInventoryTextureCube("circuit_stickypiston_arm_top.png", "circuit_piston_arm.png", "circuit_piston_arm.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->suffocation_per_second = 0;

	// push down
	i = CONTENT_CIRCUIT_STICKYPISTON_DOWN_OFF;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston");
	f->is_ground_content = true;
	f->visual_solidness = 1;
	f->draw_type = CDT_CUBELIKE;
	f->energy_type = CET_CONDUCTIVE;
	f->floormount_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_UP_OFF;
	f->wallmount_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_OFF;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_DOWN_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_STICKYPISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston.png^[transformR90");
	f->setTexture(0,"circuit_piston_bottom.png");
	f->setTexture(1,"circuit_stickypiston_arm_top.png");
	f->setInventoryTextureCube("circuit_piston_arm_bottom.png", "circuit_piston.png^[transformR90", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_STICKYPISTON_DOWN;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston");
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->draw_type = CDT_NODEBOX;
	f->energy_type = CET_CONDUCTIVE;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_DOWN_ARM;
	f->rotate_tile_with_nodebox = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_STICKYPISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston_active.png^[transformR90");
	f->setTexture(0,"circuit_piston_bottom.png");
	f->setTexture(1,"circuit_piston_top.png");
	f->onact_also_affects = v3s16(0,-1,0);
	content_nodebox_piston_down(f);
	f->setInventoryTextureCube("circuit_piston_top.png", "circuit_piston.png^[transformR90", "circuit_piston.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new PistonNodeMetadata();

	i = CONTENT_CIRCUIT_STICKYPISTON_DOWN_ARM;
	f = &content_features(i);
	f->description = wgettext("Sticky Piston Arm");
	f->is_ground_content = true;
	f->air_equivalent = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->draw_type = CDT_NODEBOX;
	f->special_alternate_node = CONTENT_CIRCUIT_STICKYPISTON_DOWN;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CIRCUIT_STICKYPISTON_OFF)+" 1";
	f->setAllTextures("circuit_piston_arm.png^[transformR90");
	f->setTexture(0,"circuit_piston_arm_bottom.png");
	f->setTexture(1,"circuit_stickypiston_arm_top.png");
	f->onact_also_affects = v3s16(0,1,0);
	content_nodebox_pistonarm_down(f);
	f->setInventoryTextureCube("circuit_piston_arm.png", "circuit_stickypiston_arm_top.png", "circuit_piston_arm.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->pressure_type = CST_SOLID;
	f->suffocation_per_second = 0;
}
