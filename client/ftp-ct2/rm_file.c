#include"func.h"

void rm_file(int sfd,char* filename)
{
	char msg[100]={0};
	send(sfd,filename,strlen(filename),0);//����Ҫɾ�����ļ���
	recv(sfd,msg,sizeof(msg),0);
	printf("%s\n",msg);
}
