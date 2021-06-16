#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "md5.h"
//队列节点结构
typedef struct tag_node
{
	int new_fd;
	struct tag_node* pNext;
}node_t,*pnode_t;//队列节点存储 文件fd+指向下一节点指针

//队列结构体
typedef struct{
	pnode_t que_head,que_tail;
	int que_capacity;
	int que_size;
	pthread_mutex_t que_mutex;
}que_t,*pque_t;

//队列初始化
void que_init(que_t*,int);

//队列元素集合
void que_set(que_t*,node_t*);

//获取队列元素
void que_get(que_t*,pnode_t*);
#endif
