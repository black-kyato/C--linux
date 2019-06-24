#include <fun.h>

void* thread1(void * val)
{
    printf("线程创建成功,传入参数：%d\n",(int)val);
    pthread_exit((void *)2);

}
int main(int args,char *argv[])
{
    pthread_t p=0;
    int ret =  pthread_create(&p,NULL,thread1,(void *)1);
    THREAD_CHECK(ret,"pthread_create")
        pthread_join( p,(void **)&ret );

    printf("父进程回收：%d\n",ret);

    return 0;

}

