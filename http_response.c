#include "network.h"

char request_buf[] = 
"GET /a.html HTTP/1.1\r\n"
"Host: 127.0.0.1:80\r\n"
"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:54.0) Gecko/20100101 Firefox/54.0\r\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
"Accept-Language: en-US,en;q=0.5\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"Connection: keep-alive\r\n"
"Upgrade-Insecure-Requests: 1\r\n"
"\r\n";

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(80);

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(-1 == ret)
    {
        perror("connect:");
        return -1;
    }

    ret = myWrite(fd, request_buf, strlen(request_buf));
    if(0 == ret)
    {
        printf("write error!\n");
        return -2;
    }

    while(1)
    {
        char response_buf[4096];
        int ret = recv(fd, response_buf, sizeof(response_buf), 0);
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
            printf("%s", response_buf);
    }

    printf("\n");

    return 0;
}
