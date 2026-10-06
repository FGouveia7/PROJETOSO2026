#ifndef BV_CODIGOS_H
#define BV_CODIGOS_H

#include <stdbool.h>
#include <stddef.h>
#include "../shared/common.h"

//Codigos de erro 
#define BV_OK                       1
#define BV_LINHA_EM_BRANCO          0
#define BV_ERRO_FICHEIRO           -1
#define BV_ERRO_CAMPOS             -2
#define BV_ERRO_NUMERO             -3
#define BV_ERRO_NOME               -4
#define BV_ERRO_VALORES            -5
#define BV_ERRO_DUPLICADO          -6
#define BV_ERRO_FICHEIRO_VAZIO     -7
#define BV_ERRO_ESCRITA            -8

const char *BV_ErroParaTexto(int codigoErro);

#endif 