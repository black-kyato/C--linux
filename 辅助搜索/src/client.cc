#include "../include/net.h"


int main()
{
    int sfd = TcpServer::connect();
    TcpLink link(sfd);
    string searchWord;
    string fileName;
    cout<<"请输入搜索关键词,输入esc结束:"<<endl;
    while(true)
    {
        cin>>searchWord;
        link.sendMsg(searchWord);
        if(searchWord == (string)"esc")
        {
            break;
        }
        link.recvFile(fileName);
        Json jsonDoc;
        jsonDoc.readFile(fileName.data());
        jsonDoc.display();
    }
    return 0;
}

