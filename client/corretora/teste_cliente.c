#include <stdio.h>
#include "config_cliente.h"
#include "log.h"

int main(void){
	log_init("cliente_teste.log");

	ConfigCliente cfg;
	CL_InicializarConfig(&cfg);
	
	if (CL_CarregarConfig("config/cliente.conf", &cfg) != CL_OK){
		log_close();
		return 1;
	}
	CL_AplicarOmissoes(&cfg);

	printf("corretora: %s\n", cfg.idCorretora);
	printf("ip:       %s\n", cfg.ipBv);
	printf("porta:    %ld\n", cfg.portaBv);
	printf("ipc:      %s\n", cfg.ipc);

	log_close();
	return 0;
}
