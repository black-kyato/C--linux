#include <fun.h>
void sighandle1(int sig)
{
    printf("信号处理%d:打印0-9\n",sig);
    for(int i=0;i<10;i++)
    {
        printf("%d\n",i);
        sleep(1);
    }
}
void sighandle2(int sig)
{
    printf("信号处理%d:\n中断\n",sig);
    int i = 0;
    while(i++<10)
    {
        sleep(1);
        printf("!\n");
    }
}
int main(int args,char *argv[])
{
    signal(2,sighandle1);
    signal(3,sighandle2);
    while(1);
    return 0;
}

