#include "Client.h"

int main(int argc, const char* argv[])
{
	if (argc != 4)
	{
		printf("Not enought arguments!\nExample:127.0.0.1 8080 /home/test/somefolder/filename.txt\n");
		return -1;
	}
		
	Client cln(argv[1], argv[2], argv[3]);

	cln.SendFileToServer();

	printf("Sending file succed!\n");
	return 0;
}