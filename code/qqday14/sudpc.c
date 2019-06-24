#include <fun.h>

int main(int args,char *argv[])
{
	ARG_CHECK(args,3)
	int sfd = udp_init(argv[1],atoi(argv[2]));
	int count;
	char buf[1024];
	memset(buf,0,1024);
	struct sockaddr_in addr;
	int len;
	memset(&addr,0,sizeof(struct sockaddr_in));
	while(1)
	{
		count = recvfrom(sfd,(void *)buf,(size_t)1024,(short int)0,(struct sockaddr *)&addr,(socklen_t *)&len);
		if(count>0)
		{
			break;
		}
	}
	printf("messgae from client:\nip = %s,port = %d\n%s\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port),buf);
	printf("message send to client:\n");
	memset(buf,0,1024);
	scanf("%s",buf);
    sendto(sfd,buf,strlen(buf)+1,0,(const struct sockaddr *)&addr,len);
	return 0;
}

