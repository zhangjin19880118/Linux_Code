#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(-1 == fd)
    {
        printf("create socket error!\n");
        return -1;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "socket_file");

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(-1 == ret)
    {
        perror("connect");
        return -2;
    }

    write(fd, "hello world", 12);

    return 0;
}
