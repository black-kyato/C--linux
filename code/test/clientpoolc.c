#include <fun.h>
#include "pool.h"
int main(int args,char *argv[])
{
    struct winsize wsize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsize);
    
    ARG_CHECK(args,3);
    int sfd = tcp_connect(argv[1],atoi(argv[2]));
    RET_CHECK(sfd,-1,"client tcp connetc");

    printf("successfully connetc server.\n");

    Train_t train;
    int ret;
    //get filename and create file
    int len;
    ret =  recv(sfd,&len,4,0);
    RET_CHECK(ret,-1,"服务器已满负荷，请稍候再试");
    
    ret = recv(sfd,train.p,len,0);
    RET_CHECK(ret,-1,"getfilename");
    train.p[ret]='\0';
    int fd = open(train.p,O_RDWR|O_CREAT,0666);
    if(fd==-1)
    {
        printf("文件创建失败，可能存在同名文件!client exit.\n");
        close(sfd);
        return 0;
    }
    //get size of file 
    int size;
    ret = recv(sfd,&size,sizeof(int),0);
    RET_CHECK(fd,-1,"recv");
    printf("the size of file is %d\n",size);
    printf("start downloading file %s\n",train.p);
    
    void *buf = (void *)calloc(size,1);
    ret = recv(sfd,buf,size,MSG_WAITALL);
    write(fd,buf,ret);
    while(ret<size)
    {
        printf("!\n");
        memset(buf,0,size);
        int tre=recv(sfd,buf,size,MSG_WAITALL);
         write(fd,buf,tre);
         ret+=tre;
         printf("tre=%d\n",tre);
    }
        if(ret!=size)
        {
            printf("write error\n");
            close(fd);
            free(buf);
            return 0;
        }
        else
        {
            printf("download finished\n");
            free(buf);
            close(fd);
            return 0;
        }
}
