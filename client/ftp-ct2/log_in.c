#include "func.h"

int login_client(int sfd)
{
	char account[15]={0};
	char salt[15]={0};
	char buf[200]={0};
	char* code;
	char* password;
	char flag=0;
	int i;
	int cnt=3;
lable:
	printf("请输入选项：0.退出，1.登陆，2.注册\n");
	read(0,buf,sizeof(buf));//输入相关选项
	i=atoi(buf);
//	printf("%d\n",i);
exit:
	send(sfd,(char*)&i,sizeof(i),0);
	if(i==0)//退出命令
	{
		printf("即将退出...\n");
		sleep(2);
		return -1;
	}
	else if(i==1)//登陆命令
	{
		printf("请输入用户名：");
		bzero(&buf,sizeof(buf));
		bzero(&account,sizeof(buf));
		fgets(account,15,stdin);//输入用户名
		send(sfd,account,strlen(account)-1,0);//发送用户名
		recv(sfd,&flag,1,0);
		if(flag=='n')//用户名不存在
		{
			printf("用户名不存在，请注册！(1.返回上级菜单，0.退出)\n");
reoption:
			bzero(&buf,sizeof(buf));
			read(0,buf,sizeof(buf));
			i=atoi(buf);
			if(i==1)//注册账户
			{
				goto lable;
			}else if(i==0)//退出
			{
				goto exit;
			}else{
				printf("命令无效！\n");
				goto reoption;
			}

		}//否则输入密码
login:	
		bzero(&password,sizeof(password));
		password=getpass("请输入登陆密码：");
		recv(sfd,salt,sizeof(salt),0);
		//puts(salt);//测试
		code=crypt(password,salt);
		//puts(code);//测试
		send(sfd,code,strlen(code),0);//收salt值发明文
		recv(sfd,&flag,1,0);
		if(flag=='o')
		{
			printf("登陆成功\n");
		}else{
			printf("登陆失败，密码不正确\n");
			if(--cnt)//三次失败的机会
				goto lable;
			else 
				return -1;
		}
	}
	else if(i==2)
	{
redit:
		bzero(&account,sizeof(account));
		printf("请输入要注册的用户名：");
		fgets(account,15,stdin);//注册的帐户名
		send(sfd,account,strlen(account)-1,0);//发送帐户名
		recv(sfd,&flag,1,0);
		if(flag=='r')
		{
			printf("用户名已存在！（2，注册，1.登陆，0.退出)\n");
			bzero(&buf,sizeof(buf));
			read(0,buf,sizeof(buf));
			i=atoi(buf);
			if(i==2)
			{
				goto redit;//注册
			}else if(i==0)
			{
				goto exit;//退出
			}else if(i==1)
			{
				goto lable;//登陆
			}
			else{
				printf("命令无效！\n");
				goto lable;
			}
		}
		bzero(&password,sizeof(password));
		password=getpass("请输入注册密码：");
		bzero(&salt,sizeof(salt));
		bzero(&code,sizeof(code));
		recv(sfd,salt,sizeof(salt),0);//接收salt值
		//puts(salt);//测试
		code=crypt(password,salt);//将密码和salt值进行加密
		//puts(code);//测试
		send(sfd,code,strlen(code),0);//加密的密码发给服务器
		recv(sfd,&flag,1,0);
		if(flag=='o')
		{
			printf("注册成功！\n");//注册成功
			goto lable;
		}else{
			goto lable;//注册失败
		}
	}else
	{
		printf("命令无效！\n");
		goto lable;
	}
}








