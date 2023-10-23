/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "seqQueue.h"
#include "linkQueue.h"

/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
seqQueue *l_seqQueue;
linkQueue *l_linkQueue;
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{
	int i;
	int num;

	l_seqQueue = (seqQueue*)malloc(sizeof(l_seqQueue));
	l_linkQueue = (linkQueue*)malloc(sizeof(l_linkQueue));

	printf("\n==========seqQueue==============\n");
	initSeqQueue(l_seqQueue);
	if(isSeqQueueEmpty(l_seqQueue))
		printf("queue is empty\n");
	enSeqQueue(l_seqQueue, 1);
	enSeqQueue(l_seqQueue, 2);
	enSeqQueue(l_seqQueue, 3);
	enSeqQueue(l_seqQueue, 7);
	enSeqQueue(l_seqQueue, 9);
	enSeqQueue(l_seqQueue, 10);
	
	getSeqQueueHead(l_seqQueue, &i);
	printf("head of queue:%d\n", i);

	deSeqQueue(l_seqQueue, &i);
	printf("element1:%d\n", i);

	deSeqQueue(l_seqQueue, &i);
	printf("element2:%d\n", i);

	deSeqQueue(l_seqQueue, &i);
	printf("element3:%d\n", i);

	num = getSeqQueueNum(l_seqQueue);
	printf("number of queue:%d\n", num);


	printf("\n==========LinkQueue==============\n");
	initLinkQueue(l_linkQueue, 5);
	if(isLinkQueueEmpty(l_linkQueue))
		printf("queue is empty\n");
	enLinkQueue(l_linkQueue, 1);
	enLinkQueue(l_linkQueue, 2);
	enLinkQueue(l_linkQueue, 3);
	enLinkQueue(l_linkQueue, 7);
	enLinkQueue(l_linkQueue, 9);
	enLinkQueue(l_linkQueue, 10);
	enLinkQueue(l_linkQueue, 11);
	
	getLinkQueueHead(l_linkQueue, &i);
	printf("head of queue:%d\n", i);

	deLinkQueue(l_linkQueue, &i);
	printf("element1:%d\n", i);

	deLinkQueue(l_linkQueue, &i);
	printf("element2:%d\n", i);

	deLinkQueue(l_linkQueue, &i);
	printf("element3:%d\n", i);

	num = getLinkQueueNum(l_linkQueue);
	printf("number of queue:%d\n", num);


	return 0;
}
