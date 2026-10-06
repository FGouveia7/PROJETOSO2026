#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "validacao_bv.h"
#include "../leitura/leitura_bv.h"
#include "../../shared/log.h"

int BV_ValidarNomeEmpresa(const char *nomeEmpresa)
{
    if (nomeEmpresa == NULL) {
        return BV_ERRO_NOME;
    }

    if (!BV_NomeEmpresaValido(nomeEmpresa)) {
        return BV_ERRO_NOME;
    }

    return BV_OK;
}

int BV_ValidarValoresEmpresa(const Empresa *pEmpresa) 
{
    bool existeAlgumValorErrado = (pEmpresa == NULL || pEmpresa->acoes_por_colocar < 0 || pEmpresa->valor_nominal <= 0.0 ||  pEmpresa->cotacao <= 0.0);
        
    if (existeAlgumValorErrado) {
        return BV_ERRO_VALORES;
    }

    return BV_OK;
}

int BV_ValidarDuplicado(const Empresa tabelaEmpresas[], int numEmpresas,const char *nomeEmpresa)
{   
    bool jaExisteEmpresa = (BV_ProcurarEmpresa(tabelaEmpresas, numEmpresas, nomeEmpresa) != NULL);

    if (jaExisteEmpresa) {
        return BV_ERRO_DUPLICADO;
    }

    return BV_OK;
}

static bool LinhaEmBranco(const char *linhaDoFicheiro)
{
    const char *pCaraterAtual = linhaDoFicheiro;

    while (*pCaraterAtual != '\0') {
    
        //cast para unsigned char pois a funcao isspace() tem comportamento indefinido com valores negativos (caracteres com acentuo) 
        if (!isspace((unsigned char)*pCaraterAtual)) {
            return false;
        }
        pCaraterAtual++;   //avanca para o proximo carater incrementando o pointer para o proximo local de memoria
    }
    return true;
}
int BV_ValidarLinha(const char *linhaDoFicheiro, int numLinha, Empresa *pEmpresa, const Empresa tabelaEmpresas[], int numEmpresas)
{
    int codigoErro = BV_OK; // inicializa so por seguranca

    if (linhaDoFicheiro == NULL || pEmpresa == NULL) {
        return BV_ERRO_FICHEIRO;
    }

    if (LinhaEmBranco(linhaDoFicheiro)) {
        return BV_LINHA_EM_BRANCO;
    }

    codigoErro = BV_ParseLinha(linhaDoFicheiro, pEmpresa);
    if (codigoErro == BV_OK) {
        codigoErro = BV_ValidarNomeEmpresa(pEmpresa->nome);
    }
    if (codigoErro == BV_OK) {
        codigoErro = BV_ValidarValoresEmpresa(pEmpresa);
    }
    if (codigoErro == BV_OK) {
        codigoErro = BV_ValidarDuplicado(tabelaEmpresas, numEmpresas, pEmpresa->nome);
    }

    if (codigoErro != BV_OK) {
        log_evento(LOG_ERRO, "Linha %d: %s", numLinha, BV_ErroParaTexto(codigoErro));
    }
    return codigoErro;
}

int BV_ValidarFicheiro(const char *caminhoFicheiro)
{
    Empresa tabelaTemporaria[MAX_EMPRESAS];
    int numEmpresasTemporarias = 0;
    int resultado;

    if (caminhoFicheiro == NULL) {
        return BV_ERRO_FICHEIRO;
    }

    resultado = BV_CarregarTabelaDeEmpresas(caminhoFicheiro, tabelaTemporaria, &numEmpresasTemporarias);

    if (resultado == BV_OK) {
        log_evento(LOG_INFO, "Verificacao: '%s' e valido (%d empresa(s))",caminhoFicheiro, numEmpresasTemporarias);
    } else {
        log_evento(LOG_ERRO, "Verificacao: '%s' e invalido (%s)",caminhoFicheiro, BV_ErroParaTexto(resultado));
    }

    return resultado;
}