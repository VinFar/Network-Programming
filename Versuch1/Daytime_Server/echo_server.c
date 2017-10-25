
/*-
 * Copyright (c) 2013 Michael Tuexen
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdlib.h>
#include <fcntl.h>

#include "Socket.h"

#define BUFFER_SIZE (1 << 16)

int main(void)
{
	int fd, sel_res, tr = 1, fd_stdin;

	fd_set fdset_recv, fdset_err;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len;
	ssize_t len;
	int connfd;
	char buf[9216];
	struct timeval time_val;
	time_t current_time;
	char *c_time_string;

	fd = Socket(AF_INET, SOCK_STREAM, 0);

	memset((void *)&server_addr, 0, sizeof(server_addr)); //Flush server address struct

	server_addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
	server_addr.sin_len = sizeof(struct sockaddr_in);
#endif
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(2007);

	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	client_addr.sin_port = htons(2007);

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) == -1)
	{
		perror("setsockopt");
		_exit(1);
	}

	Bind(fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)); //Bind port to socket

	addr_len = (socklen_t)sizeof(client_addr);

	Listen(fd, 3); //Accept incoming connections

	memset(buf, 0, sizeof(buf));

	connfd = Accept(fd, (struct sockaddr *)&client_addr, addr_len); //wait for client connection to arrive. fill in client_addr struct. fd is the listening socket.
																	//connfd is the connected socket
	time_val.tv_sec = 10;
	time_val.tv_usec = 1;

	FD_ZERO(&fdset_recv); /*zero fd_set*/
	FD_SET(connfd, &fdset_recv);
	if (!FD_ISSET(connfd, &fdset_recv))
	{
		perror("FD_ISSET recv error");
		_exit(-1);
	}
	

	while (Select(connfd + 1, NULL, &fdset_recv, NULL, &time_val))
	{
		

		if (!strncmp(buf, (char *)"exit\n", sizeof("exit\n")))
		{
			puts("closing connection...");
			shutdown(connfd, SHUT_RDWR);
			return 0;
		}
		current_time = time(NULL);

		c_time_string = ctime(&current_time);

		snprintf(buf,sizeof(buf),"%.24s\r\n",ctime(&current_time));

		Send(connfd, buf, sizeof(buf), 0);
		printf("send date:%s", c_time_string);

		Listen(fd, 3); //Accept incoming connections

		memset(buf, 0, sizeof(buf));

		connfd = Accept(fd, (struct sockaddr *)&client_addr, addr_len); //wait for client connection to arrive. fill in client_addr struct. fd is the listening socket.
																		//connfd is the connected socket
	}

	puts("Closing connection...");

	return 0;

	puts("1");

	while ((sel_res = Select(connfd + 1, &fdset_recv, NULL, NULL, &time)))
	{
		if (sel_res == -1)
		{
			perror("select error");
			_exit(-1);
		}

		if (FD_ISSET(connfd, &fdset_recv) != 0)
		{
			len = Recv(connfd, buf, sizeof(buf), 0);

			if (len <= 0)
			{
				puts("Closing connection...");
				shutdown(fd, SHUT_RDWR);
				return 0;
			}

			Write(STDOUT_FILENO, buf, len);

			FD_SET(connfd, &fdset_recv);
			if (!FD_ISSET(connfd, &fdset_recv))
			{
				perror("FD_ISSET error");
				_exit(-1);
			}
		}
	}

	puts("timeout after 10s!\n");

	Close(fd);

	return (0);
}
