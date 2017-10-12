#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc,char **argv)
{
    char *src_filename=argv[1];
    char *dst_filename=argv[2];

    int fd_src=open(src_filename,O_RDONLY);
    if(fd_src<0)
    {
        perror("open src_filename");
        return 0;
    }

    int fd_dst=open(dst_filename,O_WRONLY|O_CREAT|O_EXCL,0777);
    if(fd_dst<0)
    {
       fd_dst=open(dst_filename,O_WRONLY|O_TRUNC);
       if(fd_dst<0)
       {
        perror("open dst_filename");
        return 0;          
       } 
    }

    char buf[1024*4];
    while(1)
    {
        int ret=read(fd_src,buf,sizeof(buf));
        printf("ret=%d\n",ret);
        if(0==ret)
            break;
        write(fd_dst,buf,ret);
    }

    close(fd_src);
    close(fd_dst);

    return 0;
}
