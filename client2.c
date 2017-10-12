#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        printf("arg error!\n");
        return -1;
    }

    const char *file = argv[1];
    const char *ip = argv[2];
    const char *port = argv[3];
   
    signal(SIGPIPE,SIG_IGN);

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(-1 == ret)
    {
        perror("connect");
        return -2;
    }

    int filefd = open(file, O_RDONLY);
   
    char buf[4096];
    while(1)
    {
        int ret = read(filefd, buf, sizeof(buf));
        if(0 == ret)
            break;

        int writelen = 0;
        while(1)
        {
           int r = write(fd, buf + writelen, ret - writelen);
           if(r < 0)
           {
               perror("write");
               break;      
           }
           if(r > 0)
               writelen += r;
           if(writelen == ret)
               break;
        }
    }    

    return 0;
}
