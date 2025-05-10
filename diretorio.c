#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "diretorio.h"
#include "manipulador_arquivos.h"

#define TAM_N_MEMBROS sizeof(int)
#define TAM_METADADOS sizeof(struct membro)

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
	
	strcpy(novo_m->nome, nome);
	novo_m->udi = info.st_uid;
	novo_m->tam_orig = info.st_size;
	novo_m->tam_comp = 0;
	novo_m->data_mod = info.st_mtime;
	novo_m->ordem = 0;
	novo_m->offset = 0;
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
		aux = NULL;
	}

	free(dir);
}
   
int diretorio_insere(struct diretorio *d, struct membro *m){
	
	struct membro *atual;

	if(d == NULL || m == NULL)
		return -1;
	
	atual = d->prim;
	while(atual != NULL){
		if(atual == m)
			return -1;
		atual = atual->prox;
	}

	if(d->prim == NULL){
		d->prim = m;
		d->ult = m;	
		m->prox = NULL;
		m->ant = NULL;

		return ++d->tam;
	}
		m->prox = NULL;
		m->ant = d->ult;
		d->ult->prox = m;
		d->ult = m;
	

	return ++d->tam;
}

void diretorio_imprime(struct diretorio *d){
	
	struct membro *aux;

	if(d == NULL)
		return;

	aux = d->prim;
	while(aux != NULL){
		printf("%s 		", aux->nome);
		printf("%d		", aux->udi);
		printf("%d		", aux->tam_orig);
		printf("%d		", aux->tam_comp);
 		printf("%s		", asctime(gmtime(&aux->data_mod)));
		printf("%d\n", aux->ordem);
		aux = aux->prox;
	}
}
