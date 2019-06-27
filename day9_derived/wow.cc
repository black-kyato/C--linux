#include <fun.h>
#define RED  true
#define BLUE false
using namespace std;


class Weapon
{
    enum weaponType
    {
        Sword,
        Bomb,
        Arrow
    };
private:
    weaponType _type;
    string weaponName;
    unsigned int _damage;
public:
    Weapon(unsigned int No):_type((enum weaponType)No){
        switch(_type)
        {
        case Sword:
            weaponName = "sword";
            _damage = 8;
            break;
        case Bomb:
            weaponName = "bomb";
            _damage = 6;
            break;
        case Arrow:
            weaponName = "arrow";
            _damage = 4;
        default:
            break;
        }
    }
    void damageChange(int ch = -1)
    {
        _damage = _damage + ch;
    }
    string getWeapon()
    {
        return weaponName;
    }
    int getDamage()
    {
        return _damage;
    }
};

class Warrior
{
private:
    int _No;
    int _life;
public:
    Warrior(int No,int life)
    {
        _No = No;
        _life = life;
    }
    int getNo()
    {
        return _No;
    }
    int getLife()
    {
        return _life;
    }
    void changeLife(int desc)
    {
        _life=+desc;
    }
};

class Dragon:public Warrior
{
private:
    Weapon _w;
    float _morale;
public:
    Dragon(int No,float morale,int life  = 40):Warrior(No,life),_w(No%3),_morale(morale){}
    float getMorale()
    {
        return _morale;
    }
    Weapon& getWeapon()
    {
        return  _w;
    }
    void changeMorale(float m)
    {
        _morale +=m;
    }
    void setMorale(float m)
    {
        _morale = m;
    }
};
class Ninja:public Warrior
{
private:
    Weapon _w1;
    Weapon _w2;
public:
    Ninja(int No,int life = 15):Warrior(No,life),_w1(No%3),_w2((No+1)%3){}
    Weapon& getWeapon1()
    {
        return _w1;
    }
    Weapon& getWeapon2()
    {
        return  _w2;
    }
};
class Iceman:public Warrior
{
private:
    Weapon _w;
public:
    Iceman(int No,int life = 30):Warrior(No,life),_w(No%3){}
    Weapon& getWeapon()
    {
        return  _w;
    }
};
class Lion:public Warrior
{
private:
    int _loyalty;
public:
    Lion(int No,int loyalty,int life = 25):Warrior(No,life),_loyalty(loyalty){}
    int getLoyalty()
    {
        return _loyalty;
    }
    void setLoyalty(int loyalty)
    {
        _loyalty = loyalty;
    }
};
class Wolf:public Warrior
{
public:
    Wolf(int No,int life):Warrior(No,life){}
};

