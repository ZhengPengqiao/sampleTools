CFLAGS            += -Wall -g

CFLAGS    +=
LDFLAGS    +=

TARGET            := main

OBJS = main.o

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c -O3 -o $@ $< $(CFLAGS)

%.o:%.cpp
	$(CXX) -c -O3 -o $@ $< $(CFLAGS)

$(TARGET)_debug:$(TARGET)
	$(OBJCOPY) --add-gnu-debuglink=main.debug main

clean:
	@rm -rf *.o $(TARGET)
	@rm -rf main.debug
