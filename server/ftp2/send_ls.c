#include "factory.h"

struct file_msg{
	long size;//8字节-文件大小
	char type;//1字节-目录或文件
	char file_name[100];//文件目录名
};

void send_ls(int new_fd,char* path)
{
	writeFile("ls -l");
	char chpath[100]={0};
	strcpy(chpath,path);
	chdir(chpath);//切换到当前目录
	train t;//火车 4+文件长度
	struct file_msg msg;
	bzero(&msg,sizeof(struct file_msg));
	DIR* dir;
	dir=opendir(chpath);
	struct stat buf;//文件属性结构体
	bzero(&buf,sizeof(struct stat));
	struct dirent* p;//目录属性结构体
	int len;
	while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,"..")||p->d_name[0]=='.'){//不是文件、目录名
		}else
		{
			stat(p->d_name,&buf);
			if(S_ISDIR(buf.st_mode))//如果时目录
			{
				msg.type='d';
			}else
			{
				msg.type='-';//文件
			}
			strcpy(msg.file_name,p->d_name);//文件名拷贝到msg结构体
			t.len=strlen(msg.file_name);//文件名的长度赋值
			msg.size=buf.st_size;//文件的大小赋值
			memcpy(t.buf,&msg,9+t.len);//8+1 文件大小和文件类型
			send_n(new_fd,(char*)&t,13+t.len);//8+1+strlen+sizeof(t.len)
		}
	}
	t.len=0;//目录已遍历完
	send_n(new_fd,(char*)&t,4);//发送t.len=0作为结束标志
	chdir(path);//切回源目录,多个客户端时改动
}
	



