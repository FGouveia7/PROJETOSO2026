#include <stdio.h>
#include "validacao/validacao_bv.h"
#include "leitura/leitura_bv.h"
#include "escrita/escrita_bv.h"
#include "../shared/log.h"

int main(void)
{
    Empresa tabelaEmpresas[MAX_EMPRESAS];
    int numEmpresas = 0;
    int resultado;

    log_init(LOG_SERVIDOR);

    resultado = BV_CarregarTabelaDeEmpresas("config/" BVDATA_FICHEIRO,
                                            tabelaEmpresas, &numEmpresas);
    printf("%s -> %s\n", BVDATA_FICHEIRO, BV_ErroParaTexto(resultado));
    BV_MostrarTabela(tabelaEmpresas, numEmpresas);

    const char *invalidos[] = {
        "config/invalido_campos_a_menos.txt",
        "config/invalido_campos_a_mais.txt",
        "config/invalido_nome_especial.txt",
        "config/invalido_quantidade_texto.txt",
        "config/invalido_quantidade_negativa.txt",
        "config/invalido_nominal_zero.txt",
        "config/invalido_duplicado.txt",
        "config/invalido_vazio.txt"
    };
    int totalInvalidos = sizeof(invalidos) / sizeof(invalidos[0]);

    for (int i = 0; i < totalInvalidos; i++) {
        resultado = BV_ValidarFicheiro(invalidos[i]);
        printf("%-42s -> %s\n", invalidos[i], BV_ErroParaTexto(resultado));
    }

    BV_AtualizarCotacao(tabelaEmpresas, numEmpresas, "Frisa", 7.50);
    resultado = BV_GuardarTabelaDeEmpresas("config/saida/teste_saida.txt",
                                           tabelaEmpresas, numEmpresas);
    printf("guardar -> %s\n", BV_ErroParaTexto(resultado));

    log_close();
    return 0;
}