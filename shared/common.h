#ifndef COMMON_H
#define COMMON_H

#define BVDATA_FICHEIRO "bvdata.txt"
#define LOG_SERVIDOR "bv.log"
#define LOG_CORRETORA "corretora.log"

#define MAX_NOME_EMPRESA 32
#define MAX_ID_CORRETORA 32
#define MAX_ID_INVESTIDOR 32
#define MAX_LINHA 256
#define MAX_EMPRESAS 64
#define MAX_CORRETORAS 5

typedef enum { ORDEM_COMPRA, ORDEM_VENDA } TipoOrdem;
typedef enum { LOG_INFO, LOG_AVISO, LOG_ERRO } NivelLog;

typedef struct {
    char nome[ MAX_NOME_EMPRESA +1 ];
    long acoes_por_colocar;
    double valor_nominal;
    double cotacao;
} Empresa;


//3.1.3 ORdem de CV
typedef struct {
    TipoOrdem tipo;
    char corretora [MAX_ID_CORRETORA +1];
    char investidor[MAX_ID_INVESTIDOR +1];
    char empresa[MAX_NOME_EMPRESA +1];
    long quantidade;
    double preco;
} OrdemCV;

//3.1.3 NOTIFICACOES de ordem de C/V
typedef struct{
    char corretora[MAX_ID_CORRETORA +1];
    char investidor[MAX_ID_INVESTIDOR +1];
    char empresa[MAX_NOME_EMPRESA +1];
    long quantidade;
    double preco_final;
} NotificacaoCV;

// Valida nome de empresa
int BV_NomeEmpresaValido(const char *nome);


#endif