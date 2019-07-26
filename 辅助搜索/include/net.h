#pragma once
#include <fun.h>
#include "mutcond.h"
#include "pthread.h"
#include "dict.h"
#include "cache.h"
#include "task.h"
#include "json.h"
#define RESULTAMOUNT 5//返回的候选词的个数
#define MTU 4096
//服务器地址和端口
#define serverip "192.168.4.115"
#define serverport 2020


typedef enum{
    dead,
    rest,
    busy
}state;

typedef struct{
    pthread_t tid;
    state st;
}thread_info;


//信息报文
typedef struct
{
    int len;
    char buf[MTU];
}seg;

//文件传输函数
int recvseg(int sfd,seg *train);
int sendseg(int sfd,seg *train);
int send_file(int sfd,char *filename);
int recv_file(int sfd,string& filename);

//确认函数
int  send_ack(int sfd,int suc);
int recv_ack(int sfd);
//TCP连接
class TcpLink
{
private:
    int sfd;
    bool connect;
public:
    TcpLink(int fd=0):sfd(fd)
    {
        if(fd>0)
            connect = true;
        else
            connect = false;
    }
    void set(int fd)
    {
        sfd = fd;
        connect = true;
    }
    //失败返回-1
    int sendFile(char *filename)
    {
        int ret = 0;
        ret = send_file(sfd,filename);
        if(ret  == -1)
        {
            connect = false;
        }
        return ret;
    }
    //失败返回-1
    int recvFile(string& filename)
    {
        int ret;
        ret = recv_file(sfd,filename);
        if(ret  == -1)
        {
            connect = false;
        }
        return ret;
    }
    //失败返回-1
    int sendMsg(const char *msg)
    {
        int ret = 0;
        seg train;
        memset(&train,0,sizeof(train));
        strcpy(train.buf,msg);
        train.len = strlen(msg)+1;
        ret = sendseg(sfd,&train);
        if(ret  == -1)
        {
            connect = false;
        }
        return ret;
    }
    int sendMsg(std::string msg)
    {
        int ret = 0;
        ret = this->sendMsg(msg.data());
        if(ret  == -1)
        {
            connect = false;
        }
        return ret;
    }
    //失败返回-1
    int recvMsg(char *msg)
    {
        seg train;
        int ret;
        memset(&train,0,sizeof(train));
        ret = recvseg(sfd,&train);
        strcpy(msg,train.buf);
        if(ret  == -1)
        {
            connect = false;
        }
        return  ret; 
    }
    int recvMsg(string & msg)
    {
        seg train;
        int ret;
        memset(&train,0,sizeof(train));
        ret = recvseg(sfd,&train);
        msg = train.buf;
        if(ret  == -1)
        {
            connect = false;
        }
        return ret;
    }
    void closeLink()
    {
        close(sfd);
    }
    bool isConnect()
    {
        return connect;
    }
};
//服务器建立TCP连接
class TcpServer
{
private:
    int _clientFd;
    int _serverFd;
    bool _isConnect;
public:
    int clientFd() const
    {
        return _clientFd;
    }
    int serverFd() const
    {
        return _serverFd;
    }
    int start(std::string ip=serverip,int port = serverport)
    {    
        _serverFd = tcp_init(ip.data(),port);
        return 0;
    }
    int accept()
    {
        _clientFd =  tcp_accept(_serverFd);
        return _clientFd;
    }
    static int connect(string ip = serverip,int port = serverport)
    {
        int clientfd = tcp_connect(ip.data(),port);
        return clientfd;
    }
};
