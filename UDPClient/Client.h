#pragma once
#include "Socket.h"
#include <string>
#include "string.h"
#include <vector>

class Client : public Socket
{
private:
	std::string m_addr;
	std::string m_port;
	std::string m_filename;

public:
	Client(const char* addr, const char* port, const char* filename);

	void SendFileLength(std::vector<char>& buffer);
	void SendFileName();
	void SendDataFromFile(std::vector<char>& buffer);
	void SendParts(int sock, const char* buff, long int len, long int chunk_size);

	std::string GetFileName();
	void SendFileToServer();
	void ReadFile(std::string& fileName, std::vector<char>& buf);
};

