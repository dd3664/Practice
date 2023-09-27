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

//单链表反转
void SListReverse(SListNode **pplist)
{
    SListNode *pre = NULL;
    SListNode *cur = NULL;
    SListNode *next = NULL;

    assert(*pplist);
    
    cur = *pplist;
    while (cur)
    {
        next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    *pplist = pre;
}

//插入排序(升序)
void SListInsertSort(SListNode **pplist)
{
    SListNode *sorted = NULL; //已排序部分的节点
    SListNode *cur = NULL; //未排序部分的当前节点
    SListNode *next = NULL;
    SListNode *insert_after = NULL;

    if( *pplist == NULL || (*pplist)->next == NULL) //空链表或只有一个节点不用排序
    {
        return; 
    }

    cur = *pplist;
    while(cur != NULL)
    {
        next = cur->next; //保存下一节点

        cur->next = NULL; //将current丛未排序部分移除

        //将current在sorted中插入
        if (sorted == NULL || cur->data <= sorted->data)
        {
            cur->next = sorted;
            sorted = cur;
        }
        else
        {
            insert_after = sorted;
            while (insert_after->next != NULL && insert_after->next->data < cur->data)
            {
                insert_after = insert_after->next;
            }
            cur->next = insert_after->next;
            insert_after->next = cur;            
        }

        cur = next; //继续处理下一个节点
    }
    *pplist = sorted;
}

//合并两个有序链表
SListNode *SListMerge(SListNode *left, SListNode *right)
{
    SListNode *result = NULL;

    if (left == NULL)
        return right;
    if (right == NULL)
        return left;
    
    if (left->data <= right->data)
    {
        result = left;
        result->next = SListMerge(left->next, right);
    }
    else
    {
        result = right;
        result->next = SListMerge(left, right->next);
    }
    return result;
}

//拆分链表为两半
void SListSplit(SListNode *source, SListNode **left, SListNode **right)
{
    SListNode *fast;
    SListNode *slow;

    if (source == NULL || source->next == NULL)
    {
        *left = source;
        *right = NULL;
    }
    else
    {
        slow = source;
        fast = source->next;

        while (fast != NULL)
        {
            fast = fast->next;
            if (fast != NULL)
            {
                slow = slow->next;
                fast = fast->next;
            }
        }
        *left = source;
        *right = slow->next;
        slow->next = NULL;
        
    }
}

//归并排序
void SListMergeSort(SListNode **pplist)
{
    SListNode *head = *pplist;
    SListNode *left;
    SListNode *right;

    if (head == NULL || head->next == NULL)
    {
        return;
    }

    //拆分链表为两半
    SListSplit(head, &left, &right);

    //递归第对左半部分和右半部分进行排序
    SListMergeSort(&left);
    SListMergeSort(&right);

    //合并两个有序链表
    *pplist = SListMerge(left, right);
}