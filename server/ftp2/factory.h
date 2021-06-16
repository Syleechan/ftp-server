#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "work_que.h"
//函数指针
typedef void* (*pfunc)(void*);

//线程动作集合结构体
typedef struct{
	pthread_t* pthid;//线程ID
	pthread_cond_t cond;//条件变量唤醒线程
	que_t que;//队列
	int pthread_num;//线程数量
	pfunc thread_func;//函数指针（指向函数的指针）
	int start_flag;//启动标志
}fac,*pfac;

//控制发送数据结构体
typedef struct{
	int len;
	char buf[1024];
}train;

//初始化
void factory_init(fac*,pfunc,int);

//启动子线程开始工作
void factory_start(fac*);

//传递new_fd函数
int send_n(int,char*,int);
int recv_n(int,char*,int);

//处理客户端发送的操作信息
void tran_file(int,char*);//下载文件
void get_file(int,char*);//上传文件
void handle_cmd(int,char*);//处理指令
void send_ls(int,char*);//查看文件列表
void send_cd(int,char*);//切换目录
void send_pwd(int,char*);//查看当前目录路径
void send_rm(int,char*);//删除文件

int login_account(int,char*);//登陆账户
int new_account(int,char*);//注册账户
int insert_mysql(char*,char*,char*);//插入数据库
int query_mysql(int,char*);//查询数据库
int Isuser_mysql(int,char*);//判断用户是否存在

FILE* creat_log(char*);//建立日志记录
void writeFile(char*);//对应操作记录写入本地日志
int Is_file_md5(char*,char*,char*);//上传文件时检查文件是否存在
int md5_insert_mysql(char*,char*,char*,char*);//文件MD5码插入数据库
#endif
