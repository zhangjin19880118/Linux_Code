#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8888);

    if(-1 == connect(fd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("connect");
        return -1;
    }

    write(fd, "hello", strlen("hello"));

    close(fd);

    return 0;
}
