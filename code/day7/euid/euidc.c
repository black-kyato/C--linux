#include <fun.h>

int main()
{
    printf("uid=%d\tgid=%d\teuid=%d\tegid=%d\n",getuid(),getgid(),geteuid(),getegid());
    int fd = open("file", O_RDWR|O_CREAT,0666);
    RET_CHECK(fd,-1,"open");
    write(fd,"hello",6);
    sleep(20);
    return 0;
}

