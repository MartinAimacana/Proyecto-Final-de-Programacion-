#ifndef ORDENES_H
#define ORDENES_H

#include <stddef.h>

#define MAX_ORDENES 1000
#define MAX_COD     16   // 1..15 + '\0'
#define MAX_NOM     60
#define MAX_EQU     40
#define MAX_TIPO    50

typedef struct {
    char  codigo_orden[MAX_COD];
    char  nombre_cliente[MAX_NOM];
    char  equipo[MAX_EQU];
    char  tipo_trabajo[MAX_TIPO];
    double costo_base;
    int   horas_trabajo;
} Orden;

int  cargar_ordenes_csv(const char *nombre_archivo, Orden *arr, int *n);
int  guardar_ordenes_csv(const char *nombre_archivo, const Orden *arr, int n);

int  registrar_orden(Orden *arr, int *n, double tarifa_hora);
void listar_ordenes(const Orden *arr, int n, double tarifa_hora);
int  buscar_por_codigo(const Orden *arr, int n, const char *codigo);
void buscar_por_nombre(const Orden *arr, int n, const char *subcadena, double tarifa_hora);
int  actualizar_orden(Orden *arr, int n, double tarifa_hora);
int  eliminar_orden(Orden *arr, int *n);

double costo_total(const Orden *o, double tarifa_hora);

int es_codigo_valido(const char *codigo);
int codigo_unico(const Orden *arr, int n, const char *codigo);

#endif
