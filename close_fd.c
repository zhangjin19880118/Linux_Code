#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    int fd1 = open("test.c", O_RDONLY);//3
    int fd2 = open("test.c", O_RDONLY);//4
    int fd3 = open("test.c", O_RDONLY);//5
    
    close(fd2);
    int fd4 = open("test.c", O_RDONLY);//4
    
    printf("fd4 = %d\n", fd4);

    return 0;
}
