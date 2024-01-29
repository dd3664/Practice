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
#define MAX_LISTEN_BACKLOG          (5)
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

void *client_thread_start(void)
{
	int client_socket;
	struct sockaddr_un server_address;
	struct timeval ti;
	fd_set read_fds;
	int ret;
	size_t size;
	char buffer[64] = {0};
	char message[] = "Message form client\n";

	sleep(1); //等待server初始化完成

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
		ti.tv_sec = 5;
		ti.tv_usec = 0;
		ret = select(MAX_FDS, &read_fds, NULL, NULL, &ti);
		if (-1 == ret)
		{
			perror("select");
		}
		else if (0 == ret)
		{
			printf("client select timeout\n");
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

		sleep(6);
	}

	return NULL;
}

void init_client_thread(void)
{
	pthread_t client_tid;
	pthread_attr_t client_thread_attr;

	pthread_mutex_init(&l_mutex, NULL);
	pthread_cond_init(&l_condition, NULL);

	pthread_attr_init(&client_thread_attr);
	pthread_attr_setstacksize(&client_thread_attr, MY_THREAD_STACK_SIZE);
	pthread_attr_setdetachstate(&client_thread_attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&client_tid, &client_thread_attr, (void *)client_thread_start, NULL);
	
	pthread_attr_destroy(&client_thread_attr);

}

int main(int argc, char *argv[])
{
	struct sockaddr_un server_address;
	int client_socket;
	socklen_t client_addr_len;
	struct sockaddr_un client_address;
	char buffer[64] = {0};
	char message[] = "Message form server\n";
	size_t size;
	struct timeval ti;
	fd_set read_fds;
	int ret;

	init_client_thread();

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
	if (-1 == listen(l_server_socket, MAX_LISTEN_BACKLOG))
	{
		perror("listen");
		goto out;
	}

	signal(SIGINT, handle_interupt);

	while(1)
	{
		FD_ZERO(&read_fds);
		FD_SET(l_server_socket, &read_fds);
		ti.tv_sec = 5;
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
			printf("server select time out\n");
		}
		else
		{
			if (FD_ISSET(l_server_socket, &read_fds))
			{
				client_addr_len = sizeof(struct sockaddr);
				client_socket = accept(l_server_socket, (struct sockaddr*)&client_address, &client_addr_len);
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
