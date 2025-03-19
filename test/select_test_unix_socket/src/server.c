/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
#define SERVER_PATH                 "/tmp/server_unix_sock"
#define MAX_LISTEN_BACKLOG          (5)
#define MAX_CLIENT_CNT				(3)
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int init_unix_server(char *path)
{
    int sockfd;
    struct sockaddr_un  addr;

	if (NULL == path)
	{
		return -1;
	}
	
	//创建UNIX域套接字
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        perror("socket");
        return -1;
    }

    unlink(path);

	//设置套接字地址
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	//将套接字绑定到地址
    if (-1 == bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)))
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
	int client_sock;
	struct sockaddr_un client_addr;
	socklen_t client_addr_len;
	int client_socks[MAX_CLIENT_CNT] = {0};
	int client_cnt = 0;
	int i;
	char buffer[64] = {0};
	char message[] = "Message form server\n";
	size_t size;
	struct timeval ti;
	fd_set read_fds;
	int max_fd;
	int ret;

	server_sock = init_unix_server(SERVER_PATH);

	while(1)
	{
		FD_ZERO(&read_fds);
		FD_SET(server_sock, &read_fds);
		max_fd = server_sock;
		for (i = 0; i < client_cnt; i++)
		{
			FD_SET(client_socks[i], &read_fds);
			if (client_socks[i] > max_fd)
			{
				max_fd = client_socks[i];
			}
		}
		ti.tv_sec = 5;
		ti.tv_usec = 0;
		ret = select(max_fd + 1, &read_fds, NULL, NULL, &ti);
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
				memset(&client_addr, 0, sizeof(struct sockaddr_un));
				client_addr_len = sizeof(struct sockaddr_un);
				client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);
				if (-1 == client_sock)
				{
					perror("accept");
				}
				else
				{
					if (client_cnt < MAX_CLIENT_CNT)
					{
						client_socks[client_cnt] = client_sock;
						client_cnt++;
					}
					else
					{
						close(client_sock);
						printf("too many clients, rejecting connection\n");
					}							
				}
			}
			for (i = 0; i < client_cnt; i++)
			{
				if (FD_ISSET(client_socks[i], &read_fds))
				{
					memset(buffer, 0, sizeof(buffer));
					size = recv(client_socks[i], buffer, sizeof(buffer), 0);
					if (0 > size)
					{
						perror("recv");
					}
					else if (0 == size)
					{
						close(client_socks[i]);
						client_socks[i] = 0;
						memmove(&client_socks[i], &client_socks[i + 1], (client_cnt - i - 1) * sizeof(int));
						client_socks[client_cnt - 1] = 0;
						client_cnt--;
						i--;
					}
					else
					{
						printf("recv form client, buffer=%s\n", buffer);
						size = send(client_socks[i], message, strlen(message), 0);
					}
				}
			}
		}
	}

	close(server_sock);
	unlink(SERVER_PATH);
	return 0;
}
