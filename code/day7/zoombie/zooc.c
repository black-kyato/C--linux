#include <fun.h>

int main()
{
    int pid = fork();
    if(pid==0)
    {
        exit(0);
    }
    else{
        printf("子进程pid=%d\n",wait(NULL));
        sleep(10);
    }
    return 0;
}

