#include <iostream>
using namespace std;

class Test{
public:
    Test(const Test & t)
    {
        this->a = t.a;
        cout<<"!copy!\n";
    }
    int a;
    Test(int a):a(a){
        cout<<"creat\n";
    }
};
int main()
{
    Test ts(12);
    Test p=ts;
    //cout<<"tst:%d\n"<<p.a<<endl;
    return 0;
}

