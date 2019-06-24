#include <fun.h>

int main(int args,char *argv[])
{   
    ARG_CHECK(args,3);
    printf("加法调用！\n");
    printf("%s+%s=%d\n", argv[1],argv[2],atoi(argv[1])+atoi(argv[2]));
}

