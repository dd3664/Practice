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
#define MAX_FDS                     (32)
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int init_unix_client(char *path)
{
    int sockfd;
	struct sockaddr_un addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        perror("socket");
        return -1;
    }

	if (NULL == path)
    {
		return sockfd;
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
	return sockfd;
}

int connect_unix_server(int sockfd, char *srv_path)
{
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, srv_path, sizeof(addr.sun_path) - 1);

    if (-1 == connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)))
    {
        perror("connect");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
	int client_sock;
	int ret;
	struct timeval ti;
	fd_set read_fds;
	size_t size;
	char buffer[64] = {0};
	char message[] = "Message form client\n";

	client_sock = init_unix_client(NULL);
    if (-1 == client_sock)
    {
        exit(-1);
    }
    
    if (0 > connect_unix_server(client_sock, SERVER_PATH))
    {
        exit(-1);
    }

	size = send(client_sock, message, strlen(message), 0);

	FD_ZERO(&read_fds);
	FD_SET(client_sock, &read_fds);
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
		if (FD_ISSET(client_sock, &read_fds))
		{
			memset(buffer, 0, sizeof(buffer));
			size = recv(client_sock, buffer, sizeof(buffer), 0);
			if (0 > size)
			{
				perror("recv");
			}
			else if (0 == size)
			{
				/* socket closed */
			}
			else
			{
				printf("recv form server, buffer=%s\n", buffer);
			}
		}
	}

	close(client_sock);
}
