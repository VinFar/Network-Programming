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

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
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

		perror("connect error:");
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

ssize_t Recv(int sockfd, void *buf, size_t len, int flags)
{

	ssize_t recv_byte;

	if ((recv_byte = recv(sockfd, buf, len, flags)) < 0)
	{

		printf("recv error:");
		_exit(-1);
	}

	return recv_byte;
}

ssize_t Send(int sockfd, const void *buf, size_t len, int flags)
{

	ssize_t send_byte;

	if ((send_byte = send(sockfd, buf, len, flags)) < 0)
	{

		printf("send error:");
		_exit(-1);
	}

	return send_byte;
}

int Accept(int fd, struct sockaddr *addr, socklen_t len)
{

	int fd_acc;

	if ((fd_acc = accept(fd, addr, &len)) < 0)
	{

		perror("accept error:");
		_exit(-1);
	}
	return fd_acc;
}

int Listen(int sockfd, int backlog)
{

	if (listen(sockfd, backlog))
	{

		perror("listen error:");
		_exit(-1);
	}

	return 0;
}

ssize_t Write(int fd, const void *buf, size_t nbytes)
{

	ssize_t wrote;

	if ((wrote = write(fd, buf, nbytes)) < 0)
	{

		perror("write error:");
		_exit(-1);
	}
	return wrote;
}

/*------------------------------------------------*/
int getch(void)
{
	int c = 0;

	struct termios org_opts, new_opts;
	int res = 0;
	//-----  store old settings -----------
	res = tcgetattr(STDIN_FILENO, &org_opts);
	assert(res == 0);
	//---- set new terminal parms --------
	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
	c = getchar();
	//------  restore old settings ---------
	res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res == 0);
	return (c);
}

int Select(int fd, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{

	int sel;

	sel = select(fd, readset, writeset, exceptset, timeout);

	if (sel < 0)
	{

		perror("select error");
		_exit(-1);
	}

	return sel;
}

void *thr_echo(int *index)
{

	/*
	Thread function for echoing the messages
	each thread has an index for a thr_attr_struct in which the fd is stored
	if the thread returns it sets this fd to -1, so that this is signed as free
	
	*/

	char buf[BUFFER_SIZE];
	ssize_t len[CLIENT_CNT];
	printf("thread created: %d\n", *index);
	while (1)
	{

		len[*index] = Recv(thr_attr_struct[*index].connfd, buf, sizeof(buf), 0);
		Write(STDOUT_FILENO, buf, len[*index]);
		Send(thr_attr_struct[*index].connfd, buf, len[*index], 0);
		memset(buf, 0, len[*index]);
		if (len[*index] == 0) //If nothing was received terminate the connection
		{
			close(thr_attr_struct[*index].connfd);
			printf("thread %d return\n", *index);
			thr_attr_struct[*index].connfd = -1;
			return NULL;
		}
	}
}

void *thr_chargen(int *index)
{

	char buf[BUFFER_SIZE];
	ssize_t len[CLIENT_CNT];
	fd_set recv;

	struct timeval time;

	printf("thread created: %d\n", *index);
	time.tv_usec = 1;
	time.tv_sec = 1000;

	FD_ZERO(&recv);
	FD_SET(thr_attr_struct[*index].connfd, &recv);

	memset(buf, 0, sizeof(buf));

	char i = 1;

	while(i){
		buf[i-1] = i;
		i++;
	}
	buf[256] = '\n';

	select(thr_attr_struct[*index].connfd, NULL, &recv, NULL, &time); //Wait read ready

	Send(thr_attr_struct[*index].connfd, buf, sizeof(buf), 0);
	Write(STDIN_FILENO,buf,sizeof(buf));
	close(thr_attr_struct[*index].connfd);
	printf("thread %d return\n", *index);
	thr_attr_struct[*index].connfd = -1;
	return NULL;
}