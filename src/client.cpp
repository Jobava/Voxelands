/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* client.cpp
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


#include "client.h"
#include "utility.h"
#include <iostream>
#include "clientserver.h"
#include "jmutexautolock.h"
#include "main.h"
#include <sstream>
#include "porting.h"
#include "mapsector.h"
#include "mapblock_mesh.h"
#include "mapblock.h"
#include "settings.h"
#include "profiler.h"
#include "log.h"
#include "http.h"
#include "sound.h"
#include "content_clothesitem.h"
#include "content_toolitem.h"
#include "content_craftitem.h"
#include "content_mob.h"

/*
	QueuedMeshUpdate
*/

QueuedMeshUpdate::QueuedMeshUpdate():
	p(-1337,-1337,-1337),
	data(NULL),
	ack_block_to_server(false)
{
}

QueuedMeshUpdate::~QueuedMeshUpdate()
{
	if(data)
		delete data;
}

/*
	MeshUpdateQueue
*/

MeshUpdateQueue::MeshUpdateQueue()
{
	m_mutex.Init();
}

MeshUpdateQueue::~MeshUpdateQueue()
{
	JMutexAutoLock lock(m_mutex);

	core::list<QueuedMeshUpdate*>::Iterator i;
	for(i=m_queue.begin(); i!=m_queue.end(); i++)
	{
		QueuedMeshUpdate *q = *i;
		delete q;
	}
}

/*
	peer_id=0 adds with nobody to send to
*/
void MeshUpdateQueue::addBlock(v3s16 p, MeshMakeData *data, bool ack_block_to_server)
{
	DSTACK(__FUNCTION_NAME);

	assert(data);

	JMutexAutoLock lock(m_mutex);

	/*
		Find if block is already in queue.
		If it is, update the data and quit.
	*/
	core::list<QueuedMeshUpdate*>::Iterator i;
	for (i=m_queue.begin(); i!=m_queue.end(); i++) {
		QueuedMeshUpdate *q = *i;
		if (q->p == p) {
			if (q->data && data->m_refresh_only) {
				q->data->m_daynight_ratio = data->m_daynight_ratio;
				delete data;
			}else{
				if (q->data)
					delete q->data;
				q->data = data;
			}
			if (ack_block_to_server)
				q->ack_block_to_server = true;
			return;
		}
	}

	/*
		Add the block
	*/
	QueuedMeshUpdate *q = new QueuedMeshUpdate;
	q->p = p;
	q->data = data;
	q->ack_block_to_server = ack_block_to_server;
	m_queue.push_back(q);
}

// Returned pointer must be deleted
// Returns NULL if queue is empty
QueuedMeshUpdate * MeshUpdateQueue::pop()
{
	JMutexAutoLock lock(m_mutex);

	core::list<QueuedMeshUpdate*>::Iterator i = m_queue.begin();
	if(i == m_queue.end())
		return NULL;
	QueuedMeshUpdate *q = *i;
	m_queue.erase(i);
	return q;
}

/*
	MeshUpdateThread
*/

void * MeshUpdateThread::Thread()
{
	ThreadStarted();

	log_register_thread("MeshUpdateThread");

	DSTACK(__FUNCTION_NAME);

	BEGIN_DEBUG_EXCEPTION_HANDLER

	while (getRun()) {
		QueuedMeshUpdate *q = m_queue_in.pop();
		if (q == NULL) {
			sleep_ms(3);
			continue;
		}

		ScopeProfiler sp(g_profiler, "Client: Mesh making");

		if (q->data && q->data->m_refresh_only) {
			MapBlock *block = m_env->getMap().getBlockNoCreateNoEx(q->p);
			if (block && block->mesh) {
				{
					JMutexAutoLock lock(block->mesh_mutex);
					block->mesh->refresh(q->data->m_daynight_ratio);
				}
			}
		}else{
			MapBlock *block = m_env->getMap().getBlockNoCreateNoEx(q->p);
			if (block && block->mesh) {
				block->mesh->generate(q->data, m_camera_offset, &block->mesh_mutex);
				if (q->ack_block_to_server) {
					MeshUpdateResult r;
					r.p = q->p;
					r.mesh = NULL;
					r.ack_block_to_server = true;
					m_queue_out.push_back(r);
				}
			}else if (block) {
				MapBlockMesh *mesh_new = new MapBlockMesh(q->data, m_camera_offset);
				MeshUpdateResult r;
				r.p = q->p;
				r.mesh = mesh_new;
				r.ack_block_to_server = q->ack_block_to_server;

				m_queue_out.push_back(r);
			}
		}

		delete q;
	}

	END_DEBUG_EXCEPTION_HANDLER(errorstream)

	return NULL;
}

Client::Client(
		IrrlichtDevice *device,
		const char *playername,
		std::string password,
		MapDrawControl &control,
		ISoundManager *sound):
	m_mesh_update_thread(),
	m_env(
		this,
		new ClientMap(this, control,
			device->getSceneManager()->getRootSceneNode(),
			device->getSceneManager(), 666),
		device->getSceneManager()
	),
	m_server_damage(false),
	m_server_suffocation(false),
	m_server_hunger(false),
	m_con(PROTOCOL_ID, 512, CONNECTION_TIMEOUT, this),
	m_sound(sound),
	m_device(device),
	m_server_ser_ver(SER_FMT_VER_INVALID),
	m_inventory_updated(false),
	m_pointed_node(-32768,-32768,-32768),
	m_pointed_content(CONTENT_IGNORE),
	m_form_open(false),
	m_map_seed(0),
	m_map_type(MGT_DEFAULT),
	m_password(password),
	m_access_denied(false),
	m_time_of_day_set(false),
	m_last_time_of_day_f(-1),
	m_time_of_day_update_timer(0),
	m_sleeping(false),
	m_waking(false),
	m_sleep_state(0.0)
{
	m_mesh_update_thread.m_env = &m_env;
	m_packetcounter_timer = 0.0;
	//m_delete_unused_sectors_timer = 0.0;
	m_connection_reinit_timer = 0.0;
	m_avg_rtt_timer = 0.0;
	m_playerpos_send_timer = 0.0;
	m_ignore_damage_timer = 0.0;

	//m_env_mutex.Init();
	//m_con_mutex.Init();

	m_mesh_update_thread.Start();

	/*
		Add local player
	*/
	{
		//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out

		Player *player = new LocalPlayer();

		player->updateName(playername);

		m_env.addPlayer(player);

		// Initialize player in the inventory context
		m_inventory_context.current_player = player;
	}
}

Client::~Client()
{
	{
		//JMutexAutoLock conlock(m_con_mutex); //bulk comment-out
		m_con.Disconnect();
	}

	m_mesh_update_thread.setRun(false);
	while(m_mesh_update_thread.IsRunning())
		sleep_ms(100);
}

void Client::connect(Address address)
{
	DSTACK(__FUNCTION_NAME);
	//JMutexAutoLock lock(m_con_mutex); //bulk comment-out
	m_con.SetTimeoutMs(0);
	m_con.Connect(address);
}

bool Client::connectedAndInitialized()
{
	//JMutexAutoLock lock(m_con_mutex); //bulk comment-out

	if(m_con.Connected() == false)
		return false;

	if(m_server_ser_ver == SER_FMT_VER_INVALID)
		return false;

	return true;
}

