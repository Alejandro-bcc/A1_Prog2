#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "archive.h"
#include "diretorio.h"
#include "manipulador_arquivos.h"

#define TAM_N_MEMBROS sizeof(int)
#define TAM_PROPRIEDADES sizeof(struct membro)

   
struct archive * cria_archive(const char *archive_nome){
	
	struct archive *arc;

	arc = (struct archive *)malloc(sizeof(struct archive));
	arc->arq = fopen(archive_nome, "a+");
	
	if(arc->arq == NULL){
		free(arc);
		return NULL;
	}
	
	arc->dir = cria_diretorio();

	return arc;
}
 
int archive_inicializa(struct archive *arc){

	struct membro atual;
	int n_membros_inicial = 0;

	if(arc == NULL)
		return -1;
	
	fseek(arc->arq, 0, SEEK_SET);

	if(tam_arq(arc->arq) == 0){
		printf("Archive originalmente vazio!\n");
		fwrite(&n_membros_inicial, TAM_N_MEMBROS, 1, arc->arq);
		return n_membros_inicial;
	}

	fread(&n_membros_inicial, TAM_N_MEMBROS, 1, arc->arq);
	if(n_membros_inicial == 0){
		printf("Archive originalmente vazio!\n");
		return n_membros_inicial;
	}

	printf("1: Archive originalmente com %d membros\n", n_membros_inicial);

	for(int i = 0; i < n_membros_inicial; i++){
		fread(&atual, TAM_PROPRIEDADES, 1, arc->arq);
		diretorio_insere(arc->dir, &atual);
	}

	printf("2: Archive originalmente com %d membros\n", arc->dir->tam);

	fseek(arc->arq, 0, SEEK_SET);
	
	return n_membros_inicial;
}
  
int archive_insere(struct archive *arc, const char *membro_nome){
	
	struct membro *novo_m;
	FILE *membro_arq;
	unsigned char *buffer;
	int tam_conteudo, n_membros;

	if(arc == NULL || membro_nome == NULL)
		return -1;

	membro_arq = fopen(membro_nome, "rb");
	if(membro_arq == NULL)
		return -1;

	novo_m = cria_membro(membro_nome);
	if(novo_m == NULL){
		fclose(membro_arq);
		return -1;
	}

	n_membros = diretorio_insere(arc->dir, novo_m);
	printf("numero de membros apos inserção: %d\n", n_membros);
	fseek(arc->arq, 0, SEEK_SET);
	fwrite(&n_membros, TAM_N_MEMBROS, 1, arc->arq);
	fwrite(novo_m, TAM_PROPRIEDADES, 1, arc->arq);
	tam_conteudo = arq_to_buffer(membro_arq, &buffer);
	fseek(arc->arq, 0 , SEEK_END);
	fwrite(buffer, tam_conteudo, 1, arc->arq);
	fseek(arc->arq, 0, SEEK_SET);

	free(buffer);
	fclose(membro_arq);
	return 0;
}

int archive_print_cont(struct archive *arc){

	unsigned int n_membros;

	if(arc == NULL)
		return -1;
	
	if(arc->dir->tam == 0){
		printf("Archive vazio! Nada para listar\n");
		return -1;
	}

	fread(&n_membros, TAM_N_MEMBROS, 1, arc->arq);
	if(n_membros <= 0)
		return n_membros;
	
	if(n_membros != arc->dir->tam)
		return -1;
	
	diretorio_imprime(arc->dir);

	return n_membros;
}

