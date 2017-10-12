#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);

    if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind");
        return -1;
    }

    listen(fd, 10);

    int newfd = accept(fd, NULL, NULL);
    if(newfd < 0)
    {
        perror("accept");
        return -2;
    }
    
    int bufsize = 0;
    int len = sizeof(bufsize);
    getsockopt(newfd, SOL_SOCKET, SO_RCVBUF, &bufsize, (socklen_t *)&len);
    printf("revbufsize = %d\n", bufsize);

    getsockopt(newfd, SOL_SOCKET, SO_SNDBUF, &bufsize, (socklen_t *)&len);
    printf("sndbufsize = %d\n", bufsize);
   
    char buf[1024];
    int ret = read(newfd, buf, sizeof(buf));
    buf[ret] = 0;
    printf("%s\n",buf);

    close(newfd);
    close(fd);

    return 0;
}
