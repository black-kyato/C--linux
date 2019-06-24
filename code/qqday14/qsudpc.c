#include <fun.h>
//服务端，输入参数为服务器ip和端口
int main(int args,char *argv[])
{
    ARG_CHECK(args,3)
    int sfd = udp_init(argv[1],atoi(argv[2]));
    RET_CHECK(sfd,-1,"udp_init")

    
    char buf[1024];
    memset(buf,0,1024);

    struct sockaddr_in addr;//客户端地址
    int len,ret;
start:  
    //begin信号，和服务器建立连接
    ret = recvfrom(sfd,buf,1024,0,(struct sockaddr *)&addr,(socklen_t *)&len);
    RET_CHECK(ret,-1,"recvfrom")
    printf("len=%d\n",len);
    if(strcmp(buf,"begin")==0)
     {
        printf("connection set up from client:ip =%s,port = %d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
    }

    //设置非阻塞
    set_no_block(sfd);
    //建立epoll监控实例
    int efd = epoll_create(2);
    struct epoll_event eve_send,eve_read;
    eve_send.events = EPOLLIN;
    eve_send.data.fd = STDIN_FILENO;
    eve_read.events = EPOLLIN;
    eve_read.data.fd = sfd;

    epoll_ctl(efd,EPOLL_CTL_ADD,sfd,&eve_read);
    epoll_ctl(efd,EPOLL_CTL_ADD,STDIN_FILENO,&eve_send);

    struct epoll_event e[2];
    while(1)
    {
        memset(buf,0,1024);
        ret = epoll_wait(efd,e,2,-1);
        for(int i =0;i<ret;i++)
        {
            if(e[i].data.fd == STDIN_FILENO)
            {
                fgets(buf,sizeof(buf),stdin);
                sendto(sfd,buf,strlen(buf)+1,0,(const struct sockaddr *)&addr,len);
            }
            if(e[i].data.fd==sfd)
            {
                int count = recvfrom(sfd,(void *)buf,(size_t)1024,(short int)0,NULL,NULL);
                RET_CHECK(count,-1,"recvfrom")
                if(count == 0)
                {
                    printf("client is closed.waiting for connection...\n");
                    close(efd);
                    goto start;
                }
                else
                {
                    printf("From client:%s\n",buf);
                }
            }
        }
    }
    return 0;
}

