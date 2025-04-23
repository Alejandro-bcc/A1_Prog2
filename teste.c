#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "lz/lz.h"
#include "vina.h"

int main(int argc, char **argv){
	
	int opcao;

	if(argc < 2){
		printf("Erro: parametros insuficientes\n");
		return 1;
	}

	while((opcao = getopt(argc, argv, ":ip:ic:m:x:r:c")) != 1)
		switch(opcao){
			case "ip":
				printf("opcao ip");
				break;
			case "ic":
				printf("opcao ic");
				break;
			case 'm':
				printf("opcao m");
				break;
			case 'x':
				printf("opcao x");
				break;
			case 'r':
				printf("opcao r");
				break;
			case 'c':
				printf("opcao c");
				break;
			default:
				printf("opcao invalida");
				break;
		}

	return 0;
}
