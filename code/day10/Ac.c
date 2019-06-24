#include <fun.h>

struct MES{
    int type;
    char ms[1024];
};
int pid;
int sid;
int mid;
int fdr;
int fdw;
void exitsig(int sig)
{
    close(fdr);
    close(fdw);

    semctl(sid,4,IPC_RMID);
    shmctl(mid,IPC_RMID,NULL);

    printf("资源清理完毕!\n");

    kill(pid,9);
}

int main(int args,char *argv[])
{
    //注册信号处理函数
    signal(2,exitsig);
    //获取进程号
    pid = getpid();

    //创建管道
    ARG_CHECK(args,3);
    fdr = open(argv[1],O_RDONLY);//存放要接收的信息的文件
    fdw = open(argv[2],O_WRONLY);//存放要发送的信息的文件
    printf("chatter A,fdr=%d,fdw=%d\n",fdr,fdw);
    char sentence[1024] = {0};

    fd_set rdset;//文件描述符数组，数组中的文件描述符需要手动添加
    struct timeval tc;//等待时间

    mid =  shmget(1000,2048,0666|IPC_CREAT);//申请共享内存
    struct MES *cp = (struct MES *)shmat(mid,NULL,0);//映射共享内存
    RET_CHECK(cp,(struct MES *)-1,"shmat")

    sid = semget(2000,1,0666|IPC_CREAT);

    semctl(sid,0,SETVAL,1);

    struct sembuf p0,v0;
    p0.sem_op = -1;
    p0.sem_num = 0;
    p0.sem_flg = 0;
    v0.sem_op = 1;
    v0.sem_flg = 0;
    v0.sem_num = 0;

    int ret;
    while(1)
    {
        FD_ZERO(&rdset);//清空描述符集合
        FD_SET(STDIN_FILENO,&rdset);//将标准输入流放入集合中
        FD_SET(fdr,&rdset);     //将收到消息存放文件放入集合中
        tc.tv_sec =999;
        tc.tv_usec = 0;

        int fdReadyNum = select(fdr+1,&rdset,NULL,NULL,&tc);

        //监控到就绪描述符
        if(fdReadyNum>0)
        {
            //标准输入就绪，则接收字符串写入fdw中
            if(FD_ISSET(STDIN_FILENO,&rdset))
            {
                memset(sentence,0,sizeof(sentence));
                //从输入中读入消息
                read(0,sentence,sizeof(sentence)-1);
                semop(sid,&p0,1);
                strcpy(cp->ms,sentence);
                cp->type = 1;
                semop(sid,&v0,1);
                ret = write(fdw,sentence,strlen(sentence)+1);
                RET_CHECK(ret,-1,"write")
            }
            //fdr文件可读，代表管道内有新消息
            if(FD_ISSET(fdr,&rdset))
            {
                memset(sentence,0,sizeof(sentence));
                read(fdr,sentence,sizeof(sentence)-1);
                semop(sid,&p0,1);
                strcpy(cp->ms,sentence);
                cp->type = 2;
                semop(sid,&v0,1);
            }
        }
    }
    return 0;
}