void Client::step(float dtime)
{
	DSTACK(__FUNCTION_NAME);

	// Limit a bit
	if(dtime > 2.0)
		dtime = 2.0;

	if (m_ignore_damage_timer > dtime) {
		m_ignore_damage_timer -= dtime;
	}else{
		m_ignore_damage_timer = 0.0;
	}

	if (m_sleeping) {
		m_sleep_state += dtime;
		if (m_sleep_state > 1.0)
			m_sleep_state = 1.0;
	}else if (m_waking) {
		m_sleep_state -= dtime;
		if (m_sleep_state < 0.5) {
			m_sleep_state = 0.0;
			m_waking = false;
		}
	}

	m_time_of_day_update_timer += dtime;

	//infostream<<"Client steps "<<dtime<<std::endl;

	{
		//TimeTaker timer("ReceiveAll()", m_device);
		// 0ms
		ReceiveAll();
	}

	{
		//TimeTaker timer("m_con_mutex + m_con.RunTimeouts()", m_device);
		// 0ms
		//JMutexAutoLock lock(m_con_mutex); //bulk comment-out
		m_con.RunTimeouts(dtime);
	}

	/*
		Packet counter
	*/
	{
		float &counter = m_packetcounter_timer;
		counter -= dtime;
		if (counter <= 0.0) {
			counter = 20.0;
			infostream<<"Client packetcounter (20s):"<<std::endl;
			m_packetcounter.print(infostream);
			m_packetcounter.clear();
		}
	}

	// Get connection status
	bool connected = connectedAndInitialized();

	if (connected == false) {
		float &counter = m_connection_reinit_timer;
		counter -= dtime;
		if (counter <= 0.0) {
			counter = 2.0;

			//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out

			Player *myplayer = m_env.getLocalPlayer();
			assert(myplayer != NULL);

			// Send TOSERVER_INIT
			// [0] u16 TOSERVER_INIT
			// [2] u8 SER_FMT_VER_HIGHEST
			// [3] u8[20] player_name
			// [23] u8[28] password (new in some version)
			// [51] u16 client network protocol version (new in some version)
			SharedBuffer<u8> data(2+1+PLAYERNAME_SIZE+PASSWORD_SIZE+2);
			writeU16(&data[0], TOSERVER_INIT);
			writeU8(&data[2], SER_FMT_VER_HIGHEST);

			memset((char*)&data[3], 0, PLAYERNAME_SIZE);
			snprintf((char*)&data[3], PLAYERNAME_SIZE, "%s", myplayer->getName());

			/*infostream<<"Client: sending initial password hash: \""<<m_password<<"\""
					<<std::endl;*/

			memset((char*)&data[23], 0, PASSWORD_SIZE);
			snprintf((char*)&data[23], PASSWORD_SIZE, "%s", m_password.c_str());

			// This should be incremented in each version
			writeU16(&data[51], PROTOCOL_VERSION);

			// Send as unreliable
			Send(0, data, false);
		}

		// Not connected, return
		return;
	}

	/*
		Do stuff if connected
	*/

	/*
		Run Map's timers and unload unused data
	*/
	const float map_timer_and_unload_dtime = 5.25;
	if (m_map_timer_and_unload_interval.step(dtime, map_timer_and_unload_dtime)) {
		ScopeProfiler sp(g_profiler, "Client: map timer and unload");
		core::list<v3s16> deleted_blocks;
		m_env.getMap().timerUpdate(map_timer_and_unload_dtime,
				g_settings->getFloat("client_unload_unused_data_timeout"),
				&deleted_blocks);

		/*if(deleted_blocks.size() > 0)
			infostream<<"Client: Unloaded "<<deleted_blocks.size()
					<<" unused blocks"<<std::endl;*/

		/*
			Send info to server
			NOTE: This loop is intentionally iterated the way it is.
		*/

		core::list<v3s16>::Iterator i = deleted_blocks.begin();
		core::list<v3s16> sendlist;
		for (;;) {
			if (sendlist.size() == 255 || i == deleted_blocks.end()) {
				if (sendlist.size() == 0)
					break;
				/*
					[0] u16 command
					[2] u8 count
					[3] v3s16 pos_0
					[3+6] v3s16 pos_1
					...
				*/
				u32 replysize = 2+1+6*sendlist.size();
				SharedBuffer<u8> reply(replysize);
				writeU16(&reply[0], TOSERVER_DELETEDBLOCKS);
				reply[2] = sendlist.size();
				u32 k = 0;
				for (core::list<v3s16>::Iterator j = sendlist.begin(); j != sendlist.end(); j++) {
					writeV3S16(&reply[2+1+6*k], *j);
					k++;
				}
				m_con.Send(PEER_ID_SERVER, 1, reply, true);

				if (i == deleted_blocks.end())
					break;

				sendlist.clear();
			}

			sendlist.push_back(*i);
			i++;
		}
	}

	/*
		Handle environment
	*/
	{
		// 0ms
		//JMutexAutoLock lock(m_env_mutex); //bulk comment-out

		// Control local player (0ms)
		LocalPlayer *player = m_env.getLocalPlayer();
		assert(player != NULL);
		player->applyControl(dtime);

		//TimeTaker envtimer("env step", m_device);
		// Step environment
		m_env.step(dtime);

		/*
			Get events
		*/
		for (;;) {
			ClientEnvEvent event = m_env.getClientEvent();
			if (event.type == CEE_NONE) {
				break;
			}else if (event.type == CEE_PLAYER_DAMAGE && getServerDamage()) {
				if (m_ignore_damage_timer <= 0) {
					s8 damage = event.player_damage.amount;
					sendDamage(damage,0,0);

					// Add to ClientEvent queue
					ClientEvent event;
					event.type = CE_PLAYER_DAMAGE;
					event.player_damage.amount = damage;
					m_client_event_queue.push_back(event);
				}
			}else if (event.type == CEE_PLAYER_SUFFOCATE && getServerSuffocation()) {
				if (m_ignore_damage_timer <= 0) {
					s8 damage = event.player_damage.amount;
					sendDamage(0,damage,0);

					// Add to ClientEvent queue
					ClientEvent event;
					event.type = CE_PLAYER_SUFFOCATE;
					// this will cause the damage screen to flash
					// when suffocation starts effecting damage
					if (getAir() < 1 && damage > 0)
						event.type = CE_PLAYER_DAMAGE;
					event.player_damage.amount = damage;
					m_client_event_queue.push_back(event);
				}
			}else if (event.type == CEE_PLAYER_HUNGER && getServerHunger()) {
				if (m_ignore_damage_timer <= 0) {
					s8 damage = event.player_damage.amount;
					sendDamage(0,0,damage);

					// Add to ClientEvent queue
					ClientEvent event;
					event.type = CE_PLAYER_HUNGER;
					// this will cause the damage screen to flash
					// when hunger starts effecting damage
					if (getHunger() < 1 && damage > 0)
						event.type = CE_PLAYER_DAMAGE;
					event.player_damage.amount = damage;
					m_client_event_queue.push_back(event);
				}
			}else if (event.type == CEE_PLAYER_WEARCLOTHES && getServerDamage()) {
				sendClothesWear(event.player_wear.amount);
			}
		}
	}

	/*
		Print some info
	*/
	{
		float &counter = m_avg_rtt_timer;
		counter += dtime;
		if (counter >= 10) {
			counter = 0.0;
			//JMutexAutoLock lock(m_con_mutex); //bulk comment-out
			// connectedAndInitialized() is true, peer exists.
			float avg_rtt = m_con.GetPeerAvgRTT(PEER_ID_SERVER);
			infostream<<"Client: avg_rtt="<<avg_rtt<<std::endl;
		}
	}

	/*
		Send player position to server
	*/
	{
		float &counter = m_playerpos_send_timer;
		counter += dtime;
		if (counter >= 0.2) {
			counter = 0.0;
			sendPlayerPos();
		}
	}

	/*
		Replace updated meshes
	*/
	{
		//JMutexAutoLock lock(m_env_mutex); //bulk comment-out

		//TimeTaker timer("** Processing mesh update result queue");
		// 0ms

		/*infostream<<"Mesh update result queue size is "
				<<m_mesh_update_thread.m_queue_out.size()
				<<std::endl;*/

		while (m_mesh_update_thread.m_queue_out.size() > 0) {
			MeshUpdateResult r = m_mesh_update_thread.m_queue_out.pop_front();
			MapBlock *block = m_env.getMap().getBlockNoCreateNoEx(r.p);
			if (block && r.mesh != NULL) {
				JMutexAutoLock lock(block->mesh_mutex);

				MapBlockMesh *mesh_old = block->mesh;
				block->mesh = r.mesh;
				block->setMeshExpired(false);

				if (mesh_old != NULL)
					delete mesh_old;
			}
			if (r.ack_block_to_server) {
				/*infostream<<"Client: ACK block ("<<r.p.X<<","<<r.p.Y
						<<","<<r.p.Z<<")"<<std::endl;*/
				/*
					Acknowledge block
				*/
				/*
					[0] u16 command
					[2] u8 count
					[3] v3s16 pos_0
					[3+6] v3s16 pos_1
					...
				*/
				u32 replysize = 2+1+6;
				SharedBuffer<u8> reply(replysize);
				writeU16(&reply[0], TOSERVER_GOTBLOCKS);
				reply[2] = 1;
				writeV3S16(&reply[3], r.p);
				// Send as reliable
				m_con.Send(PEER_ID_SERVER, 1, reply, true);
			}
		}
	}
}

// Virtual methods from con::PeerHandler
void Client::peerAdded(con::Peer *peer)
{
	infostream<<"Client::peerAdded(): peer->id="
			<<peer->id<<std::endl;
}
void Client::deletingPeer(con::Peer *peer, bool timeout)
{
	infostream<<"Client::deletingPeer(): "
			"Server Peer is getting deleted "
			<<"(timeout="<<timeout<<")"<<std::endl;
}

