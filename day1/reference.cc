#include <iostream>
using namespace std;

int main()
{
    int a = 123;
    int &r = a;
    cout << " r = "<<r<<endl;
    cout << "&r= "<<&r<<endl;
    cout <<"&a ="<<&a<<endl;
    cout<<"a="<<a<<endl;
    return 0;
}

