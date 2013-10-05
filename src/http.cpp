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
#include "main.h"
#include "settings.h"
#include "filesys.h"
#include "debug.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include "utility.h"
#include "connection.h"
#include "log.h"
#include "sha1.h"
#include "path.h"
#include "config.h"

/* server thread main loop */
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

/* constructor */
HTTPServer::HTTPServer(Server &server):
	m_thread(this)
{
	m_server = &server;
}

/* destructor */
HTTPServer::~HTTPServer()
{
}

/* start the server running */
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

/* stop the running server */
void HTTPServer::stop()
{
	DSTACK(__FUNCTION_NAME);

	m_thread.stop();
	delete m_socket;

	infostream<<"HTTPServer: Threads stopped"<<std::endl;
}

/* the main function for the server loop */
void HTTPServer::step()
{
	if (m_socket->WaitData(50)) {
		TCPSocket *s = m_socket->Accept();
		HTTPRemoteClient *c = new HTTPRemoteClient(s,this);
		m_peers.push_back(c);
	}

	std::vector<HTTPRemoteClient*> p;

	p.swap(m_peers);

	for (std::vector<HTTPRemoteClient*>::iterator i = p.begin(); i != p.end(); ++i) {
		HTTPRemoteClient *c = *i;
		try{
			if (c->receive()) {
				delete c;
				continue;
			}
		}catch (SocketException &e) {
			delete c;
			continue;
		}
		m_peers.push_back(c);
	}
}

/*
 * HTTPRemoteClient
 */

/* destructor */
HTTPRemoteClient::~HTTPRemoteClient()
{
	delete m_socket;
}

/* receive and handle data from a remote http client */
int HTTPRemoteClient::receive()
{
	int r = fillBuffer();
	if (!r)
		return 0;
	if (r<1)
		return 1;

	m_recv_headers.clear();
	m_send_headers.clear();

	int h = m_recv_headers.read(m_buff,r);
	if (h == -1) {
		return 1;
	}else if (h == -2) {
		if (m_recv_headers.getUrl() == "")
			return 1;
		while (m_socket->WaitData(1000)) {
			r = fillBuffer();
			if (r<1)
				return 1;
			h = m_recv_headers.read(m_buff,r);
			if (h > -1)
				break;
			if (h == -1)
				return 1;
		}
		if (h < 0)
			return 1;
	}

	if (m_recv_headers.getCookie() != "" && m_recv_headers.getHeader("User") != "") {
		if (m_recv_headers.getCookie() == m_server->getPlayerCookie(m_recv_headers.getHeader("User"))) {
			m_send_headers.setCookie(m_recv_headers.getCookie());
			m_auth = true;
		}
	}

	if (m_recv_headers.getUrl(0) == "texture") {
		return handleTexture();
	}else if (m_recv_headers.getUrl(0) == "player") {
		return handlePlayer();
	}else if (m_recv_headers.getUrl(0) == "") {
		return handleIndex();
	}

	return handleSpecial("404 Not Found");
}

/* read data from a remote http client */
int HTTPRemoteClient::read(char* buff, int size)
{
	if (size > 2048) {
		int r = 0;
		int l;
		int c = 2048;
		int s = size;
		while (fillBuffer()) {
			c = 2048;
			if (s<c)
				c = s;
			l = m_end-m_start;
			if (l<c)
				c = l;
			if (c < 1)
				break;
			memcpy(buff+r,m_buff+m_start,c);
			m_start += c;
			r += c;
			s -= c;
		}
		return r;
	}

	if (fillBuffer() < size)
		return 0;

	memcpy(buff,m_buff+m_start,size);
	m_start+=size;
	return size;
}

/* data read from remote http clients is buffered, fill the buffer */
int HTTPRemoteClient::fillBuffer()
{
	int l = m_end-m_start;
	if (l && m_start) {
		memcpy(m_buff,m_buff+m_start,l);
		m_start = 0;
		m_end = l;
	}
	l = 2048-m_end;

	if (!m_socket->WaitData(30))
		return m_end;

	return m_end+m_socket->Receive(m_buff+m_end,l);
}

