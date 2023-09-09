/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define BACKTRACE_SIZE 100
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void print_backtrace()
{
	void *buffer[BACKTRACE_SIZE] = {0};
	int pointer_num = 0;
	char **string_buffer = NULL;

	pointer_num = backtrace(buffer, BACKTRACE_SIZE);
	string_buffer = backtrace_symbols(buffer, pointer_num);

	if (string_buffer == NULL)
	{
		printf("backtrace_sysbols error\n");
		exit(-1);
	}
	printf(">>>BEGIN\n");
	for (int i = 0; i < pointer_num; i++)
	{
		printf("frame %d:  %s\n", i, string_buffer[i]);
	}
	printf(">>>END\n");
	free(string_buffer);
}

void dummy_func()
{
	char *a = NULL;
	strcpy(a, "test");
}

void func()
{
	dummy_func();
}

int main(int argc, char *argv[])
{
	signal(SIGSEGV, print_backtrace);
	func();
	return 0;
}
