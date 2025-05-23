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
	novo_m->compresso = FALSE;
	novo_m->prox = NULL;
	novo_m->ant = NULL;

	strncpy(novo_m->info->nome, membro_nome, MAX_NAME_LEN - 1);
	novo_m->info->udi = st.st_uid;
	novo_m->info->tam_orig = st.st_size;
	novo_m->info->tam_comp = 0;
	novo_m->info->tam_disc = 0;
	novo_m->info->data_mod = st.st_mtime;
	novo_m->info->ordem = 0;
	novo_m->info->offset_ant = 0;
	novo_m->info->offset = 0;

	return novo_m;
}

struct membro * inicializa_membro(struct membro_disco *info){

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

int busca_membro(struct diretorio *d, const char *membro_nome){
	
	struct membro *aux;
	int i;

	if(!d || d->prim == NULL)
		return -1;

	aux = d->prim;
	i = 0;
	while(aux != NULL){
		if(strcmp(aux->info->nome, membro_nome) == 0)
			return i;
		aux = aux->prox;
		i++;
	}

	// Percorreu todos os membros e não achou
	return -1;
} 

struct membro * acha_membro(struct diretorio *d, const char *membro_nome){
	
	struct membro *aux;

	if(!d || !d->prim)
		return NULL;
	
	aux = d->prim;
	while(aux != NULL){
		if(strcmp(aux->info->nome, membro_nome) == 0)
			return aux;
		aux = aux->prox;
	}

	return aux;
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

	// Insere no final e atribui a ordem de certa ao novo membro no archive
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

int diretorio_remove(struct diretorio *d, const char *membro_nome){
	
	struct membro *m, *atual;
	unsigned int ordem;

	if(!d || !d->prim)
		return -1;

	m = acha_membro(d, membro_nome);
	if(!m)
		return -1;

	// Atualiza ponteiros
	if(m->ant != NULL){
		m->ant->prox = m->prox;
	}else{
		d->prim = m->prox;
	}
	if(m->prox != NULL){
		m->prox->ant = m->ant;
	}else{
		d->ult = m->ant;
	}

	free(m->info);
	free(m);
	
	d->tam--;

	// Atualiza a ordem de todos os membros
	atual = d->prim;
	ordem = 1;
	while(atual != NULL){
		atual->info->ordem = ordem++;
		atual = atual->prox;
	}

	return d->tam;
}

int diretorio_move(struct diretorio *d, int pos_membro, int pos_target){
	
	struct membro *m, *target, *atual;
	int i;

	if(!d || !d->prim || pos_membro < 0 || pos_membro >= (int)(d->tam) || pos_target >= (int)(d->tam))
		return -1;

	if(pos_membro == pos_target)
		return 0;

	if(pos_membro == pos_target + 1)
		return 0;

	// Acha o membro e o target
	m = d->prim;
	for(i = 0; m != NULL && i < pos_membro; i++)
		m = m->prox;
	
	if(pos_target >= 0){
		target = d->prim;
		for(i = 0; target != NULL && i < pos_target; i++)
				target = target->prox;
	}else{
		target = NULL;
	}
	
	// Remove o membro da sua posição atual
	if(m->ant != NULL){
		m->ant->prox = m->prox;
	}else{
		d->prim = m->prox;
	}
	if(m->prox != NULL){
		m->prox->ant = m->ant;
	}else{
		d->ult = m->ant;
	}

	// Ajusta os ponteiros
	if(target == NULL){
		m->ant = NULL;
		m->prox = d->prim;
		if(d->prim != NULL)
			d->prim->ant = m;
		d->prim = m;
		if(d->ult == NULL)
			d->ult = m;
	}else{
		m->ant = target;
		m->prox = target->prox;
		if(target->prox != NULL)
			target->prox->ant = m;
		target->prox = m;
		if(d->ult == target)
			d->ult = m;
	}
	
	// Atribui a nova ordem aos membros do archive necessarios
	if(target == NULL){
		m->info->ordem = 1;
		atual = m;
	}else if(pos_membro < pos_target){
		// Membro movimentado estava antes do target		
		if(pos_membro == 0){
		// Estava no primeira posição da lista
			atual = d->prim;
			atual->info->ordem = 1;
		}else{
			for(atual = d->prim; atual != NULL && atual->info->ordem < (unsigned int)(pos_membro + 2); atual = atual->prox);
		}
		}else{
	   	// Membro movimentado estava depois do target
			atual = target;
		}

	while(atual->prox != NULL){
		atual->prox->info->ordem = atual->info->ordem + 1;
		atual = atual->prox;
	}
	return 0;
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

        printf("%-16s\t%-5u\t%-10u\t%-10u\t%-10u\t%-19s\t%-5u\n",
            	aux->info->nome,
            	aux->info->udi,
            	aux->info->tam_orig,
				aux->info->tam_comp,
            	aux->info->tam_disc,
            	data_str,
            	aux->info->ordem
        );
		aux = aux->prox;
	}
}


