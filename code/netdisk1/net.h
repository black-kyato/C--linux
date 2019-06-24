#pragma once
#include <fun.h>
//服务器地址和端口
#define serverip "192.168.4.115"
#define serverport 2020

//服务端最大同时连线数目
#define maxclient 10

//服务器目录
#define homepath "/home/jzk/netdiskServer"

//命令列表
typedef enum
{
    CD,
    LS,
    PUTS,
    GETS,
    REMOVE,
    PWD,
    EXIT,
    OTHER
}cmd_type;
//存放线程信息
typedef enum{
    dead,
    rest,
    busy
}state;

typedef struct{
    pthread_t tid;
    state st;
}thread_info;

//存放任务的队列
typedef struct sfd{
    int sockfd;
    struct sfd *next;
}node,*pnode;

typedef struct{
    pnode head,tail;
    int capacity;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}queue;

//组合结构体
typedef struct
{
    queue *p;
    thread_info *t;
}info;

//信息报文
typedef struct
{
    int len;
    char buf[4096];
}seg;
//线程池函数
void que_init(queue *p,int capacity);
int que_in(queue *p,int fd);
int que_out(queue *p);
void * mainthread(void *g);
void creatpool(info *pt);

//文件传输函数
int recvseg(int sfd,seg *train);
int sendseg(int sfd,seg *train);
int send_file(int sfd,char *fileinfo);
int recv_file(int sfd);
int printf_file_recv(int sfd);

//网盘函数
void print_main();
void print_login();
void print_ctrl(int num,char c);
//命令解析函数
cmd_type trans_cmd(seg *train);
char * get_argv(seg *train);

//确认函数
int  send_ack(int sfd,int suc);
int recv_ack(int sfd);


