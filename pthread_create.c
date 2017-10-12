#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void foo()
{
    pthread_t tid = pthread_self();
    
    while(1)
    {
        printf("in thread %d\n", (int)tid);
        usleep(10*1000);
    }
}

void *thread_func(void *ptr)
{
    pthread_exit(0);
    //exit(0);

    foo();

    return NULL;
}

int main()
{

    pthread_t tid1;
    pthread_create(&tid1, NULL, thread_func, NULL);
    
    pthread_t tid2;
    pthread_create(&tid2, NULL ,thread_func, NULL);
    
    sleep(1);

    if(!pthread_equal(tid1,tid2))
        printf("defferent thread!\n");
   
    foo();
        
    return 0;
}
