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
#define SERVER_IP		"127.0.0.1"
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
	int client_sock;
	struct sockaddr_in server_addr;
	struct timeval ti;
	fd_set read_fds;
	int ret;
	size_t size;
	char buffer[64] = {0};
	char message[] = "Message form client\n";

	client_sock = init_udp_socket(NULL, 0);

	while (1)
	{
		//设置服务器地址
		memset(&server_addr, 0, sizeof(struct sockaddr_in));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
		server_addr.sin_port = htons(SERVER_PORT);

		size = sendto(client_sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

		FD_ZERO(&read_fds);
		FD_SET(client_sock, &read_fds);
		ti.tv_sec = 5;
		ti.tv_usec = 0;
		ret = select(client_sock + 1, &read_fds, NULL, NULL, &ti);
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
			if (FD_ISSET(client_sock, &read_fds))
			{
				memset(buffer, 0, sizeof(buffer));
				size = recv(client_sock, buffer, sizeof(buffer), 0);
				printf("recv form server buffer=%s\n", buffer);
			}
		}

		sleep(6);

	}

	close(client_sock);

	return 0;
}
