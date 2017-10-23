/*
 * Socket.h
 *
 *  Created on: 18.10.2017
 *      Author: vincent
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

ssize_t Recvfrom(int sockfd,void *buf,size_t len,int flags, struct sockaddr *src_addr,socklen_t *addrlen);
ssize_t Sendto(int sockfd,const void *buf,size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
int Bind(int fd,const struct sockaddr *addr,socklen_t addrlen);
int Socket(int fd, int type, int protocol);
in_addr_t Inet_addr(const char *p);
int Close(int fd);	
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t Send(int sockfd, const void *buf,size_t len,int flags);
ssize_t Recv(int sockfd,void *buf,size_t len,int flags);
int Accept(int fd, struct sockaddr* addr, socklen_t len);
ssize_t Write(int fd,const void* buf,size_t nbytes);
int Listen(int sockfd,int backlog);
int getch(void);

#endif /* SOCKET_H_ */
