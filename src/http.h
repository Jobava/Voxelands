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
#include "utility.h"
#include "server.h"
#include "player.h"
#include <map>

class HTTPServer;

class HTTPServerThread : public SimpleThread
{
	HTTPServer *m_server;

public:

	HTTPServerThread(HTTPServer *server):
		SimpleThread(),
		m_server(server)
	{
	}

	void * Thread();
};

class HTTPHeaders
{
public:
	HTTPHeaders()
	{
		clear();
	}
	~HTTPHeaders() {};
	void clear()
	{
		m_contentLength = 0;
		m_url = std::string("");
		m_url_split.clear();
	}
	virtual int read(TCPSocket *sock) = 0;
	u32 length() {return m_contentLength;}
	std::string getUrl() {return m_url;}
	std::string getUrl(unsigned int index) { if (m_url_split.size() > index) return m_url_split[index]; return std::string("");}
	std::string getHeader(std::string name) {return m_headers[name];}
	std::string getMethod() {return m_method;}
	u32 getLength() {return m_contentLength;}

	void setUrl(std::string url) {std::string u(url); m_url = u;}
	void addUrl(std::string url) {m_url_split.push_back(url);}
	void setHeader(std::string name, std::string value) {m_headers[name] = value;}
	void setMethod(std::string method) {std::string m(method); m_method = m;}
	void setLength(u32 length) {m_contentLength = length;}
private:
	std::map<std::string,std::string> m_headers;
	u32 m_contentLength;
	std::string m_url;
	std::vector<std::string> m_url_split;
	std::string m_method;
};

class HTTPRequestHeaders : public HTTPHeaders
{
public:
	virtual int read(TCPSocket *sock);
private:
};

class HTTPResponseHeaders : public HTTPHeaders
{
public:
	virtual int read(TCPSocket *sock);
	void setResponse(int r) {m_response = r;}
	int getResponse() {return m_response;}
private:
	int m_response;
};

class HTTPRemoteClient
{
public:
	HTTPRemoteClient(TCPSocket *sock, HTTPServer *server):
		m_response(""),
		m_auth(false)
	{
		m_socket = sock;
		m_server = server;
	}
	~HTTPRemoteClient();
	int receive();
private:
	int read(char* buff, int size) {return m_socket->Receive(buff,size);}
	int readline(char* buff, int size) {return m_socket->ReceiveLine(buff,size);}
	void sendHeaders();

	int handleTexture();
	int handlePlayer();
	int handleMap();
	int handleIndex();
	int handleAPI();
	int handleSpecial(const char* response, std::string content);
	int handleSpecial(const char* response) {return handleSpecial(response,"");}

	void send(char* data);
	void send(std::string &data) {send((char*)data.c_str());}
	void sendHTML(char* data);
	void sendHTML(std::string &data) {sendHTML((char*)data.c_str());}
	void sendFile(std::string &file);
	void setResponse(const char* response) {std::string r(response); m_response = r;}

	HTTPRequestHeaders m_recv_headers;
	HTTPResponseHeaders m_send_headers;
	std::string m_response;
	bool m_auth;
	TCPSocket *m_socket;
	HTTPServer *m_server;
};

class HTTPServer
{
public:
	HTTPServer(Server &server);
	~HTTPServer();
	void start(u16 port);
	void stop();
	void step();
	std::string getPlayerPrivs(std::string name) {return privsToString(m_server->getPlayerAuthPrivs(name));}
	Server *getGameServer() {return m_server;}
private:
	TCPSocket *m_socket;
	std::vector<HTTPRemoteClient*> m_peers;
	HTTPServerThread m_thread;
	Server *m_server;
};

std::string http_request(char* host, char* url, char* post=NULL, int port=80);
std::string http_url_encode(std::string &str);

#endif
