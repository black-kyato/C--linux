#pragma once
#define FILENAME "test.txt"
typedef struct{

    int pid;
    int fd;
    int busy;
}pro_data;

// struct used to transfer data.
typedef struct{
    int len;
    char p[1024];
}Train_t;

int fork_child(int num,pro_data * p);
int transp(int);
int work(int);
int sendFD(int,int);
int recvFD(int,int *);
