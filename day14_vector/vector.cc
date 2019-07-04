#include <fun.h>
using namespace std;

template<class T>
class MyVector
{
private:
    static std::allocator<T> _alloc;
    T * _start;      //指向数组中的第一个元素
    T * _finish; //指向最后一个实际元素之后的那个元素
    T * _end_of_storage;        //指向数组本身之后的位置
    void reallocate(int n)
    {
        T* newStart = _alloc.allocate(n);
       
        int length = size();

        memcpy(newStart,_start,length*sizeof(T));
        
        for(int i = 0;i<size();i++)
        {
            _alloc.destroy(_start+i);
        }
        _alloc.deallocate(_start,capacity());

        _start = newStart;
        _finish = _start + length;
        _end_of_storage = _start + n;
    }
public:
    MyVector()
    {
        _start = NULL;
        _finish = NULL;
        _end_of_storage = NULL;
    }
    ~MyVector()
    {
        for(int i = 0;i<size();i++)
        {
            _alloc.destroy(_start+i);
        }
        _alloc.deallocate(_start,capacity());
    }

    void push_back(const T &data)
    {
        if(size()<capacity())
        {
            memcpy(_finish,&data,sizeof(data));
            ++_finish;
        }
        else
        {
            if(capacity()<100)
            {
                reallocate(capacity()+10);
            }
            else
            {
                reallocate(capacity()*2);
            }
            memcpy(_finish,&data,sizeof(data));
            ++_finish;
        }
    }
    void pop_back()
    {
        if(size()>0)
        {
            _alloc.destroy(_finish--);
        }
        else
            cout<<"error,empty vector"<<endl;
    }
    T& operator[](int n)
    {
        if(n>=size())
        {
            cout<<"array out of range"<<endl;
            exit(-1);
        }
        else
        {
            return *(_start+n);
        }
    }
    int size()
    {
        return (_finish - _start);
    }
    int capacity()
    {
        return (_end_of_storage - _start);
    }
};
template<class T>
std::allocator<T> MyVector<T>::_alloc;

int main()
{
    MyVector<int> test;
    test.push_back(24);
    test.push_back(10);
    test.push_back(7);
    cout<<test.size()<<endl;
    cout<<test.capacity()<<endl;
    cout<<test[0]<<endl;
    cout<<test[1]<<endl;
    cout<<test[2]<<endl;
    return 0;
}

