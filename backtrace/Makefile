
libadd = add.o 
backtrace = backtrace.o dump.o 

# 编译生成libadd.so
libadd: $(libadd)
	$(CC)  -g -rdynamic $< -fPIC -shared -o libadd.so


# 编译生成backtrace可执行文件
backtrace: $(backtrace)
	$(CC) -g -rdynamic $^ -L. -ladd -Wl,-rpath=. -o $@

%.o:%.c
	$(CC) -c -O3 -o $@ $< $(CFLAGS)

%.o:%.cpp
	$(CXX) -c -O3 -o $@ $< $(CFLAGS)


.PHYON:clean
clean:
	rm *.o libadd.so backtrace addelf.txt -rf