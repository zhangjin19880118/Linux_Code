#include <sys/socket.h>
#include <sys/types.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);

    if(-1 == bind(fd, (struct sockaddr*)&addr, sizeof(addr)))
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

        char buf[1024];
        struct sockaddr_in addrpeer;
        int len = sizeof(addrpeer);
        int ret = recvfrom(newfd, buf, sizeof(buf), 0, (struct sockaddr *)&addrpeer, (socklen_t *)&len);
        buf[ret] = 0; 

        sendto(newfd, buf, strlen(buf), 0, (struct sockaddr *)&addrpeer, (socklen_t)len);
    }

    return 0;
}
