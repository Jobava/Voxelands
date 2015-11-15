/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mapblock.cpp
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

#include "mapblock.h"
#include "map.h"
// For g_settings
#include "main.h"
#include "light.h"
#include <sstream>
#ifndef SERVER
#include "sound.h"
#endif

/*
	MapBlock
*/

MapBlock::MapBlock(Map *parent, v3s16 pos, bool dummy):
	has_spawn_area(false),
	spawn_area(0,0,0),
	water_spawn(false),
	last_spawn(0),
	m_parent(parent),
	m_pos(pos),
	m_modified(MOD_STATE_WRITE_NEEDED),
	is_underground(false),
	m_lighting_expired(true),
	m_day_night_differs(false),
	m_generated(false),
	m_timestamp(BLOCK_TIMESTAMP_UNDEFINED),
	m_usage_timer(0)
{
	data = NULL;
	if (dummy == false)
		reallocate();

	//m_spawn_timer = -10000;

#ifndef SERVER
	m_mesh_expired = false;
	mesh_mutex.Init();
	mesh = NULL;
#endif
}

MapBlock::~MapBlock()
{
#ifndef SERVER
	{
		JMutexAutoLock lock(mesh_mutex);

		if (mesh) {
			delete mesh;
			mesh = NULL;
		}
		if (g_sound) {
			for (std::map<v3s16,MapBlockSound>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++) {
				g_sound->stopSound(i->second.id);
			}
		}
	}
#endif

	if (data)
		delete[] data;
}

bool MapBlock::isValidPositionParent(v3s16 p)
{
	if (isValidPosition(p.X,p.Y,p.Z))
		return true;
	return m_parent->isValidPosition(getPosRelative() + p);
}

MapNode MapBlock::getNodeParent(v3s16 p, bool *is_valid_position)
{
	if (isValidPosition(p.X,p.Y,p.Z) == false)
		return m_parent->getNodeNoEx(getPosRelative() + p, is_valid_position);
	if (data == NULL) {
		if (is_valid_position)
			*is_valid_position = false;
		return MapNode(CONTENT_IGNORE);
	}
	if (is_valid_position)
		*is_valid_position = true;
	return data[p.Z*MAP_BLOCKSIZE*MAP_BLOCKSIZE + p.Y*MAP_BLOCKSIZE + p.X];
}

void MapBlock::setNodeParent(v3s16 p, MapNode & n)
{
	if (isValidPosition(p.X,p.Y,p.Z) == false) {
		m_parent->setNode(getPosRelative() + p, n);
	}else{
		data[p.Z*MAP_BLOCKSIZE*MAP_BLOCKSIZE + p.Y*MAP_BLOCKSIZE + p.X] = n;
	}
}

