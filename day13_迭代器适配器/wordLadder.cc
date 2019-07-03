#include <fun.h>
using namespace std;


/*
 * 思路:利用迪杰斯特拉算法,针对beginWord遍历一遍单词表,可以一步到位的标识长度为1
 * 把长度为1的单词加入遍历队列中,队列中存储所有已知到达路径的单词和路径长度,
 * 如果endWord出现在遍历队列里,则返回对应长度.
 * 如果不在,则继续用遍历队列中的每个单词遍历单词表,如果长度小于单词表中的长度,则更新
 * 单词表中的路径长度
 * */
#define MAXLENGTH 1024
class Solution
{
    class Path
    {
        public:
            int length;
            vector<string> path;
    };
private:
    //保存所有单词
    map<string,int> wordlist;
    //保存已知单词
    map<string,Path> knownlist;
    string beginWord;
    string endWord;
    //保存每次遍历的单词
    stack<string> stacklist;
public:
    //生成问题
    void input()
    {
        cout<<"input beginWord:";
        cin>>beginWord;
        cout<<"input endWord:";
        cin>>endWord;
        cout<<"input wordList,end with a !:";
        string word;
        cin>>word;
        while(word!="!")
        {
            wordlist.insert(pair<string,int>(word,MAXLENGTH));
            cin>>word;
        }
    }
    //判断到一个单词的路径是否已知,即单词是否在已知列表里
    bool unknown(string word)
    {
        map<string,Path>::iterator tp = knownlist.find(word);
        return (tp == knownlist.end());
    }
    //用一个单词遍历单词表
    bool listMap( map<string,Path>::iterator wordPair)
    {
        for(auto &a:wordlist)
        {
            //未知节点距离为1的记录路径长度,并加入已知列表
            if(a.second == MAXLENGTH) 
            {
                if(oneStep(a.first,wordPair->first))
                {
                    a.second = wordPair->second.length+1;   
                    Path tp;
                    tp.length = a.second;
                    for(auto a:wordPair->second.path)
                    {
                        tp.path.push_back(a);
                    }
                    tp.path.push_back(a.first);
                    knownlist.insert(pair<string,Path>(a.first, tp));
                    stacklist.push(a.first);
                }
            }
        }
        return !unknown(endWord);
    }
    //解决问题
    void getLength()
    {
        bool find = false;
        Path tp;
        tp.length = 1;
        tp.path.push_back(beginWord);
        knownlist.insert(pair<string,Path>(beginWord,tp));
        stacklist.push(beginWord);
        string keyword;
        while(!stacklist.empty())
        {
            keyword = stacklist.top();
            stacklist.pop();

            find = listMap(knownlist.find(keyword));
            if(find)
            {
                cout<<"length:"<<knownlist.find(endWord)->second.length<<endl;
                cout<<"path:"<<endl;
                for(auto a:knownlist.find(endWord)->second.path)
                {
                    cout<<a<<endl;
                }
                return;
            }
        }
        cout<<"length:0(No Path)"<<endl;
    }
    //判断是否相差一个字母
    bool oneStep(string word1,string word2)
    {
        int count = 0;
        for(int i = 0;i<(int)word1.size();i++)
        {
            if(word1[i]!=word2[i])
            {
                count++;
            }
        }
        return count==1?true:false;
    }
};

int main()
{
    Solution    test;
    test.input();
    test.getLength();
    return 0;
}

