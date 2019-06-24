#include "net.h"

int delfile(char *user,char* path,char *filename)
{
    char md5[100] ={0};
    if(findfile(user,path,filename,md5)!=0)
    {
        return -1;
    }
    if(strcmp(md5,"\0")==0)
    {
        return -1;
    }
    MYSQL *mp = NULL;
    char cmd[200] = {0};
    sprintf(cmd,"delete from file where user = '%s'and path ='%s' and filename = '%s';",user,path,filename);
    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);
    mysql_query(mp,cmd);
    printf("del cmd:%s\n",cmd);
    mysql_close(mp);
    if(findmd5(md5)==0)
    {
       sprintf(cmd,"rm -f %s",md5);
       printf("delete cmd =%s\n",cmd); 
       system(cmd);
    }
    return 0;
}
int findfile(char *user,char * path,char *filename,char *md5)
{
    MYSQL *mp = NULL;
    MYSQL_RES *rp;
    MYSQL_ROW row;
    char cmd[200] = {0};
    sprintf(cmd,"select md5 from file where user = '%s'and path ='%s' and filename = '%s';",user,path,filename);
    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);
    printf("findfile cmd =%s\n",cmd);
    mysql_query(mp,cmd);
    rp = mysql_use_result(mp);
    row = mysql_fetch_row(rp);
    if(row == NULL)
    {
    mysql_close(mp);
        return -1;
    }
    else
    {
        if(md5!=NULL)
        {
            strcpy(md5,row[0]);
        }
    mysql_close(mp);
        return 0;
    }
}
int addfile(char *username,char *path,char *filename,char type,char *md5)
{
    if(findfile(username,path,filename,NULL)== 0 )
    {
        printf("存在相同文件名");
        return -1;
    }
    MYSQL *mp = NULL;
    char cmd[200] = {0};
    sprintf(cmd,"insert into file value('%s','%s','%s', '%c','%s');",filename,username,md5, type,path);
    printf("addfile:%s\n",cmd);
    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);
    mysql_query(mp,cmd);
    mysql_close(mp);
    return 0;
}
int findmd5(char *md5)
{
    MYSQL *mp = NULL;
    MYSQL_RES *rp;
    MYSQL_ROW row;
    char cmd[200] = {0};
    sprintf(cmd,"select * from file where md5 = '%s';",md5);
    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);
    mysql_query(mp,cmd);
    rp = mysql_use_result(mp);
    row = mysql_fetch_row(rp);
    mysql_close(mp);
    if(row == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int lsmysql(char *path,seg *train)
{
    MYSQL *mp = NULL;
    MYSQL_RES *rp;
    MYSQL_ROW row;
    char cmd[200] = {0};
    char  temp[500] = {0};
    sprintf(cmd,"select filename, user, type from file where path = '%s';",path);
    sprintf(temp,"%12s%12s%12s\n","filename","owner","filetype");

    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);
    mysql_query(mp,cmd);
    printf("ls cmd: %s\n",cmd);
    rp = mysql_use_result(mp);
    row = mysql_fetch_row(rp);
    int len = mysql_num_fields(rp);
    while(row!=NULL)
    {
        for(int i =0;i<len;i++)
        {
            sprintf(train->buf,"%s%12s",temp,row[i]);
            strcpy(temp, train->buf);
        }
        sprintf(train->buf,"%s\n",temp);
        strcpy(temp, train->buf);
        row = mysql_fetch_row(rp);
    }
    mysql_close(mp);
    return 0;
}

int writeLog(char *user, char *option,char *time,int done)
{
    MYSQL *mp = NULL;
    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);
    char cmd[500] = {0};
    time[strlen(time)-2]='\0';
    sprintf(cmd,"insert into log value ('%s','%s','%s',%d);",user,option, time,done);
    int ret = mysql_query(mp,cmd);
    return ret;
}
int Compute_file_md5(const char *file_path, char *md5_str)
{
    int i;
    int fd;
    int ret;
    unsigned char data[READ_DATA_SIZE];
    unsigned char md5_value[MD5_SIZE];
    MD5_CTX md5;

    fd = open(file_path, O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        return -1;
    }

    MD5_Init(&md5);

    while (1)
    {
        ret = read(fd, data, READ_DATA_SIZE);
        if (-1 == ret)
        {
            perror("read");
            return -1;
        }

        MD5_Update(&md5, data, ret);

        if (0 == ret || ret < READ_DATA_SIZE)
        {
            break;
        }
    }
    close(fd);

    MD5_Final(md5_value,&md5);

    for(i = 0; i < MD5_SIZE; i++)
    {
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
    }
    md5_str[MD5_STR_LEN] = '\0'; 
    return 0;
}
void randsalt(char *salt)
{
    int flag;
    srand(time(NULL));
    flag = rand()%3;
    salt[0]='$';
    salt[1]='6';
    salt[2]='$';
    for(int i = 3;i< 11;i++)
    {
        switch(flag)
        {
        case 0:
            salt[i]= rand()%26+'a';
            break;
        case 1:
            salt[i]=rand()%26+'A';
            break;
        case 2:
            salt[i]=rand()%10;
            break;
        default:
            salt[i]=0;
            break;
        }
    }
    salt[11]='$';
    salt[12]='\0';
}
int mysqlUser(MYSQL *mp, char *cmd)
{

    if(mp == NULL)
    {
        printf("connect MySQL error\n");
        return -1;

    }
    int ret = mysql_query(mp,cmd);
    if(ret == -1)
    {
        printf("mysql command error.\n");
        return -1;
    }
    return 0;
}
int findUser(char *user)
{
    MYSQL *mp = NULL;
    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);
    char cmd[100] = {0};
    sprintf(cmd,"select * from user where name= '%s';",user);
    int ret = mysqlUser(mp,cmd);
    if(ret != 0)
    {
        mysql_close(mp);
        return -1;
    }
    MYSQL_RES* rp;
    rp = mysql_use_result(mp);
    if(rp == NULL)
    {
        printf("NULL\n");
        return 0;
    }
    MYSQL_ROW row = mysql_fetch_row(rp);
    if(row == NULL)
    {
        mysql_close(mp);
        return 0;
    }
    else
    {
        mysql_close(mp);
        return 1;
    }
}
int findPass(char *user,char *salt,char *passwd)
{
    MYSQL *mp = NULL;
    MYSQL_RES *rp;
    MYSQL_ROW row;
    char cmd[200] = {0};
    sprintf(cmd,"select salt,password from user where name = '%s';",user);

    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);

    int ret = mysqlUser(mp,cmd);
    printf("mysql cmd =%s\n",cmd);
    if(ret != 0)
    {
        return -1;
    }
    rp = mysql_use_result(mp);
    row = mysql_fetch_row(rp);
    if(row == NULL)
    {
        printf("查无此用户\n");
        return -1;
    }
    strcpy(salt,row[0]);
    strcpy(passwd,row[1]);
    return 0;
}
int adduser(char *user,char *pass)
{
    char passwd[200] ={0};
    char salt[13];
    randsalt(salt);
    strcpy(passwd,crypt(pass,salt));
    char cmd[300];
    sprintf(cmd,"insert into user value('%s','%s','%s');",user,passwd,salt);

    MYSQL *mp = NULL;
    mp = mysql_init(NULL);
    mp = mysql_real_connect(mp,"localhost","root","jzk","netdisk",0,NULL,0);
    printf("执行命令:%s\n",cmd);
    int ret = mysqlUser(mp,cmd);
    mysql_close(mp);
    if(ret != 0)
    {
        printf("add user failed.\n");
        return -1;
    }
    return 0;
}
