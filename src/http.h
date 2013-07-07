/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* http.h
* Copyright (C) Lisa 'darkrose' Milne 2013 <lisa@ltmnet.com>
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


#ifndef HTTP_HEADER
#define HTTP_HEADER

#include "socket.h"
#include "common_irrlicht.h"

class HTTPServer
{
public:
	HTTPServer();
	~HTTPServer();
private:
	TCPSocket &m_socket;
	core::map<u16,TCPSocket&> m_peers;
};

class HTTPClient
{
public:
	HTTPClient();
	~HTTPClient();
private:
	TCPSocket &m_socket;
};

#endif
