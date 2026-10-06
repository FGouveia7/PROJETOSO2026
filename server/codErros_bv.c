#include "codErros_bv.h"

const char *BV_ErroParaTexto(int codigoErro)
{
    switch (codigoErro) {
        case BV_OK:                   return "Executado com sucesso";
        case BV_LINHA_EM_BRANCO:      return "linha em branco";
        case BV_ERRO_FICHEIRO:        return "ficheiro inexistente, ilegivel ou argumento invalido";
        case BV_ERRO_CAMPOS:          return "numero de campos incorreto (esperados 4)";
        case BV_ERRO_NUMERO:          return "campo numerico invalido";
        case BV_ERRO_NOME:            return "nome da empresa invalido";
        case BV_ERRO_VALORES:         return "valores fora do intervalo permitido (acoes >= 0, nominal > 0 e cotacao > 0)";
        case BV_ERRO_DUPLICADO:       return "empresa duplicada";
        case BV_ERRO_FICHEIRO_VAZIO:  return "ficheiro sem empresas";
        case BV_ERRO_ESCRITA:         return "erro ao escrever o ficheiro";
        default:                      return "erro desconhecido";
    }
}