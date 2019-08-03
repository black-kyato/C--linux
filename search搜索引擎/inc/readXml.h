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
    string content;
    string tokenkey;
    void getToken();
};

//字符串匹配,除去正文中的不相关符号
void replace(string& str);
//输出网页信息到网页库文件和偏移库文件
void traverse(vector <information> & doc);
//接收xml文件夹路径,解析文件夹下对应的xml文件
void translate(const char *dirname);
//求lcs长度
int lenLCS(string s1,string s2);
