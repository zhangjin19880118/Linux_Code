#include "network.h"

void *pthread_func(void *ptr)
{
    int newfd = (int)(intptr_t)ptr;
    close(open("user.dat", O_RDWR | O_CREAT | O_EXCL, 0777));    
    char *buf = myReadBuf(newfd);

    if(NULL == buf)
    {
        close(newfd);
        return NULL;
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

    while(1)
    {
        int newfd = myAccept(server, NULL, NULL);
        if(-1 == newfd)
        {
            printf("accept error!\n");
            return -2;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, pthread_func, (void *)(intptr_t)newfd);
        pthread_detach(tid);    
    }

    close(server);

    return 0;
}
