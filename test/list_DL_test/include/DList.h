#ifndef __DLIST_H_
#define __DLIST_H_

/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef int DLDataType;
typedef struct _DListNode
{
    DLDataType data;
    struct _DListNode *prev;
    struct _DListNode *next;
} DListNode;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
DListNode *DListCreate();
void DListPrint(DListNode *phead);
void DListPushBack(DListNode *phead, DLDataType x);
void DListPopBack(DListNode *phead);
void DListPushFront(DListNode *phead, DLDataType x);
void DListPopFront(DListNode *phead);
DListNode *DListFind(DListNode *phead, DLDataType x);
void DListInsert(DListNode *pos, DLDataType x);
void DListErase(DListNode *pos);
void DListDestroy(DListNode **phead);
#endif /* __DLIST_H_ */