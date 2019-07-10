#include <fun.h>
#define RED true
#define BLUE false
#define DRAGON 0
#define NINJA 1
#define ICEMAN 2
#define LION 3
#define WOLF 4
using namespace std;

int enemyInBlue = 0;
int enemyInRed = 0;
//依次保存dragon,ninja,iceman,lion,wolf的初始信息
typedef struct
{
    int life;
    int attack;
}warriorInfo;
warriorInfo info[5];
//时间类
class Time
{
private:
    int hour;
    int minute;
    static auto_ptr<Time> _tp;
    Time()
    {
        hour = 0;
        minute = 0;
    }
public:
    int Minute()
    {
        return minute;
    }
    int getAll()
    {
        return hour*60+minute;
    }
    void setTime()
    {
        hour =0;
        minute =0;
    }
    static Time* getTime()
    {
        if(_tp.get() == NULL)
        {
            _tp.reset(new Time);
        }
        return _tp.get();
    }
    static void timeRun()
    {
        Time::getTime()->minute += 10;
        if(_tp->minute>=60)
        {
            _tp->minute -= 60;
            _tp->hour += 1;
        }
    }
    friend ostream& operator<<(ostream& os,Time *time);
};
ostream& operator<<(ostream& os,Time *time)
{
    os<<setw(3)<<setfill('0')<<time->hour<<":"<<setw(2)<<time->minute<<" "<<setfill(' ');
    return os;
}
auto_ptr<Time> Time::_tp(NULL);

class Warrior;
class Commander;
//城市类
typedef enum color
{
    Red,
    Blue,
    NoColor
}FLAG;
class City
{
public:
    //城市编号
    int _cityNumber;
    //城市生命元数量
    int _element;
    //城市中的红军
    deque<Warrior*> red;
    //城市中的蓝军
    deque<Warrior*> blue;
    //下一个城市
    City* next;
    //上一个城市
    City* prev;
    //城市的旗子
    FLAG flag;
    //红军胜利次数
    int redWin;
    //蓝军胜利次数
    int blueWin;
    static vector<City*> _ac;

