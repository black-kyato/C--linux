EXE:=$(patsubst %.cc, %.out, $(wildcard *.cc))
#编译后得到的可执行文件的名字
CC:=g++
#C编译器

CFLAGS:=-Wall -g 
#gcc编译参数 显示所有警告信息 可调试
LIBSO:=-lpthread
#库依赖文件

OC := $(patsubst %.cc, %.o, $(wildcard *.cc))
#中间文件 .o依赖文件 用来生成可执行文件

all:$(EXE)
%.out:%.cc
	 $(CC) $(CFLAGS) $< -o $@ $(LIBSO)


.PHONY : clean rebuild debug
clean:
	rm -f $(EXE) $(wildcard *.o) $(wildcard *.out)
rebuild:
	clean all
debug:
	gdb -q $(EXE)
