#pragma once

#include <fun.h>
 using namespace std;
class Json
{
private:
    vector<pair<string,string>> docs; 
public:
    void addDoc(pair<string,string> doc)
    {
        docs.push_back(doc);
    }
    void addDoc(string s1,string s2)
    {
        pair<string ,string> t(s1,s2);
        docs.push_back(t);
    }
    void toFile(char* filename)
    {
        fstream jsonFile;
        jsonFile.open(filename,ios::out);
        jsonFile<<"{";
        for(auto & doc:docs)
        {
            jsonFile<<"\""<< doc.first<<"\":";
            jsonFile<<"\""<< doc.second<<"\"";
            if(doc!=docs.back())
            {
                jsonFile<<",";
            }
        }
        jsonFile<<"}"<<endl;
        jsonFile.close();
    }
    void readFile(const char *filename)
    {
        fstream jsonFile;
        jsonFile.open(filename,ios::in);
        pair<string,string> doc;
        char ch;
        jsonFile>>ch;
        bool isFirst = true;
        while(ch!='}')
        {
            jsonFile>>ch;
            if(ch==','||ch=='}')
            {
                docs.push_back(doc);
                doc.first.clear();
                doc.second.clear();
                isFirst = true;
            }
            else
            {
                if(ch=='"')
                {
                    continue;
                }
                if(ch==':')
                {
                    isFirst = false;
                    continue;
                }
                if(isFirst)
                {
                    doc.first.push_back(ch);
                }
                else
                {
                    doc.second.push_back(ch);
                }
            }
        }
    }
    void display()
    {
       for(auto &a:docs)
       {
           if(a.first.empty())
           {
               continue;
           }
            cout<<a.first<<":"<<a.second<<endl;
       }
    }
};
