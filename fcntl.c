#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd1 = open("a.temp",O_WRONLY);
    int fd2 = open("a.temp",O_WRONLY);
    
    int flag=fcntl(fd1,F_GETFL);
    flag |= O_APPEND;
    fcntl(fd1,F_SETFL,flag);

    flag=fcntl(fd2,F_GETFL);
    flag |= O_APPEND;
    fcntl(fd2,F_SETFL,flag);

    write(fd1,"hello",strlen("hello"));
    write(fd2,"world",strlen("world"));
    
    close(fd1);
    close(fd2);
    return 0;
}
