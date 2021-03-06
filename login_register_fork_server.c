#include "network.h"

void sig_handle(int v)
{
    if(SIGCHLD == v)
    {
        while(1)
        {
            pid_t pid = waitpid(-1, NULL, WNOHANG);
            if(pid <= 0)
                break;
        }
    }
}

int main()
{
    signal(SIGCHLD, sig_handle);

    int server = myServer(9988, "127.0.0.1");
    if(-1 == server)
    {
        printf("create server error!\n");
        return -1;
    }

    int newfd;
    while(1)
    {
        newfd = myAccept(server, NULL, NULL);
        if(-1 == newfd)
        {
            printf("accept error!\n");
            return -2;
        }    

        pid_t pid = fork();
        if(0 == pid)
            break;

        close(newfd);
    }

    close(server);

    close(open("user.dat", O_RDWR | O_CREAT | O_EXCL, 0777));    
    char *buf = myReadBuf(newfd);

    if(NULL == buf)
    {
        close(newfd);
        return -3;
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

    close(server);

    return 0;
}
