#ifndef __FUNC_H__
#define __FUNC_H__
#include "md5.h"

typedef struct{
	int len;
	char buf[1024];
}train;//С�𳵷��� 4+sizeof(buf)�ֽ�����

void get_file(int,char*);//�ͻ����ϴ��ļ�
void get_ls(int);//�鿴Ŀ¼�ļ��б�
void get_cd(int,char*);//�����ӦĿ¼ cd xx
void get_pwd(int);//��ȡ��ǰĿ¼
void rm_file(int,char*);//ɾ���ļ�

int send_n(int,char*,int);//����С������
int recv_n(int,char*,int);//����С������
void tran_file(int,char*);//��ͻ��˴����ļ�

int login_client(int sfd);//��½�ͻ���


#endif
