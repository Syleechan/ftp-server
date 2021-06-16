#include "factory.h"
//cd ..
//cd 相对路径名
void send_wrg(int new_fd)
{
	char wrg[100]="please enter the right directory";
	send(new_fd,wrg,sizeof(wrg),0);
	return;
}

void send_cd(int new_fd,char* path)//path为当前目录路径
{
	DIR* dir;
	dir=opendir(path);
	struct dirent* p;
	char msg[100]={0};//接收client的cd命令
	int i=0,count=0;
	while(path[i]!='\0')
	{
		if(path[i]=='/')
			count++;
		i++;
	}
	printf("%s\n",path);
	recv(new_fd,msg,sizeof(msg),0);//接收要切换的目录信息
	char log[100]={0};
	sprintf(log,"%s %s","cd",msg);
	writeFile(log);//记录cd
	while(count>2)//保证在home目录下
	{
		if(!strcmp(msg,".."))//返回上一级目录
		{
			while(path[i]!='/')
				i--;
			path[i]='\0';
			count--;//逆序遍历遇到第一个‘/’结束
			//printf("%s\n",path);
			bzero(&msg,sizeof(msg));
			strcpy(msg,path);
			send(new_fd,msg,strlen(msg),0);//发送当前目录路径
			goto end;
		}else{
			while((p=readdir(dir))!=NULL)
			{
				/*if(!strcmp(msg,"..")||!strcmp(msg,"."))
				{
					bzero(&msg,sizeof(msg));
					strcpy(msg,"access denied");
					send(new_fd,msg,strlen(msg),0);
					break;
				}else*/ if(!strcmp(p->d_name,msg)&&p->d_type==4)//信息匹配且时目录类型
				{
					sprintf(path,"%s%s%s",path,"/",msg);
					//printf("%s\n",path);
					count++;
					bzero(&msg,sizeof(msg));
					strcpy(msg,path);
					send(new_fd,msg,strlen(msg),0);
					goto end;
				}
			}
			send_wrg(new_fd);//切换的目录错误
			goto end;
		}
	}
	bzero(&msg,sizeof(msg));
	strcpy(msg,"access denied");
	send(new_fd,msg,strlen(msg),0);//到了home目录没有访问权限
	return;
end:
	return;
}


