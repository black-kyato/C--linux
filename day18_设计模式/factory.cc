#include <fun.h>
using namespace std;

class Product
{
public:
    Product(){}
    virtual void use()=0;
    virtual ~Product(){}
};
class ProductA:public Product
{
public:
    ProductA()
    {
        cout<<"product A is produced"<<endl;
    }
    void use()
    {
        cout<<"use product A"<<endl;
    }
};
class ProductB:public Product
{
public:
    ProductB()
    {
        cout<<"product B is produced"<<endl;
    }
    void use()
    {
        cout<<"use product B"<<endl;
    }

};
class Factory
{
    virtual Product* produce()=0;
};
class FactoryA:public Factory
{
public:
    Product* produce()
    {
        return new ProductA;
    }
};
class FactoryB:public Factory
{
public:
    Product* produce()
    {
        return new ProductB;
    }
};
int main()
{
    FactoryA f1;
    FactoryB f2;
    f1.produce()->use();
    f2.produce()->use();
    return 0;
}

