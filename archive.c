#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "archive.h"
#include "diretorio.h"
#include "manipulador_arquivos.h"

#define TAM_N_MEMBROS sizeof(int)
#define TAM_PROPRIEDADES sizeof(struct membro_disco)

   
struct archive * cria_archive(const char *archive_nome){
	
	struct archive *arc;

	arc = (struct archive *)malloc(sizeof(struct archive));
	if(arc == NULL)
		return NULL;

	strcpy(arc->nome, archive_nome);

	arc->arq = fopen(arc->nome, "rb+");
	if(arc->arq == NULL){
		arc->arq = fopen(arc->nome, "wb+");
		if(arc->arq == NULL){
			free(arc);
			return NULL;
		}
	}
	
	arc->dir = cria_diretorio();

	return arc;
}
 
int archive_inicializa(struct archive *arc){

	struct membro *atual;
	struct membro_disco *info;
	int n_membros_inicial = 0;

	if(arc == NULL)
		return -1;
	
	fseek(arc->arq, 0, SEEK_SET);
	if(tam_arq(arc->arq) == 0){
		printf("Archive originalmente vazio!\n");
		fwrite(&n_membros_inicial, TAM_N_MEMBROS, 1, arc->arq);
		return n_membros_inicial;
	}

	if (fread(&n_membros_inicial, TAM_N_MEMBROS, 1, arc->arq) != 1) {
        printf("Erro ao ler número de membros.\n");
        return -1;
    }
	if(n_membros_inicial == 0){
		printf("Archive originalmente vazio!\n");
		return n_membros_inicial;
	}

	for(int i = 0; i < n_membros_inicial; i++){
		info = (struct membro_disco *)malloc(sizeof(struct membro_disco));
		if(!info)
			return -1;

		if(fread(info, TAM_PROPRIEDADES, 1, arc->arq) != 1){
			free(info);
			return -1;
		}

		atual = membro_inicializa(info);
		if(!atual){
			free(info);
			return -1;
		}

		diretorio_insere(arc->dir, atual);
	}

	fseek(arc->arq, 0, SEEK_SET);
	
	return n_membros_inicial;
}
  
int archive_insere(struct archive *arc, const char *membro_nome){
	
	struct membro *novo_m;
	struct membro *atual;
	FILE *membro_arq;
	FILE *temp_arq;
	unsigned char *buffer;
	int tam_conteudo;
	unsigned int offset, ordem;

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
	
	// Calcula ordem de inserção para o novo membro
	if(!arc->dir->ult){
		// Archive vazio, ordem 1
		ordem = 1;
	}else{
		// Archive não vazio, ordem do ultimo membro + 1
		ordem = arc->dir->ult->info->ordem + 1;
	}
	novo_m->info->ordem = ordem;

	// Insere o novo membro no diretorio
	diretorio_insere(arc->dir, novo_m);

	// Atualiza o offset de todos os membros apos a inserção
	offset = TAM_N_MEMBROS + (TAM_PROPRIEDADES * arc->dir->tam);
	atual = arc->dir->prim;
	while(atual != NULL){
		atual->info->offset_ant = atual->info->offset;
		atual->info->offset = offset;
		offset += atual->info->tam_orig;
		atual = atual->prox;
	}

	// Escreve o espaço do diretorio do archive atualizado num arquivo temporário
	temp_arq = fopen("temp", "wb+");
	if(temp_arq == NULL){
		fclose(membro_arq);
		return -1;
	}
	fseek(temp_arq, 0, SEEK_SET);
	fwrite(&arc->dir->tam, TAM_N_MEMBROS, 1, temp_arq);
	atual = arc->dir->prim;
	while(atual != NULL){
		fwrite(atual->info, TAM_PROPRIEDADES, 1, temp_arq);
		atual = atual->prox;
	}

	// Escreve o conteudo dos membros no arquivo temporario
	atual = arc->dir->prim;
	while(atual != NULL){
		if(strcmp(atual->info->nome, novo_m->info->nome) == 0){
			// Novo membro, le do arquivo original
			fseek(membro_arq, 0, SEEK_SET);
			tam_conteudo = arq_to_buffer(membro_arq, &buffer);
			fwrite(buffer, tam_conteudo, 1, temp_arq);
			free(buffer);
		}else{
			// Membro já existente, le do archive original usando o offset antigo
			buffer = malloc(atual->info->tam_orig);
			fseek(arc->arq, atual->info->offset_ant, SEEK_SET);
			fread(buffer, atual->info->tam_orig, 1, arc->arq);
			fwrite(buffer, atual->info->tam_orig, 1, temp_arq);
			free(buffer);
		}
		atual = atual->prox;
	}

	fclose(membro_arq);
	fclose(temp_arq);
	fclose(arc->arq);

	remove(arc->nome);
	rename("temp", arc->nome); 

	arc->arq = fopen(arc->nome, "rb+");

	//Sucesso, retorna 0
	return 0;
}

int archive_print_cont(struct archive *arc){

	if(arc == NULL)
		return -1;
	
	if(arc->dir->tam == 0){
		printf("Archive vazio! Nada para listar\n");
		return -1;
	}

	printf("%-16s\t%-5s\t%-10s\t%-10s\t%-19s\t%-5s\n",
    		"Nome", "UDI", "Tam. Orig.", "Tam. Disc.", "Data Mod.", "Ordem");
	diretorio_imprime(arc->dir);

	return 0;
}

