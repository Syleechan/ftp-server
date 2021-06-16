#include "factory.h"
#define mysql_Server   "localhost"
#define mysql_User     "root"
#define mysql_Passwd   "woaini99"
#define mysql_Database "filemsg"
//查询成功返回-1，失败返回1
int Is_file_md5(char* username,char* filename,char* encrypted)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[300]={0};
	conn=mysql_init(NULL);
	strcpy(query,"select * from FILE_MSG where Username='");
	sprintf(query,"%s%s%s",query,username,"'");
	puts(query);//测试
	if(!mysql_real_connect(conn,mysql_Server,mysql_User,mysql_Passwd,mysql_Database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return 0;
	}else
	{
		printf("Connected..\n");
	}
	int ret;
	ret=mysql_query(conn,query);
	if(ret)//空表返回0
	{
		printf("Error making query:%s\n",mysql_error(conn));
		mysql_close(conn);
		return 1;
	}
	res=mysql_use_result(conn);
	if(res)
	{
		while((row=mysql_fetch_row(res))!=NULL)
		{
			if((!strcmp(row[1],username))&&(!strcmp(row[2],filename)))//用户名和文件名同时存在
			{
				if(!strcmp(row[3],encrypted))
				{
					mysql_free_result(res);
					mysql_close(conn);
					return -1;
				}
			}
		}
		mysql_free_result(res);
		mysql_close(conn);
		return 1;
	}else{
		printf("mysql_use_result:%s",mysql_error(conn));
		mysql_close(conn);
		return 1;
	}
}
//查询不成功，客户端上传完将信息插入数据库
int md5_insert_mysql(char* username,char* filename,char* encrypted,char* file_path)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[500]="insert into FILE_MSG(Username,Filename,FileMD5,Filepath) values('','','','')";
	sprintf(query,"insert into FILE_MSG(Username,Filename,FileMD5,Filepath) values('%s','%s','%s','%s')",username,filename,encrypted,file_path);
	puts(query);
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



