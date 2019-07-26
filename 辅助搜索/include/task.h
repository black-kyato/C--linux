#pragma once
#include <fun.h>
#include "mutcond.h"
class TaskQueue
{
private:
    deque<int> _queue;
    int _capacity;
    MutexLock _mutex;
    Condition _cond;
public:
    TaskQueue(int scale):_capacity(scale){}
    void lock()
    {
        _mutex.lock();

    }
    void unlock()
    {
        _mutex.unlock();

    }
    void wait()
    {
        _cond.wait();

    }
    void notify()
    {
        _cond.notify();

    }
    void notifyAll()
    {
        _cond.notifyall();

    }
    bool addTask(int clientfd)
    {
        if((int)_queue.size()>=_capacity)
        {
            std::cout<<"服务器满载"<<endl;
            return 0;

        }
        this->lock();
        _queue.push_back(clientfd);
        this->unlock();
        return 1;

    }
    bool getTask(int& clientfd)
    {
        this->lock();
        if(_queue.size()<=0)
        {
            this->unlock();
            return false;
        }
        clientfd = _queue.front();
        _queue.pop_front();
        this->unlock();
        return true;
    }
};
