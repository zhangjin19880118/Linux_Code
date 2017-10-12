#include "network.h"

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

        uint32_t len;
        myReadLen(newfd, &len);
        printf("dst_file len is %d\n", len);
                
        char *path = (char *)alloca(len + 1);
        path[len] = 0;

        myRead(newfd, path, len);

        int filefd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);

        uint32_t file_size;
        myReadLen(newfd, &file_size);
        printf("dst_file file_size is %d\n", file_size);
        
        char buf[4096];
        while(file_size > 0)
        {
            int ret = myRead(newfd, buf, file_size > 4096 ? 4096 : file_size);
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
        close(newfd);
    }

    close(server);

    return 0;
}
