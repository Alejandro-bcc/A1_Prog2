# Makefile para vinac
# Autor: Alejandro Nava - Dinf - UFPR - 2025/1

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c99 -g
OBJS = vina.o archive.o diretorio.o manipulador_arquivos.o

# gera executavel
all: $(OBJS)
	$(CC) $(CFLAGS) -o vina $(OBJS)

#lz/lz.o: lz/lz.c lz/lz.h
#	$(CC) $(CFLAGS) -c lz/lz.c -o lz/lz.o

vina.o: vina.c archive.h
	$(CC) $(CFLAGS) -c vina.c

archive.o: archive.c archive.h
	$(CC) $(CFLAGS) -c archive.c

diretorio.o: diretorio.c diretorio.h
	$(CC) $(CFLAGS) -c diretorio.c

manipulador_arquivos.o: manipulador_arquivos.c manipulador_arquivos.h
	$(CC) $(CFLAGS) -c manipulador_arquivos.c

valgrind: all
	valgrind --leak-check=full --track-origins=yes ./vina

clean:
	rm -f *.o *~ vina

