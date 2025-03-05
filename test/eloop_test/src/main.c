/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "eloop.h"

/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define MAX_BUF_LEN			(512)
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/
static void test_timer_callback(void *eloop_ctx, void *timeout_ctx)
{
	int *para_a = (int *)eloop_ctx;
	int *para_b = (int *)timeout_ctx;
	printf("Timer expired, para_a=%d, para_b=%d\n", *para_a, *para_b);
	(*para_a)++;
	(*para_b)++;
	eloop_register_timeout(5, 0, test_timer_callback, para_a, para_b);
}

static void test_fd_callback(int sock, void *eloop_ctx, void *sock_ctx)
{
	int *para_a = (int *)eloop_ctx;
	int *para_b = (int *)sock_ctx;
	char buffer[MAX_BUF_LEN];
    int len;

    len = read(sock, buffer, sizeof(buffer));
    if (len > 0)
    {
        buffer[len] = '\0';
        printf("Read data: %s, para_a=%d, para_b=%d\n", buffer, *para_a, *para_b);
    }
    return;
}
/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{
	int para_a = 1;
	int para_b = 2;
	eloop_init(NULL);
	eloop_register_read_sock(STDIN_FILENO, test_fd_callback, &para_a, &para_b);
	eloop_register_timeout(5, 0, test_timer_callback, &para_a, &para_b);
	eloop_run();
	eloop_destroy();
	return 0;
}
