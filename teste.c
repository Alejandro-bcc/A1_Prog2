#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lz.h"

#define MAX_STR_LEN 101

int main(){
	
	FILE *arq;
	char nome[MAX_STR_LEN];
	unsigned int arq_tam;
	
	printf("Digite o nome do arquivo:\n");
	scanf("%s", nome);
	arq = fopen(nome, "r+");
	
	if(arq == NULL){
		printf("Erro ao tentar abrir o arquivo\n");
		return 1;
	}

	fseek(arq, 0, SEEK_END);
	arq_tam = ftell(file);
	printf("tamanho do arquivo %d\n", arq_tam);

	fclose(arq);
	
	return 0;
}
