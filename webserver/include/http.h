#ifndef HTTP_H
#define HTTP_H

#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>

#define MAXLINE	8196
#define BYTES 	8192

void httpreq(int connfd, char *ROOT);

void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);

#endif
