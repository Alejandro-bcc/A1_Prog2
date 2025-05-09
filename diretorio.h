#ifndef DIRETORIO
#define DIRETORIO

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

struct membro * cria_membro(const char *nome);

struct diretorio * cria_diretorio();

void destroi_diretorio(struct diretorio *dir);

int diretorio_insere(struct diretorio *d, struct membro *m);

#endif
