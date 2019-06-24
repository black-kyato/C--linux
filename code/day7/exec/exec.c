#include <fun.h>

int main()
{
    printf("this is father！\n");
    printf("调用加法：\n");
    int ret;
    ret = execl("add.exe","add.exe","34","45",NULL);
    RET_CHECK(ret,-1,"execl")
    return 0;
}

