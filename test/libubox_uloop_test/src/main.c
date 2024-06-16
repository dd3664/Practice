/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "uloop.h"
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define MAX_BUF_LEN        128
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
struct uloop_timeout g_test_timer;
struct uloop_fd g_test_fd;

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/
static void test_timer_callback(struct uloop_timeout *t)
{
	printf("Timer expired\n");
	uloop_timeout_set(t, 5000);
	return;
}

static void test_fd_callback(struct uloop_fd *ufd, unsigned int events)
{
	char buffer[MAX_BUF_LEN];
	int len;

	len = read(ufd->fd, buffer, sizeof(buffer));
	if (len > 0)
	{
		buffer[len] = '\0';
		printf("Read data: %s\n", buffer);
	}
	return;
}
/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{
	uloop_init();

	g_test_fd.fd = STDIN_FILENO; //监听标准输入文件描述符
	g_test_fd.cb = test_fd_callback;
	g_test_fd.flags = ULOOP_READ;
	uloop_fd_add(&g_test_fd, ULOOP_READ);

	g_test_timer.cb = test_timer_callback;
	uloop_timeout_set(&g_test_timer, 5000);

	uloop_run();

	uloop_done();
	
	return 0;
}
