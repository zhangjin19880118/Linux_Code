#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handle(int v)
{
    if(v == SIGCHLD)
    {
        while(1)
        {
            pid_t pid = waitpid(-1, NULL, WNOHANG);
            if(pid <= 0) 
                break;
        }
    }
}

int main()
{
    signal(SIGCHLD, sig_handle);
    
    pid_t pid = fork();
    if(0 == pid)
    {
        int fd_key = open("/dev/input/event1", O_RDONLY);
        char buf_key[128];
        while(1)
        {
            int ret_key = read(fd_key, buf_key, sizeof(buf_key));
            printf("ret_key is %d\n", ret_key);
        }

        close(fd_key);
    }

    int fd = open("/dev/input/mice", O_RDONLY);

    char buf[8];
    while(1)
    {
        int ret = read(fd, buf, sizeof(buf));
        printf("0x%02x, 0x%02x, 0x%02x, ret=%d\n", (unsigned char)buf[0], (unsigned char)buf[1], (unsigned char)buf[2], ret);

    }

    close(fd);
    
    return 0;
}
