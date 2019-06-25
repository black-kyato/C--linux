#include <fun.h>

class String
{
    class CharProxy{
    public:
        CharProxy(String &str, int index):theString(str),charIndex(index){}
        CharProxy& operator=(const CharProxy&rhs)
        {
            if(theString.isSingle())
            {
                theString._str[charIndex+4] = rhs.theString.c_str()[rhs.Index()+4];
                return *this;
            }
            String *stp = new String(theString._str+4);
            theString = *stp;
            theString._str[charIndex+4] = rhs.theString._str[rhs.Index()+4];
            stp->deleteString();
            return *this;
        }
        CharProxy& operator=(char c)
        {
            if(theString.isSingle())
            {
                theString._str[charIndex+4] = c;
                return *this;
            }
            String *stp = new String(theString._str+4);
            //std::cout<<"等于复制\n";
            theString = *stp;
            theString._str[charIndex+4] = c;
            stp->deleteString();
            return *this;
        }
        operator char()const
        {
            return theString.c_str()[charIndex+4];
        }
        int Index()const
        {
            return charIndex;
        }
    private:
        String &theString;
        int charIndex;
    };
    friend class CharProxy;
private:
    char *_str;
    size_t capacity;
public:
    int getNum()
    {
        if(_str == NULL)
        {
            return 0;
        }
        return *((int*)_str);
    }
    bool isSingle()
    {
        if(*((int*)_str)==1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    String()
    {
        _str = NULL;
        capacity = 0;
    }
    String(const char *src)
    {
        _str = new char[strlen(src)+1+4];
        strcpy(_str+4,src);
        capacity = strlen(src)+1;
        *((int *)_str) = 1; 
    }
    String(const String& src)
    {
        _str = (char *)src.c_str();
        *((int*)_str) += 1;
        capacity = src.size();
    }
    void deleteString()
    {
        if(_str== NULL)
            return ;
        if(*((int*)_str) > 1)
        {
            *((int*)_str) -= 1;
        }
        else
        {
           delete  [] _str;
        }
    }
    ~String()
    {
        std::cout<<"析构~\n";
        deleteString();
    }
    String &operator=(const String &src)
    {
        //std::cout<<"引用复制\n";
        deleteString();
        capacity = src.size();
        _str = (char *)src.c_str();
        *((int*)_str) += 1;
        return *this;
    }
    String &operator=(const char *src)
    {
        //std::cout<<"字符串复制\n";
        deleteString();
        capacity = strlen(src)+1;
        _str = new char [capacity+4];
        *((int*)_str) = 1;
        strcpy(_str+4,src);
        return *this;
    }
    const CharProxy operator[](std::size_t index) const
    {
        return CharProxy(const_cast<String&>(*this),index); 
    }
    CharProxy operator[](std::size_t index)
    {
        return CharProxy(*this,index);

    }

    std::size_t size() const
    {
        return capacity;
    }
    const char* c_str() const
    {
        return  _str;
    }

    friend std::ostream &operator<<(std::ostream &os, const String &s);
};
std::ostream &operator<<(std::ostream &os, const String &s)
{
    if(s.c_str()==NULL)
    {
        os<<0;
        return os;
    }
    os<<(s._str+4);
    return os;
}
using namespace std;
int main()
{
    String s1("hello");
    String s2;
    cout<<"s1="<<s1<<",&s1="<<(long)s1.c_str()<<" num of s1="<<s1.getNum()<<endl;
    cout<<"s2="<<s2<<",&s2="<<(long)s2.c_str()<<" num of s2="<<s2.getNum()<<endl;
    s2 = s1;
    cout<<"s1="<<s1<<",&s1="<<(long)s1.c_str()<<" num of s1="<<s1.getNum()<<endl;
    cout<<"s2="<<s2<<",&s2="<<(long)s2.c_str()<<" num of s2="<<s2.getNum()<<endl;
    cout<<"s2[1]="<<s2[1]<<endl;
    s2[1] ='a';
    cout<<"s2[1] ='a'"<<endl;
    cout<<"s1="<<s1<<",&s1="<<(long)s1.c_str()<<" num of s1="<<s1.getNum()<<endl;
    cout<<"s2="<<s2<<",&s2="<<(long)s2.c_str()<<" num of s2="<<s2.getNum()<<endl;
    cout<<"s2[4] ='e'"<<endl;
    s2[4]='e';
    cout<<"s1="<<s1<<",&s1="<<(long)s1.c_str()<<" num of s1="<<s1.getNum()<<endl;
    cout<<"s2="<<s2<<",&s2="<<(long)s2.c_str()<<" num of s2="<<s2.getNum()<<endl;
    s2[4] = s1[1];
    cout<<"s2[4] = s1[1]"<<endl;
    cout<<"s1="<<s1<<",&s1="<<(long)s1.c_str()<<" num of s1="<<s1.getNum()<<endl;
    cout<<"s2="<<s2<<",&s2="<<(long)s2.c_str()<<" num of s2="<<s2.getNum()<<endl;
    return 0;
}

