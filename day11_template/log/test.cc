#include <fun.h>
#include "cpplog.h"
using namespace std;

int maintest()
{
    MyLogINFO("start:%d",10);
    cout<<"test"<<endl;
    MyLogINFO("close:%d",20);
    return 0;


}
int main()
{
    int a =maintest();
    std::cout<<"a="<<a<<std::endl;
    return 0;
}

