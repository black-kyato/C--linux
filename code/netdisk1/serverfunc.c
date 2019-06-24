#include "net.h"

//命令解析函数
cmd_type trans_cmd(seg *train)
{
    int i = 0;
    //忽略命令前的空格
    for(;i<train->len;i++)
    {
        if(train->buf[i]!=' ')
        {
            break;
        }
    }

    if(train->buf[i] == 'c'&&train->buf[i+1]=='d'&&train->buf[i+2]==' ')
    {
        return CD;
    }
    if(train->buf[i] == 'l'&&train->buf[i+1]=='s')
    {
        return LS;
    }
    if(train->buf[i] == 'p'&&train->buf[i+1]=='u'&&train->buf[i+2]=='t'&&train->buf[i+3]=='s'&&train->buf[i+4]==' ')
    {
        return PUTS;
    }
    if(train->buf[i] == 'g'&&train->buf[i+1]=='e'&&train->buf[i+2]=='t'&&train->buf[i+3]=='s'&&train->buf[i+4]==' ')
    {
        return GETS;
    }
    if(train->buf[i] == 'p'&&train->buf[i+1]=='w'&&train->buf[i+2]=='d')
    {
        return PWD; 
    }
    char p[8];
    memcpy(p,&train->buf[i],7);
    p[7] = '\0';
    if(!strcmp(p,"remove "))
    {
        return REMOVE;
    }
    p[4]='\0';
    if(!strcmp(p,"exit"))
    {
        return EXIT;
    }
    return OTHER;
}
char * get_argv(seg *train)
{         
    char *p = NULL;
    int i;
    int flag = 0;
    for(i = 0;i<train->len;i++)
    {     
        if(train->buf[i]!=' ')
        {
            flag =1;

        }
        else
        {
            if(train->buf[i+1]!=' '&&flag == 1)
            {
                p = (char *)calloc(train->len - i -1,sizeof(char));
                for(int j = i+1;j<train->len;j++)
                {
                    p[j-i-1]=train->buf[j];

                }
                return p;
            }

        }

    }     
    return p;
}


