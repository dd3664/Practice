/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include "DList.h"
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
DListNode *buyDlistNode(DLDataType x)
{
    DListNode *ptr = (DListNode*)malloc(sizeof(DListNode));
    if (ptr == NULL)
    {
        perror("malloc");
        exit(-1);
    }
    ptr->data = x;
    return ptr;
}

//创建双向循环链表的头节点，将哨兵位值域置零
DListNode *DListCreate()
{
    DListNode *head = buyDlistNode(0);
    head->prev = head;
    head->next = head;
    return head;
}

//双向循环链表打印
void DListPrint(DListNode *phead)
{
    DListNode *cur;

    if (phead == NULL)
    {
        printf("NULL\n");
        return;
    }

    cur  = phead->next;
    while (cur != phead)
    {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("HEAD\n");
}

//双向循环链表尾插
void DListPushBack(DListNode *phead, DLDataType x)
{
    DListNode *newNode;

    assert(phead);

    newNode = buyDlistNode(x);
    newNode->prev = phead->prev; //新尾节点prev指向以前的尾节点
    newNode->next = phead;  //新尾节点next指向头节点
    phead->prev->next = newNode; //原尾节点next指向新尾节点
    phead->prev = newNode; //头节点prev指向新尾节点
}

//双向循环链表尾删
void DListPopBack(DListNode *phead)
{
    DListNode *tail;

    assert(phead);
    assert(phead->next != phead); //链表中是否只包含头节点

    tail = phead->prev;
    tail->prev->next = phead; //倒数第二节点next指向头节点
    phead->prev = tail->prev; //头节点prev指向倒数第二节点
    free(tail);
}

//双向循环链表头插
void DListPushFront(DListNode *phead, DLDataType x)
{
    DListNode *newNode;

    assert(phead);

    newNode = buyDlistNode(x);
    newNode->next = phead->next; //新节点作为头节点后的第一个数据节点
    newNode->prev = phead;
    phead->next->prev = newNode; //原第一个数据节点prev指向新节点
    phead->next = newNode;  //头节点next指向新节点
}

//双向循环链表头删
void DListPopFront(DListNode *phead)
{
    DListNode *first;
    
    assert(phead);
    assert(phead->next != phead); //链表中是否只包含头节点

    first = phead->next;
    first->next->prev = phead; //第二个数据节点prev指向头节点
    phead->next = first->next;  //头节点next指向第二个数据节点
    free(first);

}


//双向循环链链表查找
DListNode *DListFind(DListNode *phead, DLDataType x)
{
    DListNode *cur;

    assert(phead);

    cur = phead->next;
    while (cur != phead)
    {
        if (cur->data == x)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

//双向循环链表中间插入:插入到pos之前
void DListInsert(DListNode *pos, DLDataType x)
{
    DListNode *prev;
    DListNode *newNode;

    assert(pos);

    newNode = buyDlistNode(x);
    prev = pos->prev;
    newNode->next = pos;
    newNode->prev = prev;
    prev->next = newNode;
    pos->prev = newNode;
}

//双向循环链表中间删除：删除pos节点
void DListErase(DListNode *pos)
{
    assert(pos);
    
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    free(pos);
}

//双向循环链表销毁
void DListDestroy(DListNode **phead)
{
    DListNode *cur;
    DListNode *next;

    assert(*phead);
    
    cur = (*phead)->next;
    next = cur->next;
    while (cur != (*phead))
    {
        free(cur);
        cur = next;
        next = cur->next;
    }
    free(*phead);
    *phead = NULL;
}