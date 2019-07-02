#include <fun.h>
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
class DictVector
{
private:
    vector<WordFrequency> wordP;
public:
    DictVector():wordP(){
    }
    ~DictVector(){
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
class DictMap
{
private:
    map<string,int> _dict;
public:
    void addWord(string word)
    {
        pair<string,int> WordFrequency(word,1);
        pair<map<string,int>::iterator,bool> res = _dict.insert(WordFrequency);
        if(res.second == false)
        {
            ++(_dict.find(word)->second);
        }
    }
    void showDict()
    {
        for(auto a:_dict)
        {
            cout<<a.first<<" "<<a.second<<endl;
        }
    }
};
class DictUnMap
{
private:
    unordered_map<string,int> _dict;
public:
    void addWord(string word)
    {
        pair<string,int> WordFrequency(word,1);
        pair<unordered_map<string,int>::iterator,bool> res = _dict.insert(WordFrequency);
        if(res.second == false)
        {
            ++(_dict.find(word)->second);
        }
    }
};
int main()
{
    ifstream  filein;
    filein.open("bible.txt");
    DictVector bibleV;
    string word;
    time_t start,end;
    start = clock();
    while(filein>>word)
    {
        bibleV.addWord(word);
    }
    end = clock();
    cout<<"vector统计时间:"<<(end-start)/(float)CLOCKS_PER_SEC<<"s"<<endl;
    filein.close();
    
    filein.open("bible.txt");
    DictMap bibleM;
    start = clock();
    while(filein>>word)
    {
        bibleM.addWord(word);
    }
    end = clock();
    cout<<"map统计时间:"<<(end-start)/(float)CLOCKS_PER_SEC<<"s"<<endl;
    filein.close();
    
    filein.open("bible.txt");
    DictUnMap bibleUM;
    start = clock();
    while(filein>>word)
    {
        bibleUM.addWord(word);
    }
    end = clock();
    cout<<"unordered_map统计时间:"<<(end-start)/(float)CLOCKS_PER_SEC<<"s"<<endl;
    filein.close();
    return 0;
}

