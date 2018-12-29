#include "Server.h"

int main(int argc, const char* argv[])
{
		if (argc != 4)
		{
			printf("Not enought arguments!\nExample:127.0.0.1 8080 /home/test/somefolder\n");
			return -1;
		}
	
		Server srv(argv[1], argv[2], argv[3]);
		
		srv.Bind();
		srv.ReceiveFileFromClient();

		printf("Accepting file succed!\n");
		return 0;
}