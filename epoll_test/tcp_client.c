#include "func.h"

int main(int argc,char** argv)
{
	if(argc!=3)
	{
		printf("./client lack of args\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in clnt;
	bzero(&clnt,sizeof(clnt));
	clnt.sin_family=AF_INET;
	clnt.sin_port=htons(atoi(argv[2]));
	clnt.sin_addr.s_addr=inet_addr(argv[1]);
	int ret=connect(sfd,(struct sockaddr*)&clnt,sizeof(clnt));
	if(ret==-1)
	{
		perror("connect");
		return -1;
	}
	fd_set rdset;
	char buf[128]={0};
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(sfd,&rdset);
		ret=select(sfd+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(sfd,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=recv(sfd,buf,sizeof(buf),0);
				if(ret==-1)
				{
					perror("recv");
					return -1;
				}else if(ret==0)
				{
					printf("chat over\n");
					break;
				}
				printf("I am client,buf=%s\n",buf);
			}
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=read(0,buf,sizeof(buf));
				if(ret<=0)
				{
					printf("ret=%d\n",ret);
					break;
				}
				send(sfd,buf,strlen(buf)-1,0);
			}
		}
	}
	close(sfd);
	return 0;
}
