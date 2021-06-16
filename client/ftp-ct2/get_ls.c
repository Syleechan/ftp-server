#include "func.h"

struct file_msg{
	long size;//8字节-文件大小
	char type;//1字节-文件类型-目录或文件
	char file_name[100];//文件目录名
};

void get_ls(int sfd)
{
	train t;
	struct file_msg* msg;
	do{
		bzero(&msg,sizeof(struct file_msg));
		bzero(&t,sizeof(t));
		recv(sfd,(char*)&t.len,sizeof(t.len),0);//需要接收文件信息的长度-火车len
		if(t.len==0)
			break;//若为0直接退出循环
		recv_n(sfd,t.buf,9+t.len);//接收火车buf内容
		msg=(struct file_msg*)t.buf;//强制转换类型
		printf("%c %-20s \t\t\t %ldB\n",msg->type,msg->file_name,msg->size);

	}while(1);
}

