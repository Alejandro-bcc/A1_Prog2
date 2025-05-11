#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "diretorio.h"
#include "manipulador_arquivos.h"

#define TAM_N_MEMBROS sizeof(int)
#define TAM_METADADOS sizeof(struct membro)
#define DATA_STR_LEN 32

struct membro * cria_membro(const char *membro_nome){

	struct membro *novo_m;
	struct membro_disco *info;
	struct stat st;

	novo_m = (struct membro *)malloc(sizeof(struct membro));
	if(!novo_m)
		return NULL;

	info = (struct membro_disco *)malloc(sizeof(struct membro_disco));
	if(!info){
		free(novo_m);
		return NULL;
	}
	
	// Inicializa todos os bytes com zero
	memset(info, 0, sizeof(struct membro_disco));

	if(stat(membro_nome, &st) == -1){
		free(novo_m);
		free(info);
		return NULL;
	}

	novo_m->info = info;
	novo_m->prox = NULL;
	novo_m->ant = NULL;

	strncpy(novo_m->info->nome, membro_nome, MAX_NAME_LEN - 1);
	novo_m->info->udi = st.st_uid;
	novo_m->info->tam_orig = st.st_size;
	novo_m->info->tam_comp = 0;
	novo_m->info->tam_real = 0;
	novo_m->info->data_mod = st.st_mtime;
	novo_m->info->ordem = 0;
	novo_m->info->offset_ant = 0;
	novo_m->info->offset = 0;

	return novo_m;
}

struct membro * membro_inicializa(struct membro_disco *info){

	struct membro *m;

	if(!info)
		return NULL;

	m = (struct membro *)malloc(sizeof(struct membro));
	if(!m)
		return NULL;

	m->info = info;
	m->prox = NULL;
	m->ant = NULL;

	return m;
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
		free(aux->info);
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
		if(strcmp(m->info->nome, atual->info->nome) == 0){
			// Membro já existe, substitui
			m->ant = atual->ant;
			m->prox = atual->prox;
			if(atual->ant != NULL){
				atual->ant->prox = m;
			}else{
				d->prim = m;
			}
			if(atual->prox != NULL){
				atual->prox->ant = m;
			}else{
				d->ult = m;
			}
			// Recebe a ordem do membro original
			m->info->ordem = atual->info->ordem;
			free(atual->info);
			free(atual);
			return d->tam;
		}
		atual = atual->prox;
	}

	m->prox = NULL;
	m->ant = d->ult;

 	if(d->ult != NULL){
        d->ult->prox = m;
		// Diretorio não vazio, recebe ordem do ultimo membro + 1
		m->info->ordem = d->ult->info->ordem + 1;
	}else{
        d->prim = m;
		// Diretorio vazio, recebe ordem 1
		m->info->ordem = 1;
	}
    d->ult = m;

	return ++d->tam;
}

void diretorio_imprime(struct diretorio *d){
	
	struct membro *aux;
	char data_str[DATA_STR_LEN];

	if(d == NULL)
		return;

	aux = d->prim;
	while(aux != NULL){
		struct tm *tm_info; 
		tm_info = localtime(&aux->info->data_mod);
        strftime(data_str, sizeof(data_str), "%Y-%m-%d %H:%M:%S", tm_info);

        printf("%-16s\t%-5u\t%-10u\t%-10u\t%-19s\t%-5u\n",
            	aux->info->nome,
            	aux->info->udi,
            	aux->info->tam_orig,
            	aux->info->tam_comp,
            	data_str,
            	aux->info->ordem
        );
		aux = aux->prox;
	}
}
