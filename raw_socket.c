#include <unistd.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_arp.h>

void print_eth_header(unsigned char *eth_frame)
{
   unsigned char *p = eth_frame;
   printf("dst_addr: %02x:%02x:%02x:%02x:%02x:%02x\n", p[0], p[1], p[2], p[3], p[4], p[5]);
   printf("src_addr: %02x:%02x:%02x:%02x:%02x:%02x\n", p[6], p[7], p[8], p[9], p[10], p[11]);
   printf("eth_type: %04x\n", ntohs(*(short *)(p + 12)));
}

void handle_tcp(unsigned char *tcp)
{
    for(int i = 0; i < 5; ++i)
    {
        for(int j = 0; j < 4; ++j)
            printf("%02x ", tcp[i * 4 + j]);
        printf("\n");
    }

    int len = ((tcp[12] & 0xf0) >> 4) * 4;
    printf("header_length = %d\n", len);

    printf("%s\n", tcp + len);

    printf("---------------------------tcp\n");

}

void handle_ip(unsigned char *ip)
{
#if 0
    for(int i = 0; i < 5; ++i)
    {
        for(int j = 0; j < 4; ++j)
            printf("%02x ", ip[i * 4 + j]);
        printf("\n");
    }

    printf("---------------------------ip header\n");
#endif

    int len = (ip[0] & 0x0f) * 4;
    int protocol = *(ip + 9);
    if(0x06 == protocol) //TCP
        handle_tcp(ip + len);
}

int main()
{
    int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(-1 == packet_socket)
    {
        printf("create packet_socket error!\n");
        return -1;
    }

    struct ifreq ifstruct;
    strcpy(ifstruct.ifr_name, "eth0");
    
    int ret = ioctl(packet_socket, SIOCGIFINDEX, &ifstruct);
    if(-1 == ret)
    {
        printf("ioctl SIOCGIFINDEX error!\n");
        return -2;       
    }

    struct sockaddr_ll addr;
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = ifstruct.ifr_ifindex;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_pkttype = PACKET_OTHERHOST;
    addr.sll_hatype = ARPHRD_ETHER;
    addr.sll_halen = ETH_ALEN;
    addr.sll_addr[6] = 0;
    addr.sll_addr[7] = 0;

    ret = ioctl(packet_socket, SIOCGIFHWADDR, &ifstruct);
    if(-1 == ret)
    {
        printf("ioctl SIOCGIFHWADDR erorr!\n");
        return -3;
    }

    ret = ioctl(packet_socket, SIOCGIFFLAGS, &ifstruct);
    if(-1 == ret)
    {
        printf("ioctl SIOCGIFFLAGS error!\n");
        return -4;
    }

    ifstruct.ifr_flags |= IFF_PROMISC;
    ret = ioctl(packet_socket, SIOCSIFFLAGS, &ifstruct);
    if(-1 == ret)
    {
        printf("ioctl SIOCSIFFLAGS error!\n");
        return -5;
    }

    ret = bind(packet_socket, (struct sockaddr *)&addr, sizeof(addr));
    if(-1 == ret)
    {
        perror("bind");
        return -6;
    }

    ret = ioctl(packet_socket, SIOCGIFHWADDR, &ifstruct);
    if(-1 == ret)
    {
        printf("ioctl SIOCGIFHWADDR error!\n");
        return -7;
    }
    
    char buf[1518];
    while(1)
    {
        read(packet_socket, buf, sizeof(buf));
        unsigned short eth_type = ntohs(*(unsigned short *)(buf + 12));
        if(0x0800 == eth_type)
            handle_ip((unsigned char *)(buf + 14));
        //print_eth_header((unsigned char *)buf);
    }

    return 0;        
}
