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

/* interface builders, these just keep some code below clean */
static std::string http_player_interface(Player *player, HTTPServer *server, bool full)
{
	char buff[2048];
	std::string html("<div class=\"panel\"><h2><a href=\"/player/");
	html += player->getName();
	html += "\" class=\"secret\">";
	html += player->getName();
	html += "</a></h2>";
	if (full) {
		//html += "<p class=\"right\"><img src=\"/player/";
		//html += player->getName();
		//html += "/skin\" /></p>";
	}
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
	html += server->getPlayerPrivs(player->getName());
	html += "</p></div>";
	return html;
}

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
		}catch(SendFailedException &e) {
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
		if (m_peers.size() > 20) {
			delete s;
		}else{
			HTTPRemoteClient *c = new HTTPRemoteClient(s,this);
			m_peers.push_back(c);
		}
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
	m_recv_headers.clear();
	m_send_headers.clear();

	int h = m_recv_headers.read(m_socket);
	if (h == -1)
		return 1;

	std::string u0 = m_recv_headers.getUrl(0);

	if (u0 == "texture") {
		return handleTexture();
	}else if (u0 == "player") {
		return handlePlayer();
	}else if (u0 == "map") {
		return handleMap();
	}else if (u0 == "api") {
		return handleAPI();
	}else if (u0 == "") {
		return handleIndex();
	}

	return handleSpecial("404 Not Found");
}

/* handle /player/<name> url's */
int HTTPRemoteClient::handlePlayer()
{
	/* player list */
	std::string u1 = m_recv_headers.getUrl(1);
	if (u1 == "" || u1.substr(0,5) == "page-") {
		core::list<Player*> players = m_server->getGameServer()->getPlayers();
		std::string html("<h1>Players</h1>\n");
		std::string pagination("");
		int player_skip = 0;
		if (players.size() > 50) {
			int current_page = 1;
			if (u1.substr(0,5) == "page-") {
				current_page = mystoi(u1.substr(5));
				if (current_page < 1)
					current_page = 1;
			}
			int total_pages = (players.size()/50)+1;
			if (total_pages < 1)
				total_pages = 1;
			if (current_page > total_pages)
				current_page = total_pages;
			int prev_page = current_page-1;
			int next_page = current_page+1;
			if (prev_page < 1)
				prev_page = 1;
			if (next_page > total_pages)
				next_page = total_pages;
			pagination = std::string("<div class=\"pagination\"><a href=\"/player/page-")+itos(prev_page)+"\">&lt;&lt; prev</a> ";
			pagination += std::string("Page ")+itos(current_page)+" of "+itos(total_pages)+" ";
			pagination += std::string("<a class=\"pagination\" href=\"/player/page-")+itos(next_page)+"\">next &gt;&gt;</a></div>";
			player_skip = (current_page-1)*50;
		}
		html += pagination;
		int p = 0;
		for (core::list<Player*>::Iterator i = players.begin(); i != players.end(); i++,p++) {
			if (p < player_skip)
				continue;
			if (p > player_skip+50)
				break;
			Player *player = *i;
			html += http_player_interface(player,m_server,false);
		}
		html += pagination;
		sendHTML((char*)html.c_str());
		return 1;
	}else if (m_server->getGameServer()->getPlayer(u1)) {
		std::string html("<h1>Players</h1>\n");
		Player *player = m_server->getGameServer()->getPlayer(m_recv_headers.getUrl(1));
		html += http_player_interface(player,m_server,true);
		sendHTML((char*)html.c_str());
		return 1;
	}
	return handleSpecial("404 Not Found");
}

/* handle /texture/<file> url's */
int HTTPRemoteClient::handleTexture()
{
	std::string file = getPath("texture",m_recv_headers.getUrl(1),true);
	if (file == "")
		return handleSpecial("404 Not Found");
	m_send_headers.setHeader("Content-Type","image/png");
	sendFile(file);
	return 1;
}

/* handle /map/<x>/<y>/<z> url's */
int HTTPRemoteClient::handleMap()
{
	return handleSpecial("404 Not Found");
}

