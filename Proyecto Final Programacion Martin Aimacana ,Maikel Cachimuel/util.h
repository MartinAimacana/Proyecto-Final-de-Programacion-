#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

void limpiar_salto(char *s);
void trim(char *s);

int  leer_linea(const char *msg, char *buf, size_t tam);
int  leer_int(const char *msg, int *out, int min, int max);
int  leer_double(const char *msg, double *out, double min, double max);

int  contiene_subcadena_ci(const char *texto, const char *sub);

#endif