/* handle /player/<name> url's */
int HTTPRemoteClient::handlePlayer()
{
	char buff[2048];
	/* player list */
	if (m_recv_headers.getUrl(1) == "") {
		core::list<Player*> players = m_server->getGameServer()->getPlayers();
		std::string html("<h1>Players</h1>\n");
		for (core::list<Player*>::Iterator i = players.begin(); i != players.end(); i++) {
			Player *player = *i;
			html += "<div class=\"panel\"><h2><a href=\"/player/";
			html += player->getName();
			html += "\" class=\"secret\">";
			html += player->getName();
			html += "</a></h2>";
			html += "<p class=\"right\"><img src=\"/player/";
			html += player->getName();
			html += "/skin\" /></p>";
			snprintf(buff, 2048,"% .1f, % .1f, % .1f",player->getPosition().X/BS,player->getPosition().Y/BS,player->getPosition().Z/BS);
			if (player->peer_id == 0) {
				html += "<p class=\"red\">Offline</p>";
				html += "<p><strong>Last seen at:</strong> ";
			}else{
				html += "<p class=\"green bold\">Online</p>";
				html += "<p><strong>Currently at:</strong> ";
			}
			html += buff;
			html += "</p><p><strong>Privileges:</strong> ";
			html += m_server->getPlayerPrivs(player->getName());
			html += "</p></div>";
		}
		sendHTML((char*)html.c_str());
		return 1;
	/* player skin */
	}else if (m_recv_headers.getUrl(2) == "skin") {
		std::string data_path = g_settings->get("data_path");
		if (data_path == "")
			data_path = "data";
		std::string file = data_path + DIR_DELIM + "textures" + DIR_DELIM + "players" + DIR_DELIM + "player_" + m_recv_headers.getUrl(1) + ".png";
		/* compare hash */
		if (m_recv_headers.getUrl(3) != "") {
			FILE *f;
			f = fopen(file.c_str(),"rb");
			if (!f)
				return handleSpecial("204 No Content");
			fclose(f);
			SHA1 s;
			s.addFile(file.c_str());
			s.getDigest(buff);
			if (std::string(buff) == m_recv_headers.getUrl(3)) {
				return handleSpecial("204 No Content");
			}else if (m_auth && m_recv_headers.getHeader("User") == m_recv_headers.getUrl(1)) {
				return handleSpecial("304 Not Modified");
			}
			m_send_headers.setHeader("Content-Type","image/png");
			sendFile(file);
			return 1;
		}
		/* get file */
		if (m_recv_headers.getMethod() != "PUT") {
			m_send_headers.setHeader("Content-Type","image/png");
			sendFile(file);
			return 1;
		}
		/* put only works for the owner */
		if (!m_auth || m_recv_headers.getHeader("User") != m_server->getPlayerFromCookie(m_recv_headers.getCookie()))
			return handleSpecial("405 Method Not Allowed");
		FILE *f;
		f = fopen(file.c_str(),"wb");
		if (!f)
			return handleSpecial("500 Internal Server Error");
		size_t s = m_recv_headers.getLength();
		if (!s)
			return handleSpecial("411 Length Required");
		size_t l;
		size_t c = 2048;
		if (c > s)
			c = s;
		if (c) {
			while ((l = read(buff,c)) > 0) {
				s -= l;
				c = fwrite(buff,1,l,f);
				if (c != l) {
					fclose(f);
					return handleSpecial("500 Internal Server Error");
				}
				c = 2048;
				if (c > s)
					c = s;
				if (!c)
					break;
			}
		}
		fclose(f);
		return handleSpecial("201 Created");
	}else if (m_server->getGameServer()->getPlayer(m_recv_headers.getUrl(1))) {
		std::string html("<h1>Players</h1>\n");
		Player *player = m_server->getGameServer()->getPlayer(m_recv_headers.getUrl(1));
		html += "<div class=\"panel\"><h2>";
		html += player->getName();
		html += "</h2>";
		html += "<p class=\"right\"><img src=\"/player/";
		html += player->getName();
		html += "/skin\" /></p>";
		snprintf(buff, 2048,"% .1f, % .1f, % .1f",player->getPosition().X/BS,player->getPosition().Y/BS,player->getPosition().Z/BS);
		if (player->peer_id == 0) {
			html += "<p class=\"red\">Offline</p>";
			html += "<p><strong>Last seen at:</strong> ";
		}else{
			html += "<p class=\"green bold\">Online</p>";
			html += "<p><strong>Currently at:</strong> ";
		}
		html += buff;
		html += "</p><p><strong>Privileges:</strong> ";
		html += m_server->getPlayerPrivs(player->getName());
		html += "</p></div>";
		sendHTML((char*)html.c_str());
		return 1;
	}
	return handleSpecial("404 Not Found");
}

