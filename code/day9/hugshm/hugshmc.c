#include <fun.h>
#define SHM_HUGE_SHIFT 26
#define SHM_HUGE_2MB 21<<SHM_HUGE_SHIFT

int main(int args,char *argv[])
{
    int mp = shmget(IPC_PRIVATE,20,SHM_HUGE_2MB|SHM_HUGETLB|0600|IPC_CREAT);
    RET_CHECK(mp,-1,"shmget");
    char * buf;
    if(fork()>0)
    {
        buf = (char *)shmat(mp,NULL,0);
        RET_CHECK(buf,(char *)-1,"shmat");
        strcpy(buf,"hello world!");
        wait(NULL);
        shmctl(mp,IPC_RMID,NULL);
        exit(0);
    }
    else
    {
        buf = (char *)shmat(mp,NULL,0);
       RET_CHECK(buf,(char *)-1,"shmat"); 
       printf("孩子：\n"); 
       puts(buf);
    }
    return 0;
}

