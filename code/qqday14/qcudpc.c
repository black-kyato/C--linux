#include <fun.h>
//客户端，输入参数为服务器ip和端口
int main(int args,char *argv[])
{
    ARG_CHECK(args,3)
    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    RET_CHECK(sfd,-1,"socket")

        //设置非阻塞
    set_no_block(sfd);

    char buf[1024];
    memset(buf,0,1024);
    struct sockaddr_in addr;//服务器地址
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family =AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int len = sizeof(struct sockaddr_in);
    memset(buf,0,1024);
    strcpy(buf,"begin");
    //发送begin信号，和服务器建立连接
    int ret = sendto(sfd,buf,strlen(buf)+1,0,(const struct sockaddr *)&addr,len);
    RET_CHECK(ret,-1,"connection error")
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
                int test = sendto(sfd,buf,strlen(buf)+1,0,(const struct sockaddr *)&addr,len);
                if(test<=0)
                {
                    printf("server is closed.Close client\n");
                    close(efd);
                    close(sfd);
                    printf("Client closed.\n");
                    return 0;
                }
            }
            if(e[i].data.fd==sfd)
            {
                int count = recvfrom(sfd,(void *)buf,(size_t)1024,(short int)0,NULL,NULL);
                RET_CHECK(count,-1,"recvfrom")
                if(count == 0)
                {
                    printf("server is closed.Close client\n");
                    close(efd);
                    close(sfd);
                    printf("Client closed.\n");
                    return 0;
                }
                else
                {
                    printf("From server:%s\n",buf);
                }
            }
        }
    }
    return 0;
}

