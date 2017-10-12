#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int fd1=open("a.temp",O_WRONLY);
   
    printf("hello to screen1\n");
    
    int fd_backup=dup(1);
    dup2(fd1,1);
    close(fd1);
    printf("hello to file\n");
    
    dup2(fd_backup,1);
    close(fd_backup);
    printf("hello to screen2\n");

    return 0;
}
