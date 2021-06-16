#include"func.h"

int send_n(int sfd,char* buf,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=send(sfd,buf+total,len-total,0);//buf偏移，len减少
		if(ret==-1)
		{
			printf("server close s\n");
			return -1;
		}
		total=total+ret;
	}
	return 0;
}

int recv_n(int sfd,char* buf,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=recv(sfd,buf-total,len-total,0);
		if(ret<=0)
		{
			printf("server close r\n");
			return -1;
		}
		total=total+ret;
	}
	return 0;
}
