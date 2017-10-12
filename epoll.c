#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main()
{
    int fd_mice = open("/dev/input/mice", O_RDONLY);
    int fd_key = open("/dev/input/event1", O_RDONLY);

    int fd_epoll = epoll_create(512);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd_mice;
    epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_mice, &ev);

    ev.data.fd = fd_key;
    epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_key, &ev);

    struct epoll_event event[2];
    while(1)
    {
        int ret = epoll_wait(fd_epoll, event, 2, 5000);

        if(0 == ret)
            continue;

        if(-1 == ret && EINTR == ret)
            continue;

        if(-1 == ret)
            break;

        int i=0;
        for(i=0; i < ret; ++i)
        {
            int fd = event[i].data.fd;

            char buf[8];
            if(fd == fd_mice)
            {
                int ret_mice = read(fd_mice, buf, sizeof(buf));
                printf("0x%02x, 0x%02x, 0x%02x, ret=%d\n", (unsigned char)buf[0], (unsigned char)buf[1], (unsigned char)buf[2], ret_mice);
            }

            char buf_key[128];
            if(fd == fd_key)
            {
                int ret_key = read(fd_key, buf_key, sizeof(buf_key));
                printf("ret_key is %d\n", ret_key);
            }    
        }
    }

    return 0;
}
