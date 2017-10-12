#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <map>
#include <string.h>
#include <stdio.h>

using namespace std;

class Channel
{
public:
    Channel()
    {
        this->readSize = 0;
    }
    bufferevent *bev;
    char buf[4096];
    int readSize;
    int packetSize;
    string ip;
};

map<string, Channel *> channels;

void readHeader(struct bufferevent *bev, Channel *channel)
{
    channel->readSize += bufferevent_read(bev, channel->buf + channel->readSize, 4 - channel->readSize);
    if(4 == channel->readSize)
    {
        channel->packetSize = ntohl(*(uint32_t *)channel->buf);
    }
}

void readData(struct bufferevent *bev, Channel *channel)
{
    channel->readSize += bufferevent_read(bev, channel->buf + channel->readSize, 4 + channel->packetSize - channel->readSize);
}

void handleData(Channel *channel)
{
    channel->buf[channel->readSize] = 0;
    char *buf = channel->buf + 4;

    char *toIp = strtok(buf, "|");
    char *content = strtok(NULL, "\0");

    Channel *peer = channels[toIp];
    if(NULL == peer)
    {
        printf("peer offline!\n");
    }
    else
    {
        char buf[4096];
        sprintf(buf, "%s|%s", channel->ip.c_str(), content);
        uint32_t len = strlen(buf);
        len = htonl(len);

        bufferevent_write(peer->bev, (char *)&len, 4);

        bufferevent_write(peer->bev, buf, strlen(buf));
    }

    channel->readSize = 0;
}

void newUsersOnline(Channel *channel)
{
    for(auto it =channels.begin(); it !=channels.end(); ++it)
    {
        Channel *user = it->second;
        
        uint32_t len = htonl(channel->packetSize);
        bufferevent_write(channel->bev, (char *)&len, 4);
        bufferevent_write(channel->bev, channel->ip.c_str(), strlen(channel->ip.c_str()));

        len = htonl(user->packetSize);
        bufferevent_write(user->bev, (char *)&len, 4);
        bufferevent_write(user->bev, user->ip.c_str(), strlen(channel->ip.c_str()));    
    }
}

void readcb(struct bufferevent *bev, void *ctx)
{
    Channel *channel = (Channel *)ctx;
    if(channel->readSize < 4)
    {
        readHeader(bev, channel);
        if(4 == channel->readSize)
            readData(bev, channel);
    }
    else
        readData(bev, channel);

    if(channel->readSize == 4 + channel->packetSize)
        handleData(channel);
}

void eventcb(struct bufferevent *bev, short what, void *ctx)
{
    if(what & BEV_EVENT_CONNECTED)
        printf("connect\n");
    else if(what & BEV_EVENT_EOF)
    {
        printf("peer close socket!\n");
        bufferevent_free(bev);
    }
}

void listener_cb(struct evconnlistener *listener, evutil_socket_t newfd, struct sockaddr *addr, int socklen, void *ptr)
{
    struct sockaddr_in *p = (struct sockaddr_in *)addr;
    string ip = inet_ntoa(p->sin_addr);
    printf("connect ip is %s\n", ip.c_str());

    struct event_base *base = (struct event_base *)ptr;

    struct bufferevent *bev = bufferevent_socket_new(base, newfd, BEV_OPT_CLOSE_ON_FREE);

    Channel *channel = new Channel;
    channel->ip = ip;
    channel->bev = bev;
    
    uint32_t len = htonl(5);
    bufferevent_write(bev, (char *)&len, 4);
    bufferevent_write(bev, "hello", 5);

    newUsersOnline(channel);

    channels[ip] = channel;

    bufferevent_setcb(bev, readcb, NULL, eventcb, channel);
    bufferevent_enable(bev, EV_READ);
}

int main()
{
    struct event_base *base = event_base_new();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("0.0.0.0")
    addr.sin_port = htons(8888);

    struct evconnlistener *listener = evconnlistener_new_bind(base, listener_cb, base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 250, (struct sockaddr *)&addr, sizeof(addr));

    evconnlistener_enable(listener);

    event_base_dispatch(base);

    event_base_free(base);

    return 0;
}
