#include "network.h"

void recvDir(int fd)
{
    uint32_t len;
    myReadLen(fd, &len);
    
    char *dirpath = (char *)alloca(len + 1);
    myRead(fd, dirpath, len);
    dirpath[len] = 0;

    mkdir(dirpath, 0777);
}

int recvFile(int fd)
{
    uint32_t len;
    myReadLen(fd, &len);

    char *filepath = (char *)alloca(len + 1);
    myRead(fd, filepath, len);
    filepath[len] = 0;

    uint32_t file_size;
    myReadLen(fd, &file_size);
    
    int filefd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0777); 

    char buf[4096];
    while(file_size > 0)
    {
        int ret = myRead(fd, buf, file_size > 4096 ? 4096 : file_size);
        
        if(0 == ret)
            break;
        if(-1 == ret)
            break;
        if(ret > 0)
        {
            write(filefd, buf, ret);
            file_size -= ret;
        }
    }

    close(filefd);
    if(0 == file_size)
        return 0;
    return -1;
}

int main()
{
    int server = myServer(9988, "127.0.0.1");
    if(-1 == server)
    {
        printf("creat server error!\n");
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

        while(1)
        {
            char flag;
            myRead(newfd, &flag, 1);
            
            if('d' == flag)
                recvDir(newfd);
            if('r' == flag)
                recvFile(newfd);
            if('x' == flag)
                break;
        }

        close(newfd);
    }

    close(server);

    return 0;
}

