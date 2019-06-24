#include <fun.h>
void sigh(int sig)
{
    printf("SIGNAL %d,client is already closed!\n",sig);
}
void * pthread_send(void *p)
{
    signal(SIGPIPE,sigh);
    printf("server sender thread is running:\n");
    char buf[1024]= "\0";
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
    printf("server receiver is running:\n");
    char buf[1024]="\0";
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
        printf("\33[31;1mClient：\n%s\33[0m\n",buf);
    }
}
int main(int args,char *argv[])
{
    ARG_CHECK(args,3)
    int sfd = tcp_init(argv[1],atoi(argv[2]));
    RET_CHECK(sfd,-1,"server initialization error\n")
    long sfds;
    pthread_t sp,rp;
start:
    sfds = tcp_accept(sfd);
    RET_CHECK(sfds,-1,"server tcp connection error\n")
    int ret = pthread_create(&sp,NULL,pthread_send,(void *)sfds);
    THREAD_CHECK(ret,"pthread_create")
    ret = pthread_create(&rp,NULL,pthread_receive,(void *)sfds);
    THREAD_CHECK(ret,"pthread_create")
    long ret1 = 0;
    pthread_join(rp,(void **)&ret1);
    if(ret1==2)
    {
        printf("client is closed.Begin waiting\n");
        close(sfds);
        goto start;
    }
    else
    {
        close(sfd);
        close(sfds);
        printf("server is shutdown.\n");
    }
    return 0;
}

