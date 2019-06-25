#include <fun.h>
using namespace std;
//静态变量
class Single
{
class Recycle
{
public:
    ~Recycle()
    {
        Single::destroy();
    }
};
private:
    static Recycle rec;
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
        }
        return sp;
    }
};
Single::Recycle Single::rec;
Single* Single::sp = NULL;
int main()
{
    Single *test =Single::getInstance();
    cout << test <<endl;
    return 0;
}

