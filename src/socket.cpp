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

#include "socket.h"
#include "debug.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include "utility.h"

// Debug printing options
// Set to 1 for debug output
#define DP 0
// This is prepended to everything printed here
#define DPS ""

bool g_sockets_initialized = false;

void sockets_init()
{
#ifdef _WIN32
	WSADATA WsaData;
	if(WSAStartup( MAKEWORD(2,2), &WsaData ) != NO_ERROR)
		throw SocketException("WSAStartup failed");
#else
#endif
	g_sockets_initialized = true;
}

void sockets_cleanup()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

Address::Address()
{
	m_address = 0;
	m_port = 0;
}

Address::Address(unsigned int address, unsigned short port)
{
	m_address = address;
	m_port = port;
}

Address::Address(unsigned int a, unsigned int b,
		unsigned int c, unsigned int d,
		unsigned short port)
{
	m_address = (a<<24) | (b<<16) | ( c<<8) | d;
	m_port = port;
}

bool Address::operator==(Address &address)
{
	return (m_address == address.m_address
			&& m_port == address.m_port);
}

bool Address::operator!=(Address &address)
{
	return !(*this == address);
}

void Address::Resolve(const char *name)
{
	struct addrinfo *resolved;
	int e = getaddrinfo(name, NULL, NULL, &resolved);
	if(e != 0)
		throw ResolveError("");
	/*
		FIXME: This is an ugly hack; change the whole class
		to store the address as sockaddr
	*/
	struct sockaddr_in *t = (struct sockaddr_in*)resolved->ai_addr;
	m_address = ntohl(t->sin_addr.s_addr);
	freeaddrinfo(resolved);
}

std::string Address::serializeString() const
{
	unsigned int a, b, c, d;
	a = (m_address & 0xFF000000)>>24;
	b = (m_address & 0x00FF0000)>>16;
	c = (m_address & 0x0000FF00)>>8;
	d = (m_address & 0x000000FF);
	return itos(a)+"."+itos(b)+"."+itos(c)+"."+itos(d);
}

unsigned int Address::getAddress() const
{
	return m_address;
}

unsigned short Address::getPort() const
{
	return m_port;
}

void Address::setAddress(unsigned int address)
{
	m_address = address;
}

void Address::setAddress(unsigned int a, unsigned int b,
		unsigned int c, unsigned int d)
{
	m_address = (a<<24) | (b<<16) | ( c<<8) | d;
}

void Address::setPort(unsigned short port)
{
	m_port = port;
}

void Address::print(std::ostream *s) const
{
	(*s)<<((m_address>>24)&0xff)<<"."
			<<((m_address>>16)&0xff)<<"."
			<<((m_address>>8)&0xff)<<"."
			<<((m_address>>0)&0xff)<<":"
			<<m_port;
}

void Address::print() const
{
	print(&dstream);
}

UDPSocket::UDPSocket()
{
	if(g_sockets_initialized == false)
		throw SocketException("Sockets not initialized");

	m_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(DP)
		dstream<<DPS<<"UDPSocket("<<(int)m_handle<<")::UDPSocket()"<<std::endl;

	if(m_handle <= 0)
	{
		throw SocketException("Failed to create socket");
	}

	setTimeoutMs(0);
}

UDPSocket::~UDPSocket()
{
	if(DP)
	dstream<<DPS<<"UDPSocket("<<(int)m_handle<<")::~UDPSocket()"<<std::endl;

#ifdef _WIN32
	closesocket(m_handle);
#else
	close(m_handle);
#endif
}

