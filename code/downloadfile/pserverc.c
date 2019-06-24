#include <fun.h>
#include "pool.h"

typedef struct sfd 
{
    int sockfd;
    struct sfd *next;
}node,*pnode;
//队列结构，待通信的存放描述符
typedef struct{
    pnode head,tail;
    int capacity;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}queue;

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
int que_add(queue *p,int fd)
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

//出队,队列为空返回-1
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
void *thread_child(void *vp)
{
    queue *p = (queue *)vp;
    while(1)
    {
        pthread_mutex_lock(&p->mutex);
        pthread_cond_wait(&p->cond,&p->mutex);
        pthread_mutex_unlock(&p->mutex);

        int sockfd = que_out(p);
        if(sockfd == -1)
        {
            printf("socket = -1, que_out error.\n");
            continue;
        }
        transp(sockfd);
        sleep(10);
        close(sockfd);
    }
}

int  thread_pool(void * vp)
{
    queue *p =(queue *)vp;
    pthread_t *pt = (pthread_t*)calloc(p->capacity,sizeof(pthread_t));
    for(int i = 0;i<p->capacity;i++)
    {
        pt[i] = pthread_create(pt+i,NULL,thread_child,vp);
    }
    printf("server is ready for download request.\n");
    return 0;
}

int main(int args,char *argv[])
{
    ARG_CHECK(args,4);
    int capacity = atoi(argv[3]);
    int sfd = tcp_init(argv[1],atoi(argv[2]));
    queue *p = (queue *)calloc(1,sizeof(queue));
    que_init(p,capacity);
    thread_pool((void *)p);
    int newfd;
    int ret;
    while(1)
    {
        newfd = tcp_accept(sfd);
        RET_CHECK(newfd,-1,"tcp-accept");
        ret = que_add(p,newfd);
        if(ret == -1)
        {
            close(newfd);
            continue;
        }
        pthread_cond_signal(&p->cond);
    }   
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
    
    
    ret = sendfile(sfd,fd,NULL,train.len);
    
    if(ret!=train.len)
    {
        printf("pid = %d transfer error,ret = %d\n",getpid(),ret);
    }
    else{
    printf("pid = %d completed transfer of file %s\n",getpid(),FILENAME);
    }
    close(fd);
    return 0;
}
