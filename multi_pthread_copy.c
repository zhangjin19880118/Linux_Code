#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
    char src[128];
    char dst[128];
    int start;
    int len;
}Arg;

void *pthread_func(void *ptr)
{
    Arg *arg = (Arg*)ptr;

    int src_fd = open(arg->src, O_RDONLY);
    int dst_fd = open(arg->dst, O_WRONLY, 0777);

    lseek(src_fd, arg->start, SEEK_SET);
    lseek(dst_fd, arg->start, SEEK_SET);

    char buf[1024*4];
    while(arg->len > 0)
    {
        memset(buf, 0, sizeof(buf));
        int ret = read(src_fd, buf, arg->len > 1024*4 ? 1024*4 : arg->len);
        if(ret <= 0)
            break;
        write(dst_fd, buf, ret);
        arg->len -= ret;
    }

    close(src_fd);
    close(dst_fd);

    free(arg);
    
    return NULL;
}

void copy_file(int n, char *src_path, char *dst_path)
{
    struct stat st;
    stat(src_path,&st);
    
    int avgsize = st.st_size / n;
    int lastsize = st.st_size % n;
    
    int src_fd = open(src_path,O_RDONLY);
    if(src_fd < 0)
    {
        perror("open src");
        return;
    }

    int dst_fd = open(dst_path,O_WRONLY|O_CREAT|O_EXCL,0777);
    if(dst_fd < 0)
        dst_fd = open(dst_path,O_WRONLY|O_TRUNC,0777);
    ftruncate(dst_fd,st.st_size);

    pthread_t *tid = (pthread_t *)calloc(n,sizeof(pthread_t));
    int i=0;
    for(i=0;i<n;++i)
    {
        Arg *arg = (Arg *)calloc(1,sizeof(Arg));
        strcpy(arg->src, src_path);
        strcpy(arg->dst, dst_path);
        arg->start = i*avgsize;

        if(i==n-1)
            arg->len = avgsize + lastsize;
        else
            arg->len = avgsize;
        
        pthread_create(tid+i, NULL, pthread_func, (void *)arg);
    }

    for(i=0;i<n;++i)
         pthread_join(tid[i],NULL);
    
    close(src_fd);
    close(dst_fd);

    free(tid);
}

int main(int argc,char **argv)
{
    if(argc != 4)
    {
        printf("Arg error!\n");
        return -1;
    }

    int n = atoi(argv[1]);
    char *src_filename = argv[2];
    char *dst_filename = argv[3];

    copy_file(n,src_filename,dst_filename);

    return 0;
}

