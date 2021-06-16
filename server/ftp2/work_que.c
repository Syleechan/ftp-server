#include "factory.h"

void que_init(que_t* pq,int capacity)//���г�ʼ��
{
	pq->que_capacity=capacity;//����������ʼ������ֵ��
	pq->que_size=0;//��ʼΪ0
	pthread_mutex_init(&pq->que_mutex,NULL);//�߳��ź�����ʼ��
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
}//β�巨��������

void que_get(que_t* pq,pnode_t* p)
{
	*p=pq->que_head;
	pq->que_head=pq->que_head->pNext;
	pq->que_size--;
}//���г���
