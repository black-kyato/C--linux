#include <iostream>
#define Type int
using namespace std;
class Stack
{
private:
    Type* _top;
    int _length;
    int _capacity;
public:
    Stack(int capacity = 10) 
    {
        _top = new Type[capacity];
        _length = 0;
        _capacity = capacity;
    }
    ~Stack()
    {
        delete[] _top;
    }
    bool full()
    {
        if(_length == _capacity)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool empty()
    {
        if(this->_length == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void push(Type data)
    {
        if(this->full())
        {
            cout<<"栈空间已满，入栈失败\n";
            return;
        }
        else
        {
            _top[_length] = data;
            _length++;
        }
    }
    Type pop()
    {
        if(this->empty())
        {
            cout<<"栈空，出栈失败"<<endl;
            return 0;
        }
        else
        {
            return _top[--_length];
        }
    }
    Type top()
    {
        if(this->empty())
        {
            cout<<"栈空，无栈顶数据"<<endl;
            return 0;
        }
        else
        {
            return _top[_length-1];
        }
    }
};
int main()
{
    Stack s1;
    s1.push(10);
    s1.push(12);
    s1.push(14);
    cout<<"top="<<s1.top()<<endl;
    s1.pop();
    cout<<"top = "<<s1.top()<<endl;
    return 0;
}

