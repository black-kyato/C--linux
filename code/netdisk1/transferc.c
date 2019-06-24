#include "net.h"
//成功返回真实数据长度，失败返回-1
int recvseg(int sfd,seg *train)
{   
    int ret;
    ret = recv(sfd,&train->len,sizeof(int),MSG_WAITALL);
    RET_CHECK(ret,-1,"recv");
    ret = recv(sfd,train->buf,train->len,MSG_WAITALL);
#ifdef debug
    printf("receive buf=%s\n",train->buf);
#endif 
    RET_CHECK(ret,-1,"recv");
    return ret;
}
int sendseg(int sfd,seg *train)
{
    int ret;
    ret = send(sfd,&train->len,sizeof(int),MSG_NOSIGNAL);
    RET_CHECK(ret,-1,"send");
    ret = send(sfd,train->buf,train->len,MSG_NOSIGNAL);
#ifdef debug    
    printf("sendseg buf =%s\n",train->buf);
#endif   
    RET_CHECK(ret,-1,"send");
    return ret;
}

//发文件
int send_file(int sfd, char *fileinfo)
{
    char filename[100];
    int fsize = 0;
    sscanf(fileinfo,"%s%d",filename,&fsize);
    int fd = open(filename,O_RDWR,0666);
    RET_CHECK(fd,-1,"open");
    lseek(fd,fsize,SEEK_SET);
    seg train;

    //发送文件名
    train.len = strlen(filename)+1;
    strcpy(train.buf,filename);

    int ret = sendseg(sfd,&train);
    if(-1 ==ret)
    {
        printf("send filename failed\n");
        close(fd);
        return -1;
    }

    //发送文件大小
    struct stat statbuf;
    fstat(fd,&statbuf);
    train.len=statbuf.st_size - fsize;
    printf("剩余发送大小：%d\n",train.len);
    ret = send(sfd,&train.len,sizeof(int),0);
    if(-1 ==ret)
    {
        printf("send file size failed\n");
        close(fd);
        return -1;
    }
    printf("begin to transfer file %s\n",filename);
    if(train.len<1024*1024*100)
    {
        while((train.len = read(fd,train.buf,sizeof(train.buf)))>0)
        {
            sendseg(sfd,&train);
        }
    }
    else
    {
        char *pMap=(char*)mmap(NULL,train.len,PROT_READ,MAP_SHARED,fd,0);
        off_t pos=0;
        while(statbuf.st_size-pos>=(long)sizeof(train.buf))
        {
            train.len = sizeof(train.buf);
            memcpy(train.buf,pMap+pos,train.len);
            ret = sendseg(sfd,&train);
            RET_CHECK(ret,-1,"sendseg");
            pos+=sizeof(train.buf);
        }
        train.len = statbuf.st_size - pos;
        memcpy(train.buf,pMap+pos,train.len);
        sendseg(sfd,&train);
        train.len = 0;
        sendseg(sfd,&train);
    }
    printf("send file successfully\n");
    close(fd);
    return 0;
}
//收文件
int recv_file(int sfd)
{
    struct winsize wsize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsize);

    seg train;

    int ret = recvseg(sfd,&train);
    RET_CHECK(ret,-1,"receive filename error\n");
    train.buf[train.len] = '\0';

    int fd = open(train.buf,O_CREAT|O_RDWR|O_APPEND,0666);
    RET_CHECK(fd,-1,"open file");

    int filesize;
    ret =recv(sfd,&filesize,sizeof(int),0);
    RET_CHECK(ret,-1,"receive filesize error\n");
    printf("filesize = %d\n",filesize);
    int getsize = 0;

    printf("receive file %s start:\n",train.buf);
    ret = 0;
    while(getsize<filesize)
    {
        ret = recvseg(sfd,&train);
        RET_CHECK(ret,-1,"recvseg error");
        ret = write(fd,train.buf,train.len);
        RET_CHECK(ret,-1,"write");
        getsize+=ret;
        printf("\r");
        printf(">进度：%5.2f%s 已下载：%d",(double)getsize/filesize*100,"%",getsize);
        for(int i = 0;i<(double)getsize/filesize*(wsize.ws_col-40);i++)
        {
            printf("-");

        }
        printf(">");
    }
    printf("]\nreceive file successfully.\n");
    close(fd);
    return 0;
}
//打印接收的文件
int printf_file_recv(int sfd)
{
    seg train;

    int ret = recvseg(sfd,&train);
    RET_CHECK(ret,-1,"receive filename error\n");
    train.buf[train.len] = '\0';

    int filesize;
    ret =recv(sfd,&filesize,sizeof(int),0);
    RET_CHECK(ret,-1,"receive filesize error\n");
    int getsize = 0;

    while(getsize<filesize)
    {
        ret = recvseg(sfd,&train);
        RET_CHECK(ret,-1,"recvseg error");
        ret = write(1,train.buf,train.len);
        RET_CHECK(ret,-1,"write");
        getsize+=ret;
    }
    printf("打印完毕\n");
    return 0;
}
//发送确认
int  send_ack(int sfd,int suc)
{
    seg train;
    train.len = 8;
    if(suc)
    {
    strcpy(train.buf,"succeed");
    }
    else
    {
        strcpy(train.buf,"failed!");
    }
#ifdef debug
    printf("this is ack send\n");
#endif 
    return sendseg(sfd,&train);
}
int recv_ack(int sfd)
{
    seg train;
#ifdef debug 
    printf("this is ack receive\n");
#endif    
    recvseg(sfd,&train);
    if(strcmp(train.buf,"failed!")==0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
