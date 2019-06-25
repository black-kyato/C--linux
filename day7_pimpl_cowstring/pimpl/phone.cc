#include <fun.h>
#include "phone.h"
using namespace std;

class phone::_phone
{
private:
    string _name;
    int _price;
public:
    _phone(string name,int price){
        _name = name;
        _price = price;
    }
    _phone()
    {
    }
    int Price(){
        return _price;
    }
    string Name()
    {
        return _name;
    }
    void setPrice(int price)
    {
        _price = price;
    }
};
phone::phone(){}
phone::phone(string name,int price):mp(new _phone(name,price)){}
phone::~phone(){delete mp;}
int phone::getPrice()
{
    return mp->Price();
}
string phone::getName()
{
    return mp->Name();
}
void phone::setPrice(int price)
{
    mp->setPrice(price);
}
int main()
{
    phone xiaoMi("redMi k20",1999);
    phone xiaoMi9("xiaoMi9",2799);
    xiaoMi9.setPrice(2599);
    cout<<xiaoMi9.getName()<<":"<<xiaoMi9.getPrice()<<"￥"<<endl;
    return 0;
}

