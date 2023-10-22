#ifndef __SEQ_QUEUE_H_
#define __SEQ_QUEUE_H_

/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdbool.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define MAXSIZE (5 + 1) //牺牲一个存储单元来区分队空和队满
typedef int elemType;
typedef struct _seqQueue
{
    elemType data[MAXSIZE];
    int front;
    int rear;
} seqQueue;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void initSeqQueue(seqQueue *q);
bool isSeqQueueEmpty(seqQueue *q);
bool enSeqQueue(seqQueue *q, elemType x);
bool deSeqQueue(seqQueue *q, elemType *x);
bool getSeqQueueHead(seqQueue *q, elemType *x);
int getSeqQueueNum(seqQueue *q);

#endif /* __SEQ_QUEUE_H_ */