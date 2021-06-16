#include "func.h"

void get_cd(int sfd,char* cmd)
{
	if(cmd==NULL)
	{
		printf("please enter the right command\n");
		return;
	}
	char path[100]={0};
	send(sfd,cmd,strlen(cmd),0);//发送切换对应目录的信息
	recv(sfd,path,sizeof(path),0);//接收对应目录
	//printf("%s:\n",path);
}
