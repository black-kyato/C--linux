#include <fun.h>
#include <time.h>

int main(int args, char * argv[])
{
	ARG_CHECK(args,2);
	DIR *dp;
	struct dirent *fdp;
	struct stat buf;
	int ret;
	char path[1024];
	char times[100];
	dp = opendir(argv[1]);
	while((fdp=readdir(dp))!=NULL)
	{
		memset(path,0,sizeof(path));
		sprintf(path,"%s%s%s",argv[1],"/",fdp->d_name);
		ret =stat(path,&buf);
		if(ret==-1)
		{
			printf("error!no such file or dirdctory\n");
			return -1;
		}
		if(S_ISREG(buf.st_mode))
		{
			printf("-");

		}
		else
		{
			if(S_ISDIR(buf.st_mode))
			{ 
				printf("d");
			}
			else
			{
				if(S_ISLNK(buf.st_mode))
					printf("l");
			}
		}
		printf(" ");
		strcpy(times,ctime(&buf.st_mtime));
		times[strlen(times)-1]='\0';
		printf("%s\t%s\t%ld\t%s\t%s\n\n",getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,times,fdp->d_name);		
	}
	closedir(dp);
}
