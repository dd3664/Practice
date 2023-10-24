#ifndef __STACK_H_
#define __STACK_H_

/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdbool.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef int STDatatype;
typedef struct _Stack
{
    STDatatype *data;
    int top; //栈顶位置
    int capacity; //容量
} Stack;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void stackInit(Stack *pstack);
void stackDestroy(Stack *pstack);
void stackPush(Stack *pstack, STDatatype x);
void stackPop(Stack *pstack, STDatatype *x);
void stackGetTopData(Stack *pstack, STDatatype *x);
int stackSize(Stack *pstack);
bool stackIsEmpty(Stack *pstack);
#endif /* __STACK_H_ */