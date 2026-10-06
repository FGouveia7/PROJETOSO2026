#ifndef ESCRITA_BV_H
#define ESCRITA_BV_H

#include "../codErros_bv.h"

/* 3. Escrita */

int BV_EmpresaParaTexto(const Empresa *pEmpresa, char *bufferSaida, size_t tamanhoBuffer);
int BV_AtualizarCotacao(Empresa tabelaEmpresas[], int numEmpresas,const char *nomeEmpresa, double novaCotacao);
int BV_GuardarTabelaDeEmpresas(const char *caminhoFicheiro, const Empresa tabelaEmpresas[],int numEmpresas);

#endif /* ESCRITA_BV_H */