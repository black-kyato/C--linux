#include <fun.h>
using namespace std;

class Person
{
private:
    string _name;
    int _age;
public:
    Person(const string name,int age):_name(name),_age(age){}
    void display()
    {
        cout<<"姓名:"<<_name<<"\t年龄:"<<_age<<endl;
    }
};

class Employee:public Person
{
private:
    string _depart;
    double _salary;
public:
    Employee(string name,string department,int age,double salary):Person(name,age),_depart(department),_salary(salary){}
    void display()
    {
        cout<<"员工信息:"<<endl;
        this->Person::display();
        cout<<"部门:"<<_depart<<"\t工资:"<<_salary<<endl;
    }
    double salary()
    {
        return _salary;
    }
};
int main()
{
    Employee e1("龙哥","C&linux",32,30000.0),e2("浩哥","C++",25,20000.0),e3("风华哥","老板",33,200000);
    e1.display();
    e2.display();
    e3.display();
    cout<<"平均工资:"<<(e1.salary()+e2.salary()+e3.salary())/3.0<<endl;
    return 0;
}

