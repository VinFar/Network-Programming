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


ssize_t Recvfrom(int sockfd,void *buf,size_t len,int flags,const struct sockaddr *src_addr,socklen_t *addrlen);
ssize_t Sendto(int sockfd,const void *buf,size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
int Bind(int fd,const struct sockaddr *addr,socklen_t addrlen);
int Socket(int fd, int type, int protocol);
in_addr_t Inet_addr(const char *p);
int Close(int fd);	


#endif /* SOCKET_H_ */
