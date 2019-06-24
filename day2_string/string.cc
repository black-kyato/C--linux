#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

class String
{
public:
    String(){
        _pstr = NULL;
    }
    String(const char *pstr)
    {
        cout <<"before new"<<endl;
        _pstr = new char[strlen(pstr)+1];
        cout<<"new"<<endl;
        strcpy(_pstr,pstr);
        cout<<"char creat"<<endl;
    }
    String(const String & rhs)
    {
        _pstr = new char[rhs.getLen()+1];
        strcpy(_pstr,rhs.getSp());
        cout<<"& create"<<endl;
    }
    String & operator=(const String & rhs)
    {
        if(strlen(_pstr)<rhs.getLen())
        {
            delete[] _pstr;
            _pstr = new char[rhs.getLen()+1];
        }
        strcpy(_pstr,rhs.getSp());
        cout<<"=create"<<endl;
        return *this;
    }
    size_t getLen()const
    {
        return strlen(_pstr);
    }
    char * getSp() const
    {
        return _pstr;
    }
    ~String(){
        delete[] _pstr;
    }

    void print()
    {
        if(_pstr == NULL)
        {
            cout<<"string is null"<<endl;
        }
        else
        {
        puts(_pstr);
        }
    }

private:
    char * _pstr;
};
int main()
{
    String str1;
    str1.print();
    String str2 = "Hello,world";
    String str3("wangdao");

    str2.print();       
    str3.print();   

    String str4 = str3;
    str4.print();

    str4 = str2;
    str4.print();

    return 0;
    return 0;
}

