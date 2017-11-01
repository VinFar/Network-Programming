
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
	int fd, tr = 1;
	int thr_fd_index[CLIENT_CNT];
	fd_set fdset_recv;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len;
	struct timeval time_val;
	pthread_t thr;

	fd = Socket(AF_INET, SOCK_STREAM, 0);

	for (int i = 0; i < CLIENT_CNT; i++)
	{
		thr_fd[i].connfd = -1;
	}

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

	Listen(fd, CLIENT_CNT); //Accept incoming connections

	time_val.tv_sec = 1000;
	time_val.tv_usec = 1;

	FD_ZERO(&fdset_recv); /*zero fd_set*/
	FD_SET(fd, &fdset_recv);
	if (!FD_ISSET(fd, &fdset_recv))
	{
		perror("FD_ISSET recv error");
		_exit(-1);
	}

	while (1)
	{
		FD_ZERO(&fdset_recv);
		FD_SET(fd, &fdset_recv);
		select(fd + 1, &fdset_recv, NULL, NULL, &time_val);
		if (FD_ISSET(fd, &fdset_recv))
		{
			memset((void *)&client_addr, 0, sizeof(client_addr));
			addr_len = (socklen_t)sizeof(client_addr);
			for (int i = 0; i < CLIENT_CNT; i++)
			{
				if (thr_fd[i].connfd == -1)
				{
					thr_fd[i].connfd = Accept(fd, (struct sockaddr *)&client_addr, addr_len);
					thr_fd_index[i] = i;
					printf("creating thread %d\n", i);

					if ((pthread_create(&thr, NULL, Senddate, &thr_fd_index[i]) < 0))
					{
						perror("pthread_create error");
						_exit(-1);
					}
					break;
				}
			}
		}
	}

	puts("Closing connection...");
	Shutdown(fd, SHUT_RDWR);
	return 0;
}
