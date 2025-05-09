#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "archive.h"
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
   
int insere_diretorio(struct diretorio *d, struct membro *m){
	
	if(d == NULL || m == NULL)
		return -1;
	
	if(d->prim == NULL){
		d->prim = m;
		d->ult = m;

	}

	d->ult->prox = m;
	d->ult = m;

	d->tam++;

	return d->tam;
}  

int inicializa_archive(struct archive *arc){

	int n_membros = 0;

	if(arc == NULL)
		return -1;
	
	fseek(arc->arq, 0, SEEK_SET);
	fwrite(&n_membros, TAM_N_MEMBROS, 1, arc->arq);
	return 0;
}
  
int insere_archive(struct archive *arc, const char *nome){

	struct membro *novo_m;
	int tam_conteudo;

	if(arc == NULL || nome == NULL)
		return -1;

	novo_m = cria_membro(nome);

	if(novo_m == NULL)
		return -1;
	
	fseek(arc->arq, 0 , SEEK_END);
	fwrite(novo_m, TAM_METADADOS, 1, arc->arq);
	tam_conteudo = arq_to_buffer(arc->arq, &novo_m->conteudo);
	fseek(arc->arq, 0 , SEEK_END);
	fwrite(novo_m->conteudo, tam_conteudo, 1, arc->arq);
	fseek(arc->arq, 0, SEEK_SET);
	insere_diretorio(arc->dir, novo_m);

	return 0;
}




