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
    printf("success.\n");
}
int main()
{
    int sfd = tcp_init(ip1,port1);
    long newfd;
    pthread_t tid;
    while(1)
    {
        newfd = tcp_accept(sfd);
        printf("begin\n");
        pthread_create(&tid,NULL,server_file,(void *)&newfd);
        printf("end\n");
    } 
    return 0;
}

