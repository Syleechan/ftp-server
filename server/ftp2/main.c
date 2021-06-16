#include"factory.h"

void* thread_handle(void* p)//函数指针--由pthread_create的第四个参数传入
{
	fac* pf=(fac*)p;//转换为fac类型
	que_t* pq=&pf->que;//线程池队列取地址赋值
	node_t* pcur;//队列节点
	char path[100]={0};
	getcwd(path,sizeof(path));//获取目录路径
	while(1)
	{
		pthread_mutex_lock(&pq->que_mutex);
		if(!pq->que_size)//队列为空没有任务则等待
		{
			pthread_cond_wait(&pf->cond,&pq->que_mutex);//等待唤醒-等待条件变量触发-已将客户端放入队列
		}
		que_get(pq,&pcur);//取队列元素
		pthread_mutex_unlock(&pq->que_mutex);//解锁
		handle_cmd(pcur->new_fd,path);//传递new_fd，当前绝对路径，处理命令
		free(pcur);//释放节点
	}
}



int main(int argc,char** argv)
{
	if(argc!=5)
	{
		printf("./server lack of args IP PORT Thread_NUM CAPACITY");
		return -1;
	}
	fac f;
	bzero(&f,sizeof(f));//初始线程池
	f.pthread_num=atoi(argv[3]);//线程数量
	int capacity=atoi(argv[4]);//队列容量
	factory_init(&f,thread_handle,capacity);//线程池初始化（结构体，函数指针，队列容量）
	int sfd=socket(AF_INET,SOCK_STREAM,0);//生成套接口描述符
	if(sfd==-1)
	{
		perror("socket");
		return -1;
	}
	int reuse=1;
	int ret;
	ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));//设置套接口选项
	if(ret==-1)
	{
		perror("setsockopt");
		return -1;
	}//必须放在bind之前
	struct sockaddr_in sevr;
	bzero(&sevr,sizeof(sevr));//初始化结构体
	//memset(&sevr,0,sizeof(sevr));
	sevr.sin_family=AF_INET;//TCP/IP协议
	sevr.sin_port=htons(atoi(argv[2]));//端口
	sevr.sin_addr.s_addr=inet_addr(argv[1]);//IP地址
	ret=bind(sfd,(struct sockaddr*)&sevr,sizeof(sevr));//绑定IP和端口号
	if(ret==-1)
	{
		perror("bind");
		return -1;
	}
	factory_start(&f);//线程池启动，启动后的线程，队列无任务即队列为空，都会进入睡眠状态

	ret=listen(sfd,10);//监听最大数量为10
	if(ret==-1)
	{
		perror("listen");
		return -1;
	}
	int new_fd;
	node_t* pnew;
	que_t* pq=&f.que;
	while(1)
	{
		new_fd=accept(sfd,NULL,NULL);//同意客户端的连接请求
		writeFile("connected");
		pnew=(node_t*)calloc(1,sizeof(node_t));//创建新的队列节点元素
		pnew->new_fd=new_fd;//客户端的fd赋给队列节点元素
		pthread_mutex_lock(&pq->que_mutex);//加锁
		que_set(pq,pnew);//new_fd按序加入队列，有任务来了
		pthread_mutex_unlock(&pq->que_mutex);//解锁
		pthread_cond_signal(&f.cond);//有任务，触发条件变量
	}
	return 0;
}

