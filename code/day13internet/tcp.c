#include <fun.h>

//实现服务端创建socket并初始化监听，成功返回描述符，失败返回-1
int tcp_init(const char* ip,int port)
{
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;

    //设定reuse选项
    int reuse = 1;
    int ret = setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    RET_CHECK(ret,-1,"setsockopt")
        ret = bind(sfd,(struct sockaddr *)&addr,(socklen_t)sizeof(struct sockaddr_in));
    RET_CHECK(ret,-1,"bind")
        ret = listen(sfd,10);
    RET_CHECK(ret,-1,"listen")
        return sfd;
}
//服务端socket接收连接，创建新的socket并返回其描述符，失败返回-1
int tcp_accept(int sfd)
{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    int len = sizeof(struct sockaddr_in);
    int sfds =  accept(sfd,(struct sockaddr *)&addr,(socklen_t *)&len);
    RET_CHECK(sfds, -1,"accept")
        printf("connection successfully set up:\nserver ip = %s\t server port = %d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
    return sfds;

}
//客户端建立socket连接指定ip和端口的服务端，成功返回socket描述符，失败返回-1
int tcp_connect(const char* ip,int port)
{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port =  htons(port);
    addr.sin_family = AF_INET;

    int sfd = socket(AF_INET,SOCK_STREAM,0);
    RET_CHECK(sfd,-1,"socket")
        int ret = connect(sfd,(struct sockaddr *)&addr,(socklen_t)sizeof(struct sockaddr_in));
    RET_CHECK(ret,-1,"connect")
        printf("client socket %d creat successfully\n",sfd);
    return sfd;
}
//udp服务器初始化,成功返回socket描述符，失败返回-1
int udp_init(const char*ip,int port)
{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    int sfd =socket(AF_INET,SOCK_DGRAM,0);
    RET_CHECK(sfd,-1,"socket")
        int reuse = 1;
    int ret = setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    RET_CHECK(ret,-1,"setsockopt")
        ret = bind(sfd,(struct sockaddr *)&addr,(socklen_t)sizeof(struct sockaddr_in));
    RET_CHECK(ret,-1,"bind")
        return sfd;
}
//设置非阻塞
int set_no_block(int sfd)
{
    int status=fcntl(sfd,F_GETFL);
    status=status|O_NONBLOCK;
    fcntl(sfd,F_SETFL,status);
    return 0;
}
