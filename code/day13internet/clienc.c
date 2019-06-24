#include <fun.h>

int main(int args,char *argv[])
{
    ARG_CHECK(args,3)
    int port = atoi(argv[2]);
    int sfd = tcp_connect(argv[1],port);
    RET_CHECK(sfd,-1,"clinet init error\n")
    send(sfd,"hello,I am client.\n",20,0);
    char buf[30];
    recv(sfd,buf,30,0);
    printf("message from server：%s\n",buf);
    close(sfd);
    return 0;
}

