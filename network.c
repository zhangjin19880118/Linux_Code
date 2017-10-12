#include "network.h"

int myFileSize(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    return buf.st_size;
}

int myServer(uint16_t port, const char *ip)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        close(fd);
        return -1;
    }

    listen(fd, 250);
    
    return fd;
}

int myAccept(int server, struct sockaddr *addr, socklen_t *addrlen)
{
    while(1)
    {
        int newfd = accept(server, addr, addrlen);
        if(-1 == newfd && EINTR == errno)
            continue;
        if(-1 == newfd)
        {
            perror("accpet");
            break;     
        }
        return newfd;
    }

    return -1;
}

int myRead(int fd, char buf[], int len)
{
    int readlen = 0;
    while(readlen != len)
    {
        int ret = read(fd, buf + readlen, len - readlen);
        if(0 == ret)
        {
            printf("peer close socket!\n");
            break;
        }
        if(-1 == ret && EINTR == errno)
            continue;
        
        if(-1 == ret)
        {
            perror("read");
            return ret;
        }
        if(ret > 0)
            readlen += ret;
    }

    return readlen;
}

int myRead2(int fd, char buf[], int len)
{
    int ret = -1;
    while(1)
    {
        ret = read(fd, buf, len);
        if(0 == ret)
        {
            printf("peer close socket!\n");
            return ret;
        }
        if(-1 == ret && EINTR == errno)
            continue;
        if(-1 == ret)
        {
            perror("read");
            break;
        }
        if(ret > 0)
            return ret;
    }
    return ret;
}

int myReadLen(int fd, uint32_t *len)
{
    int ret = myRead(fd, (char *)len, sizeof(*len));
    if(0 == ret)
        *len = 0;
    *len = ntohl(*len);
    return ret;
}

char *myReadBuf(int fd)
{
    uint32_t len;
    myReadLen(fd, &len);
    char *buf = (char *)malloc(len + 1);
    buf[len] = 0;
    int ret = myRead(fd, buf, len);
    if(-1 == ret)
    {
        printf("read error!\n");
        return NULL;
    }
    if(0 == ret)
        return NULL;
    return buf;
}

int myWrite(int fd, const char buf[], int len)
{
    int writelen = 0;

    while(writelen != len)
    {
        int ret = write(fd, buf + writelen, len - writelen);
        if(-1 == ret && EINTR == errno)
            continue;
        if(-1 == ret)
        {
            perror("write");
            break;
        }
        if(ret > 0)
            writelen += ret;
    }

    return writelen;
}

int myWriteLen(int fd, uint32_t len)
{
    len = htonl(len);
    int ret = myWrite(fd, (const char *)&len, sizeof(len));
    return ret;    
}

void myWriteBuf(int fd, const char *buf)
{
    uint32_t len = strlen(buf);
    myWriteLen(fd, len);
    myWrite(fd, buf, len);
}

int myConnect(int fd, uint16_t port, const char *ip)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(-1 == ret)
    {
        perror("connect");
        return -1;
    }
    
    return ret;
}

int myConnect2(uint16_t port, const char *ip)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    
    int ret = myConnect(fd, port, ip);
    if(-1 == ret)
    {
        close(fd);
        return -1;
    }

    return fd;
}
    
