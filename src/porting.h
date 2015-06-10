/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* porting.h
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

/*
	Random portability stuff
*/

#ifndef PORTING_HEADER
#define PORTING_HEADER

#include <string>
// Included for u32 and such
#include "common_irrlicht.h"
#include "debug.h"
#include "constants.h"

#ifdef _MSC_VER
	#define SWPRINTF_CHARSTRING L"%S"
#else
	#define SWPRINTF_CHARSTRING L"%s"
#endif

#ifdef _WIN32
	#include <windows.h>
	#define sleep_ms(x) Sleep(x)
#else
	#include <unistd.h>
	#define sleep_ms(x) usleep(x*1000)
#endif

#if defined(__APPLE__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#endif

namespace porting
{

/*
	Signal handler (grabs Ctrl-C on POSIX systems)
*/

void signal_handler_init(void);
// Returns a pointer to a bool.
// When the bool is true, program should quit.
bool * signal_handler_killstatus(void);

/*
	Path of static data directory.
*/
extern std::string path_data;

/*
	Directory for storing user data. Examples:
	Windows: "C:\Documents and Settings\user\Application Data\<PROJECT_NAME>"
	Linux: "~/.<PROJECT_NAME>"
	Mac: "~/Library/Application Support/<PROJECT_NAME>"
*/
extern std::string path_userdata;

/*
	Get full path of stuff in data directory.
	Example: "stone.png" -> "../data/textures/stone.png"
*/
std::string getDataPath(const char *subpath);

/*
	Initialize path_data and path_userdata.
*/
void initializePaths(char* argv0);

/*
	Resolution is 10-20ms.
	Remember to check for overflows.
	Overflow can occur at any value higher than 10000000.
*/
#ifdef _WIN32 // Windows
	#include <windows.h>
	inline u32 getTimeMs()
	{
		return GetTickCount();
	}
#else // Posix
	#include <sys/time.h>
	inline u32 getTimeMs()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
	/*#include <sys/timeb.h>
	inline u32 getTimeMs()
	{
		struct timeb tb;
		ftime(&tb);
		return tb.time * 1000 + tb.millitm;
	}*/
#endif

std::string getUser();

} // namespace porting

#endif // PORTING_HEADER

