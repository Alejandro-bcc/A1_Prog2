#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "archive.h"

struct membro * cria_membro(FILE *arq){

	struct membro novo_m;
	struct stat *info;
	int f;

	novo_m = (struct membro *)malloc(sizeof(struct stat *));
	info = (struct stat *)malloc(sizeof(struct stat *));

	if(info)//?
		return NULL;

	free(info);

	novo->prox = NULL;
	novo->ant = NULL;

	return novo_m;
}

struct diretorio * cria_diretorio(){

	struct diretorio *novo_d;

	novo_d = (struct diretorio *)malloc(sizeof(struct diretorio));

	if(!novo_d)
		return NULL;

	novo_d->prim = NULL;
	novo_d->ult = NULL;
	novo_d->n_membros = 0;

	return novo_d;
}

void destroi_diretorio(struct diretorio *dir){

	struct membro *aux;

	if(dir == NULL)
		return;

	while(dir->prim != NULL){
		aux = dir->prim;
		dir->prim = aux->prox;
		free(aux);
	}

	free(dir);
}
/*   
int insere_membro(struct diretorio *d, struct membro *m){

}  */

