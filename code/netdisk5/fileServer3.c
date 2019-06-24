#include "net.h"

void *server_file(void *g)
{
    long sfd =*(long *)g;
    seg train;
    char filename[100]={0};
    size_t filesize = 0;
    size_t start = 0;
    
    int fd;
    
    recvseg(sfd,&train);
    strcpy(filename,train.buf);
   
    recvseg(sfd,&train);
    memcpy(&filesize, train.buf,train.len);

    recvseg(sfd,&train);
    memcpy(&start, train.buf,train.len);
    
    fd = open(filename,O_RDONLY);
    lseek(fd,start,0);

    //传文件
    int remain = filesize;
    train.len = sizeof(train.buf);
    while(remain>0)
    {
        if(remain>train.len)
        {
            remain = remain -train.len;
        }
        else
        {
            train.len =remain;
            remain = 0;
        }
        read(fd,train.buf, train.len);
        sendseg(sfd,&train);
    }
    train.len = 0;
    sendseg(sfd,&train);
    close(fd);
    close(sfd);
    pthread_exit((void*)0);
}
int main()
{
    int sfd = tcp_init(ip3,port3);
    long newfd;
    pthread_t tid;
    while(1)
    {
        newfd = tcp_accept(sfd);
        pthread_create(&tid,NULL,server_file,(void *)&newfd);
    } 
    return 0;
}

