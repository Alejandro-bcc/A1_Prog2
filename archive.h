#ifndef ARCHIVE
#define ARCHIVE

#define MAX_STR_LEN 256

struct diretorio{
	
	struct membro *prim;
	unsigned int n_membros;
};

struct membro{
	
	char nome[MAX_STR_LEN];
	unsigned int udi;
	unsigned int tam_orig;
	unsigned int tam_comp;
	unsigned int data_mod;
	unsigned int ordem;
	unsigned int offset;
	char *conteudo;
	struct membro *prox;
};

char * arquivo_para_buffer(FILE *arq);

#endif
