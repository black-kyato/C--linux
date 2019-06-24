#include <fun.h>
#define MAXS 100000000
int main(int args,char *argv[])
{
    int mp =shmget(IPC_PRIVATE,20,0600|IPC_CREAT);
    if(fork()>0)
    {
        int *p,add = 0;
        p =(int *) shmat(mp,NULL,0);
        RET_CHECK(p,(int *)-1,"shmat")
        for(int i =0;i<MAXS;i++)\
        {
            (*p)++;
            add++;
        }
        printf("father:%d\n",add);
        wait(NULL);
        printf("result:%d\n",(*p));
        return 0;
    }
    else
    {
        int *p,add = 0;
        p = (int*)shmat(mp,NULL,0);
        RET_CHECK(p,(int *)-1,"shmat")
        for(int i =0;i< MAXS;i++)
        {
            add++;
            (*p)++;
        }
        printf("child:%d\n",add);
        exit(0);
    }
    return 0;
}

