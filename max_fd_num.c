#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    for(int i=0;;++i)
    {
       int fd=open("max_fd_num.c",O_RDONLY);
       if(fd<0)
       {
         perror("open");
         printf("error fd is %d\n",i);
         break;
       }
       else
        printf("fd=%d\n",fd);   
    }

    return 0;
}
