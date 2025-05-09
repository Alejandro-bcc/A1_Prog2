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
	int tam_inicial = 0;

	arc = (struct archive *)malloc(sizeof(struct archive));
	arc->arq = fopen(archive_nome, "a+");
	
	if(arc->arq == NULL){
		free(arc);
		return NULL;
	}
	
	fseek(arc->arq, 0, SEEK_SET);
	fwrite(&tam_inicial, TAM_N_MEMBROS, 1, arc->arq);

	arc->dir = cria_diretorio();
	arc->n_membros = 0;

	return arc;
}
 
int archive_inicializa(struct archive *arc){

	struct membro *atual;
	int n_membros_inicial = 0;

	if(arc == NULL)
		return -1;
	
	fseek(arc->arq, 0, SEEK_SET);
	fread(&n_membros_inicial, TAM_N_MEMBROS, 1, arc->arq);

	for(int i = 0; i < n_membros_inicial; i++){
		fread(&atual, TAM_PROPRIEDADES, 1, arc->arq);
		diretorio_insere(arc->dir, atual);
		arc->n_membros++;
	}

	fseek(arc->arq, 0, SEEK_SET);
	fwrite(&arc->n_membros, TAM_N_MEMBROS, 1, arc->arq);
	
	printf("n membros %d\n", arc->n_membros);
	return n_membros_inicial;
}
  
int archive_insere(struct archive *arc, const char *membro_nome){
	
	struct membro *novo_m;
	FILE *membro_arq;
	unsigned char *buffer;
	int tam_conteudo;

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

	fseek(arc->arq, 0 , SEEK_END);
	fwrite(novo_m, TAM_PROPRIEDADES, 1, arc->arq);
	tam_conteudo = arq_to_buffer(membro_arq, &buffer);
	fseek(arc->arq, 0 , SEEK_END);
	fwrite(buffer, tam_conteudo, 1, arc->arq);
	fseek(arc->arq, 0, SEEK_SET);
	diretorio_insere(arc->dir, novo_m);

	free(buffer);
	fclose(membro_arq);
	return 0;
}

int archive_print_cont(struct archive *arc){

	unsigned int n_membros;

	if(arc == NULL)
		return -1;
	fread(&n_membros, TAM_N_MEMBROS, 1, arc->arq);
	printf("num arqs: %d\n", n_membros);
	if(n_membros <= 0)
		return n_membros;
	
	if(n_membros != arc->dir->tam)
		return -1;
	
	diretorio_imprime(arc->dir);

	return n_membros;
}

