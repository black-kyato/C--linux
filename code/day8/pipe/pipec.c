#include <fun.h>

int main(int args,char *argv[])
{
    int fd[2];
    int ret = pipe(fd);
    RET_CHECK(ret,-1,"pipe")
    char buf[128]={0};
    if(fork()>0)
    {
        read(fd[0],buf,sizeof(buf));
        printf("%s\n",buf);
        wait(NULL);
        close(fd[0]);
        return 0;
    }
    else
    {
         write(fd[1],"hello",6);
         close(fd[1]);
         exit(0);
    }
}

