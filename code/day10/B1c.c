#include <fun.h>

struct msgbuf{
    long mtype;
    char mtext[1024];
};
int mid;
int main(int args,char *argv[])
{
    //打开消息队列
    mid = msgget(3000,0666);
    RET_CHECK(mid,-1,"msgget")
    struct msgbuf mbf = {0};

    int ret;
    printf("消息记录:\n");
    while(1)
    {
        memset(mbf.mtext,0,1024);
        ret = msgrcv(mid,&mbf,1024,0,0);
        RET_CHECK(ret,-1,"msgrcv")
        if(mbf.mtype == 1&&ret!=0)
        {
            printf("chatter B:\n%s\n",mbf.mtext);
        }
        else
        {
           if(strlen(mbf.mtext)>0)
            printf("chatter A:\n%s\n",mbf.mtext);
        }
    }
    return 0;
}