void Client::ReceiveAll()
{
	DSTACK(__FUNCTION_NAME);
	for(;;)
	{
		try{
			Receive();
		}
		catch(con::NoIncomingDataException &e)
		{
			break;
		}
		catch(con::InvalidIncomingDataException &e)
		{
			infostream<<"Client::ReceiveAll(): "
					"InvalidIncomingDataException: what()="
					<<e.what()<<std::endl;
		}
	}
}

void Client::Receive()
{
	DSTACK(__FUNCTION_NAME);
	SharedBuffer<u8> data;
	u16 sender_peer_id;
	u32 datasize;
	{
		//TimeTaker t1("con mutex and receive", m_device);
		//JMutexAutoLock lock(m_con_mutex); //bulk comment-out
		datasize = m_con.Receive(sender_peer_id, data);
	}
	//TimeTaker t1("ProcessData", m_device);
	ProcessData(*data, datasize, sender_peer_id);
}

/*
	sender_peer_id given to this shall be quaranteed to be a valid peer
*/
void Client::ProcessData(u8 *data, u32 datasize, u16 sender_peer_id)
{
	DSTACK(__FUNCTION_NAME);

	// Ignore packets that don't even fit a command
	if(datasize < 2)
	{
		m_packetcounter.add(60000);
		return;
	}

	ToClientCommand command = (ToClientCommand)readU16(&data[0]);

	//infostream<<"Client: received command="<<command<<std::endl;
	m_packetcounter.add((u16)command);

	/*
		If this check is removed, be sure to change the queue
		system to know the ids
	*/
	if(sender_peer_id != PEER_ID_SERVER)
	{
		infostream<<"Client::ProcessData(): Discarding data not "
				"coming from server: peer_id="<<sender_peer_id
				<<std::endl;
		return;
	}

	u8 ser_version = m_server_ser_ver;

	//infostream<<"Client received command="<<(int)command<<std::endl;

	if (command == TOCLIENT_INIT) {
		if (datasize < 3)
			return;

		u8 deployed = data[2];

		infostream<<"Client: TOCLIENT_INIT received with "
				"deployed="<<((int)deployed&0xff)<<std::endl;

		if (deployed < SER_FMT_VER_LOWEST || deployed > SER_FMT_VER_HIGHEST) {
			infostream<<"Client: TOCLIENT_INIT: Server sent "
					<<"unsupported ser_fmt_ver"<<std::endl;
			return;
		}

		m_server_ser_ver = deployed;

		// Get player position
		v3s16 playerpos_s16(0, BS*2+BS*20, 0);
		if (datasize >= 2+1+6)
			playerpos_s16 = readV3S16(&data[2+1]);
		v3f playerpos_f = intToFloat(playerpos_s16, BS) - v3f(0, BS/2, 0);

		{ //envlock
			//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out

			// Set player position
			Player *player = m_env.getLocalPlayer();
			assert(player != NULL);
			player->setPosition(playerpos_f);
		}

		if (datasize >= 2+1+6+8) {
			// Get map seed
			m_map_seed = readU64(&data[2+1+6]);
			infostream<<"Client: received map seed: "<<m_map_seed<<std::endl;
		}

		if (datasize >= 2+1+6+8+2) {
			// Get map seed
			u16 type = readU16(&data[2+1+6+8]);
			m_map_type = (MapGenType)type;
			infostream<<"Client: received map type: "<<m_map_type<<std::endl;
		}

		{
			// Reply to server
			std::string chardef = std::string(PLAYER_DEFAULT_CHARDEF);
			if (g_settings->exists("character_definition"))
				chardef = g_settings->get("character_definition");

			std::ostringstream os(std::ios_base::binary);
			u8 buf[12];

			// Write command
			writeU16(buf, TOSERVER_INIT2);
			os.write((char*)buf, 2);

			// Write chardef
			os<<serializeString(chardef);

			// Make data buffer
			std::string s = os.str();
			SharedBuffer<u8> data((u8*)s.c_str(), s.size());
			// Send as reliable
			m_con.Send(PEER_ID_SERVER, 1, data, true);
		}
		return;
	}

	if (command == TOCLIENT_ACCESS_DENIED) {
		// The server didn't like our password. Note, this needs
		// to be processed even if the serialisation format has
		// not been agreed yet, the same as TOCLIENT_INIT.
		m_access_denied = true;
		m_access_denied_reason = L"Unknown";
		if (datasize >= 4) {
			std::string datastring((char*)&data[2], datasize-2);
			std::istringstream is(datastring, std::ios_base::binary);
			m_access_denied_reason = deSerializeWideString(is);
		}
		return;
	}

	if (ser_version == SER_FMT_VER_INVALID) {
		infostream<<"Client: Server serialization"
				" format invalid or not initialized."
				" Skipping incoming command="<<command<<std::endl;
		return;
	}

	// Just here to avoid putting the two if's together when
	// making some copypasta
	{}

	switch (command) {
	case TOCLIENT_REMOVENODE:
	{
		if(datasize < 8)
			return;
		v3s16 p;
		p.X = readS16(&data[2]);
		p.Y = readS16(&data[4]);
		p.Z = readS16(&data[6]);

		//TimeTaker t1("TOCLIENT_REMOVENODE");

		// This will clear the cracking animation after digging
		((ClientMap&)m_env.getMap()).clearTempMod(p);

		removeNode(p);
	}
	break;
	case TOCLIENT_ADDNODE:
	{
		if(datasize < 8 + MapNode::serializedLength(ser_version))
			return;

		v3s16 p;
		p.X = readS16(&data[2]);
		p.Y = readS16(&data[4]);
		p.Z = readS16(&data[6]);

		//TimeTaker t1("TOCLIENT_ADDNODE");

		MapNode n;
		n.deSerialize(&data[8], ser_version);

		addNode(p, n);
	}
	break;
	case TOCLIENT_BLOCKDATA:
	{
		// Ignore too small packet
		if(datasize < 8)
			return;

		v3s16 p;
		p.X = readS16(&data[2]);
		p.Y = readS16(&data[4]);
		p.Z = readS16(&data[6]);

		/*infostream<<"Client: Thread: BLOCKDATA for ("
				<<p.X<<","<<p.Y<<","<<p.Z<<")"<<std::endl;*/
		/*infostream<<"Client: Thread: BLOCKDATA for ("
				<<p.X<<","<<p.Y<<","<<p.Z<<")"<<std::endl;*/

		std::string datastring((char*)&data[8], datasize-8);
		std::istringstream istr(datastring, std::ios_base::binary);

		MapSector *sector;
		MapBlock *block;

		v2s16 p2d(p.X, p.Z);
		sector = m_env.getMap().emergeSector(p2d);

		if (sector->getPos() != p2d)
			return;

		//TimeTaker timer("MapBlock deSerialize");
		// 0ms

		block = sector->getBlockNoCreateNoEx(p.Y);
		if (block) {
			/*
				Update an existing block
			*/
			//infostream<<"Updating"<<std::endl;
			block->deSerialize(istr, ser_version);
		}else{
			/*
				Create a new block
			*/
			//infostream<<"Creating new"<<std::endl;
			block = new MapBlock(&m_env.getMap(), p);
			block->deSerialize(istr, ser_version);
			sector->insertBlock(block);

		}

		/*
			Update Mesh of this block and blocks at x-, y- and z-.
			Environment should not be locked as it interlocks with the
			main thread, from which is will want to retrieve textures.
		*/

		/*
			Add it to mesh update queue and set it to be acknowledged after update.
		*/
		addUpdateMeshTaskWithEdge(p, true);
	}
	break;
	case TOCLIENT_SERVERSETTINGS:
	{
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);

		u8 damage = readU8(is);
		u8 suffocation = readU8(is);
		u8 hunger = readU8(is);

		setServerSettings(damage,suffocation,hunger);

		if (!damage)
			m_env.getLocalPlayer()->setIgnoreEnergy(true);
	}
	break;
	case TOCLIENT_PLAYERINFO:
	{
		u16 our_peer_id = m_con.GetPeerID();
		// Cancel if we don't have a peer id
		if (our_peer_id == PEER_ID_INEXISTENT) {
			infostream<<"TOCLIENT_PLAYERINFO cancelled: we have no peer id"<<std::endl;
			return;
		}
		{
			u32 item_size = 2+PLAYERNAME_SIZE;
			u32 player_count = (datasize-2) / item_size;
			u32 start = 2;
			// peer_ids
			core::list<u16> players_alive;
			for (u32 i=0; i<player_count; i++) {
				// Make sure the name ends in '\0'
				data[start+2+20-1] = 0;
				u16 peer_id = readU16(&data[start]);
				players_alive.push_back(peer_id);
				// Don't update the info of the local player
				if (peer_id == our_peer_id) {
					start += item_size;
					continue;
				}
				Player *player = m_env.getPlayer(peer_id);
				// Create a player if it doesn't exist
				if (player == NULL) {
					player = new RemotePlayer(
						m_device->getSceneManager()->getRootSceneNode(),
						m_device,
						-1
					);
					player->peer_id = peer_id;
					m_env.addPlayer(player);
					infostream<<"Client: Adding new player "<<peer_id<<std::endl;
				}
				player->updateName((char*)&data[start+2]);
				start += item_size;
			}
			/*
			Remove those players from the environment that
			weren't listed by the server.
			*/
			core::list<Player*> players = m_env.getPlayers();
			core::list<Player*>::Iterator ip;
			for (ip=players.begin(); ip!=players.end(); ip++) {
				// Ignore local player
				if ((*ip)->isLocal())
					continue;
				// Warn about a special case
				if ((*ip)->peer_id == 0)
					infostream<<"Client: Removing dead player with id=0"<<std::endl;
				bool is_alive = false;
				core::list<u16>::Iterator i;
				for (i=players_alive.begin(); i!=players_alive.end(); i++) {
					if ((*ip)->peer_id == *i) {
						is_alive = true;
						break;
					}
				}
				if (is_alive)
					continue;
				infostream<<"Removing dead player "<<(*ip)->peer_id<<std::endl;
				m_env.removePlayer((*ip)->peer_id);
			}
		} //envlock
	}
	break;
	case TOCLIENT_PLAYERDATA:
	{
		u16 our_peer_id = m_con.GetPeerID();

		// Cancel if we don't have a peer id
		if (our_peer_id == PEER_ID_INEXISTENT) {
			infostream<<"TOCLIENT_PLAYERINFO cancelled: we have no peer id"<<std::endl;
			return;
		}

		{
			std::string datastring((char*)&data[2], datasize-2);
			std::istringstream is(datastring, std::ios_base::binary);
			char pname[PLAYERNAME_SIZE];
			std::string chardef;

			u16 player_count = readU16(is);
			u16 field_count = readU16(is);

			// peer_ids
			core::list<u16> players_alive;
			for (u16 i=0; i<player_count; i++) {
				u16 peer_id = readU16(is);

				players_alive.push_back(peer_id);
				is.read(pname,PLAYERNAME_SIZE);
				pname[PLAYERNAME_SIZE-1] = '\0';
				chardef = deSerializeString(is);
				for (u16 j=1; j<field_count; j++) {
					std::string dump = deSerializeString(is);
				}

				// Don't update the info of the local player
				if (peer_id == our_peer_id)
					continue;

				Player *player = m_env.getPlayer(peer_id);

				// Create a player if it doesn't exist
				if (player == NULL) {
					player = new RemotePlayer(
						m_device->getSceneManager()->getRootSceneNode(),
						m_device,
						-1
					);
					player->peer_id = peer_id;
					m_env.addPlayer(player);
					infostream<<"Client: Adding new player "<<peer_id<<std::endl;
				}

				player->setCharDef(chardef);
				player->updateName(pname);
			}

			/*
				Remove those players from the environment that
				weren't listed by the server.
			*/
			core::list<Player*> players = m_env.getPlayers();
			core::list<Player*>::Iterator ip;
			for (ip=players.begin(); ip!=players.end(); ip++) {
				// Ignore local player
				if ((*ip)->isLocal())
					continue;

				// Warn about a special case
				if ((*ip)->peer_id == 0)
					infostream<<"Client: Removing dead player with id=0"<<std::endl;

				bool is_alive = false;
				core::list<u16>::Iterator i;
				for (i=players_alive.begin(); i!=players_alive.end(); i++) {
					if ((*ip)->peer_id == *i) {
						is_alive = true;
						break;
					}
				}
				if (is_alive)
					continue;
				infostream<<"Removing dead player "<<(*ip)->peer_id
						<<std::endl;
				m_env.removePlayer((*ip)->peer_id);
			}
		} //envlock
	}
	break;
	case TOCLIENT_PLAYER_ANIMATION:
	{
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);
		u16 peer_id = readU16(is);
		u8 anim_id = readU8(is);
		content_t pointed = readU16(is);

		Player *player = m_env.getPlayer(peer_id);

		// Create a player if it doesn't exist
		if (player == NULL) {
			player = new RemotePlayer(
					m_device->getSceneManager()->getRootSceneNode(),
					m_device,
					-1);
			player->peer_id = peer_id;
			m_env.addPlayer(player);
			infostream<<"Client: Adding new player " <<peer_id<<std::endl;
		}

		player->updateAnim(anim_id,pointed);
	}
	break;
	case TOCLIENT_PLAYERHP:
	{
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);
		LocalPlayer *player = m_env.getLocalPlayer();
		assert(player != NULL);
		u8 hp = readU8(is);
		u8 air = readU8(is);
		u8 hunger = readU8(is);
		u16 energy_effect = readU16(is);
		if (energy_effect > player->energy_effectf)
			player->energy_effectf = energy_effect;
		player->cold_effectf += readU16(is);
		if (m_server_damage) {
			if (!player->hp)
				player->setEnergy(hp);
			player->hp = hp;
		}
		if (m_server_suffocation)
			player->air = air;
		if (m_server_hunger)
			player->hunger = hunger;
	}
	break;
	case TOCLIENT_INVENTORY:
	{
		if (datasize < 3)
			return;
		{
			std::string datastring((char*)&data[2], datasize-2);
			std::istringstream is(datastring, std::ios_base::binary);

			Player *player = m_env.getLocalPlayer();
			assert(player != NULL);

			player->inventory.deSerialize(is);

			m_inventory_updated = true;
		}
	}
	break;
	case TOCLIENT_INVENTORY_UPDATE:
	{
		if (datasize < 3)
			return;
		{
			std::string datastring((char*)&data[2], datasize-2);
			std::istringstream is(datastring, std::ios_base::binary);

			Player *player = m_env.getLocalPlayer();
			assert(player != NULL);

			u16 list_count = readU16(is);
			for (int i=0; i<list_count; i++) {
				std::string name = deSerializeString(is);
				u16 slots = readU16(is);
				InventoryList *l = player->inventory.getList(name);
				if (!l)
					return;
				for (int k=0; k<slots; k++) {
					u16 index = readU16(is);
					u16 type = readU16(is);
					u16 count = readU16(is);
					u16 data = readU16(is);
					l->updateItem(index,type,count,data);
				}
			}

			m_inventory_updated = true;
		}
	}
	//DEBUG
	break;
	case TOCLIENT_OBJECTDATA:
	{
		// Strip command word and create a stringstream
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);

		u8 buf[12];

		/*
			Read players
		*/

		is.read((char*)buf, 2);
		u16 playercount = readU16(buf);

		for (u16 i=0; i<playercount; i++) {
			is.read((char*)buf, 2);
			u16 peer_id = readU16(buf);
			is.read((char*)buf, 12);
			v3s32 p_i = readV3S32(buf);
			is.read((char*)buf, 12);
			v3s32 s_i = readV3S32(buf);
			is.read((char*)buf, 4);
			s32 pitch_i = readS32(buf);
			is.read((char*)buf, 4);
			s32 yaw_i = readS32(buf);

			Player *player = m_env.getPlayer(peer_id);

			// Skip if player doesn't exist
			if (player == NULL)
				continue;

			// Skip if player is local player
			if (player->isLocal())
				continue;

			f32 pitch = (f32)pitch_i / 100.0;
			f32 yaw = (f32)yaw_i / 100.0;
			v3f position((f32)p_i.X/100., (f32)p_i.Y/100., (f32)p_i.Z/100.);
			v3f speed((f32)s_i.X/100., (f32)s_i.Y/100., (f32)s_i.Z/100.);

			player->setPosition(position);
			player->setSpeed(speed);
			player->setPitch(pitch);
			player->setYaw(yaw);
		}

		/*
			Read block objects
			NOTE: Deprecated stuff
		*/

		// Read active block count
		u16 blockcount = readU16(is);
		if (blockcount != 0) {
			infostream<<"TOCLIENT_OBJECTDATA: blockcount != 0 not supported"<<std::endl;
			return;
		}
	}
	break;
	case TOCLIENT_TIME_OF_DAY:
	{
		if(datasize < 4)
			return;

		u32 time = 0;
		u16 time_of_day = readU16(&data[2]);
		time_of_day = time_of_day % 24000;
		float time_speed = 0;
		if (datasize >= 2 + 2 + 4 + 4) {
			time = readU32(&data[4]);
			time_speed = readF1000(&data[8]);
		}else{
			// Old message; try to approximate speed of time by ourselves
			float time_of_day_f = (float)time_of_day / 24000.0;
			float tod_diff_f = 0;
			if (time_of_day_f < 0.2 && m_last_time_of_day_f > 0.8) {
				tod_diff_f = time_of_day_f - m_last_time_of_day_f + 1.0;
			}else{
				tod_diff_f = time_of_day_f - m_last_time_of_day_f;
			}
			m_last_time_of_day_f = time_of_day_f;
			float time_diff = m_time_of_day_update_timer;
			m_time_of_day_update_timer = 0;
			if (m_time_of_day_set) {
				time_speed = 3600.0*24.0 * tod_diff_f / time_diff;
				infostream<<"Client: Measured time_of_day speed (old format): "
				<<time_speed<<" tod_diff_f="<<tod_diff_f
				<<" time_diff="<<time_diff<<std::endl;
			}
		}

		// Update environment
		m_env.setTimeOfDay(time_of_day);
		m_env.setTimeOfDaySpeed(time_speed);
		m_env.setTime(time);
		m_time_of_day_set = true;
		u32 dr = m_env.getDayNightRatio();
		verbosestream<<"Client: time_of_day="<<time_of_day
			<<" time_speed="<<time_speed
			<<" dr="<<dr<<std::endl;
	}
	break;
	case TOCLIENT_CHAT_MESSAGE:
	{
		/*
			u16 command
			u16 length
			wstring message
		*/
		u8 buf[6];
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);

		// Read stuff
		is.read((char*)buf, 2);
		u16 len = readU16(buf);

		std::wstring message;
		for (u16 i=0; i<len; i++) {
			is.read((char*)buf, 2);
			message += (wchar_t)readU16(buf);
		}

		/* because I can't remember which random stream prints to stdout */
		printf("CHAT: %s\n",wide_to_narrow(message).c_str());

		m_chat_queue.push_back(message);
	}
	break;
	case TOCLIENT_ACTIVE_OBJECT_REMOVE_ADD:
	{
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

		char buf[6];
		// Get all data except the command number
		std::string datastring((char*)&data[2], datasize-2);
		// Throw them in an istringstream
		std::istringstream is(datastring, std::ios_base::binary);

		// Read stuff

		// Read removed objects
		is.read(buf, 2);
		u16 removed_count = readU16((u8*)buf);
		for (u16 i=0; i<removed_count; i++) {
			is.read(buf, 2);
			u16 id = readU16((u8*)buf);
			// Remove it
			m_env.removeActiveObject(id);
		}

		// Read added objects
		is.read(buf, 2);
		u16 added_count = readU16((u8*)buf);
		for (u16 i=0; i<added_count; i++) {
			is.read(buf, 2);
			u16 id = readU16((u8*)buf);
			is.read(buf, 1);
			u8 type = readU8((u8*)buf);
			std::string data = deSerializeLongString(is);
			// Add it
			m_env.addActiveObject(id, type, data);
		}
	}
	break;
	case TOCLIENT_ACTIVE_OBJECT_MESSAGES:
	{
		/*
			u16 command
			for all objects
			{
				u16 id
				u16 message length
				string message
			}
		*/
		char buf[6];
		// Get all data except the command number
		std::string datastring((char*)&data[2], datasize-2);
		// Throw them in an istringstream
		std::istringstream is(datastring, std::ios_base::binary);

		while (is.eof() == false) {
			// Read stuff
			is.read(buf, 2);
			u16 id = readU16((u8*)buf);
			if (is.eof())
				break;
			is.read(buf, 2);
			u16 message_size = readU16((u8*)buf);
			std::string message;
			message.reserve(message_size);
			for (u16 i=0; i<message_size; i++) {
				is.read(buf, 1);
				message.append(buf, 1);
			}
			// Pass on to the environment
			m_env.processActiveObjectMessage(id, message);
		}
	}
	break;
	case TOCLIENT_HP:
	{
		infostream<<"Client received DEPRECATED TOCLIENT_HP"<<std::endl;
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);
		Player *player = m_env.getLocalPlayer();
		assert(player != NULL);
		u8 hp = readU8(is);
		player->hp = hp;
	}
	break;
	case TOCLIENT_MOVE_PLAYER:
	{
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);
		Player *player = m_env.getLocalPlayer();
		assert(player != NULL);
		v3f pos = readV3F1000(is);
		f32 pitch = readF1000(is);
		f32 yaw = readF1000(is);
		player->setPosition(pos);
		/*player->setPitch(pitch);
		player->setYaw(yaw);*/

		infostream<<"Client got TOCLIENT_MOVE_PLAYER"
				<<" pos=("<<pos.X<<","<<pos.Y<<","<<pos.Z<<")"
				<<" pitch="<<pitch
				<<" yaw="<<yaw
				<<std::endl;

		/*
			Add to ClientEvent queue.
			This has to be sent to the main program because otherwise
			it would just force the pitch and yaw values to whatever
			the camera points to.
		*/
		ClientEvent event;
		event.type = CE_PLAYER_FORCE_MOVE;
		event.player_force_move.pitch = pitch;
		event.player_force_move.yaw = yaw;
		m_client_event_queue.push_back(event);

		// Ignore damage for a few seconds, so that the player doesn't
		// get damage from falling on ground
		m_ignore_damage_timer = 3.0;
	}
	break;
	case TOCLIENT_PLAYERITEM:
	{
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);

		u16 count = readU16(is);

		for (u16 i = 0; i < count; ++i) {
			u16 peer_id = readU16(is);
			Player *player = m_env.getPlayer(peer_id);

			if (player == NULL) {
				infostream<<"Client: ignoring player item "
					<< deSerializeString(is)
					<< " for non-existing peer id " << peer_id
					<< std::endl;
				continue;
			}else if (player->isLocal()) {
				infostream<<"Client: ignoring player item "
					<< deSerializeString(is)
					<< " for local player" << std::endl;
				continue;
			}else{
				InventoryList *inv = player->inventory.getList("main");
				std::string itemstring(deSerializeString(is));
				if (itemstring.empty()) {
					inv->deleteItem(0);
					infostream
						<<"Client: empty player item for peer "
						<< peer_id << std::endl;
				}else{
					std::istringstream iss(itemstring);
					delete inv->changeItem(0, InventoryItem::deSerialize(iss));
					infostream<<"Client: player item for peer " << peer_id << ": ";
					player->getWieldItem()->serialize(infostream);
					infostream<<std::endl;
				}
				player->wieldItem(0);
			}
		}
	}
	break;
	case TOCLIENT_PLAYERITEMS:
	{
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);

		u16 count = readU16(is);
		u16 icount = readU16(is);
		u16 itm;

		for (u16 i = 0; i < count; ++i) {
			u16 peer_id = readU16(is);
			Player *player = m_env.getPlayer(peer_id);

			if (player == NULL) {
				for (int j=0; j<icount; j++) {
					itm = readU16(is);
				}
				infostream<<"Client: ignoring player items "
					<< " for non-existing peer id " << peer_id
					<< std::endl;
				continue;
			}else if (player->isLocal()) {
				for (int j=0; j<icount; j++) {
					itm = readU16(is);
				}
				infostream<<"Client: ignoring player items "
					<< " for local player" << std::endl;
				continue;
			}else{
				for (int j=0; j<icount; j++) {
					itm = readU16(is);
					InventoryList *inv = NULL;
					switch (j) {
					case 0:
						inv = player->inventory.getList("main");
						break;
					case 1:
						inv = player->inventory.getList("hat");
						break;
					case 2:
						inv = player->inventory.getList("shirt");
						break;
					case 3:
						inv = player->inventory.getList("pants");
						break;
					case 4:
						inv = player->inventory.getList("boots");
						break;
					case 5:
						inv = player->inventory.getList("decorative");
						break;
					case 6:
						inv = player->inventory.getList("jacket");
						break;
					case 7:
						inv = player->inventory.getList("belt");
						break;
					}
					if (inv == NULL)
						continue;
					if (itm == CONTENT_IGNORE) {
						inv->deleteItem(0);
					}else{
						InventoryItem *item = inv->changeItem(0, InventoryItem::create(itm,1));
						if (item)
							delete item;
					}
				}
				// for 1409 servers, this stops players appearing naked
				if (icount == 5) {
					{
						InventoryList *inv = player->inventory.getList("shirt");
						if (!inv->getItem(0)) {
							InventoryItem *item = inv->changeItem(
								0,
								InventoryItem::create(CONTENT_CLOTHESITEM_COTTON_TSHIRT_GREEN,1)
							);
							if (item)
								delete item;
						}
					}
					{
						InventoryList *inv = player->inventory.getList("pants");
						if (!inv->getItem(0)) {
							InventoryItem *item = inv->changeItem(
								0,
								InventoryItem::create(CONTENT_CLOTHESITEM_CANVAS_PANTS_BLUE,1)
							);
							if (item)
								delete item;
						}
					}
					{
						InventoryList *inv = player->inventory.getList("boots");
						if (!inv->getItem(0)) {
							InventoryItem *item = inv->changeItem(
								0,
								InventoryItem::create(CONTENT_CLOTHESITEM_LEATHER_SHOES,1)
							);
							if (item)
								delete item;
						}
					}
				}
				player->wieldItem(0);
				player->updateName(NULL);
			}
		}
	}
	break;
	case TOCLIENT_DEATHSCREEN:
	{
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);

		bool set_camera_point_target = readU8(is);
		v3f camera_point_target = readV3F1000(is);

		ClientEvent event;
		event.type = CE_DEATHSCREEN;
		event.deathscreen.set_camera_point_target = set_camera_point_target;
		event.deathscreen.camera_point_target_x = camera_point_target.X;
		event.deathscreen.camera_point_target_y = camera_point_target.Y;
		event.deathscreen.camera_point_target_z = camera_point_target.Z;
		m_client_event_queue.push_back(event);
	}
	break;
	case TOCLIENT_ENV_EVENT:
	{
		/*
			u16 command
			u8 event type (sound,nodemod,particles,etc)
			v3f1000 event position
			u16 length of serialised event data
			string serialised event data
		*/
		std::string datastring((char*)&data[2], datasize-2);
		std::istringstream is(datastring, std::ios_base::binary);

		u8 type = readU8(is);
		v3f pos = readV3F1000(is);
		std::string ev = deSerializeString(is);
		switch (type) {
		case ENV_EVENT_SOUND: // sound
			playSoundAt(ev,pos,false);
			break;
		case ENV_EVENT_DIG_PARTICLES: // dig particles
			break;
		case ENV_EVENT_PUNCH_PARTICLES: // punch particles
			break;
		case ENV_EVENT_NODE_PARTICLES: // node particles
			break;
		case ENV_EVENT_SLEEP: // go to sleep
			m_sleeping = true;
			m_waking = false;
			m_sleep_state = 0.5;
			break;
		case ENV_EVENT_WAKE: // wake up
			m_sleeping = false;
			m_waking = true;
			m_sleep_state = 1.0;
			break;
		default:
			infostream<<"Client: unknown env_event: "<<type<<": "<<ev<<std::endl;
		}
	}
	break;
	default:
	{
		infostream<<"Client: Ignoring unknown command "
				<<command<<std::endl;
	}
	}
}

