#include <fun.h>

int main(int args,char* argv[])
{
	int f=open(argv[1],O_RDWR|O_CREAT);
	
	write(f,"hello",sizeof("hello"));
	
	char *p;
	
	p=(char*)mmap(NULL,5,PROT_READ|PROT_WRITE,MAP_SHARED,f,0);
	
	strcpy(p,"world");

	munmap(p,5);
	return 0;
}
