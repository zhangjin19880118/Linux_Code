#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

int main()
{
    int fd = open("/dev/input/mice",O_RDONLY);

    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
    
    char buf[8];
    while(1)
    {
        int ret = read(fd, buf, sizeof(buf));
        if(ret < 0)
        {
            //perror("read");
            if(errno == EAGAIN)
                printf("EAGAIN\n");
        }
    }

    return 0;
}
