#pragma  once
#include <iostream>
#include <fun.h>
#include "nocopy.h"
using namespace std;

class MutexLock:Nocopy
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
class Condition:Nocopy
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
        pthread_cond_wait(&_cond,&_mutex);
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
