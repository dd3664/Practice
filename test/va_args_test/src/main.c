/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdarg.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define MAX_BUF_LEN 256
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void log_message(const char *fmt, ...)
{
	char buf[MAX_BUF_LEN] = {0};
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, MAX_BUF_LEN, fmt, args);
	printf("buf=%s\n", buf);
	va_end(args);
}

int main(int argc, char *argv[])
{
	int a = 10;
	int b = 100;
	log_message("This is a test, a=%d, b=%d\n", a, b);
	return 0;
}