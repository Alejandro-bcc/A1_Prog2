#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lz/lz.h"

#define MAX_STR_LEN 101

int main(){
	
	FILE *arq;
	FILE *arq_comp;
	char nome[MAX_STR_LEN];
	unsigned char *buffer_in, *buffer_out;
	unsigned int arq_tam;
	
	printf("Digite o nome do arquivo:\n");
	scanf("%s", nome);
	arq = fopen(nome, "r+");
	arq_comp = fopen("output.txt", "w+");
	
	if(arq == NULL || arq_comp == NULL){
		printf("Erro ao tentar abrir o arquivos\n");
		return 1;
	}

	fseek(arq, 0, SEEK_END);
	arq_tam = ftell(arq);

	buffer_in = (unsigned char *)malloc(arq_tam + 1);
	buffer_out = (unsigned char *)malloc(arq_tam * 2);

	if(buffer_in == NULL || buffer_out == NULL){
		printf("Erro ao alocar os buffers\n");
		free(buffer_in);
		free(buffer_out);
		fclose(arq);
		fclose(arq_comp);
		return 1;
	}

	fread(buffer_in, 1, arq_tam, arq);
	buffer_in[arq_tam] = '\0';

	/*  
	LZ_Compress(buffer_in, buffer_out, arq_tam);
  */
	fputs((char *)buffer_in, arq_comp);

	free(buffer_in);
	free(buffer_out);
	fclose(arq);
	fclose(arq_comp);
	
	return 0;
}
