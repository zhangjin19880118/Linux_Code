#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/input/mice", O_RDONLY);
    int fd_key = open("/dev/input/event1", O_RDONLY);

    char buf[8];
    char buf_key[128];
    while(1)
    {
        int ret = read(fd, buf, sizeof(buf));
        int ret_key = read(fd_key, buf_key, sizeof(buf_key));
        printf("0x%02x, 0x%02x, 0x%02x, ret=%d\n", (unsigned char)buf[0], (unsigned char)buf[1], (unsigned char)buf[2], ret);
        printf("ret_key is %d\n", ret_key);
    }
    
    close(fd);
    close(fd_key);
    
    return 0;
}
