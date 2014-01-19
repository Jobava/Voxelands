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

#include "path.h"
#include "debug.h"
#include "main.h" // for g_settings
#include "filesys.h"
#include "utility.h"
#include "settings.h"


/*
	A cache from texture name to texture path
*/
MutexedMap<std::string, std::string> g_texturename_to_path_cache;
MutexedMap<std::string, std::string> g_modelname_to_path_cache;

/*
	Replaces the filename extension.
	eg:
		std::string image = "a/image.png"
		replace_ext(image, "jpg")
		-> image = "a/image.jpg"
	Returns true on success.
*/
static bool replace_ext(std::string &path, const char *ext)
{
	if(ext == NULL)
		return false;
	// Find place of last dot, fail if \ or / found.
	s32 last_dot_i = -1;
	for(s32 i=path.size()-1; i>=0; i--)
	{
		if(path[i] == '.')
		{
			last_dot_i = i;
			break;
		}

		if(path[i] == '\\' || path[i] == '/')
			break;
	}
	// If not found, return an empty string
	if(last_dot_i == -1)
		return false;
	// Else make the new path
	path = path.substr(0, last_dot_i+1) + ext;
	return true;
}

/*
	Find out the full path of an image by trying different filename
	extensions.

	If failed, return "".
*/
static std::string getImagePath(std::string path)
{
	// A NULL-ended list of possible image extensions
	const char *extensions[] = {
		"png", "jpg", "bmp", "tga",
		"pcx", "ppm", "psd", "wal", "rgb",
		NULL
	};

	const char **ext = extensions;
	do{
		bool r = replace_ext(path, *ext);
		if(r == false)
			return "";
		if(fs::PathExists(path))
			return path;
	}
	while((++ext) != NULL);

	return "";
}

/*
 * The new path resolver, so much prettier
 *
 * gets the path to a texture by first checking if the texture exists
 * in data_path and if not, using the default data path.
 *
 * checks all supported extensions by replacing the original extension.
 *
 * if not found, returns "".
 *
 * utilizes a thread-safe cache.
 */
std::string getPath(const char* tp, const std::string &filename, bool must_exist)
{
	std::string fullpath = "";
	std::string type(tp);

	/* check from cache */
	bool incache = g_modelname_to_path_cache.get(filename, &fullpath);
	if (incache) {
		if (must_exist == true) {
			if (!fs::PathExists(fullpath))
				fullpath = "";
		}
		if (fullpath != "")
			return fullpath;
	}

	std::string rel_path = std::string("");
	if (type == "model") {
		rel_path += std::string("models")+DIR_DELIM+filename;
	}else if (type == "texture") {
		rel_path += std::string("textures")+DIR_DELIM+filename;
	}else if (type == "html") {
		rel_path += std::string("html")+DIR_DELIM+filename;
	}else if (type == "player") {
		rel_path += std::string("textures")+DIR_DELIM+"players"+DIR_DELIM+filename;
	}else if (type == "sound") {
		rel_path += std::string("sounds")+DIR_DELIM+filename;
	}else{
		rel_path += filename;
	}

	/* check from data_path */
	std::string data_path = g_settings->get("data_path");
	if (data_path != "") {
		std::string testpath = data_path + DIR_DELIM + rel_path;
		if (type == "model" || type == "html") {
			if (fs::PathExists(testpath))
				fullpath = std::string(testpath);
		}else{
			fullpath = getImagePath(testpath);
		}
	}

	/* check from user data directory */
	if (fullpath == "") {
		std::string testpath = porting::path_userdata + DIR_DELIM + rel_path;
		if (type == "model" || type == "html") {
			if (fs::PathExists(testpath))
				fullpath = std::string(testpath);
		}else{
			fullpath = getImagePath(testpath);
		}
	}

	/* check from default data directory */
	if (fullpath == "") {
		std::string testpath = porting::path_data + DIR_DELIM + rel_path;
		if (type == "model" || type == "html") {
			if (fs::PathExists(testpath))
				fullpath = std::string(testpath);
		}else{
			fullpath = getImagePath(testpath);
		}
	}

	if (must_exist == false && fullpath == "")
#ifdef RUN_IN_PLACE
		fullpath = porting::path_data + DIR_DELIM + rel_path;
#else
		fullpath = porting::path_userdata + DIR_DELIM + rel_path;
#endif

	/* add to cache (also an empty result is cached) */
	g_modelname_to_path_cache.set(filename, fullpath);

	/* finally return it */
	return fullpath;
}
