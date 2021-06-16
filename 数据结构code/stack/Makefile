object:=$(patsubst %.c,%.o,$(wildcard *.c))

main:$(object)
	gcc -o main $(object)

main.o:stacklib.o stacklib.h
	gcc -c main.c
stacklib.o:stacklib.c stacklib.h
	gcc -c stacklib.c stacklib.h

clean:
	rm *.o main