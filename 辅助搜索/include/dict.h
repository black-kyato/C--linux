#pragma  once
#include <fun.h>
using namespace std;

class Dict
{
private:
    map<string,int> _wordList;
    map<string,set<string>> _index;
public:
    ~Dict()
    {
        _wordList.clear();
        for(auto &a:_index)
        {
            a.second.clear();
        }
        _index.clear();
    }
    void createEnDict(string);
    void createChDict(string);
    void searchWord(string word,vector<string>& res);
    void searchIndex(string ch,set<string>& res)
    {
        cout<<"search index key:"<<ch<<endl;
        if(_index.find(ch)==_index.end())
        {
            cout<<"查无"<<ch<<endl;
            return;
        }
        for(auto a:_index.find(ch)->second)
        {
            res.insert(a);
        }
        cout<<"查找"<<ch<<"完毕"<<endl;
    }
    void readDict(string dictFile,string indexFile)
    {
        fstream dict(dictFile);
        fstream index(indexFile);
        pair<string,int> dictDoc;
        pair<string,set<string>> indexDoc;
        
        _wordList.clear();
        if(!dict.is_open()||!index.is_open())
            cout<<"读取词典数据失败"<<endl;
        string line;
        while(getline(dict,line))
        {
            stringstream outline(line);
            if(line.empty())
            {
                break;
            }
            outline>>dictDoc.first>>dictDoc.second;
            if(dictDoc.first.empty())
            {
                break;
            }
            _wordList.insert(dictDoc);
            line.clear();
        }
        cout<<"词库读取完毕"<<endl;
        _index.clear();
        line.clear();
        while(getline(index,line))
        {
            if(line.empty())
            {
                break;
            }
            stringstream outline(line);
            outline>>indexDoc.first;
            string word;
            while(!outline.eof())
            {
                outline>>word;
                indexDoc.second.insert(word);
            }
            _index.insert(indexDoc);
            line.clear();
        }
        cout<<"索引读取完毕"<<endl;
        index.close();
        dict.close();
    }
    void outTOfile(string dictFile, string indexFile)
    {
        fstream dict;
        fstream index;
        dict.open(dictFile,ios::out);
        index.open(indexFile,ios::out);
        if(!dict.is_open()||!index.is_open())
        {
            cout<<"保存词典文件失败"<<endl;
        }
        for(auto &a:_wordList)
        {
            dict<<a.first<<" "<<a.second<<endl;
        }
        for(auto &a:_index)
        {
            index<<a.first<<" ";
            for(auto &b:a.second)
            {
                index<<b<<" ";
            }
            index<<endl;
        }
        dict.close();
        index.close();
    }
};

