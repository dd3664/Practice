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
#define MAX_LISTEN_BACKLOG             (5)
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int init_tcp_server(char *ip, int port)
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

	//监听套接字
	if (-1 == listen(sockfd, MAX_LISTEN_BACKLOG))
	{
		perror("listen");
		close(sockfd);
		return -1;
	}

    return sockfd;
}

int main(int argc, char *argv[])
{
	int server_sock;
	int client_socket;
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	char *buffer;
	char message[] = "Message form server\n";
	int message_len;
	size_t size;
	int epoll_fd;
	struct epoll_event event, events[MAX_EVENTS];
	int ready_events;
	int i;

	server_sock = init_tcp_server(SERVER_IP, SERVER_PORT);
	if (-1 == server_sock)
	{
		goto out;
	}

	epoll_fd = epoll_create(1);
	if (-1 == epoll_fd)
	{
		perror("failed to create epoll");
		goto out;
	}

	event.events = EPOLLIN;
	event.data.fd = server_sock;
	if (-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_sock, &event))
	{
		printf("faild to add socket to epoll");
		goto out;
	}

	while(1)
	{
		ready_events = epoll_wait(epoll_fd, events, MAX_EVENTS, EPOLL_WATI_TIME);
		if (-1 == ready_events)
		{
			perror("epoll");
			continue;
		}
		else if (0 == ready_events)
		{
			/* epoll timeout */
			continue;
		}
		else
		{
			for (i = 0; i < ready_events; ++i)
			{
				if (events[i].data.fd == server_sock)
				{
					memset(&client_addr, 0, sizeof(struct sockaddr_in));
					client_addr_len = sizeof(struct sockaddr);
					client_socket = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
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
						/* client socket has been closed */
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
	if (server_sock >= 0)
	{
		close(server_sock);
	}
	return 0;
}