/*
	Propagates sunlight down through the block.
	Doesn't modify nodes that are not affected by sunlight.

	Returns false if sunlight at bottom block is invalid.
	Returns true if sunlight at bottom block is valid.
	Returns true if bottom block doesn't exist.

	If there is a block above, continues from it.
	If there is no block above, assumes there is sunlight, unless
	is_underground is set or highest node is water.

	All sunlighted nodes are added to light_sources.

	if remove_light==true, sets non-sunlighted nodes black.

	if black_air_left!=NULL, it is set to true if non-sunlighted
	air is left in block.
*/
bool MapBlock::propagateSunlight(core::map<v3s16, bool> & light_sources,
		bool remove_light, bool *black_air_left)
{
	// Whether the sunlight at the top of the bottom block is valid
	bool block_below_is_valid = true;

	v3s16 pos_relative = getPosRelative();

	for (s16 x=0; x<MAP_BLOCKSIZE; x++) {
		for (s16 z=0; z<MAP_BLOCKSIZE; z++) {
			bool no_sunlight = false;
			//bool no_top_block = false;
			// Check if node above block has sunlight
			bool is_valid_position;
			MapNode np = getNodeParent(v3s16(x, MAP_BLOCKSIZE, z), &is_valid_position);
			if (is_valid_position) {
				if (np.getContent() == CONTENT_IGNORE) {
					// Trust heuristics
					no_sunlight = is_underground;
				}else if (np.getLight(LIGHTBANK_DAY) != LIGHT_SUN) {
					no_sunlight = true;
				}
			}else{
				//no_top_block = true;

				// NOTE: This makes over-ground roofed places sunlighted
				// Assume sunlight, unless is_underground==true
				if (is_underground) {
					no_sunlight = true;
				}else{
					MapNode n = getNodeNoEx(v3s16(x, MAP_BLOCKSIZE-1, z));
					//if(n.getContent() == CONTENT_WATER || n.getContent() == CONTENT_WATERSOURCE)
					if (content_features(n).sunlight_propagates == false)
						no_sunlight = true;
				}
				// NOTE: As of now, this just would make everything dark.
				// No sunlight here
				//no_sunlight = true;
			}

			s16 y = MAP_BLOCKSIZE-1;

			// This makes difference to diminishing in water.
			bool stopped_to_solid_object = false;

			u8 current_light = no_sunlight ? 0 : LIGHT_SUN;

			for (; y >= 0; y--) {
				v3s16 pos(x, y, z);
				MapNode &n = getNodeRef(pos);
				ContentFeatures &f = content_features(n);

				if (current_light != 0 && (current_light != LIGHT_SUN || !f.sunlight_propagates)) {
					if (!f.light_propagates) {
						// A solid object is on the way.
						stopped_to_solid_object = true;

						// Light stops.
						current_light = 0;
					}else{
						// Diminish light
						current_light = diminish_light(current_light);
					}
				}

				u8 old_light = n.getLight(LIGHTBANK_DAY);

				if (current_light > old_light || remove_light)
					n.setLight(LIGHTBANK_DAY, current_light);

				if (diminish_light(current_light) != 0)
					light_sources.insert(pos_relative + pos, true);

				if (current_light == 0 && stopped_to_solid_object && black_air_left)
					*black_air_left = true;
			}

			// Whether or not the block below should see LIGHT_SUN
			bool sunlight_should_go_down = (current_light == LIGHT_SUN);

			/*
				If the block below hasn't already been marked invalid:

				Check if the node below the block has proper sunlight at top.
				If not, the block below is invalid.

				Ignore non-transparent nodes as they always have no light
			*/
			try {
				if (block_below_is_valid) {
					MapNode n = getNodeParent(v3s16(x, -1, z));
					if (content_features(n).light_propagates) {
						if (
							n.getLight(LIGHTBANK_DAY) == LIGHT_SUN
							&& sunlight_should_go_down == false
						) {
							block_below_is_valid = false;
						}else if (
							n.getLight(LIGHTBANK_DAY) != LIGHT_SUN
							&& sunlight_should_go_down == true
						) {
							block_below_is_valid = false;
						}
					}
				}
			}catch(InvalidPositionException &e) {
				// Just no block below, no need to panic.
			}
		}
	}

	return block_below_is_valid;
}


void MapBlock::copyTo(VoxelManipulator &dst)
{
	v3s16 data_size(MAP_BLOCKSIZE, MAP_BLOCKSIZE, MAP_BLOCKSIZE);
	VoxelArea data_area(v3s16(0,0,0), data_size - v3s16(1,1,1));

	// Copy from data to VoxelManipulator
	dst.copyFrom(data, data_area, v3s16(0,0,0),
			getPosRelative(), data_size);
}

void MapBlock::copyFrom(VoxelManipulator &dst)
{
	v3s16 data_size(MAP_BLOCKSIZE, MAP_BLOCKSIZE, MAP_BLOCKSIZE);
	VoxelArea data_area(v3s16(0,0,0), data_size - v3s16(1,1,1));

	// Copy from VoxelManipulator to data
	dst.copyTo(data, data_area, v3s16(0,0,0),
			getPosRelative(), data_size);
}

void MapBlock::updateDayNightDiff()
{
	if(data == NULL)
	{
		m_day_night_differs = false;
		return;
	}

	bool differs = false;

	/*
		Check if any lighting value differs
	*/
	for(u32 i=0; i<MAP_BLOCKSIZE*MAP_BLOCKSIZE*MAP_BLOCKSIZE; i++)
	{
		MapNode &n = data[i];
		if(n.getLight(LIGHTBANK_DAY) != n.getLight(LIGHTBANK_NIGHT))
		{
			differs = true;
			break;
		}
	}

	/*
		If some lighting values differ, check if the whole thing is
		just air. If it is, differ = false
	*/
	if(differs)
	{
		bool only_air = true;
		for(u32 i=0; i<MAP_BLOCKSIZE*MAP_BLOCKSIZE*MAP_BLOCKSIZE; i++)
		{
			MapNode &n = data[i];
			if(n.getContent() != CONTENT_AIR)
			{
				only_air = false;
				break;
			}
		}
		if(only_air)
			differs = false;
	}

	// Set member variable
	m_day_night_differs = differs;
}

