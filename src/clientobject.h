/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* clientobject.h
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

#ifndef CLIENTOBJECT_HEADER
#define CLIENTOBJECT_HEADER

#include "common_irrlicht.h"
#include "activeobject.h"
#include "mapnode.h"

/*
* Some planning
*
* Client receives a network packet with information of added objects
* in it
*
* Client supplies the information to its ClientEnvironment
* The environment adds the specified objects to itself
*/

class ClientEnvironment;

class ClientActiveObject : public ActiveObject
{
public:
	ClientActiveObject(u16 id);
	virtual ~ClientActiveObject();

	virtual void addToScene(scene::ISceneManager *smgr){}
	virtual void removeFromScene(){}
	// 0 <= light_at_pos <= LIGHT_SUN
	virtual void updateLight(u8 light_at_pos){}
	virtual v3s16 getLightPosition(){return v3s16(0,0,0);}
	virtual core::aabbox3d<f32>* getSelectionBox(){return NULL;}
	virtual core::aabbox3d<f32>* getCollisionBox(){return NULL;}
	virtual v3f getPosition(){return v3f(0,0,0);}
	virtual bool doShowSelectionBox(){return true;}
	virtual void updateCameraOffset(v3s16 camera_offset){};

	// Step object in time
	virtual void step(float dtime, ClientEnvironment *env){}

	// Process a message sent by the server side object
	virtual void processMessage(const std::string &data){}

	virtual std::string infoText() {return "";}

	/*
		This takes the return value of
		ServerActiveObject::getClientInitializationData
	*/
	virtual void initialize(const std::string &data){}

	// Create a certain type of ClientActiveObject
	static ClientActiveObject* create(u8 type);

	// If returns true, punch will not be sent to the server
	virtual bool directReportPunch(content_t punch_item, v3f dir)
	{ return false; }

	// get the content type of whatever this is
	virtual content_t getContent() {return CONTENT_IGNORE;}

protected:
	// Used for creating objects based on type
	typedef ClientActiveObject* (*Factory)();
	static void registerType(u16 type, Factory f);
private:
	// Used for creating objects based on type
	static core::map<u16, Factory> m_types;
};

#endif

