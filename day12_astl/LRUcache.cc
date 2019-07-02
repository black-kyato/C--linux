#include <fun.h>
using namespace std;

class LRUCache {
private:
    struct CacheNode {
        int key;
        int value;
        CacheNode(int k, int v): key(k), value(v){}
    };
    int _capacity;
    list<CacheNode> cache;
    unordered_map<int,list<CacheNode>::iterator> getValue;
public:
    LRUCache(int capacity) 
    {
        _capacity = capacity;
    }
    int get(int key) 
    {
        list<CacheNode>::iterator keyNode;
        
        if(getValue.find(key) == getValue.end())
        {
            return -1;
        }
        //获取key对应的迭代器
        keyNode = getValue.find(key)->second;
        //保存迭代器指向的元素
        CacheNode temp = *keyNode;
        //将该元素从原有位置删除
        cache.erase(keyNode);
        //重新插入队列头部,表明最近刚刚访问过
        cache.push_front(temp);
        //更新哈希map中的迭代器
        getValue.find(key)->second = cache.begin();
        return  temp.value;
    }
    void put(int key, int value) 
    {
        CacheNode newNode(key,value);
            cache.push_front(newNode);
            pair<int,list<CacheNode>::iterator> newInfo(key,cache.begin());
            getValue.insert(newInfo);
        if((int)cache.size()>_capacity)
        {
            getValue.erase(cache.back().key);
            cache.pop_back();
        }
    }
};
int main()
{
    LRUCache cache(2);
    cache.put(1,1);
    cache.put(2,2);
    cache.put(3,3);
   
    cout<<cache.get(1)<<endl;
    cout<<cache.get(2)<<endl;
    cout<<cache.get(3)<<endl;
    return 0;
}
