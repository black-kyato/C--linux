#include <fun.h>
#include "mutcond.h"
#include "nocopy.h"



using namespace std;

 class Storage
{
private:
    deque<int> StoreHouse;
    int capacity;
    MutexLock _mutex;
    Condition _empty;
    Condition _full;
public:
    Storage(int cap=10):capacity(cap){}
    void setCapacty(int newCapacity)
    {
        capacity = newCapacity;
    }
    int howMany()
    {
        return StoreHouse.size();
    }
    bool isEmpty()
    {
        return StoreHouse.empty();
    }
    int getCargo()
    {
        if(StoreHouse.size()<= 0)
        {
            _empty.wait();
        }
        _mutex.lock();
       int tmp = StoreHouse.front();
       StoreHouse.pop_front();
       _full.notifyall();
       _mutex.unlock();
       return tmp;
    }
    bool storeCargo()
    {
        int cargo;
        if(capacity==(int)StoreHouse.size())
        {
            _full.wait();
        }
        StoreHouse.push_back(cargo);
        _empty.notifyall();
        return true;
    }
};
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
    std::function<void ()> run;
public:
    Thread(std::function<void ()> run):_pid(0),_running(false),run(run){}
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
class Producer
{
public:
    static void  run(int plan,Storage* buffer)
    {
        int i = 0;
        while(i<plan)
        {
            buffer->storeCargo();
            ++i;
           cout<<"produced "<<i<<" product"<<endl;
           cout<<buffer->howMany()<<" products left"<<endl;
        }
    }
};
class  Consumer
{
public:
    static void  run(int plan,Storage* _buffer,string name)
    {
        int i = 0;
        while(i<plan)
        {
            _buffer->getCargo();
            ++i;
           cout<<name<<" consumed "<<i<<" product"<<endl;
           cout<<_buffer->howMany()<<" products left"<<endl;
        }
    }
};
int main()
{
    Storage buff(10);
    Thread p1(bind(Producer::run,20,&buff));
    Thread c1(bind(Consumer::run,15,&buff,"c1"));
    Thread c2(bind(Consumer::run,5,&buff,"c2"));
    p1.start();
    c1.start();
    c2.start();
    p1.stop();
    c1.stop();
    c2.stop();
    return 0;
}