s16 MapBlock::getGroundLevel(v2s16 p2d)
{
	if(isDummy())
		return -3;
	try
	{
		s16 y = MAP_BLOCKSIZE-1;
		for(; y>=0; y--)
		{
			MapNode n = getNodeRef(p2d.X, y, p2d.Y);
			if(content_features(n).walkable)
			{
				if(y == MAP_BLOCKSIZE-1)
					return -2;
				else
					return y;
			}
		}
		return -1;
	}
	catch(InvalidPositionException &e)
	{
		return -3;
	}
}

/*
	Serialization
*/

void MapBlock::serialize(std::ostream &os, u8 version)
{
	if (!ser_ver_supported(version))
		throw VersionMismatchException("ERROR: MapBlock format not supported");

	if (data == NULL)
		throw SerializationError("ERROR: Not writing dummy block.");

	{
		// First byte
		u8 flags = 0;
		if (is_underground)
			flags |= 0x01;
		if (m_day_night_differs)
			flags |= 0x02;
		if (m_lighting_expired)
			flags |= 0x04;
		if (m_generated == false)
			flags |= 0x08;
		os.write((char*)&flags, 1);

		u32 nodecount = MAP_BLOCKSIZE*MAP_BLOCKSIZE*MAP_BLOCKSIZE;

		u32 sl = MapNode::serializedLength(version);

		/*
			Get data
		*/
		// Serialize nodes
		SharedBuffer<u8> databuf_nodelist(nodecount*sl);
		for (u32 i=0; i<nodecount; i++) {
			data[i].serialize(&databuf_nodelist[i*sl], version);
		}

		// Create buffer with different parameters sorted
		SharedBuffer<u8> databuf(nodecount*sl);
		for (u32 i=0; i<nodecount; i++) {
			for (u32 k=0; k<sl; k++) {
				databuf[i+(nodecount*k)] = databuf_nodelist[(i*sl)+k];
			}
		}

		/*
			Compress data to output stream
		*/

		compress(databuf, os, version);

		/*
			NodeMetadata
		*/
		{
			std::ostringstream oss(std::ios_base::binary);
			m_node_metadata.serialize(oss);
			compressZlib(oss.str(), os);
		}
	}
}

void MapBlock::deSerialize(std::istream &is, u8 version)
{
	if (!ser_ver_supported(version))
		throw VersionMismatchException("ERROR: MapBlock format not supported");

	{
		u32 nodecount = MAP_BLOCKSIZE*MAP_BLOCKSIZE*MAP_BLOCKSIZE;

		u8 flags;
		is.read((char*)&flags, 1);
		is_underground = (flags & 0x01) ? true : false;
		m_day_night_differs = (flags & 0x02) ? true : false;
		m_lighting_expired = (flags & 0x04) ? true : false;
		m_generated = (flags & 0x08) ? false : true;
		u32 sl = MapNode::serializedLength(version);

		// Uncompress data
		std::ostringstream os(std::ios_base::binary);
		decompress(is, os, version);
		std::string s = os.str();
		if (s.size() != nodecount*sl)
			throw SerializationError("MapBlock::deSerialize: decompress resulted in size"
						" other than nodecount*nodelength");

		// deserialize nodes from buffer
		for (u32 i=0; i<nodecount; i++) {
			SharedBuffer<u8> buf(sl);
			for (u32 k=0; k<sl; k++) {
				buf[k] = s[i+(nodecount*k)];
			}
			data[i].deSerialize(*buf, version);
		}

		/*
			NodeMetadata
		*/
		// Ignore errors
		try{
			std::ostringstream oss(std::ios_base::binary);
			decompressZlib(is, oss);
			std::istringstream iss(oss.str(), std::ios_base::binary);
			m_node_metadata.deSerialize(iss);
		}catch(SerializationError &e) {
			dstream<<"WARNING: MapBlock::deSerialize(): Ignoring an error"
					<<" while deserializing node metadata"<<std::endl;
		}
	}
}

