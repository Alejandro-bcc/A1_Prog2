# Makefile para vinac
# Autor: Alejandro Nava - Dinf - UFPR - 2025/1

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c99 -g
OBJS = vina.o archive.o

# gera executavel
all: $(OBJS)
	$(CC) $(CFLAGS) -o vina $(OBJS)

#lz/lz.o: lz/lz.c lz/lz.h
#	$(CC) $(CFLAGS) -c lz/lz.c -o lz/lz.o

vina.o: vina.c archive.h
	$(CC) $(CFLAGS) -c vina.c

archive.o: archive.c archive.h
	$(CC) $(CFLAGS) -c archive.c

valgrind: teste
	valgrind --leak-check=full --track-origins=yes ./vina

clean:
	rm -f *.o *~ vina