void Client::Send(u16 channelnum, SharedBuffer<u8> data, bool reliable)
{
	//JMutexAutoLock lock(m_con_mutex); //bulk comment-out
	m_con.Send(PEER_ID_SERVER, channelnum, data, reliable);
}

void Client::groundAction(u8 action, v3s16 nodepos_undersurface,
		v3s16 nodepos_oversurface, u16 item)
{
	if(connectedAndInitialized() == false){
		infostream<<"Client::groundAction() "
				"cancelled (not connected)"
				<<std::endl;
		return;
	}

	/*
		length: 17
		[0] u16 command
		[2] u8 action
		[3] v3s16 nodepos_undersurface
		[9] v3s16 nodepos_abovesurface
		[15] u16 item
		actions:
		0: start digging
		1: place block
		2: stop digging (all parameters ignored)
		3: digging completed
	*/
	u8 datasize = 2 + 1 + 6 + 6 + 2;
	SharedBuffer<u8> data(datasize);
	writeU16(&data[0], TOSERVER_GROUND_ACTION);
	writeU8(&data[2], action);
	writeV3S16(&data[3], nodepos_undersurface);
	writeV3S16(&data[9], nodepos_oversurface);
	writeU16(&data[15], item);
	Send(0, data, true);
	if (action == 3) {
		content_t c = m_env.getMap().getNodeNoEx(nodepos_undersurface).getContent();
		playDigSound(c);
	}
}

