
/*
 * 程序功能:打开特定文件,输入单词,检索出现单词的行
 * 支持多个单词查询,支持单词之间进行与或非
 * */
#include <fun.h>
using namespace std;

//按行保存了整个文件的信息
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
    int LineCount()
    {
        return _text.size();
    }
    friend class Dict;
};
//保存查询结果,单词出现的次数和出现的行数
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
//词典类,保存每个单词出现的次数和行数
class Dict
{
private:
    TextInfo _text;
    map<string,WordInfo> _dict;
public:
    //建立词典,保存了每个单词
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
    //查询单词,打印单词出现的次数,返回一个保存单词出现的行数的数组
    vector<int> search(string word)
    {
        vector<int> lines;
        map<string,WordInfo>::iterator dp = _dict.find(word);
        if(dp==_dict.end())
        {
            cout<<"no such word..."<<endl;
            return lines;
        }
        cout<<word<<" occcurs "<<dp->second.getF()<< "times."<<endl;
        return dp->second.getLines();
    }
    void output(vector<int> lines)
    {
        for(auto a:lines)
        {
            cout<<"line "<<a<<":"<<_text.getLine(a)<<endl;
        }
    }
    vector<int> Non(vector<int> lines)
    {
        int count = _text.LineCount();
        vector<int> res;
        for(int i = 1;i<=count;i++)
        {
            bool find = false;
            for(auto a:lines)
            {
                if(a == i)
                {
                    find = true;
                    break;
                }
            }
            if(!find)
            res.push_back(i);
        }
        return res;
    }
    vector<int> Or(vector<int> a,vector<int> b)
    {
        vector<int> res;
        for(auto ac:a)
        {
            bool find = false;
            for(auto rc:res)
            {
                if(rc == ac)
                {
                    find = true;
                    break;
                }
            }
            if(!find)
            {
                res.push_back(ac);
            }
        }
        for(auto bc:b)
        {
            bool find = false;
            for(auto rc:res)
            {
                if(rc == bc)
                {
                    find = true;
                    break;
                }
            }
            if(!find)
            {
                res.push_back(bc);
            }
        }
        return res;
    }
    vector<int> And(vector<int> a, vector<int> b)
    {
        vector<int> res;
        for(auto ac:a)
        {
            for(auto bc:b)
            {
                if(ac == bc)
                {
                    res.push_back(ac);
                    break;    
                }       
            }
        }
        return res;
    }
};
class Query
{
private:
    Dict dict;
    string expr;
    queue<char> cmdlist;
    queue<string> wordlist;
public:
    void getText(const char * file)
    {
        dict.getDict(file);
    }
    void input()
    {
        cin>>expr;
    }
    //分解命令表达式
    void separate()
    {
        string word;
        for(auto a:expr)
        {
            if(a=='&'||a=='|'||a=='~')
            {
                if(!word.empty())
                {
                    cmdlist.push(a);
                    wordlist.push(word);
                    word.clear();
                }
                else
                {
                    if(a =='~')
                        cmdlist.push(a);
                }
            }
            else
            {
                word.push_back(a);
            }
        }
        wordlist.push(word);
    }
    //处理表达式
    void handle()
    {
        input();
        separate();
        vector<int> lines;
        vector<int> lines_c;
        if(cmdlist.empty())
       {
           cout<<"no cmd"<<endl;
            lines = dict.search(wordlist.front());
            dict.output(lines);
            return;
        }
        char cmd;
        string word;
        string word_c;
        while(!cmdlist.empty())
        {
            cmd  = cmdlist.front();
            cmdlist.pop();
            switch(cmd)
            {
            case '~':
                word = wordlist.front();
                wordlist.pop();
                lines = dict.search(word);
                lines = dict.Non(lines);
                break;
            case '|':
                word = wordlist.front();
                wordlist.pop();
                if(lines.empty())
                {
                    word_c = wordlist.front();
                    wordlist.pop();
                    lines = dict.search(word_c);
                }
                lines_c = dict.search(word);
                lines = dict.Or(lines,lines_c);
                break;
            case '&':
                word = wordlist.front();
                wordlist.pop();
                if(lines.empty())
                {
                    word_c = wordlist.front();
                    wordlist.pop();
                    lines = dict.search(word_c);
                }
                lines_c = dict.search(word);
                lines = dict.And(lines,lines_c);
                break;
            default:
                cout<<"cmd error"<<endl;
                return;
                break;
            }
        }
        dict.output(lines);
    }
};
int main()
{
    Query q;
    char text[200];
    cout<<"input the searching source:"<<endl;
    cin>>text;
    q.getText(text);
    string word;
    cout<<"please input search cmd:"<<endl;
    q.handle();
    return 0;
}

