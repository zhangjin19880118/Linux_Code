#include "network.h"

int main()
{
    int fd = myConnect2(8888, "127.0.0.1");
    if(-1 == fd)
    {
        printf("connect error!\n");
        return -1;
    }

    myWriteBuf(fd, "hello");
    myWriteBuf(fd, "world");

    close(fd);

    return 0;
}