/* handle /texture/<file> url's */
int HTTPRemoteClient::handleTexture()
{
	std::string file = getTexturePath(m_recv_headers.getUrl(1));
	m_send_headers.setHeader("Content-Type","image/png");
	sendFile(file);
	return 1;
}

/* handle /model/<file> url's */
int HTTPRemoteClient::handleModel()
{
	std::string file = getModelPath(m_recv_headers.getUrl(1));
	m_send_headers.setHeader("Content-Type","application/octet-stream");
	sendFile(file);
	return 1;
}

/* handle /map/<x>/<y>/<z> url's */
int HTTPRemoteClient::handleMap()
{
	return handleSpecial("404 Not Found");
}

/* handle / url's */
int HTTPRemoteClient::handleIndex()
{
	int c = 0;
	std::string html("<div class=\"panel\"><h2>");
	html += g_settings->get("motd");
	html += "</h2><p><strong>Version: </strong>";
	html += VERSION_STRING;
	html += "<br /><strong><a href=\"/player\" class=\"secret\">Players</a>: </strong>";
	core::list<Player*> players = m_server->getGameServer()->getPlayers();
	for (core::list<Player*>::Iterator i = players.begin(); i != players.end(); i++) {
		Player *player = *i;
		if (player->peer_id != 0) {
			if (c++)
				html += ", ";
			html += "<a href=\"/player/";
			html += player->getName();
			html += "\" class=\"secret\">";
			html += player->getName();
			html += "</a>";
		}
	}
	html += "</div>";
	sendHTML((char*)html.c_str());
	return 1;
}

/* simple wrapper for sending html content and/or errors */
int HTTPRemoteClient::handleSpecial(const char* response, std::string content)
{
	setResponse(response);
	std::string html("<h1>");
	html += std::string(response) + "</h1>" + content;
	sendHTML(html);
	return 1;
}

/* send text data to a remote http client */
void HTTPRemoteClient::send(char* data)
{
	int l = strlen(data);
	m_send_headers.setHeader("Content-Type","text/plain");
	m_send_headers.setLength(l);
	sendHeaders();
	m_socket->Send(data,l);
}

/* send html data to a remote http client */
void HTTPRemoteClient::sendHTML(char* data)
{
	FILE *h;
	FILE *f;
	int l[4];
	char* b;
	std::string data_path = g_settings->get("data_path");
	if (data_path == "")
		data_path = "data";
	std::string file = data_path + DIR_DELIM + "html" + DIR_DELIM + "header.html";
	h = fopen(file.c_str(),"r");
	if (!h && data_path != "data") {
		file = std::string("data") + DIR_DELIM + "html" + DIR_DELIM + "header.html";
		h = fopen(file.c_str(),"r");
	}
	file = data_path + DIR_DELIM + "html" + DIR_DELIM + "footer.html";
	f = fopen(file.c_str(),"r");
	if (!f && data_path != "data") {
		file = std::string("data") + DIR_DELIM + "html" + DIR_DELIM + "footer.html";
		f = fopen(file.c_str(),"r");
	}

	if (h) {
		fseek(h,0,SEEK_END);
		l[0] = ftell(h);
		fseek(h,0,SEEK_SET);
	}else{
		l[0] = 0;
	}
	l[1] = strlen(data);
	if (f) {
		fseek(f,0,SEEK_END);
		l[2] = ftell(f);
		fseek(f,0,SEEK_SET);
	}else{
		l[2] = 0;
	}

	if (l[0] > l[2]) {
		b = (char*)alloca(l[0]);
	}else{
		b = (char*)alloca(l[2]);
	}

	l[3] = l[0]+l[1]+l[2];
	m_send_headers.setHeader("Content-Type","text/html");
	m_send_headers.setLength(l[3]);
	sendHeaders();
	if (h) {
		l[3] = fread(b,1,l[0],h);
		m_socket->Send(b,l[3]);
		fclose(h);
	}
	m_socket->Send(data,l[1]);
	if (f) {
		l[3] = fread(b,1,l[2],f);
		m_socket->Send(b,l[3]);
		fclose(f);
	}
}

