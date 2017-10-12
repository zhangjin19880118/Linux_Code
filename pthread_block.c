#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *mice_func(void *ptr)
{
    int fd = open("/dev/input/mice", O_RDONLY);

    char buf[8];
    while(1)
    {
        int ret = read(fd, buf, sizeof(buf));
        printf("0x%02x, 0x%02x, 0x%02x, ret=%d\n", (unsigned char)buf[0], (unsigned char)buf[1], (unsigned char)buf[2], ret);
    }

    close(fd);
    return NULL;    
}

void *key_func(void *ptr)
{

    int fd_key = open("/dev/input/event1", O_RDONLY);

    char buf_key[128];
    while(1)
    {
        int ret_key = read(fd_key, buf_key, sizeof(buf_key));
        printf("ret_key is %d\n", ret_key);
    }

    close(fd_key);
    return NULL;
}

int main()
{
    pthread_t m_tid, k_tid;
    pthread_create(&m_tid, NULL, mice_func, NULL);
    pthread_create(&k_tid, NULL, key_func, NULL);

    pthread_join(m_tid, NULL);
    pthread_join(k_tid, NULL);

    return 0;
}