void UDPSocket::Bind(unsigned short port)
{
	if(DP)
	dstream<<DPS<<"UDPSocket("<<(int)m_handle
			<<")::Bind(): port="<<port<<std::endl;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if(bind(m_handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
	{
#ifndef DISABLE_ERRNO
		dstream<<(int)m_handle<<": Bind failed: "<<strerror(errno)<<std::endl;
#endif
		throw SocketException("Failed to bind socket");
	}
}

void UDPSocket::Send(const Address & destination, const void * data, int size)
{
	bool dumping_packet = false;
	if(INTERNET_SIMULATOR)
		dumping_packet = (myrand()%10==0); //easy
		//dumping_packet = (myrand()%4==0); // hard

	if(DP){
		/*dstream<<DPS<<"UDPSocket("<<(int)m_handle
				<<")::Send(): destination=";*/
		dstream<<DPS;
		dstream<<(int)m_handle<<" -> ";
		destination.print();
		dstream<<", size="<<size<<", data=";
		for(int i=0; i<size && i<20; i++){
			if(i%2==0) DEBUGPRINT(" ");
			DEBUGPRINT("%.2X", ((int)((const char*)data)[i])&0xff);
		}
		if(size>20)
			dstream<<"...";
		if(dumping_packet)
			dstream<<" (DUMPED BY INTERNET_SIMULATOR)";
		dstream<<std::endl;
	}
	else if(dumping_packet)
	{
		// Lol let's forget it
		dstream<<"UDPSocket::Send(): "
				"INTERNET_SIMULATOR: dumping packet."
				<<std::endl;
	}

	if(dumping_packet)
		return;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination.getAddress());
	address.sin_port = htons(destination.getPort());

	int sent = sendto(m_handle, (const char*)data, size,
		0, (sockaddr*)&address, sizeof(sockaddr_in));

	if(sent != size)
	{
		throw SendFailedException("Failed to send packet");
	}
}

int UDPSocket::Receive(Address & sender, void * data, int size)
{
	if(WaitData(m_timeout_ms) == false)
	{
		return -1;
	}

	sockaddr_in address;
	socklen_t address_len = sizeof(address);

	int received = recvfrom(m_handle, (char*)data,
			size, 0, (sockaddr*)&address, &address_len);

	if(received < 0)
		return -1;

	unsigned int address_ip = ntohl(address.sin_addr.s_addr);
	unsigned int address_port = ntohs(address.sin_port);

	sender = Address(address_ip, address_port);

	if(DP){
		//dstream<<DPS<<"UDPSocket("<<(int)m_handle<<")::Receive(): sender=";
		dstream<<DPS<<(int)m_handle<<" <- ";
		sender.print();
		//dstream<<", received="<<received<<std::endl;
		dstream<<", size="<<received<<", data=";
		for(int i=0; i<received && i<20; i++){
			if(i%2==0) DEBUGPRINT(" ");
			DEBUGPRINT("%.2X", ((int)((const char*)data)[i])&0xff);
		}
		if(received>20)
			dstream<<"...";
		dstream<<std::endl;
	}

	return received;
}

int UDPSocket::GetHandle()
{
	return m_handle;
}

void UDPSocket::setTimeoutMs(int timeout_ms)
{
	m_timeout_ms = timeout_ms;
}

bool UDPSocket::WaitData(int timeout_ms)
{
	fd_set readset;
	int result;

	// Initialize the set
	FD_ZERO(&readset);
	FD_SET(m_handle, &readset);

	// Initialize time out struct
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout_ms * 1000;
	// select()
	result = select(m_handle+1, &readset, NULL, NULL, &tv);

	if(result == 0){
		// Timeout
		/*dstream<<"Select timed out (timeout_ms="
				<<timeout_ms<<")"<<std::endl;*/
		return false;
	}
	else if(result < 0){
		// Error
#ifndef DISABLE_ERRNO
		dstream<<(int)m_handle<<": Select failed: "<<strerror(errno)<<std::endl;
#endif
#ifdef _WIN32
		int e = WSAGetLastError();
		dstream<<(int)m_handle<<": WSAGetLastError()="<<e<<std::endl;
		if(e == 10004 /*=WSAEINTR*/)
		{
			dstream<<"WARNING: Ignoring WSAEINTR."<<std::endl;
			return false;
		}
#endif
		throw SocketException("Select failed");
	}
	else if(FD_ISSET(m_handle, &readset) == false){
		// No data
		//dstream<<"Select reported no data in m_handle"<<std::endl;
		return false;
	}

	// There is data
	//dstream<<"Select reported data in m_handle"<<std::endl;
	return true;
}

/* TCPSocket */

TCPSocket::TCPSocket()
{
	if (g_sockets_initialized == false)
		throw SocketException("Sockets not initialized");

	m_handle = socket(AF_INET, SOCK_STREAM, 0);
	m_bstart = 0;
	m_bend = 0;

	if (DP)
		dstream<<DPS<<"TCPSocket("<<(int)m_handle<<")::TCPSocket()"<<std::endl;

	if (m_handle <= 0) {
		throw SocketException("Failed to create socket");
	}

#ifdef _WIN32
	char a=1;
	setsockopt(m_handle, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int));
#else
	int a=1;
	setsockopt(m_handle, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int));
#if defined(__FreeBSD__)
	setsockopt(m_handle, SOL_SOCKET, SO_NOSIGPIPE, &a, sizeof(int));
#endif
#endif
	setTimeoutMs(0);
}

TCPSocket::~TCPSocket()
{
	if (DP)
		dstream<<DPS<<"TCPSocket("<<(int)m_handle<<")::~TCPSocket()"<<std::endl;

#ifdef _WIN32
	closesocket(m_handle);
#else
	close(m_handle);
#endif
}

