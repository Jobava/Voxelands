/*
Minetest-c55
Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef PATH_HEADER
#define PATH_HEADER

#include "filesys.h"
#include <string>

/*
	Gets the path to a texture/model by first checking if it exists
	in data_path and if not, using the default data path.

	Checks all supported extensions by replacing the original extension.

	If not found, returns "".

	Utilizes a thread-safe cache.
*/
std::string getPath(const char* type, const std::string &filename, bool must_exist);

/* wrappers for the old functions, because too lazy to replace them all */
inline std::string getTexturePath(const std::string &filename)
{
	return getPath("texture",filename,true);
}
inline std::string getModelPath(const std::string &filename)
{
	return getPath("model",filename,true);
}

#endif
