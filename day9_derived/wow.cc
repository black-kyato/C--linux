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
public:
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
    bool proDragon(int life)
    {
        if(life>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= life;
        Dragon d(count,count/(float)life,life);
        armyD.push_back(d);
        cout<<color<<" dragon "<<count<<" born with strength "<<life<<","<<armyD.size()<<" dragon in "<<color<<" headquarter."<<endl;
        cout<<"It has a "<<d.getWeapon().getWeapon()<<",and it's morale is "<<d.getMorale()<<endl;
        return true;
    }
    bool proNinja(int life)
    {
        if(life>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= life;
        Ninja n(count,life);
        armyN.push_back(n);
        cout<<color<<" ninja "<<count<<" born with strength "<<life<<","<<armyN.size()<<" ninja in "<<color<<" headquarter."<<endl;
        cout<<"It has a "<<n.getWeapon1().getWeapon()<<" and a "<<n.getWeapon2().getWeapon()<<endl;
        return true;
    }
    bool proIceman(int life)
    {
        if(life>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= life;
        Iceman i(count,life);
        armyI.push_back(i);
        cout<<color<<" iceman "<<count<<" born with strength "<<life<<","<<armyI.size()<<" iceman in "<<color<<" headquarter."<<endl;
        cout<<"It has a "<<i.getWeapon().getWeapon()<<endl;
        return true;
    }
    bool proLion(int life)
    {
        if(life>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= life;
        Lion l(count,_lifeCount,life);
        armyL.push_back(l);
        cout<<color<<" lion "<<count<<" born with strength "<<life<<","<<armyL.size()<<" lion in "<<color<<" headquarter."<<endl;
        cout<<"It's loyalty is "<<l.getLoyalty()<<endl;
        return true;
    }
    bool proWolf(int life)
    {
        if(life>_lifeCount)
        {
            return false;
        }
        ++count;
        _lifeCount -= life;
        Wolf w(count,life);
        armyW.push_back(w);
        cout<<color<<" wolf "<<count<<" born with strength "<<life<<","<<armyW.size()<<" wolf in "<<color<<" headquarter."<<endl;
        return true;
    }
    void produce(int dragonLife = 40,int ninjaLife = 15,int icemanLife = 35,int lionLife = 30,int wolfLife = 20)
    {
        bool b1,b2,b3,b4,b5;
        b1=b2=b3=b4=b5=true;
        if(_color)
        {
            while(b1 || b2 || b3 || b4 || b5)
            {
                b1 = proIceman(icemanLife);
                b2 = proLion(lionLife);
                b3 = proWolf(wolfLife);
                b4 = proNinja(ninjaLife);
                b5 = proDragon(dragonLife);
            }
        }
        else
        {
            while(b1 || b2 || b3 || b4 || b5)
            {
                b1 = proLion(lionLife);
                b2 = proDragon(dragonLife);
                b3 = proNinja(ninjaLife);
                b4 = proIceman(icemanLife);
                b5 = proWolf(wolfLife);
            }
        }
    }
};

int main()
{
    Command red(RED,3000);
    red.produce(50,15,40,35,20);
    return 0;
}

