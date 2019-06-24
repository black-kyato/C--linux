#include <fun.h>

int main(int args,char *argv[])
{
    ARG_CHECK(args,3)
    int port = atoi(argv[2]);
    int sfd = tcp_init(argv[1],port);
    RET_CHECK(sfd,-1,"server init error\n")
    int sfds = tcp_accept(sfd);
    send(sfds,"hello,I am server.\n",20,0);
    char buf[30];
    recv(sfds,buf,30,0);
    printf("message from client:%s\n",buf);
    close(sfds);
    close(sfd);
    return 0;
}

