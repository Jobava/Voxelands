/************************************************************************
* content_list.cpp
* voxelands - 3d voxel world sandbox game
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

std::map< std::string , std::vector<ListData> > c_lists;

void add(std::string name, content_t c, uint16_t count, uint16_t data)
{
	for (std::vector<ListData>::iterator i = c_lists[name].begin(); i != c_lists[name].end(); ++i) {
		ListData d = *i;
		if (d.content == c && d.data == data) {
			i->count = count;
			return;
		}
	}

	c_lists[name].push_back(ListData(c,count,data));
}

std::vector<ListData> &get(std::string name)
{
	return c_lists[name];
}

};
