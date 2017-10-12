#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);

    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(-1 == ret)
    {
        perror("bind:");
        return -1;
    }

    listen(fd, 250);

    int newfd = accept(fd, NULL, NULL);
    if(-1 == newfd)
    {
        perror("accept:");
        return -2;
    }

    while(1)
    {
        char buf[4096];
        int ret = recv(newfd, buf, sizeof(buf), 0);
        if(-1 == ret && EINTR == errno)
            continue;

        if(-1 == ret)
            break;

        if(0 == ret)
        {
            printf("peer close socket!\n");
            break;
        }
        
        if(ret > 0)
            printf("%s", buf);
    }

    printf("\n");

    return 0;
}
