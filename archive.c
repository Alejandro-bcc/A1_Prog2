#include <stdio.h>
#include <stdlib.h>
#include "archive.h"

unsigned char * arquivo_para_buffer(FILE *arq){
	
	unsigned char *buffer;
	unsigned int tam_arq;

	if(arq == NULL)
		return NULL;

	fseek(arq, 0, SEEK_END);
	tam_arq = ftell(arq);
	fseek(arq, 0, SEEK_SET);

	buffer = (unsigned char *)malloc(tam);

	if(buffer == NULL){
		free(buffer);
		return NULL;
	}

	fread(buffer, 1, tam_arq, arq);

	return buffer;
}
