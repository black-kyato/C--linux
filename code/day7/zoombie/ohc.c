#include <fun.h>

int main()
{
    int pid = fork();
    if(pid == 0)
    {
        sleep(10);
    }
    else{
    exit(0);
         }
    return 0;
}

