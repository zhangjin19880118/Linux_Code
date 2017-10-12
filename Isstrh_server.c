#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int Isstrh(char *str)
{
    char *start = str;
    char *end = str + strlen(str) - 1;
    while (start <= end)
    {
        if (*start != *end)
            return -1;
        start++;
        end--;
    }
    return 0;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);

    if(-1 == bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
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

        int ret = recv(newfd, buf, sizeof(buf), 0);
        buf[ret] = 0;

        if (!Isstrh(buf))
            printf("该字符串是回文!\n");
        else
            printf("该字符串不是回文!\n");

        close(newfd);
    }

    close(fd);

    return 0;
}
