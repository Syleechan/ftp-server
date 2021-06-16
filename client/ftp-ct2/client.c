#include "func.h"

int main(int argc,char** argv)
{
	if(argc!=3)
	{
		printf("./client lack of args\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);//使用TCP协议-客户端描述符，设置套接口选项
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in clnt;
	bzero(&clnt,sizeof(clnt));
	clnt.sin_family=AF_INET;//TCP
	clnt.sin_port=htons(atoi(argv[2]));//端口
	clnt.sin_addr.s_addr=inet_addr(argv[1]);//地址
	int ret=connect(sfd,(struct sockaddr*)&clnt,sizeof(clnt));//请求与服务器连接，将sfd的socket连至参数serv_addr指定的服务器IP和端口号上去
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	char *str[10]={"cd","ls","puts","gets","remove","pwd"};
	char buf[100];
	ret=login_client(sfd);//登陆账户
	if(ret==-1)
	{
		return -1;
	}
	//登陆连接接口
	while(1)
	{
lable:
		memset(&buf,0,sizeof(buf));
		printf("enter command:\n");
		fgets(buf,100,stdin);//输入命令
		int len=strlen(buf);
		//printf("%d\n",len);
		buf[len-1]='\0';
		char* part_1=NULL;
		char* part_2=NULL;//命令分割
		part_1=strtok(buf," ");
		part_2=strtok(NULL," ");
		//printf("%s\n",part_1);
		int i,command=0;
		for(i=0;i<6;i++)
		{
			if(!strcmp(part_1,*(str+command)))
				break;
			else
				command++;
		}
		if(part_2==NULL)
		{
			if(command==0||command==2||command==3)//cd,puts,gets命令后不能为空
			{
				printf("please enter the right command\n");
				goto lable;
			}
		}
		if(command==6)
		{
			goto lable;
		}
		send(sfd,&command,4,0);//先把case传递给服务器端
	  	//printf("%d\n",command);//command=6要跳转
		switch(command)
		{
			case 0:get_cd(sfd,part_2);break;//cd命令
			case 1:get_ls(sfd);break;//ls命令
			case 2:tran_file(sfd,part_2);break;//puts命令
			case 3:get_file(sfd,part_2);break;//gets命令
			case 4:rm_file(sfd,part_2);break;//remove命令
			case 5:get_pwd(sfd);break;//pwd命令
			default:break;
		}
	}
}
