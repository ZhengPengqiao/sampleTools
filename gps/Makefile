objs=uart1.o gps_analyse.o main.o
srcs=uart1.c gps_analyse.c main.c

gps_test: $(objs)
	$(CC) -o gps_test $(objs)

main.o: $(srcs) gps.h
	$(CC) -c  $(srcs)

uart1.o:  uart1.c
	$(CC) -c  uart1.c

analyse_gps.o: gps_analyse.c gps.h
	$(CC) -c  gps_analyse.c

clean:
	rm *.o