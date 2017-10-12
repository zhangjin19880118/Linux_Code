#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main()
{
    int fd=open("/dev/input/mouse1", O_RDONLY);    
    if(fd<0)
    {
        printf("open errro %d %s\n",errno,strerror(errno));
        // perror("open");
        return -1;
    }

    while(1)
    {
        char buf[8];
        int ret=read(fd,buf,sizeof(buf));

        printf("ret is %d\n",ret);
    }
    return 0;
}
