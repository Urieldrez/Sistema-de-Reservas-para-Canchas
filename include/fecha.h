#ifndef FECHA_H
#define FECHA_H

#include <stdbool.h>

/* Cantidad de dias a futuro que se pueden reservar (ademas de hoy) */
#define DIAS_ADELANTE 7

/* TAD Fecha: fechas basadas en el reloj de la PC */
typedef struct {
    char texto[32];      /* Ej: "16/11/2026" */
    int clave;           /* Ej: 20261116 (para ordenar y comparar) */
    char nombreDia[12];  /* Ej: "Lunes" */
    bool esHoy;
} tFecha;

/* Carga en pFecha la fecha de hoy + diasDesdeHoy */
void obtenerFechaDeDia(int diasDesdeHoy, tFecha *pFecha);

/* Devuelve la hora actual de la PC (0 a 23) */
int horaActual(void);

#endif