#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8889);

    if(-1 == connect(fd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("connect");
        return -1;
    }

    char buf_send[1024];
    fgets(buf_send, sizeof(buf_send), stdin);
    buf_send[strlen(buf_send) - 1] = 0;
    write(fd, buf_send, strlen(buf_send));

    char buf_recv[1024];
    recv(fd, buf_recv, sizeof(buf_recv), 0);
    buf_recv[strlen(buf_recv)] = 0;
    printf("%s\n", buf_recv);

    return 0;
}
