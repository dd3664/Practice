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
#define SERVER_IP		"0.0.0.0"
#define SERVER_PORT		12345
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
	char buffer[64] = {0};
	char message[] = "Message form server\n";
	size_t size;
	struct timeval ti;
	fd_set read_fds;
	int ret;

	server_sock = init_udp_socket(SERVER_IP, SERVER_PORT);
	if (-1 == server_sock)
	{
		exit(-1);
	}

	while(1)
	{
		FD_ZERO(&read_fds);
		FD_SET(server_sock, &read_fds);
		ti.tv_sec = 5;
		ti.tv_usec = 0;
		ret = select(server_sock + 1, &read_fds, NULL, NULL, &ti);
		if (-1 == ret)
		{
			perror("select");
			continue;
		}
		else if (0 == ret)
		{
			/* select timeout */
			continue;
		}
		else
		{
			if (FD_ISSET(server_sock, &read_fds))
			{
				memset(&client_addr, 0, sizeof(struct sockaddr_in));
				client_addr_len = sizeof(client_addr);
				size = recvfrom(server_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
				printf("recv from client, buffer=%s\n", buffer);
				size = sendto(server_sock, message, strlen(message), 0, (struct sockaddr *)&client_addr, client_addr_len);
			}
		}
	}

	close(server_sock);
	return 0;
}