void Client::throwItem(v3f dir, u16 item)
{
	std::ostringstream os(std::ios_base::binary);
	u8 buf[15];

	LocalPlayer* player = m_env.getLocalPlayer();

	// Write command
	writeU16(buf, TOSERVER_THROWITEM);
	os.write((char*)buf, 2);

	// Write position - this is one node ahead of the player
	// to prevent damage occurring to the thrower
	v3f pf = player->getEyePosition()+(dir*BS);
	v3s32 position(pf.X*100, pf.Y*100, pf.Z*100);
	writeV3S32(buf,position);
	os.write((char*)buf, 12);

	// Write speed/direction
	v3s32 dir_i(dir.X*100, dir.Y*100, dir.Z*100);
	writeV3S32(buf,dir_i);
	os.write((char*)buf, 12);

	// Write item
	writeU16(buf, item);
	os.write((char*)buf, 2);

	// Make data buffer
	std::string s = os.str();
	SharedBuffer<u8> data((u8*)s.c_str(), s.size());
	// Send as reliable
	Send(0, data, true);
}

void Client::useItem()
{
	u8 buf[2];
	// Write command
	writeU16(buf, TOSERVER_USEITEM);
	SharedBuffer<u8> data(buf, 2);
	// Send as reliable
	Send(0, data, true);

	if (g_sound) {
		InventoryItem *item = (InventoryItem*)m_env.getLocalPlayer()->getWieldItem();
		if (!item)
			return;
		std::string snd("");
		content_t w = item->getContent();
		if ((w&CONTENT_CRAFTITEM_MASK) == CONTENT_CRAFTITEM_MASK)
			snd = content_craftitem_features(w).sound_use;
		if (snd != "")
			g_sound->playSound(snd,false);
	}
}

