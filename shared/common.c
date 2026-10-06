#include "common.h"
#include <string.h>
#include <ctype.h>

int BV_NomeEmpresaValido(const char *nome){
    if (nome == NULL) return 0;

    size_t len = strlen(nome);
    if (len == 0 || len > MAX_NOME_EMPRESA) return 0;
    for(size_t i=0;i<len;i++){
        if(!isalnum((unsigned char)nome[i])) return 0;
    }
    return 1;
}