#include <fun.h>
#include <vector>
#include <fstream>
#include <regex>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

//保存解析得到的网页信息
class information{
public:
    int docid;
    string title;
    string link;
    string  content;
};

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
        //replace(child.content);
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

    dirent* fileInfo = readdir(dir);
    //保存记录信息的数组 
    vector<information> inforDoc;

    while(fileInfo)
    {
        fileInfo = readdir(dir);
        
        XMLDocument doc;
        doc.LoadFile(fileInfo->d_name);

        //加载文件判断
        if(0!=doc.Error()){
            cout<<"load file error"<<endl;
            return;
        }

        //获取根节点
        XMLElement*  root = doc.RootElement();
        //获取第一个channel节点
        XMLElement* channel = root->FirstChildElement("channel");
        int channelCount = 0;
        while(channel){
            //打开dat文件,写入channel的头部信息 
            std::ofstream dataFile("pagelib.dat",std::ios::app);
            ++channelCount;
            cout<<"解析channel"<<channelCount<<endl;
            dataFile<<"<channel>"<<endl;
            dataFile<<"<channelID>"<<channelCount<<"<channelID>"<<endl;
            XMLElement * title = channel->FirstChildElement("title");
            cout<<title->Name()<<endl;
            cout<<title->GetText()<<endl;
            dataFile<<"<title>"<<title->GetText()<<"</title>"<<endl;
            //防止tile不存在导致cout标志位错误,下同
            cout.clear();

            XMLElement * description = title->NextSiblingElement("description");
            cout<<description->Name()<<endl;
            cout<<description->GetText()<<endl;
            dataFile<<"<description>"<<description->GetText()<<"</description>"<<endl;
            cout.clear();

            XMLElement *time = description->NextSiblingElement("lastBuildDate");
            cout<<time->Name()<<endl;
            cout<<time->GetText()<<endl;
            dataFile<<"<lastBuildDate>"<<time->GetText()<<"</lastBuildDate>"<<endl;
            cout.clear();

            XMLElement * language = description->NextSiblingElement("language");
            cout<<language->Name()<<endl;
            cout<<language->GetText()<<endl;
            dataFile<<"<language>"<<language->GetText()<<"</language>"<<endl;
            cout.clear();

            //头部信息处理完毕,开始处理item
            XMLElement * item=language->NextSiblingElement("item");
            int i = 0;
            while(item){
                information  tmp;
                cout<<"sucess in item "<<i<<endl;
                i++;
                XMLElement *item_child= item->FirstChildElement();
                while(item_child){
                    if(strcmp(item_child->Name(),"title")==0){
                        tmp.title.clear();
                        tmp.title.append(item_child->GetText());
                        cout<<"解析title-------------------------------------------"<<endl;
                        cout<<item_child->GetText()<<endl;
                    }else if(strcmp(item_child->Name(),"link")==0){
                        tmp.link.clear();
                        tmp.link.append(item_child->GetText());
                        cout<<"解析link-------------------------------------------"<<endl;
                        cout<<item_child->GetText()<<endl;
                    }else if(strcmp(item_child->Name(),"description")==0){
                        tmp.description.clear();
                        tmp.description.append(item_child->GetText());
                        cout<<"解析description------------------------------------"<<endl;
                        cout<<item_child->GetText()<<endl;
                    }else if(strncmp(item_child->Name(),"content",7)==0){
                        tmp.content.clear();
                        tmp.content.append(item_child->GetText());
                        cout<<"解析content------------------------------------"<<endl;
                        cout<<"..."<<endl;
                    }
                    item_child = item_child->NextSiblingElement();
                }
                inforDoc.push_back(tmp);
                item = item->NextSiblingElement("item");
            }
            traverse(inforDoc);
            dataFile<<"</channel>"<<endl;
            dataFile.close();
            channel = channel->NextSiblingElement("channel");
        }
    }

    closedir(dir);
}

