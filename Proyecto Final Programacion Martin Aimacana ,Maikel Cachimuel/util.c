#include "util.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void limpiar_salto(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

void trim(char *s) {
    if (!s) return;

    size_t i = 0;
    while (s[i] && isspace((unsigned char)s[i])) i++;

    if (i > 0) memmove(s, s + i, strlen(s + i) + 1);

    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

int leer_linea(const char *msg, char *buf, size_t tam) {
    if (!buf || tam == 0) return 0;
    printf("%s", msg);

    if (!fgets(buf, (int)tam, stdin)) return 0;
    limpiar_salto(buf);
    trim(buf);
    return 1;
}

int leer_int(const char *msg, int *out, int min, int max) {
    char tmp[64];
    while (1) {
        if (!leer_linea(msg, tmp, sizeof(tmp))) return 0;
        char *end = NULL;
        long val = strtol(tmp, &end, 10);
        if (end == tmp || *end != '\0') {
            printf("  -> Entrada invalida. Intenta de nuevo.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("  -> Debe estar entre %d y %d.\n", min, max);
            continue;
        }
        *out = (int)val;
        return 1;
    }
}

int leer_double(const char *msg, double *out, double min, double max) {
    char tmp[64];
    while (1) {
        if (!leer_linea(msg, tmp, sizeof(tmp))) return 0;
        char *end = NULL;
        double val = strtod(tmp, &end);
        if (end == tmp || *end != '\0') {
            printf("  -> Entrada invalida. Intenta de nuevo.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("  -> Debe estar entre %.2f y %.2f.\n", min, max);
            continue;
        }
        *out = val;
        return 1;
    }
}

static char lower_char(char c) {
    return (char)tolower((unsigned char)c);
}

int contiene_subcadena_ci(const char *texto, const char *sub) {
    if (!texto || !sub) return 0;
    if (*sub == '\0') return 1;

    size_t n = strlen(texto);
    size_t m = strlen(sub);

    for (size_t i = 0; i + m <= n; i++) {
        size_t j = 0;
        while (j < m && lower_char(texto[i + j]) == lower_char(sub[j])) {
            j++;
        }
        if (j == m) return 1;
    }
    return 0;
}
