#pragma once
#include <fun.h>
#include "mutcond.h"
#define MAXCACHE 1000
#define CACHEPATH "../conf/cache.txt"
using namespace std;
class Cache
{
private:
    int _capacity;//cache容量
    unordered_map<string,vector<string>> _cache;
    unordered_map<string,vector<string>> _newCache;
    list<unordered_map<string,vector<string>>::iterator> _LRUlist; 
    MutexLock _mutex;
public:
    Cache(int capacity = MAXCACHE):_capacity(capacity){}
    void readCache()
    {
        fstream  cache;
        cache.open(CACHEPATH,ios::in);
        if(cache.is_open())
        {
            cout<<"成功读取cache文件"<<endl;
        }
        int count = 0;
        string word;
        string line;
        string opt;
        vector<string> options;
        while(count<_capacity && !cache.eof())
        {
            getline(cache,line);
            stringstream lineOut(line);
            lineOut>>word;
            while(!lineOut.eof())
            {
                lineOut>>opt;
                options.push_back(opt);
            }
            pair<string,vector<string>> res(word,options);
            _cache.insert(res);
            _LRUlist.push_back(_cache.find(word));
        }
        cache.close();
    }
    bool getRes(string word,vector<string>& res)
    {
        cout<<"查找缓存"<<endl;
        if(_cache.find(word) == _cache.end())
        {
            return false;
        }
        else
        {
            res = _cache[word];
            _LRUlist.remove(_cache.find(word));
            _LRUlist.push_back(_cache.find(word));
        }
        return true;
    }
    void LRUCache(string word,vector<string> res)
    {
        if(_cache.find(word) == _cache.end())
        {
            pair<string,vector<string>> add(word,res);
            _cache.insert(add);
            _newCache.insert(add);
            if((int)_cache.size()>_capacity)
            {
               _cache.erase(_LRUlist.front());
               _LRUlist.pop_front();
            }
        }
    }
    void writeAll()
    {
        fstream cache;
        this->_mutex.lock();
        cache.open(CACHEPATH,ios::out);
        for(auto &a:_cache)
        {
            cache<<a.first<<" ";
            for(auto &s:a.second)
            {
                cache<<s<<" ";
            }
            cache<<endl;
        }
        cache.close();
        this->_mutex.unlock();
    }
    void writeNew()
    {
        fstream cache;
        this->_mutex.lock();
        cache.open(CACHEPATH,ios::out|ios::app);
        for(auto &a:_newCache)
        {
            cache<<a.first<<" ";
            for(auto &s:a.second)
            {
                cache<<s<<" ";
            }
            cache<<endl;
        }
        cache.close();
        this->_mutex.unlock();
    }
    void writeCache()
    {
        if((int)_cache.size()>=_capacity)
        {
            writeAll();
        }
        else
        {
            writeNew();
        }
    }
};
