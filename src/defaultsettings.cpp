/*
Minetest-c55
Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "settings.h"
#include "defaultsettings.h"

void set_default_settings(Settings *settings)
{
	// Client and server

	settings->setDefault("port", "");
	settings->setDefault("name", "");

	// Client stuff

	settings->setDefault("keymap_forward", "KEY_KEY_W");
	settings->setDefault("keymap_backward", "KEY_KEY_S");
	settings->setDefault("keymap_left", "KEY_KEY_A");
	settings->setDefault("keymap_right", "KEY_KEY_D");
	settings->setDefault("keymap_jump", "KEY_SPACE");
	settings->setDefault("keymap_sneak", "KEY_LSHIFT");
	settings->setDefault("keymap_inventory", "KEY_KEY_I");
	settings->setDefault("keymap_special1", "KEY_KEY_E");
	settings->setDefault("keymap_chat", "KEY_KEY_T");
	settings->setDefault("keymap_cmd", "/");
	settings->setDefault("keymap_rangeselect", "KEY_KEY_R");
	settings->setDefault("keymap_freemove", "KEY_KEY_K");
	settings->setDefault("keymap_fastmove", "KEY_KEY_J");
	settings->setDefault("keymap_screenshot", "KEY_F12");
	settings->setDefault("keymap_toggle_hud", "KEY_F1");
	settings->setDefault("keymap_toggle_chat", "KEY_F2");
	settings->setDefault("keymap_toggle_force_fog_off", "KEY_F3");
	settings->setDefault("keymap_toggle_update_camera", "KEY_F4");
	settings->setDefault("keymap_toggle_debug", "KEY_F5");
	settings->setDefault("keymap_toggle_profiler", "KEY_F6");
	settings->setDefault("keymap_increase_viewing_range_min", "KEY_PRIOR");
	settings->setDefault("keymap_decrease_viewing_range_min", "KEY_NEXT");
	// Some (temporary) keys for debugging
	settings->setDefault("keymap_print_debug_stacks", "KEY_KEY_P");

	// Show debug info by default?
#ifdef NDEBUG
	settings->setDefault("show_debug", "false");
#else
	settings->setDefault("show_debug", "true");
#endif

	settings->setDefault("mouse_sensitivity", "0.2");
	settings->setDefault("mainmenu_tab","singleplayer");
	settings->setDefault("wanted_fps", "30");
	settings->setDefault("fps_max", "60");
	settings->setDefault("viewing_range_nodes_max", "300");
	settings->setDefault("viewing_range_nodes_min", "15");
	settings->setDefault("screenW", "1024");
	settings->setDefault("screenH", "600");
	settings->setDefault("fullscreen","false");
	settings->setDefault("fullscreen_bpp", "24");
	settings->setDefault("fsaa", "0");
	settings->setDefault("vsync", "false");
	settings->setDefault("high_precision_fpu","true");
	settings->setDefault("address", "");
	settings->setDefault("random_input", "false");
	settings->setDefault("client_unload_unused_data_timeout", "600");
	settings->setDefault("enable_fog", "true");
	settings->setDefault("fov", "72");
	settings->setDefault("view_bobbing", "true");
	settings->setDefault("new_style_water", "false");
	settings->setDefault("new_style_leaves", "true");
	settings->setDefault("smooth_lighting", "true");
	settings->setDefault("enable_texture_atlas", "false");
	settings->setDefault("data_path", "");
	settings->setDefault("video_driver", "opengl");
	settings->setDefault("free_move", "false");
	settings->setDefault("continuous_forward", "false");
	settings->setDefault("fast_move", "false");
	settings->setDefault("invert_mouse", "false");
	settings->setDefault("enable_farmesh", "false");
	settings->setDefault("enable_clouds", "true");
	settings->setDefault("invisible_stone", "false");
	settings->setDefault("screenshot_path", ".");
	settings->setDefault("view_bobbing_amount", "1.0");
	settings->setDefault("enable_3d_clouds", "true");
	settings->setDefault("opaque_water", "false");
	settings->setDefault("enable_particles", "true");
	settings->setDefault("mip_map", "true");
	settings->setDefault("anisotropic_filter", "true");
	settings->setDefault("bilinear_filter", "false");
	settings->setDefault("trilinear_filter", "false");
	settings->setDefault("sound_volume", "50");

	// Server stuff
	// "map-dir" doesn't exist by default.
	settings->setDefault("motd", "");
	settings->setDefault("max_users", "20");
	settings->setDefault("strict_protocol_version_checking", "false");
	settings->setDefault("disallow_empty_passwords","false");
	settings->setDefault("fixed_map_seed", "");
	settings->setDefault("default_password", "");
	settings->setDefault("default_privs", "build, shout");
	settings->setDefault("borderstone_radius","5");
	settings->setDefault("enable_footprints","true");
	settings->setDefault("game_mode","adventure");
	set_adventure_defaults(settings);

	// only enable http on the server for now
	// adventurous players can enable it on the client
#ifndef SERVER
	settings->setDefault("enable_http","false");
#else
	settings->setDefault("enable_http","true");
#endif
	settings->setDefault("profiler_print_interval", "0");
	settings->setDefault("enable_mapgen_debug_info", "false");
	settings->setDefault("objectdata_interval", "0.2");
	settings->setDefault("active_object_send_range_blocks", "3");
	settings->setDefault("active_block_range", "2");
	//settings->setDefault("max_simultaneous_block_sends_per_client", "1");
	// This causes frametime jitter on client side, or does it?
	settings->setDefault("max_simultaneous_block_sends_per_client", "2");
	settings->setDefault("max_simultaneous_block_sends_server_total", "8");
	settings->setDefault("max_block_send_distance", "7");
	settings->setDefault("max_block_generate_distance", "5");
	settings->setDefault("time_send_interval", "20");
	settings->setDefault("time_speed", "96");
	settings->setDefault("server_unload_unused_data_timeout", "19");
	settings->setDefault("server_map_save_interval", "1.238");
	settings->setDefault("full_block_send_enable_min_time_from_building", "2.0");
	settings->setDefault("onload_ignore_objects","false");
	settings->setDefault("enable_experimental", "false");
	settings->setDefault("enable_lavabuckets", "true");
	settings->setDefault("enable_tnt", "true");
}

void set_creative_defaults(Settings *settings)
{
	settings->setDefault("infinite_inventory", "true");
	settings->setDefault("droppable_inventory", "false");
	settings->setDefault("enable_damage", "false");
	settings->setDefault("enable_suffocation", "false");
	settings->setDefault("max_mob_level", "passive");
	settings->setDefault("initial_inventory", "false");
	settings->setDefault("tool_wear","false");
}

void set_adventure_defaults(Settings *settings)
{
	settings->setDefault("infinite_inventory", "false");
	settings->setDefault("droppable_inventory", "true");
	settings->setDefault("enable_damage", "true");
	settings->setDefault("enable_suffocation", "false");
	settings->setDefault("max_mob_level", "aggressive");
	settings->setDefault("initial_inventory", "true");
	settings->setDefault("tool_wear","true");
}

void set_survival_defaults(Settings *settings)
{
	settings->setDefault("infinite_inventory", "false");
	settings->setDefault("droppable_inventory", "true");
	settings->setDefault("enable_damage", "true");
	settings->setDefault("enable_suffocation", "true");
	settings->setDefault("max_mob_level", "aggressive");
	settings->setDefault("initial_inventory", "false");
	settings->setDefault("tool_wear","true");
}

void GameSettings::setGameDefaults(std::string mode)
{
	if (mode == "creative") {
		set_creative_defaults(this);
	}else if (mode == "survival") {
		set_survival_defaults(this);
	}else{
		set_adventure_defaults(this);
	}
}
