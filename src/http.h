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
		m_cookie = std::string("");
		m_url = std::string("");
		m_url_split.clear();
	}
	virtual int read(char* buff, int length) = 0;
	u32 length() {return m_contentLength;}
	std::string getCookie() {return m_cookie;}
	std::string getUrl() {return m_url;}
	std::string getUrl(unsigned int index) { if (m_url_split.size() > index) return m_url_split[index]; return std::string("");}
	std::string getHeader(std::string name) {return m_headers[name];}
	std::string getMethod() {return m_method;}
	u32 getLength() {return m_contentLength;}

	void setCookie(std::string cookie) {std::string c(cookie.c_str()); m_cookie = c;}
	void setUrl(std::string url) {std::string u(url); m_url = u;}
	void addUrl(std::string url) {m_url_split.push_back(url);}
	void setHeader(std::string name, std::string value) {m_headers[name] = value;}
	void setMethod(std::string method) {std::string m(method); m_method = m;}
	void setLength(u32 length) {m_contentLength = length;}
private:
	std::map<std::string,std::string> m_headers;
	u32 m_contentLength;
	std::string m_cookie;
	std::string m_url;
	std::vector<std::string> m_url_split;
	std::string m_method;
};

class HTTPRequestHeaders : public HTTPHeaders
{
public:
	virtual int read(char* buff, int length);
private:
};

class HTTPResponseHeaders : public HTTPHeaders
{
public:
	virtual int read(char* buff, int length);
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
		m_start = 0;
		m_end = 0;
	}
	~HTTPRemoteClient();
	int receive();
private:
	int read(char* buff, int size);
	int fillBuffer();
	void sendHeaders();

	int handlePlayer();
	int handleTexture();
	int handleModel();
	int handleMap();
	int handleIndex();
	int handleSpecial(const char* response, std::string content);
	int handleSpecial(const char* response) {return handleSpecial(response,"");}

	void send(char* data);
	void send(std::string &data) {send((char*)data.c_str());}
	void sendHTML(char* data);
	void sendHTML(std::string &data) {sendHTML((char*)data.c_str());}
	void sendFile(std::string &file);
	void setResponse(const char* response) {std::string r(response); m_response = r;}

	char m_buff[2048];
	int m_start;
	int m_end;
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
	std::string getPlayerCookie(std::string name) {return m_server->getPlayerCookie(name);}
	std::string getPlayerFromCookie(std::string cookie) {return m_server->getPlayerFromCookie(cookie);}
	std::string getPlayerPrivs(std::string name) {return privsToString(m_server->getPlayerAuthPrivs(name));}
	Server *getGameServer() {return m_server;}
private:
	TCPSocket *m_socket;
	std::vector<HTTPRemoteClient*> m_peers;
	HTTPServerThread m_thread;
	Server *m_server;
};

enum HTTPRequestType {
	HTTPREQUEST_NULL,
	HTTPREQUEST_SKIN,
	HTTPREQUEST_SKIN_HASH,
	HTTPREQUEST_SKIN_SEND
};

struct HTTPRequest
{
	HTTPRequest():
		url(""),
		post(""),
		data("")
	{
	}
	HTTPRequest(std::string &u, std::string &p, std::string &d)
	{
		url = u;
		post = p;
		data = d;
	}
	HTTPRequest(std::string &u, std::string &p)
	{
		url = u;
		post = p;
		data = "";
	}
	HTTPRequest(std::string &u)
	{
		url = u;
		post = "";
		data = "";
	}
	std::string url;
	std::string post;
	std::string data;
};

#ifndef SERVER
#include "client.h"
class HTTPClient;

class HTTPClientThread : public SimpleThread
{
	HTTPClient *m_client;

public:

	HTTPClientThread(HTTPClient *client):
		SimpleThread(),
		m_client(client)
	{
	}

	void * Thread();
};

class HTTPClient
{
public:
	HTTPClient(Client *client);
	~HTTPClient();
	void start(const Address &address);
	void stop();
	void step();
	void setCookie(std::string &cookie) {std::string c(cookie.c_str()); m_cookie = c;}
	void pushRequest(HTTPRequestType type, std::string &data);
	void pushRequest(std::string &url, std::string &data);
private:
	bool get(std::string &url);
	bool post(std::string &url, char* data);
	bool put(std::string &url, std::string &file);
	int read(char* buff, int size);
	int fillBuffer();
	void sendHeaders();

	char m_buff[2048];
	int m_start;
	int m_end;
	Address m_address;
	TCPSocket *m_socket;
	HTTPResponseHeaders m_recv_headers;
	HTTPRequestHeaders m_send_headers;
	std::string m_cookie;
	std::vector<HTTPRequest> m_requests;
	JMutex m_req_mutex;
	HTTPClientThread m_thread;
	Client *m_client;
};
#endif

#endif
