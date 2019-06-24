#include <fun.h>
void print_result(MYSQL_RES *rp)
{
    MYSQL_ROW row;
    int len = mysql_num_fields(rp);
    while((row = mysql_fetch_row(rp))!=NULL)
    {
        for(int i =0;i<len;i++)
        {
            printf("%s==",row[i]);
        }
        printf("\n");
    }
}
int main()
{
    MYSQL *mp = NULL;
    MYSQL_RES *rp;
    mp = mysql_init(NULL);
    
    mp = mysql_real_connect(mp,"localhost","root","jzk","day3mon6",0,NULL,0);
    if(mp == NULL)
    {
        printf("connect MySQL error\n");
        return -1;
    }
    int ret = mysql_query(mp,"select * from teacher union select * from student;");
    if(ret)
    {
        printf("commend error\n");
        return -2;
    }
    rp = mysql_use_result(mp);
    print_result(rp);
    mysql_free_result(rp);

    char cmd[100];
    printf("请输入选课的sql插入命令\n");
    gets(cmd);
    ret = mysql_query(mp,cmd);
    if(ret)
    {
        printf("commend error\n");
        return -3;
    }
    ret = mysql_query(mp,"select * from select_class;");
    rp = mysql_use_result(mp);
    print_result(rp);
    mysql_free_result(rp);
    mysql_close(mp);
    return 0;
}

