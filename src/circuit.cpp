/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* circuit.cpp
* Copyright (C) Lisa 'darkrose' Milne 2013 <lisa@ltmnet.com>
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
************************************************************************/

#include "content_mapnode.h"
#include "content_nodemeta.h"
#include "circuit.h"
#include "environment.h"

/* CircuitManager */

CircuitManager::CircuitManager(Environment *env)
{
	m_env = env;
	m_ignore_nodes.clear();
}

bool CircuitManager::energise(u8 level, v3s16 powersrc, v3s16 pos)
{
	Map &map = m_env->getMap();
	MapNode n = map.getNodeNoEx(powersrc);
	NodeMetadata *meta;
	/* this could be a player, or something like tnt exploding */
	if (n.getContent() < CONTENT_CIRCUIT_POWERSRC_MIN || n.getContent() > CONTENT_CIRCUIT_POWERSRC_MAX)
		return propogate(level,powersrc,powersrc,pos);
	/* if it's not conductive, check if it's flammable */
	if (content_features(n).conductive == false) {
		if (level < ENERGY_HALF || content_features(n).flammable != 2)
			return false;
		return propogate(level,powersrc,powersrc,pos);
	}
	meta = map.getNodeMetadata(powersrc);
	if (!meta)
		return false;
	if (!meta->energise(level,powersrc,powersrc,powersrc))
		return false;
	if (meta->getEnergy() != level)
		return false;
	return propogate(level,powersrc,powersrc,pos);
}

