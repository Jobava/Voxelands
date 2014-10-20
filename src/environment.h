/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* environment.h
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


#ifndef ENVIRONMENT_HEADER
#define ENVIRONMENT_HEADER

/*
	This class is the game's environment.
	It contains:
	- The map
	- Players
	- Other objects
	- The current time in the game (actually it only contains the brightness)
	- etc.
*/

#include <list>
#include <map>
#include <vector>
#include "common_irrlicht.h"
#include "player.h"
#include "map.h"
#include <ostream>
#include "utility.h"
#include "activeobject.h"

class Server;
class ServerActiveObject;

class Environment
{
public:
	// Environment will delete the map passed to the constructor
	Environment();
	virtual ~Environment();

	/*
		Step everything in environment.
		- Move players
		- Step mobs
		- Run timers of map
	*/
	virtual void step(f32 dtime) = 0;

	virtual Map & getMap() = 0;

	virtual void addPlayer(Player *player);
	void removePlayer(u16 peer_id);
	Player * getPlayer(u16 peer_id);
	Player * getPlayer(const char *name);
	Player * getRandomConnectedPlayer();
	Player * getNearestConnectedPlayer(v3f pos);
	core::list<Player*> getPlayers();
	core::list<Player*> getPlayers(bool ignore_disconnected);
	void printPlayers(std::ostream &o);
	virtual bool propogateEnergy(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos) {return false;};

	//void setDayNightRatio(u32 r);
	u32 getDayNightRatio();

	// 0-23999
	virtual void setTimeOfDay(u32 time)
	{
		m_time_of_day = time;
	}

	u32 getTimeOfDay()
	{
		return m_time_of_day;
	}

protected:
	// peer_ids in here should be unique, except that there may be many 0s
	core::list<Player*> m_players;
	// Brightness
	//u32 m_daynight_ratio;
	// Time of day in milli-hours (0-23999); determines day and night
	u32 m_time_of_day;
	// A list of positions and nodes that should be set *after* the entire loop runs
	std::map<v3s16,MapNode> m_delayed_node_changes;
};

/*
	List of active blocks, used by ServerEnvironment
*/

class ActiveBlockList
{
public:
	void update(std::vector<v3s16> &active_positions,
			s16 radius,
			std::map<v3s16, bool> &blocks_removed,
			std::map<v3s16, bool> &blocks_added);

	bool contains(v3s16 p){
		return (m_list.find(p) != m_list.end());
	}

	void clear(){
		m_list.clear();
	}

	std::map<v3s16, bool> m_list;

private:
};

/*
	The server-side environment.

	This is not thread-safe. Server uses an environment mutex.
*/

class ServerEnvironment : public Environment
{
public:
	ServerEnvironment(ServerMap *map, Server *server);
	~ServerEnvironment();

	Map & getMap()
	{
		return *m_map;
	}

	ServerMap & getServerMap()
	{
		return *m_map;
	}

	Server * getServer()
	{
		return m_server;
	}

	void step(f32 dtime);

	/*
		Save players
	*/
	void serializePlayers(const std::string &savedir);
	void deSerializePlayers(const std::string &savedir);

	/*
		Save and load time of day and game timer
	*/
	void saveMeta(const std::string &savedir);
	void loadMeta(const std::string &savedir);

	/*
		External ActiveObject interface
		-------------------------------------------
	*/

	ServerActiveObject* getActiveObject(u16 id);

	/*
		Add an active object to the environment.
		Environment handles deletion of object.
		Object may be deleted by environment immediately.
		If id of object is 0, assigns a free id to it.
		Returns the id of the object.
		Returns 0 if not added and thus deleted.
	*/
	u16 addActiveObject(ServerActiveObject *object);

	/*
		Find out what new objects have been added to
		inside a radius around a position
	*/
	void getAddedActiveObjects(v3s16 pos, s16 radius,
			std::map<u16, bool> &current_objects,
			std::map<u16, bool> &added_objects);

	/*
		Find out what new objects have been removed from
		inside a radius around a position
	*/
	void getRemovedActiveObjects(v3s16 pos, s16 radius,
			std::map<u16, bool> &current_objects,
			std::map<u16, bool> &removed_objects);

