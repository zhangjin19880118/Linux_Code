#include "network.h"
#include <list>
#include <map>
using namespace std;

pthread_cond_t cond;
pthread_mutex_t mutex;

int epollfd;
list<int> socks;

struct Channel
{
    Channel(int fd)
    {
        this->fd = fd;
        buf = new char[4096];
        readSize = 0;
        packetSize = 0;
    }

    ~Channel()
    {
        delete[] buf;
        close(fd);
    }
    int fd;
    char *buf;//[size]xxxxx
    int readSize;//readSize = packetSize + 4;
    int packetSize;
};

map<int, Channel *> channels;

struct Auto_Lock
{
    Auto_Lock(pthread_mutex_t &m):mutex(m)
    {
        pthread_mutex_lock(&mutex);
    }

    ~Auto_Lock()
    {
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_t &mutex;
};

void epoll_add(int epollfd, int event, int fd)
{
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void handleData(Channel *channel)
{
    close(open("user.dat", O_RDWR | O_CREAT | O_EXCL, 0777));    

    char *buf = channel->buf + 4;
    int newfd = channel->fd;
    if(NULL == buf)
    {
        delete channel;
        return;   
    }

    char *saveptr;
    const char *type = strtok_r(buf, "|", &saveptr);
    const char *user = strtok_r(NULL, "|", &saveptr);
    const char *password = strtok_r(NULL, "|", &saveptr);

    if(0 == strncmp(type,"r", 1))
    {
        FILE *fp = fopen("user.dat", "a+");
        fprintf(fp, "%s\n%s\n", user, password);
        fclose(fp);

        myWriteBuf(newfd, "r|ok");
    }
    else if(0 == strncmp(type, "l", 1))
    {
        char ubuf[1024];
        char pbuf[1024];    
        FILE *fp = fopen("user.dat", "r");
        char *flag;
        while(1)
        {
            flag = fgets(ubuf, sizeof(ubuf), fp);
            if(NULL == flag)
                break;

            fgets(pbuf, sizeof(pbuf), fp);
            ubuf[strlen(ubuf) - 1] = 0;
            pbuf[strlen(pbuf) - 1] = 0;
            if(0 == strcmp(ubuf, user) && 0 == strcmp(pbuf, password))
            {
                myWriteBuf(newfd, "l|ok");
                break;
            }
        }

        if(NULL == flag)
            myWriteBuf(newfd, "l|error");

        fclose(fp);
    }
    else
        myWriteBuf(newfd, "unknow command");

    delete channel;
}

void recvData(Channel *channel)
{    
    int ret = read(channel->fd, channel->buf + channel->readSize, 4 + channel->packetSize - channel->readSize);
    if((ret < 0 && EAGAIN != errno) || 0 == ret)
    {
        {
            Auto_Lock lock(mutex);
            channels.erase(channels.find(channel->fd));
        }

        delete channel;
    }
    else if(ret > 0)
    {
        channel->readSize += ret;
        if(channel->packetSize == channel->readSize - 4)
            handleData(channel);
    }
    else
    {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLONESHOT;
        ev.data.fd = channel->fd;

        epoll_ctl(epollfd, EPOLL_CTL_MOD, channel->fd, &ev);
    }
}

void handle(int newfd)
{
    Channel *channel;
    {
        Auto_Lock lock(mutex);
        channel = channels[newfd];
    }

    if(channel->readSize < 4)
    {
        int ret = read(channel->fd, channel->buf + channel->readSize, 4 - channel->readSize);
        if((ret < 0 && EAGAIN != errno) || 0 == ret)
        {
            if(0 == ret)
                printf("peer close socket!\n");
            {
                Auto_Lock lock(mutex);
                channels.erase(channels.find(newfd));
            }

            delete channel;
        }
        else if(ret > 0)
        {
            channel->readSize += ret;
            if(4 == channel->readSize)
            {
                uint32_t *p = (uint32_t *)channel->buf; 
                channel->packetSize = ntohl(*p);
                recvData(channel);
            }
        }
        else
        {
            struct epoll_event ev;
            ev.events = EPOLLIN | EPOLLONESHOT;
            ev.data.fd = channel->fd;

            epoll_ctl(epollfd, EPOLL_CTL_MOD, newfd, &ev);
        }
    }
    else
        recvData(channel);

}

void *pthread_func(void *ptr)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
        int newfd;
        while(1)
        {
            {
                Auto_Lock lock(mutex);
                if(0 == socks.size())
                    break;
                newfd = *socks.begin();
                socks.pop_front();
            }
            handle(newfd);
        }
    }

    return NULL;
}

int main()
{
    pthread_cond_init(&cond, NULL);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex,&attr);

    int server = myServer(9988, "127.0.0.1");
    if(-1 == server)
    {
        printf("create server error!\n");
        return -1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, pthread_func, NULL);

    epollfd = epoll_create(1024);

    epoll_add(epollfd, EPOLLIN, server);

    struct epoll_event ev[8];
    while(1)
    {
        int ret = epoll_wait(epollfd, ev, sizeof(ev)/sizeof(*ev), 5000);

        if(0 == ret)
            continue;
        if(-1 == ret && EINTR == errno)
            continue;
        if(-1 == ret)
            break;

        int i;
        for(i=0; i<ret; ++i)
        {
            if(server == ev[i].data.fd)
            {
                int newfd = myAccept(server, NULL, NULL);
                if(-1 == newfd)
                {
                    printf("accept error!\n");
                    return -2;
                }    
                epoll_add(epollfd, EPOLLIN | EPOLLONESHOT, newfd);    

                Channel *channel = new Channel(newfd);

                channels[newfd] = channel;

                int flag = fcntl(newfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(newfd, F_SETFL, flag);
            }
            else
            {
                {
                    Auto_Lock lock(mutex);
                    socks.push_back(ev[i].data.fd);
                }
                pthread_cond_signal(&cond);
            }   
        }
    }

    close(server);

    return 0;
}
