#include <iostream>
#include <fun.h>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

class WordFrequency
{
private:
    string _word;
    int _freq;
public:
    WordFrequency(string word):_word(word){
        _freq = 1;
    }
    string showWord()
    {
        return _word;
    }
    int showFre()
    {
        return _freq;
    }
    bool isEqual(string word)
    {
        if(word == _word)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void add(int t = 1)
    {
        _freq+=t;
    }   
};
class Dict
{
private:
    vector<WordFrequency> wordP;
public:
    Dict():wordP(){
    }
    ~Dict(){
    }
    void addWord(string word)
    {
        for(auto child:wordP)
        {
            if(child.isEqual(word))
            {
                child.add();
                return;
            }
        }
        WordFrequency t(word);
        wordP.push_back(t);
    }
    void showDict()
    {
        cout<<"--------------------------------"<<endl;
        for(auto child:wordP)
        {
            cout<<"|"<<left<<setw(22)<<child.showWord()<<setw(8)<<child.showFre()<<"|"<<endl;
        }
        cout<<"--------------------------------"<<endl;
    }
};

int main()
{
    ifstream  filein("/home/jzk/The_Holy_Bible.txt");
    Dict bible;
    string word;
    cout<<"初始词典"<<endl;
    bible.showDict();
    while(filein>>word)
    {
        bible.addWord(word);
    }
    cout<<"圣经词典"<<endl;
    bible.showDict();
    filein.close();
    return 0;
}

