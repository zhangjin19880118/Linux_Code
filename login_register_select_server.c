#include "network.h"

fd_set savefd;
int maxfd;

void handle(int newfd)
{
    close(open("user.dat", O_RDWR | O_CREAT | O_EXCL, 0777));    
    char *buf = myReadBuf(newfd);

    if(NULL == buf)
    {
        close(newfd);
        FD_CLR(newfd, &savefd);
        if(newfd == maxfd)
           maxfd --;
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

    FD_CLR(newfd, &savefd);
    if(newfd == maxfd)
        maxfd --;
}

int main()
{
    int server = myServer(9988, "127.0.0.1");
    if(-1 == server)
    {
        printf("create server error!\n");
        return -1;
    }

    maxfd = server;
    FD_ZERO(&savefd);
    FD_SET(server, &savefd);

    while(1)
    {
        fd_set set;
        struct timeval tv;
        memcpy(&set, &savefd, sizeof(set));

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int ret = select(maxfd + 1, &set, NULL, NULL, &tv);

        if(0 == ret)
            continue;

        if(-1 == ret && EINTR == errno)
            continue;

        if(-1 == ret)
            break;

        if(ret > 0)
        {
            if(FD_ISSET(server, &set))
            {
                int newfd = myAccept(server, NULL, NULL);
                if(-1 == newfd)
                {
                    printf("accept error!\n");
                    return -2;
                }    

                if(maxfd < newfd)
                    maxfd = newfd;

                FD_SET(newfd, &savefd);
            }

            int i;
            for(i = server + 1; i < maxfd + 1; ++ i)
            {
                if(FD_ISSET(i, &set))
                    handle(i);
            }
        }
    }

    close(server);

    return 0;
}
