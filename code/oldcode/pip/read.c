#include <fun.h>

int main(int argc,char *argv[])
{
    int fdr;
    ARG_CHECK(argc,2)
    fdr=open(argv[1],O_RDONLY);
    printf("I am reader,fdr=%d\n",fdr);
   while(1) {
    char buf[128]={0};
    
    int ret=read(fdr,buf,sizeof(buf));
    if(0==ret)
    {
        printf("close\n");
        break;
    }
    printf("%s\n",buf);
   }
}

