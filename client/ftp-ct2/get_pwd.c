#include "func.h"

void get_pwd(int sfd)
{
	char pwdpath[100]={0};
	recv(sfd,pwdpath,sizeof(pwdpath),0);
	printf("current directory is:%s\n",pwdpath);
}
