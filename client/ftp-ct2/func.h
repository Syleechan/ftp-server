#ifndef __FUNC_H__
#define __FUNC_H__
#include "md5.h"

typedef struct{
	int len;
	char buf[1024];
}train;//小火车发送 4+sizeof(buf)字节数据

void get_file(int,char*);//客户端上传文件
void get_ls(int);//查看目录文件列表
void get_cd(int,char*);//进入对应目录 cd xx
void get_pwd(int);//获取当前目录
void rm_file(int,char*);//删除文件

int send_n(int,char*,int);//发送小火车数据
int recv_n(int,char*,int);//接收小火车数据
void tran_file(int,char*);//向客户端传送文件

int login_client(int sfd);//登陆客户端


#endif
