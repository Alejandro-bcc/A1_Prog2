#include <stdio.h>
#include <stdlib.h>

#include "archive.h"

void print_error(){
	
	printf("\n");
	printf("Formato adequado:\n\n");

	printf("vinac <opção> <archive> [membro1 membro2 ...]\n\n");
	
	printf("Opções:\n");

	printf("-p : insere/acrescenta um ou mais membros sem compressão ao archive. Caso o membro já exista no archive, ele deve ser substituído. Novos membros são inseridos respeitando a ordem da linha de comando, ao final do archive;\n\n");

	printf("-i) : insere/acrescenta um ou mais membros com compressão ao archive. Caso o membro já exista no archive, ele deve ser substituído. Novos membros são inseridos respeitando a ordem da linha de comando, ao final do archive;\n\n");

	printf("-m membro : move o membro indicado na linha de comando para imediatamente depois do membro target existente em archive. A movimentação deve ocorrer na seção de dados do archive; para mover para o início, o target deve ser NULL.\n\n");

	printf("-x : extrai os membros indicados de archive. Se os membros não forem indicados, todos devem ser extraídos. A extração consiste em ler o membro de archive e criar um arquivo correspondente, com conteúdo idêntico, em disco;\n\n");

	printf("-r : remove os membros indicados de archive;\n\n");

	printf("-c : lista o conteúdo de archive em ordem, incluindo as propriedades de cada membro (nome, UID, tamanho original, tamanho em disco e data de modificação) e sua ordem no arquivo.\n\n");
}

int main (int argc, char **argv){
	
	char opcao;
	char *archive_nome;
	struct archive *arc;

	if(argc < 2){
		printf("Argumentos insuficientes!\n");
		print_error();
		return 1;
	}

	if(argv[1][0] != '-'){
		printf("Insira uma opção válida com '-'!\n");
		print_error();
		return 1;
	}

	if(argv[2] == NULL){
		printf("Insira o nome do archive!\n");	
		print_error();
		return 1;
	}

	opcao = argv[1][1];
	archive_nome = argv[2];
	
	arc = (struct archive *)malloc(sizeof(struct archive));
	arc->arq = fopen(archive_nome, "w+");
	arc->dir = cria_diretorio();
	

	switch(opcao){
		case 'p':
			printf("Opcao p\n");
			break;
		case 'i':
			printf("Opcao i\n");
			break;
		case 'm':
			printf("Opcao m\n");
			break;
		case 'x':
			printf("Opcao x\n");
			break;
		case 'r':
			printf("Opcao r\n");
			break;
		case 'c':
			printf("Opcao c\n");
			break;
		default:
			printf("Insira uma opção válida!\n");
			print_error();
	}

	destroi_diretorio(arc->dir);
	fclose(arc->arq);
	free(arc);
	return 0;
}
