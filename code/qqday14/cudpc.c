#include <fun.h>

int main(int args,char *argv[])
{
	ARG_CHECK(args,3)
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
    RET_CHECK(sfd,-1,"socket")
	char buf[1024];
	memset(buf,0,1024);
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family =AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int len = sizeof(struct sockaddr_in);
	printf("message send to server:\n");
	memset(buf,0,1024);
	scanf("%s",buf);
    sendto(sfd,buf,strlen(buf)+1,0,(const struct sockaddr *)&addr,len);
	while(1)
	{
        memset(buf,0,1024);
		int count = recvfrom(sfd,(void *)buf,(size_t)1024,(short int)0,NULL,NULL);
		if(count>0)
		{
			break;
		}
	}
    printf("messgae from server:%s\n",buf);
	return 0;
}

