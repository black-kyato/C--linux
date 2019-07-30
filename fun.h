#ifndef __MYFUN_H
#define  __MYFUN_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ARG_CHECK(argc,num) {if(argc!=num){printf("arg error!\n");return -1;}}
#define RET_CHECK(ret,num,name) {if(num == ret){printf("errno = %d\n",errno);perror(name);return -1 ;}}
#define THREAD_CHECK(ret,funame) {if(ret!=0){printf("%s:%s\n",funame,strerror(ret));}}

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include<sys/ioctl.h>
#include<termios.h>

#include <time.h>
#include <signal.h>

#include <pthread.h>


#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <arpa/inet.h>

#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <aio.h>
#include <mysql/mysql.h>
#include <shadow.h>
#include<openssl/md5.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <set>
#include <map>
#include <deque>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <stack>
#include <queue>
#include <memory>

#define READ_DATA_SIZE  1024
#define MD5_SIZE        16
#define MD5_STR_LEN     (MD5_SIZE * 2)

extern "C"
{
    extern int set_no_block(int sfd);
    extern int tcp_init(const char* ip,int port);
    extern int tcp_accept(int sfd);
    extern int tcp_connect(const char* ip,int port);
    extern int udp_init(const char*ip,int port);
}
#endif

