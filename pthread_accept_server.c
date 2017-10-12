#include "network.h"

int server;
void *pthread_func(void *ptr)
{   
    while(1)
    {
        int newfd = accept(server, NULL, NULL);
        printf("newfd = %02d, tid = 0x%x\n", newfd, (int)pthread_self());
        close(newfd);
    }
    
    return NULL;
}

int main()
{
    server = myServer(8888, "0.0.0.0");
    if(-1 == server)
    {
        printf("create server error!\n");
        return -1;
    }

    pthread_t tid1, tid2, tid3, tid4, tid5;
    pthread_create(&tid1, NULL, pthread_func, NULL);
    pthread_create(&tid2, NULL, pthread_func, NULL);
    pthread_create(&tid3, NULL, pthread_func, NULL);
    pthread_create(&tid4, NULL, pthread_func, NULL);
    pthread_create(&tid5, NULL, pthread_func, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);

    return 0;
}
