#include <fun.h>
#include "pool.h"
pro_data *tp;
int *num;
int efd;
void clean(int sig)
{
    int no = 0;
    static int flag = 0;
    struct epoll_event event;
    event.events = EPOLLIN;
    if(flag == *num)
    {
        printf("all child processes are exit!\n");
    }
    for(int i = 0;i<*num;i++)
    {
        if(tp[i].busy!=1 && tp[i].busy != -1)
        {
            tp[i].busy = -1;
            event.data.fd = tp[i].fd;
            epoll_ctl(efd,EPOLL_CTL_DEL,tp[i].fd,&event);
            close(tp[i].fd);
            kill(tp[i].pid,9);
            printf("child process pid= %d is exit.\n",tp[i].pid);
            flag ++;
            no = 1;
            break;
        }
    }
    if(!no)
        printf("no free child process!\n");
}
int main(int args,char *argv[])
{
    signal(2,clean);
    ARG_CHECK(args,4);
    int process_number = atoi(argv[3]);
    num =&process_number;
    pro_data *p =  (pro_data *)calloc(process_number,sizeof(pro_data));
    //创建子进程，保存子进程信息
    fork_child(process_number,p);
    tp = p;
    //创建tcp端口等待客户端连接
    int sfd = tcp_init(argv[1],atoi(argv[2]));
    //create epoll instance
    efd = epoll_create(process_number+1);
    struct epoll_event event,evs[11];
    event.events = EPOLLIN;
    event.data.fd = sfd;
    int ret = epoll_ctl(efd,EPOLL_CTL_ADD,sfd,&event);
    RET_CHECK(ret,-1,"epoll_ctl");
    for(int i =0;i<process_number;i++)
    {
        event.data.fd = p[i].fd;
        ret = epoll_ctl(efd,EPOLL_CTL_ADD,p[i].fd,&event);
        RET_CHECK(ret,-1,"epoll_ctl")
    }
    //begin task
    int newfd;
    int count;
    int i,j;
    int flag;
    printf("server is ready for download task\n");
    while(1)
    {
        count = epoll_wait(efd,evs,process_number+1,-1);
        if(count == -1&&errno == 4)
        {
            continue;
        }

        RET_CHECK(count,-1,"epoll_wait");
        for(i=0;i<count;i++)
        {
            //sfd is ready, a new client is connecting
            if(evs[i].data.fd == sfd)
            {
                flag = 0;
                newfd = tcp_accept(sfd);
                RET_CHECK(newfd,-1,"accept");
                for(j=0;j<process_number;j++)
                {
                    if(!p[j].busy)
                    {
                        flag = 1;
                        p[j].busy = 1;
                        printf("child process %d get the task.\n",j+1);
                        sendFD(p[j].fd,newfd);
                        close(newfd);
                        break;
                    }
                }
                //no child is free
                if(!flag)
                {
                    close(newfd);
                    printf("no child is free,the task is rejected.\n");
                }
            }
            for(j = 0;j < process_number; j++)
            {
                if(evs[i].data.fd == p[j].fd&&p[j].busy == 1)
                {
                    read(p[j].fd,&ret,1);
                    p[j].busy = 0;
                    printf("child process %d completed his task.\n",j+1);
                    break;
                }
            }  
        }
    }
    return 0;
}
//创建子进程，子进程循环等待父进程唤醒
int fork_child(int num,pro_data* p)
{
    int fds[2];
    int pid;
    int ret;
    for(int i=0;i<num;i++)
    {
        //申请两个套接字，第一个用于子进程读写，第二个用于父进程读写
        ret = socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        RET_CHECK(ret,-1,"socketpair");
        pid = fork();
        //子进程
        if(0 == pid)
        {
#ifdef debug
            printf("子进程%d创建成功\n",i);
#endif
            close(fds[1]);
            work(fds[0]);
        }
        //父进程
        p[i].pid = pid;
        p[i].busy = 0;
        p[i].fd = fds[1];
        close(fds[0]);
    }
    return  0;
}
int work(int fd)
{
    int sfd=0;//和客户端通信
    while(1)
    {
        recvFD(fd,&sfd);
#ifdef debug
        printf("pid= %d,sfd=%d\n",getpid(),sfd);
#endif
        printf("pid =%d begin working.\n",getpid());
        transp(sfd);
        sleep(10);
        close(sfd);
        printf("pid =%d stop workinng.\n",getpid());
        write(fd,&sfd,1);
    }
}
//进程间发送文件描述符
int sendFD(int fd,int newfd)
{
    struct msghdr msg;
    memset(&msg,0,sizeof(msg));

    char buf[10]= "lalalala!";
    struct iovec iov[1];
    iov[0].iov_base = buf;
    iov[0].iov_len = 10;

    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    struct cmsghdr *cmsg;
    int len = CMSG_LEN(sizeof(int));
    cmsg = (struct cmsghdr *)calloc(1,len);
    cmsg->cmsg_len = len;
    cmsg->cmsg_level=SOL_SOCKET;
    cmsg->cmsg_type=SCM_RIGHTS;
    *(int*)CMSG_DATA(cmsg)=newfd;

    msg.msg_control = cmsg;
    msg.msg_controllen = len;

    int ret;
    ret=sendmsg(fd,&msg,0);
    RET_CHECK(ret,-1,"sendmsg");
    return 0;

}
//进程间接收文件描述符
int recvFD(int fd,int *newfd)
{
    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    struct iovec iov[1];
    char buf[10];
    iov[0].iov_base=buf;
    iov[0].iov_len=10;
    msg.msg_iov=iov;
    msg.msg_iovlen=1;

    struct cmsghdr *cmsg;
    int len=CMSG_LEN(sizeof(int));
    cmsg=(struct cmsghdr *)calloc(1,len);
    cmsg->cmsg_len=len;
    cmsg->cmsg_level=SOL_SOCKET;
    cmsg->cmsg_type=SCM_RIGHTS;
    msg.msg_control=cmsg;
    msg.msg_controllen=len;
    int ret;
    ret=recvmsg(fd,&msg,0);
    RET_CHECK(ret,-1,"sendmsg");
#ifdef debug
    printf("recvFD:%s\n",(char *)msg.msg_iov[0].iov_base);
#endif
    *newfd=*(int*)CMSG_DATA(cmsg);
    return 0;
}

//传输文件的函数,sfd是已经和客户端建立连接的套接字
int transp(int sfd)
{
    Train_t train;
    memset(&train,0,sizeof(train));

    int fd = open(FILENAME,O_RDONLY);
    RET_CHECK(fd,-1,"open");
    //transfer filename
    train.len = strlen(FILENAME);
    strcpy(train.p,FILENAME);
    int ret = send(sfd,&train,4+train.len,0);
    if(-1 ==ret)
    {
        printf("client is closed\n");
        close(fd);
        return 0;
    }
    //transfer the size of file 
    struct stat statbuf;
    fstat(fd,&statbuf);
    train.len=statbuf.st_size;
    ret = send(sfd,&train.len,sizeof(int),0);
    if(-1 ==ret)
    {
        printf("client is closed\n");
        close(fd);
        return 0;
    }

    printf("pid = %d begin to transfer file %s\n",getpid(),FILENAME);


    while((train.len = read(fd,train.p,sizeof(train.p)))>0)
    {
        ret = send(sfd,&train,4+train.len,0);
        if(-1 ==ret)
        {
            printf("client is closed\n");
            close(fd);
            return 0;
        }
    }
    printf("pid = %d completed transfer of file %s\n",getpid(),FILENAME);
    close(fd);
    return 0;
}
