
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

void handleTCPEcho(int connfd)
{

	int len;
	char buf[BUFFER_SIZE];
	do
	{
		int charsend = 0;
		len = Recv(connfd, (void *)buf, sizeof(buf), 0);
		do
		{
			charsend += Send(connfd, (void *)&buf[charsend], len - charsend, 0);
			printf("Charsend: %d\n", charsend);
		} while (charsend < len);
	} while (len > 0);
	Close(connfd);
}

static void *ThreadMain(void *thread_arg)
{
	int connfd = *((int *)thread_arg);
	printf("created thread fd: %d\n",connfd);
	free(thread_arg);
	handleTCPEcho(connfd);
	return NULL;

}

int main(void)
{
	int fd, connfd;
	struct sockaddr_in server_addr, client_addr;
	pthread_t threadID;
	socklen_t client_addr_len;

	fd = Socket(AF_INET, SOCK_STREAM, 0);
	memset((void *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;

	// server_addr.sin_len = sizeof(struct sockaddr_in);

	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(2007);
	Bind(fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	Listen(fd, 1);
	while (1)
	{
		memset((void *)&client_addr, 0, sizeof(client_addr));
		client_addr_len = (socklen_t)sizeof(client_addr);
		connfd = Accept(fd, (struct sockaddr *)&client_addr, &client_addr_len);
		int *thread_arg = (int *)malloc(sizeof(int));
		*thread_arg = connfd;
		printf("creating thread fd%d\n",connfd);
		pthread_create(&threadID, NULL, ThreadMain, (void *)thread_arg);
	}
	Close(fd);
	return (0);
}
