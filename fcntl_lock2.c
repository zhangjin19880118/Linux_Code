#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    int fd = open("a.txt", O_RDWR);
    
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 4096;
    lock.l_len = 1024;

    fcntl(fd, F_SETLKW, &lock);

    printf("get lock!\n");
    getchar();
    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    return 0;
}
