/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* http.cpp
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

#include "socket.h"
#include "http.h"
#include "debug.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include "utility.h"
#include "connection.h"
#include "log.h"
#include "sha1.h"

void * HTTPServerThread::Thread()
{
	ThreadStarted();

	log_register_thread("HTTPServerThread");

	DSTACK(__FUNCTION_NAME);

	BEGIN_DEBUG_EXCEPTION_HANDLER

	while (getRun())
	{
		try{
			m_server->step();
		}catch (con::NoIncomingDataException &e) {
		}catch(con::PeerNotFoundException &e) {
		}
	}

	END_DEBUG_EXCEPTION_HANDLER(errorstream)

	return NULL;
}

/*
 * HTTPServer
 */

HTTPServer::HTTPServer(Server &server):
	m_thread(this)
{
	m_server = &server;
}

HTTPServer::~HTTPServer()
{
}

void HTTPServer::start(u16 port)
{
	DSTACK(__FUNCTION_NAME);
	// Stop thread if already running
	m_thread.stop();

	m_socket = new TCPSocket();
	m_socket->setTimeoutMs(30);
	m_socket->Bind(port);

	// Start thread
	m_thread.setRun(true);
	m_thread.Start();

	infostream<<"HTTPServer: Started on port "<<port<<std::endl;
}

void HTTPServer::stop()
{
	DSTACK(__FUNCTION_NAME);

	m_thread.stop();
	delete m_socket;

	infostream<<"HTTPServer: Threads stopped"<<std::endl;
}

void HTTPServer::step()
{
	if (m_socket->WaitData(50)) {
		TCPSocket *s = m_socket->Accept();
		HTTPRemoteClient *c = new HTTPRemoteClient(s,this);
		m_peers.push_back(c);
	}

	for (std::vector<HTTPRemoteClient*>::iterator it = m_peers.begin(); it != m_peers.end(); ++it) {
		HTTPRemoteClient *c = *it;
		try{
			if (c->receive()) {
				m_peers.erase(it);
				delete c;
			}
		}catch (SocketException &e) {
			// assume it's closed
			m_peers.erase(it);
			delete c;
			continue;
		}
	}
}

/*
 * HTTPRemoteClient
 */

HTTPRemoteClient::~HTTPRemoteClient()
{
}

int HTTPRemoteClient::receive()
{
	char buff[2048];
	if (!m_socket->WaitData(30))
		return 0;
	int r = m_socket->Receive(buff,2048);
	if (r<1)
		return 1;

	m_recv_headers.clear();
	m_send_headers.clear();

	int h = m_recv_headers.read(buff,r);
	if (h == 1) {
		return 1;
	}else if (h == 2) {
		//if (m_recv_headers.url() == "")
			//return 1;
		//while (m_socket->WaitData(1000)) {
			//r = m_socket->Receive(buff,2048);
			//if (r<1)
				//return 1;
			//h = m_recv_headers.read(buff,r);
			//if (!h)
				//break;
			//if (h == 1)
				//return 1;
		//}
		//if (h)
			return 1;
	}

	if (m_recv_headers.cookie() != "" && m_recv_headers.get("User") != "") {
		if (m_recv_headers.cookie() == m_server->getPlayerCookie(m_recv_headers.get("User"))) {
			m_send_headers.setCookie(m_recv_headers.cookie());
			m_auth = true;
		}
	}

	//std::string u;
	//for (int i=0; (u = m_recv_headers.url(i)) != ""; i++) {
		//printf("%d: '%s'\n",i,u.c_str());
	//}


	//setResponse("404 Not Found");
	//std::string html("<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>");
	//send(html);
	send((char*)"grar");

	return 0;//m_recv_headers.keepAlive() == true ? 0 : 1;
}

void HTTPRemoteClient::send(char* data)
{
	int l = strlen(data);
	m_send_headers.set("Content-Type","text/plain");
	m_send_headers.setLength(l);
	sendHeaders();
	m_socket->Send(data,l);
}

