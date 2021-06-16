object:=$(patsubst %.c,%.o,$(wildcard *.c))

main:$(object)
	gcc -o main $(object)

main.o:listlib.o listlib.h
	gcc -c main.c
listlib.o:listlib.c listlib.h
	gcc -c listlib.c listlib.h

clean:
	rm *.o main