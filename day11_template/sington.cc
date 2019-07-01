#include <fun.h>
using namespace std;

template<class Type>
class Sington
{
private:
    static Sington* _datap;
    Type* _data = NULL;
    Sington(...)
    {
    }
    ~Sington()
    {
    }
public:
    template<class ...ARG>
    static Sington * getInstance(ARG ...args)
    {
        if(_datap == NULL)
        {
            _datap = new Sington;
            atexit(destory);           
        }
        else
        {
            delete _datap;
            _datap = new Sington;
        }
        Type td(args...);
        _datap->_data = new Type; 
        *(_datap->_data)= td;
        return _datap;
    }
    static void destory()
    {
        delete _datap->_data;
        delete _datap;
    }
    void display()
    {
        cout<<_data<<endl;;
    }
};
template<class T>
Sington<T>* Sington<T>::_datap = NULL;
int main()
{
    Sington<int> * tp = Sington<int>::getInstance(24);
    tp->display();
    Sington<string> *sp = Sington<string>::getInstance("hello world");
    sp->display();
    return 0;
}

