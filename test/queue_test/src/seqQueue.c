/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "seqQueue.h"
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
void initSeqQueue(seqQueue *q)
{
	q->front = q->rear = 0;
}

//判断队列是否为空
bool isSeqQueueEmpty(seqQueue *q)
{
	if (q->rear == q->front) //队空
		return true;
	else
		return false;
}

//入队
bool enSeqQueue(seqQueue *q, elemType x)
{
	if ((q->rear + 1) % MAXSIZE == q->front) //队满
	{
		printf("enSeqQueue failed due to the queue is fulled!\n");
		return false;
	}
	q->data[q->rear] = x;
	q->rear = (q->rear + 1) % MAXSIZE; //队尾指针后移
	return true;
}

//出队
bool deSeqQueue(seqQueue *q, elemType *x)
{
	if (q->rear == q->front)
	{
		printf("deSeqQueue failed due to the queue is empty!\n");
		return false;
	}
	*x = q->data[q->front];
	q->front = (q->front + 1) % MAXSIZE; //队头指针后移
	return true;
}

//获取队头元素
bool getSeqQueueHead(seqQueue *q, elemType *x)
{
	if (q->rear == q->front)
	{
		printf("getSeqQueueHead failed due to the queue is empty!\n");
		return false;
	}
	*x = q->data[q->front];
	return true;
}

//获取队列中元素个数
int getSeqQueueNum(seqQueue *q)
{
	return (q->rear - q->front + MAXSIZE) % MAXSIZE;
}
