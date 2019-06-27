#include <fun.h>
#define PI 3.14159
using namespace std;

class Circle
{
private:
    double _r;
public:
    Circle()
    {
        _r = 0;
    }
    Circle(double r):_r(r){}
    Circle(Circle &c)
    {
        _r = c._r;
    }
    double getArea()
    {
        return PI*_r*_r;
    }
    double getPerimeter()
    {
        return 2*PI*_r;
    }
    void show()
    {
        cout<<"圆:\n半径="<<_r<<" 周长="<<getPerimeter()<<" 面积="<<getArea()<<endl;
    }
};

class Cylinder:public Circle
{
private:
    double _h;
public:
    Cylinder():Circle()
    {
        _h = 0;
    }
    Cylinder(double r,double h):Circle(r)
    {
        _h = h;
    }
    double getVolume()
    {
        return _h*this->getArea();
    }
    void showVolume()
    {
        cout<<"圆柱体:\n体积="<<getVolume()<<endl;
    }
};
int main()
{
    double r,h;
    cout << "请依次输入圆的半径和圆柱体的高度"<<endl;
    cin>>r>>h;
    Cylinder c(r,h);
    c.show();
    c.showVolume();
    return 0;
}

