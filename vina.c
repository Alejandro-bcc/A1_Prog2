#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "vina.h"

unsigned int arquivo_para_buffer(FILE *arq, unsigned char **buffer){
	
	unsigned int tam_arq;

	if(arq == NULL)
		return -1;

	fseek(arq, 0, SEEK_END);
	tam_arq = ftell(arq);
	fseek(arq, 0, SEEK_SET);
	
	if(tam_arq == 0)
		return -1;

	*buffer = (unsigned char *)malloc(tam_arq);

	if(*buffer == NULL){
		return -1;
	}

	if((fread(*buffer, 1, tam_arq, arq)) != tam_arq){
		free(*buffer);
		*buffer = NULL;
		return -1;
	}

	return tam_arq;
}

int buffer_para_arquivo(unsigned char *buffer, unsigned int tam_arq, FILE *arq){

	if(buffer == NULL || arq == NULL)
		return -1;

	if(tam_arq == 0)
		return -1;

	if((fwrite(buffer, 1, tam_arq, arq)) != tam_arq)
		return -1;

	return 0;
}

struct diretorio * cria_diretorio(){

	struct diretorio *novo_d;

	novo_d = (struct diretorio *)malloc(sizeof(struct diretorio));

	if(!novo_d)
		return NULL;

	novo_d->prim = NULL;
	novo_d->n_membros = 0;

	return novo_d;
}

struct membro * cria_membro(FILE *arq){
	
	struct membro novo_m;
	struct stat *info;
	int f;

	novo_m = (struct membro *)malloc(sizeof(struct stat *));
	info = (struct stat *)malloc(sizeof(struct stat *));
	
	if(info)
		return NULL;
	

	f = fileno(arq);
	fstat(f, info);


}

int insere_membro(struct diretorio *d, struct membro *m){
	

}




















