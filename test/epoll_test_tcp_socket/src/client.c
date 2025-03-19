/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/epoll.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define SERVER_IP                      "127.0.0.1"
#define SERVER_PORT                    12345
#define MAX_EVENTS                     (32)
#define EPOLL_WATI_TIME                (5000) /*ms*/
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int init_tcp_client(char *ip, int port)
{
	int sockfd;
    struct sockaddr_in  addr;

	//创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        perror("socket");
        return -1;
    }

	//设置套接字地址
    memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = NULL == ip ? INADDR_ANY : inet_addr(ip);
	addr.sin_port = htons(port);

	//将套接字绑定到地址
    if (-1 == bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
    {
        perror("bind");
		close(sockfd);
        return -1;
    }

    return sockfd;
}

int connect_tcp_server(int sockfd, char *ip, int port)
{
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

    if (-1 == connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
    {
        perror("connect");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
	int client_sock;
	int opt_val;
	size_t size;
	char *buffer;
	char message[] = "Message form client\n";
	int message_len;
	int epoll_fd;
	struct epoll_event event, events[MAX_EVENTS];
	int ready_events;
	int i;


	client_sock = init_tcp_client(NULL, 0);
	if (-1 == client_sock)
	{
		goto out;
	}

	opt_val = 1;
	if (0 > setsockopt(client_sock, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)))
	{
		printf("Error setting SO_REUSEADDR");
		goto out;
	}

	if (0 > connect_tcp_server(client_sock, SERVER_IP, SERVER_PORT))
	{
		goto out;
	}

	message_len = strlen(message);
	size = send(client_sock, &message_len, sizeof(message_len), 0);
	size = send(client_sock, message, strlen(message), 0);
	
	epoll_fd = epoll_create(1);
	if (-1 == epoll_fd)
	{
		perror("failed to create epoll");
		goto out;
	}

	event.events = EPOLLIN;
	event.data.fd = client_sock;
	if (-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock, &event))
	{
		printf("faild to add socket to epoll");
		goto out;
	}
	ready_events = epoll_wait(epoll_fd, events, MAX_EVENTS, EPOLL_WATI_TIME);
	if (-1 == ready_events)
	{
		perror("epoll");
		goto out;
	}
	else if (0 == ready_events)
	{
		printf("client epoll timeout\n");
	}
	else
	{
		for (i = 0; i < ready_events; ++i)
		{
			if (events[i].data.fd == client_sock)
			{
				size = recv(client_sock, &message_len, sizeof(message_len), 0);
				buffer = malloc(message_len + 1);
				memset(buffer, 0, message_len + 1);
				size = recv(client_sock, buffer, message_len, 0);
				printf("recv form server, buffer=%s\n", buffer);
				free(buffer);
			}
		}
	}

	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, &event);

out:
	if (client_sock >= 0)
	{
		close(client_sock);
	}

	return 0;
}
