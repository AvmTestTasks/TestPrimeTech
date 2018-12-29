#include "Client.h"
#include <fstream>
#include <libgen.h>

Client::Client(const char* addr, const char* port, const char* filename) :
	m_addr(addr), m_port(port), m_filename(filename)
{
	bzero(&m_socketaddr, sizeof(m_socketaddr));
	m_socketaddr.sin_family = AF_INET;
	m_socketaddr.sin_port = htons((unsigned short)std::atoi(m_port.c_str()));

	if (inet_pton(AF_INET, m_addr.c_str(), &m_socketaddr.sin_addr) < 0)
	{
		printf("Client Initialization: Error converting address!\n");
		exit(EXIT_FAILURE);
	}
}

void Client::SendFileToServer()
{
	std::vector<char> buffer;

	ReadFile(m_filename, buffer);

	SendFileLength(buffer);

	SendFileName();

	if (!buffer.empty())
		SendDataFromFile(buffer);
}

void Client::ReadFile(std::string& fileName, std::vector<char>& buff)
{
	size_t length = 0;
	std::ifstream ifs;
	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		ifs.open(fileName, std::ifstream::binary);
		ifs.seekg(0, ifs.end);
		length = ifs.tellg();
		if (length != 0)
		{
			buff.resize(length);
			ifs.seekg(0, ifs.beg);
			ifs.read(buff.data(), length);
		}
		ifs.close();
	}
	catch (std::ifstream::failure e)
	{
		printf("Client Reading File: File opening/reading/closing failed!\n");
		exit(EXIT_FAILURE);
	}
}

void Client::SendParts(int sock, const char* buff, long int len, long int chunk_size)
{
	try
	{
		while (len > 0)
		{
			ssize_t sendOK = sendto(sock, buff, chunk_size, 0, (sockaddr*)&m_socketaddr, sizeof(m_socketaddr));

			if (sendOK < 0)
			{
				throw 1;
			}

			buff += sendOK;
			len -= sendOK;
		}
	}
	catch(...)
	{
		printf("Client Sending: Error sending data!\n");
		exit(EXIT_FAILURE);
	}
}

std::string Client::GetFileName()
{
	std::string filename = basename(&m_filename[0]);
	return filename;
}

void Client::SendFileLength(std::vector<char>& buffer)
{
	size_t converted_length = htonl((unsigned int)buffer.size());
	SendParts(m_socketFD, (char *)&converted_length, sizeof(converted_length), 10);
	printf("Client SendFileLength : %u\n", buffer.size());
}

void Client::SendFileName()
{
	std::string filename = GetFileName();
	SendParts(m_socketFD, filename.c_str(),filename.size(), filename.size());
	printf("Client SendFileName : %s\n", &filename[0]);

}

void Client::SendDataFromFile(std::vector<char>& buffer)
{
	SendParts(m_socketFD, buffer.data(), (long int)buffer.size(), (long int)buffer.size());
}