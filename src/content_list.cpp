/************************************************************************
* content_list.cpp
* minetest-classic - 3d voxel world sandbox game
* Copyright (C) Lisa Milne 2014 <lisa@ltmnet.com>
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

#include "mapnode.h"
#include "content_mapnode.h"
#include "content_list.h"
#include <map>
#include <vector>
#include <algorithm>

namespace lists {

std::map< std::string , std::vector<content_t> > c_lists;

void add(std::string name, content_t c)
{
	if (std::find(c_lists[name].begin(),c_lists[name].end(),c) == c_lists[name].end())
		c_lists[name].push_back(c);
}

std::vector<content_t> &get(std::string name)
{
	return c_lists[name];
}

};
