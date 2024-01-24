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
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define MY_THREAD_STACK_SIZE        (8 * 1024 * 1024)
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 23456
#define MAX_FDS                     (32)
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
	close(l_server_socket);
	close(l_client_socket);
	exit(EXIT_SUCCESS);
}

void *my_thread_start(void)
{
	struct sockaddr_in client_address;
	struct sockaddr_in server_address;
	struct timeval ti;
	fd_set read_fds;
	int ret;
	size_t size;
	char buffer[64] = {0};
	char message[] = "Message form client\n";

	//创建客户端套接字
	l_client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == l_client_socket)
	{
		perror("scoket");
	}
	//设置客户端地址
	memset(&client_address, 0, sizeof(struct sockaddr_in));
	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = inet_addr(CLIENT_IP);
	client_address.sin_port = htons(CLIENT_PORT);

	//将套接字绑定到地址
	if (-1 == bind(l_client_socket, (struct sockaddr*)&client_address, sizeof(struct sockaddr_in)))
	{
		perror("bind");
	}

	while (1)
	{
		//设置服务器地址
		memset(&server_address, 0, sizeof(struct sockaddr_in));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
		server_address.sin_port = htons(SERVER_PORT);

		size = sendto(l_client_socket, message, strlen(message), 0, (struct sockaddr *)&server_address, sizeof(server_address));

		FD_ZERO(&read_fds);
		FD_SET(l_client_socket, &read_fds);
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
			if (FD_ISSET(l_client_socket, &read_fds))
			{
				memset(buffer, 0, sizeof(buffer));
				size = recv(l_client_socket, buffer, sizeof(buffer), 0);
				printf("recv form server buffer=%s\n", buffer);
			}
		}
		
		sleep(6);

	}

	close(l_client_socket);

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
	struct sockaddr_in server_address;	
	struct sockaddr_in client_address;
	socklen_t client_addr_len;
	char buffer[64] = {0};
	char message[] = "Message form server\n";
	size_t size;
	struct timeval ti;
	fd_set read_fds;
	int ret;

	init_my_thread();

	//创建UNIX域套接字
	l_server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == l_server_socket)
	{
		perror("socket");
		exit(EXIT_FAILURE);
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
			printf("server select timeout\n");
		}
		else
		{
			if (FD_ISSET(l_server_socket, &read_fds))
			{
				client_addr_len = sizeof(client_address);
				size = recvfrom(l_server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_addr_len);
				printf("recv from client, buffer=%s\n", buffer);
				size = sendto(l_server_socket, message, strlen(message), 0, (struct sockaddr *)&client_address, client_addr_len);
			}
		}
	}
out:
	close(l_server_socket);
	return 0;
}
