#include <fun.h>
#include <vector>
#include <fstream>
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
        dataFile<<"<content>\n"<<child.content<<"\n</content>"<<endl;
        dataFile<<"</doc>"<<endl;
    }
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
    
    //非空判断
    if(NULL==channel){
        cout<<"get first chile element error "<<endl;
        return ;
    }

    XMLElement * title = channel->FirstChildElement("title");
    cout<<title->Name()<<endl;
    cout<<title->GetText()<<endl;

    XMLElement * description = title->NextSiblingElement("description");
    cout<<description->Name()<<endl;
    cout<<description->GetText()<<endl;
    
    XMLElement * language = description->NextSiblingElement("language");
    cout<<language->Name()<<endl;
    cout<<language->GetText()<<endl;
    
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
                cout<<"title-------------------------------------------"<<endl;
                cout<<item_child->GetText()<<endl;
            }else if(strcmp(item_child->Name(),"link")==0){
                tmp.link.clear();
                tmp.link.append(item_child->GetText());
                cout<<"link-------------------------------------------"<<endl;
                cout<<item_child->GetText()<<endl;
            }else if(strcmp(item_child->Name(),"description")==0){
                tmp.description.clear();
                tmp.description.append(item_child->GetText());
                cout<<"description------------------------------------"<<endl;
                cout<<item_child->GetText()<<endl;
            }else if(strncmp(item_child->Name(),"content",7)==0){
                tmp.content.clear();
                tmp.content.append(item_child->GetText());
                cout<<"content------------------------------------"<<endl;
                cout<<"..."<<endl;
            }
            item_child = item_child->NextSiblingElement();
        }
        inforDoc.push_back(tmp);
        item = item->NextSiblingElement("item");
    }
    cout<<"over"<<endl;
    traverse(inforDoc);
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
