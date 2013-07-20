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
		m_keepalive = false;
		m_cookie = std::string("");
		m_url = std::string("");
		m_url_split.clear();
	}
	int read(char* buff, int length);
	u32 length() {return m_contentLength;}
	bool keepAlive() {return m_keepalive;}
	std::string &cookie() {return m_cookie;}
	std::string &url() {return m_url;}
	std::string &url(int index) {return m_url_split[index];}
	std::string &get(std::string name) {return m_headers[name];}
	void set(std::string name, std::string value) {m_headers[name] = value;}
	void setLength(u32 length) {m_contentLength = length;}
	void setKeepAlive(bool ka) {m_keepalive = ka;}
	void setCookie(std::string cookie) {std::string c(cookie.c_str()); m_cookie = c;}
	void setUrl(std::string url) {std::string u(url); m_url = u;}
	void setMethod(std::string method) {std::string m(method); m_method = m;}
	void setProtocol(std::string proto) {std::string p(proto); m_protocol = p;}
private:
	std::map<std::string,std::string> m_headers;
	u32 m_contentLength;
	bool m_keepalive;
	std::string m_cookie;
	std::string m_url;
	std::vector<std::string> m_url_split;
	std::string m_method;
	std::string m_protocol;
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
	void send(char* data);
	void send(std::string &data) {send((char*)data.c_str());}
	void sendHTML(char* data);
	void sendHTML(std::string &data) {sendHTML((char*)data.c_str());}
	void sendFile(std::string &file);
	void setResponse(const char* response) {std::string r(response); m_response = r;}
private:
	void sendHeaders();
	HTTPHeaders m_recv_headers;
	HTTPHeaders m_send_headers;
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
	std::string getPlayerCookie(std::string &name) {return m_server->getPlayerCookie(name);}
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
		post("")
	{
	}
	HTTPRequest(std::string &u, std::string &p)
	{
		url = u;
		post = p;
	}
	HTTPRequest(std::string &u)
	{
		url = u;
		post = "";
	}
	std::string url;
	std::string post;
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
	void get(std::string &url);
	void post(std::string &url, char* data);
	void postFile(std::string &url, std::string &file);
	HTTPRequest popRequest();
	void sendHeaders();
	TCPSocket *m_socket;
	HTTPHeaders m_recv_headers;
	HTTPHeaders m_send_headers;
	std::string m_cookie;
	std::vector<HTTPRequest> m_requests;
	JMutex m_req_mutex;
	HTTPClientThread m_thread;
	Client *m_client;
};
#endif

#endif
