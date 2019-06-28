#include <fun.h>
using namespace std;

class transportation
{
public:
    transportation(int i)
    {
        cout<<"transportation "<<i<<" born"<<endl;
    }
};
class car:virtual public transportation
{
public:
    car(int i):transportation(i)
    {
        cout<<"car "<<i<<" born"<<endl;
    }
};
class plane:virtual public transportation
{
public:
    plane(int i):transportation(i)
    {
        cout<<"plane "<<i<<" born"<<endl;
    }
};
class carplane:public car,public plane
{
public:
    carplane(int i ):transportation(i+1),car(i+2),plane(i+3)
    {
        cout<<"carplane "<<i<<" born"<<endl;
    }
};
int main()
{
    carplane t(0);
    return 0;
}

