#include <fun.h>

class String
{
private:
    char *_str;
    size_t capacity;
public:
    String()
    {
        _str = NULL;
        capacity = 0;
    }
    String(const char *src)
    {
        _str = new char[strlen(src)+1];
        strcpy(_str,src);
        capacity = strlen(src)+1;
    }
    String(const String& src)
    {
        _str = new char[src.size()];
        strcpy(_str,src.c_str());
        capacity = src.size();
    }
    ~String()
    {
        delete [] _str;
    }
    String &operator=(const String &src)
    {
        if(capacity>=src.size())
        {
            strcpy(_str,src.c_str());
        }
        else
        {
            delete  [] _str;
            _str = new char [src.size()];
            capacity = src.size(); 
            strcpy(_str,src.c_str());
        }
        return *this;
    }
    String &operator=(const char *src)
    {
        if(capacity>strlen(src))
        {
            strcpy(_str,src);
        }
        else
        {
            delete  [] _str;
            capacity = strlen(src)+1;
            _str = new char [capacity];
            strcpy(_str,src);
        }
        return *this;
    }

    String &operator+=(const String &ads)
    {
       if(capacity<strlen(_str)+ads.size()) 
       {
            String str(_str);
            delete  [] _str;
            capacity = strlen(str.c_str())+ads.size();
            _str = new char[capacity];
            sprintf(_str,"%s%s",str.c_str(),ads.c_str());
       }
       else
       {
            strcat(_str,ads.c_str());
       }
       return *this;
    }
    String &operator+=(const char *ads)
    {
       if(capacity<(strlen(ads)+strlen(_str))) 
       {
            String str(_str);
            delete  [] _str;
            capacity = strlen(str.c_str())+strlen(ads)+1;
            _str = new char[capacity];
            sprintf(_str,"%s%s",str.c_str(),ads);
       }
       else
       {
            strcat(_str,ads);
       }
       return *this;
    }
    char &operator[](std::size_t index)
    {
        return _str[index];
    }
    const char &operator[](std::size_t index) const
    {
        return _str[index];

    }

    std::size_t size() const
    {
        return capacity;
    }
    const char* c_str() const
    {
        return  _str;
    }

    friend bool operator==(const String &, const String &);
    friend bool operator!=(const String &, const String &);

    friend bool operator<(const String &, const String &);
    friend bool operator>(const String &, const String &);
    friend bool operator<=(const String &, const String &);
    friend bool operator>=(const String &, const String &);

    friend std::ostream &operator<<(std::ostream &os, const String &s);
    friend std::istream &operator>>(std::istream &is, String &s);
};
bool operator==(const String &a, const String &b)
{
    if(strcmp(a._str,b._str)==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool operator!=(const String &a, const String &b)
{
    if(strcmp(a._str,b._str))
    {
        return true;
    }
    else
    {
        return false;
    }
}
                                                                        
bool operator<(const String &a, const String &b)
{
    if(strcmp(a._str,b._str)<0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool operator>(const String &a, const String &b)
{
    if(strcmp(a._str,b._str)>0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool operator<=(const String &a, const String &b)
{
    if(strcmp(a._str,b._str)<=0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool operator>=(const String &a, const String &b)
{
    if(strcmp(a._str,b._str)>=0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
                                                                    
std::ostream &operator<<(std::ostream &os, const String &s)
{
    os<<s._str;
    return os;
}
std::istream &operator>>(std::istream &is, String &s)
{
    is>>s._str;
    return is;
}
String operator+(const String &a, const String &b)
{
    String str;
    str+=a;
    str+=b;
    return str;
}
String operator+(const String &a, const char *b)
{
    String str;
    str+=a;
    str+=b;
    return str;
}
String operator+(const char *a, const String &b)
{
    String str;
    str+=a;
    str+=b;
    return str;
}
using namespace std;
int main()
{   
    String a("hello");
    cout<<a<<endl;
    a+=" world";
    cout<<a<<endl;
    return 0;
}

