#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../validacao/validacao_bv.h"
#include "../../shared/log.h"
#include <string.h>
#include "leitura_bv.h"

void BV_LimparTabela(Empresa tabelaEmpresas[], int *pNumEmpresas)
{   
    if (tabelaEmpresas == NULL || pNumEmpresas == NULL) {
        return;
    }
    size_t numBytesEmpresas = MAX_EMPRESAS * sizeof(Empresa);//calculo de bytes que a tabela tem de ter 
    memset(tabelaEmpresas, 0, numBytesEmpresas);//preenche a memoria com 0's para a tabela
    *pNumEmpresas = 0;
}

int BV_ParseLinha(const char *linhaDoFicheiro, Empresa *pEmpresa)
{
    char nomeEmpresasDoTexto[MAX_NOME_EMPRESA + 2];
    char acoesDoTexto[32];
    char nominaisDoTexto[32];
    char cotacoesDoTexto[32];
    char campoExtra[32];
    char *pCaraterParagem;

    if(linhaDoFicheiro == NULL || pEmpresa == NULL){
        return BV_ERRO_FICHEIRO;
    }

    int camposLidos = sscanf(linhaDoFicheiro, "%33s %31s %31s %31s %31s",
                         nomeEmpresasDoTexto, acoesDoTexto,
                         nominaisDoTexto, cotacoesDoTexto, campoExtra);

    if (camposLidos != 4){
        return BV_ERRO_CAMPOS;
    }

    if (strlen(nomeEmpresasDoTexto) > MAX_NOME_EMPRESA) {
        return BV_ERRO_NOME;
    }

    errno = 0;
    long acoesPorColocar = strtol(acoesDoTexto, &pCaraterParagem, 10);
    if (errno != 0 || *pCaraterParagem != '\0') {
        return BV_ERRO_NUMERO;
    }

    errno = 0;
    double valorNominal = strtod(nominaisDoTexto, &pCaraterParagem);
    if (errno != 0 || *pCaraterParagem != '\0') {
        return BV_ERRO_NUMERO;
    }

    errno = 0;
    double cotacao = strtod(cotacoesDoTexto, &pCaraterParagem);
    if (errno != 0 || *pCaraterParagem != '\0') {
        return BV_ERRO_NUMERO;
    }

    strncpy(pEmpresa->nome, nomeEmpresasDoTexto, MAX_NOME_EMPRESA);//copia o nome obtido para o campo da Empresa 
    
    //preenche a struct da empresa e adiciona o null terminator no nome da empresa
    pEmpresa->nome[MAX_NOME_EMPRESA] = '\0';
    pEmpresa->acoes_por_colocar = acoesPorColocar;
    pEmpresa->valor_nominal = valorNominal;
    pEmpresa->cotacao = cotacao;
    
    return BV_OK;
}

int BV_CarregarTabelaDeEmpresas(const char *caminhoFicheiro, Empresa tabelaEmpresas[], int *pNumEmpresas)
{
    FILE *pFicheiro;
    char bufferLinhasDeTexto[MAX_LINHA];
    int contaLinhas = 0;
    int contaLinhasInvalidas = 0;
    int erroDaLinhaInvalida = BV_LINHA_EM_BRANCO;
    int primeiroErro = BV_OK;
    Empresa novaEmpresa;
    memset(&novaEmpresa, 0, sizeof(novaEmpresa)); // inicializacao do objeto Empresa so para nao haver bugs
    bool argumentosInvalidos = (caminhoFicheiro == NULL || tabelaEmpresas == NULL|| pNumEmpresas == NULL);

    if(argumentosInvalidos){
        return BV_ERRO_FICHEIRO;
    }

    BV_LimparTabela(tabelaEmpresas, pNumEmpresas);

    //Acede a localizacao do ficheiro no modo de leitura "r"
    pFicheiro = fopen(caminhoFicheiro, "r");

    //Se nao existir o ficheiro
    if (pFicheiro == NULL) {
        log_evento(LOG_ERRO, "Nao foi possivel abrir '%s'", caminhoFicheiro);
        return BV_ERRO_FICHEIRO;
    }


    while (fgets(bufferLinhasDeTexto, MAX_LINHA, pFicheiro) != NULL) {
        contaLinhas++;
        erroDaLinhaInvalida = BV_ValidarLinha(bufferLinhasDeTexto, contaLinhas, &novaEmpresa,tabelaEmpresas, *pNumEmpresas);

        if (erroDaLinhaInvalida == BV_LINHA_EM_BRANCO) {
            continue;                      
        }

        if (erroDaLinhaInvalida != BV_OK) {
            contaLinhasInvalidas++;               //erro: conta e segue para a proxima
            if (primeiroErro == BV_OK) {
                primeiroErro = erroDaLinhaInvalida;
            }
            continue;
        }

        if (*pNumEmpresas >= MAX_EMPRESAS) {
            log_evento(LOG_ERRO, "Linha %d: excede o maximo de %d empresas",contaLinhas, MAX_EMPRESAS);
            contaLinhasInvalidas++;
            continue;
        }
        tabelaEmpresas[*pNumEmpresas] = novaEmpresa; //Adiciona a nova empresa a tabela
        (*pNumEmpresas)++; // Incrementa o numero de empresas na tabela
    }

    fclose(pFicheiro);

    if (contaLinhasInvalidas > 0) {
        log_evento(LOG_ERRO, "'%s' rejeitado: %d linha(s) invalida(s)",caminhoFicheiro, contaLinhasInvalidas);
        BV_LimparTabela(tabelaEmpresas, pNumEmpresas);   //limpa a tabela 
        return primeiroErro;
    }

    if (*pNumEmpresas == 0) {
        log_evento(LOG_ERRO, "'%s' nao tem empresas", caminhoFicheiro);
        return BV_ERRO_FICHEIRO_VAZIO;
    }

    log_evento(LOG_INFO, "'%s' carregado: %d empresa(s)", caminhoFicheiro, *pNumEmpresas);
    return BV_OK;
}

const Empresa *BV_ProcurarEmpresa(const Empresa tabelaEmpresas[], int numEmpresas,const char *nomeEmpresa)
{
    if (tabelaEmpresas == NULL || nomeEmpresa == NULL) {
        return NULL;
    }

    for (int i = 0; i < numEmpresas; i++) {

        bool nomesIguais = (strcmp(tabelaEmpresas[i].nome, nomeEmpresa) == 0);

        if (nomesIguais) {
            return &tabelaEmpresas[i];
        }
    }

    return NULL;
}

void BV_MostrarTabela(const Empresa tabelaEmpresas[], int numEmpresas)
{
    if (tabelaEmpresas == NULL) {
        printf("Tabela invalida (NULL)\n");
        return;
    }

    if (numEmpresas <= 0) {
        printf("Tabela vazia\n");
        return;
    }

    printf("\n%-*s %12s %10s %10s\n",
           MAX_NOME_EMPRESA, "EMPRESA", "POR COLOCAR", "NOMINAL", "COTACAO");

    for (int i = 0; i < numEmpresas; i++) {
        printf("%-*s %12ld %10.2f %10.2f\n",
               MAX_NOME_EMPRESA,
               tabelaEmpresas[i].nome,
               tabelaEmpresas[i].acoes_por_colocar,
               tabelaEmpresas[i].valor_nominal,
               tabelaEmpresas[i].cotacao);
    }

    printf("\nTotal: %d empresa(s)\n", numEmpresas);
}