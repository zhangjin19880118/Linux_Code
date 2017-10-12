#include <sys/select.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main()
{   
    int fd = open("/dev/input/mice", O_RDONLY);
    int fd_key = open("/dev/input/event1", O_RDONLY);

    while(1)
    {
        int nfds = fd;
        if(nfds < fd_key)
            nfds = fd_key;

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        FD_SET(fd_key, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int ret = select(nfds + 1, &readfds, NULL, NULL, &timeout);

        if(0 == ret)
            continue;

        if(-1 == ret && EINTR == ret)
            continue;

        if(-1 == ret)
            break;

        char buf[8]; 
        if(FD_ISSET(fd, &readfds))
        {
            int ret = read(fd, buf, sizeof(buf));
            printf("0x%02x, 0x%02x, 0x%02x, ret=%d\n", (unsigned char)buf[0], (unsigned char)buf[1], (unsigned char)buf[2], ret);
        }

        char buf_key[128];
        if(FD_ISSET(fd_key, &readfds))
        {
            int ret_key = read(fd_key, buf_key, sizeof(buf_key));
            printf("ret_key is %d\n", ret_key);
        }
    }

    close(fd);
    close(fd_key);

    return 0;
}
