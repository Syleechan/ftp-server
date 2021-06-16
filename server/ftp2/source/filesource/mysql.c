#include "factory.h"
#define mysql_Server   "localhost"
#define mysql_User     "root"
#define mysql_Passwd   "woaini99"
#define mysql_Database "account"
int Isuser_mysql(int new_fd,char* account)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[300]={0};
	char salt[15]={0};//
	char name[20]={0};
	char code[512]={0};
	char buf[128]={0};
	char flag=0;
	conn=mysql_init(NULL);
	bzero(&query,sizeof(query));
	strcpy(query,"select * from MSG where Username='");
	sprintf(query,"%s%s%s",query,account,"'");
	puts(query);//调试
	if(!mysql_real_connect(conn,mysql_Server,mysql_User,mysql_Passwd,mysql_Database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -2;
	}else
	{
		printf("Connected..\n");
	}
	int ret;
	ret=mysql_query(conn,query);
	if(ret)//空表返回1
	{
		printf("Error making query:%s\n",mysql_error(conn));
		mysql_close(conn);
		//printf("123\n");//测试
		return -1;
	}
	res=mysql_use_result(conn);
	if(res)
	{
		while((row=mysql_fetch_row(res))!=NULL)
		{
			if(!strcmp(row[1],account))
			{
				mysql_free_result(res);
				mysql_close(conn);
				return 1;
			}
		}
		mysql_free_result(res);
		mysql_close(conn);
		return -1;
	}else{
		printf("mysql_use_result:%s",mysql_error(conn));
		mysql_close(conn);
		//printf("456\n");//测试
		return -1;
	}
}




//匹配后成功返回1，连接不成功,用户不存在返回-1，密码不正确返回0
int query_mysql(int new_fd,char* account)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[300]={0};
	char salt[15]={0};//
	char name[20]={0};
	char code[512]={0};
	char buf[128]={0};
	char flag=0;
	conn=mysql_init(NULL);
	sprintf(query,"select * from MSG where Username='%s'",account);
	puts(query);//调试
	if(!mysql_real_connect(conn,mysql_Server,mysql_User,mysql_Passwd,mysql_Database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -2;
	}else
	{
		printf("Connected..\n");
	}
	int ret;
	ret=mysql_query(conn,query);
	if(ret)//在查询用户后不会发生
	{
		printf("Error making query:%s\n",mysql_error(conn));
		mysql_close(conn);
		return -1;
	}
	res=mysql_use_result(conn);
	if(res)
	{
		while((row=mysql_fetch_row(res))!=NULL)
		{
			//printf("%s %s\n",row[1],row[2]);
			if(!strcmp(row[1],account))
			{
				strcpy(salt,row[2]);
				strcpy(name,row[1]);
				break;
			}
		}
		if(!strcmp(name,account))//说明database里有账户将salt发给客户端
		{
			//printf("%s %s\n",name,account);
			send(new_fd,salt,strlen(salt),0);
			recv(new_fd,code,sizeof(code),0);//发salt收code进行匹配
			//printf("%s\n",code);
			//printf("%s\n",row[3]);
			if(!strcmp(code,row[3]))
			{
				printf("Match success!\n");//通知登陆成功，返回1
				mysql_free_result(res);
				mysql_close(conn);
				return 1;
			}else//密码不正确
			{   
				printf("Match failed!\n");//登陆失败，返回0
				mysql_free_result(res);
				mysql_close(conn);
				return 0;
			}
		}else//用户不存在返回-1
		{
			printf("mysql_use_result:%s",mysql_error(conn));
			mysql_close(conn);
			return -1;
		}
	}
}


//插入成功返回1，失败返回-1
int insert_mysql(char* account,char* salt_c,char* code)//新增用户信息
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[300]="insert into MSG(Username,salt,code) values('','','')";
	sprintf(query,"insert into MSG(Username,salt,code) values('%s','%s','%s')",account,salt_c,code);
	puts(query);//调试
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,mysql_Server,mysql_User,mysql_Passwd,mysql_Database,0,NULL,0))
	{	
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected..\n");
	}
	int ret;
	ret=mysql_query(conn,query);
	if(ret)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("insert success\n");
		mysql_close(conn);
		return 1;
	}
}






