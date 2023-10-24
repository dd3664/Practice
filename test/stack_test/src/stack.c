/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "stack.h"
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
//初始化栈
void stackInit(Stack *pstack)
{
	assert(pstack);
	pstack->data = NULL;
	pstack->top = 0;
	pstack->capacity = 0;
}

//销毁栈
void stackDestroy(Stack *pstack)
{
	assert(pstack);
	free(pstack->data);
	pstack->data = NULL;
	pstack->top = 0;
	pstack->capacity = 0;
}

//入栈
void stackPush(Stack *pstack, STDatatype x)
{
	int newCapacity;

	assert(pstack);

	if(pstack->top == pstack->capacity) //栈满时动态扩容
	{
		newCapacity = pstack->capacity == 0 ? 4 : pstack->capacity * 2;
		pstack->data = (STDatatype*)realloc(pstack->data, newCapacity * sizeof(STDatatype));
		if (pstack->data == NULL)
		{
			printf("realloc failed!\n");
			exit(-1);
		}
		pstack->capacity = newCapacity;
	}

	memcpy(pstack->data + pstack->top, &x, sizeof(STDatatype));
	pstack->top++;

}

//出栈
void stackPop(Stack *pstack, STDatatype *x)
{
	assert(pstack);
	assert(x);
	if (pstack->top == 0)
	{
		printf("stackPop failed due to the stack is empty!\n");
		return;
	}

	memcpy(x, pstack->data + pstack->top - 1, sizeof(STDatatype));
	pstack->top--;
}

//获取栈顶数据
void stackGetTopData(Stack *pstack, STDatatype *x)
{
	assert(pstack);
	assert(x);
	if (pstack->top == 0)
	{
		printf("stackGetTopData failed due to the stack is empty!\n");
		return;
	}
	memcpy(x, pstack->data + pstack->top - 1, sizeof(STDatatype));
}

//获取栈的大小
int stackSize(Stack *pstack)
{
	assert(pstack);
	return pstack->top;
}

//判断栈是否为空
bool stackIsEmpty(Stack *pstack)
{
	if (pstack->top == 0)
		return true;
	else
		return false;
}