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
int init_udp_socket(char *ip, int port)
{
	int sockfd;
    struct sockaddr_in  addr;

	//创建套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

int main(int argc, char *argv[])
{
	int server_sock;
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

	server_sock = init_udp_socket(SERVER_IP, SERVER_PORT);
	if (-1 == server_sock)
	{
		exit(-1);
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
			/* epoll_wait timeout */
			continue;
		}
		else
		{
			for (i = 0; i < ready_events; ++i)
			{
				if (events[i].data.fd == server_sock)
				{
					memset(&client_addr, 0, sizeof(struct sockaddr_in));
					client_addr_len = sizeof(client_addr);
					size = recvfrom(server_sock, &message_len, sizeof(message_len), 0, (struct sockaddr *)&client_addr, &client_addr_len);
					buffer = malloc(message_len + 1);
					memset(buffer, 0, message_len + 1);
					size = recvfrom(server_sock, buffer, message_len, 0, (struct sockaddr *)&client_addr, &client_addr_len);
					printf("recv from client, buffer=%s\n", buffer);
					free(buffer);

					message_len = strlen(message);
					size = sendto(server_sock, &message_len, sizeof(message_len), 0, (struct sockaddr *)&client_addr, client_addr_len);
					size = sendto(server_sock, message, strlen(message), 0, (struct sockaddr *)&client_addr, client_addr_len);
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
