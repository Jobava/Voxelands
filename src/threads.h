/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* threads.h
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

#ifndef THREADS_HEADER
#define THREADS_HEADER

#include <jmutex.h>

#if (defined(WIN32) || defined(_WIN32_WCE))
typedef DWORD threadid_t;
#define __NORETURN __declspec(noreturn)
#define __FUNCTION_NAME __FUNCTION__
#else
typedef pthread_t threadid_t;
#define __NORETURN __attribute__ ((__noreturn__))
#define __FUNCTION_NAME __PRETTY_FUNCTION__
#endif

inline threadid_t get_current_thread_id()
{
#if (defined(WIN32) || defined(_WIN32_WCE))
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

#endif

