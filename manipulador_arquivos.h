#ifndef MANIPULADOR
#define MANIPULADOR

#include <stdio.h>

int arq_to_buffer(FILE *arq, unsigned char **buffer);

int buffer_to_arq(unsigned char *buffer, unsigned int tam, const char *arq_nome);

int tam_arq(FILE *arq);

int arq_comprime(FILE *arq, unsigned char **buffer_out);

int arq_descomprime(unsigned char *arq_cont, unsigned int tam_orig, unsigned int tam_comp, const char *arq_nome);

void print_cont_arq(FILE *arq);

#endif
