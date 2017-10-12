#include "network.h"

int main()
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("224.0.0.100");
    addr.sin_port = htons(9988);

    //int len = 1;
    //setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &len, sizeof(len));

    sendto(fd, "hello world", strlen("hello world"), 0, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));
    
    char buf[1024] = {0};
    read(fd, buf, sizeof(buf));
    printf("%s\n", buf);

    return 0;
}
