#include <fun.h>
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void unlock(void *p)
{
    pthread_mutex_unlock(&mutex);
    printf("进程%d清理完毕\n",(int)p);
}
void* cancelp(void *p)
{
    printf("线程%d创建成功！\n",(int)p);
    pthread_cleanup_push(unlock,p);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond,&mutex);
    pthread_cleanup_pop(0);
    pthread_mutex_unlock(&mutex);
    printf("线程%d顺利结束！\n",(int)p);
}
int main(int args,char *argv[])
{
    pthread_t d1,d2;
    printf("创建线程1\n");
    pthread_create(&d1,NULL,cancelp,(void *)1);
    printf("创建线程2\n");
    pthread_create(&d2,NULL,cancelp,(void *)2);
    sleep(1);
    pthread_cancel(d1);
    pthread_cond_signal(&cond);
    pthread_join(d1,NULL);
    pthread_join(d2,NULL);
    return 0;
}

