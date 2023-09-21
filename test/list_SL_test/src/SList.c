/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include "SList.h"
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
//动态申请一个节点
SListNode *buySListNode(SLDataType x)
{
    SListNode *ptr = (SListNode*)malloc(sizeof(SListNode));
    if (ptr == NULL)
    {
        perror("malloc");
        exit(-1);
    }
    ptr->data = x;
    ptr->next = NULL;
    return ptr;
}

//单链表打印
void SListPrint(SListNode *plist)
{
    while (plist)
    {
        printf("%d ", plist->data);
        plist = plist->next;
    }
    printf("NULL\n");
}

//单链表尾插
void SListPushBack(SListNode **pplist, SLDataType x)
{
    SListNode *tail = NULL;

    if (*pplist == NULL)
    {
        *pplist = buySListNode(x);
    }
    else
    {
        tail = *pplist;
        while (tail->next)
            tail = tail->next;

        tail->next = buySListNode(x);
    }
}

//单链表尾删
void SListPopBack(SListNode **pplist)
{
    SListNode *tail = *pplist;

    assert(*pplist);

    while(tail->next->next)
        tail=tail->next;
    free(tail->next);
    tail->next = NULL;
}

//单链表头插
void SListPushFront(SListNode **pplist, SLDataType x)
{
    SListNode *cur = NULL;

    if (*pplist == NULL)
    {
        *pplist = buySListNode(x);
    }
    else
    {
        cur = buySListNode(x);
        cur->next = *pplist;
        *pplist = cur;
    }
}

//单链表头删
void SListPopFront(SListNode **pplist)
{
    SListNode *cur = *pplist;

    assert(*pplist);

    *pplist = cur->next;
    free(cur);
}

//单链表查找
SListNode *SListFind(SListNode *plist, SLDataType x)
{
    SListNode *cur = plist;

    assert(plist);

    while(cur)
    {
        if (cur->data == x)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

//单链表中间插入
void SListInsertAfter(SListNode *pos, SLDataType x)
{
    SListNode *tmp = NULL;
    SListNode *cur = NULL;

    assert(pos);

    tmp = buySListNode(x);
    cur = pos->next;
    pos->next = tmp;
    tmp->next = cur;
}

//单链表中间删除
void SListEraseAfter(SListNode *pos)
{
    SListNode *cur = NULL;

    assert(pos);

    if (pos->next == NULL)
        return;
    cur = pos->next;
    pos->next = cur->next;
    free(cur);
}

//单链表销毁
void SListDestroy(SListNode **pplist)
{
    SListNode *cur = NULL;
    assert(*pplist);
    while (*pplist)
    {
        cur = (*pplist)->next;
        free(*pplist);
        *pplist = cur;
    }
    *pplist = NULL;
}