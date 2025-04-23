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
	unsigned char *conteudo;
	struct membro *prox;
};

unsigned int arquivo_para_buffer(FILE *arq, unsigned char **buffer);

int buffer_para_arquivo(unsigned char *buffer, unsigned int tam_arq, FILE *arq);

struct diretorio * cria_diretorio();

int insere_membro(struct diretorio *d);

#endif
