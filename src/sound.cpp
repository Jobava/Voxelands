/************************************************************************
* Minetest-c55
* Copyright (C) 2012 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* sound.cpp
* voxelands - 3d voxel world sandbox game
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
*
* License updated from GPLv2 or later to GPLv3 or later by Lisa Milne
* for Voxelands.
************************************************************************/

#include "sound.h"
#include "map.h"
#include "mapnode.h"
#include "content_mapnode.h"
#include "content_mob.h"

// Global DummySoundManager singleton
DummySoundManager dummySoundManager;
ISoundManager *g_sound = NULL;

void init_sounds(ISoundManager *sound)
{
	// sounds must be mono sounds, stereo will not work right!
	// exceptions: background music
	// walking
	// CMT_DIRT
	sound->loadSound("dirt-step-left","step_dirt.1.ogg",0.3);
	sound->loadSound("dirt-step-right","step_dirt.2.ogg",0.3);
	// CMT_STONE
	sound->loadSound("stone-step-left","step_stone.1.ogg");
	sound->loadSound("stone-step-right","step_stone.2.ogg");
	sound->loadSound("stone-step-left","step_stone.3.ogg");
	sound->loadSound("stone-step-right","step_stone.4.ogg");
	// CMT_PLANT
	sound->loadSound("plant-step-left","step_plant.1.ogg",0.3);
	sound->loadSound("plant-step-right","step_plant.2.ogg",0.3);
	// CMT_LIQUID
	sound->loadSound("liquid-step-left","step_liquid.1.ogg",0.5);
	sound->loadSound("liquid-step-right","step_liquid.2.ogg",0.5);
	// CMT_WOOD
	sound->loadSound("wood-step-left","step_wood.1.ogg");
	sound->loadSound("wood-step-right","step_wood.2.ogg");
	sound->loadSound("wood-step-left","step_wood.3.ogg");
	sound->loadSound("wood-step-right","step_wood.4.ogg");
	// CMT_GLASS
	sound->loadSound("glass-step-left","step_glass.1.ogg",0.3);
	sound->loadSound("glass-step-right","step_glass.1.ogg",0.3);
	// special for grass
	sound->loadSound("grass-step-left","step_grass.1.ogg");
	sound->loadSound("grass-step-right","step_grass.2.ogg");

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
	// CMT_GLASS
	sound->loadSound("glass-dig","dig_glass.1.ogg");
	// mobs
	sound->loadSound("mob-dig","dig_mob.ogg");
	// miss
	sound->loadSound("miss-dig","dig_miss.ogg");

	// placing
	sound->loadSound("place","place_node.1.ogg");
	sound->loadSound("place","place_node.2.ogg");
	sound->loadSound("place","place_node.3.ogg");
	// CMT_DIRT
	// CMT_STONE
	// CMT_PLANT
	// CMT_LIQUID
	sound->loadSound("liquid-place","place_liquid.1.ogg");
	// CMT_WOOD
	// CMT_GLASS

	// open formspec
	sound->loadSound("open-menu","open_menu.ogg");
	sound->loadSound("open-book","open_book.ogg");
	sound->loadSound("open-chest","open_chest.ogg");

	// environment and node sounds
	sound->loadSound("env-piston","env_piston.ogg");
	sound->loadSound("env-dooropen","env_dooropen.ogg");
	sound->loadSound("env-doorclose","env_doorclose.ogg");
	sound->loadSound("env-fire","env_fire.ogg");
	sound->loadSound("env-lava","env_lava.ogg",0.5);
	sound->loadSound("env-water","env_water.ogg",0.5);
	sound->loadSound("env-steam","env_steam.ogg");
	sound->loadSound("env-tnt","env_tnt.ogg");
	sound->loadSound("env-teleport","env_teleport.ogg");

	// mobs
	sound->loadSound("mob-oerkki-spawn","mob_oerkki_spawn.ogg");
	sound->loadSound("mob-wolf-hit","mob_wolf_hit.ogg");
	sound->loadSound("mob-wolf-spawn","mob_wolf_spawn.ogg");
	sound->loadSound("mob-sheep-env","mob_sheep_env.ogg");
	sound->loadSound("mob-ducksheep-env","mob_ducksheep_env.ogg");
	sound->loadSound("mob-deer-env","mob_deer_env.ogg");

	// special
	sound->loadSound("wield","wield_item.ogg");
	sound->loadSound("low-energy-F","low_energy_F.ogg");
	sound->loadSound("low-energy-M","low_energy_M.ogg");
	sound->loadSound("player-hurt-F","player_hurt_F.ogg");
	sound->loadSound("player-hurt-M","player_hurt_M.ogg");

	// use
	sound->loadSound("use-eat","use_eat.ogg");
	sound->loadSound("use-drink","use_drink.ogg");

	// menu backgrounds
	sound->loadSound("bg-mainmenu","bg_mainmenu.ogg");
	sound->loadSound("bg-charcreator","bg_charcreator.ogg");
}

void sound_playStep(Map *map, v3f pos, int foot, float gain)
{
	if (!g_sound)
		return;

	v3s16 p = floatToInt(pos,BS);
	MapNode n = map->getNodeNoEx(p);
	ContentFeatures *f = &content_features(n);
	if (f->type == CMT_AIR) {
		p.Y--;
		n = map->getNodeNoEx(p);
		f = &content_features(n);
	}

	std::string snd("");

	if (f->sound_step != "") {
		snd = f->sound_step;
	}else{
		switch (f->type) {
		case CMT_PLANT:
			snd = "plant-step";
			break;
		case CMT_DIRT:
			snd = "dirt-step";
			break;
		case CMT_STONE:
			snd = "stone-step";
			break;
		case CMT_LIQUID:
			snd = "liquid-step";
			break;
		case CMT_TREE:
		case CMT_WOOD:
			snd = "wood-step";
			break;
		case CMT_GLASS:
			snd = "glass-step";
			break;
		default:;
		}
	}

	if (snd == "")
		return;

	if (foot == 0) {
		snd += "-left";
	}else{
		snd += "-right";
	}

	g_sound->playSoundAt(snd,false,pos,gain);
}

void sound_playDig(content_t c, v3f pos)
{
	if (!g_sound)
		return;

	if (c == CONTENT_IGNORE)
		return;

	ContentFeatures *f = &content_features(c);
	if (f->sound_dig != "") {
		g_sound->playSoundAt(f->sound_dig,false,pos);
		return;
	}
	switch (f->type) {
	case CMT_PLANT:
		g_sound->playSoundAt("plant-dig",false,pos);
		break;
	case CMT_DIRT:
		g_sound->playSoundAt("dirt-dig",false,pos);
		break;
	case CMT_STONE:
		g_sound->playSoundAt("stone-dig",false,pos);
		break;
	case CMT_LIQUID:
		g_sound->playSoundAt("liquid-dig",false,pos);
		break;
	case CMT_TREE:
	case CMT_WOOD:
		g_sound->playSoundAt("wood-dig",false,pos);
		break;
	case CMT_GLASS:
		g_sound->playSoundAt("glass-dig",false,pos);
		break;
	default:
		g_sound->playSoundAt("miss-dig",false,pos);
	}
}


#ifdef SERVER

ISoundManager *createSoundManager()
{
	return NULL;
};

#endif
