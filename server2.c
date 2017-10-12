#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_addr.s_addr = INADDR_ANY;//0;
    addr.sin_port = htons(9988);

    if(-1 == bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind");
        return -1;
    }

    listen(fd, 250);

    while(1)
    {
        char buf[4096];

        int newfd = accept(fd, NULL, NULL);
        if(-1 == newfd && EINTR == errno)
            continue;
        if(-1 == newfd)
            break;

        int filefd = open("a", O_WRONLY | O_CREAT | O_TRUNC, 0777);

        while(1)
        {
            int ret = read(newfd, buf, sizeof(buf));
            if(0 == ret)
            {
                printf("peer close socket!\n");
                break;
            }
            if(-1 == ret && EINTR == errno)
                continue;
            if(-1 == ret)
                break;
            write(filefd, buf, ret);
        }

        close(filefd);
        close(newfd);
    }

    close(fd);

    return 0;
}
