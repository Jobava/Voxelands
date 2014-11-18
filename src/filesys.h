/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* filesys.h
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

#ifndef FILESYS_HEADER
#define FILESYS_HEADER

#include <string>
#include <vector>
#include "exceptions.h"

#ifdef _WIN32 // WINDOWS
#define DIR_DELIM "\\"
#define DIR_DELIM_C '\\'
#else // POSIX
#define DIR_DELIM "/"
#define DIR_DELIM_C '/'
#endif

namespace fs
{

struct DirListNode
{
	std::string name;
	bool dir;
};

std::vector<DirListNode> GetDirListing(std::string path);

// Returns true if already exists
bool CreateDir(std::string path);

// Create all directories on the given path that don't already exist.
bool CreateAllDirs(std::string path);

bool PathExists(std::string path);

// Only pass full paths to this one. True on success.
// NOTE: The WIN32 version returns always true.
bool RecursiveDelete(std::string path);

// Only pass full paths to this one. True on success.
bool RecursiveDeleteContent(std::string path);

}//fs

#endif