/* handle /api/xxx url's */
int HTTPRemoteClient::handleAPI()
{

	std::string u1 = m_recv_headers.getUrl(1);

	if (u1 == "summary" || u1 == "") {
		std::string txt(VERSION_STRING);
		txt += "\n";
		std::string n = g_settings->get("server_name");
		if (n == "")
			n = g_settings->get("server_address");
		txt += n+"\n";
		txt += g_settings->get("motd")+"\n";
		txt += g_settings->get("server_address")+"\n";
		txt += g_settings->get("port")+"\n";
		txt += g_settings->get("game_mode")+"\n";
		if (g_settings->get("default_password") == "") {
			txt += "public\n";
		}else{
			txt += "private\n";
		}
		txt += g_settings->get("default_privs")+"\n";
		txt += "summary,motd,mode,name,players,public,version,privs,features";
		send((char*)txt.c_str());
		return 1;
	}else if (u1 == "motd") {
		std::string txt = g_settings->get("motd");
		send((char*)txt.c_str());
		return 1;
	}else if (u1 == "mode") {
		std::string txt = g_settings->get("game_mode");
		send((char*)txt.c_str());
		return 1;
	}else if (u1 == "name") {
		std::string txt = g_settings->get("server_name");
		if (txt == "")
			txt = g_settings->get("server_address");
		send((char*)txt.c_str());
		return 1;
	}else if (u1 == "features") {
		std::string txt = "summary,motd,mode,name,players,public,version,privs,features";
		send((char*)txt.c_str());
		return 1;
	}else if (u1 == "version") {
		std::string txt = VERSION_STRING;
		send((char*)txt.c_str());
		return 1;
	}else if (u1 == "privs") {
		std::string txt = g_settings->get("default_privs");
		send((char*)txt.c_str());
		return 1;
	}else if (u1 == "players") {
		core::list<Player*> players = m_server->getGameServer()->getPlayers(true);
		std::string txt = itos(players.size())+"\n";
		int c = 0;
		for (core::list<Player*>::Iterator i = players.begin(); i != players.end(); i++) {
			Player *player = *i;
			if (c++)
				txt += ", ";
			txt += player->getName();
		}
		send((char*)txt.c_str());
		return 1;
	}else if (u1 == "public") {
		if (g_settings->get("default_password") == "") {
			send((char*)"public");
		}else{
			send((char*)"private");
		}
		return 1;
	}

	setResponse("404 Not Found");
	send((char*)"404 Not Found");
	return 1;
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
	core::list<Player*> players = m_server->getGameServer()->getPlayers(true);
	for (core::list<Player*>::Iterator i = players.begin(); i != players.end(); i++) {
		Player *player = *i;
		if (c++)
			html += ", ";
		html += "<a href=\"/player/";
		html += player->getName();
		html += "\" class=\"secret\">";
		html += player->getName();
		html += "</a>";
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
	std::string file = getPath("html","header.html",true);
	h = fopen(file.c_str(),"r");
	file = getPath("html","footer.html",true);
	f = fopen(file.c_str(),"r");

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
		b = new char[l[0]];
	}else{
		b = new char[l[2]];
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

	delete b;
}

/* send a file to a remote http client */
void HTTPRemoteClient::sendFile(std::string &file)
{
	FILE *f;
	f = fopen(file.c_str(),"rb");
	if (!f) {
		handleSpecial("404 Not Found");
		return;
	}
	fseek(f,0,SEEK_END);
	size_t l = ftell(f);
	fseek(f,0,SEEK_SET);
	//size_t s = l;
	size_t t = 0;

	m_send_headers.setLength(l);
	sendHeaders();

	char buff[1024];
	while ((l = fread(buff,1,1024,f)) > 0) {
		t += l;
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

/*
 * HTTP request
 */

std::string http_request(char* host, char* url, char* post, int port)
{
	Address addr;
	TCPSocket *sock;
	HTTPResponseHeaders headers;
	int s;
	char buff[2048];

	addr.setPort(port);
	if (!host || !host[0]) {
		std::string h = g_settings->get("api_server");
		if (h == "")
			return "";
		host = (char*)h.c_str();
	}

	addr.Resolve(host);

	sock = new TCPSocket();
	if (!sock)
		return "";

	if (!sock->Connect(addr)) {
		delete sock;
		return "";
	}

	if (post) {
		int l = strlen(post);
		s = snprintf(buff,2048,
			"POST %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"From: Voxelands HTTP Fetcher\r\n"
			"User-Agent: Voxelands/%s (Irrlicht; Voxelands) Voxelands/%s\r\n"
			"Accept: text/html,application/xhtml+xml,text/plain\r\n"
			"Accept-Language: en-us,en\r\n"
			"Accept-Charset: ISO-8859-1,utf-8\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"Content-Length: %d\r\n"
			"Connection: close\r\n\r\n",
			url,
			host,
			VERSION_STRING,
			VERSION_STRING,
			l
		);
		sock->Send(buff,s);
		sock->Send(post,l);
	}else{
		s = snprintf(buff,2048,
			"GET %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"From: Voxelands HTTP Fetcher\r\n"
			"User-Agent: Voxelands/%s (Irrlicht; Voxelands) Voxelands/%s\r\n"
			"Accept: text/html,application/xhtml+xml,text/plain\r\n"
			"Accept-Language: en-us,en\r\n"
			"Accept-Charset: ISO-8859-1,utf-8\r\n"
			"Connection: close\r\n\r\n",
			url,
			host,
			VERSION_STRING,
			VERSION_STRING
		);
		sock->Send(buff,s);
	}

	if (!sock->WaitData(5000)) {
		delete sock;
		return "";
	}

	if (headers.read(sock) < 0) {
		delete sock;
		return "";
	}

	if (headers.getResponse() != 200) {
		delete sock;
		return "";
	}

	std::string response("");

	while (response.size() < headers.getLength() && (s = sock->Receive(buff,2047)) > 0) {
		buff[s] = 0;
		response += buff;
	}

	delete sock;

	return response;
}

/*
 * HTTPHeaders
 */

/* read in headers */
int HTTPRequestHeaders::read(TCPSocket *sock)
{
	char lbuff[2048];
	char* n;
	char* v;
	int i = 0;

	i = sock->ReceiveLine(lbuff,2048);
	if (!i)
		return -1;
	n = lbuff;
	lbuff[i] = 0;
	{
		v = strchr(n,' ');
		if (!v)
			return -1;
		*v = 0;
		setMethod(n);
		v++;
		while (*v == ' ') {
			v++;
		}
		n = v;
	}
	{
		v = strchr(n,' ');
		if (!v)
			return -1;
		*v = 0;
		setUrl(n);
		size_t current;
		size_t next = -1;
		std::string s(n);
		do{
			current = next + 1;
			next = s.find_first_of("/", current);
			if (s.substr(current, next-current) != "")
				addUrl(s.substr(current, next-current));
		} while (next != std::string::npos);
	}

	while ((i = sock->ReceiveLine(lbuff,2048))) {
		n = lbuff;
		v = strchr(lbuff,':');
		if (!v)
			return -1;
		*v = 0;
		v++;
		while (*v && *v == ' ') {
			v++;
		}
		if (!strcmp(n,"Content-Length")) {
			setLength(strtoul(v,NULL,10));
		}else{
			setHeader(n,v);
		}
	}

	return 0;
}

/* read in headers */
int HTTPResponseHeaders::read(TCPSocket *sock)
{
	char lbuff[2048];
	char* n;
	char* v;
	int i = 0;

	i = sock->ReceiveLine(lbuff,2048);
	if (!i)
		return -1;
	n = lbuff;
	lbuff[i] = 0;
	{
		v = strchr(n,' ');
		if (!v)
			return -1;
		while (*v == ' ') {
			v++;
		}
		n = v;
	}
	{
		v = strchr(n,' ');
		if (!v)
			return -1;
		*v = 0;
		setResponse(strtol(n,NULL,10));
	}

	while ((i = sock->ReceiveLine(lbuff,2048))) {
		n = lbuff;
		v = strchr(n,':');
		if (!v)
			return -1;
		*v = 0;
		v++;
		while (*v && *v == ' ') {
			v++;
		}
		if (!strcmp(n,"Content-Length")) {
			setLength(strtoul(v,NULL,10));
		}else{
			setHeader(n,v);
		}
	}

	return 0;
}

static char url_reserved[33] = {
	'!',
 	'*',
 	'\'',
 	'(',
 	')',
 	';',
 	':',
 	'@',
 	'&',
 	'=',
 	'+',
 	'$',
 	',',
 	'/',
 	'?',
 	'#',
 	'[',
 	']',
	'"',
 	'%',
 	'-',
 	'.',
 	'<',
 	'>',
 	'\\',
 	'^',
 	'_',
 	'`',
 	'{',
 	'|',
 	'}',
 	'~',
	0
};

static int32_t is_reserved(char c)
{
	int32_t i;
	if (c < 33)
		return 1;
	for (i=0; url_reserved[i]; i++) {
		if (c == url_reserved[i])
			return 1;
	}
	return 0;
}

/* % encodes a string for http urls */
std::string http_url_encode(std::string &str)
{
	int32_t i;
	char buff[10];
	char* in = (char*)str.c_str();
	std::string out("");
	for (i=0; in[i] != 0; i++) {
		if (is_reserved(in[i])) {
			sprintf(buff,"%.2X",in[i]);
			out += "%";
			out += buff;
			continue;
		}
		out += in[i];
	}
	return out;
}

/* decodes a % encoded string from http urls */
std::string http_url_decode(std::string &str)
{
	int32_t i;
	int32_t k;
	char buff[10];
	char* in = (char*)str.c_str();
	std::string out("");
	for (i=0; in[i] != 0; i++) {
		if (in[i] == '%') {
			i++;
			if (in[i] == 0 || in[i+1] == 0)
				return 0;
			buff[0] = in[i++];
			buff[1] = in[i];
			buff[2] = 0;
			k = strtol(buff,NULL,16);
			if (!k)
				break;
			buff[0] = k;
			buff[1] = 0;
			out += buff;
			continue;
		}
		out += in[i];
	}
	return out;
}
