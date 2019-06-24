#include <fun.h>
#define MAXS 1000    
int main(int args,char *argv[])
{
    int sid = semget(IPC_PRIVATE,1,0600|IPC_CREAT);
    struct sembuf p,v;
    p.sem_op = -1;
    v.sem_op = 1;
    p.sem_num = 0;
    v.sem_num = 0;
    p.sem_flg = SEM_UNDO;
    v.sem_flg = SEM_UNDO;
     int  ret=semctl(sid,0,SETVAL,1);
    RET_CHECK(ret,-1,"semctl");

    int mp =shmget(IPC_PRIVATE,20,0600|IPC_CREAT);
    if(fork()>0)
    {
        int *np,add=0;
        np =(int *) shmat(mp,NULL,0);
        RET_CHECK(np,(int *)-1,"shmat")
        for(int i =0;i<MAXS;i++)\
        {
            semop(sid,&p,1);
            (*np)++;
            semop(sid,&v,1);
            add++;
        }
        printf("father:%d\n",add);
        wait(NULL);
        printf("result:%d\n",(*np));
        return 0;
    }
    else
    {
        int *np,add = 0;
        np = (int*)shmat(mp,NULL,0);
        RET_CHECK(np,(int *)-1,"shmat")
        for(int i =0;i< MAXS;i++)
        {
            semop(sid,&p,1);
            (*np)++;
            semop(sid,&v,1);
            add++;
        }
        printf("child:%d\n",add);
        exit(0);
    }
    return 0;
}