void TCPSocket::Bind(unsigned short port)
{
	if(DP)
		dstream<<DPS<<"TCPSocket("<<(int)m_handle<<")::Bind(): port="<<port<<std::endl;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(m_handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
#ifndef DISABLE_ERRNO
		dstream<<(int)m_handle<<": Bind failed: "<<strerror(errno)<<std::endl;
#endif
		throw SocketException("Failed to bind socket");
	}

	if (listen(m_handle, 5) < 0) {
#ifndef DISABLE_ERRNO
		dstream<<(int)m_handle<<": Listen failed: "<<strerror(errno)<<std::endl;
#endif
		throw SocketException("Failed to set socket to listening mode");
	}
}

bool TCPSocket::Connect(const Address &destination)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination.getAddress());
	address.sin_port = htons(destination.getPort());

	if (connect(m_handle, (sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
#ifndef DISABLE_ERRNO
		dstream<<(int)m_handle<<": Connect failed: "<<strerror(errno)<<std::endl;
#endif
		return false;
	}
	return true;
}

void TCPSocket::Send(const void *data, int size)
{
	int flags = 0;
#if defined(linux)
	flags = MSG_NOSIGNAL;
#endif
	int sent = send(m_handle, (const char*)data, size, flags);

	if (sent != size)
		throw SendFailedException("Failed to send data");
}

/* data read from remote http clients is buffered, fill the buffer */
int TCPSocket::FillBuffer()
{
	int r;
	int l = m_bend-m_bstart;
	if (!m_bstart && l == 2048)
		return l;

	if (l && m_bstart)
		memcpy(m_buff,m_buff+m_bstart,l);
	m_bstart = 0;
	m_bend = l;

	if (!WaitData(1000))
		return m_bend;


	r = (int)recv(m_handle,m_buff+l,2048-l,0);
	if (r<0)
		return m_bend;

	m_bend = l+r;

	return m_bend;
}

int TCPSocket::Receive(void *data, int size)
{
	if (size > 2048) {
		unsigned char* buff = (unsigned char*)data;
		int r = 0;
		int l;
		int c = 2048;
		int s = size;
		while (FillBuffer()) {
			c = 2048;
			if (s<c)
				c = s;
			l = m_bend-m_bstart;
			if (l<c)
				c = l;
			if (c < 1)
				break;
			memcpy(buff+r,m_buff+m_bstart,c);
			m_bstart += c;
			r += c;
			s -= c;
		}
		return r;
	}

	if (FillBuffer() < size)
		return 0;

	memcpy(data,m_buff+m_bstart,size);
	m_bstart+=size;
	return size;
}

int TCPSocket::ReceiveLine(char* data, int size)
{
	int i = 0;
	for (; i<size; i++) {
		if (m_bstart >= m_bend)
			FillBuffer();
		if (m_bstart >= m_bend)
			break;

		data[i] = m_buff[m_bstart++];
		if (data[i] == '\r') {
			i--;
		}else if (data[i] == '\n') {
			data[i] = 0;
			break;
		}else if (i > size-2) {
			data[i+1] = 0;
			break;
		}
	}
	return i;
}

TCPSocket* TCPSocket::Accept()
{
	sockaddr_in address;
	socklen_t address_len = sizeof(address);

	int client = accept(m_handle, (sockaddr*)&address, &address_len);
	if (client < 0) {
#ifndef DISABLE_ERRNO
		dstream<<(int)m_handle<<": Accept failed: "<<strerror(errno)<<std::endl;
#endif
		throw SocketException("Failed to accept socket");
	}

	TCPSocket *socket = new TCPSocket();
	socket->m_handle = client;

	return socket;
}

int TCPSocket::GetHandle()
{
	return m_handle;
}

void TCPSocket::setTimeoutMs(int timeout_ms)
{
	m_timeout_ms = timeout_ms;
}

bool TCPSocket::WaitData(int timeout_ms)
{
	fd_set readset;
	int result;

	// Initialize the set
	FD_ZERO(&readset);
	FD_SET(m_handle, &readset);

	// Initialize time out struct
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout_ms * 1000;
	// select()
	result = select(m_handle+1, &readset, NULL, NULL, &tv);

	if (result == 0) {
		return false;
	}else if (result < 0 && errno == EINTR) {
		return false;
	}else if (result < 0) {
		// Error
#ifndef DISABLE_ERRNO
		dstream<<(int)m_handle<<": Select failed: "<<strerror(errno)<<std::endl;
#endif
#ifdef _WIN32
		int e = WSAGetLastError();
		dstream<<(int)m_handle<<": WSAGetLastError()="<<e<<std::endl;
		if (e == 10004 /*=WSAEINTR*/) {
			dstream<<"WARNING: Ignoring WSAEINTR."<<std::endl;
			return false;
		}
#endif
		throw SocketException("Select failed");
	}else if (FD_ISSET(m_handle, &readset) == false) {
		// No data
		return false;
	}

	// There is data
	return true;
}
