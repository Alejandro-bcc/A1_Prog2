#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "archive.h"
#include "diretorio.h"
#include "manipulador_arquivos.h"

#define COMPRIME 1
#define NAO_COMPRIME 0

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
	char archive_extensao[] = ".vc";
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
	}else{
		if(!strstr(argv[2], archive_extensao)){
			printf("Insira um archive válido, com extesão .vc!\n");
			print_error();
			return 1;
		}
	}

	opcao = argv[1][1];
	archive_nome = argv[2];
	
	arc = cria_archive(archive_nome);
	
	archive_inicializa(arc);

	switch(opcao){
		case 'p':
			printf("Opcao -p:\n");

			if(argv[3] == NULL){
				printf("Argumentos insuficientes!\n");
				print_error();
				return -1;
			}
			for(int i = 3; i < argc; i++){
				printf("Inserindo %s em %s sem compressão\n", argv[i], archive_nome);
				if(archive_insere(arc, argv[i], NAO_COMPRIME) < 0){
					printf("Erro ao inserir! Abortando...\n");
					return -1;
				}else{
					printf("Inserção feita com sucesso!\n");
				}
			}
			break;
		case 'i':
			printf("Opcao -i:\n");

			if(argv[3] == NULL){
				printf("Argumentos insuficientes!\n");
				print_error();
				return -1;
			}
			for(int i = 3; i < argc; i++){
				printf("Inserindo %s em %s com compressão\n", argv[i], archive_nome);
				if(archive_insere(arc, argv[i], COMPRIME) < 0){
					printf("Erro ao inserir! Abortando...\n");
					return -1;
				}else{
					printf("Inserção feita com sucesso!\n");
				}
			}
			break;
		case 'm':
			printf("Opcao -m:\n");
			if(argv[3] == NULL){
				printf("Argumentos insuficientes!\n");
				print_error();
				return -1;
			}
			printf("Movendo %s para ", argv[3]);
			if(argv[4] == NULL){
				printf("o começo do archive.\n");
			}else{
				printf("exatamente depois de %s.\n", argv[4]);
			}
			if(archive_move(arc, argv[3], argv[4]) < 0)
				printf("Movimentação feita com sucesso!\n");
			break;
		case 'x':
			printf("Opcao -x:\n");
			if(argv[3] != NULL){
				for(int i = 3; i < argc; i++){
					printf("Extraindo %s do archive.\n", argv[i]);
					if(archive_extrai(arc, argv[i]) < 0){
						printf("Erro ao tentar extrair o membro. Abortando...\n");
						return -1;
					}else{
						printf("Membro extraido com sucesso.\n");
					}
				}
			}else{
				printf("Extraindo todos os membros do archive.\n");
				archive_extrai_todos(arc);
			}
			break;
		case 'r':
			printf("Opcao -r:\n");
			if(argv[3] == NULL){
				printf("Argumentos insuficientes!\n");
				print_error();
				return -1;
			}
			for(int i = 3; i < argc; i++){
				printf("Removendo %s do archive.\n", argv[i]);	
				if(archive_remove(arc, argv[i]) < 0){
					printf("Erro ao remover! Abortando...\n");
					return -1;
				}else{
					printf("Remoção feita com sucesso!\n");
				}
			}
			break;
		case 'c':
			printf("Opcao -c:\nListando conteúdo do archive:\n");
			if(archive_print_cont(arc) < 0)
				printf("Erro ao listar conteúdo. Abortando...\n");
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
