/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* clientserver.h
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2013-2014 <lisa@ltmnet.com>
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


#ifndef CLIENTSERVER_HEADER
#define CLIENTSERVER_HEADER

#include "utility.h"

#define PROTOCOL_VERSION 9
/* the last protocol version used by 0.3.x minetest-c55 clients */
#define PROTOCOL_DOTTHREE 3
/* this is the oldest protocol that we will allow to connect
 * even with strict_protocol_version_checking = false */
#define PROTOCOL_OLDEST 7

#define PROTOCOL_ID 0x4f457403

#define PASSWORD_SIZE 28       // Maximum password length. Allows for base64-encoded SHA-1 (27+\0).

enum ToClientCommand
{
	TOCLIENT_INIT = 0x10,
	/*
		Server's reply to TOSERVER_INIT.
		Sent second after connected.

		[0] u16 TOSERVER_INIT
		[2] u8 deployed version
		[3] v3s16 player's position + v3f(0,BS/2,0) floatToInt'd
		[12] uint64_t map seed (new as of 2011-02-27)

		NOTE: The position in here is deprecated; position is
		      explicitly sent afterwards
	*/

	TOCLIENT_BLOCKDATA = 0x20, //TODO: Multiple blocks
	TOCLIENT_ADDNODE = 0x21,
	TOCLIENT_REMOVENODE = 0x22,

	TOCLIENT_SERVERSETTINGS = 0x23,
	/*
		u16 command
		u8 enable_damage
		u8 enable_suffocation
		u8 enable_hunger
	*/

	TOCLIENT_PLAYERINFO = 0x24, // deprecated, see TOCLIENT_PLAYERDATA
	/*
		[0] u16 command
		for each player:
			u16 peer_id
			char[20] name
	*/

	TOCLIENT_PLAYER_ANIMATION = 0x25,
	/*
		[0] u16 command
		[2] u16 peer_id
		[4] u8 animation_id
	*/

	TOCLIENT_PLAYERHP = 0x26,
	/*
		u16 command
		s8 hp
		s8 air
		s8 hunger
	*/

	TOCLIENT_INVENTORY = 0x27,
	/*
		[0] u16 command
		[2] serialized inventory
	*/

	TOCLIENT_OBJECTDATA = 0x28,
	/*
		Sent as unreliable.

		u16 command
		u16 number of player positions
		for each player:
			u16 peer_id
			v3s32 position*100
			v3s32 speed*100
			s32 pitch*100
			s32 yaw*100
		u16 count of blocks
		for each block:
			v3s16 blockpos
			block objects
	*/

	TOCLIENT_TIME_OF_DAY = 0x29,
	/*
		u16 command
		u16 time (0-23999)
	*/

	TOCLIENT_CHAT_MESSAGE = 0x30,
	/*
		u16 command
		u16 length
		wstring message
	*/

	TOCLIENT_ACTIVE_OBJECT_REMOVE_ADD = 0x31,
	/*
		u16 command
		u16 count of removed objects
		for all removed objects {
			u16 id
		}
		u16 count of added objects
		for all added objects {
			u16 id
			u8 type
			u32 initialization data length
			string initialization data
		}
	*/

	TOCLIENT_ACTIVE_OBJECT_MESSAGES = 0x32,
	/*
		u16 command
		for all objects
		{
			u16 id
			u16 message length
			string message
		}
	*/

	TOCLIENT_HP = 0x33,
	/*
		u16 command
		u8 hp
	*/

	TOCLIENT_MOVE_PLAYER = 0x34,
	/*
		u16 command
		v3f1000 player position
		f1000 player pitch
		f1000 player yaw
	*/

	TOCLIENT_ACCESS_DENIED = 0x35,
	/*
		u16 command
		u16 reason_length
		wstring reason
	*/

	TOCLIENT_PLAYERITEM = 0x36, // Obsolete, see TOCLIENT_PLAYERITEMS
	/*
		u16 command
		u16 count of player items
		for all player items {
			u16 peer id
			u16 length of serialized item
			string serialized item
		}
	*/

	TOCLIENT_DEATHSCREEN = 0x37,
	/*
		u16 command
		u8 bool set camera point target
		v3f1000 camera point target (to point the death cause or whatever)
	*/

	TOCLIENT_HAVECOOKIE = 0x38,
	/*
		u16 command
		u16 textlen
		textdata
	*/

	TOCLIENT_PLAYERITEMS = 0x39,
	/*
		u16 command
		u16 count of players
		u16 count of items
		for all players {
			u16 peer id
			u16 wield
			u16 hat
			u16 shirt
			u16 pants
			u16 boots
		}
	*/

	TOCLIENT_PLAYERDATA = 0x40,
	/*
		[0] u16 command
		[2] u16 player count
		[4] u16 field count
		for each player {
			u16 peer_id
			char[20] name
			u16 length of serialized chardef
			string serialized character definition
		}
	*/

	TOCLIENT_ENV_EVENT = 0x41,
	/*
		u16 command
		u8 event type (sound,nodemod,particles,etc)
		v3f1000 event position
		u16 length of serialised event data
		string serialised event data
	*/

