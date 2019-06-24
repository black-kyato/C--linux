#include <fun.h>
long realtime = 0;
long virtime = 0;
long  usetime = 0;
struct itimerval itime;


void rtime(int sig)
{
    realtime+=(long)itime.it_interval.tv_usec;
}
void vtime(int sig)
{
    virtime+=(long)itime.it_interval.tv_usec;
}
void utime(int sig)
{
    usetime+=(long)itime.it_interval.tv_usec;
}
void print(int sig)
{

    printf("真实时间:%ldus\t虚拟时间:%ldus\t运行时间:%ldus\n",realtime,virtime,usetime);
}
int main(int args,char *argv[])
{
    itime.it_value.tv_usec=0;
    itime.it_value.tv_sec=0;
    itime.it_interval.tv_sec =0;
    itime.it_interval.tv_usec =5;
    setitimer(ITIMER_REAL,&itime,NULL);
    setitimer(ITIMER_VIRTUAL,&itime,NULL);
    setitimer(ITIMER_PROF,&itime,NULL);
    signal(SIGALRM,rtime);
    signal(SIGVTALRM,vtime);
    signal(SIGPROF,utime);
    signal(2,print);
    printf("统计开始.统计间隔为%ld\n",(long)itime.it_interval.tv_usec);
    sleep(2);
    while(1){
    printf("输入一个字符\n");
    getchar();
    }return 0;
}

