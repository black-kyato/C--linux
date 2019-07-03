#include <fun.h>
using namespace std;

bool isRight(char c)
{
    if(c==')'||c==']'||c=='}')
    {
        return true;
    }
    else
        return false;
}
bool isLeft(char c)
{
    if(c=='('||c=='['||c=='{')
    {
        return true;
    }
    else
        return false;
}
bool isMatch(char a,char b)
{
    if(a=='('&&b ==')')
    {
        return true;
    }
    if(a=='['&& b ==']')
    {
        return true;
    }
    if(a=='{'&&b =='}')
    {
        return true;
    }
    return false;
}
int main()
{
    stack<char> test;
    cout<<"input:"<<endl;
    char b;
    bool res = true;
    string bracket;
    cin>>bracket;
    for(auto c:bracket)
    {
        if(isLeft(c))
        {
            test.push(c);
        }
        if(isRight(c))
        {
            if(test.empty())
            {
                res = false;
                break;
            }
            b = test.top();
            test.pop();
            if(!isMatch(b,c))
            {
                res = false;
                break;
            }
        }
    }
    cout<<boolalpha<<res<<endl;
    return 0;
}

