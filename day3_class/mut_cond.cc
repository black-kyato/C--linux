#include <iostream>
#include <fun.h>
using namespace std;

class MutexLock
{
private:
    pthread_mutex_t _mutex;
public:
    MutexLock()
    {
        pthread_mutex_init(&_mutex,NULL);
    }
    ~MutexLock()
    {
        pthread_mutex_destroy(&_mutex);
    }
    void lock()
    {
        pthread_mutex_lock(&_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }
};
class Condition
{
private:
    pthread_cond_t _cond;
    pthread_mutex_t _mutex;
public:
    Condition()
    {
        pthread_mutex_init(&_mutex,NULL);
        pthread_cond_init(&_cond,NULL);
    }
    ~Condition()
    {
        pthread_cond_destroy(&_cond);
        pthread_mutex_destroy(&_mutex);
    }
    void wait()
    {
        pthread_mutex_lock(&_mutex);
        cout<<"is waiting"<<endl;
        pthread_cond_wait(&_cond,&_mutex);
        cout<<"stop waiting"<<endl;
        pthread_mutex_unlock(&_mutex);
    }
    void notify()
    {
        pthread_cond_signal(&_cond);
    }
    void notifyall()
    {
        pthread_cond_broadcast(&_cond);
    }
};
MutexLock lock;
Condition cond;
void * test1(void *g)
{
    cout<<"test1: this is test1"<<endl;
    cout<<"test1: waiting for signal from main program"<<endl;
    cond.wait();
    cout<<"test1: signal come"<<endl;
    pthread_exit(g);
}
void * test2(void *g)
{
    cout<<"test2: this is thread "<<(long)g<<", thread "<<(long)g<<" will lock"<<endl;
    lock.lock();
    cout<<"test2: thread "<<(long)g<<" get the lock and sleep 1s"<<endl;
    sleep(1);
    lock.unlock();
    cout<<"test2: thread "<<(long)g<<" free the lock"<<endl;
    pthread_exit(g);
}
int main()
{
    pthread_t p1,p2;
    cout<<"main: create a test1"<<endl;
    pthread_create(&p1,NULL,test1,NULL);
    sleep(1);
    cout<<"main: notify test1"<<endl;
    cond.notify();
    sleep(1);
    cout<<"main: create two test1"<<endl;
    pthread_create(&p1,NULL,test1,NULL);
    pthread_create(&p1,NULL,test1,NULL);
    sleep(2);
    cout<<"main: notify all test1"<<endl;
    cond.notifyall();
    cout<<"main: create three test2"<<endl;
    pthread_create(&p2,NULL,test2,(void *)1);
    pthread_create(&p2,NULL,test2,(void *)2);
    pthread_create(&p2,NULL,test2,(void *)3);
    sleep(5);
    return 0;
}

