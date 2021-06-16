#include "factory.h"
void delete(char* filename,int new_fd)
{
	char msg[100]={0};
	if(unlink(filename)<0)//文件的链接数小于0说明没有该文件
	{
		strcpy(msg,"Delete file failed,please check the file name");
		send(new_fd,msg,strlen(msg),0);
		bzero(&msg,sizeof(msg));
		sprintf(msg,"Delete file:%s failed!",filename);
		writeFile(msg);//记录日志
	}//删除失败
	else{
		strcpy(msg,"The file is deleted");
		send(new_fd,msg,strlen(msg),0);
		bzero(&msg,sizeof(msg));
		sprintf(msg,"Delete file:%s success!",filename);
		writeFile(msg);
	}//删除成功
}
 
void send_rm(int new_fd,char* path)
{
	DIR* dir;
	struct dirent* p;
	char rmpath[100]={0};
	strcpy(rmpath,path);
	chdir(rmpath);
	dir=opendir(rmpath);
	char filename[100]={0};
	recv(new_fd,filename,sizeof(filename),0);//接收要删除的文件名
	while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,filename)&&p->d_type==8)//如果是要删除的文件
		{
			delete(filename,new_fd);//删除new_fd客户端的文件
			break;
		}
	}
	if(p==NULL)//如果目录为空
	{
		delete(filename,new_fd);
	}//说明没有相应文件，发送失败信息
	chdir(path);
}

