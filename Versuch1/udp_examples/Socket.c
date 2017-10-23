/*
 * Socket.c
 *
 *  Created on: 18.10.2017
 *      Author: vincent
 */

#include "Socket.h"

int Socket(int fd, int type, int protocol)
{

	int n;
	if ((n = socket(fd, type, protocol)) < 0)
	{ //create socket

		perror("socket errror!"); //if no filedescriptor was returned
		_exit(-1);
	}
	return n;
}

int Bind(int fd, const struct sockaddr *addr, socklen_t addrlen)
{

	if (bind(fd, addr, addrlen) < 0)
	{

		perror("bind errror!");
		_exit(-1);
	}
	return 0;
}

ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{

	ssize_t send_len;

	if ((send_len = sendto(sockfd, buf, len, flags, dest_addr, addrlen)) < 0)
	{

		printf("sendto error!");
		_exit(-1);
	}

	return send_len;
}

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, const struct sockaddr *src_addr, socklen_t *addrlen)
{

	ssize_t recv_len;

	if ((recv_len = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) < 0)
	{

		printf("recvfrom error");
		_exit(-1);
	}

	return recv_len;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{

	if (connect(sockfd, addr, addrlen) < 0)
	{

		printf("connect error");
		_exit(-1);
	}
	return 0;
}

in_addr_t Inet_addr(const char *p)
{

	in_addr_t addr;

	if ((addr = inet_addr(p)) == INADDR_NONE)
	{
		printf("inet_addr errror!");
		_exit(-1);
	}

	return addr;
}

int Close(int fd)
{

	if (close(fd) < 0)
	{

		printf("close error!");
		_exit(-1);
	}

	return 0;
}