#include <fun.h>
using namespace std;

class Abs
{
private:
    int i;
public:
    Abs(int i = 0):i(i)
    {
        cout<<"Abs born"<<endl;
    }
    virtual void display()=0;
};
class base:public Abs
{
public:
    void display()
    {
        cout<<"this is base"<<endl;
    }
    base(int i):Abs(i){
        cout<<"base is born"<<endl;
    }
private:
};
int main()
{
    base b(10);
    b.display();
    return 0;
}