void HTTPRemoteClient::sendHTML(char* data)
{
	int l = strlen(data);
	m_send_headers.set("Content-Type","text/html");
	m_send_headers.setLength(l);
	sendHeaders();
	m_socket->Send(data,l);
}

void HTTPRemoteClient::sendFile(std::string &file)
{
	m_send_headers.set("Content-Type","text/plain");
	m_send_headers.setLength(0);
	sendHeaders();
}

void HTTPRemoteClient::sendHeaders()
{
	std::string v;
	int s;
	char buff[1024];

	//v = m_response;
	//if (v == "")
		//v = std::string("200 OK");

	//s = snprintf(buff,1024,"HTTP/1.1 %s\r\n",v.c_str());
	//m_socket->Send(buff,s);
	m_socket->Send("HTTP/1.1 200 OK\r\n",17);

	//v = m_send_headers.get("Content-Type");
	//if (v == "") {
		m_socket->Send("Content-Type: text/plain\r\n",26);
	//}else{
		//s = snprintf(buff,1024,"Content-Type: %s\r\n",v.c_str());
		//m_socket->Send(buff,s);
	//}

	s = m_send_headers.length();
	s = snprintf(buff,1024,"Content-Length: %d\r\n",s);
	m_socket->Send(buff,s);

	//v = m_send_headers.cookie();
	//if (v != "") {
		//s = snprintf(buff,1024,"Set-Cookie: MTID=%s\r\n",v.c_str());
		//m_socket->Send(buff,s);
	//}

	m_socket->Send("\r\n",2);
}

#ifndef SERVER
#include "client.h"

void * HTTPClientThread::Thread()
{
	ThreadStarted();

	log_register_thread("HTTPClientThread");

	DSTACK(__FUNCTION_NAME);

	BEGIN_DEBUG_EXCEPTION_HANDLER

	while (getRun())
	{
		try{
			m_client->step();
		}catch (con::NoIncomingDataException &e) {
		}catch(con::PeerNotFoundException &e) {
		}
	}

	END_DEBUG_EXCEPTION_HANDLER(errorstream)

	return NULL;
}

/*
 * HTTPClient
 */

HTTPClient::HTTPClient(Client *client):
	m_cookie(""),
	m_thread(this)
{
	m_client = client;
	m_req_mutex.Init();
}

HTTPClient::~HTTPClient()
{
}

void HTTPClient::start(const Address &address)
{
	DSTACK(__FUNCTION_NAME);
	// Stop thread if already running
	m_thread.stop();

	m_socket = new TCPSocket();
	m_socket->setTimeoutMs(30);
	m_socket->Connect(address);

	// Start thread
	m_thread.setRun(true);
	m_thread.Start();

	infostream<<"HTTPClient: Started"<<std::endl;
}

void HTTPClient::stop()
{
	DSTACK(__FUNCTION_NAME);

	m_thread.stop();
	delete m_socket;

	infostream<<"HTTPClient: Threads stopped"<<std::endl;
}

void HTTPClient::step()
{
	sleep(1);
}

void HTTPClient::pushRequest(HTTPRequestType type, std::string &data)
{
	switch (type) {
	case HTTPREQUEST_NULL:
		break;
	/*
	 * /player/<name>/skin
	 * request the skin texture for <name>
	 * response:
	 * 	if skin exists:
	 *		200 OK + texture data
	 *	else:
	 *		303 See Other + Location header to /texture/character.png
	 * this client will ignore the redirect, while browsers will see the default skin
	 */
	case HTTPREQUEST_SKIN:
	{
		std::string url("/player/");
		url += data + "/skin";
		HTTPRequest r(url);
		m_req_mutex.Lock();
		m_requests.push_back(r);
		m_req_mutex.Unlock();
		break;
	}
	/*
	 * /player/<name>/skin/hash/<sha1>
	 * sends the sha1 hash of the skin texture for <name>
	 * response:
	 * 	if hash is the same on client and server:
	 *		204 No Content
	 *	if client is authenticated as <name>:
	 *		304 Not Modified
	 *	else:
	 *		200 OK + texture data
	 */
	case HTTPREQUEST_SKIN_HASH:
	{
		std::string tex = std::string("players/player_")+data+".png";
		std::string ptex = getTexturePath(tex);
		if (ptex == "") {
			pushRequest(HTTPREQUEST_SKIN,data);
			return;
		}
		break;
	}
	/*
	 * /player/<name>/skin
	 * PUT request type, sends the skin to the server
	 * response:
	 *	if client is authenticated as <name>:
	 *		201 Created
	 *	else:
	 *		405 Method Not Allowed
	 */
	case HTTPREQUEST_SKIN_SEND:
	{
		break;
	}
	default:;
	}
}

