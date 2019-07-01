#include <iostream>
using namespace std;

template <class Type>
class Queue
{
private:
    Type *_queue;
    int _head;
    int _tail;
    int _capacity;
public:
    Queue(size_t capacity=10):_capacity(capacity){
        _head = 0;
        _tail = 0;
        _queue = new Type[capacity];
    }
    ~Queue()
    {
        delete [] _queue;
    }
    bool empty()
    {
        if(_head ==_tail)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool full()
    {
        if(_tail==_capacity)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void push(int data)
    {
        if(full())
        {
            cout<<"队列已满"<<endl;
            return;
        }
        else
        {
            _queue[_tail++]=data;
        }
    }
    Type pop()
    {
         Type temp = _queue[_head];
         for(int i = _head ; i<_tail; i++)
         {
            _queue[i]=_queue[i+1];
         }
         _tail--;
         return temp;
    }
    Type front()
    {
        return _queue[_head];
    }
    Type back()
    {
        return _queue[_tail];
    }
};
int main()
{
    Queue<int> q1;
    q1.push(14);
    q1.push(16);
    q1.push(18);
    cout<<q1.pop()<<endl;
    cout<<q1.pop()<<endl;
    return 0;
}

