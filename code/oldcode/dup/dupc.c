#include <fun.h>

int main()
{
    int fd1 = open("dup.doc", O_RDWR | O_CREAT);
    int fd2 = dup(6);
    printf("fd1=%d\nfd2=dup(fd1)\nfd2==%d\n",fd1,fd2);
    close(fd1);
    printf("fd1 closed!\n");
    write(fd2,"HELLO WORLD!",13);
    printf("write dup.doc by fd2!\n");
}

