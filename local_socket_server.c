#include <sys/socket.h>
#include <sys/types.h>
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

    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(-1 == ret)
    {
        perror("bind");
        return -2;
    }

    listen(fd, 250);

    int newfd = accept(fd, NULL, NULL);
    if(-1 == newfd)
    {
        perror("accept");
        return -3;
    }

    char buf[1024];
    read(newfd, buf, sizeof(buf));
    printf("%s\n", buf);

    return 0;
}
