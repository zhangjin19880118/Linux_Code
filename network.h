#ifndef   MYNETWORK_H
#define  MYNETWORK_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <alloca.h>
#include <dirent.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C"{
#endif

int myFileSize(const char *path);
int myServer(uint16_t port, const char *ip);//unsigned short port 
int myAccept(int server, struct sockaddr *addr, socklen_t *addrlen);
int myRead(int fd, char buf[], int len);
int myReadLen(int fd, uint32_t *len);
char *myReadBuf(int fd);
int myRead2(int fd, char buf[], int len);
int myWrite(int fd, const char buf[], int len);
int myWriteLen(int fd, uint32_t len);
void myWriteBuf(int fd, const char *buf);
int myConnect(int fd, uint16_t port, const char *ip);
int myConnect2(uint16_t port, const char *ip);

#ifdef __cplusplus
}
#endif

#endif
