
CFLAGS			:= -Wall
LDFLAGS			:= -lm -lpthread 

CFLAGS	+=
LDFLAGS	+=
LDFLAGS	+=
TARGET			:= BigLittEndian funPoint mkdirCmd unsig ifTest ifJudgement \
				rmdirCmd mountCmd mod ANSICtrl showProgress mktime_localtime\
				copyFile chmod systemCmd popenFun checkProgram

all:$(TARGET)

checkProgramOBJS = \
	checkProgram.o
checkProgram:$(checkProgramOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

fockOBJS = \
	fock.o
fock:$(fockOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

popenFunOBJS = \
	popenFun.o
popenFun:$(popenFunOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)


systemCmdOBJS = \
	systemCmd.o
systemCmd:$(systemCmdOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)



chmodOBJS = \
	chmod.o
chmod:$(chmodOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)


copyFileOBJS = \
	copyFile.o
copyFile:$(copyFileOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)


mktime_localtimeOBJS = \
	mktime_localtime.o
mktime_localtime:$(mktime_localtimeOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

ANSICtrlOBJS = \
	ANSICtrl.o
ANSICtrl:$(ANSICtrlOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

BigLittEndianOBJS = \
	BigLittEndian.o
BigLittEndian:$(BigLittEndianOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

funPointOBJS = \
	funPoint.o
funPoint:$(funPointOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

mkdirCmdOBJS = \
	mkdirCmd.o
mkdirCmd:$(mkdirCmdOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

unsigOBJS = \
	unsig.o
unsig:$(unsigOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)


ifTestOBJS = \
	ifTest.o
ifTest:$(ifTestOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

ifJudgementOBJS = \
	ifJudgement.o
ifJudgement:$(ifJudgementOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

rmdirCmdOBJS = \
	rmdirCmd.o
rmdirCmd:$(rmdirCmdOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

mountCmdOBJS = \
	mountCmd.o
mountCmd:$(mountCmdOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

modOBJS = \
	mod.o
mod:$(modOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)


showProgressOBJS = \
	showProgress.o
showProgress:$(showProgressOBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

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
