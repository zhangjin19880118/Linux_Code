#include "network.h"

void test()
{
    int fd = myConnect2(8888, "127.0.0.1");
    if(-1 == fd)
    {
        printf("connect error!\n");
        return;
    }
    
    myWrite(fd, "hello world", 11);

    close(fd);

    exit(0);
}

int main()
{   
    int i, n = 100000;
    for(i=0;i<n;++i)
    {
        pid_t pid = fork();
        if(pid > 0)
            continue;
        else
            test();
    }

    for(i=0;i<n;++i)
        wait(NULL);

    return 0;
}
