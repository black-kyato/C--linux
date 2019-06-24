#include <fun.h>
int main()
{
    int pid = fork();
    if(pid<0){
        printf("创建子进程失败！\n");

    }
    else
    {
        if(pid==0)
        {
            printf("这是子进程,进程号为：%d\n",getpid());
            while(1);
        }
        else
        {
            printf("这是父进程,进程号为：%d\n",getpid());
            while(1);

        }
    }
    return 0;               
}
