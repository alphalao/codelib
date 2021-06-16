object:=$(patsubst %.c,%.o,$(wildcard *.c))

main:$(object)
	gcc -o main $(object)

main.o:queuelib.o queuelib.h
	gcc -c main.c
queuelib.o:queuelib.c queuelib.h
	gcc -c queuelib.c queuelib.h

clean:
	rm *.o main