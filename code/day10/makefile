EXE:=$(patsubst %.c, %, $(wildcard *c.c))
#编译后得到的可执行文件的名字

CC:=gcc
#C编译器

CFLAGS:=-Wall -g 
#gcc编译参数 显示所有警告信息 可调试

LIBSO:=
#库依赖文件

OC := $(patsubst %.c, %.o, $(wildcard *.c))
#中间文件 .o依赖文件 用来生成可执行文件

all:$(EXE)
	@for i in $(EXE);do $(CC) $(CFLAGS)-o $${i} $${i}.c;done


.PHONY : clean rebuild debug
clean:
	rm -f $(EXE) $(wildcard *.o)
rebuild:
	clean all
debug:
	gdb -q $(EXE)
