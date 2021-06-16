#include"factory.h"

void factory_init(fac* pf,pfunc pthread_func,int capacity)//线程池初始化
{
	pf->pthid=(pthread_t*)calloc(pf->pthread_num,sizeof(pthread_t));//初始化pthread_num个线程
	pthread_cond_init(&pf->cond,NULL);//条件变量初始化
	que_init(&pf->que,capacity);//队列初始化
	pf->thread_func=pthread_func;//函数指针初始化
}//pf->start_flag已在main函数初始化

void factory_start(fac* pf)//启动线程池
{
	if(pf->start_flag==0)//初始化为0 
	{
		int i;
		for(i=0;i<pf->pthread_num;i++)
		{
			pthread_create(&pf->pthid[i],NULL,pf->thread_func,pf);//创建线程，thread_func函数指针指向线程入口函数，由pf传入（可为NULL表示不传）
		}
		pf->start_flag=1;//标志线程池已启动
	}
}
