/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* client.h
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

#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#ifndef SERVER

#include "connection.h"
#include "environment.h"
#include "common_irrlicht.h"
#include "jmutex.h"
#include <ostream>
#include "clientobject.h"
#include "particles.h"
#include "utility.h" // For IntervalLimiter
#include "sound.h"

struct MeshMakeData;

class ClientNotReadyException : public BaseException
{
public:
	ClientNotReadyException(const char *s):
		BaseException(s)
	{}
};

struct QueuedMeshUpdate
{
	v3s16 p;
	MeshMakeData *data;
	bool ack_block_to_server;

	QueuedMeshUpdate();
	~QueuedMeshUpdate();
};

/*
	A thread-safe queue of mesh update tasks
*/
class MeshUpdateQueue
{
public:
	MeshUpdateQueue();

	~MeshUpdateQueue();

	/*
		peer_id=0 adds with nobody to send to
	*/
	void addBlock(v3s16 p, MeshMakeData *data, bool ack_block_to_server);

	// Returned pointer must be deleted
	// Returns NULL if queue is empty
	QueuedMeshUpdate * pop();

	u32 size()
	{
		JMutexAutoLock lock(m_mutex);
		return m_queue.size();
	}

private:
	core::list<QueuedMeshUpdate*> m_queue;
	JMutex m_mutex;
};

class MapBlockMesh;

struct MeshUpdateResult
{
	v3s16 p;
	MapBlockMesh *mesh;
	bool ack_block_to_server;

	MeshUpdateResult():
		p(-1338,-1338,-1338),
		mesh(NULL),
		ack_block_to_server(false)
	{
	}
};

class MeshUpdateThread : public SimpleThread
{
public:

	MeshUpdateThread()
	{
	}

	void * Thread();

	MeshUpdateQueue m_queue_in;

	MutexedQueue<MeshUpdateResult> m_queue_out;

	v3s16 m_camera_offset;
	ClientEnvironment *m_env;
};

enum ClientEventType
{
	CE_NONE,
	CE_PLAYER_DAMAGE,
	CE_PLAYER_SUFFOCATE,
	CE_PLAYER_HUNGER,
	CE_PLAYER_FORCE_MOVE,
	CE_DEATHSCREEN,
};

struct ClientEvent
{
	ClientEventType type;
	union{
		struct{
		} none;
		struct{
			s8 amount;
		} player_damage;
		struct{
			f32 pitch;
			f32 yaw;
		} player_force_move;
		struct{
			bool set_camera_point_target;
			f32 camera_point_target_x;
			f32 camera_point_target_y;
			f32 camera_point_target_z;
		} deathscreen;
	};
};

class Client : public con::PeerHandler, public InventoryManager
{
public:
	/*
		NOTE: Nothing is thread-safe here.
	*/

	Client(
		IrrlichtDevice *device,
		const char *playername,
		std::string password,
		MapDrawControl &control,
		ISoundManager *sound
	);

	~Client();
	/*
		The name of the local player should already be set when
		calling this, as it is sent in the initialization.
	*/
	void connect(Address address);
	/*
		returns true when
			m_con.Connected() == true
			AND m_server_ser_ver != SER_FMT_VER_INVALID
		throws con::PeerNotFoundException if connection has been deleted,
		eg. timed out.
	*/
	bool connectedAndInitialized();
	/*
		Stuff that references the environment is valid only as
		long as this is not called. (eg. Players)
		If this throws a PeerNotFoundException, the connection has
		timed out.
	*/
	void step(float dtime);

	// Called from updater thread
	// Returns dtime
	//float asyncStep();

	void ProcessData(u8 *data, u32 datasize, u16 sender_peer_id);
	// Returns true if something was received
	bool AsyncProcessPacket();
	bool AsyncProcessData();
	void Send(u16 channelnum, SharedBuffer<u8> data, bool reliable);

	// Pops out a packet from the packet queue
	//IncomingPacket getPacket();

	void groundAction(u8 action, v3s16 nodepos_undersurface,
			v3s16 nodepos_oversurface, u16 item);
	void throwItem(v3f dir, u16 item);
	void useItem();
	void clickActiveObject(u8 button, u16 id, u16 item_i);

	void sendNodemetaFields(v3s16 p, const std::string &formname,
			const std::map<std::string, std::wstring> &fields);
	void sendInventoryAction(InventoryAction *a);
	void sendChatMessage(const std::wstring &message);
	void sendChangePassword(const std::wstring oldpassword,
		const std::wstring newpassword);
	void sendDamage(s8 damage, s8 suffocate, s8 hunger);
	void sendClothesWear(u16 wear);
	void sendRespawn();

	ClientEnvironment& getEnv() { return m_env; }

	// locks envlock
	void removeNode(v3s16 p);
	// locks envlock
	void addNode(v3s16 p, MapNode n);

	void updateCamera(v3f pos, v3f dir, f32 fov, v3s16 camera_offset);

	void renderPostFx();

	// Returns InvalidPositionException if not found
	MapNode getNode(v3s16 p);
	// Wrapper to Map
	NodeMetadata* getNodeMetadata(v3s16 p);

	Player* getPlayer(const char* name) { return m_env.getPlayer(name);}
	LocalPlayer* getLocalPlayer();

	void setPlayerControl(PlayerControl &control);

