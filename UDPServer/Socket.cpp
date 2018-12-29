#include "Socket.h"

Socket::Socket()
{  
	//Create UDP socket
	if ((m_socketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("Socket Initialization: Error creating socket\n");
		exit(EXIT_FAILURE);
	}
}


Socket::~Socket()
{
	close(m_socketFD);
}
