#include "factory.h"

void que_init(que_t* pq,int capacity)//队列初始化
{
	pq->que_capacity=capacity;//队列容量初始化（赋值）
	pq->que_size=0;//初始为0
	pthread_mutex_init(&pq->que_mutex,NULL);//线程信号量初始化
}

void que_set(que_t* pq,node_t* pnew)
{
	if(pq->que_head==NULL)
	{
		pq->que_head=pnew;
		pq->que_tail=pnew;
	}else{
		pq->que_tail->pNext=pnew;
		pq->que_tail=pnew;
	}
	pq->que_size++;
}//尾插法建立队列

void que_get(que_t* pq,pnode_t* p)
{
	*p=pq->que_head;
	pq->que_head=pq->que_head->pNext;
	pq->que_size--;
}//队列出队
