/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
* Copyright (C) 2011 Ciaran Gultnieks <ciaran@ciarang.com>
*
* servercommand.h
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

#ifndef SERVERCOMMAND_HEADER
#define SERVERCOMMAND_HEADER

#include <vector>
#include <sstream>
#include "common_irrlicht.h"
#include "player.h"
#include "server.h"

#define SEND_TO_SENDER (1<<0)
#define SEND_TO_OTHERS (1<<1)
#define SEND_NO_PREFIX (1<<2)

struct ServerCommandContext
{
	std::vector<std::wstring> parms;
	std::wstring paramstring;
	Server* server;
	ServerEnvironment *env;
	Player* player;
	// Effective privs for the player, which may be different to their
	// stored ones - e.g. if they are named in the config as an admin.
	uint64_t privs;
	u32 flags;

	ServerCommandContext(
		std::vector<std::wstring> parms,
		std::wstring paramstring,
		Server* server,
		ServerEnvironment *env,
		Player* player,
		uint64_t privs)
		: parms(parms), paramstring(paramstring),
		server(server), env(env), player(player), privs(privs)
	{
	}

};

// Process a command sent from a client. The environment and connection
// should be locked when this is called.
// Returns a response message, to be dealt with according to the flags set
// in the context.
std::wstring processServerCommand(ServerCommandContext *ctx);

#endif