/* send a file to a remote http client */
void HTTPRemoteClient::sendFile(std::string &file)
{
	FILE *f;
	f = fopen(file.c_str(),"rb");
	if (!f) {
		std::string data_path = g_settings->get("data_path");
		if (data_path == "")
			data_path = "data";
		if (file.substr(data_path.size()+1,24) == "textures/players/player_") {
			m_send_headers.setHeader("Location","/texture/character.png");
			handleSpecial("303 See Other","<p><a href=\"/texture/character.png\">/texture/character.png</a></p>");
			return;
		}
		handleSpecial("404 Not Found");
		return;
	}
	fseek(f,0,SEEK_END);
	size_t l = ftell(f);
	fseek(f,0,SEEK_SET);

	m_send_headers.setLength(l);
	sendHeaders();

	char buff[1024];
	while ((l = fread(buff,1,1024,f)) > 0) {
		m_socket->Send(buff,l);
	}

	fclose(f);
}

/* send response headers to a remote http client */
void HTTPRemoteClient::sendHeaders()
{
	std::string v;
	int s;
	char buff[1024];

	v = m_response;
	if (v == "")
		v = std::string("200 OK");

	s = snprintf(buff,1024,"HTTP/1.0 %s\r\n",v.c_str());
	m_socket->Send(buff,s);
	//m_socket->Send("HTTP/1.0 200 OK\r\n",17);

	v = m_send_headers.getHeader("Content-Type");
	if (v == "") {
		m_socket->Send("Content-Type: text/plain\r\n",26);
	}else{
		s = snprintf(buff,1024,"Content-Type: %s\r\n",v.c_str());
		m_socket->Send(buff,s);
	}

	s = m_send_headers.length();
	s = snprintf(buff,1024,"Content-Length: %d\r\n",s);
	m_socket->Send(buff,s);

	v = m_send_headers.getCookie();
	if (v != "") {
		s = snprintf(buff,1024,"Set-Cookie: MTID=%s\r\n",v.c_str());
		m_socket->Send(buff,s);
	}

	v = m_send_headers.getHeader("Location");
	if (v != "") {
		s = snprintf(buff,1024,"Location: %s\r\n",v.c_str());
		m_socket->Send(buff,s);
	}

	v = m_send_headers.getHeader("Refresh");
	if (v != "") {
		s = snprintf(buff,1024,"Refresh: %s\r\n",v.c_str());
		m_socket->Send(buff,s);
	}

	m_socket->Send("\r\n",2);
}

#ifndef SERVER
#include "client.h"

/* main loop for the client http fetcher */
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

/* constructor */
HTTPClient::HTTPClient(Client *client):
	m_cookie(""),
	m_thread(this)
{
	m_client = client;
	m_req_mutex.Init();
}

/* destructor */
HTTPClient::~HTTPClient()
{
}

/* start the client http fetcher thread */
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

/* stop the client http fetcher thread */
void HTTPClient::stop()
{
	DSTACK(__FUNCTION_NAME);

	m_thread.stop();
	delete m_socket;

	infostream<<"HTTPClient: Threads stopped"<<std::endl;
}

/* the main function for the client loop */
void HTTPClient::step()
{
	sleep(1);
}

/* add a request to the http client queue */
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
		std::string tex = std::string("players") + DIR_DELIM + "player_" + data + ".png";
		std::string ptex = getTexturePath(tex);
		if (ptex == "") {
			pushRequest(HTTPREQUEST_SKIN,data);
			return;
		}
		char buff[100];
		SHA1 s;
		s.addFile(ptex.c_str());
		s.getDigest(buff);
		std::string url("/player/");
		url += data + "/skin/" + buff;
		HTTPRequest r(url);
		m_req_mutex.Lock();
		m_requests.push_back(r);
		m_req_mutex.Unlock();
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
		std::string tex = std::string("player.png");
		std::string ptex = getTexturePath(tex);
		if (ptex == "")
			return;

		std::string url("/player/");
		url += data + "/skin";
		HTTPRequest r(url,ptex);
		m_req_mutex.Lock();
		m_requests.push_back(r);
		m_req_mutex.Unlock();
		break;
	}
	default:;
	}
}

