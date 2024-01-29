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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/epoll.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define MY_THREAD_STACK_SIZE           (8 * 1024 * 1024)
#define SERVER_IP                      "127.0.0.1"
#define SERVER_PORT                    12345
#define CLIENT_IP                      "127.0.0.1"
#define CLIENT_PORT                    23456
#define MAX_EVENTS                     (32)
#define EPOLL_WATI_TIME                (5000) /*ms*/
#define MAX_LISTEN_BACKLOG             (5)
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
pthread_mutex_t l_mutex;
pthread_cond_t l_condition;
int l_server_socket;
int l_client_socket;
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void handle_interupt(int signum)
{
	printf("handle_interupt\n");
	if (l_server_socket >= 0)
	{
		close(l_server_socket);
	}
	if (l_client_socket >= 0)
	{
		close(l_client_socket);
	}
	exit(EXIT_SUCCESS);
}

void *client_thread_start(void)
{
	struct sockaddr_in client_address;
	struct sockaddr_in server_address;
	int reuse;
	size_t size;
	char *buffer;
	char message[] = "Message form client\n";
	int message_len;
	int epoll_fd;
	struct epoll_event event, events[MAX_EVENTS];
	int ready_events;
	int i;

	epoll_fd = epoll_create(1);
	if (-1 == epoll_fd)
	{
		perror("failed to create epoll");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		//创建客户端套接字
		l_client_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (-1 == l_client_socket)
		{
			perror("scoket");
			goto next;
		}
		//设置客户端地址
		memset(&client_address, 0, sizeof(struct sockaddr_in));
		client_address.sin_family = AF_INET;
		client_address.sin_addr.s_addr = inet_addr(CLIENT_IP);
		client_address.sin_port = htons(CLIENT_PORT);

		//设置SO_REUSEADDR选项，允许端口迅速重用
		reuse = 1;
		if (0 > setsockopt(l_client_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
		{
			perror("Error setting SO_REUSEADDR");
			goto next;
		}

		//将套接字绑定到地址
		if (-1 == bind(l_client_socket, (struct sockaddr*)&client_address, sizeof(struct sockaddr_in)))
		{
			perror("bind");
			goto next;
		}

		//设置服务器地址
		memset(&server_address, 0, sizeof(struct sockaddr_in));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
		server_address.sin_port = htons(SERVER_PORT);

		//连接服务器
		if (-1 == connect(l_client_socket, (struct sockaddr*)&server_address, sizeof(struct sockaddr_in)))
		{
			perror("connect");
			goto next;
		}

		message_len = strlen(message);
		size = send(l_client_socket, &message_len, sizeof(message_len), 0);
		size = send(l_client_socket, message, strlen(message), 0);
		
		event.events = EPOLLIN;
		event.data.fd = l_client_socket;
		if (-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, l_client_socket, &event))
		{
			perror("faild to add socket to epoll");
			goto next;
		}
		ready_events = epoll_wait(epoll_fd, events, MAX_EVENTS, EPOLL_WATI_TIME);
		if (-1 == ready_events)
		{
			perror("epoll");
			goto next;
		}
		else if (0 == ready_events)
		{
			printf("client epoll timeout\n");
		}
		else
		{
			for (i = 0; i < ready_events; ++i)
			{
				if (events[i].data.fd == l_client_socket)
				{
					size = recv(l_client_socket, &message_len, sizeof(message_len), 0);
					buffer = malloc(message_len + 1);
					memset(buffer, 0, message_len + 1);
					size = recv(l_client_socket, buffer, message_len, 0);
					printf("recv form server buffer=%s\n", buffer);
					free(buffer);
				}
			}
		}

		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, l_client_socket, &event);
next:
		if (l_client_socket >= 0)
		{
			close(l_client_socket);
		}

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
	struct sockaddr_in server_address;	
	struct sockaddr_in client_address;
	int client_socket;
	socklen_t client_addr_len;
	char *buffer;
	char message[] = "Message form server\n";
	int message_len;
	size_t size;
	int epoll_fd;
	struct epoll_event event, events[MAX_EVENTS];
	int ready_events;
	int i;

	init_client_thread();

	//创建UNIX域套接字
	l_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == l_server_socket)
	{
		perror("socket");
		goto out;
	}

	//设置套接字地址
	memset(&server_address, 0, sizeof(struct sockaddr_in));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_address.sin_port = htons(SERVER_PORT);

	//将套接字绑定到地址
	if (-1 == bind(l_server_socket, (struct sockaddr*)&server_address, sizeof(struct sockaddr_in)))
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

	epoll_fd = epoll_create(1);
	if (-1 == epoll_fd)
	{
		perror("failed to create epoll");
		goto out;
	}
	event.events = EPOLLIN;
	event.data.fd = l_server_socket;
	if (-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, l_server_socket, &event))
	{
		perror("faild to add socket to epoll");
		goto out;
	}

	while(1)
	{
		ready_events = epoll_wait(epoll_fd, events, MAX_EVENTS, EPOLL_WATI_TIME);
		if (-1 == ready_events)
		{
			perror("epoll");
			goto out;
		}
		else if (0 == ready_events)
		{
			//超时
			printf("server epoll timeout\n");
		}
		else
		{
			for (i = 0; i < ready_events; ++i)
			{
				if (events[i].data.fd == l_server_socket)
				{
					client_addr_len = sizeof(struct sockaddr);
					client_socket = accept(l_server_socket, (struct sockaddr *)&client_address, &client_addr_len);
					if (-1 == client_socket)
					{
						perror("accept");
					}
					else
					{
						event.events = EPOLLIN;
						event.data.fd = client_socket;
						if (-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event))
						{
							perror("faild to add socket to epoll");
						}
					}
				}
				else
				{
					client_socket = events[i].data.fd;
					size = recv(client_socket, &message_len, sizeof(message_len), 0);
					if (0 == size)
					{
						printf("client socket has been closed\n");
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket, &event);
						close(client_socket);
						continue;
					}
					buffer = malloc(message_len + 1);
					memset(buffer, 0, message_len + 1);
					size = recv(client_socket, buffer, message_len, 0);
					printf("recv from client, buffer=%s\n", buffer);
					free(buffer);

					message_len = strlen(message);
					size = send(client_socket, &message_len, sizeof(message_len), 0);
					size = send(client_socket, message, strlen(message), 0);
				}
			}
		}
	}
out:
	if (l_server_socket >= 0)
	{
		close(l_server_socket);
	}
	return 0;
}
