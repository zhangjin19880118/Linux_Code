#include "network.h"
#include <list>
using namespace std;

int accept_threads = 4;
int worker_threads = 8;

pthread_mutex_t mutex;
sem_t sem;

struct Channel
{
    Channel(int fd):packetSize(0),readSize(0)
    {
        this->fd = fd;
        buf = new char[4096];
    }

    ~Channel()
    {
	close(fd);
        delete[] buf;
    }
    int fd;
    char *buf;
    uint32_t packetSize;
    uint32_t readSize;
};

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

list<Channel *> channels;
//list<Channel *> channels_free;
int server;
int epollfd;

#if 0
Channel *allocaChannel(int fd)
{
    Channel *ret = NULL;

    Auto_Lock lock(mutex);
    if(channels_free.size() > 0)
    {
        ret = *channels_free.begin();
	channels_free.pop_front();
    }
    else
        ret = new Channel(fd);

    return ret;
}

void freeChannel(Channel *channel)
{
    close(channel->fd);
    channel->packetSize = 0;
    channel->readSize = 0;
    Auto_Lock lock(mutex);
    channels_free.push_back(channel);
}
#endif

void epoll_add(int newfd)
{
    Channel *channel = new Channel(newfd);
    //allocaChannel(newfd);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.ptr = channel;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &ev);
}

void epoll_mode(Channel *channel)
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.ptr = channel;

    epoll_ctl(epollfd, EPOLL_CTL_MOD, channel->fd, &ev);
}

void *accept_pthread_func(void *ptr)
{
    while(1)
    {   
        int newfd = accept(server, NULL, NULL);
        if(-1 == newfd && EINTR == errno)
            continue;
        if(-1 == newfd)
        {
            perror("accept");
            break;
        }

        int flag = fcntl(newfd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(newfd, F_SETFL, flag);

        epoll_add(newfd);       
    }
    return NULL;
}

Channel *readData(Channel *channel)
{
    int ret;
    if(channel->readSize < 4)
    {
        ret = read(channel->fd, channel->buf + channel->readSize, 4 - channel->readSize);
        if(ret > 0)
        {
            channel->readSize += ret;
            if(4 == channel->readSize)
            {
                uint32_t temp = *(uint32_t *)channel->buf;
                channel->packetSize = ntohl(temp);
            }    

            return readData(channel);
        }
    }
    else
    {
        ret = read(channel->fd, channel->buf + channel->readSize, 4 + channel->packetSize - channel->readSize);
        if(ret > 0)
        {
            channel->readSize += ret;
            if(channel->readSize == 4 + channel->packetSize)
                return channel;
            else
                return readData(channel); 
        }
    }

    if((ret < 0 && errno != EAGAIN) || 0 == ret)
    {
        if(0 == ret)
            printf("peer close socket!\n");

        delete channel;
        //freeChannel(channel);
    }
    else
        epoll_mode(channel);

    return NULL;
}

void handleData(Channel *channel)
{
    channel->readSize = 0;
    channel->buf[channel->readSize] = 0;
    printf("%s\n", channel->buf + 4);
    epoll_mode(channel);
}

void *worker_pthread_func(void *ptr)
{
    while(1)
    {
        sem_wait(&sem);
        Channel *channel = NULL;
        {
            Auto_Lock lock(mutex);
            channel = *channels.begin();
            if(0 == channels.size())
                break;
            channels.pop_front();
        }

        if(readData(channel))
            handleData(channel);
    }

    return NULL;
}

void init()
{
#if 0	
    int i;
    for(i=0;i<10000;++i)
    {
        Channel *channel = new Channel(-1);
        channels_free.push_back(channel);
    }
#endif
    server = myServer(8888, "0.0.0.0");
    if(-1 == server)
    {
        printf("create server error!\n");
        return;
    }

    epollfd = epoll_create(1024);

    pthread_t tid;
    int i;
    for(i=0;i<accept_threads;++i)
    {
        pthread_create(&tid, NULL, accept_pthread_func, NULL);
        pthread_detach(tid);
    }

    for(i=0;i<worker_threads;++i)
    {
        pthread_create(&tid, NULL, worker_pthread_func, NULL);
        pthread_detach(tid);
    }

    sem_init(&sem, 0, 0);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
}

void exec()
{
    struct epoll_event ev[8];
    while(1)
    {
        int ret = epoll_wait(epollfd, ev, 8, 5000);
        if(0 == ret)
            continue;
        if(-1 == ret && EINTR == errno)
            continue;
        if(-1 == ret)
            break;

        int i;
        for(i=0;i<ret;++i)
        {
            Channel *channel = (Channel *)ev[i].data.ptr;
            {
                Auto_Lock lock(mutex);
                channels.push_back(channel);
            }

            sem_post(&sem);
        }
    }
}

int main()
{
    init();
    exec();
    return 0;
}
