#include <stdio.h>
#include <stdlib.h>

#include "manipulador_arquivos.h"

int arquivo_para_buffer(FILE *arq, unsigned char **buffer){

   unsigned int tam_arq;

   if(arq == NULL)
       return -1;

   fseek(arq, 0, SEEK_END);
   tam_arq = ftell(arq);
   fseek(arq, 0, SEEK_SET);

   if(tam_arq == 0)
       return -1;

   *buffer = (unsigned char *)malloc(tam_arq);

   if(*buffer == NULL){
       return -1;
   }

   if((fread(*buffer, 1, tam_arq, arq)) != tam_arq){
       free(*buffer);
       *buffer = NULL;
       return -1;
   }

   return tam_arq;
}

int buffer_para_arquivo(unsigned char *buffer, unsigned int tam_arq, FILE *arq){

    if(buffer == NULL || arq == NULL)
        return -1;

    if(tam_arq == 0)
        return -1;

    if((fwrite(buffer, 1, tam_arq, arq)) != tam_arq)
        return -1;

    return 0;
}
