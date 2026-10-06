#include <stdio.h>
#include <string.h>
#include "escrita_bv.h"
#define MAX_CAMINHO 512
#include "../../shared/log.h"

int BV_EmpresaParaTexto(const Empresa *pEmpresa, char *bufferSaida, size_t tamanhoBuffer)
{
    if (pEmpresa == NULL || bufferSaida == NULL || tamanhoBuffer == 0) {
        return BV_ERRO_FICHEIRO;
    }

    int caracteresEscritos = snprintf(bufferSaida, tamanhoBuffer, "%s %ld %.2f %.2f",
                                      pEmpresa->nome,
                                      pEmpresa->acoes_por_colocar,
                                      pEmpresa->valor_nominal,
                                      pEmpresa->cotacao);

    bool excedeBuffer = ((size_t)caracteresEscritos >= tamanhoBuffer);

    if (caracteresEscritos < 0 || excedeBuffer) {
        return BV_ERRO_ESCRITA;      
    }
    return BV_OK;
}

int BV_AtualizarCotacao(Empresa tabelaEmpresas[], int numEmpresas,const char *nomeEmpresa, double novaCotacao)
{
    if (tabelaEmpresas == NULL || nomeEmpresa == NULL) {
        return BV_ERRO_FICHEIRO;
    }

    if (novaCotacao <= 0.0) {
        log_evento(LOG_ERRO, "Cotacao invalida (%.2f) para '%s'", novaCotacao, nomeEmpresa);
        return BV_ERRO_VALORES;
    }

    for (int i = 0; i < numEmpresas; i++) {

        bool nomesIguais = (strcmp(tabelaEmpresas[i].nome, nomeEmpresa) == 0);

        if (nomesIguais) {
            log_evento(LOG_INFO, "Cotacao de '%s': %.2f -> %.2f",nomeEmpresa, tabelaEmpresas[i].cotacao, novaCotacao);
            tabelaEmpresas[i].cotacao = novaCotacao;
            return BV_OK;
        }
    }

    log_evento(LOG_ERRO, "Empresa '%s' nao encontrada", nomeEmpresa);
    return BV_ERRO_NOME;
}

int BV_GuardarTabelaDeEmpresas(const char *caminhoFicheiro, const Empresa tabelaEmpresas[],int numEmpresas)
{
    char caminhoTemporario[MAX_CAMINHO];//criamos um buffer para um caminho temporario para nao perder o original

    if (caminhoFicheiro == NULL || tabelaEmpresas == NULL || numEmpresas < 0) {
        return BV_ERRO_FICHEIRO;
    }

    snprintf(caminhoTemporario, sizeof(caminhoTemporario), "%s.tmp", caminhoFicheiro);

    FILE *pFicheiro = fopen(caminhoTemporario, "w");
    if (pFicheiro == NULL) {
        log_evento(LOG_ERRO, "Nao foi possivel criar '%s'", caminhoTemporario);
        return BV_ERRO_ESCRITA;
    }

    for (int i = 0; i < numEmpresas; i++) {
        fprintf(pFicheiro, "%s %ld %.2f %.2f\n",
                tabelaEmpresas[i].nome,
                tabelaEmpresas[i].acoes_por_colocar,
                tabelaEmpresas[i].valor_nominal,
                tabelaEmpresas[i].cotacao);
    }

    if (fclose(pFicheiro) != 0 || rename(caminhoTemporario, caminhoFicheiro) != 0) {
        remove(caminhoTemporario);
        log_evento(LOG_ERRO, "Falha ao guardar '%s'", caminhoFicheiro);
        return BV_ERRO_ESCRITA;
    }

    log_evento(LOG_INFO, "'%s' guardado: %d empresa(s)", caminhoFicheiro, numEmpresas);
    return BV_OK;
}