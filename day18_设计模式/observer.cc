#include <fun.h>
using namespace std;

class Observer;
class Subject
{
protected:
    list<Observer*> _observers;
public:
    virtual void attach(Observer *) =0;
    virtual void detach(Observer *) =0;
    virtual void notify() = 0;
    virtual int getState() = 0;
    virtual void setState(int) = 0;
};
class Observer
{
protected:
    Subject * _subject;
public:
    Observer(Subject* sp):_subject(sp){}
    virtual void update() = 0;
};
class ConcreteSubject:public Subject
{
private:
    int _state;
public:
    ConcreteSubject(int state = 0)
    {
        _state = state;
    }
    int getState(){
        return _state;
    }
    void setState(int s)
    {
        _state = s;
    }
    void notify()
    {
        for(auto a:_observers)
        {
            a->update();
        }
    }
    void attach(Observer* op)
    {
        _observers.push_back(op);
    }
    void detach(Observer* op)
    {
        _observers.remove(op);
    }
};
class Observer1:public Observer
{
public:
    Observer1(Subject* sp):Observer(sp){}
    void update()
    {
        cout<<"Observer1 get the state:"<<_subject->getState()<<endl;
    }
};
class Observer2:public Observer
{
public:
    Observer2(Subject* sp):Observer(sp){}
    void update()
    {
        cout<<"Observer2 get the state:"<<_subject->getState()<<endl;
    }
};
int main()
{
    ConcreteSubject sb(10);
    Observer1 *o1 = new Observer1(&sb);
    Observer2 *o2 = new Observer2(&sb);
    sb.attach(o1);
    sb.attach(o2);
    sb.notify();
    sb.setState(100);
    o1->update();
    return 0;
}

