#include <fun.h>
void make()
{
    int i =0;
    while(i<10)
    {
        if(fork()==0)
        {
            printf("hh\n");
            while(1);
        }
        i++;
    }
}

int main(int args,char *argv[])
{
    make();
    printf("over\n");
    return 0;
}

