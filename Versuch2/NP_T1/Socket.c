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

ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen)
{

	ssize_t send_len;

	if ((send_len = sendto(sockfd, buf, len, flags, dest_addr, addrlen)) < 0)
	{

		perror("sendto error!");
		_exit(-1);
	}

	return send_len;
}

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{

	ssize_t recv_len;

	if ((recv_len = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) < 0)
	{

		perror("recvfrom error");
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
		perror("inet_addr errror!");
		_exit(-1);
	}

	return addr;
}

int Close(int fd)
{

	if (close(fd) < 0)
	{

		perror("close error!");
		_exit(-1);
	}

	return 0;
}

ssize_t Recv(int sockfd,void *buf,size_t len,int flags){
	
	ssize_t recv_byte;
	
	if((recv_byte = recv(sockfd,buf,len,flags))<0){
		
		perror("recv error:");
		_exit(-1);
	}
	
	return recv_byte;
}

ssize_t Send(int sockfd, const void *buf,size_t len,int flags){
	
	ssize_t send_byte;
	
	if((send_byte = send(sockfd,buf,len,flags))<0){
		
		perror("send error:");
		_exit(-1);
		
	}
	
	return send_byte;
}

int Accept(int fd, struct sockaddr* addr, socklen_t len){
	
	int fd_acc;	

	if((fd_acc = accept(fd,addr,&len))<0){
	
		perror("accept error:");
		_exit(-1);
	}	
	return fd_acc;
}	

int Listen(int sockfd,int backlog){

	if(listen(sockfd,backlog)){

		perror("listen error:");
		_exit(-1);
	}

	return 0;
}

ssize_t Write(int fd,const void* buf,size_t nbytes){

	ssize_t wrote;

	if((wrote = write(fd,buf,nbytes))<0){

		perror("write error:");
		_exit(-1);
	}
	return wrote;
}

