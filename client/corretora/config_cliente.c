#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "config_cliente.h"
#include "log.h"

static char *CL_RemoverEspacos(char *pTexto){
	while (isspace((unsigned char)*pTexto)) pTexto++;
	char *pFim = pTexto + strlen(pTexto);
	while (pFim > pTexto && isspace((unsigned char)pFim[-1])) *--pFim = '\0';
	return pTexto;
}

static int CL_CopiarCampo(char *pDestino, size_t tamanho, const char *pValor, const char *pChave, int numLinha){
	if (strlen(pValor) >= tamanho){
		log_evento(LOG_ERRO, "Linha %d: '%s' demasiado grande (max %zu)",
			numLinha, pChave, tamanho - 1);
		return -1;
	}
 snprintf(pDestino, tamanho, "%s", pValor);
	return 0;
}

void CL_InicializarConfig(ConfigCliente *pCfg){
	memset(pCfg, 0 , sizeof(*pCfg));
	pCfg->portaBv = CL_PORTA_AUSENTE;
}

int CL_CarregarConfig(const char *pCaminho, ConfigCliente *pCfg){
	FILE *pFicheiro = fopen(pCaminho, "r");
	if (!pFicheiro){
		log_evento(LOG_ERRO, "Config '%s' inexistente ou ilegivel: %s",pCaminho, strerror(errno));
		return CL_ERRO_FICHEIRO;
	}
	
	char linha[MAX_LINHA];
	int numLinha = 0;

	while (fgets(linha, sizeof(linha), pFicheiro)){
		numLinha++;
		if (!strchr(linha, '\n') && !feof(pFicheiro)){
			log_evento(LOG_ERRO, "Linha %d demasiado longa", numLinha);
			fclose(pFicheiro);
			return CL_ERRO_LINHA;
		}

		char *pLinha = CL_RemoverEspacos(linha);
		if (*pLinha == '\0' || *pLinha == '#') continue;

		char *pIgual = strchr(pLinha, '=');
		if  (!pIgual){
			log_evento(LOG_ERRO, "Linha %d sem '=': '%s'", numLinha, pLinha);
			fclose(pFicheiro);
			return CL_ERRO_LINHA;
		}

		*pIgual = '\0';
		char *pChave = CL_RemoverEspacos(pLinha);
		char *pValor = CL_RemoverEspacos(pIgual + 1);
		int temErro = 0;

		if (strcmp(pChave, "id_corretora") == 0)
			temErro = CL_CopiarCampo(pCfg->idCorretora, sizeof(pCfg->idCorretora),pValor,pChave, numLinha);

		else if (strcmp(pChave, "ip_bv") == 0)
			temErro = CL_CopiarCampo(pCfg->ipBv, sizeof(pCfg->ipBv),pValor, pChave, numLinha);
		else if (strcmp(pChave, "ipc") == 0)
			temErro = CL_CopiarCampo(pCfg->ipc, sizeof(pCfg->ipc),pValor,pChave, numLinha);
		else if (strcmp(pChave, "porta_bv") == 0){
			char *pFimNumero;
			errno = 0;
			long porta = strtol(pValor, &pFimNumero, 10);
			if (*pValor == '\0' || *pFimNumero != '\0' || errno != 0)
				pCfg->portaBv = CL_PORTA_NAO_NUMERICA;
			else
				pCfg->portaBv = porta;
		}
		
		else
			log_evento(LOG_AVISO, "Linha %d: chave desconhecida '%s'", numLinha, pChave);
		
		if (temErro){
			fclose(pFicheiro);
			return CL_ERRO_LINHA;
		}
	}

	fclose(pFicheiro);
	log_evento(LOG_INFO, "Config '%s' lida", pCaminho);
	return CL_OK;
}

void CL_AplicarOmissoes(ConfigCliente *pCfg){
	if (pCfg->ipc[0] == '\0'){
		snprintf(pCfg->ipc, sizeof(pCfg->ipc), "/tmp/corretora_%s.fifo",pCfg->idCorretora);
		log_evento(LOG_INFO, "IPC por omissao: %s", pCfg->ipc);
	}
}
	
