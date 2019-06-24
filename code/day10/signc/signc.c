#include <fun.h>
void sighandle1(int sig,siginfo_t *pinfo,void *p)
{
    printf("信号处理%d:打印0-9\n",sig);
    for(int i=0;i<10;i++)
    {
        printf("%d\n",i);
        sleep(1);
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
    struct sigaction sigm;
    sigm.sa_sigaction = sighandle1;
    sigm.sa_flags = SA_NODEFER|SA_SIGINFO;
    sigaction(2,&sigm,NULL);
    sigm.sa_sigaction = sighandle2;
    sigaction(3,&sigm,NULL);
    while(1);
    return 0;
}

