#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
    struct sockaddr_in addrpeer;
    int len;
    int newfd;
}Arg;

char *transform(char *str)
{
    char *start = str;
    while(*start != '\0')
    {
        if(*start >= 'a' && *start <= 'z')
            *start -= 32;
        ++start;
    }
    return str;
}

void *pthread_func(void *ptr)
{
    Arg *arg = (Arg *)ptr;
    struct sockaddr_in addrpeer = arg->addrpeer;
    int len = arg->len;
    int newfd = arg->newfd;

    char buf[1024];
    int ret = recvfrom(newfd, buf, sizeof(buf), 0, (struct sockaddr *)&addrpeer, (socklen_t *)&len);
    buf[ret] = 0;
    
    if(0 == strcmp(buf, "HELLO"))
        sendto(newfd, "world", strlen("world"), 0, (struct sockaddr *)&addrpeer, (socklen_t)len);
    else if(0 == strcmp(buf, "WORLD"))
        sendto(newfd, "hello", strlen("hello"), 0, (struct sockaddr *)&addrpeer, (socklen_t)len);
    else
        sendto(newfd, transform(buf), strlen(buf), 0, (struct sockaddr *)&addrpeer, (socklen_t)len);

    return NULL;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);

    if(-1 == bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind");
        return -1;
    }

    listen(fd, 250);

    while(1)
    {
        int newfd = accept(fd, NULL, NULL);
        if(-1 == newfd)
        {
            perror("accept");
            return -2;
        }
        
        struct sockaddr_in addrpeer;
        int len = sizeof(addrpeer);

        Arg *arg = (Arg *)calloc(1,sizeof(Arg));
        arg->addrpeer = addrpeer;
        arg->newfd = newfd;
        arg->len = len;

        pthread_t tid;
        pthread_create(&tid, NULL, pthread_func, arg);
        pthread_detach(tid);
        
        free(arg);
    }

    close(fd);

    return 0;
}
