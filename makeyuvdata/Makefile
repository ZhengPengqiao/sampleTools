
CFLAGS			+= -Wall

CFLAGS	+=
LDFLAGS	+=
LDFLAGS	+=

TARGET			:= makeuyvy

OBJS = makeuyvy.c

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c -O3 -o $@ $< $(CFLAGS)

%.o:%.cpp
	$(CXX) -c -O3 -o $@ $< $(CFLAGS)

clean:
	@rm -rf *.o $(TARGET)

