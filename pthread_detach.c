#include <pthread.h>
#include <stdio.h>

void *thread_func(void *ptr)
{
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    pthread_detach(tid);

    int ret = pthread_join(tid, NULL);
    printf("ret is %d\n",ret);

    return 0;
}
