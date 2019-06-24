#include "net.h"
//队列初始化
void que_init(queue *p,int capacity)
{
    pthread_mutex_init(&p->mutex,NULL);
    pthread_cond_init(&p->cond,NULL);

    p->capacity = capacity;
    p->head = p->tail = NULL;
    p->size = 0;

}
//入队
int que_in(queue *p,int fd)
{
    if(p->size == p->capacity)
    {
        printf("服务器满负载，请稍后再试!\n");
        return -1;

    }
    p->size++ ;
    pnode tp=(pnode)calloc(1,sizeof(node));
    tp->next =                                                                                               NULL;
    tp->sockfd = fd;

    if(p->head == NULL)
    {
        p->head = tp;
        p->tail =tp;

    }
    else
    {
        p->tail->next = tp;
        p->tail = tp;

    }
    return 0;

}
//出队
int que_out(queue *p)
{
    pnode tp;
    int ret;
    if(p->head ==NULL) 
    {
        return -1;

    }
    else
    {
        tp = p -> head;
        ret = tp->sockfd;
        p->head = tp->next;
        if(p->tail == tp)
        {
            p->tail = NULL;

        }
        free(tp);
        p->size--; 
        return ret;
    }

}
//主线程
void * mainthread(void *g)
{
    info *pt =(info *)g;
    queue *p = pt->p;
    thread_info *t = pt->t;
    int newfd;
    FILE* logfd;

    char username[50];
    struct spwd *sp;
    char password[50];
    char salt[512]={0};

    seg train;
    int ret = 0;

    char nowtime[50] ={0};
    time_t times;
    while(1)
    {
start:     pthread_mutex_lock(&p->mutex);
           for(int i =0;i<maxclient;i++)
           {
               if(t[i].tid == pthread_self())
               {
                   t[i].st = rest;
               }
           }
           pthread_cond_wait(&p->cond,&p->mutex);
           newfd = que_out(p);
           while(newfd==-1)
           {
               pthread_cond_wait(&p->cond,&p->mutex);
               newfd = que_out(p);
           }

           logfd = fopen("netdisk.log","a+");
           pthread_mutex_unlock(&p->mutex);
           times =time(NULL);
           ctime_r(&times,nowtime);
           fprintf(logfd,"connection with client set up at time :%s",nowtime);
           for(int i =0;i<maxclient;i++)
           {
               if(t[i].tid == pthread_self())
               {
                   t[i].st = busy;
               }
           }
           //登录验证
           recvseg(newfd,&train);
           strcpy(username, train.buf);
           recvseg(newfd,&train);
           strcpy(password, train.buf);
            printf("user:%s\npass:%s\n",username,password);
           sp=getspnam(username);
           if(sp == NULL)
           {
               perror("getspnam");
failed:     
               send_ack(newfd,0);
               close(newfd);
               times = time(NULL);
               ctime_r(&times,nowtime);
               fprintf(logfd,"\nat time %s:login failed.\n", nowtime);
               goto start;
           }
           int ti,tj;

           for(ti=0,tj=0;sp->sp_pwdp[ti] && tj != 3;++ti)
           {
               if(sp->sp_pwdp[ti] == '$')
                   ++tj;
           }
           strncpy(salt,sp->sp_pwdp,ti-1);
            if(strcmp(sp->sp_pwdp,crypt(password,salt)) == 0)
            {   printf("login\n");
                send_ack(newfd,1);
               times = time(NULL);
               ctime_r(&times,nowtime);
               fprintf(logfd,"\nat time %s:login from user %s.\n", nowtime,username);
            }
            else
            {
                printf("login failed\n");
                goto failed;   
            }
            
           cmd_type handle;

           char dir_name[1024] = {0};
           char *argvs = NULL;
           DIR * dirp = NULL;
           chdir(homepath);
           getcwd(dir_name,1024);

           while(1)
           {
               memset(&train,0,sizeof(seg));
               handle = OTHER;
               argvs =NULL;
               ret = recvseg(newfd,&train);
               times = time(NULL);
               ctime_r(&times,nowtime);
               fprintf(logfd,"\nat time %s: cmd received = %s\n",nowtime,train.buf);

               if(ret ==-1)
               {
                   times = time(NULL);
                   ctime_r(&times,nowtime);
                   fprintf(logfd,"\nat time %s:", nowtime);
                   fprintf(logfd,"recvseg failed,connect error\n");
                   close(newfd);
                   fclose(logfd);
                   free(argvs);
                   break;
               }
               handle = trans_cmd(&train);
               times = time(NULL);
               argvs = get_argv(&train);
               //printf("argvs:%s\n",argvs);
               ret = 0;
               int fsize = 0;
               char filename[100] = {0};
               switch(handle)
               {
               case CD:
                   dirp = opendir(argvs);
                   if(dirp != NULL)
                   {
                       sprintf(dir_name,"%s/%s",dir_name,argvs);
                       chdir(dir_name);
                       send_ack(newfd,1);
                   }
                   else
                   {
                       ret = -1;
                   }
                   break;
               case LS:
                   if(argvs == NULL)
                   {
                       send_ack(newfd,1);
                       system("ls -l|cat >.file_dir");
                       system("cat .file_dir");
                       const char *file_dir = ".file_dir";
                       ret = send_file(newfd,(char *)file_dir);
                       system("rm -f .file_dir");
                   }
                   else
                   {
                       ret = -1;
                   }
                   break;
               case PUTS:
                   send_ack(newfd,1);
                   ret = recv_file(newfd);
                   break;
               case GETS:
                   sscanf(argvs,"%s%d",filename,&fsize);
                    
                   ret = open(filename,O_RDONLY);
                   close(ret);
                   if(ret ==-1)
                   {
                       break;
                   }
                   send_ack(newfd,1);
                   ret = send_file(newfd,argvs);
                   break;
               case REMOVE:
                   ret = open(argvs,O_RDONLY);
                   close(ret);
                   if(ret == -1)
                   {
                       break;
                   }
                   else{
                       send_ack(newfd,1);
                       ret = 0;
                       sprintf(train.buf,"rm -rf %s",argvs);
                       system(train.buf);
                       break;
                   }
               case PWD:
                   if(argvs != NULL)
                   {
                       ret = -1;
                       break;
                   }
                   send_ack(newfd,1);
                   getcwd(dir_name,sizeof(dir_name));
                   train.len = strlen(dir_name)+1;
                   strcpy(train.buf,dir_name);
                   sendseg(newfd,&train);
                   break;
               case EXIT:
                   times = time(NULL);
                   ctime_r(&times,nowtime);
                   fprintf(logfd,"\nat time %s:", nowtime);
                   fprintf(logfd,"client exit.\n");
                   free(argvs);
                   close(newfd);
                   fclose(logfd);
                   goto start;
                   break;
               default:
                   ret = -1;
                   break;
               }
               free(argvs);
               if(ret == -1)
               {
                   ret = send_ack(newfd,0);
                   if(ret == -1)
                   {
                       perror("sendack");
                       close(newfd);
                       times = time(NULL);
                       ctime_r(&times,nowtime);
                       fprintf(logfd,"\nat time %s:connection error\n", nowtime);
                       fclose(logfd);
                       goto start;
                   }
                   times = time(NULL);
                   ctime_r(&times,nowtime);
                   fprintf(logfd,"\nat time %s:command error.\n",nowtime);
               }
               times = time(NULL);
               ctime_r(&times,nowtime);
               fprintf(logfd,"\nat time %s:cmd received finished successfully\n", nowtime);
           }
    }
}
//创建线程，记录线程信息
void creatpool(info *pt)
{
    pt->t = (thread_info *)calloc(maxclient,sizeof(thread_info));
    for(int i = 0; i<maxclient;i++)
    {
        if(0!= pthread_create(&pt->t[i].tid,NULL,mainthread,pt))
        {
            printf("线程%dc创建失败\n",i);
            i--;
        }
        pt->t[i].st = rest;
    }
    printf("server is ready to work.\n");
}
int main()
{
    int ret;
    info *pt = (info *)calloc(1,sizeof(info));
    pt->t = (thread_info *)calloc(maxclient,sizeof(thread_info));

    if(-1 == chdir(homepath))
    {
        ret = mkdir(homepath,0755);
        RET_CHECK(ret,-1,"mkdir");
        ret = chdir(homepath);
        RET_CHECK(ret,-1,"chdir");
    }
    pt->p = (queue *)calloc(1,sizeof(queue));
    que_init(pt->p,maxclient);

    //创建线程
    creatpool(pt);

    int tcpfd = tcp_init(serverip,serverport);
    int newfd;

    //监听服务端
    while(1)
    {
        newfd = tcp_accept(tcpfd);
        RET_CHECK(newfd,-1,"client connect error");
        ret = que_in(pt->p,newfd);
        if(ret == -1)
        {
            printf("服务器已满载，请稍后再试\n");
            close(newfd);
            continue;
        }
        pthread_cond_signal(&pt->p->cond);
    }
}

