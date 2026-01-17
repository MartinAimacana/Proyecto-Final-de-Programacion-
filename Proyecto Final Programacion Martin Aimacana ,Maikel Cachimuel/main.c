#include <stdio.h>
#include "ordenes.h"
#include "util.h"

int main(void) {
    const double TARIFA_HORA = 10.0;
    const char *ARCHIVO = "ordenes.csv";

    Orden ordenes[MAX_ORDENES];
    int n = 0;

    cargar_ordenes_csv(ARCHIVO, ordenes, &n);

    int opcion = 0;
    do {
        printf("\n==============================\n");
        printf("  SISTEMA: ORDENES DE TRABAJO\n");
        printf("==============================\n");
        printf("Tarifa por hora: %.2f\n", TARIFA_HORA);
        printf("Archivo: %s | Registros en memoria: %d\n\n", ARCHIVO, n);

        printf("1) Registrar orden\n");
        printf("2) Listar ordenes\n");
        printf("3) Buscar orden (por codigo o por nombre)\n");
        printf("4) Actualizar orden\n");
        printf("5) Eliminar orden\n");
        printf("6) Guardar cambios en archivo\n");
        printf("7) Salir (guarda automatico)\n");

        leer_int("Opcion: ", &opcion, 1, 7);

        if (opcion == 1) {
            registrar_orden(ordenes, &n, TARIFA_HORA);
        } else if (opcion == 2) {
            listar_ordenes(ordenes, n, TARIFA_HORA);
        } else if (opcion == 3) {
            printf("\nBuscar por:\n");
            printf("1) codigo_orden exacto\n");
            printf("2) subcadena en nombre_cliente\n");
            int t = 0;
            leer_int("Opcion: ", &t, 1, 2);

            if (t == 1) {
                char cod[MAX_COD];
                leer_linea("Ingrese codigo_orden: ", cod, sizeof(cod));
                int idx = buscar_por_codigo(ordenes, n, cod);
                if (idx < 0) {
                    printf("No encontrado.\n");
                } else {
                    printf("\nResultado:\n");
                    printf("%-15s | %-20s | %-12s | %-22s | %-10s | %-5s | %-10s\n",
                           "codigo_orden", "nombre_cliente", "equipo", "tipo_trabajo",
                           "costo_base", "hrs", "costo_total");
                    printf("--------------------------------------------------------------------------------------------------------------\n");
                    printf("%-15s | %-20s | %-12s | %-22s | %10.2f | %5d | %10.2f\n",
                           ordenes[idx].codigo_orden,
                           ordenes[idx].nombre_cliente,
                           ordenes[idx].equipo,
                           ordenes[idx].tipo_trabajo,
                           ordenes[idx].costo_base,
                           ordenes[idx].horas_trabajo,
                           costo_total(&ordenes[idx], TARIFA_HORA));
                }
            } else {
                char sub[MAX_NOM];
                leer_linea("Ingrese parte del nombre_cliente: ", sub, sizeof(sub));
                buscar_por_nombre(ordenes, n, sub, TARIFA_HORA);
            }
        } else if (opcion == 4) {
            actualizar_orden(ordenes, n, TARIFA_HORA);
        } else if (opcion == 5) {
            eliminar_orden(ordenes, &n);
        } else if (opcion == 6) {
            if (guardar_ordenes_csv(ARCHIVO, ordenes, n)) printf("Guardado exitoso.\n");
        } else if (opcion == 7) {
            if (guardar_ordenes_csv(ARCHIVO, ordenes, n)) {
                printf("Guardado automatico listo. Saliendo...\n");
            } else {
                printf("No se pudo guardar, pero el programa saldra.\n");
            }
        }
    } while (opcion != 7);

    return 0;
}
