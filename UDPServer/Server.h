#pragma once
#include "Socket.h"
#include <string>
#include "string.h"
#include <vector>
#include <arpa/inet.h>

class Server : public Socket
{
private:
	std::string m_addr;
	std::string m_port;
	std::string m_dirpath;

public:
	Server(const char* addr, const char* port , const char* dirpath);

	void Bind();

	void ReceiveFileLength(size_t& buffer);
	void ReceiveFileName(std::string& fileName);
	void ReceiveDataFromFile(std::vector<char>& buffer);
	void ReceiveFileFromClient();

private:
	void GetFullFilePath(std::string& fileName);
	void CreateEmptyFile(std::string& filename);
	void WriteDataToFile(std::string& filename, std::vector<char>& data);
};