void Client::clickActiveObject(u8 button, u16 id, u16 item_i)
{
	if (connectedAndInitialized() == false) {
		infostream<<"Client::clickActiveObject() "
				"cancelled (not connected)"
				<<std::endl;
		return;
	}

	Player *player = m_env.getLocalPlayer();
	if (player == NULL)
		return;

	ClientActiveObject *obj = m_env.getActiveObject(id);
	if (obj) {
		if (button == 0) {
			content_t punch_item = CONTENT_IGNORE;

			InventoryList *mlist = player->inventory.getList("main");
			if (mlist != NULL) {
				InventoryItem *item = mlist->getItem(item_i);
				if (item)
					punch_item = item->getContent();
			}

			v3f playerpos = player->getPosition();
			v3f objpos = obj->getPosition();
			v3f dir = (objpos - playerpos).normalize();

			if (obj->directReportPunch(punch_item, dir))
				return;
		}
	}

	/*
		length: 7
		[0] u16 command
		[2] u8 button (0=left, 1=right)
		[3] u16 id
		[5] u16 item
	*/
	u8 datasize = 2 + 1 + 6 + 2 + 2;
	SharedBuffer<u8> data(datasize);
	writeU16(&data[0], TOSERVER_CLICK_ACTIVEOBJECT);
	writeU8(&data[2], button);
	writeU16(&data[3], id);
	writeU16(&data[5], item_i);
	Send(0, data, true);
}

void Client::sendNodemetaFields(v3s16 p, const std::string &formname,
		const std::map<std::string, std::wstring> &fields)
{
	std::ostringstream os(std::ios_base::binary);
	u8 buf[12];

	// Write command
	writeU16(buf, TOSERVER_NODEMETA_FIELDS);
	os.write((char*)buf, 2);

	// Write p
	writeV3S16(buf, p);
	os.write((char*)buf, 6);

	os<<serializeString(formname);

	// Write number of fields
	writeU16(buf, fields.size());
	os.write((char*)buf, 2);
	for (std::map<std::string, std::wstring>::const_iterator i = fields.begin(); i != fields.end(); i++) {
		const std::string &name = i->first;
		const std::wstring &value = i->second;
		os<<serializeString(name);
		os<<serializeLongString(wide_to_narrow(value));
	}

	// Make data buffer
	std::string s = os.str();
	SharedBuffer<u8> data((u8*)s.c_str(), s.size());
	// Send as reliable
	Send(0, data, true);
}

void Client::sendInventoryAction(InventoryAction *a)
{
	std::ostringstream os(std::ios_base::binary);
	u8 buf[12];

	// Write command
	writeU16(buf, TOSERVER_INVENTORY_ACTION);
	os.write((char*)buf, 2);

	a->serialize(os);

	// Make data buffer
	std::string s = os.str();
	SharedBuffer<u8> data((u8*)s.c_str(), s.size());
	// Send as reliable
	Send(0, data, true);
}

