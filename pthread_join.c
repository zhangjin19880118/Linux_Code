#include <pthread.h>
#include <stdio.h>

pthread_t tid,tid1;

void *thread_func1(void *ptr)
{   
    pthread_join(tid, NULL);

    return NULL;
}

void *thread_func(void *ptr)
{
    pthread_create(&tid1, NULL, thread_func1, NULL);

    sleep(1);

    return NULL;
}

int main()
{
    pthread_create(&tid, NULL, thread_func, NULL);

    void *ret;
    pthread_join(tid1, &ret);

    return 0;
}
