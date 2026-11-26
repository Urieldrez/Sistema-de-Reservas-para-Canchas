#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fecha.h"

static const char *nombresDias[] = {
    "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"
};

/* mktime corrige meses y años al sumar dias */
static struct tm tmDeDia(int diasDesdeHoy) {
    time_t ahora = time(NULL);
    struct tm t = *localtime(&ahora);
    t.tm_mday += diasDesdeHoy;
    mktime(&t);
    return t;
}

void obtenerFechaDeDia(int diasDesdeHoy, tFecha *pFecha) {
    struct tm t = tmDeDia(diasDesdeHoy);

    strftime(pFecha->texto, sizeof(pFecha->texto), "%d/%m/%Y", &t);
    pFecha->clave = (t.tm_year + 1900) * 10000 + (t.tm_mon + 1) * 100 + t.tm_mday;
    /* tm_wday: 0=Domingo ... 6=Sabado. Lo pasamos a 0=Lunes ... 6=Domingo */
    strcpy(pFecha->nombreDia, nombresDias[(t.tm_wday + 6) % 7]);
    pFecha->esHoy = (diasDesdeHoy == 0);
}

int horaActual(void) {
    time_t ahora = time(NULL);
    struct tm *t = localtime(&ahora);
    return t->tm_hour;
}