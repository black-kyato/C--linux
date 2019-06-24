#include <fun.h>
int pid;
long csfd;
void sigh(int sig)
{
    printf("SIGNAL %d,server is already closed!\n",sig);
}
void clean(int sig)
{
    printf("SIGNAL %d is received,close client\n",sig);
    close(csfd);
    kill(pid,9);
}
void * pthread_send(void *p)
{
    signal(SIGPIPE,sigh);
    printf("client sender thread is running:\n");
    char buf[1024];
    while(1)
    {
        memset(buf,0,1024);
        fgets(buf,1023,stdin);
        int ret = send((long)p,buf,strlen(buf),0);
        if(ret==-1)
        {
            if(errno==EPIPE)
            {
                pthread_exit((void *)2);
            }
            perror("send");
            pthread_exit((void *)-1);
        }
    }
}
void * pthread_receive(void *p)
{
    printf("client receiver is running:\n");
    char buf[1024];
    while(1)
    {
        memset(buf,0,1024);
        int ret =  recv((long)p,buf,1024,0);
        if(ret == -1)
        {   
            perror("recv");
            pthread_exit((void *)-1);
        }
        if(ret == 0)
        {
            pthread_exit((void *)2);
        }
        printf("\33[31;1mServer：\n%s\33[0m\n",buf);
    }
}
int main(int args,char *argv[])
{
    pid = getpid();
    ARG_CHECK(args,3)
    signal(2,clean);
    signal(3,clean);
    csfd = tcp_connect(argv[1],atoi(argv[2]));
    RET_CHECK(csfd,-1,"client connection error\n")
    pthread_t sp,rp;
    int ret = pthread_create(&sp,NULL,pthread_send,(void *)csfd);
    THREAD_CHECK(ret,"pthread_create")
    ret = pthread_create(&rp,NULL,pthread_receive,(void *)csfd);
    THREAD_CHECK(ret,"pthread_create")
    int ret1,ret2;
    pthread_join(sp,(void **)&ret1);
    pthread_join(rp,(void **)&ret2);
    if(ret1 ==2||ret2==2)
    {
        close(csfd);
        printf("server is closed.\n");
    }
    else
    {
        close(csfd);
        printf("client is shutdown now.\n");
    }
    return 0;
}

