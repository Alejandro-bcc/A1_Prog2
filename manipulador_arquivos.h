#ifndef MANIPULADOR
#define MANIPULADOR

#include <stdio.h>

int arq_to_buffer(FILE *arq, unsigned char **buffer);

int buffer_to_arq(unsigned char *buffer, unsigned int tam_arq, FILE *arq);

int tam_arq(FILE *arq);

int arq_comprime(FILE *arq, unsigned char **buffer_out);

void print_cont_arq(FILE *arq);

#endif
