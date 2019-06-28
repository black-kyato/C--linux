#include <fun.h>
using namespace std;

class base
{
public:
    virtual void display()
    {
        cout<<"I am  base display\n";
    }
    void display(int i)
    {
        cout<<"base i="<<i<<endl;
    }
    void show()
    {
        cout<<"this is base show"<<endl;
        display();
    }
};
class derived:public base
{
public:
    void display( )
    {
        cout<<"I am derived display\n";
    }
    void display(int i)
    {
        cout <<"test"<<i<<endl;
    }
    void show()
    {
        cout<<"this is derived show"<<endl;
        display();
    }
};
int main()
{
    base *b;
    derived d;
    b=&d;
    b->base::display();
    b->display(10);
    b->show();
    return 0;
}