bool CircuitManager::propogate(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos)
{
	Map &map = m_env->getMap();
	MapNode n = map.getNodeNoEx(pos);
	NodeMetadata *meta;

	/* if it's not conductive, check if it's flammable */
	if (content_features(n).conductive == false) {
		if (level < ENERGY_HALF || content_features(n).flammable != 2)
			return false;
		n = map.getNodeNoEx(pos+v3s16(0,1,0));
		if (n.getContent() != CONTENT_AIR && content_features(n).flammable == 0)
			return false;
		n.setContent(CONTENT_FIRE);
		map.addNodeWithEvent(pos+v3s16(0,1,0),n);
		return false;
	}
	meta = map.getNodeMetadata(pos);
	if (!meta)
		return false;

printf("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	/* if we're not sending 0 energy (power off), decrement the energy
	 * by the node's energy_drop value */
	if (level > ENERGY_MIN) {
		if (pos != powersrc) {
			if (meta->getEnergy() >= level) {
				return false;
			}
		}
		if (content_features(n).powered_node != CONTENT_IGNORE) {
			n.setContent(content_features(n).powered_node);
			map.addNodeWithEvent(pos,n);
			meta = map.getNodeMetadata(pos);
			if (!meta)
				return false;
		}
		if (!meta->energise(level,powersrc,powersrc,pos))
			return false;
		if (meta->getEnergy() != level)
			return false;

		level -= content_features(n).energy_drop;
		if (level <= ENERGY_MIN)
			return false;
	}else{
		if (pos != powersrc) {
			if (meta->getEnergy() == ENERGY_MIN)
				return false;
		}
		if (content_features(n).unpowered_node != CONTENT_IGNORE) {
			n.setContent(content_features(n).unpowered_node);
			map.addNodeWithEvent(pos,n);
			meta = map.getNodeMetadata(pos);
			if (!meta)
				return false;
		}
		if (!meta->energise(level,powersrc,powersrc,pos))
			return false;
		if (meta->getEnergy() != level)
			return false;
	}

	MapNode n_plus_y = map.getNodeNoEx(pos + v3s16(0,1,0));
	MapNode n_minus_x = map.getNodeNoEx(pos + v3s16(-1,0,0));
	MapNode n_plus_x = map.getNodeNoEx(pos + v3s16(1,0,0));
	MapNode n_minus_z = map.getNodeNoEx(pos + v3s16(0,0,-1));
	MapNode n_plus_z = map.getNodeNoEx(pos + v3s16(0,0,1));
	MapNode n_minus_xy = map.getNodeNoEx(pos + v3s16(-1,1,0));
	MapNode n_plus_xy = map.getNodeNoEx(pos + v3s16(1,1,0));
	MapNode n_minus_zy = map.getNodeNoEx(pos + v3s16(0,1,-1));
	MapNode n_plus_zy = map.getNodeNoEx(pos + v3s16(0,1,1));
	MapNode n_minus_x_y = map.getNodeNoEx(pos + v3s16(-1,-1,0));
	MapNode n_plus_x_y = map.getNodeNoEx(pos + v3s16(1,-1,0));
	MapNode n_minus_z_y = map.getNodeNoEx(pos + v3s16(0,-1,-1));
	MapNode n_plus_z_y = map.getNodeNoEx(pos + v3s16(0,-1,1));
	bool y_plus = false;
	// +Y
	if (n_plus_y.getContent() == CONTENT_AIR || content_features(n_plus_y).conductive == true)
		y_plus = true;

	// +X
	if (content_features(n_plus_x).conductive == false && content_features(n_plus_x).flammable != 2) {
		if (y_plus && (content_features(n_plus_x).draw_type == CDT_CUBELIKE || content_features(n_plus_x).draw_type == CDT_GLASSLIKE)) {
			if (content_features(n_plus_xy).conductive != false)
				propogate(level,powersrc,pos,pos + v3s16(1,1,0));
		}else if (n_plus_x.getContent() == CONTENT_AIR && content_features(n_plus_x_y).conductive != false) {
			propogate(level,powersrc,pos,pos + v3s16(1,-1,0));
		}
	}else{
		propogate(level,powersrc,pos,pos + v3s16(1,0,0));
	}

	// -X
	if (content_features(n_minus_x).conductive == false && content_features(n_minus_x).flammable != 2) {
		if (y_plus && (content_features(n_minus_x).draw_type == CDT_CUBELIKE || content_features(n_minus_x).draw_type == CDT_GLASSLIKE)) {
			if (content_features(n_minus_xy).conductive != false)
				propogate(level,powersrc,pos,pos + v3s16(-1,1,0));
		}else if (n_minus_x.getContent() == CONTENT_AIR && content_features(n_minus_x_y).conductive != false) {
			propogate(level,powersrc,pos,pos + v3s16(-1,-1,0));
		}
	}else{
		propogate(level,powersrc,pos,pos + v3s16(-1,0,0));
	}

	// +Z
	if (content_features(n_plus_z).conductive == false && content_features(n_plus_z).flammable != 2) {
		if (y_plus && (content_features(n_plus_z).draw_type == CDT_CUBELIKE || content_features(n_plus_z).draw_type == CDT_GLASSLIKE)) {
			if (content_features(n_plus_zy).conductive != false)
				propogate(level,powersrc,pos,pos + v3s16(0,1,1));
		}else if (n_plus_z.getContent() == CONTENT_AIR && content_features(n_plus_z_y).conductive != false) {
			propogate(level,powersrc,pos,pos + v3s16(0,-1,1));
		}
	}else{
		propogate(level,powersrc,pos,pos + v3s16(0,0,1));
	}

	// -Z
	if (content_features(n_minus_z).conductive == false && content_features(n_minus_z).flammable != 2) {
		if (y_plus && (content_features(n_minus_z).draw_type == CDT_CUBELIKE || content_features(n_minus_z).draw_type == CDT_GLASSLIKE)) {
			if (content_features(n_minus_zy).conductive != false)
				propogate(level,powersrc,pos,pos + v3s16(0,1,-1));
		}else if (n_minus_z.getContent() == CONTENT_AIR && content_features(n_minus_z_y).conductive != false) {
			propogate(level,powersrc,pos,pos + v3s16(0,-1,-1));
		}
	}else{
		propogate(level,powersrc,pos,pos + v3s16(0,0,-1));
	}

	return true;
}

bool CircuitManager::connect(v3s16 pos)
{
	Map &map = m_env->getMap();
	MapNode n = map.getNodeNoEx(pos);
	NodeMetadata *meta = map.getNodeMetadata(pos);
	NodeMetadata *nmeta;
	content_t c = n.getContent();

printf("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	if (content_features(c).conductive) {
		if (n.getContent() >= CONTENT_CIRCUIT_POWERSRC_MIN && n.getContent() <= CONTENT_CIRCUIT_POWERSRC_MAX)
			return energise(ENERGY_MAX,pos,pos);
		std::map<v3s16, u8> *sources;
		std::map<v3s16, u8> csources;
		v3s16 p;
		MapNode pn;
		for(s16 x=-1; x<=1; x++)
		for(s16 y=-1; y<=1; y++)
		for(s16 z=-1; z<=1; z++)
		{
			if (!x && !y && !z)
				continue;
			p = pos+v3s16(x,y,z);
			pn = map.getNodeNoEx(p);
			if (content_features(pn).conductive == false)
				continue;
			nmeta = map.getNodeMetadata(p);
			if (!nmeta)
				continue;
			sources = nmeta->getSources();
			if (!sources)
				continue;
			csources = *sources;
			for (std::map<v3s16,u8>::iterator i = csources.begin(); i != csources.end(); i++) {
				u8 v = i->second-content_features(pn).energy_drop;
				propogate(v,i->first,p,pos);
			}
		}
	}else{
		std::map<v3s16, u8> *sources;
		std::map<v3s16, u8> csources;
		std::vector<v3s16> unique_sources;
		v3s16 p;
		MapNode pn;
		v3s16 s;
		MapNode sn;
		unique_sources.clear();
		// collect sources
		for(s16 x=-1; x<=1; x++)
		for(s16 y=-1; y<=1; y++)
		for(s16 z=-1; z<=1; z++)
		{
			if (!x && !y && !z)
				continue;
			p = pos+v3s16(x,y,z);
			pn = map.getNodeNoEx(p);
			if (content_features(pn).conductive == false)
				continue;
			nmeta = map.getNodeMetadata(p);
			if (!nmeta)
				continue;
			sources = nmeta->getSources();
			if (!sources)
				continue;
			csources = *sources;
			for (std::map<v3s16,u8>::iterator i = csources.begin(); i != csources.end(); i++) {
printf("%s %s %d (%d,%d,%d)\n",__FILE__,__FUNCTION__,__LINE__,i->first.X,i->first.Y,i->first.Z);
				s = i->first;
				sn = map.getNodeNoEx(s);
				if (content_features(sn).conductive == false)
					continue;
printf("%s %s %d (%d,%d,%d)\n",__FILE__,__FUNCTION__,__LINE__,i->first.X,i->first.Y,i->first.Z);
				meta = map.getNodeMetadata(s);
				if (!meta)
					continue;
printf("%s %s %d (%d,%d,%d)\n",__FILE__,__FUNCTION__,__LINE__,i->first.X,i->first.Y,i->first.Z);
				if (meta->getEnergy() == ENERGY_MIN)
					continue;
printf("%s %s %d (%d,%d,%d)\n",__FILE__,__FUNCTION__,__LINE__,i->first.X,i->first.Y,i->first.Z);
				bool have = false;
				for (std::vector<v3s16>::iterator j = unique_sources.begin(); j != unique_sources.end(); j++) {
					if ((*j) == s) {
						have = true;
						break;
					}
				}
printf("%s %s %d (%d,%d,%d) %d (%d,%d,%d)\n",__FILE__,__FUNCTION__,__LINE__,i->first.X,i->first.Y,i->first.Z,(int)have,s.X,s.Y,s.Z);
				if (!have)
					unique_sources.push_back(s);
			}
		}
printf("%s %s %d %lu\n",__FILE__,__FUNCTION__,__LINE__,unique_sources.size());
		// propogate deenergise
		for(s16 x=-1; x<=1; x++)
		for(s16 y=-1; y<=1; y++)
		for(s16 z=-1; z<=1; z++)
		{
			if (!x && !y && !z)
				continue;
			p = pos+v3s16(x,y,z);
			pn = map.getNodeNoEx(p);
			if (content_features(pn).conductive == false)
				continue;
			nmeta = map.getNodeMetadata(p);
			if (!nmeta)
				continue;
			sources = nmeta->getSources();
			if (!sources)
				continue;
			csources = *sources;
			for (std::map<v3s16,u8>::iterator i = csources.begin(); i != csources.end(); i++) {
				propogate(ENERGY_MIN,i->first,pos,p);
			}
		}
		//energise sources
		for (std::vector<v3s16>::iterator j = unique_sources.begin(); j != unique_sources.end(); j++) {
			energise(ENERGY_MAX,*j,*j);
		}
	}

	return false; //propogate(level,powersrc,powersrc,pos);
}

bool CircuitManager::disconnect(v3s16 pos)
{
	Map &map = m_env->getMap();
	MapNode n;
	//NodeMetadata *meta = map.getNodeMetadata(pos);
	content_t c = n.getContent();

	if (content_features(c).conductive) {
		if (n.getContent() >= CONTENT_CIRCUIT_POWERSRC_MIN && n.getContent() <= CONTENT_CIRCUIT_POWERSRC_MAX)
			return energise(ENERGY_MIN,pos,pos);
	}else{
		MapNode n = map.getNodeNoEx(pos + v3s16(0,-1,0));
		n = map.getNodeNoEx(pos + v3s16(0,1,0));
		n = map.getNodeNoEx(pos + v3s16(-1,0,0));
		n = map.getNodeNoEx(pos + v3s16(1,0,0));
		n = map.getNodeNoEx(pos + v3s16(0,0,-1));
		n = map.getNodeNoEx(pos + v3s16(0,0,1));
	}

	return false; //propogate(level,powersrc,powersrc,pos);
}
