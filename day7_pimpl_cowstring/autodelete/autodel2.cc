#include <fun.h>
using namespace std;
//atexit+静态方法
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
    static Single* getInstance()
    {
        if(sp == NULL)
        {
            sp = new Single;
            atexit(destroy);
        }
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

