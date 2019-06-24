# include <fun.h>
int main(int argc,char* argv[])
{
   	ARG_CHECK(argc,2);	
	int fdw;
    fdw=open(argv[1],O_WRONLY);
    printf("I am writer fdw=%d\n",fdw);
    char buf[1024]={0};

    while(1)
    {
     memset(buf,0,sizeof(buf)); 
     scanf("%s",buf);
    write(fdw,buf,strlen(buf));
    }
}
