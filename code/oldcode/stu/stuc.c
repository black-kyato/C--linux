#include <fun.h>

typedef struct student
{
	int num;
	int score;
	char name[50];
}stu;

int main()
{
	stu s[3]={{001,600,"jackma"},{002,550,"ponyma"},{003,450,"tonyli"}};
	int f = open("doc",O_RDWR |O_CREAT);
	if(-1 == f)
	{
		perror("open failed!\n");
		 exit(-1);
	}
	int i =0;
	write(f,s,sizeof(stu)*3);
	lseek(f,0,SEEK_SET);
	stu st;
	for(i=0;i<3;i++)
	{
		read(f,&st,sizeof(stu));
		printf("学号：%d\t成绩：%d\t姓名：%s\n",st.num,st.score,st.name);
	}
}
