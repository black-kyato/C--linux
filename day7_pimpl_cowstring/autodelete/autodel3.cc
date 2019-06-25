#include <fun.h>
using namespace std;
//atexit+pthread_once
class Single
{
private:
    static Single *sp;
    Single(){
    }
    ~Single(){}
public:
     static void destroy()
    {
        delete sp;
        std::cout<<"destroyed"<<std::endl;
    }
    static void init()
    {
        if(sp == NULL)
        {
            sp = new Single;
            atexit(destroy);
        }
    }
    static Single* getInstance()
    {
        pthread_once_t t = PTHREAD_ONCE_INIT;
        pthread_once(&t,init);
        return sp;
    }
};
Single* Single::sp = NULL;
int main()
{
    Single *test =Single::getInstance();
    cout << test <<endl;
    return 0;
}

