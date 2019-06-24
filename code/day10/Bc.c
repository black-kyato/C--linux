#include <fun.h>

struct msgbuf{
    long mtype;
    char mtext[1024];
};
int pid;
int mid;
int fdw,fdr;
void exitsig(int sig)
{
    msgctl(mid,IPC_RMID,NULL);
    close(fdw);
    close(fdr);

    printf("资源清理完毕!\n");
    kill(pid,9);
}
int main(int args,char *argv[])
{
    //注册信号
    signal(2,exitsig);

    ARG_CHECK(args,3);

    fdw = open(argv[1],O_WRONLY);//存放发送信息的文件
    fdr = open(argv[2],O_RDONLY);//存放接收信息的文件
    printf("chatter B,fdr=%d,fdw=%d\n",fdr,fdw);     
    char sentence[1024] = {0};

    fd_set rdset;//文件描述符数组，数组中的文件描述符需要手动添加
    int fdReadyNum;//计数
    
    //创建消息队列
    mid = msgget(3000,0666|IPC_CREAT);
    RET_CHECK(mid,-1,"msgget")
    struct msgbuf mbfs = {0},mbfr ={0};
    mbfs.mtype = 1;//代表发出去的消息
    mbfr.mtype = 2;//代表接收的消息

    int ret;
    while(1)
    {
        FD_ZERO(&rdset);//清空描述符集合
        FD_SET(STDIN_FILENO,&rdset);//将标准输入流放入集合中
        FD_SET(fdr,&rdset);     //将收到的消息存放文件放入集合中

        fdReadyNum = select(fdr+1,&rdset,NULL,NULL,NULL);
        //标准输入就绪，则接收字符串写入fdw中
        if(FD_ISSET(STDIN_FILENO,&rdset))
        {
            memset(sentence,0,sizeof(sentence));
            
            read(0,sentence,sizeof(sentence)-1);
            
            strcpy(mbfs.mtext,sentence);
            ret = msgsnd(mid,&mbfs,1024,0);
            RET_CHECK(ret,-1,"msgsnd");
            ret=write(fdw,sentence,strlen(sentence)+1);
            RET_CHECK(ret,-1,"write");
        }
        //fdr文件可读，代表管道内有新消息
        if(FD_ISSET(fdr,&rdset))
        {
            memset(sentence,0,sizeof(sentence));
            read(fdr,sentence,sizeof(sentence));
            strcpy(mbfr.mtext,sentence);
            ret = msgsnd(mid,&mbfr,1024,0);
        }
    }
    return 0;
}

