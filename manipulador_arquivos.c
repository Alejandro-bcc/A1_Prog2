#include <stdio.h>
#include <stdlib.h>

#include "manipulador_arquivos.h"
#include "lz/lz.h"

int tam_arq(FILE *arq){
	
	int tamanho;

	if(arq == NULL)
		return -1;
	
	fseek(arq, 0, SEEK_END);
	tamanho = ftell(arq);
	fseek(arq, 0, SEEK_SET);

	return tamanho;
}

void print_cont_arq(FILE *arq){
	
	unsigned char *buff; 
	if(arq == NULL){
		printf("arq null\n");
		return;
	}
	arq_to_buffer(arq, &buff);
	
	if(buff != NULL){
		printf("%s\n", buff);
		free(buff);
	}
}

int arq_to_buffer(FILE *arq, unsigned char **buffer){

	unsigned int tam_arq;

	if(arq == NULL)
		return -1;

	fseek(arq, 0, SEEK_END);
	tam_arq = ftell(arq);
	fseek(arq, 0, SEEK_SET);

	if(tam_arq == 0)
		return -1;

	*buffer = (unsigned char *)malloc(tam_arq);

	if(*buffer == NULL)
		return -1;


	if((fread(*buffer, 1, tam_arq, arq)) != tam_arq){
		free(*buffer);
		*buffer = NULL;
		return -1;
	}	

	return tam_arq;
}

int buffer_to_arq(unsigned char *buffer, unsigned int tam_arq, FILE *arq){

    if(buffer == NULL || arq == NULL)
        return -1;

    if(tam_arq == 0)
        return -1;

    if((fwrite(buffer, 1, tam_arq, arq)) != tam_arq)
        return -1;

    return 0;
}

int arq_comprime(FILE *arq, unsigned char **buffer_out){

	unsigned char *buffer_in;
	unsigned int outsize;
	int tam_orig, tam_comp;
	
	if(!arq || !buffer_out)
		return -1;

	tam_orig = arq_to_buffer(arq, &buffer_in);

	outsize = (tam_orig * 257)/256 + 1;
	*buffer_out = (unsigned char *)malloc(outsize);
	if(!*buffer_out){
		free(buffer_in);
		return -1;
	}

	tam_comp = LZ_Compress(buffer_in, *buffer_out, tam_orig);

	if(tam_comp < tam_orig){
		free(buffer_in);
		return tam_comp;
	}else{
		free(*buffer_out);
		*buffer_out = buffer_in; // Coloca conteudo original do arquivo no buffer de saida
		return tam_comp;
	}
}
