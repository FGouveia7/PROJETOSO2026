#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>
#include "log.h"

static FILE *g_log = NULL;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static const char *sNomesNivel[]={"INFO","AVISO","ERRO"};

/* ----------------------------------------------------------------------------
 * log_init: abre (ou cria) o ficheiro de log com o nome recebido.
 * Usa o modo "a" (append): acrescenta ao fim, sem apagar o log anterior.
 * Devolve 0 se correu bem, -1 se não conseguiu abrir.
 * Se falhar, o programa NÃO termina: log_evento passa a escrever no stderr.
 * ---------------------------------------------------------------------------- */
int log_init(const char *ficheiro){
    pthread_mutex_lock(&g_mutex);
    if(g_log) fclose(g_log);
    g_log = fopen(ficheiro,"a");
    pthread_mutex_unlock(&g_mutex);
    if (!g_log){
        fprintf(stderr, "Erro: Nao foi possivel abrir o log '%s'\n", ficheiro);
        return -1;
    }
    return 0;

}

/* ----------------------------------------------------------------------------
 * log_evento: escreve uma linha no log com o formato:
 *       [AAAA-MM-DD HH:MM:SS] [NIVEL] mensagem
 * Funciona como o printf: log_evento(LOG_ERRO, "Linha %d invalida", n);
 * Pode ser chamada por várias threads em simultâneo.
 * ---------------------------------------------------------------------------- */
void log_evento(NivelLog nivel, const char *fmt, ...){
    char ts[32];
    time_t agora = time(NULL);
    struct tm tm_info;
    localtime_r(&agora, &tm_info);
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm_info);

    pthread_mutex_lock(&g_mutex);
    FILE *out = g_log ? g_log : stderr;
    fprintf(out, "[%s] [%s] ", ts, sNomesNivel[nivel]);
    va_list args;
    va_start(args,fmt);
    vfprintf(out,fmt,args);
    va_end(args);
    fputc('\n', out);
    fflush(out);
    pthread_mutex_unlock(&g_mutex);
}

/* ----------------------------------------------------------------------------
 * log_close: fecha o ficheiro de log. Chamar ao terminar o programa.
 * Depois disto, log_evento volta a escrever no stderr.
 * ---------------------------------------------------------------------------- */
void log_close(void){
    pthread_mutex_lock(&g_mutex);
    if (g_log){
        fclose(g_log);
        g_log = NULL;
    }
    pthread_mutex_unlock(&g_mutex);
}
