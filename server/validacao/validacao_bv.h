#ifndef VALIDACAO_BV_H
#define VALIDACAO_BV_H

#include "../codErros_bv.h"

/* 2. Validacao */
int  BV_ValidarNomeEmpresa(const char *nomeEmpresa);
int  BV_ValidarValoresEmpresa(const Empresa *pEmpresa);
int  BV_ValidarDuplicado(const Empresa tabelaEmpresas[], int numEmpresas,
                         const char *nomeEmpresa);
int  BV_ValidarLinha(const char *linhaTexto, int numLinha, Empresa *pEmpresa,
                     const Empresa tabelaEmpresas[], int numEmpresas);
int  BV_ValidarFicheiro(const char *caminhoFicheiro);

#endif /* VALIDACAO_BV_H */