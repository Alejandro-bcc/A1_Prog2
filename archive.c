#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "archive.h"
#include "diretorio.h"
#include "manipulador_arquivos.h"

#define TAM_N_MEMBROS sizeof(int)
#define TAM_METADADOS sizeof(struct membro)

   
struct archive * cria_archive(const char *archive_nome){
	
	struct archive *arc;

	arc = (struct archive *)malloc(sizeof(struct archive));
	arc->arq = fopen(archive_nome, "a+");
	arc->dir = cria_diretorio();

	return arc;
}
 
int archive_inicializa(struct archive *arc){

	int n_membros = 0;

	if(arc == NULL)
		return -1;
	
	fseek(arc->arq, 0, SEEK_SET);
	fwrite(&n_membros, TAM_N_MEMBROS, 1, arc->arq);
	return 0;
}
  
int archive_insere(struct archive *arc, const char *nome){

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
	diretorio_insere(arc->dir, novo_m);

	return 0;
}




