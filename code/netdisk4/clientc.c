#include "net.h"
int timer = 0;
int sfd = 0;
void stop_connect(int sig)
{
    if(timer == 1)
    {
        timer =0;
    }
    else
    {
        close(sfd);
        printf("30s无响应，收到信号%d，已断开连接,即将退出...\n",sig);
        kill(getpid(),9);
    }
}
int main(int args,char *argv[])
{
    sfd = tcp_connect(serverip,serverport);
    RET_CHECK(sfd,-1,"tcp_connect");
    seg train;
    int ack = 0;
    print_login();
    char c;
    char username[50];
    char *password;

    pthread_t pid[2];

    struct itimerval tt;
    tt.it_interval.tv_sec = 30;
    tt.it_interval.tv_usec = 0;
    tt.it_value.tv_sec = 30;
    tt.it_value.tv_usec = 0;
    signal(SIGALRM,stop_connect);
    setitimer(ITIMER_REAL,&tt,NULL);
    while(1)
    {
        scanf("%c",&c);
        if(c=='3')
        {
            close(sfd);
            return 0;
        }
        if(c=='1')
        {
            send_ack(sfd,1);
            printf("用户名：\n");
            scanf("%s",username);
            timer = 1;
            printf("请输入密码：\n");
            password = getpass(">>:");
            timer = 1;
            //发送用户名
            train.len = strlen(username)+1;
            strcpy(train.buf,username);
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(ack == 0)
            {
                printf("用户不存在...即将退出\n");
                sleep(1);
                close(sfd);
                return 0;
            }
            //接收salt值
            recvseg(sfd,&train);
            //发送加密密码
            timer = 1;
            strcpy(train.buf,crypt(password,train.buf));
            train.len = 1+strlen(train.buf);
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(ack == 0)
            {
                printf("密码错误，即将退出\n");
                sleep(1);
                close(sfd);
                return 0;
            }
            else
            {
                break;
            }
        }
        if(c=='2')
        {
            send_ack(sfd,0);
            while(1)
            {
                printf("请输入用户名(不要包含空格和特殊字符)：\n");
                scanf("%s",train.buf);
            timer = 1;
                train.len = strlen(train.buf)+1;
                sendseg(sfd,&train);
                ack = recv_ack(sfd);
                if(ack == 1)
                {
                    break;
                }               
                else
                {
                    printf("用户名重复！\n");
                }
            }
            while(1){
            printf("请输入密码：\n");
            password = getpass(">>:");
            timer = 1;
            strcpy(train.buf,password);
            printf("请确认密码：\n");
            timer = 1;
            password = getpass(">>:");
            if(strcmp(train.buf,password))
            {
                printf("两次密码输入不一致，请重新输入：\n");
            }
            else
            {
                break;
            }
            }
            printf("注册成功！即将退出，请重新登录...\n");
            train.len = strlen(train.buf)+1;
            sendseg(sfd,&train);
            close(sfd);
            usleep(500);
            return 0;
        }
    }
    getchar();
    print_main();

    char cmd[1024];
    ack = 0;
    cmd_type type;
    char *arg = NULL;
    char dir[2048]={0};
    strcpy(dir,username);

    while(1)
    {
        memset(cmd,0,sizeof(cmd));
        memset(&train,0,sizeof(train));
        printf("netdisk>>%s:",dir);  
        gets(cmd);
            timer = 1;
        //    printf("gets cmd =%s\n", cmd);
        train.len = strlen(cmd)+1;
        strcpy(train.buf,cmd);
        //    printf("sending cmd =%s\n", train.buf);
        type = trans_cmd(&train);
        arg = get_argv(&train); 

        switch(type)
        {
        case MKDIR:
            if(arg == NULL)
            {
                printf("netdisk>>%s:argv error,no target.\n",dir);
                break;
            }
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(!ack)
            {
                printf("netdisk>>%s:create directory failed.\n",dir);
                break;
            }
            break;
        case CD:
            if(arg == NULL)
            {
                printf("netdisk>>%s:argv error,no target.\n",dir);
                break;
            }
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(!ack)
            {
                printf("no such directory.\n");
                break;
            }
            sprintf(dir,"%s/%s",dir,arg);
            break;
        case LS:
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            printf("ack = %d\n",ack);
            if(!ack)
            {
                printf("netdisk>>%s:ls command error or lost connection.\n",dir);
                break;
            }
            printf("目录下的文件信息如下：\n");
            memset(&train,0,sizeof(train));
            recvseg(sfd,&train);
            puts(train.buf);
            break;
        case PUTS:
            //检查本地文件是否可以打开
            ack = open(arg,O_RDONLY);
            if(ack == -1)
            {
                ack = 0;
                break;
            }
            close(ack);
            //发送上传命令
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            //收到确认
            if(ack)
            {
                //发送MD5值
                Compute_file_md5(arg, train.buf);
                train.len = strlen(train.buf)+1;
                sendseg(sfd,&train);
                ack =recv_ack(sfd);
                //没有重复MD5值
                if(!ack)
                {
                train.len = sfd;
                strcpy(train.buf,arg);
                pthread_create(pid,NULL,send_thread,(void *)&train);
                usleep(500);
                ack = 1;
                }
                else{
                printf("upload finished.\n");
                }
            }
            else
            {
                printf("netdisk>>%s:upload error,check and try again\n",dir);
            }
            break;
        case GETS:
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(!ack)
            {
                printf("netdisk>>%s:download error,no such file.\n",dir);
                break;
            }
            train.len =sfd;
            printf("sfd =%ld\n",(long)&train);
            strcpy(train.buf,arg);
            pthread_create(pid+1,NULL,client_recv_thread,(void *)&train);
            usleep(500);
            ack = 1;
            break;
        case REMOVE:
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(!ack)
            {
                printf("netdisk>>%s:no such file.\n",dir);
                break;
            }
            printf("netdisk>>%s:delete succeed\n.",dir);
            break;
        case PWD:
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(ack)
            {
                recvseg(sfd,&train);
                printf("netdisk>>%s:%s\n",dir, train.buf);
            }
            else
            {
                printf("netdisk>>%s:command error or lost connection.\n",dir);
            }
            break;
        case EXIT:
            if(arg == NULL)
            {
                sendseg(sfd,&train);
                printf("bye\n");
                close(sfd);
                return 0;
            }
            else
            {
                printf("netdisk>>%s:command error.\n",dir);
            }
            break;
        case OTHER:
            printf("netdisk>>%s:command error.\n",dir);
            break;
        default:
            break;
        }
        free(arg);
    }
    return 0;
}
void print_ctrl(int num,char c)
{
    for(int i = 0; i<num ;i++)
    {
        printf("%c",c);
    }
}
void print_login()
{
    struct winsize wsize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsize);
    int wid = wsize.ws_col;

    const char *title = "< 个-人-简-易-网-盘 >---< 出品人：sillydog--blackkyato >";
    const char *context[4] = {"键入数字选择对应命令\n\n","1.用户登录\n\n","2.用户注册\n\n","3.退出\n\n"};
    int len = strlen(title);
    print_ctrl((wid-len)/2+1,'=');
    printf("%s",title);
    print_ctrl((wid-len)/2+1,'=');
    printf("\n\n\n");

    int i = 0;
    while(i<4)
    {
        print_ctrl((wid-strlen(context[i]))/2,' ');
        printf("%s",context[i]);
        i++;
    }
}
void print_main()
{
    struct winsize wsize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsize);
    int wid = wsize.ws_col;

    const char *title = "< 欢-迎-使-用 >---< 个-人-简-易-网-盘>---< version - 1.0.0  >";
    const char *context[] = {"使用帮助：\n\n","0.mkdir：创建目录\n\n","1.pwd :获取当前目录路径\n\n","2.ls :列出当前目录下的文件信息\n\n","3.cd 目录：打开某个目录\n\n","4.gets 文件名：下载文件\n\n","5.puts 文件名：上传本地文件\n\n","6.remove 文件名：删除文件\n\n","7.exit：退出，其他命令不响应\n\n"};
    int len = strlen(title);
    print_ctrl((wid-len)/2+1,'=');
    printf("%s",title);
    print_ctrl((wid-len)/2+1,'=');
    printf("\n\n\n");

    int i = 0;
    while(i<9)
    {
        print_ctrl((wid-strlen(context[1]))/2,' ');
        printf("%s",context[i]);
        i++;
    }
}

