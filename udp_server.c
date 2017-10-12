#include "network.h"

int main()
{
    int server = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(9988);

    int ret_bind = bind(server, (struct sockaddr *)&addr, sizeof(addr));
    if(-1 == ret_bind)
    {
        perror("bind");
        return -1;
    }

    struct ip_mreq mreq; //register
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.100");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    int ret = setsockopt(server, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if(-1 == ret)
    {
        perror("setsockopt");
        return -2;
    }

    char buf[1024]={0};
    //read(server, buf, sizeof(buf));
    struct sockaddr_in addrpeer;
    int len = sizeof(addrpeer);

    recvfrom(server, buf, sizeof(buf), 0, (struct sockaddr *)&addrpeer, (socklen_t *)&len);
    printf("%s\n", buf);

    sendto(server, "hello", 5, 0, (struct sockaddr *)&addrpeer, (socklen_t)len); 
    
    return 0;
}
