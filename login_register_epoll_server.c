#include "network.h"

void epoll_add(int epollfd, int event, int fd)
{
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void handle(int newfd)
{
    close(open("user.dat", O_RDWR | O_CREAT | O_EXCL, 0777));    
    char *buf = myReadBuf(newfd);

    if(NULL == buf)
    {
        close(newfd);
        return; 
    }

    char *saveptr;
    const char *type = strtok_r(buf, "|", &saveptr);
    const char *user = strtok_r(NULL, "|", &saveptr);
    const char *password = strtok_r(NULL, "|", &saveptr);

    if(0 == strncmp(type,"r", 1))
    {
        FILE *fp = fopen("user.dat", "a+");
        fprintf(fp, "%s\n%s\n", user, password);
        fclose(fp);

        myWriteBuf(newfd, "r|ok");
    }
    else if(0 == strncmp(type, "l", 1))
    {
        char ubuf[1024];
        char pbuf[1024];    
        FILE *fp = fopen("user.dat", "r");
        char *flag;
        while(1)
        {
            flag = fgets(ubuf, sizeof(ubuf), fp);
            if(NULL == flag)
                break;

            fgets(pbuf, sizeof(pbuf), fp);
            ubuf[strlen(ubuf) - 1] = 0;
            pbuf[strlen(pbuf) - 1] = 0;
            if(0 == strcmp(ubuf, user) && 0 == strcmp(pbuf, password))
            {
                myWriteBuf(newfd, "l|ok");
                break;
            }
        }

        if(NULL == flag)
            myWriteBuf(newfd, "l|error");

        fclose(fp);
    }
    else
        myWriteBuf(newfd, "unknow command");

    free(buf);
    close(newfd);
}

int main()
{
    int server = myServer(9988, "127.0.0.1");
    if(-1 == server)
    {
        printf("create server error!\n");
        return -1;
    }

    int epollfd = epoll_create(1024);

    epoll_add(epollfd, EPOLLIN, server);

    struct epoll_event ev[8];
    while(1)
    {
        int ret = epoll_wait(epollfd, ev, sizeof(ev)/sizeof(*ev), 5000);

        if(0 == ret)
            continue;
        if(-1 == ret && EINTR == errno)
            continue;
        if(-1 == ret)
            break;

        int i;
        for(i=0; i<ret; ++i)
        {
            if(server == ev[i].data.fd)
            {
                int newfd = myAccept(server, NULL, NULL);
                if(-1 == newfd)
                {
                    printf("accept error!\n");
                    return -2;
                }    
                epoll_add(epollfd, EPOLLIN, newfd);    
            }
            else
                handle(ev[i].data.fd);   
        }
    }

    close(server);

    return 0;
}
