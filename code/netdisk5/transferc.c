#include "net.h"
typedef struct
{
    int sfd;
    void *buf;
}mul_file;
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
    char md5[100]={0};
    Compute_file_md5(filename,md5);
    //发送MD5码
    train.len = strlen(md5)+1;
    strcpy(train.buf,md5);

    int ret = sendseg(sfd,&train);
    if(-1 ==ret)
    {
        printf("send md5 failed\n");
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
    RET_CHECK(ret,-1,"receive md5 error\n");
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

void send_ip(int sfd)
{
    seg train;
    memset(&train,0,sizeof(train));
    strcpy(train.buf,ip1);
    train.len = strlen(train.buf)+1;
    sendseg(sfd,&train);
    train.len = port1;
    sendseg(sfd,&train);

    strcpy(train.buf,ip2);
    train.len = strlen(train.buf)+1;
    sendseg(sfd,&train);
    train.len = port2;
    sendseg(sfd,&train);

    strcpy(train.buf,ip3);
    train.len = strlen(train.buf)+1;
    sendseg(sfd,&train);
    train.len = port3;
    sendseg(sfd,&train);
}

int recv_file_ip(int sfd,char *filename,size_t size)
{
    long file_fd[3];
    seg train;
    size_t fs[3];
    char ip[16] = {0};
    int port = 0;
    mul_file file_info[3];
    int fd = open(filename,O_RDWR|O_CREAT,0666);
    ftruncate(fd,size);
    file_info[0].buf = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    file_info[1].buf = (char *)file_info[0].buf+size/3;
    file_info[2].buf = (char *)file_info[1].buf+size/3;
    //与三个服务器建立连接
    for(int i = 0;i<3;i++)
    {
        recvseg(sfd,&train);
        strcpy(ip, train.buf);
        recvseg(sfd,&train);
        port = train.len;
        file_fd[i] = tcp_connect(ip,port);
        file_info[i].sfd = file_fd[i];
    }
    strcpy(train.buf,filename);
    train.len = 1+strlen(train.buf);
    //传文件名
    for(int i = 0;i<3;i++)
    {
        sendseg(file_fd[i],&train);      
    }
    //传文件长度
    fs[0] = size/3;
    fs[1] = size/3;
    fs[2] = size - fs[0] -fs[1];
    for(int i = 0;i<3;i++)
    {
        train.len = sizeof(size_t);
        memcpy(train.buf,fs+i,train.len);
        sendseg(file_fd[i],&train);
    }
    //传文件起始地址
    fs[0] = 0;
    fs[1] = size/3;
    fs[2] = 2*size/3;
    for(int i = 0;i<3;i++)
    {
        train.len = sizeof(size_t);
        memcpy(train.buf,fs+i,train.len);
        sendseg(file_fd[i],&train);
    }

    //接收文件
    pthread_t pt[3];
    for(int i = 0;i<3;i++)
    {
        pthread_create(pt+i,NULL,rec_file_thread,(void *)&file_info[i]);
    }
    //等待文件传输结束
    int ret[3];
    int flag = 1;
    for(int i = 0;i<3;i++)
    {
        pthread_join(pt[i],(void **)ret+i);
        if(ret[i]!=0)
        {
            printf("从服务器%d下载文件失败.\n",i);
            flag = 0;
        }
        close(file_fd[i]);
    }
    if(flag)
    {
        printf("文件下载成功.\n");         
    }
    else
    {
        printf("文件下载失败.\n");
    }
    munmap(file_info[0].buf,size);
    close(fd);
    return 0;
}
void * rec_file_thread(void *g)
{
    mul_file *mfp = (mul_file*)g;
    int sfd = mfp->sfd;
    char *p = (char*)mfp->buf;
    
    seg train;
    memset(&train,0,sizeof(train));
  
    int ret = 0;
    off_t pos = 0;
    do
    {
        ret= recvseg(sfd,&train);
        if(ret == -1)
        {
            pthread_exit((void *)-1);
        }
        memcpy(p+pos,train.buf, train.len);
        pos+=train.len;
    }while(train.len!=0);
    pthread_exit((void *)0);
}
