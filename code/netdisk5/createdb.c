#include <fun.h>

int main()
{
    MYSQL *mp =NULL;
    mp = mysql_init(NULL);

    mp = mysql_real_connect(mp,"localhost","root","jzk",NULL,0,NULL,0);
    if(mp == NULL)
    {
        printf("connect MySQL error\n");
        return -1;

    }
    int ret = mysql_query(mp,"creare database netdisk;use netdisk;create table user(name varchar(50),password varchar(200),salt varchar(40));create table file(filename varchar(100),user varchar(50),md5 varchar(100),type char,path varchar(100));create table log(username varchar(50),options varchar(200),time varchar(50),result bool);");
    if(ret)
    {
        printf("commend error\n");
        return -2;

    }
    printf("create database successfully.\n");
    return 0;
}

