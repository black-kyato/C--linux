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
        printf("create a string\n");
        _str = NULL;
        capacity = 0;
    }
    String(const char *src)
    {
        printf("create a string through copy an array\n");
        _str = new char[strlen(src)+1+4];
        strcpy(_str+4,src);
        capacity = strlen(src)+1;
        *((int *)_str) = 1; 
    }
    String(const String& src)
    {
        printf("create a string through copy a string\n");
        _str = (char *)src.c_str();
        *((int*)_str) += 1;
        capacity = src.size();
    }
    String(String && tc)noexcept:_str(tc._str)
    {
        printf("create a string through move\n");
        tc._str = nullptr;
        capacity = tc.capacity;
    }
    String& operator=(String && tc)noexcept
    {
        printf("copy a string through move  \n");
        _str = tc._str; 
        tc._str = nullptr;
        this->capacity = tc.capacity;
        return *this;
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
String right(String c)
{
    return c;
}
using namespace std;
int main()
{
    String s3(right("hello"));
    String s4 = std::move(s3);
    cout<<s3<<endl;
    return 0;
}

