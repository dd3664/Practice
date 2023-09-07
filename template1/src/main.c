#include <stdio.h>
#include <malloc.h>

int main(int argc, char *argv[])
{
	printf("hello world\n");
	char *a = NULL;
	while(1)
	{
		a = (char *)malloc(1024);
		memset(a, 0, 1024);
		sleep(1);
	}
	return 0;
}
