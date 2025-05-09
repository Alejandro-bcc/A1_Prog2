#ifndef MANIPULADOR
#define MANIPULADOR

#include <stdio.h>

unsigned int arquivo_para_buffer(FILE *arq, unsigned char **buffer);

int buffer_para_arquivo(unsigned char *buffer, unsigned int tam_arq, FILE *arq);

#endif