	TOCLIENT_INVENTORY_UPDATE = 0x42,
	/*
		u16 command
		u16 list count
		for each list {
			u16 length of serialised list name
			string serialised list name
			u16 slot count
			for each slot {
				u16 slot index
				u16 content type
				u16 count/wear
			}
		}
	*/
};

enum ToServerCommand
{
	TOSERVER_INIT=0x10,
	/*
		Sent first after connected.

		[0] u16 TOSERVER_INIT
		[2] u8 SER_FMT_VER_HIGHEST
		[3] u8[20] player_name
		[23] u8[28] password (new in some version)
		[51] u16 client network protocol version (new in some version)
	*/

	TOSERVER_INIT2 = 0x11,
	/*
		Sent as an ACK for TOCLIENT_INIT.
		After this, the server can send data.

		[0] u16 TOSERVER_INIT2
	*/

	TOSERVER_THROWITEM = 0x12,
	/*
		[0] u16 command
		[2] v3s32 position*100
		[14] v3s32 speed*100
		[26] u16 item
	*/

	TOSERVER_GETBLOCK=0x20, // Obsolete
	TOSERVER_ADDNODE = 0x21, // Obsolete
	TOSERVER_REMOVENODE = 0x22, // Obsolete

	TOSERVER_PLAYERPOS = 0x23,
	/*
		[0] u16 command
		[2] v3s32 position*100
		[2+12] v3s32 speed*100
		[2+12+12] s32 pitch*100
		[2+12+12+4] s32 yaw*100
	*/

	TOSERVER_GOTBLOCKS = 0x24,
	/*
		[0] u16 command
		[2] u8 count
		[3] v3s16 pos_0
		[3+6] v3s16 pos_1
		...
	*/

	TOSERVER_DELETEDBLOCKS = 0x25,
	/*
		[0] u16 command
		[2] u8 count
		[3] v3s16 pos_0
		[3+6] v3s16 pos_1
		...
	*/

	TOSERVER_ADDNODE_FROM_INVENTORY = 0x26, // Obsolete
	/*
		[0] u16 command
		[2] v3s16 pos
		[8] u16 i
	*/

	TOSERVER_CLICK_OBJECT = 0x27,
	/*
		length: 13
		[0] u16 command
		[2] u8 button (0=left, 1=right)
		[3] v3s16 blockpos
		[9] s16 id
		[11] u16 item
	*/

	TOSERVER_GROUND_ACTION = 0x28,
	/*
		length: 17
		[0] u16 command
		[2] u8 action
		[3] v3s16 nodepos_undersurface
		[9] v3s16 nodepos_abovesurface
		[15] u16 item
		actions:
		0: start digging (from undersurface)
		1: place block (to abovesurface)
		2: stop digging (all parameters ignored)
		3: digging completed
	*/

	TOSERVER_PLAYERDAMAGE = 0x29,
	/*
		u16 command
		s8 damage_amount
		s8 suffocate_amount
		s8 hunger_amount
	*/

	TOSERVER_PLAYERWEAR = 0x30,
	/*
		u16 command
		s8 amount
	*/

	TOSERVER_INVENTORY_ACTION = 0x31,
	/*
		See InventoryAction in inventory.h
	*/

	TOSERVER_CHAT_MESSAGE = 0x32,
	/*
		u16 command
		u16 length
		wstring message
	*/

	TOSERVER_SIGNNODETEXT = 0x33,
	/*
		u16 command
		v3s16 p
		u16 textlen
		textdata
	*/

	TOSERVER_CLICK_ACTIVEOBJECT = 0x34,
	/*
		length: 7
		[0] u16 command
		[2] u8 button (0=left, 1=right)
		[3] u16 id
		[5] u16 item
	*/

	TOSERVER_DAMAGE = 0x35, // obsolete, see TOSERVER_PLAYERDAMAGE
	/*
		u16 command
		u8 amount
	*/

	TOSERVER_PASSWORD=0x36,
	/*
		Sent to change password.

		[0] u16 TOSERVER_PASSWORD
		[2] u8[28] old password
		[30] u8[28] new password
	*/

	TOSERVER_PLAYERITEM=0x37,
	/*
		Sent to change selected item.

		[0] u16 TOSERVER_PLAYERITEM
		[2] u16 item
	*/

	TOSERVER_RESPAWN=0x38,
	/*
		u16 TOSERVER_RESPAWN
	*/

	TOSERVER_WANTCOOKIE=0x39,
	/*
		u16 TOSERVER_WANTCOOKIE
	*/

	TOSERVER_NODEMETA_FIELDS = 0x3b,
	/*
		u16 command
		v3s16 p
		u16 len
		u8[len] form name (reserved for future use)
		u16 number of fields
		for each field:
			u16 len
			u8[len] field name
			u32 len
			u8[len] field value
	*/
};

inline SharedBuffer<u8> makePacket_TOCLIENT_TIME_OF_DAY(u16 time)
{
	SharedBuffer<u8> data(2+2);
	writeU16(&data[0], TOCLIENT_TIME_OF_DAY);
	writeU16(&data[2], time);
	return data;
}

#endif

