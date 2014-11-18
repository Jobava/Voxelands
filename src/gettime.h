/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* gettime.h
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

#ifndef GETTIME_HEADER
#define GETTIME_HEADER

#include "common_irrlicht.h"

/*
	Get a millisecond counter value.
	Precision depends on implementation.
	Overflows at any value above 10000000.

	Implementation of this is done in:
		Normal build: main.cpp
		Server build: servermain.cpp
*/
extern u32 getTimeMs();

/*
	Timestamp stuff
*/

#include <time.h>
#include <string>

inline std::string getTimestamp()
{
	time_t t = time(NULL);
	// This is not really thread-safe but it won't break anything
	// except its own output, so just go with it.
	struct tm *tm = localtime(&t);
	char cs[20];
	strftime(cs, 20, "%H:%M:%S", tm);
	return cs;
}


#endif
