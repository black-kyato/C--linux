#include <fun.h>
int main(int args,char* argv[])
{
	int f=open("/mnt/huge/file",O_RDWR|O_CREAT);
    ftruncate(f,0X40000);	
	
	char *p;
	p=(char*)mmap(NULL,5,PROT_READ|PROT_WRITE|MAP_HUGETLB|0x40000,MAP_SHARED,f,0);
    RET_CHECK(p,-1,"mmap")	
    while(1)
    {
	strcpy(p,"world");
    p=p+5;
    sleep(0.5);
    }
	munmap(p,0x40000);
	return 0;
}
