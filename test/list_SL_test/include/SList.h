#ifndef __SLIST_H_
#define __SLIST_H_

/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef int SLDataType;
typedef struct _SListNode
{
    SLDataType data;
    struct _SListNode *next;
} SListNode;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void SListPrint(SListNode *plist);
void SListPushBack(SListNode **pplist, SLDataType x);
void SListPopBack(SListNode **pplist);
void SListPushFront(SListNode **pplist, SLDataType x);
void SListPopFront(SListNode **pplist);
SListNode *SListFind(SListNode *plist, SLDataType x);
void SListInsertAfter(SListNode *pos, SLDataType x);
void SListEraseAfter(SListNode *pos);
void SListDestroy(SListNode **pplist);
#endif /* __SLIST_H_ */