void Client::sendChatMessage(const std::wstring &message)
{
	std::ostringstream os(std::ios_base::binary);
	u8 buf[12];

	// Write command
	writeU16(buf, TOSERVER_CHAT_MESSAGE);
	os.write((char*)buf, 2);

	// Write length
	writeU16(buf, message.size());
	os.write((char*)buf, 2);

	// Write string
	for(u32 i=0; i<message.size(); i++)
	{
		u16 w = message[i];
		writeU16(buf, w);
		os.write((char*)buf, 2);
	}

	// Make data buffer
	std::string s = os.str();
	SharedBuffer<u8> data((u8*)s.c_str(), s.size());
	// Send as reliable
	Send(0, data, true);
}

void Client::sendChangePassword(const std::wstring oldpassword,
		const std::wstring newpassword)
{
	Player *player = m_env.getLocalPlayer();
	if(player == NULL)
		return;

	std::string playername = player->getName();
	std::string oldpwd = translatePassword(playername, oldpassword);
	std::string newpwd = translatePassword(playername, newpassword);

	std::ostringstream os(std::ios_base::binary);
	u8 buf[2+PASSWORD_SIZE*2];
	/*
		[0] u16 TOSERVER_PASSWORD
		[2] u8[28] old password
		[30] u8[28] new password
	*/

	writeU16(buf, TOSERVER_PASSWORD);
	for(u32 i=0;i<PASSWORD_SIZE-1;i++)
	{
		buf[2+i] = i<oldpwd.length()?oldpwd[i]:0;
		buf[30+i] = i<newpwd.length()?newpwd[i]:0;
	}
	buf[2+PASSWORD_SIZE-1] = 0;
	buf[30+PASSWORD_SIZE-1] = 0;
	os.write((char*)buf, 2+PASSWORD_SIZE*2);

	// Make data buffer
	std::string s = os.str();
	SharedBuffer<u8> data((u8*)s.c_str(), s.size());
	// Send as reliable
	Send(0, data, true);
}


void Client::sendDamage(s8 damage,s8 suffocate,s8 hunger)
{
	DSTACK(__FUNCTION_NAME);
	std::ostringstream os(std::ios_base::binary);

	writeU16(os, TOSERVER_PLAYERDAMAGE);
	writeS8(os, damage);
	writeS8(os, suffocate);
	writeS8(os, hunger);

	// Make data buffer
	std::string s = os.str();
	SharedBuffer<u8> data((u8*)s.c_str(), s.size());
	// Send as reliable
	Send(0, data, true);
}

void Client::sendClothesWear(u16 wear)
{
	std::ostringstream os(std::ios_base::binary);

	writeU16(os, TOSERVER_PLAYERWEAR);
	writeU16(os,wear);

	// Make data buffer
	std::string s = os.str();
	SharedBuffer<u8> data((u8*)s.c_str(), s.size());
	// Send as reliable
	Send(0, data, true);
}

void Client::sendRespawn()
{
	DSTACK(__FUNCTION_NAME);
	std::ostringstream os(std::ios_base::binary);

	writeU16(os, TOSERVER_RESPAWN);

	// Make data buffer
	std::string s = os.str();
	SharedBuffer<u8> data((u8*)s.c_str(), s.size());
	// Send as reliable
	Send(0, data, true);
}

void Client::sendPlayerPos()
{
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out

	Player *myplayer = m_env.getLocalPlayer();
	if(myplayer == NULL)
		return;

	u16 our_peer_id;
	{
		//JMutexAutoLock lock(m_con_mutex); //bulk comment-out
		our_peer_id = m_con.GetPeerID();
	}

	// Set peer id if not set already
	if(myplayer->peer_id == PEER_ID_INEXISTENT)
		myplayer->peer_id = our_peer_id;
	// Check that an existing peer_id is the same as the connection's
	assert(myplayer->peer_id == our_peer_id);

	v3f pf = myplayer->getPosition();
	v3s32 position(pf.X*100, pf.Y*100, pf.Z*100);
	v3f sf = myplayer->getSpeed();
	v3s32 speed(sf.X*100, sf.Y*100, sf.Z*100);
	s32 pitch = myplayer->getPitch() * 100;
	s32 yaw = myplayer->getYaw() * 100;

	/*
		Format:
		[0] u16 command
		[2] v3s32 position*100
		[2+12] v3s32 speed*100
		[2+12+12] s32 pitch*100
		[2+12+12+4] s32 yaw*100
	*/

	SharedBuffer<u8> data(2+12+12+4+4);
	writeU16(&data[0], TOSERVER_PLAYERPOS);
	writeV3S32(&data[2], position);
	writeV3S32(&data[2+12], speed);
	writeS32(&data[2+12+12], pitch);
	writeS32(&data[2+12+12+4], yaw);

	// Send as unreliable
	Send(0, data, false);
}

void Client::sendPlayerItem(u16 item)
{
	Player *myplayer = m_env.getLocalPlayer();
	if(myplayer == NULL)
		return;

	u16 our_peer_id = m_con.GetPeerID();

	// Set peer id if not set already
	if(myplayer->peer_id == PEER_ID_INEXISTENT)
		myplayer->peer_id = our_peer_id;
	// Check that an existing peer_id is the same as the connection's
	assert(myplayer->peer_id == our_peer_id);

	SharedBuffer<u8> data(2+2);
	writeU16(&data[0], TOSERVER_PLAYERITEM);
	writeU16(&data[2], item);

	// Send as reliable
	Send(0, data, true);
}

void Client::setServerSettings(bool damage, bool suffocation, bool hunger)
{
	m_server_damage = damage;
	m_server_suffocation = suffocation;
	m_server_hunger = hunger;
}

void Client::removeNode(v3s16 p)
{
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out

	core::map<v3s16, MapBlock*> modified_blocks;

	try
	{
		//TimeTaker t("removeNodeAndUpdate", m_device);
		m_env.getMap().removeNodeAndUpdate(p, modified_blocks);
	}
	catch(InvalidPositionException &e)
	{
	}

	for(core::map<v3s16, MapBlock * >::Iterator
			i = modified_blocks.getIterator();
			i.atEnd() == false; i++)
	{
		v3s16 p = i.getNode()->getKey();
		addUpdateMeshTaskWithEdge(p);
	}
}

void Client::addNode(v3s16 p, MapNode n)
{
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out

	TimeTaker timer1("Client::addNode()");

	core::map<v3s16, MapBlock*> modified_blocks;

	try
	{
		//TimeTaker timer3("Client::addNode(): addNodeAndUpdate");
		std::string st = std::string("");
		m_env.getMap().addNodeAndUpdate(p, n, modified_blocks, st);
	}
	catch(InvalidPositionException &e)
	{}

	//TimeTaker timer2("Client::addNode(): updateMeshes");

	for(core::map<v3s16, MapBlock * >::Iterator
			i = modified_blocks.getIterator();
			i.atEnd() == false; i++)
	{
		v3s16 p = i.getNode()->getKey();
		addUpdateMeshTaskWithEdge(p);
	}
}

void Client::updateCamera(v3f pos, v3f dir, f32 fov, v3s16 camera_offset)
{
	m_env.getClientMap().updateCamera(pos, dir, fov, camera_offset);
}

void Client::renderPostFx()
{
	m_env.getClientMap().renderPostFx();
}

MapNode Client::getNode(v3s16 p)
{
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out
	return m_env.getMap().getNode(p);
}

NodeMetadata* Client::getNodeMetadata(v3s16 p)
{
	return m_env.getMap().getNodeMetadata(p);
}

LocalPlayer* Client::getLocalPlayer()
{
	return m_env.getLocalPlayer();
}

void Client::setPlayerControl(PlayerControl &control)
{
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out
	LocalPlayer *player = m_env.getLocalPlayer();
	assert(player != NULL);
	player->control = control;
}

void Client::selectPlayerItem(u16 item)
{
	LocalPlayer *player = m_env.getLocalPlayer();
	assert(player != NULL);

	player->wieldItem(item);

	sendPlayerItem(item);
}

// Returns true if the inventory of the local player has been
// updated from the server. If it is true, it is set to false.
bool Client::getLocalInventoryUpdated()
{
	// m_inventory_updated is behind envlock
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out
	bool updated = m_inventory_updated;
	m_inventory_updated = false;
	return updated;
}

// Copies the inventory of the local player to parameter
void Client::getLocalInventory(Inventory &dst)
{
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out
	Player *player = m_env.getLocalPlayer();
	assert(player != NULL);
	dst = player->inventory;
}

InventoryContext *Client::getInventoryContext()
{
	return &m_inventory_context;
}

