#include <fun.h>

int main(int args,char *argv[])
{
    int mp =shmget(IPC_PRIVATE,20,0600|IPC_CREAT);
    if(fork()>0)
    {
        char *p;
        p =(char *) shmat(mp,NULL,0);
        RET_CHECK(p,(char *)-1,"shmat")
        strcpy(p,"How are you");
        wait(NULL);
        shmctl(mp, IPC_RMID, NULL);
        return 0;
    }
    else
    {
        char *p;
        p = (char*)shmat(mp,NULL,0);
        RET_CHECK(p,(char *)-1,"shmat")
        printf("%s\n",p);
        exit(0);
    }
    return 0;
}

