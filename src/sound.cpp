/*
Minetest-Classic
Copyright (C) 2012 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "sound.h"

// Global DummySoundManager singleton
DummySoundManager dummySoundManager;

void init_sounds(ISoundManager *sound)
{
	// walking
	// CMT_DIRT
	sound->loadSound("dirt-walk","dirt_footstep.1.ogg");
	sound->loadSound("dirt-walk","dirt_footstep.2.ogg");
	sound->loadSound("dirt-walk","dirt_footstep.3.ogg");
	sound->loadSound("dirt-walk","dirt_footstep.4.ogg");
	// CMT_STONE
	sound->loadSound("stone-walk","stone_footstep.1.ogg");
	sound->loadSound("stone-walk","stone_footstep.2.ogg");
	sound->loadSound("stone-walk","stone_footstep.3.ogg");
	sound->loadSound("stone-walk","stone_footstep.4.ogg");
	// CMT_PLANT
	sound->loadSound("plant-walk","plant_footstep.1.ogg");
	sound->loadSound("plant-walk","plant_footstep.2.ogg");
	sound->loadSound("plant-walk","plant_footstep.3.ogg");
	sound->loadSound("plant-walk","plant_footstep.4.ogg");
	// CMT_LIQUID
	sound->loadSound("liquid-walk","liquid_footstep.1.ogg");
	sound->loadSound("liquid-walk","liquid_footstep.2.ogg");
	sound->loadSound("liquid-walk","liquid_footstep.3.ogg");
	sound->loadSound("liquid-walk","liquid_footstep.4.ogg");
	// CMT_WOOD
	sound->loadSound("wood-walk","wood_footstep.1.ogg");
	sound->loadSound("wood-walk","wood_footstep.2.ogg");
	sound->loadSound("wood-walk","wood_footstep.3.ogg");
	sound->loadSound("wood-walk","wood_footstep.4.ogg");

	// digging
	// CMT_DIRT
	sound->loadSound("dirt-dig","dig_dirt.1.ogg");
	// CMT_STONE
	sound->loadSound("stone-dig","dig_stone.1.ogg");
	// CMT_PLANT
	sound->loadSound("plant-dig","dig_plant.1.ogg");
	// CMT_LIQUID
	sound->loadSound("liquid-dig","dig_liquid.1.ogg");
	// CMT_WOOD
	sound->loadSound("wood-dig","dig_wood.1.ogg");

	// placing
	sound->loadSound("place","place_node.1.ogg");
	sound->loadSound("place","place_node.2.ogg");
	sound->loadSound("place","place_node.3.ogg");
	// CMT_DIRT
	// CMT_STONE
	// CMT_PLANT
	// CMT_LIQUID
	// CMT_WOOD
}
