#include <fun.h>
void freept(void * p)
{
    printf("开始清理！\n");
    free((char *)p);
}
void* thread1(void * val)
{

    printf("线程创建成功!\n");
    char *buf=(char *)malloc(128);
    pthread_cleanup_push(freept,(void *)buf);
    pthread_testcancel();
    read(0,buf,128);
    pthread_testcancel();
    pthread_cleanup_pop(0);
    pthread_exit((void *)0);
}
int main(int args,char *argv[])
{
    pthread_t p=0;
    int ret =  pthread_create(&p,NULL,thread1,NULL);
    THREAD_CHECK(ret,"pthread_create")
    sleep(1);
    printf("主进程开始发送cancel信号\n");
    sleep(1);
    ret =  pthread_cancel(p);
    THREAD_CHECK(ret,"pthread_cancel")
    long ptret;
    pthread_join(p,(void**)&ptret);
    printf("子线程退出码：%ld\n", ptret);
    return 0;

}

