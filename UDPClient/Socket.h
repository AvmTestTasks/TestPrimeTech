#pragma once
#include <cstdio>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class Socket
{
public:
	Socket();
	~Socket();

	int m_socketFD;
	sockaddr_in m_socketaddr;
};

