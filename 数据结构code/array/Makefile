object:=$(patsubst %.c,%.o,$(wildcard *.c))

main:$(object)
	gcc -o main $(object)

main.o:arraylib.o arraylib.h
	gcc -c main.c
arraylib.o:arraylib.c arraylib.h
	gcc -c arraylib.c arraylib.h

clean:
	rm *.o main