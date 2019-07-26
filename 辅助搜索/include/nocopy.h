#pragma once

class Nocopy                                                               
{
protected:
    Nocopy(){}
private:
    Nocopy(Nocopy &)=delete;
    Nocopy& operator=(Nocopy &)=delete;

};
