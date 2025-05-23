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

		atual = inicializa_membro(info);
		if(!atual){
			free(info);
			return -1;
		}

		diretorio_insere(arc->dir, atual);
	}

	fseek(arc->arq, 0, SEEK_SET);
	
	return n_membros_inicial;
}
  
int archive_insere(struct archive *arc, const char *membro_nome, int comprime){
	
	struct membro *novo_m;
	struct membro *atual;
	FILE *membro_arq;
	FILE *temp_arq;
	unsigned char *buffer, *buffer_ant;
	int tam_conteudo;
	unsigned int offset;

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
	
	// Pega o conteudo e tamanho do novo membro, compresso ou não
	fseek(membro_arq, 0, SEEK_SET);
	if(comprime){
		tam_conteudo = arq_comprime(membro_arq, &buffer);
		novo_m->info->tam_comp = tam_conteudo;
		if(novo_m->info->tam_comp < novo_m->info->tam_orig){
			novo_m->compresso = TRUE;
		}else{
			novo_m->compresso = FALSE;
			tam_conteudo = novo_m->info->tam_orig;
		}
	}else{
		tam_conteudo = arq_to_buffer(membro_arq, &buffer);
		novo_m->compresso = FALSE;
	}
	novo_m->info->tam_disc = tam_conteudo;

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
			fwrite(buffer, novo_m->info->tam_disc, 1, temp_arq);
		}else{
			// Membro já existente, le do archive original usando o offset antigo
			buffer_ant = malloc(atual->info->tam_disc);
			fseek(arc->arq, atual->info->offset_ant, SEEK_SET);
			fread(buffer_ant, atual->info->tam_disc, 1, arc->arq);
			fwrite(buffer_ant, atual->info->tam_disc, 1, temp_arq);
			free(buffer_ant);
		}
		atual = atual->prox;
	}

	free(buffer);

	fclose(membro_arq);
	fclose(temp_arq);
	fclose(arc->arq);

	remove(arc->nome);
	rename("temp", arc->nome); 

	arc->arq = fopen(arc->nome, "rb+");

	//Sucesso, retorna 0
	return 0;
}

int archive_remove(struct archive *arc, const char *membro_nome){
	
	struct membro *atual;
	FILE *temp_arq;
	unsigned char *buffer;
	unsigned int offset;
	int zero = 0;

	if(!arc)
		return -1;

	if((busca_membro(arc->dir, membro_nome)) < 0)
		return -1;

	// Antes de remover o membro do diretorio salva o offset de todos os membros
	atual = arc->dir->prim;
	while(atual != NULL){	
		atual->info->offset_ant = atual->info->offset;
		atual = atual->prox;
	}

	diretorio_remove(arc->dir, membro_nome);
	
	if(arc->dir->tam == 0){
		fclose(arc->arq);
		arc->arq = fopen(arc->nome, "wb+");
		fwrite(&zero, TAM_N_MEMBROS, 1, arc->arq);
		return 0;
	}

	// Atualiza o offset de todos os membros
	offset = TAM_N_MEMBROS + (TAM_PROPRIEDADES * arc->dir->tam);
	atual = arc->dir->prim;
	while(atual != NULL){	
		atual->info->offset = offset;
		offset += atual->info->tam_disc;
		atual = atual->prox;
	}

	// Escreve o diretorio atualizado do archive no arquivo temporario
	temp_arq = fopen("temp", "wb+");
	if(!temp_arq)
		return -1;

	fwrite(&arc->dir->tam, TAM_N_MEMBROS, 1, temp_arq);
	atual = arc->dir->prim;
	while(atual != NULL){
		fwrite(atual->info, TAM_PROPRIEDADES, 1, temp_arq);
		atual = atual->prox;
	}

	// Escreve o conteudo dos membros na nova ordem
	atual = arc->dir->prim;
	while(atual != NULL){
		buffer = malloc(atual->info->tam_disc);
		fseek(arc->arq, atual->info->offset_ant, SEEK_SET);
		fread(buffer, atual->info->tam_disc, 1, arc->arq);
		fwrite(buffer, atual->info->tam_disc, 1, temp_arq);
		free(buffer);
		atual = atual->prox;
	}

	fclose(temp_arq);
	fclose(arc->arq);

	remove(arc->nome);
	rename("temp", arc->nome);

	arc->arq = fopen(arc->nome, "rb+");

	return 0;
}

