/*                                        带头节点链式队列                                            */

/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkQueue.h"
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
//初始化队列
void initLinkQueue(linkQueue *q, int maxLen)
{
	q->front = q->rear = (linkNode*)malloc(sizeof(linkNode)); //建立头节点，并将头节点和尾节点均指向头节点；
	q->front->next = NULL;
	q->length = 0;
	q->maxLen = maxLen;
}

//判断队列是否为空
bool isLinkQueueEmpty(linkQueue *q)
{
	if (q->front == q->rear)
		return true;
	else
		return false;
}

//入队
bool enLinkQueue(linkQueue *q, elemType x)
{
	linkNode *newNode = NULL;
	
	if (q->length >= q->maxLen)
	{
		printf("enLinkQueue failed due to the queue is fulled!\n");
		return false;
	}

	newNode = (linkNode*)malloc(sizeof(linkNode));
	memcpy(&(newNode->data), &x, sizeof(elemType));
	newNode->next = NULL;

	q->rear->next = newNode;
	q->rear = newNode;
	q->length++;

	return true;
}

//出队
bool deLinkQueue(linkQueue *q, elemType *x)
{
	linkNode *tmp = q->front->next;

	if(q->front == q->rear)
	{
		printf("deLinkQueue failed due to the queue is empty!\n");
		return false;
	}

	memcpy(x, &(tmp->data), sizeof(elemType));

	q->front->next = tmp->next; //修改头节点的next指针

	if (q->rear == tmp) //若是最后一个节点出队
		q->rear = q->front;

	free(tmp);
	q->length--;
	return true;

}

//获取队头元素
bool getLinkQueueHead(linkQueue *q, elemType *x)
{
	if (q->rear == q->front)
	{
		printf("getSeqQueueHead failed due to the queue is empty!\n");
		return false;
	}
	memcpy(x, &(q->front->next->data), sizeof(elemType));
	return true;
}

//获取队列中元素个数
int getLinkQueueNum(linkQueue *q)
{
	return q->length;
}
