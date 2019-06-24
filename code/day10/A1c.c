#include <fun.h>

struct MES
{
    int type;
    char ms[1024];
};
int main(int args,char *argv[])
{

    int mid,sid;


    mid =  shmget(1000,2048,0666|IPC_CREAT);//申请共享内存
    struct MES *cp = (struct MES *)shmat(mid,NULL,0);//映射共享内存
    RET_CHECK(cp,(struct MES *)-1,"shmat")

        //打开信号量队列
        sid = semget(2000,0,0666);
    RET_CHECK(sid,-1,"semget")
        struct sembuf p0,v0;
    p0.sem_op = -1;
    p0.sem_num = 0;
    p0.sem_flg = 0;
    v0.sem_op = 1;
    v0.sem_flg = 0;
    v0.sem_num = 0;

    printf("消息记录:\n");
    while(1)
    {
        if(cp->type==1)
        {
            if(strlen(cp->ms)>0)
                printf("chatterA:\n%s\n",cp->ms);
            semop(sid,&p0,1);
            memset(cp,0,sizeof(struct MES));
            semop(sid,&v0,1);
        }
        else
        {
            if(cp->type ==2){
                if(strlen(cp->ms)>0)
                    printf("chatterB:\n%s\n",cp->ms);
                semop(sid,&p0,1);
                memset(cp,0,sizeof(struct MES));
                semop(sid,&v0,1);
            }
        }
    }
}
