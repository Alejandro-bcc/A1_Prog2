#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lz/lz.h"
#include "vina.h"

int main(){
	
	FILE *arq;
	char nome[MAX_STR_LEN];
	unsigned char *conteudo;
	unsigned int tam;
	
	scanf("%s", nome);
	arq = fopen(nome, "rb");
	
	conteudo = NULL;
	tam = arquivo_para_buffer(arq, &conteudo);
	
	fclose(arq);

	scanf("%s", nome);
	arq = fopen(nome, "wb");
	
	buffer_para_arquivo(conteudo, tam, arq);
	
	free(conteudo);
	fclose(arq);

	return 0;
}
