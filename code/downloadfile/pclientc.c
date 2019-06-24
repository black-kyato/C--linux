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
    int getsize = 0;
    int pfd[2];
    pipe(pfd);

    while(1)
    {
        ret = splice(sfd,NULL,pfd[1],NULL,4096,SPLICE_F_MORE);
        RET_CHECK(ret,-1,"splice");
        if(ret == 0)
        {
            break;
        }
        getsize += ret;
        ret = splice(pfd[0],NULL,fd,NULL,ret,SPLICE_F_MORE);
        printf("\r");
        printf(">进度：%5.2f%s 已下载：%d",(double)getsize/size*100,"%",getsize);
        for(int i = 0;i<(double)getsize/size*(wsize.ws_col-35);i++)
        {
            printf("-");
        }
        printf(">");
    }
    printf("\r");
    printf(">进度：%5.2f%s 已下载：%d",(double)getsize/size*100,"%",getsize);
    for(int i = 0;i<(getsize/size)*(wsize.ws_row-35);i++)
    {
        printf("-");
    }
    printf(">\ndownload is finished\n");
    close(fd);
    close(sfd);
    return 0;

}
