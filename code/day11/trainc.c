#include <fun.h>
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int ticket;
    int flag;
}data;


data test={PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0,1};
void *give(void *p)
{

    pthread_mutex_lock(&test.mutex);
    test.ticket = 20;
    printf("放票员：第一次放票完成\n");
    pthread_mutex_unlock(&test.mutex);
    pthread_mutex_lock(&test.mutex);
    pthread_cond_wait(&test.cond,&test.mutex);
    test.ticket = 20;
    test.flag = 0;
    printf("放票员：第二次放票完成\n");
    pthread_mutex_unlock(&test.mutex);
    printf("放票员：我下班了！\n");
}

void *sale1(void *p)
{
    int sum =0;
    while(1)
    {
        pthread_mutex_lock(&test.mutex);
        if(test.ticket>0)
        {
            test.ticket--;
            sum++;
            if(test.ticket==10)
            {
                printf("售票员1：余票还有10张！\n");
            }
            if(test.ticket==0)
            {
                pthread_cond_signal(&test.cond);
            }
                pthread_mutex_unlock(&test.mutex);
        }
        else
        {
            if(test.flag) 
            {   
                pthread_mutex_unlock(&test.mutex);
                sleep(1);
                continue;
            }
            else
            {
                pthread_mutex_unlock(&test.mutex);
                break;
            }
        }
    }
    printf("售票完成，售票员1卖了%d张票\n",sum);
}

void *sale2(void *p)
{
    int sum =0;
    while(1)
    {
        pthread_mutex_lock(&test.mutex);
        if(test.ticket>0)
        {
            test.ticket--;
            sum++;
            if(test.ticket==10)
            {
                printf("售票员2：余票还有10张！\n");
            }
            if(test.ticket==0)
            {
                pthread_cond_signal(&test.cond);
            }
                pthread_mutex_unlock(&test.mutex);
        }
        else
        {
            if(test.flag)
            {   
                pthread_mutex_unlock(&test.mutex);
                sleep(1);
                continue;
            }
            else
            {
                pthread_mutex_unlock(&test.mutex);
                break;
            }
        }
    }
    printf("售票完成：售票员2卖了%d张票\n",sum);
}
int main(int args,char *argv[])
{

    pthread_t p1,p2,c0;
    pthread_create(&p1,NULL,sale1,NULL);
    pthread_create(&p2,NULL,sale2,NULL);
    pthread_create(&c0,NULL,give,NULL);
    pthread_join(c0,NULL);
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    return 0;
}
