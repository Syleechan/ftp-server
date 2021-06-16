#include "factory.h"

void creat_salt(char* salt,int num)
{
	int i,len=0;
	char* str="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,./;'<>?";
	srand(time(NULL));//系统时间来初始化随机数发生器
	len=strlen(str);
	for(i=0;i<num;i++)
	{
		salt[i]=str[(rand()%len)];
	}
}//随机生成salt值8

int new_account(int new_fd,char* account)
{
	char salt[15]={0};
	char code[512]={0};
	char salt_c[15]={0};
	int ret;
	char flag=0;
	recv(new_fd,account,sizeof(account),0);//接收用户名
	ret=Isuser_mysql(new_fd,account);//查询用户是否存在
	if(ret==1)//用户存在
	{
		printf("用户存在\n!");
		char flag='r';
		send(new_fd,&flag,1,0);
		return 0;
	}else if(ret==-1){//不存在注册
		printf("可注册！\n");
		char flag='o';
		send(new_fd,&flag,1,0);
	}
	creat_salt(salt,8);//系统随机生成salt值
	//puts(salt);
	sprintf(salt_c,"$6$%s",salt);
	send(new_fd,salt_c,strlen(salt_c),0);//发送盐值
	recv(new_fd,code,sizeof(code),0);//收密文（经crypt加密处理）
	ret=insert_mysql(account,salt_c,code);//将账户，加密的密文插入数据库
	if(ret!=1)
	{
		flag='x';
		send(new_fd,&flag,1,0);
		return -1;//插入失败
	}else{
		flag='o';
		send(new_fd,&flag,1,0);
		return 1;//插入成功
	}
}

int login_account(int new_fd,char* account)//登陆账户
{
	char salt[15]={0};
	char code[200]={0};
	char passwd[15]={0};
	int ret;
	char flag=0;
	recv(new_fd,account,sizeof(account),0);//先接收帐户名
	ret=Isuser_mysql(new_fd,account);//查询帐户名是否存在
	if(ret==-1)//空表或账户不存在
	{
		flag='n';
		send(new_fd,&flag,1,0);//用户不存在
		return -1;
	}else{
		flag='o';
		send(new_fd,&flag,1,0);//存在就输入密码不返回值
	}
	ret=query_mysql(new_fd,account);
	if(ret==0)//密码不正确
	{
		flag='x';
		send(new_fd,&flag,1,0);
		return 0;
	}
	if(ret==1)//密码正确
	{
		flag='o';
		send(new_fd,&flag,1,0);
		return 1;
	}//只有0，1的时候还需要接收和发送
}