    City(int n):_cityNumber(n)
    {
        _element = 0;
        redWin =  0;
        blueWin = 0;
        next = NULL;
        prev = NULL;
        flag = NoColor;
    }
    static vector<City*>& cityList()
    {
        return _ac;
    }
    static vector<City*>& createCity(int n)
    {
        for(auto  &a:_ac)
        {
            delete  a;
        }
        _ac.clear();
        int len = _ac.size();
        while(len<n)
        {
            _ac.push_back(new City(++len));
            if(len == 1)
            {
                continue;
            }
            _ac[len-1]->prev = _ac[len-2];
            _ac[len-2]->next = _ac[len-1];
        }
        return _ac;
    }
    int getNumber()
    {
        return _cityNumber;
    }
};
vector<City*> City::_ac;
ostream& operator<<(ostream& os, City * city)
{
    os<<"city "<<city->getNumber()<<" ";
    return os;
}
//武士
class Warrior
{
private:
    int _No;//编号
    int _life;//生命值
    int _attack;//攻击力
    bool _alive;//是否存活
    City * _place;//记录所在城市,如果为NULL表明还未出发或者已经达到
    Commander *_color;//记录从属的司令部
public:
    bool moved;
    bool arrived;
    Warrior(int No,int life,int attack,Commander* color)
    {
        moved = false;
        _No = No;
        _life = life;
        _alive = true;
        _attack = attack;
        _place = NULL;
        _color = color;
        arrived = false;
    }
    virtual ~Warrior(){}
    int getNo()
    {
        return _No;

    }
    City* where()
    {
        return _place;
    }
    int& Life()
    {
        return _life;
    }
    int& Attack()
    {
        return _attack;
    }
    virtual void decreLife(int desc)
    {
        _life-=desc;
        //武士死亡
        if(_life<=0 &&_alive)
        {
            //打印武士死亡事件
            cout<<Time::getTime()<<outCamp()<<" "<<getName()<<" "<<getNo()<<" "<<"was killed "<<"in "<<where()<<endl;
            _alive = false;
            if(_place!=NULL)
            {
                //红方
                if(getCamp())
                {
                    _place->red.pop_back();
                }
                //蓝方
                else
                {
                    _place->blue.pop_back();
                }
            }

        }
    }
    Commander* getCommander()
    {
        return _color;
    }
    void addLife(int add)
    {
        _life+=add;
    }
    bool isAlive()
    {
        return _alive;
    }
    //打印武士的生命和攻击
    void outLifeAttack()
    {
        cout<<"with "<<_life<<" elements and "<<_attack<<" force"<<endl;
    }
    bool getCamp();
    string outCamp();
    virtual string getName() = 0;
    void move()
    {
        //红方武士移动
        if(getCamp())
        {
            //未出发则出发
            if(_place == NULL&& !arrived)
            {
                _place = City::cityList()[0];
                _place->red.push_back(this);
            }
            else
            {
                _place->red.pop_front();
                if(_place == City::cityList().back())
                {
                    arrived = true;
                    ++enemyInBlue;
                    _place = NULL;
                }
                else
                {
                    _place = _place->next;
                    _place->red.push_back(this);
                }
            }
        }
        //蓝方武士移动
        else
        {
            //未出发则出发
            if(_place == NULL&& !arrived)
            {
                _place = City::cityList()[City::cityList().size()-1];
                _place->blue.push_back(this);
            }
            else
            {
                _place->blue.pop_front();
                if(_place == City::cityList().front())
                {
                    arrived = true;
                    ++enemyInRed;
                    _place = NULL;
                }
                else
                {
                    _place = _place->prev;
                    _place->blue.push_back(this);
                }
            }
        }
    }
    //打印移动信息
    virtual void showMove()
    {
        if(!isAlive())
        {
            return;
        }
        if(arrived)
        {
            return;
        }
        move();
        if(arrived)
        {
            if(getCamp())
            {
                cout<<Time::getTime()<<this->outCamp()<<" "<<getName()<<" "<<getNo()<<" "<<"reached blue headquarter";
            }
            else
            {
                cout<<Time::getTime()<<this->outCamp()<<" "<<getName()<<" "<<getNo()<<" "<<"reached red headquarter";
            }
            outLifeAttack();
            return;
        }
        cout<<Time::getTime()<<this->outCamp()<<" "<<getName()<<" "<<getNo()<<" "<<"marched to "<<this->_place;
        outLifeAttack();
    }
    virtual void fight(Warrior *guard);
    virtual void defence(Warrior *attacker);
};
//重载输出
ostream& operator<<(ostream& os,Warrior* solider)
{
    os<<solider->outCamp()<<" "<<solider->getName()<<" "<<solider->getNo()<<" ";
    return os;
}
class Dragon:public Warrior
{
public:
    Dragon(int No,Commander *camp):Warrior(No,info[DRAGON].life,info[DRAGON].attack,camp)
    {
        //打印武士降生事件
        cout<<Time::getTime()<<this<<"born"<<endl;
    }
    string getName()
    {
        return "dragon";
    }
    virtual void fight(Warrior* guard)
    {
        this->Warrior::fight(guard);
        if(isAlive())
        {
            //打印dragon欢呼事件
            cout<<Time::getTime()<<this<<"yelled in "<<where()<<endl;
        }
    }
};
class Ninja:public Warrior
{
public:
    Ninja(int No,Commander *camp):Warrior(No,info[NINJA].life,info[NINJA].attack,camp)
    {
        //打印武士降生事件
        cout<<Time::getTime()<<this<<"born"<<endl;
    }
    string getName()
    {
        return "ninja";
    }
    virtual void defence(Warrior* attacker)
    {
        attacker->getNo();
    }
};
class Iceman:public Warrior
{
private:
    int step;
public:
    Iceman(int No,Commander *camp):Warrior(No,info[ICEMAN].life,info[ICEMAN].attack,camp)
    {
        step = 0;
        //打印武士降生事件
        cout<<Time::getTime()<<this<<"born"<<endl;
    }
    string getName()
    {
        return "iceman";
    }
    virtual void showMove()
    {
        if(!isAlive())
        {
            return;
        }
        if(arrived)
        {
            return;
        }
        this->Warrior::move();
        ++step;
        if(step == 2)
        {
            step = 0;
            Attack()+=20;
            if(Life()<=9)
            {
                Life()=1;
            }
            else
            {
                Life()-=9;
            }
        }
        if(arrived)
        {
            if(getCamp())
            {
                cout<<Time::getTime()<<outCamp()<<" "<<getName()<<" "<<getNo()<<" "<<"reached blue headquarter ";
            }
            else
            {
                cout<<Time::getTime()<<this<<"reached red headquarter ";
            }
            outLifeAttack();
            return;
        }
        cout<<Time::getTime()<<this<<"marched to "<<this->where();
        outLifeAttack();
    }
};
class Lion:public Warrior
{
private:
    int preLife;
public:
    Lion(int No,Commander *camp):Warrior(No,info[LION].life,info[LION].attack,camp)
    {
        preLife = 0;
        cout<<Time::getTime()<<this<<"born"<<endl;
    }
    string getName()
    {
        return "lion";
    }
    virtual void decreLife(int desc)
    {
        preLife = Life();
        this->Warrior::decreLife(desc);
    }
    virtual void fight(Warrior *guard)
    {
        preLife = Life();
        this->Warrior::fight(guard);
        //战死
        if(!isAlive())
        {
            guard->Life()+=preLife;
        }
    }
    virtual void defence(Warrior* attacker)
    {
        this->Warrior::defence(attacker);
        if(!isAlive())
        {
            attacker->Life()+=preLife;
        }
    }
};
class Wolf:public Warrior
{
private:
    int kill;
public:
    Wolf(int No,Commander *camp):Warrior(No,info[WOLF].life,info[WOLF].attack,camp)
    {
        kill = 0;
        cout<<Time::getTime()<<this<<"born"<<endl;
    }
    string getName()
    {
        return "wolf";
    }
    void fight(Warrior* guard);
};
//司令部类
class Commander
{
private:
    int _lifeElement;
    bool _camp;
    vector<Warrior*> _army;
    int _number;
public:
    Commander(int life,bool color)
    {
        _lifeElement = life;
        _number = 1;
        _camp = color;
    }
    ~Commander()
    {
        for(auto &a: _army)
        {
            delete a;
        }
    }
    vector<Warrior*>& getArmy()
    {
        return _army;
    }
    int& LifeElement()
    {
        return _lifeElement;
    }
    bool getCamp()
    {
        return _camp;
    }
    string outCamp()
    {
        if(_camp)
        {
            return "red";
        }
        else
        {
            return "blue";
        }
    }
    int award()
    {
        if(_lifeElement<8)
        {
            return 0;
        }
        _lifeElement -= 8;
        return 8;
    }
    void createWarrior()
    {
        //红方生产士兵
        if(_camp)
        {
            switch(_number%5)
            {
            case 0:
                if(_lifeElement>=info[DRAGON].life)
                {
                    _army.push_back(new Dragon(_number,this));
                    ++_number;
                    _lifeElement-=info[DRAGON].life;
                }
                break;
            case 1:
                if(_lifeElement>=info[ICEMAN].life)
                {
                    _army.push_back(new Iceman(_number,this));
                    ++_number;
                    _lifeElement-=info[ICEMAN].life;
                }
                break;
            case 2:
                if(_lifeElement>=info[LION].life)
                {
                    _army.push_back(new Lion(_number,this));
                    ++_number;
                    _lifeElement-=info[LION].life;
                }
                break;
            case 3:
                if(_lifeElement>=info[WOLF].life)
                {
                    _army.push_back(new Wolf(_number,this));
                    ++_number;
                    _lifeElement-=info[WOLF].life;
                }
                break;
            case 4:
                if(_lifeElement>=info[NINJA].life)
                {
                    _army.push_back(new Ninja(_number,this));
                    ++_number;
                    _lifeElement-=info[NINJA].life;
                }
                break;
            default:
                break;
            }
        }
        //蓝方生产士兵
        else
        {
            switch(_number%5)
            {
            case 0:
                if(_lifeElement>=info[WOLF].life)
                {
                    _army.push_back(new Wolf(_number,this));
                    ++_number;
                    _lifeElement-=info[WOLF].life;
                }
                break;
            case 1:
                if(_lifeElement>=info[LION].life)
                {
                    _army.push_back(new Lion(_number,this));
                    ++_number;
                    _lifeElement-=info[LION].life;
                }
                break;
            case 2:
                if(_lifeElement>=info[DRAGON].life)
                {
                    _army.push_back(new Dragon(_number,this));
                    ++_number;
                    _lifeElement-=info[DRAGON].life;
                }
                break;
            case 3:
                if(_lifeElement>=info[NINJA].life)
                {
                    _army.push_back(new Ninja(_number,this));
                    ++_number;
                    _lifeElement-=info[NINJA].life;
                }
                break;
            case 4:
                if(_lifeElement>=info[ICEMAN].life)
                {
                    _army.push_back(new Iceman(_number,this));
                    ++_number;
                    _lifeElement-=info[ICEMAN].life;
                }
                break;
            default:
                break;
            }

        }
    }
};
//战争类
class War
{
private:
    Commander *red,*blue;
public:
    void startWar(fstream& is)
    {
        int element,city, Atime;
        is>>element>>city>>Atime;
        is>>info[0].life>>info[1].life>>info[2].life>>info[3].life>>info[4].life;
        is>>info[0].attack>>info[1].attack>>info[2].attack>>info[3].attack>>info[4].attack;
        red = new Commander(element,RED);
        blue = new Commander(element,BLUE);
        City::createCity(city);
        Time::getTime()->setTime();
        enemyInRed = 0;
        enemyInBlue = 0;
        while(Time::getTime()->getAll()<Atime)
        {
            switch(Time::getTime()->Minute())
            {
            case 0:
                red->createWarrior();
                blue->createWarrior();
                break;
            case 10:
                //即将到达红军司令部的武士移动
                if(City::cityList().front()->blue.size())
                {
                    City::cityList().front()->blue.front()->showMove();
                }
                //从红军司令部出发的武士移动
                if(red->getArmy().back()->where()==NULL)
                { 
                    red->getArmy().back()->moved = true;
                    red->getArmy().back()->showMove();
                }
                //到达各个城市的武士移动
                for(auto &a:City::cityList())
                {
                    if(a->next && a->next->blue.size())
                    {
                        a->next->blue.front()->showMove();
                    }
                    if(a->next && a->red.size())
                    {
                        if(!a->red.front()->moved)
                        {
                            a->red.front()->moved = true;
                            a->red.front()->showMove();
                        }
                        else
                        {
                            a->red.front()->moved = false;
                        }
                    }
                }
                //蓝军司令部出发的武士移动
                if(blue->getArmy().back()->where()==NULL)
                {
                    blue->getArmy().back()->showMove();
                }
                //到达蓝军司令部的武士移动
                if(City::cityList().back()->red.size())
                {
                    if(!City::cityList().back()->red.front()->moved)
                    {
                        City::cityList().back()->red.front()->showMove();
                    }
                    else
                    {
                        City::cityList().back()->red.front()->moved =false;
                    }
                }
                red->getArmy().back()->moved = false;
                //胜利条件判断
                if(enemyInRed >= 2)
                {
                    cout<<Time::getTime()<<"red headquater was taken"<<endl;
                    delete red;
                    delete blue;
                    return;
                }
                if(enemyInBlue >= 2)
                {
                    cout<<Time::getTime()<<"blue headquater was taken"<<endl;
                    delete red;
                    delete blue;
                    return;
                }
                break;
                //产出生命元
            case 20:
                for(auto &a:City::cityList())
                {
                    a->_element+=10;
                }
                break;
                //取走生命元
            case 30:
                for(auto &a:City::cityList())
                {
                    if(a->blue.size()==0 && a->red.size()>=1)
                    {
                        a->red.front()->getCommander()->LifeElement()+=a->_element;
                        cout<<Time::getTime()<<a->red.front()<<"earned "<<a->_element<<" for his headquarter"<<endl;
                        a->_element = 0;
                    }
                    if(a->blue.size()!=0 && a->red.size()==0)
                    {
                        a->blue.front()->getCommander()->LifeElement()+=a->_element;
                        cout<<Time::getTime()<<a->blue.front()<<"earned "<<a->_element<<" for his headquarter"<<endl;
                        a->_element = 0;
                    }
                }
                break;
                //开战
            case 40:
                for(auto &a:City::cityList())
                {
                   // cout<<a<<"blue:"<<a->blue.size()<<endl;
                   // cout<<a<<"red:"<<a->red.size()<<endl;

                    if(a->blue.size()!=0 && a->red.size()!=0)
                    {
                        if(a->flag == Red || (a->flag == NoColor && a->_cityNumber%2==1))
                        {
                            a->red.front()->fight(a->blue.front());
                        }
                        if(a->flag == Blue || (a->flag == NoColor && a->_cityNumber%2==0))
                        {
                            a->blue.front()->fight(a->red.front());
                        }
                        if(a->flag == NoColor)
                        {
                            if(a->redWin >= 2)
                            {
                                a->flag = Red;
                                cout<<Time::getTime()<<"red flag raised in "<<a<<endl;
                            }
                            if(a->blueWin >= 2)
                            {
                                a->flag = Blue;
                                cout<<Time::getTime()<<"blue flag raised in "<<a<<endl;
                            }
                        }
                    }
                }
                break;
            case 50:
                cout<<Time::getTime()<<red->LifeElement()<<" elements in red headquarter"<<endl;
                cout<<Time::getTime()<<blue->LifeElement()<<" elements in blue headquarter"<<endl;
                break;
            default:
                break;
            }
            Time::timeRun();
        }
        delete red;
        delete blue;
    }
};
bool Warrior::getCamp()
{
    return _color->getCamp();
}
string Warrior::outCamp()
{
    return _color->outCamp();
}
void Warrior::fight(Warrior * guard)
{
    //打印进攻事件
    cout<<Time::getTime()<<this<<"attacked "<<guard<<"in "<<where();
    outLifeAttack();

    guard->decreLife(this->Attack());
    guard->defence(this);
    if(!guard->isAlive())
    {
        //打赢了,报告司令部,增加生命值
        addLife(getCommander()->award());
        getCommander()->LifeElement()+=where()->_element;
        cout<<Time::getTime()<<outCamp()<<" "<<getName()<<" "<<getNo()<<" "<<"earned "<<where()->_element<<" for his headquarter"<<endl;
        where()->_element = 0;
        if(getCamp())
        {
            _place->redWin+=1;
            _place->blueWin = 0;
        }
        else
        {
            _place->redWin=0;
            _place->blueWin += 1;
        }
    }
}
void Warrior::defence(Warrior* attacker)
{
    if(isAlive())
    {
        attacker->decreLife(Attack()/2);
    }
    if(!attacker->isAlive())
    {
        addLife(_color->award());
        getCommander()->LifeElement()+=where()->_element;
        cout<<Time::getTime()<<outCamp()<<" "<<getName()<<" "<<getNo()<<" "<<"earned "<<where()->_element<<" for his headquarter"<<endl;
        where()->_element = 0;
        if(getCamp())
        {
            _place->redWin+=1;
            _place->blueWin = 0;
        }
        else
        {
            _place->redWin=0;
            _place->blueWin += 1;
        }
    }
}
void Wolf::fight(Warrior* guard)
{
    cout<<Time::getTime()<<this<<"attacked "<<guard<<"in "<<where();
    outLifeAttack();
    
    guard->decreLife(Attack());
    if(!guard->isAlive())
    {
        if(++kill%2 == 0)
        {
            Attack()*=2;
            Life()*=2;
        }
        addLife(getCommander()->award());
        getCommander()->LifeElement()+=where()->_element;
        cout<<Time::getTime()<<outCamp()<<" "<<getName()<<" "<<getNo()<<" "<<"earned "<<where()->_element<<" for his headquarter"<<endl;
        where()->_element = 0;
        if(getCamp())
        {
            where()->redWin+=1;
            where()->blueWin = 0;
        }
        else
        {
            where()->redWin=0;
            where()->blueWin += 1;
        }
    }
    guard->defence(this);
}
int main()
{
    War wow;
    fstream fs("config.dat",ios::out);
    int count;
    cin>>count;
    int data;
    for(int i =0;i<count*13;i++)
    {
        cin>>data;
        fs<<data<<endl;
    }
    fs.close();
    fs.open("config.dat",ios::in);
    for(int i =0;i<count;i++)
    {
        cout<<"case:"<<i+1<<endl;
        wow.startWar(fs);
    }
    fs.close();
    return 0;
}

