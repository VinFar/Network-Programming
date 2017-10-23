
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
#include "Socket.h"

#define BUFFER_SIZE (1 << 16)

int main(void)
{
	int fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len;
	ssize_t len;
	int connfd;
	char buf[9216];

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

	Bind(fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)); //Bind port to socket

	addr_len = (socklen_t)sizeof(client_addr);

	Listen(fd, 3); //Accept incoming connections

	memset(buf, 0, sizeof(buf));

	connfd = Accept(fd, (struct sockaddr *)&client_addr, addr_len); //wait for client connection to arrive. fill in client_addr struct. fd is the listening socket.
																	//connfd is the connected socket
	/*len = Write(STDOUT_FILENO,&connfd,(sizeof(connfd)));*/

	do
	{
		
		len = Recv(connfd, buf, sizeof(buf), 0);

		Write(STDOUT_FILENO, buf, len);

		Send(connfd, buf, len, 0);
		
	} while (1);

	Close(fd);

	return (0);
}
