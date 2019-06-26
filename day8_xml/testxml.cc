#include <fun.h>
#include <vector>
#include <fstream>
#include <regex>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;
//保存解析得到的信息
class information{
public:
    string channel;
    string title;
    string link;
    string description;
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
//输出RSS文件到dat文件
void traverse(vector <information> & doc){

    std::ofstream dataFile("pagelib.dat",std::ios::app);
    int count  = 1;
    for(auto child: doc){
        dataFile<<"<doc>"<<endl;
        dataFile<<"<docid>"<<count<<"</docid>"<<endl;
        ++count;
        dataFile<<"<title>"<<child.title<<"</title>"<<endl;
        dataFile<<"<link>"<<child.link<<"</link>"<<endl;
        dataFile<<"<description>"<<child.description<<"</description>"<<endl;
        replace(child.content);
        dataFile<<"<content>\n"<<child.content<<"\n</content>"<<endl;
        dataFile<<"</doc>"<<endl;
    }
    dataFile.close();
}
//接收文件名,解析对应的RSS文件
void translate(const char *filename){
    //保存记录信息的数组
    vector<information> inforDoc;
    XMLDocument doc;
    doc.LoadFile(filename);

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

int main()
{
    char filename[1024];
    cout<<"输入要解析的xml文件名:"<<endl;
    cin>>filename;
    cout<<"开始解析"<<endl;
    translate(filename);
    cout<<"解析完成"<<endl;
    return 0;
}
