#include"func.h"
char path[200]="/home/leechan/linux/project/client/ftp-ct2/filesrc/";
void tran_file(int sfd,char*filename)
{
	train t;
	bzero(&t,sizeof(t));
	strcpy(t.buf,filename);//文件名
	t.len=strlen(t.buf);//文件名大小
	char file_path[200]={0};
	strcpy(file_path,path);
	strcat(file_path,filename);//路径+文件名
	int fd=open(file_path,O_RDWR);//打开文件-读和写
	char ok='o',wr='x';
	if(fd==-1)
	{
		send(sfd,&wr,sizeof(wr),0);//通知对端跳出case
		perror("open");
		goto end;
	}
	else{
		send(sfd,&ok,sizeof(ok),0);//发送可以传送的标记
	}//有此文件也要发送
	send(sfd,&t,4+t.len,0);//发文件名+文件名大小
	int ret;
	char encrypted[35]={0};
	ret=MD5_file(file_path,encrypted);//对文件进行MD5码转换
	//puts(encrypted);
	if(ret==-1)
	{
		send(sfd,&wr,1,0);
		goto end;
	}else{
		send(sfd,&ok,1,0);
	}
	send(sfd,encrypted,strlen(encrypted),0);//发文件MD5码
	char flag;
	recv(sfd,&flag,1,0);
	if(flag=='x')
	{
		goto end;
	}//有相同文件不可上传
	struct stat buf;//文件属性结构体
	ret=fstat(fd,&buf);
	if(ret==-1)
	{
		perror("fstat");
		goto end;
	}
	off_t offset=0;
	recv(sfd,&offset,sizeof(offset),0);//接收文件偏移量
	t.len=sizeof(off_t);
	memcpy(t.buf,&buf.st_size,t.len);//从buf.st_size起始地址拷贝t.len个字节到t.buf里-这里拷的就是文件长度
	send_n(sfd,(char*)&t,4+t.len);//文件大小
	lseek(fd,offset,SEEK_SET);//对文件进行偏移
	if((buf.st_size-offset)>1024*1024*100)//大于100M
	{
		off_t m_size=buf.st_size-offset;//文件实际大小=文件大小-偏移量
		char* p=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//mmap将文件映射至共享内存，对于100M以上的文件速度传输较快
		if(p==(char*)-1)
		{
			perror("mmap");
			goto end;
		}
		int cnt=m_size/1000;
		int cnt_mod=m_size%1000;
		p=p+offset;//从偏移处开始读取-断点续传
		for(int i=0;i<cnt;i++)
		{
			bzero(&t,sizeof(t));
			t.len=1000;
			memcpy(t.buf,p,1000);//每次拷1000字节
			ret=send_n(sfd,(char*)&t,4+t.len);//每次发送1000+4个字节
			if(ret==-1)
			{
				goto end;
			}
			p+=1000;//每次偏移1000个字节
		}
		bzero(&t,sizeof(t));
		t.len=cnt_mod;//剩余<1000字节的文件内容
		memcpy(t.buf,p,t.len);
		send_n(sfd,(char*)&t,4+t.len);//最后一次发送文件
		t.len=0;
		send_n(sfd,(char*)&t,4);//告诉对方文件上传完毕
		munmap(p,buf.st_size);//解除内存映射
	}else
	{
		while((t.len=read(fd,t.buf,sizeof(t.buf)))>0)
		{
			ret=send_n(sfd,(char*)&t,4+t.len);//当t.len不为0时一直发送文件
			if(ret==-1)
			{
				goto end;
			}
		}
		t.len=0;
		send_n(sfd,(char*)&t,4);//告诉对方文件上传完毕
	}
	printf("upload ->file successfully\n");
	close(fd);
	return;
end:
	printf("puts failed\n");
	close(fd);
}

