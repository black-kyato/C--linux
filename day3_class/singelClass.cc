#include <iostream>
using namespace std;

class singClass
{
public:
    static singClass * getObject(int test)
    {
        if(singClass::_p == NULL)
        {
            singClass::_p = new singClass(test);
        }
        else
        {
            cout<<"reuse"<<endl;
            singClass::_p->_test = test;
        }
        return singClass::_p;
    }
    static void destory()
    {
        
    }
private:
    singClass(int test):_test(test){
        cout<<"create"<<endl;
    }
    static singClass *_p;
    int _test;

};
singClass * singClass::_p = NULL;
int main()
{
    singClass *op1,*op2;
    op1= singClass::getObject(2);
    op2 = singClass::getObject(3);
    if(op1== op2)
    {
        cout <<"is equal"<<endl;
    }
    return 0;
}

