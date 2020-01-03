
CFLAGS			:= -Wall
LDFLAGS			:= -lm -lpthread

TARGET			:= spi_test

all:$(TARGET)

spi_test1:spi_test1.o
	$(CXX) -O3 -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c -O3 -o $@ $< $(CFLAGS) $(LDFLAGS)

%.o:%.cpp
	$(CXX) -c -O3 -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	@rm -rf *.o $(TARGET)
