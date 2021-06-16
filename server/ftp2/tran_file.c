#include "factory.h"
char path[200]="/home/leechan/linux/project/server/ftp2/source/upload/";

void tran_file(int new_fd,char* username)
{
	train t;
	int ret,len=0;
	char filename[100]={0};
	ret=recv(new_fd,&len,4,0);//接收要传送的文件名长度
	if(ret==0)
	{
		printf("66");
		goto end;
	}
	ret=recv(new_fd,filename,len,0);//接收文件名
	if(ret==0)
	{
		printf("77");
		goto end;
	}
	strcpy(t.buf,filename);//文件名拷贝至火车buf中
	char ok='o',wr='x';
	char path_file[200]={0};
	sprintf(path_file,"%s%s%s%s",path,username,"/",filename);//文件的绝对路径
	//puts(path_file);
	int fd=open(path_file,O_RDWR);
	if(fd==-1)
	{
		send(new_fd,&wr,sizeof(wr),0);
		perror("open");
		goto end;
	}else{
		send(new_fd,&ok,sizeof(ok),0);
	}//没有此文件
	//未结束就发送文件名
	t.len=strlen(t.buf);
	send(new_fd,&t,4+t.len,0);//发送文件名
	struct stat buf;//文件属性结构体
	ret=fstat(fd,&buf);//获取fd文件的结构体指针
	if(ret==-1)
	{
		perror("fstat");
		return;
	}
	off_t offset=0;
	recv(new_fd,&offset,sizeof(offset),0);//接收要传送的文件偏移
	t.len=sizeof(off_t);
	memcpy(t.buf,&buf.st_size,t.len);//从buf.st_size起始地址拷贝t.len个字节到t.buf里-这里拷的就是文件长度值
	send_n(new_fd,(char*)&t,4+t.len);//文件长+文件大小（char型）
	lseek(fd,offset,SEEK_SET);
	//printf("offset:%ld\n",offset);//测试
	if((buf.st_size-offset)>1024*1024*100)//大于100M
	{
		//printf("offset:%ld\n",offset);
		//char p_flag=0;
		off_t m_size=buf.st_size-offset;//要传送的文件实际大小=文件长度-偏移量
		/*while((ret=sendfile(new_fd,fd,&offset,buf.st_size-offset))>0)
		{
			printf("%d\n",ret);
		}
		//recv(new_fd,&p_flag,1,0);
		//if(p_flag=='x')
			goto end;*/
		char* p=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//mmap将文件映射至共享内存，对于100M以上的文件速度传输较快
		if(p==(char*)-1)
		{
			perror("mmap");
			goto end;
		}
		int cnt=m_size/1000;//以1000字节为单位分成cnt份发送
		int cnt_mod=m_size%1000;//剩余文件大小
		p=p+offset;//断点传送
		for(int i=0;i<cnt;i++)
		{
			bzero(&t,sizeof(t));
			t.len=1000;
			memcpy(t.buf,p,1000);//每次拷贝1000字节
			ret=send_n(new_fd,(char*)&t,4+t.len);//每次传送的文件大小+文件内容
			if(ret==-1)
			{
				goto end;
			}
			p+=1000;//映射至内存的指针每次偏移1000个字节（char型）
		}
		bzero(&t,sizeof(t));
		t.len=cnt_mod;//剩余文件大小
		memcpy(t.buf,p,t.len);
		send_n(new_fd,(char*)&t,4+t.len);//发送剩余文件内容
		t.len=0;
		send_n(new_fd,(char*)&t,4);//通知文件已发送完毕-t.len=0
		munmap(p,buf.st_size);//解除文件映射
	}
	else
	{
		while((t.len=read(fd,t.buf,sizeof(t.buf)))>0)
		{
			ret=send_n(new_fd,(char*)&t,4+t.len);
			if(ret==-1)
			{
				goto end;
			}//防止客户端非正常结束

		}
		t.len=0;
		send_n(new_fd,(char*)&t,4);
	}
	printf("puts file successfully!\n");
	writeFile("client download success!");
	return;
end:
	printf("puts failed!\n");
	writeFile("client download failed!");
}
