#ifndef LOG_H
#define LOG_H

#include "common.h"

int log_init(const char *ficheiro);
void log_evento(NivelLog nivel, const char *fmt, ...);
void log_close(void);

#endif