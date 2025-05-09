#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "archive.h"

struct membro * cria_membro(const char *nome){

	struct membro *novo_m;
	struct stat info;

	novo_m = (struct membro *)malloc(sizeof(struct membro));

	if(novo_m == NULL)
		return NULL;

	if(stat(nome, &info) == -1){
		free(novo_m);
		return NULL;
	}
	
	novo_m->nome = nome;
	novo_m->udi = info.st_uid;
	novo_m->tam_orig = info.st_size;
	novo_m->tam_comp = 0;
	novo_m->data_mod = info.st_mtime;
	novo_m->ordem = 0;
	novo_m->offset = 0;
	novo_m->conteudo = NULL;
	novo_m->prox = NULL;
	novo_m->ant = NULL;

	return novo_m;
}

struct diretorio * cria_diretorio(){

	struct diretorio *novo_d;

	novo_d = (struct diretorio *)malloc(sizeof(struct diretorio));

	if(!novo_d)
		return NULL;

	novo_d->prim = NULL;
	novo_d->ult = NULL;
	novo_d->tam = 0;

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