void MapBlock::serializeDiskExtra(std::ostream &os, u8 version)
{
	// Versions up from 9 have block objects. (DEPRECATED)
	if(version >= 9)
	{
		// count=0
		writeU16(os, 0);
	}

	// Versions up from 15 have static objects.
	if(version >= 15)
	{
		m_static_objects.serialize(os);
	}

	// Timestamp
	if(version >= 17)
	{
		writeU32(os, getTimestamp());
	}
}

void MapBlock::deSerializeDiskExtra(std::istream &is, u8 version)
{
	/*
		Versions up from 9 have block objects. (DEPRECATED)
	*/
	if(version >= 9)
	{
		u16 count = readU16(is);
		// Not supported and length not known if count is not 0
		if(count != 0){
			dstream<<"WARNING: MapBlock::deSerializeDiskExtra(): "
					<<"Ignoring stuff coming at and after MBOs"<<std::endl;
			return;
		}
	}

	/*
		Versions up from 15 have static objects.
	*/
	if(version >= 15)
	{
		m_static_objects.deSerialize(is);
	}

	// Timestamp
	if(version >= 17)
	{
		setTimestamp(readU32(is));
	}
	else
	{
		setTimestamp(BLOCK_TIMESTAMP_UNDEFINED);
	}
}

/*
	Get a quick string to describe what a block actually contains
*/
std::string analyze_block(MapBlock *block)
{
	if(block == NULL)
	{
		return "NULL";
	}

	std::ostringstream desc;

	v3s16 p = block->getPos();
	char spos[20];
	snprintf(spos, 20, "(%2d,%2d,%2d), ", p.X, p.Y, p.Z);
	desc<<spos;

	switch(block->getModified())
	{
	case MOD_STATE_CLEAN:
		desc<<"CLEAN,           ";
		break;
	case MOD_STATE_WRITE_AT_UNLOAD:
		desc<<"WRITE_AT_UNLOAD, ";
		break;
	case MOD_STATE_WRITE_NEEDED:
		desc<<"WRITE_NEEDED,    ";
		break;
	default:
		desc<<"unknown getModified()="+itos(block->getModified())+", ";
	}

	if(block->isGenerated())
		desc<<"is_gen [X], ";
	else
		desc<<"is_gen [ ], ";

	if(block->getIsUnderground())
		desc<<"is_ug [X], ";
	else
		desc<<"is_ug [ ], ";

#ifndef SERVER
	if(block->getMeshExpired())
		desc<<"mesh_exp [X], ";
	else
		desc<<"mesh_exp [ ], ";
#endif

	if(block->getLightingExpired())
		desc<<"lighting_exp [X], ";
	else
		desc<<"lighting_exp [ ], ";

	if(block->isDummy())
	{
		desc<<"Dummy, ";
	}
	else
	{
		// We'll just define the numbers here, don't want to include
		// content_mapnode.h
		const content_t content_water = 2;
		const content_t content_watersource = 9;
		const content_t content_tree = 0x801;
		const content_t content_leaves = 0x802;
		const content_t content_jungletree = 0x815;

		bool full_ignore = true;
		bool some_ignore = false;
		bool full_air = true;
		bool some_air = false;
		bool trees = false;
		bool water = false;
		for(s16 z0=0; z0<MAP_BLOCKSIZE; z0++)
		for(s16 y0=0; y0<MAP_BLOCKSIZE; y0++)
		for(s16 x0=0; x0<MAP_BLOCKSIZE; x0++)
		{
			v3s16 p(x0,y0,z0);
			MapNode n = block->getNodeNoEx(p);
			content_t c = n.getContent();
			if(c == CONTENT_IGNORE)
				some_ignore = true;
			else
				full_ignore = false;
			if(c == CONTENT_AIR)
				some_air = true;
			else
				full_air = false;
			if(c == content_tree || c == content_jungletree
					|| c == content_leaves)
				trees = true;
			if(c == content_water
					|| c == content_watersource)
				water = true;
		}

		desc<<"content {";

		std::ostringstream ss;

		if(full_ignore)
			ss<<"IGNORE (full), ";
		else if(some_ignore)
			ss<<"IGNORE, ";

		if(full_air)
			ss<<"AIR (full), ";
		else if(some_air)
			ss<<"AIR, ";

		if(trees)
			ss<<"trees, ";
		if(water)
			ss<<"water, ";

		if(ss.str().size()>=2)
			desc<<ss.str().substr(0, ss.str().size()-2);

		desc<<"}, ";
	}

	return desc.str().substr(0, desc.str().size()-2);
}


//END
