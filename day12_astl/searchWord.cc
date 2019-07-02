#include <fun.h>
using namespace std;

class TextInfo
{
private:
    vector<string> _text;
public:
    void getText(const char * file)
    {
        ifstream filein(file);
        string line;
        while(getline(filein,line))
        {
            _text.push_back(line);
        }
        filein.close();
    }
    string getLine(int line)
    {
        return _text[line-1];
    }
    friend class Dict;
};
class WordInfo
{
private:
    int  frequency;
    vector<int> lines;
public:
    WordInfo(int f,int l)
    {
        frequency = f;
        lines.push_back(l);
    }
    void addFrequency()
    {
        ++frequency;
    }
    void addLine(int line)
    {
        for(auto a:lines)
        {
            if(a == line)
                return;
        }
        lines.push_back(line);
    }
    int getF()
    {
        return frequency;
    }
    vector<int> getLines()
    {
        return lines;
    }
};
class Dict
{
private:
    TextInfo _text;
    map<string,WordInfo> _dict;
public:
    void getDict(const char * file)
    {
        _text.getText(file);
        int lineCounter = 0;
        string word;
        for(auto line:_text._text)
        {
            istringstream record(line);
            ++lineCounter;
            while(record>>word)
            {
                WordInfo info(1,lineCounter);
                pair<string,WordInfo> wordInfo(word,info);
                pair<map<string,WordInfo>::iterator,bool> res = _dict.insert(wordInfo);
                if(res.second==false)
                {
                    map<string,WordInfo>::iterator pt =  _dict.find(word);
                    pt->second.addFrequency();
                    pt->second.addLine(lineCounter);
                }
            }
        }
    }
    void search(string word)
    {
        map<string,WordInfo>::iterator dp = _dict.find(word);
        if(dp==_dict.end())
        {
            cout<<"no such word..."<<endl;
            return;
        }
        cout<<word<<" occcurs "<<dp->second.getF()<< "times."<<endl;
        vector<int> lines = dp->second.getLines();
        for(auto line:lines)
        {
           cout<<"(line "<<line<<")"<< _text.getLine(line)<<endl;;
        }
    }
};
int main()
{
    Dict dict;
    char text[200];
    cout<<"input the searching source:"<<endl;
    cin>>text;
    dict.getDict(text);
    string word;
    cout<<"please input the word to search:"<<endl;
    cin>>word;
    dict.search(word);
    return 0;
}

