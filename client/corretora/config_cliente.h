#ifndef CONFIG_CLIENTE_H
#define CONFIG_CLIENTE_H

#include "common.h"

#define MAX_CAMINHO_IPC 128
#define MAX_IP_BV 45
#define CL_CONFIG_POR_OMISSAO "cliente.conf"

#define CL_PORTA_AUSENTE (-1)
#define CL_PORTA_NAO_NUMERICA (-2)

#define CL_OK 0
#define CL_ERRO_ARGS -1
#define CL_ERRO_FICHEIRO -2
#define CL_ERRO_LINHA -3
#define CL_ERRO_FALTA_PARAMETRO -4

typedef struct {
	char idCorretora[MAX_ID_CORRETORA + 1];
	char idInvestidor[MAX_ID_INVESTIDOR + 1];
	char ipBv[MAX_IP_BV + 1];
	long portaBv;
	char ipc[MAX_CAMINHO_IPC + 1];
} ConfigCliente;

void CL_InicializarConfig(ConfigCliente *pCfg);
int CL_CarregarConfig(const char *pCaminho, ConfigCliente *pCfg);
void CL_AplicarOmissoes(ConfigCliente *pCfg);

int CL_IniciarCorretora(int argc, char *argv[],ConfigCliente *pCfg);
int CL_IniciarInvestidor(int argc, char *argv[],ConfigCliente *pCfg);

#endif

