#include <pthread.h>
#include <stdio.h>
#include <list>
#include <unistd.h>
#include <semaphore.h>
using namespace std;

typedef struct
{
    int a;
}Task;

sem_t sem;
pthread_mutex_t mutex;

list<Task*> task;

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

void *send_func(void *ptr)
{
    int i=0;
    while(1)
    {
        usleep(100*1000);
        Task *t = new Task;
        t->a = ++i;
        {
            Auto_Lock lock(mutex);
            task.push_back(t);
        }
        sem_post(&sem);
    }

    return NULL;
}

void *recv_func(void *ptr)
{
    Task *t;
    while(1)
    {
        sem_wait(&sem);
        {
            Auto_Lock lock(mutex);
            if(0 == task.size())
                continue;
            t = *task.begin();
            task.pop_front();
        }
        printf("t->a is %d\n",t->a);
        delete t;
    }
    return NULL;
}

int main()
{
    sem_init(&sem, 0, 0);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex,&attr);

    pthread_t s_tid, r_tid;
    pthread_create(&s_tid, NULL, send_func, NULL);
    pthread_create(&r_tid, NULL, recv_func, NULL);
    
    pthread_join(s_tid, NULL);
    pthread_join(r_tid, NULL);

    return 0;
}
