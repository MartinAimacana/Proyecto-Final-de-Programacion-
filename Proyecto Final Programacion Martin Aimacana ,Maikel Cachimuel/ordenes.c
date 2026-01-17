#include "ordenes.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

double costo_total(const Orden *o, double tarifa_hora) {
    if (!o) return 0.0;
    return o->costo_base + (double)o->horas_trabajo * tarifa_hora;
}

int es_codigo_valido(const char *codigo) {
    if (!codigo) return 0;
    size_t len = strlen(codigo);
    if (len < 1 || len > 15) return 0;

    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)codigo[i])) return 0;
    }
    return 1;
}

int codigo_unico(const Orden *arr, int n, const char *codigo) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].codigo_orden, codigo) == 0) return 0;
    }
    return 1;
}

int buscar_por_codigo(const Orden *arr, int n, const char *codigo) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].codigo_orden, codigo) == 0) return i;
    }
    return -1;
}

static void imprimir_encabezado(void) {
    printf("\n%-15s | %-20s | %-12s | %-22s | %-10s | %-5s | %-10s\n",
           "codigo_orden", "nombre_cliente", "equipo", "tipo_trabajo",
           "costo_base", "hrs", "costo_total");
    printf("--------------------------------------------------------------------------------------------------------------\n");
}

static void imprimir_orden(const Orden *o, double tarifa_hora) {
    printf("%-15s | %-20s | %-12s | %-22s | %10.2f | %5d | %10.2f\n",
           o->codigo_orden, o->nombre_cliente, o->equipo, o->tipo_trabajo,
           o->costo_base, o->horas_trabajo, costo_total(o, tarifa_hora));
}

void listar_ordenes(const Orden *arr, int n, double tarifa_hora) {
    if (n == 0) {
        printf("\nNo hay ordenes registradas.\n");
        return;
    }
    imprimir_encabezado();
    for (int i = 0; i < n; i++) {
        imprimir_orden(&arr[i], tarifa_hora);
    }
}

