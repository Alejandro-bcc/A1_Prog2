# makefile vinac
# Alejandro Nava

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c99 -g

# gera executavel
all: teste

teste: teste.o lz/lz.o
	$(CC) $(CFLAGS) -o teste teste.o lz/lz.o

teste.o: teste.c lz/lz.h
	$(CC) $(CFLAGS) -c teste.c

lz/lz.o: lz/lz.c lz/lz.h
	$(CC) $(CFLAGS) -c lz/lz.c -o lz/lz.o

clean:
	rm -f *.o *~ teste
