EXE:=$(patsubst %.cc, %.out, $(wildcard *.cc))
#编译后得到的可执行文件的名字
CC:=g++
#C编译器

CFLAGS:=-Wall -g 
#gcc编译参数 显示所有警告信息 可调试
LIBSO:=-ltcp -lpthread
#库依赖文件

OC := $(patsubst %.cc, %.o, $(wildcard *.cc))
#中间文件 .o依赖文件 用来生成可执行文件

./bin/server:./src/server.cc ./src/transfer.cc
	g++ -Wall -g ./src/server.cc ./src/transfer.cc -o ./bin/server -lpthread -ltcp 

./bin/server:./src/server.cc ./src/transfer.cc

.PHONY : clean build debug
clean:
	rm -f $(EXE) $(wildcard *.o) $(wildcard *.out)
buildclient:
	g++ -Wall -g ./src/client.cc ./src/transfer.cc -o ./bin/client/client -lpthread -ltcp 
debug:
	gdb -q $(EXE)
