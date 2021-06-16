#include "func.h"

struct file_msg{
	long size;//8�ֽ�-�ļ���С
	char type;//1�ֽ�-�ļ�����-Ŀ¼���ļ�
	char file_name[100];//�ļ�Ŀ¼��
};

void get_ls(int sfd)
{
	train t;
	struct file_msg* msg;
	do{
		bzero(&msg,sizeof(struct file_msg));
		bzero(&t,sizeof(t));
		recv(sfd,(char*)&t.len,sizeof(t.len),0);//��Ҫ�����ļ���Ϣ�ĳ���-��len
		if(t.len==0)
			break;//��Ϊ0ֱ���˳�ѭ��
		recv_n(sfd,t.buf,9+t.len);//���ջ�buf����
		msg=(struct file_msg*)t.buf;//ǿ��ת������
		printf("%c %-20s \t\t\t %ldB\n",msg->type,msg->file_name,msg->size);

	}while(1);
}

