#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd = open("a.temp",O_WRONLY|O_APPEND);
    for(int i=0;i<5;++i)
    { 
        write(fd,"hello",strlen("hello"));

        sleep(1);    
    }
    return 0;
}
