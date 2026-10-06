#ifndef LEITURA_BV_H
#define LEITURA_BV_H

#include "../codErros_bv.h"

/* 1. Leitura */
void BV_LimparTabela(Empresa tabelaEmpresas[], int *pNumEmpresas);
int  BV_ParseLinha(const char *linhaTexto, Empresa *pEmpresa);
int  BV_CarregarTabelaDeEmpresas(const char *caminhoFicheiro, Empresa tabelaEmpresas[], int *pNumEmpresas);
const Empresa *BV_ProcurarEmpresa(const Empresa tabelaEmpresas[], int numEmpresas,const char *nomeEmpresa);
void BV_MostrarTabela(const Empresa tabelaEmpresas[], int numEmpresas);

#endif /* LEITURA_BV_H */