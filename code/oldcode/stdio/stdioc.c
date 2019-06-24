#include <fun.h>

int main()
{
    //备份标准输入
    int ind = dup(0);
    //备份标准输出
    int outd = dup(1);
    //重新把标准输出文件和1联系起来
    int fd = open("test.doc",O_RDWR|O_CREAT);
    printf("fd=%d\n",fd);
    //将原本标准输出的文件描述符与fd关联起来
    dup2(fd,1);
    printf("you will see me in test.doc.\n");


    dup2(outd,1);
    printf("you will see me on the screen.\n");
    
    //将文件指针重定位到文件头
    lseek(fd,0,0);
   
    //将原本标准输入的文件描述符与fd关联起来
    dup2(fd,0);

    char str[30]={0};
   //从文件中读取字符串
    gets(str);
    printf("this is a string from the file:\t%s\n",str);
    //重新把标准输入i的文件描述符与0联系起来
    dup2(ind,0);

    //测试是否重新回到了标准输入输出流
    printf("input test：input a string\n");
    scanf("%s",str);
    printf("%s\n",str);
    return 0;
}

