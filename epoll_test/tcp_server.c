#include "func.h"

void epoll_fun(int epfd,int fd,int op)
{
	struct epoll_event event;
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=fd;
	int ret=epoll_ctl(epfd,op,fd,&event);
	if(ret==-1)
	{
		perror("epoll_ctl");
		return;
	}
}

int main(int argc,char** argv)
{
	if(argc!=3)
	{
		printf("./server lack of args\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		perror("socket");
		return -1;
	}
	int reuse=1;
	int ret;
	ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
	if(ret==-1)
	{
		perror("setsockopt");
		return -1;
	}//必须放在bind之前
	struct sockaddr_in sevr;
	bzero(&sevr,sizeof(sevr));
	//memset(&sevr,0,sizeof(sevr));
	sevr.sin_family=AF_INET;
	sevr.sin_port=htons(atoi(argv[2]));
	sevr.sin_addr.s_addr=inet_addr(argv[1]);
	ret=bind(sfd,(struct sockaddr*)&sevr,sizeof(sevr));
	if(ret==-1)
	{
		perror("bind");
		return -1;
	}
	ret=listen(sfd,10);
	if(ret==-1)
	{
		perror("listen");
		return -1;
	}
	struct epoll_event event,evs[3];//结构体别名，结构体数组
	int epfd=epoll_create(1);

	struct sockaddr_in clnt;
	int addrlen=sizeof(clnt);
	int new_fd; 

	epoll_fun(epfd,0,EPOLL_CTL_ADD);//监控标准输入
	epoll_fun(epfd,sfd,EPOLL_CTL_ADD);//监控描述符
	char buf[128]={0};
	int i;
	int res;
	while(1)
	{
		bzero(evs,sizeof(evs));
		res=epoll_wait(epfd,evs,3,-1);//f返回可读的描述符个数
		for(i=0;i<res;i++)//遍历epoll_wait返回的描述符
		{
			if(evs[i].data.fd==sfd)//为server的描述符，可接受client的请求
			{
				bzero(&clnt,sizeof(clnt));
				new_fd=accept(sfd,(struct sockaddr*)&clnt,&addrlen);
				if(new_fd==-1)
				{
					perror("accept");
					return -1;
				}
				printf("clnt ip=%s,clnt port=%d\n",inet_ntoa(clnt.sin_addr),ntohs(clnt.sin_port));
				epoll_fun(epfd,new_fd,EPOLL_CTL_ADD);//注册新的fd
			}
			if(evs[i].data.fd==new_fd)//监听到new_fd可读
			{
				bzero(buf,sizeof(buf));
				ret=recv(new_fd,buf,sizeof(buf),0);
				if(ret==-1)
				{
					perror("recv");
					return -1;
				}else if(ret==0)
				{
					printf("chat over\n");
					epoll_fun(epfd,new_fd,EPOLL_CTL_DEL);
					close(new_fd);
					break;
				}//客户端断开连接
				printf("I am server,recv_buf=%s\n",buf);
			}
			if(evs[i].events==EPOLLIN&&evs[i].data.fd==0)//监听事件为描述符可读，且fd为标准输入
			{
				bzero(buf,sizeof(buf));
				ret=read(0,buf,sizeof(buf));
				if(ret==0)
				{
					printf("chat over1\n");
					epoll_fun(epfd,new_fd,EPOLL_CTL_DEL);//删除new_fd
					close(new_fd);
					break;
				}
				send(new_fd,buf,strlen(buf)-1,0);
			}
		}
	}
	close(sfd);
	return 0;
}


