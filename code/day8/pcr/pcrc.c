#include <fun.h>
int main(int args,char *argv[])
{
    int sid = semget(IPC_PRIVATE,2,0600|IPC_CREAT);
    RET_CHECK(sid,-1,"semget");
    struct sembuf p1,v1,p2,v2;
    p1.sem_op = -1;
    v1.sem_op = 1;
    p1.sem_num = 0;
    v1.sem_num = 0;
    p1.sem_flg = SEM_UNDO;
    v1.sem_flg = SEM_UNDO;
    
    p2.sem_op = -1;
    p2.sem_num = 1;
    p2.sem_flg =SEM_UNDO;
    v2.sem_num = 1;
    v2.sem_op =1;
    v2.sem_flg =SEM_UNDO;

    short arr[2]={0,10};
    int  ret=semctl(sid,1,SETALL,arr);
    RET_CHECK(ret,-1,"semctl");

    printf("现有产品数量：%d,尚可存储产品数量：%d\n",semctl(sid,0,GETVAL),semctl(sid,1,GETVAL));
    if(fork()>0)
    {
        printf("I am a producer!\n");
        while(1)
        {
            semop(sid,&p2,1);
            printf("一个产品已生产!\n");
            semop(sid,&v1,1);
            if(semctl(sid,0,GETVAL)>1)
            {
                printf("there are now %d products.\n",semctl(sid,0,GETVAL));
            }
            else
             {
                printf("there is now %d product.\n",semctl(sid,0,GETVAL));
            }
            sleep(1);
        }
        wait(NULL);
        return 0;
    }
    else
    {
        printf("I am a consumer!\n");
        while(1)
        {
            semop(sid,&p1,1);
             printf("一个产品已消费\n");
            semop(sid,&v2,1);
            sleep(2);
        }
    }
    return 0;
}