class Command
{
private:
    bool _color;
    string color;
    int _lifeCount;
    int count;
    vector<Dragon> armyD;
    vector<Ninja> armyN;
    vector<Iceman> armyI;
    vector<Lion> armyL;
    vector<Wolf> armyW;
    static int time;
public:
    static int dragonLife,ninjaLife,icemanLife,lionLife,wolfLife;
    Command(bool color,int lifeCount):_color(color),_lifeCount(lifeCount){
        count = 0;
        if(color)
        {
            this->color = "red";
        }
        else
        {
            this->color = "blue";
        }   
    }
    int getLife()
    {
        return _lifeCount;
    }
    bool proDragon()
    {
        if(dragonLife>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= dragonLife;
        Dragon d(count,_lifeCount/(float)dragonLife,dragonLife);
        armyD.push_back(d);
        cout<<color<<" dragon "<<count<<" born with strength "<<dragonLife<<","<<armyD.size()<<" dragon in "<<color<<" headquarter."<<endl;
        cout<<"It has a "<<d.getWeapon().getWeapon()<<",and it's morale is "<<d.getMorale()<<endl;
        return true;
    }
    bool proNinja()
    {
        if(ninjaLife>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= ninjaLife;
        Ninja n(count,ninjaLife);
        armyN.push_back(n);
        cout<<color<<" ninja "<<count<<" born with strength "<<ninjaLife<<","<<armyN.size()<<" ninja in "<<color<<" headquarter."<<endl;
        cout<<"It has a "<<n.getWeapon1().getWeapon()<<" and a "<<n.getWeapon2().getWeapon()<<endl;
        return true;
    }
    bool proIceman()
    {
        if(icemanLife>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= icemanLife;
        Iceman i(count,icemanLife);
        armyI.push_back(i);
        cout<<color<<" iceman "<<count<<" born with strength "<<icemanLife<<","<<armyI.size()<<" iceman in "<<color<<" headquarter."<<endl;
        cout<<"It has a "<<i.getWeapon().getWeapon()<<endl;
        return true;
    }
    bool proLion()
    {
        if(lionLife>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= lionLife;
        Lion l(count,_lifeCount,lionLife);
        armyL.push_back(l);
        cout<<color<<" lion "<<count<<" born with strength "<<lionLife<<","<<armyL.size()<<" lion in "<<color<<" headquarter."<<endl;
        cout<<"It's loyalty is "<<l.getLoyalty()<<endl;
        return true;
    }
    bool proWolf()
    {
        if(wolfLife>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= wolfLife;
        Wolf w(count,wolfLife);
        armyW.push_back(w);
        cout<<color<<" wolf "<<count<<" born with strength "<<wolfLife<<","<<armyW.size()<<" wolf in "<<color<<" headquarter."<<endl;
        return true;
    }
    bool getColor()
    {
        return _color;
    }
    static void timeGo()
    {
        ++time;
    }
    static int getTime()
    {
        return time;
    }
    void produce(int flag)
    {
        bool conf = true;
        cout<<setw(3)<<setfill('0')<<time<<" "<<setfill(' ');
        if(_color)
        {
            switch(flag)
            {
start:      case 1:
                conf = proIceman();
                if(conf)
                    break;
            case 2:
                conf = proLion();
                    if(conf)
                        break;
            case 3:
                conf = proWolf();
                if(conf)
                    break;
            case 4:
                conf = proNinja();
                if(conf)
                    break;
            case 5:
                conf = proDragon();
                if(conf)
                    break;
            default:
                if(!conf)
                goto start;
                break;
            }
        }
        else
        {
            switch(flag)
            {
startB:      case 1:
                conf = proLion();
                if(conf)
                    break;
            case 2:
                conf = proDragon(); 
                    if(conf)
                        break;
            case 3:
                conf = proNinja();
                if(conf)
                    break;
            case 4:
                conf = proIceman();
                if(conf)
                    break;
            case 5:
                conf = proWolf();
                if(conf)
                    break;
            default:
                if(!conf)
                goto startB;
                break;
            }
        }
    }
};
int Command::time = 0;
int Command::dragonLife = 0;
int Command::lionLife = 0;
int Command::icemanLife = 0;
int Command::wolfLife = 0;
int Command::ninjaLife = 0;

int Min(int a,int b)
{
    return a>b?b:a;
}
int main()
{
    int life;
    cout<<"请输入生命元(1-10000)"<<endl;
    cin>>life;
    cout<<"请输入dragon 、ninja、iceman、lion、wolf 的初始生命值(0-10000)"<<endl;
    cin>>Command::dragonLife>>Command::ninjaLife>>Command::icemanLife>>Command::lionLife>>Command::wolfLife;
    Command red(RED,life),blue(BLUE,life);
    int min = Min(Min(Command::dragonLife,Command::ninjaLife),Min(Command::icemanLife,Min(Command::lionLife,Command::wolfLife)));
    
    bool stopR=false,stopB =false;
    int i = 1;
    while(!stopR||!stopB)
    {
        if( ++i == 6 )
        {
            i = 1;
        }
        if(red.getLife()>=min)
        {
            red.produce(i);
        }
        else
        {
            if(!stopR)
            {
                cout<<setw(3)<<setfill('0')<<Command::getTime()<<"red headquarter stops making warriors"<<endl;
                stopR = true;
            }
        }
        if(blue.getLife()>=min)
        {
            blue.produce(i);
        }
        else
        {
            if(!stopB)
            {
                cout<<setw(3)<<setfill('0')<<Command::getTime()<<"blue headquarter stops making warriors"<<endl;
                stopB = true;
            }
        }
        Command::timeGo();
    }
    return 0;
}

