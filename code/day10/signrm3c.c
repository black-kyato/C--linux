#include <fun.h>
struct sigaction sigm;
void sighandle1(int sig,siginfo_t *pinfo,void *p)
{
    printf("信号处理%d:打印0-5\n",sig);
    sigset_t pending;
    for(int i=0;i< 6;i++)
    {
        printf("%d\n",i);
        sleep(1);
        sigpending(&pending);
       if(sigismember(&pending,3))
        {
            printf("信号3已被阻塞！\n");
        }
    }

    printf("信号%d处理结束!\n",sig);
}
void sighandle2(int sig,siginfo_t *pinfo,void *p)
{
    printf("信号处理%d:\n中断\n",sig);
    int i = 0;
    while(i++<5)
    {
        sleep(1);
        printf("!\n");
    }
    printf("信号%d处理结束!\n",sig);
}
int main(int args,char *argv[])
{
    sigemptyset(&sigm.sa_mask);
    sigaddset(&sigm.sa_mask,3);
    sigm.sa_sigaction = sighandle1;
    sigm.sa_flags = SA_NODEFER|SA_SIGINFO;
    sigaction(2,&sigm,NULL);
    sigm.sa_sigaction = sighandle2;
    sigaction(3,&sigm,NULL);
    while(1);
    return 0;
}