	void selectPlayerItem(u16 item);

	// Returns true if the inventory of the local player has been
	// updated from the server. If it is true, it is set to false.
	bool getLocalInventoryUpdated();
	// Copies the inventory of the local player to parameter
	void getLocalInventory(Inventory &dst);

	InventoryContext *getInventoryContext();

	Inventory* getInventory(InventoryContext *c, std::string id);
	Inventory* getInventory(const InventoryLocation *loc);
	void inventoryAction(InventoryAction *a);

	// Gets closest object pointed by the shootline
	// Returns NULL if not found
	ClientActiveObject * getSelectedActiveObject(
			f32 max_d,
			v3f from_pos_f_on_map,
			core::line3d<f32> shootline_on_map
	);

	// Prints a line or two of info
	void printDebugInfo(std::ostream &os);

	u16 getHP();
	u16 getAir();
	u16 getHunger();
	float getEnergy();

	bool getChatMessage(std::wstring &message)
	{
		if(m_chat_queue.size() == 0)
			return false;
		message = m_chat_queue.pop_front();
		return true;
	}

	void addChatMessage(const std::wstring &message)
	{
		if (message[0] == L'/') {
			m_chat_queue.push_back(
				(std::wstring)L"issued command: "+message);
			return;
		}

		//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out
		LocalPlayer *player = m_env.getLocalPlayer();
		assert(player != NULL);
		std::wstring name = narrow_to_wide(player->getName());
		m_chat_queue.push_back(
				(std::wstring)L"<"+name+L"> "+message);
	}

	uint64_t getMapSeed() {return m_map_seed;}
	MapGenType getMapType() {return m_map_type;}

	void addUpdateMeshTask(v3s16 blockpos, bool ack_to_server=false, bool refresh_only=false);
	// Including blocks at appropriate edges
	void addUpdateMeshTaskWithEdge(v3s16 blockpos, bool ack_to_server=false);

	void updateCameraOffset(v3s16 camera_offset){ m_mesh_update_thread.m_camera_offset = camera_offset; }

	// Get event from queue. CE_NONE is returned if queue is empty.
	ClientEvent getClientEvent();

	inline bool accessDenied()
	{
		return m_access_denied;
	}

	inline std::wstring accessDeniedReason()
	{
		return m_access_denied_reason;
	}

	float getRTT(void);
	virtual ISoundManager* getSoundManager();

	void playStepSound(int foot);
	void playDigSound(content_t c);
	void playPlaceSound(content_t c);
	void playSound(std::string &name, bool loop);
	void playSoundAt(std::string &name, v3f pos, bool loop);

	void setPointedNode(v3s16 p) {m_pointed_node = p;}
	v3s16 getPointedNode() {return m_pointed_node;}

	void setPointedContent(content_t c) {m_pointed_content = c;}
	content_t getPointedContent() {return m_pointed_content;}

	bool getServerDamage() {return m_server_damage;}
	bool getServerSuffocation() {return m_server_suffocation;}
	bool getServerHunger() {return m_server_hunger;}

	bool getFormState() {return m_form_open;}
	void setFormState(bool state) {m_form_open = state;}

	u8 getSleepAlpha() {return m_sleep_state*255;}

private:

	// Virtual methods from con::PeerHandler
	void peerAdded(con::Peer *peer);
	void deletingPeer(con::Peer *peer, bool timeout);

	void ReceiveAll();
	void Receive();

	void sendPlayerPos();
	// This sends the player's current name etc to the server
	void sendPlayerInfo();
	// Send the item number 'item' as player item to the server
	void sendPlayerItem(u16 item);

	void setServerSettings(bool damage, bool suffocation, bool hunger);

	float m_packetcounter_timer;
	float m_connection_reinit_timer;
	float m_avg_rtt_timer;
	float m_playerpos_send_timer;
	float m_ignore_damage_timer; // Used after server moves player
	IntervalLimiter m_map_timer_and_unload_interval;

	MeshUpdateThread m_mesh_update_thread;

	ClientEnvironment m_env;

	// when connecting to a server it will give these via TOCLIENT_SERVERSETTINGS
	bool m_server_damage;
	bool m_server_suffocation;
	bool m_server_hunger;

	con::Connection m_con;
	ISoundManager *m_sound;

	IrrlichtDevice *m_device;

	// Server serialization version
	u8 m_server_ser_ver;

	// This is behind m_env_mutex.
	bool m_inventory_updated;

	core::map<v3s16, bool> m_active_blocks;
	v3s16 m_pointed_node;
	content_t m_pointed_content;

	PacketCounter m_packetcounter;

	bool m_form_open;

	Queue<std::wstring> m_chat_queue;

	// The seed returned by the server in TOCLIENT_INIT is stored here
	uint64_t m_map_seed;
	MapGenType m_map_type;

	std::string m_password;
	bool m_access_denied;
	std::wstring m_access_denied_reason;

	InventoryContext m_inventory_context;

	Queue<ClientEvent> m_client_event_queue;

	// time_of_day speed approximation for old protocol
	bool m_time_of_day_set;
	float m_last_time_of_day_f;
	float m_time_of_day_update_timer;

	// sleep effects
	bool m_sleeping;
	bool m_waking;
	float m_sleep_state;
};

#endif // !SERVER

#endif // !CLIENT_HEADER

