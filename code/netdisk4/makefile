EXE:=$(patsubst %.c, %.exe, $(wildcard *c.c))
#编译后得到的可执行文件的名字
CC:=gcc
#C编译器

CFLAGS:=-Wall -g 
#gcc编译参数 显示所有警告信息 可调试
LIBSO:=-lpthread -ltcp
#库依赖文件

OC := $(patsubst %.c, %.o, $(wildcard *.c))
#中间文件 .o依赖文件 用来生成可执行文件
$(EXE):$(OC)
	gcc -Wall -g serverc.c serverfunc.c mysqlc.c transferc.c  -o serverc.exe -I /usr/include/mysql/ -lmysqlclient -lpthread -ltcp -lcrypt -lssl -lcrypto
	gcc -Wall -g clientc.c transferc.c mysqlc.c  serverfunc.c -o clientc.exe -I /usr/include/mysql/ -lmysqlclient -lpthread -ltcp -lcrypt -lssl -lcrypto
	gcc -Wall -g createdb.c -o createdb.exe -I /usr/include/mysql/ -lmysqlclient
.PHONY :  remove clean rebuild debug
clean:
	rm -f $(wildcard *.o)
remove:
	rm -f $(EXE) $(wildcard *.o)
rebuild:
	clean all
debug:
	gdb -q $(EXE)
