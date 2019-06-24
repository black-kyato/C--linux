#include <fun.h>
struct sigaction sigm;
int main(int args,char *argv[])
{
    sigemptyset(&sigm.sa_mask);
    sigaddset(&sigm.sa_mask,2);
    sigprocmask(SIG_BLOCK,&sigm.sa_mask,NULL);
    sigaction(2,&sigm,NULL);
    printf("2 is apending!begin to sleep\n");
    sleep(5);
    if(sigismember(&sigm.sa_mask,2))
    {
        printf("2 is pending\n");
    }
    return 0;
}

