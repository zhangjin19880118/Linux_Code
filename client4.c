#include "network.h"

void sendFile(int fd, const char *src, const char *dst)
{
    myWrite(fd, "r", 1);
    myWriteLen(fd, strlen(dst));
    myWrite(fd, dst, strlen(dst));

    myWriteLen(fd, myFileSize(src));
   
    int filefd = open(src, O_RDONLY);
   
    char buf[4096];
    while(1)
    {
        int ret = read(filefd, buf, sizeof(buf));
        if(0 == ret)
            break;
        myWrite(fd, buf, ret);     
    }
    close(filefd);
}

void sendDir(int fd, const char *src, const char *dst)
{
    myWrite(fd, "d", 1);
    myWriteLen(fd, strlen(dst));
    myWrite(fd, dst, strlen(dst));
    
    char *srcbuf = (char *)malloc(1024 * sizeof(char));
    char *dstbuf = (char *)malloc(1024 * sizeof(char));
    DIR *dir = opendir(src);
    struct dirent *entry;
    while(1)
    {
        entry = readdir(dir);

        if(NULL == entry)
            break;
       
        if('.' == entry->d_name[0])
            continue;
        
        sprintf(srcbuf, "%s/%s", src, entry->d_name);
        sprintf(dstbuf, "%s/%s", dst, entry->d_name);
     
        if(DT_DIR == entry->d_type)
            sendDir(fd, srcbuf, dstbuf);
     
        if(DT_REG == entry->d_type)
            sendFile(fd, srcbuf, dstbuf);        
    }
    free(srcbuf);
    free(dstbuf);
    closedir(dir);
}

int main(int argc, char **argv)//./client src dst ip port
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

    sendDir(fd, src_file, dst_file);
    myWrite(fd, "x", 1);

    return 0;
}
