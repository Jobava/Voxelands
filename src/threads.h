/*
Minetest-c55
Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>

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

#ifndef THREADS_HEADER
#define THREADS_HEADER

#if (defined(WIN32) || defined(_WIN32_WCE))
#include <windows.h>
typedef DWORD threadid_t;
#define __NORETURN __declspec(noreturn)
#define __FUNCTION_NAME __FUNCTION__
#else
typedef pthread_t threadid_t;
#define __NORETURN __attribute__ ((__noreturn__))
#define __FUNCTION_NAME __PRETTY_FUNCTION__
#endif

class SimpleMutex;
class SimpleThread;

#include "porting.h"
inline threadid_t get_current_thread_id()
{
#if (defined(WIN32) || defined(_WIN32_WCE))
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

/*
	A simple mutex implementation
*/
#ifdef _WIN32
class SimpleMutex
{
	CRITICAL_SECTION mut;

public:

	SimpleMutex()
	{
		InitializeCriticalSection(&mut);
	}

	~SimpleMutex()
	{
		unlock();
		DeleteCriticalSection(&mut);
	}

	void init()
	{
		InitializeCriticalSection(&mut);
	}

	void lock()
	{
		EnterCriticalSection(&mut);
	}

	bool trylock()
	{
		if (!TryEnterCriticalSection(&mut))
			return true;
		return false;
	}

	void unlock()
	{
		LeaveCriticalSection(&mut);
	}
};
#else
class SimpleMutex
{
	pthread_mutexattr_t attr;
	pthread_mutex_t mut;

public:

	SimpleMutex()
	{
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&mut, &attr);
	}

	~SimpleMutex()
	{
		unlock();
		pthread_mutex_destroy(&mut);
		pthread_mutexattr_destroy(&attr);
	}

	void init()
	{
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&mut, &attr);
	}

	void lock()
	{
		pthread_mutex_lock(&mut);
	}

	int trylock()
	{
		if (pthread_mutex_trylock(&mut))
			return true;
		return false;
	}

	void unlock()
	{
		pthread_mutex_unlock(&mut);
	}
};
#endif

class SimpleMutexAutoLock
{
	SimpleMutex &mutex;

public:
	SimpleMutexAutoLock(SimpleMutex &m):
		mutex(m)
	{
		mutex.lock();
	}

	~SimpleMutexAutoLock()
	{
		mutex.unlock();
	}
};

/*
	A base class for simple background thread implementation
*/
class SimpleThread
{
	bool run;
	SimpleMutex run_mutex;
#ifdef _WIN32
	HANDLE thread;
#else
	pthread_t thread;
	pthread_attr_t attr;
#endif

public:

	SimpleThread():
		run(false),
		run_mutex()
	{
#ifndef _WIN32
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#endif
	}

	virtual ~SimpleThread()
	{
		kill();
	}

	virtual void * Thread() = 0;

	bool getRun()
	{
		run_mutex.lock();
		bool r = run;
		run_mutex.unlock();
		return r;
	}
	void setRun(bool a_run)
	{
		run_mutex.lock();
		run = a_run;
		run_mutex.unlock();
	}

	void start()
	{
		if (getRun()) {
#ifdef _WIN32
			ResumeThread(thread);
#else
			pthread_kill(thread,SIGCONT);
#endif
		}else{
			setRun(true);
#ifdef _WIN32
			DWORD id;
			thread = CreateThread(NULL, 0, runThread, this, 0, &id);
#else
			pthread_create(&thread, &attr, &runThread, this);
#endif
		}
	}

	void wait()
	{
		if (getRun()) {
#ifdef _WIN32
			WaitForSingleObject(thread, 2000);
			CloseHandle(thread);
#else
			pthread_join(thread,NULL);
#endif
		}
	}

	void stop()
	{
		if (!getRun())
			return;
		setRun(false);
#ifdef _WIN32
		WaitForSingleObject(thread, 2000);
		CloseHandle(thread);
#else
		pthread_join(thread,NULL);
#endif
	}

	void kill()
	{
		if (getRun()) {
			setRun(false);
#ifdef _WIN32
			TerminateThread(thread,0);
			CloseHandle(thread);
#else
			pthread_kill(thread,SIGKILL);
#endif
		}
	}

private:
#ifdef _WIN32
	static DWORD WINAPI runThread(LPVOID data)
#else
	static void *runThread(void* data)
#endif
	{
		SimpleThread *t = (SimpleThread*)data;
		void *r = t->Thread();
		t->setRun(false);
#ifdef _WIN32
		ExitThread(0);
		CloseHandle(t->thread);
		return (DWORD)r;
#else
		pthread_exit(r);
		return r;
#endif
	}
};

#endif

