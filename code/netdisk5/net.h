#pragma once
#include <fun.h>
//服务器地址和端口
#define serverip "192.168.4.115"
#define serverport 2020
#define ip1 "192.168.4.115"
#define ip2 "192.168.4.115"
#define ip3 "192.168.4.115"
#define port1 2121
#define port2 2222
#define port3 2323
//服务端最大同时连线数目
#define maxclient 10

//服务器目录
#define homepath "/home/jzk/netdiskServer"

//命令列表
typedef enum
{
    MKDIR,
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
    int i_num;
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
int lsmysql(char *path,seg *train);

//确认函数
int  send_ack(int sfd,int suc);
int recv_ack(int sfd);

//检查数据库中是否存在传入的用户名,存在返回1，不存在返回0，出错返回-1
int findUser(char *user);
//传出对应用户名的salt和密码密文
int findPass(char *user,char *salt,char *passwd);
//写入新用户
int adduser(char *user,char *pass);
//生成随机salt
void randsalt(char *salt);
//给用户数据库发送命令
int mysqlUser(MYSQL *mp, char *cmd);
//写入日志
int writeLog(char *user, char *option,char *time,int done);

//查找是否有相同文件
int findfile(char *user,char * path,char *filename,char *md5);
//在数据库中添加文件记录
int addfile(char *username,char *path,char *filename,char type,char *md5);
//查找是否存在md5值对应的文件
int findmd5(char *md5);
//删除文件
int delfile(char *user,char* path,char *filename);
//计算MD5
int Compute_file_md5(const char *file_path, char *value);

//发送下载服务器的地址
void send_ip(int sfd);
//从多个服务器下载文件
int recv_file_ip(int sfd,char* filename, size_t size);
//线程函数接收文件
void * rec_file_thread(void *g);