	/*
		Get the next message emitted by some active object.
		Returns a message with id=0 if no messages are available.
	*/
	ActiveObjectMessage getActiveObjectMessage();

	/*
		Activate objects and dynamically modify for the dtime determined
		from timestamp and additional_dtime
	*/
	void activateBlock(MapBlock *block, u32 additional_dtime=0);

	/* Other stuff */

	// Clear all objects, loading and going through every MapBlock
	void clearAllObjects();
	virtual bool propogateEnergy(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);

	// search for c within radius of pos
	bool searchNear(v3s16 pos, v3s16 radius_min, v3s16 radius_max, std::vector<content_t> c, v3s16 *found);
	bool searchNear(v3s16 pos, v3s16 radius, std::vector<content_t> c, v3s16 *found)
	{
		return searchNear(pos,-radius,radius,c,found);
	}
	bool searchNear(v3s16 pos, v3s16 radius, content_t c, v3s16 *found)
	{
		std::vector<content_t> search;
		search.push_back(c);
		return searchNear(pos,-radius,radius,search,found);
	}
	// search for not c within radius of pos
	bool searchNearInv(v3s16 pos, v3s16 radius_min, v3s16 radius_max, std::vector<content_t> c, v3s16 *found);
	bool searchNearInv(v3s16 pos, v3s16 radius, std::vector<content_t> c, v3s16 *found)
	{
		return searchNearInv(pos,-radius,radius,c,found);
	}
	bool searchNearInv(v3s16 pos, v3s16 radius, content_t c, v3s16 *found)
	{
		std::vector<content_t> search;
		search.push_back(c);
		return searchNearInv(pos,-radius,radius,search,found);
	}

private:

	/*
		Internal ActiveObject interface
		-------------------------------------------
	*/

	/*
		Add an active object to the environment.

		Called by addActiveObject.

		Object may be deleted by environment immediately.
		If id of object is 0, assigns a free id to it.
		Returns the id of the object.
		Returns 0 if not added and thus deleted.
	*/
	u16 addActiveObjectRaw(ServerActiveObject *object, bool set_changed);

	/*
		Remove all objects that satisfy (m_removed && m_known_by_count==0)
	*/
	void removeRemovedObjects();

	/*
		Convert stored objects from block to active
	*/
	void activateObjects(MapBlock *block);

	/*
		Convert objects that are not in active blocks to static.

		If m_known_by_count != 0, active object is not deleted, but static
		data is still updated.

		If force_delete is set, active object is deleted nevertheless. It
		shall only be set so in the destructor of the environment.
	*/
	void deactivateFarObjects(bool force_delete);

	/*
		Member variables
	*/

	// The map
	ServerMap *m_map;
	// Pointer to server (which is handling this environment)
	Server *m_server;
	// Active object list
	std::map<u16, ServerActiveObject*> m_active_objects;
	// Outgoing network message buffer for active objects
	Queue<ActiveObjectMessage> m_active_object_messages;
	// Some timers
	float m_random_spawn_timer; // used for experimental code
	float m_send_recommended_timer;
	IntervalLimiter m_object_management_interval;
	// List of active blocks
	ActiveBlockList m_active_blocks;
	IntervalLimiter m_active_blocks_management_interval;
	IntervalLimiter m_active_blocks_test_interval;
	IntervalLimiter m_active_blocks_nodemetadata_interval;
	IntervalLimiter m_active_blocks_circuit_interval;
	// Time from the beginning of the game in seconds.
	// Incremented in step().
	u32 m_game_time;
	// A helper variable for incrementing the latter
	float m_game_time_fraction_counter;
};

#ifndef SERVER

#include "clientobject.h"

/*
	The client-side environment.

	This is not thread-safe.
	Must be called from main (irrlicht) thread (uses the SceneManager)
	Client uses an environment mutex.
*/

enum ClientEnvEventType
{
	CEE_NONE,
	CEE_PLAYER_DAMAGE,
	CEE_PLAYER_SUFFOCATE,
	CEE_PLAYER_HUNGER,
	CEE_PLAYER_WEARCLOTHES
};

