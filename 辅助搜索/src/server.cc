#include "../include/net.h"
#include "../libs/cppjieba/include/cppjieba/Jieba.hpp"

#define MAXCLIENT 10 //最大并发线程数

using namespace std;

const char* const DICT_PATH = "../libs/cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../libs/cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../libs/cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "../libs/cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../libs/cppjieba/dict/stop_words.utf8";
const string dictPath = "../conf/dict/dict.dat";
const string indexPath = "../conf/dict/index.dat";

//字符分割
void splitWord(const string & word, vector<string> & characters)
{
    int num = word.size();
    int i = 0;
    while(i < num)
    {
        int size = 1;
        char temp = word[i];
        while(temp & 0x80)
        {
            ++size;
            temp <<= 1;
        }
        if(size>1)
        {
            --size;
        }
        characters.push_back(word.substr(i,size));
        i += size;
    }
}
//建立英文词典和索引
void Dict::createEnDict(string filename)
{
    fstream dictfile;
    dictfile.open(filename.data(),ios::in);

    if(!dictfile.is_open())
    {
        cout<<"读取语料文件"<<filename<<"失败"<<endl;
    }
    char alp;
    string word;
    word.clear();
    int i = 0;
    dictfile>>noskipws;
    while(!dictfile.eof())
    {
        dictfile>>alp;
        if(alp>='A'&&alp<='Z')
        {
            alp += 'a' -'A';
        }
        if(alp>='a'&&alp<='z')
        {
            word.push_back(alp);
            ++i;
        }
        else
        {
            i = 0;
            //插入词典
            if(!word.empty())
            {
                ++_wordList[word];
                //记录索引
                for(auto a:word)
                {
                    string key;
                    key.clear();
                    key.push_back(a);
                    _index[key].insert(word);
                }
            }
            word.clear();
        }
    }
    dictfile.close();
}
//建立中文词典和索引
void Dict::createChDict(string filename)
{
    fstream dictfile;
    dictfile.open(filename.data(),ios::in);

    cppjieba::Jieba jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);
    vector<string> words;//存放分好的词组
    vector<string> characters;//存放单字用来建立索引
    string line;//读取的每行句子

    while(!dictfile.eof())
    {
        line.clear();
        getline(dictfile,line);
        if(line.empty())
        {
            continue;
        }
        words.clear();
        jieba.Cut(line,words,true);
        for(auto& word:words)
        {
            if(words.size()==1)
            {
                continue;
            }
            ++_wordList[word];
            characters.clear();
            splitWord(word,characters);
            for(auto ch:characters)
            {
                if(ch.size()==1)
                {
                    continue;
                }
                _index[ch].insert(word);
            }
        }
    }
    dictfile.close();
}
int min(int a,int b)
{
    return a>b?b:a;
}
//计算最小编辑距离
int minDis(string dest,string src)
{
    vector<string> ch1,ch2;
    splitWord(dest,ch1);
    splitWord(src,ch2);
    int l1 = ch1.size()+1,l2 =ch2.size()+1;

    vector<vector<int>> editArry;
    editArry.resize(l1);
    for(auto &a:editArry)
    {
        a.resize(l2);
    }
    for(int i = 0;i<l1;i++)
    {
        for(int j = 0;j<l2;j++)
        {
            if(i == 0)
            {
                editArry[i][j] = j;
                continue;
            }
            if(j == 0 )
            {
                editArry[i][j] = i;
                continue;
            }
            if(ch1[i-1]==ch2[j-1])
            {
                editArry[i][j] = editArry[i-1][j-1];
            }
            else
            {
                editArry[i][j] = min(min(editArry[i-1][j],editArry[i][j-1]),editArry[i-1][j-1]) + 1;
            }
        }
    }

    return editArry[l1-1][l2-1];
}
//在字典中查找候选词
void Dict::searchWord(string word,vector<string>& res)
{
    vector<string> chs;
    cout<<"分割字符"<<word<<":"<<endl;
    splitWord(word,chs);
    //所有候选词
    set<string> wordSet;
    vector<string> options;
    //所有候选词对应的编辑距离
    vector<int> editDis;
    //暂时存放最小编辑距离一样的候选词用来排序
    vector<pair<string,int>> tempWord;

    //集中索引对应的词汇
    options.clear();
    wordSet.clear();
    for(auto ch:chs)
    {
        cout<<ch<<endl;
        searchIndex(ch,wordSet); 
    }
    for(auto &a:wordSet)
    {
        options.push_back(a);
    }
    cout<<"索引查找完毕"<<endl;;
    //计算每个词汇的最小编辑距离
    editDis.clear();
    for(auto opt:options)
    {
        editDis.push_back(minDis(opt,word));
    }
    cout<<"编辑距离计算完毕"<<endl;;
    cout<<"待筛选候选词个数:"<<editDis.size()<<endl;
    if(options.size() <= 0)
    {
        res.clear();
        cout<<"查无候选词"<<endl;
        return;
    }
    //找出候选词
    int maxWord = editDis.size();
    for(int i = 0;i<RESULTAMOUNT&&i<maxWord;i++)
    {
        //记录最小距离
        int minW = editDis[0];
        //记录最小距离对应的词的序号
        int serial = 0;
        //递增序号
        int count = 0;
        //暂存候选词和编辑距离
        pair<string,int> temp;
        //遍历数组,找出最小编辑距离
        for(auto& a:editDis)
        {
            if(a<minW)
            {
                serial = count;
                minW = a;
            }
            ++count;
        }
        temp.first = options[serial];
        temp.second = minW;
        editDis.erase(editDis.begin()+serial);
        options.erase(options.begin()+serial);

        cout<<"候选词:"<<temp<<endl;

        //候选词数组为空或者存在编辑距离一样的候选词
        if(tempWord.empty()||tempWord.front().second == minW )
        {
            tempWord.push_back(temp);
            if(i == RESULTAMOUNT -1||i>=maxWord-1)
            {
                goto sort;
            }
        }
        //最小编辑距离改变,对一样的进行排序
        else
        {   
sort:    for(int i = 0;i<(int)tempWord.size();++i)
            {
                for(int j = i+1;j<(int)tempWord.size();++j)
                {
                    if(_wordList[tempWord[j].first]>_wordList[tempWord[i].first])
                    {
                        string t = tempWord[j].first;
                        tempWord[j].first = tempWord[i].first;
                        tempWord[i].first = t;
                    }
                }
            }
            for(auto &a:tempWord)
            {
                res.push_back(a.first);
            }
            tempWord.clear();
            tempWord.push_back(temp);
        }
    }
}
Dict dictionary;
int main()
{
    TcpServer setLink;
    string ip,msg;
    fstream configFile("../conf/config");
    //读取配置文件
    if(!configFile.is_open())
    {
        cout<<"读取配置文件失败"<<endl;
        return 0;
    }
    getline(configFile,ip);
    getline(configFile,msg);

    //建立tcp监听
    setLink.start(ip,atoi(msg.data()));
    cout<<"ip="<<ip<<endl;
    cout<<"port="<<msg<<endl;
    cout<<"tcp监听就绪:"<<endl;
    //建立词典和索引
    /*if(access(dictPath.data(),R_OK)==0 &&access(indexPath.data(),R_OK)==0)
    {
        cout<<"已存在词典文件,读取词典文件"<<endl;
        dictionary.readDict(dictPath,indexPath);
    }*/
    /*else
    {
        cout<<"创建词典文件"<<endl;
      */  
        bool isCh = false;
        while(!configFile.eof())
        {
            getline(configFile,msg);
            if(!strcmp(msg.data(),"chinese"))
            {
                isCh = true;
                continue;
            }
            if(strcmp(msg.data(),"end")==0)
            {
                break;
            }
            if(isCh)
            {
                cout<<"中文语料库"<<msg<<"已读取"<<endl;
                dictionary.createChDict(msg);
            }
            else
            {
                cout<<"英文语料库"<<msg<<"已读取"<<endl;
                dictionary.createEnDict(msg);
            }
        }/*
        cout<<"保存词典文件"<<endl;
        dictionary.outTOfile(dictPath,indexPath);
    }*/
    cout<<"词典已建立"<<endl;
    configFile.close();
    //启动线程
    TaskQueue tasks(MAXCLIENT);
    MyPool pool(MAXCLIENT,&tasks);
    pool.startPool();
    cout<<"线程池已启动"<<endl;

    int newfd;
    while(1)
    {
        newfd = setLink.accept();
        if(newfd<=0)
        {
            continue;
        }
        cout<<"接收到建立连接请求,连接已建立"<<endl;
        tasks.addTask(newfd);
    }

}
void WorkThread::run()
{
    int clientfd;
    TcpLink link;
    bool got =false;

    string searchword;
    vector<string> res;
    int ret;
    while(1)
    {
start:  got = taskList->getTask(clientfd);
        if(!got||clientfd<=0)
        {
            continue;
        }
        Cache cache;
        cache.readCache();
        link.set(clientfd);
        time_t start = time(NULL);
        time_t now;
        do
        {
            now = time(NULL);
            //10分钟写一次cache
            if((now-start)>=600)
            {
                cache.writeCache();
            }
            ret = link.recvMsg(searchword); 
            if(ret ==-1)
            {
                goto start;
            }
            if(strcmp(searchword.data(),"esc")==0)
            {
                break;
            }
            cout<<"收到搜索词汇"<<searchword<<endl;
            res.clear();
            //匹配候选关键词
            bool found = cache.getRes(searchword,res);
            if(!found)
            {
                res.clear();
                cout<<"开始搜索词典:"<<endl;
                dictionary.searchWord(searchword,res);
            }
            cout<<"搜索结果:"<<endl;
            for(auto a:res)
            {
                cout<<a<<endl;
            }
            //json格式传输
            Json jsonDoc;
            for(auto &a:res)
            {
                jsonDoc.addDoc("候选词",a);
            }
            int nowTime = time(NULL);
            char fileName[100];
            sprintf(fileName,"%d.json",nowTime);
            jsonDoc.toFile(fileName);
            ret = link.sendFile(fileName);
            if(ret ==-1)
            {
                goto start;
            }
        }while(true);
    }

}
