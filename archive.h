#ifndef ARCHIVE
#define ARCHIVE
 	
#define MAX_STR_LEN 1024

struct membro{

    const char *nome;
    unsigned int udi;
    unsigned int tam_orig;
    unsigned int tam_comp;
    unsigned int data_mod;
    unsigned int ordem;
    unsigned int offset;
    unsigned char *conteudo;
    struct membro *prox;
	struct membro *ant;
};

struct diretorio{

    struct membro *prim;
    struct membro *ult;
    unsigned int tam;
};

struct archive{
	
	FILE *arq;
	struct diretorio *dir;
};

struct membro * cria_membro(const char *nome);

struct diretorio * cria_diretorio();

void destroi_diretorio(struct diretorio *dir);

int insere_membro(struct diretorio *d, struct membro *m);

int insere_archive(struct archive *arc, const char *nome);

int inicializa_archive(struct archive *arc);


#endif
