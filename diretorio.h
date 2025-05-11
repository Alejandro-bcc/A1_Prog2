#ifndef DIRETORIO
#define DIRETORIO

#include <time.h>

#define MAX_NAME_LEN 1024

struct membro_disco{
    
    char nome[MAX_NAME_LEN];
    unsigned int udi;
    unsigned int tam_orig;
    unsigned int tam_comp;
	unsigned int tam_disc;
    time_t data_mod;
    unsigned int ordem;
    unsigned int offset;
    unsigned int offset_ant;
};

struct membro{

    struct membro_disco *info;
    struct membro *prox;
	struct membro *ant;
};

struct diretorio{

    struct membro *prim;
    struct membro *ult;
    unsigned int tam;
};

struct membro * cria_membro(const char *nome);

struct membro * membro_inicializa(struct membro_disco *info);

struct diretorio * cria_diretorio();

void destroi_diretorio(struct diretorio *dir);

int diretorio_insere(struct diretorio *d, struct membro *m);

void diretorio_imprime(struct diretorio *d); 

#endif
