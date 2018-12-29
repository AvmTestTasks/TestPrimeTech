#include "Server.h"
#include <fstream>
#include <sstream>

Server::Server(const char* addr, const char* port, const char* dirpath) :
	m_addr(addr), m_port(port), m_dirpath(dirpath)
{
	bzero(&m_socketaddr, sizeof(m_socketaddr));
	m_socketaddr.sin_family = AF_INET;
	m_socketaddr.sin_port = htons((unsigned short)std::atoi(m_port.c_str()));
	
	if (inet_pton(AF_INET, m_addr.c_str(), &m_socketaddr.sin_addr) < 0)
	{
		printf("Server Initialization: Error converting address!\n");
		exit(EXIT_FAILURE);
	}
}

void Server::Bind()
{
	if ( (bind(m_socketFD, (const struct sockaddr *) &m_socketaddr, sizeof(m_socketaddr))) == -1)
	{
		printf("Can not bind socket!\n");
		exit(EXIT_FAILURE);
	}
}

void Server::GetFullFilePath(std::string& fileName)
{
	if (m_dirpath.back() != '/')
		m_dirpath += '/';

	fileName = m_dirpath + fileName;
}

void Server::CreateEmptyFile(std::string& filename)
{
	std::ofstream ofs;
	ofs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		ofs.open(filename);
		ofs.clear();
		ofs.close();
	}
	catch(std::ofstream::failure e)
	{
		printf("Server CreateEmptyFile: File creation failed!\n");
		exit(EXIT_FAILURE);
	}
}

void Server::WriteDataToFile(std::string& filename, std::vector<char>& data)
{
	std::ofstream ofs;
	ofs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		ofs.open(filename, std::ios_base::app | std::ios_base::binary);
		if (ofs)
		{
			ofs << data.data();
			ofs.close();
		}
	}
	catch(std::ofstream::failure e)
	{
		printf("Server WriteDataToFile: Fail write received data to file!\n");
		exit(EXIT_FAILURE);
	}
}

void Server::ReceiveFileLength(size_t& fileLength)
{
	socklen_t slen = sizeof(m_socketaddr);
	ssize_t ind = recvfrom(m_socketFD, &fileLength, sizeof(size_t),
		0, (sockaddr*)&m_socketaddr, &slen);

	if (ind < 0)
	{
		printf("Receiving failed!\n");
		exit(EXIT_FAILURE);
	}
	fileLength = ntohl((unsigned int)fileLength);
	printf("Server ReceiveFileLenght : %u\n", fileLength);
}

void Server::ReceiveFileName(std::string& fileName)
{
	socklen_t slen = sizeof(m_socketaddr);
	std::vector<char> buff(255);

	ssize_t ind = recvfrom(m_socketFD, &buff[0], buff.size(),
		0, (sockaddr*)&m_socketaddr, &slen);
	buff.resize(ind);

	if (ind < 0)
	{
		printf("Receiving failed!\n");
		exit(EXIT_FAILURE);
	}
	std::string ss(buff.begin(), buff.end());
	fileName = ss;

	printf("Server ReceiveFileName : %s\n", &fileName[0]);
}

void Server::ReceiveDataFromFile(std::vector<char>& buffer)
{
	socklen_t slen = sizeof(m_socketaddr);

	ssize_t ind = recvfrom(m_socketFD, &buffer[0], buffer.size(),
		0, (sockaddr*)&m_socketaddr, &slen);
	buffer.resize(ind);

	if (ind < 0)
	{
		printf("Receiving failed!\n");
		exit(EXIT_FAILURE);
	}

	printf("Server ReceiveDataFromFile: Succed!\n");
}

void Server::ReceiveFileFromClient()
{
	size_t fileLength;
	ReceiveFileLength(fileLength);

	std::string fileName;
	ReceiveFileName(fileName);
	
	std::vector<char> buffer(fileLength);
	ReceiveDataFromFile(buffer);
	GetFullFilePath(fileName);
	CreateEmptyFile(fileName);

	if (!buffer.empty())
	{
		WriteDataToFile(fileName, buffer);
	}
	
}