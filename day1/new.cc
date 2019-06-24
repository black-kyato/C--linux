#include <iostream>

using namespace std;

int main()
{
    int *i = new int;
    int *j = new int(20);
    int *a = new int[3]();
    cout<<"i ="<<i<<endl<<"j = "<<j<<endl;
    cout<<"*i ="<<*i<<endl<<"*j = "<<*j<<endl;
    cout <<"arry =:"<<a[0]<<" "<<a[1]<<" "<<a[2]<<endl;
    return 0;
}

