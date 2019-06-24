#include <fun.h>
void * timep(void *p)
{
    char buf[100];
    time_t t;
    t = time(NULL);
    char *pt;
    pt = ctime(&t);
    ctime_r(&t,buf);
    sleep(2);
    printf("this pthread%d:ctime1\n",(int)p);
    printf("phtread%d:ctime=%s\n",(int)p,pt);
    printf("pthread%d:ctime_r=%s\n",(int)p,buf);
}
int main(int args,char *argv[])
{
    pthread_t d1,d2;
    pthread_create(&d1,NULL,timep,(void*)1);
    sleep(1);
    pthread_create(&d2,NULL,timep,(void*)2);
    sleep(4);
    return 0;
}

