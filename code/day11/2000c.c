#include <fun.h>
int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *add(void *g)
{
    
   for(int i =0;i<20000000;i++)
   {
        pthread_mutex_lock(&mutex);
        sum++;
        pthread_mutex_unlock(&mutex);
   }
   return 0;
}
int main(int args,char *argv[])
{

    pthread_t pid;
    pthread_create(&pid,NULL,add,NULL);
   for(int i =0;i<20000000;i++)
   {
        pthread_mutex_lock(&mutex);
        sum++;
        pthread_mutex_unlock(&mutex);
   }
   pthread_join( pid,NULL);
   printf("sum=%d\n",sum);
    pthread_mutex_destroy(&mutex);
   return 0;
}

