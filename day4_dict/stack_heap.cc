#include <iostream>
#include <fun.h>

using namespace std;
class StackOnly
{
private:
    int _data;
    void * operator new(size_t t)
    {
        return malloc(t);    
    }
    void * operator new[](size_t t)
    {
        
        return malloc(t*sizeof(StackOnly));    
    }
    void operator delete(void *g)
    {
        cout<<"delete"<<endl;
        free(g);
    }
    void operator delete[] (void *g)
    {
        free(g);
    }
public:
    StackOnly(int data):_data(data){}
};
class HeapOnly
{
private:
    ~HeapOnly()
    {

    }
    int _data;
public:
    HeapOnly(int data):_data(data){}
    void destroy()
    {
        free (this);
    }
};

int main()
{
    try
    {
        StackOnly tp(10);
 //       StackOnly *p=new StackOnly(10);
    }catch(exception e)
    {
        cout<<"new error,stack only"<<endl;
    }
    //HeapOnly p(10);
    HeapOnly *p=new HeapOnly(10);
    p->destroy();
    return 0;
}

