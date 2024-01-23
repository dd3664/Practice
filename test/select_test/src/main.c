/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <signal.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define MY_THREAD_STACK_SIZE        (8 * 1024 * 1024)
#define SERVER_PATH                 "/tmp/server_unix_sock"
#define MAX_FDS                     (32)
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
pthread_mutex_t l_mutex;
pthread_cond_t l_condition;
int l_server_socket;
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void handle_interupt(int signum)
{
	printf("handle_interupt\n");
	close(l_server_socket);
	unlink(SERVER_PATH);
	exit(EXIT_SUCCESS);
}

void *my_thread_start(void)
{
	int client_socket;
	struct sockaddr_un server_address;
	struct timeval ti;
	fd_set read_fds;
	int ret;
	size_t size;
	char buffer[64] = {0};
	char message[] = "Message form client\n";

	while (1)
	{
		//创建客户端套接字
		client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
		if (-1 == client_socket)
		{
			perror("scoket");
		}
		//设置服务器地址
		memset(&server_address, 0, sizeof(struct sockaddr_un));
		server_address.sun_family = AF_UNIX;
		strncpy(server_address.sun_path, SERVER_PATH, sizeof(server_address.sun_path) - 1);

		//连接服务器
		if (-1 == connect(client_socket, (struct sockaddr*)&server_address, sizeof(struct sockaddr_un)))
		{
			perror("connect");
		}

		size = send(client_socket, message, strlen(message), 0);

		FD_ZERO(&read_fds);
		FD_SET(client_socket, &read_fds);
		ti.tv_sec = 10;
		ti.tv_usec = 0;
		ret = select(MAX_FDS, &read_fds, NULL, NULL, &ti);
		if (-1 == ret)
		{
			perror("select");
		}
		else if (0 == ret)
		{
			printf("select timeout\n");
		}
		else
		{
			if (FD_ISSET(client_socket, &read_fds))
			{
				memset(buffer, 0, sizeof(buffer));
				size = recv(client_socket, buffer, sizeof(buffer), 0);
				printf("recv form server buffer=%s\n", buffer);
			}
		}

		close(client_socket);

		sleep(1);
	}

	return NULL;
}

void init_my_thread(void)
{
	pthread_t my_tid;
	pthread_attr_t my_attr;

	pthread_mutex_init(&l_mutex, NULL);
	pthread_cond_init(&l_condition, NULL);

	pthread_attr_init(&my_attr);
	pthread_attr_setstacksize(&my_attr, MY_THREAD_STACK_SIZE);
	pthread_attr_setdetachstate(&my_attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&my_tid, &my_attr, (void *)my_thread_start, NULL);
	
	pthread_attr_destroy(&my_attr);

}

int main(int argc, char *argv[])
{
	struct sockaddr_un server_address;
	int client_socket;
	socklen_t client_len;
	struct sockaddr_un client_address;
	char buffer[64] = {0};
	char message[] = "Message form server\n";
	size_t size;
	struct timeval ti;
	fd_set read_fds;
	int ret;

	init_my_thread();

	//创建UNIX域套接字
	l_server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if (-1 == l_server_socket)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	//设置套接字地址
	memset(&server_address, 0, sizeof(struct sockaddr_un));
	server_address.sun_family = AF_UNIX;
	strncpy(server_address.sun_path, SERVER_PATH, sizeof(server_address.sun_path) - 1);

	//将套接字绑定到地址
	if (-1 == bind(l_server_socket, (struct sockaddr*)&server_address, sizeof(struct sockaddr_un)))
	{
		perror("bind");
		goto out;
	}

	//监听套接字
	if (-1 == listen(l_server_socket, 5))
	{
		perror("listen");
		goto out;
	}

	signal(SIGINT, handle_interupt);

	while(1)
	{
		FD_ZERO(&read_fds);
		FD_SET(l_server_socket, &read_fds);
		ti.tv_sec = 10;
		ti.tv_usec = 0;
		ret = select(MAX_FDS, &read_fds, NULL, NULL, &ti);
		if (-1 == ret)
		{
			perror("select");
			goto out;
		}
		else if (0 == ret)
		{
			//超时
		}
		else
		{
			if (FD_ISSET(l_server_socket, &read_fds))
			{
				client_socket = accept(l_server_socket, (struct sockaddr*)&client_address, &client_len);
				if (-1 == client_socket)
				{
					perror("accept");
				}
				else
				{
					memset(buffer, 0, sizeof(buffer));
					size = recv(client_socket, buffer, sizeof(buffer), 0);
					printf("recv form client buffer=%s\n", buffer);
					size = send(client_socket, message, strlen(message), 0);
					close(client_socket);
				}
			}
		}
	}
out:
	close(l_server_socket);
	//删除套接字文件
	unlink(SERVER_PATH);
	return 0;
}
