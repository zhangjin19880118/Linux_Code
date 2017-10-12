#include "network.h"

void handle(int server)
{   
    while(1)
    {
        int newfd = accept(server, NULL, NULL);
        printf("newfd = %02d, pid = 0x%x\n", newfd, (int)getpid());
        close(newfd);
    }
}

int main()
{
    int server = myServer(8888, "0.0.0.0");
    if(-1 == server)
    {
        printf("create server error!\n");
        return -1;
    }

    int i;
    for(i=0;i<5;++i)
    {
        int pid = fork();
        if(0 == pid)
            handle(server);
    }

    for(i=0;i<5;++i)
    {
        wait(NULL);
    }

    return 0;
}
