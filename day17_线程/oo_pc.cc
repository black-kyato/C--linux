#include <fun.h>
#include "mutcond.h"
#include "nocopy.h"



using namespace std;

template<class T>
 class Storage
{
private:
    deque<T> StoreHouse;
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
    T getCargo()
    {
        if(StoreHouse.size()== 0)
        {
            _empty.wait();
        }
        _mutex.lock();
       T tmp = StoreHouse.front();
       StoreHouse.pop_front();
       _full.notifyall();
       _mutex.unlock();
       return tmp;
    }
    bool storeCargo(T cargo = 0)
    {
        if(capacity==StoreHouse.size())
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
template<class T>
class Producer:public Thread
{
private:
    int plan;
    Storage<T>* _buffer;
    void * run()
    {
        int i = 0;
        while(i<plan)
        {
            _buffer->storeCargo();
            ++i;
           cout<<"produced "<<i<<" product"<<endl;
           cout<<_buffer->howMany()<<" products left"<<endl;
        }
    }
public:
    void setPlan(int newPlan)
    {
        plan = newPlan;
    }
    Producer(int number,Storage<T>* buffer):plan(number),_buffer(buffer){}
};
template<class T>
class  Consumer:public Thread
{
private:
    int plan;
    string name;
    Storage<T>* _buffer;
    void * run()
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
public:
    void setPlan(int newPlan)
    {
        plan = newPlan;
    }
    Consumer(int number,Storage<T>* buffer):plan(number),_buffer(buffer){}
    void setName(string name)
    {
        this->name = name;
    }
};
int main()
{
    Storage<int> buff(10);
    Producer<int> p(20,&buff);
    Consumer<int> c1(5,&buff);
    Consumer<int> c2(15,&buff);
    c1.setName("c1");
    c2.setName("c2");
    p.start();
    c1.start();
    c2.start();
    p.stop();
    c1.stop();
    c2.stop();
    return 0;
}

