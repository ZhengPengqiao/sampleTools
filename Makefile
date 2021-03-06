
CFLAGS			:= -Wall
LDFLAGS			:= -lm -lpthread 

TARGET			:= BigLittEndian funPoint mkdirCmd \
				rmdirCmd ANSICtrl showProgress mktime_localtime\
				copyFile chmod systemCmd popenFun checkProgram sublime-imfix \
				find-PLL-NMK  audioEsCali mkdirfile getenv string-find

all:$(TARGET)

checkProgram:checkProgram.o
mkdirfile:mkdirfile.o
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
rmdirCmd:rmdirCmd.o
showProgress:showProgress.o
find-PLL-NMK:find-PLL-NMK.o
audioEsCali:audioEsCali.o
user_input:user_input.o
getenv:getenv.o
string-find:string-find.o
	 $(CXX) -o $@ $^ $(LDFLAGS)

#>>>>>>>>>> backtrace
.PHONY:backtrace_complite
backtrace_complite:
	make -C ./backtrace

.PHONY:backtrace_clean
backtrace_clean:
	make -C ./backtrace clean
#<<<<<<<<<< backtrace

#>>>>>>>>>> checkParam
.PHONY:checkParam_complite
checkParam_complite:
	make -C ./checkParam

.PHONY:checkParam_clean
checkParam_clean:
	make -C ./checkParam clean
#<<<<<<<<<< checkParam

#>>>>>>>>>>  yuyvtorgb565
.PHONY:yuyvtorgb565_complite
yuyvtorgb565_complite:
	make -C ./yuyvtorgb565

.PHONY:yuyvtorgb565_clean
yuyvtorgb565_clean:
	make -C ./yuyvtorgb565 clean
#<<<<<<<<<<  yuyvtorgb565

#>>>>>>>>>>  changePriority
.PHONY:changePriority_complite
changePriority_complite:
	make -C ./changePriority

.PHONY:changePriority_clean
changePriority_clean:
	make -C ./changePriority clean
#<<<<<<<<<<  changePriority

#>>>>>>>>>>  getopt
.PHONY:getopt_complite
getopt_complite:
	make -C ./getopt

.PHONY:getopt_clean
getopt_clean:
	make -C ./getopt clean
#<<<<<<<<<<  getopt

#>>>>>>>>>> sublime-imfix
sublime-imfix:.PHONY
	gcc -shared -o libsublime-imfix.so sublime_imfix.c -fPIC `pkg-config --libs --cflags gtk+-2.0` 
#<<<<<<<<<< sublime-imfix


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
	@echo ifJudgement    : if中,赋值后再判断的示例
	@echo fock			 : c语言创建进程
	@echo checkProgram   : c语言检查指定程序是否在运行 
	@echo isErrCount 	 : 使用位移，处理连续错误的判断
	@echo audioEsCali 	 : 计算MP4中ES的值
	@echo find-PLL-NMK 	 : 根据给定的频率，计算满足条件的N,M,K的值
	@echo sublime_imfix  : sublime中文支持
	@echo make-uyvy-data : 根据参数生成yuv原始数据
	@echo changePriority : 将指定的进程ID的优先级设为最高
	@echo user_input     : 在用户空间创建虚拟的输入设备

clean:
	@rm -rf *.o $(TARGET)
