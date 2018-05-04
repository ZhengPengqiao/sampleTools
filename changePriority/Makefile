
CFLAGS			:= -Wall
LDFLAGS			:= -lm -lpthread

TARGET			:= changePriority  changeThreadPriority pthreads_sched_test pthreadAttrPriority

all:$(TARGET)

# 
changePriority:changePriority.o
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

changeThreadPriority:changeThreadPriority.o
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

pthreads_sched_test:pthreads_sched_test.o
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)


pthreadAttrPriority:pthreadAttrPriority.o
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c -O3 -o $@ $< $(CFLAGS) $(LDFLAGS)

%.o:%.cpp
	$(CXX) -c -O3 -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	@rm -rf *.o $(TARGET)