struct ClientEnvEvent
{
	ClientEnvEventType type;
	union {
		struct{
		} none;
		struct{
			u8 amount;
		} player_damage;
		struct {
			u16 amount;
		} player_wear;
	};
};

class Client;

class ClientEnvironment : public Environment
{
public:
	ClientEnvironment(Client *client, ClientMap *map, scene::ISceneManager *smgr);
	~ClientEnvironment();

	Map & getMap()
	{
		return *m_map;
	}

	ClientMap & getClientMap()
	{
		return *m_map;
	}

	void step(f32 dtime);

	virtual void addPlayer(Player *player);
	LocalPlayer * getLocalPlayer();

	void updateMeshes(v3s16 blockpos, v3s16 camera_offset);
	void expireMeshes(bool only_daynight_diffed);

	void setTimeOfDay(u32 time)
	{
		u32 old_dr = getDayNightRatio();

		Environment::setTimeOfDay(time);

		if(getDayNightRatio() != old_dr)
		{
			dout_client<<DTIME<<"ClientEnvironment: DayNightRatio changed"
					<<" -> expiring meshes"<<std::endl;
			expireMeshes(true);
		}
	}

	/*
		ActiveObjects
	*/

	ClientActiveObject* getActiveObject(u16 id);

	/*
		Adds an active object to the environment.
		Environment handles deletion of object.
		Object may be deleted by environment immediately.
		If id of object is 0, assigns a free id to it.
		Returns the id of the object.
		Returns 0 if not added and thus deleted.
	*/
	u16 addActiveObject(ClientActiveObject *object);

	void addActiveObject(u16 id, u8 type, const std::string &init_data);
	void removeActiveObject(u16 id);

	void processActiveObjectMessage(u16 id, const std::string &data);

	/*
		Callbacks for activeobjects
	*/
	void damageLocalPlayer(u8 damage);
	void damageLocalPlayerWithArmour(u8 damage);
	void damageLocalPlayerWithWarmth(u8 damage);
	void damageLocalPlayerWithVacuum(u8 damage);

	/*
		Client likes to call these
	*/

	// Get all nearby objects
	void getActiveObjects(v3f origin, f32 max_d, core::array<DistanceSortedActiveObject> &dest);

	void updateObjectsCameraOffset(v3s16 camera_offset);

	// Get event from queue. CEE_NONE is returned if queue is empty.
	ClientEnvEvent getClientEvent();

	// search for c within radius of pos
	bool searchNear(v3s16 pos, v3s16 radius_min, v3s16 radius_max, std::vector<content_t> c, v3s16 *found);
	bool searchNear(v3s16 pos, v3s16 radius, std::vector<content_t> c, v3s16 *found)
	{
		return searchNear(pos,-radius,radius,c,found);
	}
	bool searchNear(v3s16 pos, v3s16 radius, content_t c, v3s16 *found)
	{
		std::vector<content_t> search;
		search.push_back(c);
		return searchNear(pos,-radius,radius,search,found);
	}
	// search for not c within radius of pos
	bool searchNearInv(v3s16 pos, v3s16 radius_min, v3s16 radius_max, std::vector<content_t> c, v3s16 *found);
	bool searchNearInv(v3s16 pos, v3s16 radius, std::vector<content_t> c, v3s16 *found)
	{
		return searchNearInv(pos,-radius,radius,c,found);
	}
	bool searchNearInv(v3s16 pos, v3s16 radius, content_t c, v3s16 *found)
	{
		std::vector<content_t> search;
		search.push_back(c);
		return searchNearInv(pos,-radius,radius,search,found);
	}

private:
	Client *m_client;
	ClientMap *m_map;
	scene::ISceneManager *m_smgr;
	std::map<u16, ClientActiveObject*> m_active_objects;
	Queue<ClientEnvEvent> m_client_event_queue;
	IntervalLimiter m_active_object_light_update_interval;
	IntervalLimiter m_lava_hurt_interval;
};

#endif

#endif

