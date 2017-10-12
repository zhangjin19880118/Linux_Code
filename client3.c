#include "network.h" 

int main(int argc, char **argv)
{
    if(argc != 5)
    {
        printf("arg error!\n");
        return -1;
    }

    const char *src_file = argv[1];
    const char *dst_file = argv[2];
    const char *ip = argv[3];
    const char *port = argv[4];

    signal(SIGPIPE, SIG_IGN);

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int ret = myConnect(fd, atoi(port), ip);
    if(-1 == ret)
    {
        printf("connect error!\n");
        return -2;
    }
    
    uint32_t len = strlen(dst_file);
    myWriteLen(fd, len);
    myWrite(fd, dst_file, strlen(dst_file));

    int filefd = open(src_file, O_RDONLY);
    struct stat stat_buf;
    stat(src_file, &stat_buf);
    uint32_t file_size = stat_buf.st_size;

    myWriteLen(fd, file_size);

    char buf[4096];
    while(1)
    {
        int ret = read(filefd, buf, sizeof(buf));
        if(0 == ret)
            break;
        myWrite(fd, buf, ret);
    }

    return 0;
}
