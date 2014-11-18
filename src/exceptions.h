/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* exceptions.h
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

#ifndef EXCEPTIONS_HEADER
#define EXCEPTIONS_HEADER

#include <exception>

class BaseException : public std::exception
{
public:
	BaseException(const char *s)
	{
		m_s = s;
	}
	virtual const char * what() const throw()
	{
		return m_s;
	}
	const char *m_s;
};

class AsyncQueuedException : public BaseException
{
public:
	AsyncQueuedException(const char *s):
		BaseException(s)
	{}
};

class NotImplementedException : public BaseException
{
public:
	NotImplementedException(const char *s):
		BaseException(s)
	{}
};

class AlreadyExistsException : public BaseException
{
public:
	AlreadyExistsException(const char *s):
		BaseException(s)
	{}
};

class VersionMismatchException : public BaseException
{
public:
	VersionMismatchException(const char *s):
		BaseException(s)
	{}
};

class FileNotGoodException : public BaseException
{
public:
	FileNotGoodException(const char *s):
		BaseException(s)
	{}
};

class SerializationError : public BaseException
{
public:
	SerializationError(const char *s):
		BaseException(s)
	{}
};

class LoadError : public BaseException
{
public:
	LoadError(const char *s):
		BaseException(s)
	{}
};

class ContainerFullException : public BaseException
{
public:
	ContainerFullException(const char *s):
		BaseException(s)
	{}
};

class SettingNotFoundException : public BaseException
{
public:
	SettingNotFoundException(const char *s):
		BaseException(s)
	{}
};

class InvalidFilenameException : public BaseException
{
public:
	InvalidFilenameException(const char *s):
		BaseException(s)
	{}
};

class ProcessingLimitException : public BaseException
{
public:
	ProcessingLimitException(const char *s):
		BaseException(s)
	{}
};

class CommandLineError : public BaseException
{
public:
	CommandLineError(const char *s):
		BaseException(s)
	{}
};

class ItemNotFoundException : public BaseException
{
public:
	ItemNotFoundException(const char *s):
		BaseException(s)
	{}
};

/*
	Some "old-style" interrupts:
*/

class InvalidPositionException : public BaseException
{
public:
	InvalidPositionException():
		BaseException("Somebody tried to get/set something in a nonexistent position.")
	{}
	InvalidPositionException(const char *s):
		BaseException(s)
	{}
};

class TargetInexistentException : public std::exception
{
	virtual const char * what() const throw()
	{
		return "Somebody tried to refer to something that doesn't exist.";
	}
};

class NullPointerException : public std::exception
{
	virtual const char * what() const throw()
	{
		return "NullPointerException";
	}
};

#endif

