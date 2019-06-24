#include <fun.h>
#include "cpplog.h"
using namespace std;

int maintest()
{
    MyLogINFO("start");
    cout<<"test"<<endl;
    MyLogINFO("close");
    return 0;


}
int main()
{
    int a =maintest();
    std::cout<<"a="<<a<<std::endl;
    return 0;
}

