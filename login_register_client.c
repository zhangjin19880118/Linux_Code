#include "network.h"

int main()
{
    char cmd[16];
    char user[32];
    char password[32];

    signal(SIGPIPE, SIG_IGN);
    while(1)
    {
        int fd = myConnect2(9988, "127.0.0.1");
        if(-1 == fd)
        {
            printf("connect error!\n");
            return -1;
        }

        printf("please input cmd:");
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd) -1] = 0;

        printf("please input your name:");
        fgets(user, sizeof(user), stdin);
        user[strlen(user) - 1] = 0;

        printf("please input your password:");
        fgets(password, sizeof(password), stdin);
        password[strlen(password) - 1] = 0;
        
        if(!(strlen(cmd) || strlen(user) || strlen(password)))
        {
            printf("all input string is null!\n");
            return -2;
        }
        char packet[1024];
        sprintf(packet, "%s|%s|%s", cmd, user, password);
        myWriteBuf(fd, packet);

        char *resp = myReadBuf(fd);
        printf("server resp is %s\n", resp);

        free(resp);

        close(fd);
    }

    return 0;
}
