#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lz/lz.h"

#define MAX_STR_LEN 101

int main(){
	
	FILE *arq;
	FILE *arq_comp;
	FILE *result;
	char nome[MAX_STR_LEN];
	unsigned char *buffer_in, *buffer_out;
	unsigned int arq_tam;
	unsigned int arq_comp_tam;
	
	printf("Digite o nome do arquivo:\n");
	scanf("%s", nome);
	arq = fopen(nome, "rb");
	arq_comp = fopen("output.txt", "wb");
	
	if(arq == NULL || arq_comp == NULL){
		printf("Erro ao tentar abrir o arquivos\n");
		fclose(arq);
		fclose(arq_comp);
		return 1;
	}

	fseek(arq, 0, SEEK_END);
	arq_tam = ftell(arq);
	fseek(arq, 0, SEEK_SET);

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
  
	LZ_Compress(buffer_in, buffer_out, arq_tam);
  
	fwrite(buffer_out, 1, arq_tam * 2, arq_comp);

	free(buffer_in);
	free(buffer_out);
	buffer_in = NULL;
	buffer_out = NULL;
	fclose(arq);
	fclose(arq_comp);

	arq_comp = fopen("output.txt", "rb");
	result = fopen("result.pdf", "wb");
	if(arq_comp == NULL){
		fclose(arq_comp);
		printf("erro ao tentar abrir arquivos\n");
		return 1;
	}

	fseek(arq_comp, 0, SEEK_END);
	arq_comp_tam = ftell(arq_comp);
	fseek(arq_comp, 0, SEEK_SET);

	buffer_in = (unsigned char *)malloc(arq_comp_tam);
	buffer_out = (unsigned char *)malloc(arq_tam* 2);

	if(buffer_in == NULL || buffer_out == NULL){
		printf("Erro ao alocar os buffers\n");
		free(buffer_in);
		free(buffer_out);
		fclose(arq);
		fclose(arq_comp);
		return 1;
	}
		
	fread(buffer_in, 1, arq_comp_tam, arq_comp);

	LZ_Uncompress(buffer_in, buffer_out, arq_comp_tam);

	fwrite(buffer_out, 1, arq_tam, result);

	fclose(arq_comp);
	fclose(result);

	return 0;
}
