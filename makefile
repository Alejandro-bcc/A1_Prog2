# makefile vinac
# Alejandro Nava

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c99 -g

# gera executavel
all: teste

teste: teste.o lz/lz.o vina.o
	$(CC) $(CFLAGS) -o teste teste.o lz/lz.o vina.o

teste.o: teste.c lz/lz.h
	$(CC) $(CFLAGS) -c teste.c

lz/lz.o: lz/lz.c lz/lz.h
	$(CC) $(CFLAGS) -c lz/lz.c -o lz/lz.o

vina.o: vina.c vina.h
	$(CC) $(CFLAGS) -c vina.c -o vina.o

valgrind: teste
	valgrind --leak-check=full --track-origins=yes ./teste

clean:
	rm -f *.o *~ teste