void buscar_por_nombre(const Orden *arr, int n, const char *subcadena, double tarifa_hora) {
    int encontrados = 0;
    imprimir_encabezado();
    for (int i = 0; i < n; i++) {
        if (contiene_subcadena_ci(arr[i].nombre_cliente, subcadena)) {
            imprimir_orden(&arr[i], tarifa_hora);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("(Sin resultados)\n");
    }
}

int registrar_orden(Orden *arr, int *n, double tarifa_hora) {
    if (!arr || !n) return 0;
    if (*n >= MAX_ORDENES) {
        printf("No se pueden registrar mas ordenes (limite %d).\n", MAX_ORDENES);
        return 0;
    }

    Orden nueva;
    char buf[128];

    while (1) {
        if (!leer_linea("Codigo de orden (1-15 alfanumerico, sin espacios): ", buf, sizeof(buf))) return 0;
        if (!es_codigo_valido(buf)) {
            printf("  -> Codigo invalido.\n");
            continue;
        }
        if (!codigo_unico(arr, *n, buf)) {
            printf("  -> Ese codigo ya existe. Debe ser unico.\n");
            continue;
        }
        strncpy(nueva.codigo_orden, buf, MAX_COD);
        nueva.codigo_orden[MAX_COD - 1] = '\0';
        break;
    }

    leer_linea("Nombre del cliente: ", nueva.nombre_cliente, sizeof(nueva.nombre_cliente));
    leer_linea("Equipo (ej: Laptop/PC/Impresora): ", nueva.equipo, sizeof(nueva.equipo));
    leer_linea("Tipo de trabajo (ej: Preventivo/Correctivo/Instalacion): ", nueva.tipo_trabajo, sizeof(nueva.tipo_trabajo));

    leer_double("Costo base (>0): ", &nueva.costo_base, 0.01, 1000000.0);
    leer_int("Horas de trabajo (>=0): ", &nueva.horas_trabajo, 0, 100000);

    arr[*n] = nueva;
    (*n)++;

    printf("Orden registrada. Costo total (tarifa %.2f/h) = %.2f\n", tarifa_hora, costo_total(&nueva, tarifa_hora));
    return 1;
}

int actualizar_orden(Orden *arr, int n, double tarifa_hora) {
    if (n == 0) {
        printf("No hay ordenes para actualizar.\n");
        return 0;
    }

    char cod[MAX_COD];
    leer_linea("Ingrese el codigo_orden a actualizar: ", cod, sizeof(cod));

    int idx = buscar_por_codigo(arr, n, cod);
    if (idx < 0) {
        printf("No existe una orden con ese codigo.\n");
        return 0;
    }

    Orden *o = &arr[idx];
    printf("\nOrden encontrada:\n");
    imprimir_encabezado();
    imprimir_orden(o, tarifa_hora);

    printf("\nQue deseas modificar?\n");
    printf("1) nombre_cliente\n");
    printf("2) equipo\n");
    printf("3) tipo_trabajo\n");
    printf("4) costo_base\n");
    printf("5) horas_trabajo\n");
    printf("6) volver\n");

    int op = 0;
    leer_int("Opcion: ", &op, 1, 6);

    char buf[128];
    switch (op) {
        case 1:
            leer_linea("Nuevo nombre_cliente: ", buf, sizeof(buf));
            strncpy(o->nombre_cliente, buf, sizeof(o->nombre_cliente));
            o->nombre_cliente[sizeof(o->nombre_cliente) - 1] = '\0';
            break;
        case 2:
            leer_linea("Nuevo equipo: ", buf, sizeof(buf));
            strncpy(o->equipo, buf, sizeof(o->equipo));
            o->equipo[sizeof(o->equipo) - 1] = '\0';
            break;
        case 3:
            leer_linea("Nuevo tipo_trabajo: ", buf, sizeof(buf));
            strncpy(o->tipo_trabajo, buf, sizeof(o->tipo_trabajo));
            o->tipo_trabajo[sizeof(o->tipo_trabajo) - 1] = '\0';
            break;
        case 4: {
            double cb = 0;
            leer_double("Nuevo costo_base (>0): ", &cb, 0.01, 1000000.0);
            o->costo_base = cb;
            break;
        }
        case 5: {
            int h = 0;
            leer_int("Nuevas horas_trabajo (>=0): ", &h, 0, 100000);
            o->horas_trabajo = h;
            break;
        }
        default:
            return 1;
    }

    printf("\nActualizacion lista. Nuevo costo total = %.2f\n", costo_total(o, tarifa_hora));
    return 1;
}

int eliminar_orden(Orden *arr, int *n) {
    if (!arr || !n || *n == 0) {
        printf("No hay ordenes para eliminar.\n");
        return 0;
    }

    char cod[MAX_COD];
    leer_linea("Ingrese el codigo_orden a eliminar: ", cod, sizeof(cod));
    int idx = buscar_por_codigo(arr, *n, cod);

    if (idx < 0) {
        printf("No existe una orden con ese codigo.\n");
        return 0;
    }

    char conf[8];
    leer_linea("Seguro que deseas eliminar? (s/n): ", conf, sizeof(conf));
    if (!(conf[0] == 's' || conf[0] == 'S')) {
        printf("Eliminacion cancelada.\n");
        return 0;
    }

    for (int i = idx; i < *n - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*n)--;

    printf("Orden eliminada.\n");
    return 1;
}

int cargar_ordenes_csv(const char *nombre_archivo, Orden *arr, int *n) {
    if (!nombre_archivo || !arr || !n) return 0;

    FILE *f = fopen(nombre_archivo, "r");
    if (!f) return 0;

    char linea[512];
    int count = 0;

    if (!fgets(linea, sizeof(linea), f)) {
        fclose(f);
        return 0;
    }

    while (fgets(linea, sizeof(linea), f)) {
        limpiar_salto(linea);
        trim(linea);
        if (linea[0] == '\0') continue;

        Orden o;
        char *token = strtok(linea, ",");
        if (!token) continue;
        strncpy(o.codigo_orden, token, MAX_COD);
        o.codigo_orden[MAX_COD - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(o.nombre_cliente, token, MAX_NOM);
        o.nombre_cliente[MAX_NOM - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(o.equipo, token, MAX_EQU);
        o.equipo[MAX_EQU - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(o.tipo_trabajo, token, MAX_TIPO);
        o.tipo_trabajo[MAX_TIPO - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        o.costo_base = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        o.horas_trabajo = atoi(token);

        if (!es_codigo_valido(o.codigo_orden)) continue;
        if (o.costo_base <= 0) continue;
        if (o.horas_trabajo < 0) continue;
        if (!codigo_unico(arr, count, o.codigo_orden)) continue;

        if (count < MAX_ORDENES) arr[count++] = o;
        else break;
    }

    fclose(f);
    *n = count;
    return 1;
}

int guardar_ordenes_csv(const char *nombre_archivo, const Orden *arr, int n) {
    if (!nombre_archivo || !arr) return 0;

    FILE *f = fopen(nombre_archivo, "w");
    if (!f) {
        printf("No se pudo abrir el archivo para guardar.\n");
        return 0;
    }

    fprintf(f, "codigo_orden,nombre_cliente,equipo,tipo_trabajo,costo_base,horas_trabajo\n");

    for (int i = 0; i < n; i++) {
        fprintf(f, "%s,%s,%s,%s,%.2f,%d\n",
                arr[i].codigo_orden,
                arr[i].nombre_cliente,
                arr[i].equipo,
                arr[i].tipo_trabajo,
                arr[i].costo_base,
                arr[i].horas_trabajo);
    }

    fclose(f);
    return 1;
}