int archive_move(struct archive *arc, const char *membro_nome, const char *target_nome){
	
	struct membro *atual;
	FILE *temp_arq;
	int pos_membro, pos_target;
	unsigned char *buffer;
	unsigned int offset;

	if(!arc)
		return -1;

	pos_membro = busca_membro(arc->dir, membro_nome);
	if(pos_membro < 0)
		return -1;

	if(target_nome != NULL){
		pos_target = busca_membro(arc->dir, target_nome);
		if(pos_target < 0)
			return -1;
	}else{
		pos_target = -1;
	}
	
	diretorio_move(arc->dir, pos_membro, pos_target);

	// Atualiza o offset de todos os membros
	offset = TAM_N_MEMBROS + (TAM_PROPRIEDADES * arc->dir->tam);
	atual = arc->dir->prim;
	while(atual != NULL){
		atual->info->offset_ant = atual->info->offset;
		atual->info->offset = offset;
		offset += atual->info->tam_disc;
		atual = atual->prox;
	}

    // Escreve o diretorio atualizado do archive no arquivo temporario
    temp_arq = fopen("temp", "wb+");
    if (!temp_arq)
        return -1;

    fwrite(&arc->dir->tam, TAM_N_MEMBROS, 1, temp_arq);
    atual = arc->dir->prim;
    while(atual != NULL){
        fwrite(atual->info, TAM_PROPRIEDADES, 1, temp_arq);
        atual = atual->prox;
    }

    // Escreve o conteudo dos membros na nova ordem
    atual = arc->dir->prim;
    while(atual != NULL){
        buffer = malloc(atual->info->tam_disc);
        fseek(arc->arq, atual->info->offset_ant, SEEK_SET);
        fread(buffer, atual->info->tam_disc, 1, arc->arq);
        fwrite(buffer, atual->info->tam_disc, 1, temp_arq);
        free(buffer);
        atual = atual->prox;
    }

    fclose(temp_arq);
    fclose(arc->arq);

    remove(arc->nome);
    rename("temp", arc->nome);

    arc->arq = fopen(arc->nome, "rb+");

    return 0;
}

int archive_extrai(struct archive *arc, const char *membro_nome){
	
	struct membro *m;
	unsigned char *buffer;

	if(!arc)
		return -1;
	
	m = acha_membro(arc->dir, membro_nome);
	if(!m)
		return -1;
	/*   *
	fseek(arc->arq, m->info->offset, SEEK_SET);

	if(m->compresso){
		buffer = malloc(m->info->tam_comp);
		fread(buffer, 1, m->info->tam_comp, arc->arq);
		arq_descomprime(buffer, m->info->tam_comp, m->info->tam_orig, m->info->nome);
	}else{
		buffer = malloc(m->info->tam_orig);
		buffer_to_arq(buffer, m->info->tam_orig, m->info->nome);
		fread(buffer, 1, m->info->tam_orig, arc->arq);
	}

	free(buffer);  */

    if(m->compresso){
        // Read compressed data
        buffer = malloc(m->info->tam_comp);
        if (!buffer) return -1;
        fseek(arc->arq, m->info->offset, SEEK_SET);
        if (fread(buffer, 1, m->info->tam_comp, arc->arq) != m->info->tam_comp) {
            free(buffer);
            return -1;
        }
        // Decompress and write exactly tam_orig bytes
        arq_descomprime(buffer, m->info->tam_comp, m->info->tam_orig, m->info->nome);
        free(buffer);
    } else {
        // Read uncompressed data
        buffer = malloc(m->info->tam_orig);
        if (!buffer) return -1;
        fseek(arc->arq, m->info->offset, SEEK_SET);
        if (fread(buffer, 1, m->info->tam_orig, arc->arq) != m->info->tam_orig) {
            free(buffer);
            return -1;
        }
        // Write exactly tam_orig bytes
        buffer_to_arq(buffer, m->info->tam_orig, m->info->nome);
        free(buffer);
    }
	return 0;
}

int archive_extrai_todos(struct archive *arc){
	
	struct membro *atual;

	if(!arc || arc->dir->tam == 0)
		return -1;

	atual = arc->dir->prim;
	while(atual != NULL){
		archive_extrai(arc, atual->info->nome);
		atual = atual->prox;
	}

	return 0;
}

int archive_print_cont(struct archive *arc){

	if(arc == NULL)
		return -1;
	
	if(arc->dir->tam == 0){
		printf("Archive vazio! Nada para listar\n");
		return -1;
	}

	printf("%-16s\t%-5s\t%-10s\t%-10s\t%-10s\t%-19s\t%-5s\n",
    		"Nome", "UDI", "Tam. Orig.", "Tam. Comp.", "Tam. Disc.", "Data Mod.", "Ordem");
	diretorio_imprime(arc->dir);

	return 0;
}

