#include <fun.h>
#include "pool.h"

int main(int args,char *argv[])
{
    int sfd = tcp_init(argv[1],atoi(argv[2]));
    int newfd;
    struct aiocb rd;

    int fd;

    fd = open("test.txt",O_RDONLY);
    if(fd < 0)
    {
        perror("test.txt");
    }
    bzero(&rd,sizeof(rd));

    struct stat buf;
    fstat(fd,&buf);
    rd.aio_buf = malloc(buf.st_size);
    rd.aio_fildes = fd;
    rd.aio_nbytes = buf.st_size;
    rd.aio_offset = 0;
    int i = 1;
    while(i!=3)
    {
        newfd = tcp_accept(sfd);
        RET_CHECK(newfd,-1,"tcp-accept");
        Train_t train;
        memset(&train,0,sizeof(train));

        //transfer filename
        train.len = strlen(FILENAME);
        strcpy(train.p,FILENAME);
        int ret = send(newfd,&train,4+train.len,0);
        if(-1 ==ret)
        {
            printf("client is closed\n");
            close(fd);
            return 0;
        }
        //transfer the size of file 
        train.len=buf.st_size;
        ret = send(newfd,&train.len,sizeof(int),0);
        if(-1 ==ret)
        {
            printf("client is closed\n");
            close(fd);
            return 0;
        }
        printf("aio begin i =%d\n",i);
        rd.aio_buf = (char*) mmap (NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
        rd.aio_fildes = newfd;
        ret =aio_write(&rd);
        if(ret < 0)
        {
            perror("aio_read");
            exit(1);

        }
       /* while( aio_error(&rd) == EINPROGRESS )
        {
            printf(".");
        }
        ret = aio_return(&rd);
            printf("\n\n\n返回值为:%d\n",ret);
         if(ret < 0)
         {
                     perror("aio_write");
                             exit(1);
                                 
         }*/
        i++;
    }
}
