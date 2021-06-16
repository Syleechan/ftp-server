#include "factory.h"

void send_pwd(int new_fd,char* path)
{
	send(new_fd,path,strlen(path),0);
	writeFile("client get current pwd");
}

