#ifndef ARCHIVE
#define ARCHIVE

#include "diretorio.h"

struct archive{
	
	char nome[MAX_NAME_LEN];
	FILE *arq;
	struct diretorio *dir;
};

struct archive * cria_archive(const char *archive_nome);

int archive_insere(struct archive *arc, const char *membro_nome);

int archive_inicializa(struct archive *arc);

int archive_print_cont(struct archive *arc);
#endif
