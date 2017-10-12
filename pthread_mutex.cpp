#include <pthread.h>
#include <stdio.h>

int a = 0;
pthread_mutex_t mutex;

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

void add()
{
   Auto_Lock lock(mutex); 
   ++a;
}

void *thread_func(void *ptr)
{   
    int i;
    for(i=0;i<100000;++i)
    {
        add();
    }

    return NULL;
}

int main()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&mutex, &attr);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    
    int i;
    for(i=0;i<100000;++i)
    {
        Auto_Lock lock(mutex); 
        ++a;
    }

    pthread_join(tid,NULL);
   
    printf("%d\n",a);

    return 0;
}