Inventory* Client::getInventory(InventoryContext *c, std::string id)
{
	if(id == "current_player")
	{
		assert(c->current_player);
		return &(c->current_player->inventory);
	}

	Strfnd fn(id);
	std::string id0 = fn.next(":");

	if(id0 == "nodemeta")
	{
		v3s16 p;
		p.X = mystoi(fn.next(","));
		p.Y = mystoi(fn.next(","));
		p.Z = mystoi(fn.next(","));
		NodeMetadata* meta = getNodeMetadata(p);
		if(meta)
			return meta->getInventory();
		infostream<<"nodemeta at ("<<p.X<<","<<p.Y<<","<<p.Z<<"): "
				<<"no metadata found"<<std::endl;
		return NULL;
	}

	infostream<<__FUNCTION_NAME<<": unknown id "<<id<<std::endl;
	return NULL;
}

Inventory* Client::getInventory(const InventoryLocation *loc)
{
	switch(loc->type){
	case InventoryLocation::UNDEFINED:
	{}
	break;
	case InventoryLocation::CURRENT_PLAYER:
	{
		Player *player = m_env.getLocalPlayer();
		assert(player != NULL);
		return &player->inventory;
	}
	break;
	case InventoryLocation::PLAYER:
	{
		Player *player = m_env.getPlayer(loc->name.c_str());
		if(!player)
			return NULL;
		return &player->inventory;
	}
	break;
	case InventoryLocation::NODEMETA:
	{
		NodeMetadata *meta = m_env.getMap().getNodeMetadata(loc->p);
		if(!meta)
			return NULL;
		return meta->getInventory();
	}
	break;
	default:
		assert(0);
	}
	return NULL;
}
void Client::inventoryAction(InventoryAction *a)
{
	sendInventoryAction(a);
}

ClientActiveObject * Client::getSelectedActiveObject(
		f32 max_d,
		v3f from_pos_f_on_map,
		core::line3d<f32> shootline_on_map
	)
{
	core::array<DistanceSortedActiveObject> objects;

	m_env.getActiveObjects(from_pos_f_on_map, max_d, objects);

	// Sort them.
	// After this, the closest object is the first in the array.
	objects.sort();

	for (u32 i=0; i<objects.size(); i++) {
		ClientActiveObject *obj = (ClientActiveObject*)objects[i].obj;

		core::aabbox3d<f32> *selection_box = obj->getSelectionBox();
		if (selection_box == NULL)
			continue;

		v3f pos = obj->getPosition();

		core::aabbox3d<f32> offsetted_box(
				selection_box->MinEdge + pos,
				selection_box->MaxEdge + pos
		);

		if (offsetted_box.intersectsWithLine(shootline_on_map))
			return obj;
	}

	return NULL;
}

void Client::printDebugInfo(std::ostream &os)
{
}

u16 Client::getHP()
{
	Player *player = m_env.getLocalPlayer();
	if (!player)
		return 0;
	return player->hp;
}

u16 Client::getAir()
{
	Player *player = m_env.getLocalPlayer();
	if (!player)
		return 0;
	return player->air;
}

u16 Client::getHunger()
{
	Player *player = m_env.getLocalPlayer();
	if (!player)
		return 0;
	return player->hunger;
}

float Client::getEnergy()
{
	LocalPlayer *player = m_env.getLocalPlayer();
	if (!player)
		return 0.0;
	return player->getEnergy();
}

void Client::setTempMod(v3s16 p, NodeMod mod)
{
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out
	assert(m_env.getMap().mapType() == MAPTYPE_CLIENT);

	core::map<v3s16, MapBlock*> affected_blocks;
	((ClientMap&)m_env.getMap()).setTempMod(p, mod, &affected_blocks);

	for(core::map<v3s16, MapBlock*>::Iterator i = affected_blocks.getIterator(); i.atEnd() == false; i++) {
		MapBlock *block = i.getNode()->getValue();
		addUpdateMeshTask(block->getPos());
	}
}

void Client::clearTempMod(v3s16 p)
{
	//JMutexAutoLock envlock(m_env_mutex); //bulk comment-out
	assert(m_env.getMap().mapType() == MAPTYPE_CLIENT);

	core::map<v3s16, MapBlock*> affected_blocks;
	((ClientMap&)m_env.getMap()).clearTempMod(p, &affected_blocks);

	for(core::map<v3s16, MapBlock*>::Iterator i = affected_blocks.getIterator(); i.atEnd() == false; i++) {
		MapBlock *block = i.getNode()->getValue();
		addUpdateMeshTask(block->getPos());
	}
}

void Client::addUpdateMeshTask(v3s16 p, bool ack_to_server, bool refresh_only)
{
	MapBlock *b = m_env.getMap().getBlockNoCreateNoEx(p);
	if (b == NULL)
		return;

	/*
		Create a task to update the mesh of the block
	*/

	MeshMakeData *data = new MeshMakeData();
	data->m_env = &m_env;

	if (refresh_only) {
		data->m_daynight_ratio = m_env.getDayNightRatio();
		data->m_refresh_only = true;
	}else{
		{
			data->fill(m_env.getDayNightRatio(), b);
		}

		data->m_sounds = &b->m_sounds;
	}

	// Add task to queue
	m_mesh_update_thread.m_queue_in.addBlock(p, data, ack_to_server);

	/*
		Mark mesh as non-expired at this point so that it can
		be marked as expired again if the data changes
	*/
	b->setMeshExpired(false);
}

void Client::addUpdateMeshTaskWithEdge(v3s16 blockpos, bool ack_to_server)
{
	try{
		v3s16 p = blockpos;
		addUpdateMeshTask(p, ack_to_server);
	}
	catch(InvalidPositionException &e){}
	// Leading edge
	try{
		v3s16 p = blockpos + v3s16(-1,0,0);
		addUpdateMeshTask(p);
	}
	catch(InvalidPositionException &e){}
	try{
		v3s16 p = blockpos + v3s16(1,0,0);
		addUpdateMeshTask(p);
	}
	catch(InvalidPositionException &e){}
	try{
		v3s16 p = blockpos + v3s16(0,-1,0);
		addUpdateMeshTask(p);
	}
	catch(InvalidPositionException &e){}
	try{
		v3s16 p = blockpos + v3s16(0,1,0);
		addUpdateMeshTask(p);
	}
	catch(InvalidPositionException &e){}
	try{
		v3s16 p = blockpos + v3s16(0,0,-1);
		addUpdateMeshTask(p);
	}
	catch(InvalidPositionException &e){}
	try{
		v3s16 p = blockpos + v3s16(0,0,1);
		addUpdateMeshTask(p);
	}
	catch(InvalidPositionException &e){}
}

ClientEvent Client::getClientEvent()
{
	if(m_client_event_queue.size() == 0)
	{
		ClientEvent event;
		event.type = CE_NONE;
		return event;
	}
	return m_client_event_queue.pop_front();
}

float Client::getRTT(void)
{
	try{
		return m_con.GetPeerAvgRTT(PEER_ID_SERVER);
	} catch(con::PeerNotFoundException &e){
		return -1337;
	}
}

ISoundManager* Client::getSoundManager()
{
	if (m_sound == NULL)
		return &dummySoundManager;
	return m_sound;
}

// foot: 0 = left, 1 = right
void Client::playStepSound(int foot)
{
	if (!g_sound)
		return;

	sound_playStep(&m_env.getMap(),m_env.getLocalPlayer()->getPosition(),foot);
}

void Client::playDigSound(content_t c)
{
	if (!g_sound)
		return;

	if (c == CONTENT_IGNORE) {
		c = getPointedContent();
		if ((c&CONTENT_MOB_MASK) != 0)
			return;
	}
	if (c == CONTENT_IGNORE)
		c = CONTENT_AIR;

	sound_playDig(c,m_env.getLocalPlayer()->getPosition());
}

void Client::playPlaceSound(content_t c)
{
	if (!m_sound)
		return;

	if (c == CONTENT_IGNORE)
		c = getPointedContent();

	ContentFeatures *f = &content_features(c);
	if (f->sound_place != "") {
		m_sound->playSound(f->sound_place,false);
		return;
	}
	switch (f->type) {
	case CMT_LIQUID:
		m_sound->playSound("liquid-place",false);
		break;
	default:
		m_sound->playSound("place",false);
	}
}

void Client::playSound(std::string &name, bool loop)
{
	if (!m_sound)
		return;
	m_sound->playSound(name,loop);
}

void Client::playSoundAt(std::string &name, v3f pos, bool loop)
{
	if (!m_sound)
		return;
	m_sound->playSoundAt(name,loop,pos);
}
