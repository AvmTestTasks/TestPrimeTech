#include "Socket.h"

Socket::Socket()
{
	//Create UDP socket
	if ((m_socketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		printf("Can not creat socket!"); /* Print error message */
		exit(EXIT_FAILURE);
	}
}


Socket::~Socket()
{
	close(m_socketFD);
}
