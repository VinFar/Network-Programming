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

#define BUFFER_SIZE (1 << 16)
#define MESSAGE_SIZE (9216)

int main(int argc, char **argv)
{
	int fd;
	struct sockaddr_in server_addr;
	socklen_t addr_len;
	ssize_t len;
	char buf[BUFFER_SIZE];
	char chr;

	if (argc < 3)
	{
		puts("too few arguments!");
		_exit(-1);
	}

	fd = Socket(AF_INET, SOCK_STREAM, 0); //create socket

	memset(&server_addr, 0, sizeof(server_addr)); //flush sockaddr_in struct

	server_addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
/*server_addr.sin_len = sizeof(struct sockaddr_in);*/
#endif
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = Inet_addr(argv[1]);

	addr_len = (socklen_t)sizeof(server_addr);

	Connect(fd, (const struct sockaddr *)&server_addr, addr_len);

	memset(buf, 0, sizeof(buf));


	len = Recv(fd,buf,sizeof(buf),0);
	Write(STDOUT_FILENO,buf,len);
	memset(buf,0,len);
	shutdown(fd,SHUT_RDWR);
	return 0;

	while ((fgets(buf,BUFFER_SIZE,stdin)))	/* Read 1 byte from STDIN*/
	{
		if(!strncmp(buf,(char*)"exit\n",sizeof("exit\n"))){
			puts("closing connection...");
			break;
		}

		Send(fd, (const void *)buf, BUFFER_SIZE, 0);
		len = Recv(fd,buf,sizeof(buf),0);
		Write(STDOUT_FILENO,buf,len);
		memset(buf,0,len);
	}

	shutdown(fd,SHUT_RDWR);
	return 0;
}
