/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
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
int main(int argc, char *argv[])
{
	Stack *stack = (Stack*)malloc(sizeof(Stack));
	int size;
	STDatatype i;

	stackInit(stack);
	if (stackIsEmpty(stack))
		printf("Stack is empty\n");

	stackPush(stack, 1);
	stackPush(stack, 2);
	stackPush(stack, 3);
	stackPush(stack, 4);
	stackPush(stack, 5);
	stackPush(stack, 6);
	stackPush(stack, 7);

	size = stackSize(stack);
	printf("After Push, the size of stack:%d\n",size);

	stackGetTopData(stack, &i);
	printf("Top of the stack data:%d\n", i);

	stackPop(stack, &i);
	printf("Pop stack data:%d\n", i);

	stackPop(stack, &i);
	printf("Pop stack data:%d\n", i);

	stackPop(stack, &i);
	printf("Pop stack data:%d\n", i);

	size = stackSize(stack);
	printf("After Pop, the size of stack:%d\n",size);

	stackDestroy(stack);
	free(stack);
	stack = NULL;
}