/* add a request to the http client queue */
void HTTPClient::pushRequest(std::string &url, std::string &data)
{
	HTTPRequest r(url,data);
	m_req_mutex.Lock();
	m_requests.push_back(r);
	m_req_mutex.Unlock();
}

/* send a http GET request to the server */
void HTTPClient::get(std::string &url)
{
}

/* send a http POST request to the server */
void HTTPClient::post(std::string &url, char* data)
{
}

/* send a file to the server with a http PUT request */
void HTTPClient::put(std::string &url, std::string &file)
{
}

/* get a request from the client queue */
HTTPRequest HTTPClient::popRequest()
{
	m_req_mutex.Lock();
	HTTPRequest r = m_requests.front();
	m_requests.erase(m_requests.begin());
	m_req_mutex.Unlock();
	return r;
}

/* send http headers to the server */
void HTTPClient::sendHeaders()
{
}
#endif

/*
 * HTTPHeaders
 */

/* read in headers */
int HTTPRequestHeaders::read(char* buff, int length)
{
	char nbuff[1024];
	char vbuff[1024];
	int n = 1;
	int o = 0;
	int i = 0;
	int c = getUrl() == "" ? 0 : 1;

	for (i=0; i<length; i++) {
		if (buff[i] == '\r' || (!o && buff[i] == ' '))
			continue;
		if (buff[i] == '\n') {
			if (!c) {
				nbuff[o] = 0;
				char* u = strchr(nbuff,' ');
				if (!u)
					return -1;
				*u = 0;
				setMethod(nbuff);
				u++;
				while (*u == ' ') {
					u++;
				}
				char* p = strchr(u,' ');
				if (!p)
					return -1;
				*p = 0;
				p++;
				while (*p == ' ') {
					p++;
				}
				setUrl(u);
				size_t current;
				size_t next = -1;
				std::string s(u);
				do{
					current = next + 1;
					next = s.find_first_of("/", current);
					if (s.substr(current, next-current) != "")
						addUrl(s.substr(current, next-current));
				} while (next != std::string::npos);
				c++;
			}else{
				if (n)
					return i+1;
				vbuff[o] = 0;
				if (!strcmp(nbuff,"Content-Length")) {
					setLength(strtoul(vbuff,NULL,10));
				}else if (!strcmp(nbuff,"Cookie") && !strncmp(vbuff,"MTID=",5)) {
					printf("cookie: '%s'\n",vbuff+5);
					setCookie(vbuff+5);
				}else{
					setHeader(nbuff,vbuff);
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

	return -2;
}

/* read in headers */
int HTTPResponseHeaders::read(char* buff, int length)
{
	char nbuff[1024];
	char vbuff[1024];
	int n = 1;
	int o = 0;
	int i = 0;
	int c = getUrl() == "" ? 0 : 1;

	for (i=0; i<length; i++) {
		if (buff[i] == '\r' || (!o && buff[i] == ' '))
			continue;
		if (buff[i] == '\n') {
			if (!c) {
				nbuff[o] = 0;
				char* r = strchr(nbuff,' ');
				if (!r)
					return -1;
				*r = 0;
				r++;
				while (*r == ' ') {
					r++;
				}
				char* s = strchr(r,' ');
				if (!s)
					return -1;
				*s = 0;
				setResponse(strtol(r,NULL,10));
				c++;
			}else{
				if (n)
					return i+1;
				vbuff[o] = 0;
				if (!strcmp(nbuff,"Content-Length")) {
					setLength(strtoul(vbuff,NULL,10));
				}else if (!strcmp(nbuff,"SetCookie") && !strncmp(vbuff,"MTID=",5)) {
					printf("cookie: '%s'\n",vbuff+5);
					setCookie(vbuff+5);
				}else{
					setHeader(nbuff,vbuff);
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

	return -2;
}
