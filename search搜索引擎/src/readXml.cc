#include "../inc/readXml.h"

using namespace std;
using namespace tinyxml2;

int max3(int a,int b,int c)
{
    int m1 = a>b?a:b;
    int m2 = b>c?b:c;
    return m1>m2?m1:m2;
}
//获取网页的特征字符串
void information::getToken()
{
    this->tokenkey.clear();
    string fro;
    char bac;
    fro.clear();
    int fop = 0;
    for(auto &ch:this->content)
    {
        if(ch == ','|| ch == ';')
        {
            for(int i = 0;i<5;++i)
            {
                if(fro.empty())
                    break;
                this->tokenkey.push_back(fro.back());
                fro.pop_back();
            }
            fro.clear();
            for(int i = 1;i <= 5;++i)
            {
                bac = this->content[fop+i]; 
                this->tokenkey.push_back(bac);
                if(bac == content.back())
                {
                    break;
                }
            }
        }
        else
        {
            fro.push_back(ch);
        }
        ++fop;
    }
}
//除去重复网页
void nosame(vector<information>& doc,string content)
{
    for(auto page = doc.begin();page!=doc.end();++page)
    {
        page->getToken();
        float rate = lenLCS(page->tokenkey,content)/page->content.size();
        if(rate>0.6)
        {
            doc.erase(page);
            break;
        }
    }
}
//LCS长度计算
int lenLCS(string s1,string s2)
{
    int len1 = s1.size();
    int len2 = s2.size();
    vector<vector<int>> lcsCM;
    lcsCM.resize(len1+1);
    for(auto &child:lcsCM)
    {
        child.resize(len2+1);
    }
    for(int i = 0;i<=len1;++i)
    {
        for(int j = 0;j<=len2;++j)
        {
            if(i == 0 || j == 0)
            {
                lcsCM[i][j] = 0;
                continue;
            }
            if(s1[i-1]==s2[j-1])
            {
                lcsCM[i][j] = lcsCM[i-1][j-1]+1;
            }
            else
            {
                lcsCM[i][j] = max3(lcsCM[i-1][j-1],lcsCM[i][j-1],lcsCM[i-1][j]);
            }
        }
    }
    return lcsCM[len1][len2];
}

//字符串匹配,除去正文中的不相关符号
void replace(string& str)
{
    regex pattern1("<.*?>");
    regex pattern2("&nbsp");
    str = regex_replace(str,pattern1,"");
    str = regex_replace(str,pattern2,"");
}
//输出网页信息到网页库文件和偏移库文件
void traverse(vector <information> & doc){

    std::ofstream dataFile("../data/ripepage.lib",std::ios::app);
    std::ofstream offsetFile("../data/offset.lib",std::ios::app);

    int old;
    for(auto child: doc){
        offsetFile<<dataFile.tellp()<<" "<<child.docid<<" ";
        old = dataFile.tellp();

        dataFile<<"<doc>"<<"<docid>"<<child.docid<<"</docid>"<<"<url>"<<child.link<<"</url>"<<"<title>"<<child.title<<"</title>";
        replace(child.content);
        dataFile<<"<content>\n"<<child.content<<"\n</content>";
        dataFile<<"</doc>"<<endl;

        offsetFile<< (int)dataFile.tellp() - old  <<endl;
    }
    dataFile.close();
    offsetFile.close();
}
//接收xml文件夹路径,解析文件夹下对应的xml文件
void translate(const char *dirname){

    DIR* dir = opendir(dirname); 

    dirent* fileInfo;
    //保存记录信息的数组 
    vector<information> inforDoc;

    XMLDocument doc;

    string filename;

    //暂存网页信息
    information  tmp;

    int coutDoc = 0;
    while((fileInfo = readdir(dir)))
    {
        //加载xml文件
        filename.clear();
        stringstream filecat(filename);
        filecat<<"../data/rss/"<<fileInfo->d_name;
        doc.LoadFile(filename.c_str());

        //加载文件判断
        if(0!=doc.Error()){
            cout<<"load file "<<filename<<" error"<<endl;
            return;
        }

        //获取根节点
        XMLElement*  root = doc.RootElement();

        //获取第一个channel节点
        XMLElement* channel = root->FirstChildElement("channel");
        while(channel)
        {
            XMLElement * title = channel->FirstChildElement("title");
            XMLElement * description = title->NextSiblingElement("description");
            XMLElement * url = channel->FirstChildElement("link");
            XMLElement * item = channel->FirstChildElement("item");

            tmp.title.clear();
            tmp.title.append(title->GetText());

            tmp.link.clear();
            tmp.link.append(url->GetText());

            tmp.content.clear();
            tmp.content.append(title->GetText());
            tmp.content.append("\n");
            tmp.content.append(description->GetText());
            tmp.docid = ++coutDoc;
            
            nosame(inforDoc,tmp.content);
            inforDoc.push_back(tmp);
            while(item){
                title = item->FirstChildElement("title");
                if(title == NULL)
                {
                    title = item->FirstChildElement("content")->FirstChildElement("p");
                }
                url = item->FirstChildElement("link");
                description = item->FirstChildElement("description");
                XMLElement * content = item->FirstChildElement("content");

                tmp.docid = ++coutDoc;
                tmp.title.clear();
                tmp.title.append(title->GetText());

                tmp.link.clear();
                tmp.link.append(url->GetText());

                tmp.content.clear();
                tmp.content.append(title->GetText());
                tmp.content.append("\n");
                tmp.content.append(description->GetText());
                tmp.content.append("\n");
                tmp.content.append(content->GetText());

                nosame(inforDoc,tmp.content);
                inforDoc.push_back(tmp);
                item = item->NextSiblingElement("item");
            }
            channel = channel->NextSiblingElement("channel");
        }
        traverse(inforDoc);
    }
    closedir(dir);
}

