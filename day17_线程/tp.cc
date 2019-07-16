#include <fun.h>
#include "mutcond.h"
#include "nocopy.h"



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
    virtual void* run() = 0;
public:
    Thread():_pid(0),_running(false){}
    ~Thread()
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

class Task
{
private:
    string taskName;
    void *data;
public:
    Task(string Name):taskName(Name){}
    virtual void getData(void *Data) = 0;
    void* getData()
    {
        return data;
    }
};
template<class MyThread>
class ThreadPool
{
private:
    int threadNum;
    deque<Task*> tasklist;
    vector<unique_ptr<MyThread>> threadlist;
public:
    ThreadPool(int num):threadNum(num){}
    void start()
    {
        for(int i = 0;i<threadNum;i++)
        {
            unique_ptr<MyThread> tp(new MyThread);
            threadlist.push_back(tp);
            tp->run();
        }
    }
    void addTask(Task * t)
    {
        tasklist.push_back(t);
    }
};
