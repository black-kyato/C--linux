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

    char username[50];
    char salt[13] = {0};
    char password[300] = {0};

    seg train;
    int ret = 0;
    int ack = 0;

    char nowtime[50] ={0};
    time_t times;
    struct stat file_stat;
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

           pthread_mutex_unlock(&p->mutex);
           times =time(NULL);
           ctime_r(&times,nowtime);
           printf("connection with client set up at time :%s",nowtime);
           for(int i =0;i<maxclient;i++)
           {
               if(t[i].tid == pthread_self())
               {
                   t[i].st = busy;
               }
           }
           ack = recv_ack(newfd);
           if(ack == 1)
           {
               //登录验证
               recvseg(newfd,&train);
               strcpy(username, train.buf);
               //查找用户
               if(findUser(username)==1)
               {
                   send_ack(newfd,1);
               }
               else
               {
                   send_ack(newfd,0);
                   close(newfd);
                   goto start;
               }
               printf("user %s exists.\n",username);
               //获取用户的salt和密文密码
               ret = findPass(username,salt,password);
               if(ret != 0)
               {
                   printf("找不到用户记录\n");
                   close(newfd);
                   goto start;
               }
               //发送salt
               strcpy(train.buf,salt);
               train.len = strlen(train.buf)+1;
               sendseg(newfd,&train);

               //接收密文密码
               recvseg(newfd,&train);
               ret = strcmp(password, train.buf);
               if(ret ==0)
               {
                   send_ack(newfd,1);
                   times =time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"login",nowtime,1);
               }
               else
               {
                   send_ack(newfd,0);
                   close(newfd);
                   times =time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"login",nowtime,0);
                   goto start;
               }
           }
           if(ack == 0)
           {
               //注册用户
               printf("用户注册:\n");
               while(1)
               {
                   memset(&train,0,sizeof(train));
                   recvseg(newfd, &train);
                   printf("用户：%s\n", train.buf);
                   ret =  findUser(train.buf);
                   printf("ret =%d\n",ret);
                   if(ret == 0)
                   {
                       send_ack(newfd,1);
                       break;
                   }
                   else
                   {
                       send_ack(newfd,0);
                   }
               }
               strcpy(username, train.buf);
               recvseg(newfd, &train);
               ret = adduser(username, train.buf);
               if(ret == 0)
               {
                   printf("add user successfully.\n");
                   times =time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"register",nowtime,1);
               }
               else
               {
                   times =time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"register",nowtime,0);
               }
               close(newfd);
               goto start;
           }
           cmd_type handle;
           //当前虚拟路径
           char dir_name[1024] = {0};

           char *argvs = NULL;
           chdir(homepath);
           sprintf(dir_name,"/%s",username);

           while(1)
           {
               memset(&train,0,sizeof(seg));
               handle = OTHER;
               argvs =NULL;
               ret = recvseg(newfd,&train);
               if(ret ==-1)
               {
                   times = time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"lost connection",nowtime,1);
                   close(newfd);
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
               case MKDIR:
                   ret = addfile(username,dir_name,argvs,'d',NULL);
                   if(ret != 0)
                   {
                       times = time(NULL);
                       ctime_r(&times,nowtime);
                       writeLog(username,(char *)"mkdir",nowtime,0);
                       break;
                   }
                   send_ack(newfd,1);
                   times = time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"mkdir",nowtime,1);
                   break;
               case CD:
                   ret = findfile(username,dir_name,argvs,NULL);
                   //存在该目录
                   if(!ret)
                   {
                       sprintf(dir_name,"%s/%s",dir_name,argvs);
                       send_ack(newfd,1);
                       times = time(NULL);
                       ctime_r(&times,nowtime);
                       writeLog(username,(char *)"cd",nowtime,1);
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
                       memset(train.buf,0,sizeof(train.buf));
                       lsmysql(dir_name,&train);
                       puts(train.buf);
                       printf("%s\n", train.buf);
                       train.len = strlen(train.buf)+1;
                       sendseg(newfd,&train);
                       times = time(NULL);
                       ctime_r(&times,nowtime);
                       writeLog(username,(char *)"ls",nowtime,1);
                   }
                   else
                   {
                       ret = -1;
                   }
                   break;
               case PUTS:
                   send_ack(newfd,1);
                   recvseg(newfd,&train);
                   ack = findmd5(train.buf);
                   //存在相同文件，添加记录
                   if(ack)
                   {
                       send_ack(newfd,1);
                       addfile(username,dir_name,argvs,'f',train.buf);
                       times = time(NULL);
                       ctime_r(&times,nowtime);
                       writeLog(username,(char *)"upload file",nowtime,1);
                   }
                   else
                   {
                       send_ack(newfd,0);
                       addfile(username,dir_name,argvs,'f',train.buf);
                       ret = recv_file(newfd);
                       times = time(NULL);
                       ctime_r(&times,nowtime);
                       writeLog(username,(char *)"upload file",nowtime,1);
                   }
                   break;
               case GETS:
                   sscanf(argvs,"%s%d",filename,&fsize);
                   ret = findfile(username,dir_name,filename,train.buf);
                   if(ret ==-1)
                   {
                       break;
                   }
                   send_ack(newfd,1);
                   printf("md5 =%s\n",train.buf);
                   //发送MD5
                   train.len =1+strlen(train.buf);
                   sendseg(newfd,&train);
                   //发送文件大小
                   stat(train.buf,&file_stat);
                   train.len = sizeof(file_stat.st_size);
                   memcpy(train.buf,&file_stat.st_size,train.len);
                    sendseg(newfd,&train);

                   times = time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"download file",nowtime,1);
                   send_ip(newfd);
                   break;
               case REMOVE:
                    ret = delfile(username,dir_name,argvs);
                    if(ret !=0)
                    {
                        ret = -1;
                        break;
                    }
                    send_ack(newfd,1);
                    times = time(NULL);
                    ctime_r(&times,nowtime);
                    writeLog(username,(char *)"delete file",nowtime,1);
                    break;
               case PWD:
                   if(argvs != NULL)
                   {
                       ret = -1;
                       break;
                   }
                   send_ack(newfd,1);
                   train.len = strlen(dir_name)+1;
                   strcpy(train.buf,dir_name);
                   sendseg(newfd,&train);
                   ctime_r(&times,nowtime);
                   times = time(NULL);
                   writeLog(username,(char *)"pwd",nowtime,1);
                   break;
               case EXIT:
                   times = time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"exit",nowtime,1);
                   free(argvs);
                   close(newfd);
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
                       writeLog(username,(char *)"connection error", nowtime,1);                       
                       goto start;
                   }
                   times = time(NULL);
                   ctime_r(&times,nowtime);
                   writeLog(username,(char *)"command error",nowtime,1);
               }
               times = time(NULL);
               ctime_r(&times,nowtime);
               writeLog(username,(char *)"command complished",nowtime,1);
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