void HTTPClient::pushRequest(std::string &url, std::string &data)
{
	HTTPRequest r(url,data);
	m_req_mutex.Lock();
	m_requests.push_back(r);
	m_req_mutex.Unlock();
}

void HTTPClient::get(std::string &url)
{
}

void HTTPClient::post(std::string &url, char* data)
{
}

void HTTPClient::postFile(std::string &url, std::string &file)
{
}

HTTPRequest HTTPClient::popRequest()
{
	m_req_mutex.Lock();
	HTTPRequest r = m_requests.front();
	m_requests.erase(m_requests.begin());
	m_req_mutex.Unlock();
	return r;
}

void HTTPClient::sendHeaders()
{
}
#endif

/*
 * HTTPHeaders
 */

int HTTPHeaders::read(char* buff, int length)
{
	char nbuff[1024];
	char vbuff[1024];
	int n = 1;
	int o = 0;
	int c = m_url == "" ? 0 : 1;

	for (int i=0; i<length; i++) {
		if (buff[i] == '\r' || (!o && buff[i] == ' '))
			continue;
		if (buff[i] == '\n') {
			if (!c) {
				nbuff[o] = 0;
				printf("%s\n",nbuff);
				char* u = strchr(nbuff,' ');
				if (!u)
					return 1;
				*u = 0;
				setMethod(nbuff);
				u++;
				while (*u == ' ') {
					u++;
				}
				printf("%s\n",u);
				char* p = strchr(u,' ');
				if (!p)
					return 1;
				*p = 0;
				p++;
				while (*p == ' ') {
					p++;
				}
				printf("%s\n",p);
				if (!strcmp(p,"HTTP/1.1")) {
					setKeepAlive(true);
					setProtocol(p);
				}else{
					setKeepAlive(false);
					setProtocol("HTTP/1.0");
				}
				setUrl(nbuff);
				//size_t current;
				//size_t next = -1;
				//std::string s(u);
				//do{
					//current = next + 1;
					//next = s.find_first_of("/", current);
					//if (s.substr(current, next-current) != "")
						//m_url_split.push_back(s.substr(current, next-current));
				//} while (next != std::string::npos);
				c++;
			}else{
				if (n)
					return 0;
				vbuff[o] = 0;
				if (!strcmp(nbuff,"Content-Length")) {
					setLength(strtoul(vbuff,NULL,10));
				}else if (!strcmp(nbuff,"Cookie")) {
					setCookie(vbuff);
				}else if (!strcmp(nbuff,"Connection")) {
					if (!strcmp(vbuff,"keep-alive") || !strcmp(vbuff,"Keep-Alive")) {
						setKeepAlive(true);
					}else if (!strcmp(vbuff,"close") || !strcmp(vbuff,"Close")) {
						setKeepAlive(false);
					}
				}else{
					set(nbuff,vbuff);
				}
			}
			o = 0;
			n = 1;
		}else if (n && buff[i] == ':') {
			nbuff[o] = 0;
			o = 0;
			n = 0;
		}else if (n) {
			nbuff[o++] = buff[i];
		}else{
			vbuff[o++] = buff[i];
		}
	}

	return 2;
}
