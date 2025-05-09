# makefile vinac
# Alejandro Nava

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c99 -g

# gera executavel
all: vina.o archive.o
	$(CC) $(CFLAGS) vina.c -o vina archive.o

#lz/lz.o: lz/lz.c lz/lz.h
#	$(CC) $(CFLAGS) -c lz/lz.c -o lz/lz.o

archive.o: archive.c archive.h
	$(CC) -c $(CFLAGS) archive.c

valgrind: teste
	valgrind --leak-check=full --track-origins=yes ./vina

clean:
	rm -f *.o *~ vina

