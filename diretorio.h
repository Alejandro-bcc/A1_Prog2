#ifndef DIRETORIO
#define DIRETORIO

#include <time.h>

struct membro{

    char nome[1024];
    unsigned int udi;
    unsigned int tam_orig;
    unsigned int tam_comp;
    time_t data_mod;
    unsigned int ordem;
    unsigned int offset;
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

void diretorio_imprime(struct diretorio *d); 

#endif
