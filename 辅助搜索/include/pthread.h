#pragma once
#include <fun.h>
#include "mutcond.h"
#include "nocopy.h"
#include "task.h"


using namespace std;
//抽象线程类,继承类需要重写run函数
class Thread:Nocopy
{
protected:
    pthread_t _pid;
    bool _running;
    static void * pthreadFunction(void *vg)
    {
        Thread* pt = static_cast<Thread*>(vg);
        pt->run();
        return (void*)0;
    }
    virtual void run() = 0;
public:
    Thread():_pid(0),_running(false){}
    virtual ~Thread()
    {
        if(_running)
        {
            pthread_detach(_pid);
            _running = false;
        }
    }
    int pid()
    {
        return _pid;
    }
    bool isRun()
    {
        return _running;
    }
    void  start()
    {
        int ret =  pthread_create(&_pid,nullptr, pthreadFunction, this);
        THREAD_CHECK(ret,"pthread_create");
        _running = true;
    }
    void stop()
    {
        if(!_running)
        {
            return;
        }
        pthread_join(_pid,NULL);
        _running = false;
    }
};
class WorkThread:public Thread
{
protected:
    TaskQueue* taskList;
    void run();
public:
    WorkThread(TaskQueue* queue):taskList(queue){}

};
class ThreadPool
{
protected:
    int amount;
    vector<Thread*> workers; 
    bool start;
public:
    ThreadPool(int scale):amount(scale),start(false){}
    virtual void startPool() = 0;
    virtual void stopPool()
    {
        for(auto& child:workers)
        {
            child->stop();

        }

    }
    int Scale()
    {
        return amount;

    }
    void setScale(int scale)
    {
        amount = scale;

    }
    virtual ~ThreadPool()
    {
        this->stopPool();
        for(auto worker:workers)
        {
            delete worker;

        }

    }

};
class MyPool:public ThreadPool
{
protected:
    TaskQueue *taskQ;
public:
    MyPool(int scale,TaskQueue* tasks):ThreadPool(scale),taskQ(tasks){}
    void setTask(TaskQueue * tasks)
    {
        taskQ = tasks;

    }
    void startPool()
    {
        for(int i = 0;i<amount;i++)
        {
            Thread *pt = new WorkThread(taskQ);
            workers.push_back(pt);
            pt->start();

        }

    }

};
