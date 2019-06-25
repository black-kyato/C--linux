#pragma once

#include <fun.h>

class phone
{
public:
    phone();
    phone(std::string name,int price);
    ~phone();
    std::string getName();
    int getPrice();
    void setPrice(int price);
private:
    class _phone;
    _phone *mp;
};
