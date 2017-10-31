
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

#include "Socket.h"

int main(void)
{
	int fd, tr = 1, client_fd[CLIENT_CNT], maxfd;

	fd_set fdset_recv;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len;
	ssize_t len;
	UNUSED(len);
	char buf[9216];
	struct timeval time_val;

	for (int i = 0; i < CLIENT_CNT; i++)
	{
		client_fd[i] = -1;
	}

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
	client_addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
	server_addr.sin_len = sizeof(struct sockaddr_in);
#endif
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_KEEPALIVE, &tr, sizeof(int)) == -1)
	{
		perror("setsockopt");
		_exit(1);
	}

	Bind(fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)); //Bind port to socket

	addr_len = (socklen_t)sizeof(client_addr);

	Listen(fd, CLIENT_CNT); //Accept incoming connections

	memset(buf, 0, sizeof(buf));

	time_val.tv_usec = 1;
	time_val.tv_sec = 1000;

	maxfd = fd;
	while (1)
	{
		FD_ZERO(&fdset_recv);
		FD_SET(fd, &fdset_recv);
		for (int i = 0; i < CLIENT_CNT; i++)
		{
			if (client_fd[i] != -1)
			{
				FD_SET(client_fd[i], &fdset_recv); //Set all client fd's in fdset for receive
			}
		}

		Select(maxfd + 1, &fdset_recv, NULL, NULL, &time_val); //wait for fdset to beceome readable

		if (FD_ISSET(fd, &fdset_recv))
		{ //if select returned because of fd
			for (int i = 0; i < CLIENT_CNT; i++)
			{
				if (client_fd[i] == -1)
				{ 
					memset((void *) &client_addr, 0, sizeof(client_addr));
					addr_len = (socklen_t) sizeof(client_addr);

					client_fd[i] = Accept(fd, (struct sockaddr *)&client_addr, addr_len); //accept all connections, that are
					printf("accepted connection\nfd is: %d\n", i);
					//coming in
					if (maxfd < client_fd[i])
					{
						maxfd = client_fd[i]; //set maxfd to highest accepted fd
						
					}
					break;
				}
			}
		}
		for (int i = 0; i < CLIENT_CNT; i++)
		{
			if (client_fd[i] != -1)
			{
				if (FD_ISSET(client_fd[i], &fdset_recv))
				{ //Check all fds and recv on the one from which select returned
					len = Recv(client_fd[i], buf, sizeof(buf), 0);
					// printf("recvd: %s\n", buf);
					Write(STDOUT_FILENO, buf, len);
					if (len == 0)
					{
						printf("fd %d closed\n", i);
						close(client_fd[i]);

						client_fd[i] = -1;
					}
				}
			}
		}
	}

	return 0;
}
