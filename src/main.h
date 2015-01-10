/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* main.h
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

#ifndef MAIN_HEADER
#define MAIN_HEADER

// Settings
class GameSettings;
extern GameSettings *g_settings;

// This makes and maps textures
class ITextureSource;
extern ITextureSource *g_texturesource;

// Global profiler
class Profiler;
extern Profiler *g_profiler;

#ifndef SERVER
#include "common_irrlicht.h"
extern scene::ISceneManager* g_smgr;
#endif

// Debug streams

#include <fstream>

extern std::ostream *dout_con_ptr;
extern std::ostream *derr_con_ptr;
extern std::ostream *dout_client_ptr;
extern std::ostream *derr_client_ptr;
extern std::ostream *dout_server_ptr;
extern std::ostream *derr_server_ptr;

#define dout_con (*dout_con_ptr)
#define derr_con (*derr_con_ptr)
#define dout_client (*dout_client_ptr)
#define derr_client (*derr_client_ptr)
#define dout_server (*dout_server_ptr)
#define derr_server (*derr_server_ptr)

#endif

