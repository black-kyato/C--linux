#include <fun.h>

struct msgbuf{
    long mtype;
    char mtext[2048];
};
int main()
{
    int mid = msgget(1000,0666|IPC_CREAT);
    RET_CHECK(mid,-1,"msgget");
    struct msgbuf mbfs = {0}, mbfr = {0};
    mbfs.mtype = 1 ;
    int ret = 0;
    printf("this is chatter No.1:\n");
    if(fork()>0)
    {
        while(1)
        {
            memset(mbfs.mtext,0,2048);
            gets(mbfs.mtext);
            mbfs.mtype = 1;
            ret  = msgsnd(mid,&mbfs,2048,0);
           if(strcmp(mbfs.mtext,"再见！")==0)
           {
               break;
           }
           if(ret == -1)
               break;
        }
        wait(NULL);
        return 0;   
    }
    else
    {
        while(1)
        {
            memset(mbfr.mtext,0,2048);
            if((ret =msgrcv(mid,&mbfr,2048,2,0))!=-1)
            {
                printf("\033[31;1m对方：\n%s\033[0m\n",mbfr.mtext);
                if(strcmp(mbfr.mtext,"再见！")==0)
                {
                    msgctl(mid,IPC_RMID,NULL);
                    exit(0);
                }
            }
            else
            {
                return 0;
            }
        }
    }
}

