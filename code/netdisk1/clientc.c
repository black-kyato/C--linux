#include "net.h"

int main(int args,char *argv[])
{
    int sfd = tcp_connect(serverip,serverport);
    RET_CHECK(sfd,-1,"tcp_connect");
    seg train;
    int ack = 0;
    print_login();
    char c;
    char username[50];
    char *password;

    while(1)
    {
        scanf("%c",&c);
        if(c=='2')
        {
            close(sfd);
            return 0;
        }
        if(c=='1')
        {
            printf("用户名：\n");
            scanf("%s",username);
            printf("请输入密码：\n");
            password = getpass(">>:");
            train.len = strlen(username)+1;
            strcpy(train.buf,username);
            sendseg(sfd,&train);
            train.len = strlen(password)+1;
            strcpy(train.buf,password);
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
    }
    getchar();
    print_main();

    char cmd[1024];
    ack = 0;
    cmd_type type;
    char *arg = NULL;
    char dir[2048]={0};
    strcpy(dir,homepath);
    
    while(1)
    {
        memset(cmd,0,sizeof(cmd));
        memset(&train,0,sizeof(train));
        printf("netdisk>>%s:",dir);  
        gets(cmd);
    //    printf("gets cmd =%s\n", cmd);
        train.len = strlen(cmd)+1;
        strcpy(train.buf,cmd);
    //    printf("sending cmd =%s\n", train.buf);
        type = trans_cmd(&train);
        arg = get_argv(&train); 

        switch(type)
        {
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
            strcpy(dir,arg);
            break;
        case LS:
            sendseg(sfd,&train);
            ack =recv_ack(sfd);
            printf("ack = %d\n",ack);
            if(!ack)
            {
                printf("netdisk>>%s:ls command error.\n",dir);
                break;
            }
            printf("目录下的文件信息如下：\n");
            printf_file_recv(sfd);
            break;
        case PUTS:
            ack = open(arg,O_RDONLY);
            if(ack == -1)
            {
                ack = 0;
                break;
            }
            close(ack);
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(ack)
            {
                send_file(sfd,arg);
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
            ack = recv_file(sfd);
            if(ack == -1)
            {
                printf("netdisk>>%s:download failed,connection error. Check and try again.\n",dir);
            }
            else
            {
                printf("netdisk>>%s:download finished.\n",dir);
                ack = 1;
            }
            break;
        case REMOVE:
            sendseg(sfd,&train);
            ack = recv_ack(sfd);
            if(!ack)
            {
                printf("netdisk>>%s:no such file or directory.\n",dir);
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
                printf("netdisk>>%s:command error.\n",dir);
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
    const char *context[3] = {"键入数字选择对应命令\n\n","1.用户登录\n\n","2.退出登录\n\n"};
    int len = strlen(title);
    print_ctrl((wid-len)/2+1,'=');
    printf("%s",title);
    print_ctrl((wid-len)/2+1,'=');
    printf("\n\n\n");

    int i = 0;
    while(i<3)
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
    const char *context[] = {"使用帮助：\n\n","1.pwd :获取当前目录路径\n\n","2.ls :列出当前目录下的文件信息\n\n","3.cd 目录：打开某个目录\n\n","4.gets 文件名：下载文件\n\n","5.puts 文件名：上传本地文件\n\n","6.remove 文件名：删除文件\n\n","7.exit：退出，其他命令不响应\n\n"};
    int len = strlen(title);
    print_ctrl((wid-len)/2+1,'=');
    printf("%s",title);
    print_ctrl((wid-len)/2+1,'=');
    printf("\n\n\n");

    int i = 0;
    while(i<8)
    {
        print_ctrl((wid-strlen(context[1]))/2,' ');
        printf("%s",context[i]);
        i++;
    }
}

