
CFLAGS			:= -Wall
LDFLAGS			:= -lm -lpthread 

TARGET			:= BigLittEndian funPoint mkdirCmd unsig ifTest ifJudgement \
				rmdirCmd mountCmd mod ANSICtrl showProgress mktime_localtime\
				copyFile chmod systemCmd popenFun checkProgram

all:$(TARGET)

checkProgram:checkProgram.o
fock:fock.o
popenFun:popenFun.o
systemCmd:systemCmd.o
chmod:chmod.o
copyFile:copyFile.o
mktime_localtime:mktime_localtime.o
ANSICtrl:ANSICtrl.o
BigLittEndian:BigLittEndian.o
funPoint:funPoint.o
mkdirCmd:mkdirCmd.o
unsig:unsig.o
ifTest:ifTest.o
ifJudgement:ifJudgement.o
rmdirCmd:rmdirCmd.o
mountCmd:mountCmd.o
mod:mod.o
showProgress:showProgress.o

.PHONY:backtrace_complite
backtrace_complite:
	make -C ./backtrace

.PHONY:backtrace_clean
backtrace_clean:
	make -C ./backtrace clean


%.o:%.c
	$(CC) -c -O3 -o $@ $< $(CFLAGS)

%.o:%.cpp
	$(CXX) -c -O3 -o $@ $< $(CFLAGS)

help:
	@echo use \"make name\" to make a signal software
	@echo use \"make all\"  to make all software
	@echo software List:"\n"
	@echo BigLittEndian  : 判断程序大小端字节序
	@echo funPoint       : 函数指针的示例
	@echo mkdirCmd       : 创建文件夹的示例
	@echo rmdirCmd       : 删除文件夹的示例
	@echo unsig          : 无符号数溢出的测试
	@echo ifTest         : if的整数真假值测试
	@echo ifJudgement    : if中,赋值后再判断的示例
	@echo mod            : 正数负数取模之后的结果
	@echo fock			 : c语言创建进程
	@echo checkProgram   : c语言检查指定程序是否在运行 


clean:
	@rm -rf *.o $(TARGET)
