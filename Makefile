
CFLAGS			:= -Wall
LDFLAGS			:= -lm -lpthread 

CFLAGS	+=
LDFLAGS	+=
LDFLAGS	+=
TARGET			:= BigLittEndian funPoint mkdirCmd unsig

all:$(TARGET)

BigLittEndianOBJS = \
	BigLittlrEnd.o

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

%.o:%.c
	$(CC) -c -O3 -o $@ $< $(CFLAGS)

%.o:%.cpp
	$(CXX) -c -O3 -o $@ $< $(CFLAGS)

help:
	echo use make name to make a signal software
	echo use make all  to make all software
	echo software List:"\n"
	echo $(TARGET)

clean:
	@rm -rf *.o $(TARGET)
