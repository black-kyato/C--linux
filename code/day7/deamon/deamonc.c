#include <fun.h>

int main()
{
    if(fork()>0)
        exit(0);
    setsid();
    chdir("/");
    close(1);
    umask(0);

    int fd = open("/tmp/log",O_RDWR|O_CREAT,0666);
    dup2(fd,1);
    RET_CHECK(fd,-1,"open");
    while(1)
    {
        printf("wohenshuang\n");
        sleep(1);
    }
}

