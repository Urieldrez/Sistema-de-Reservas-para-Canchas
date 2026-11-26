#include <stdio.h>
#include "turno.h"

/* Inicio y fin (exclusivo) de cada franja. La hora 24 representa las 00hs. */
#define MANIANA_INI 9
#define MANIANA_FIN 13
#define TARDE_INI   14
#define TARDE_FIN   18
#define NOCHE_INI   19
#define NOCHE_FIN   25

bool franjaValida(int opcionFranja) {
    return opcionFranja >= FRANJA_MANIANA && opcionFranja <= FRANJA_NOCHE;
}

int obtenerInicioFranja(int opcionFranja) {
    if (opcionFranja == FRANJA_MANIANA) return MANIANA_INI;
    if (opcionFranja == FRANJA_TARDE)   return TARDE_INI;
    if (opcionFranja == FRANJA_NOCHE)   return NOCHE_INI;
    return -1;
}

int obtenerFinFranja(int opcionFranja) {
    if (opcionFranja == FRANJA_MANIANA) return MANIANA_FIN;
    if (opcionFranja == FRANJA_TARDE)   return TARDE_FIN;
    if (opcionFranja == FRANJA_NOCHE)   return NOCHE_FIN;
    return -1;
}

int horaMinimaPara(const tFecha *pFecha) {
    if (pFecha->esHoy) return horaActual() + 1;
    return 0;
}

int obtenerHorasLibres(tArbol *reservas, const tFecha *pFecha, int opcionFranja,
                       int horaMinima, int horas[]) {
    int h, cont = 0;
    int inicio, fin;

    if (!franjaValida(opcionFranja)) return 0;

    inicio = obtenerInicioFranja(opcionFranja);
    fin = obtenerFinFranja(opcionFranja);

    for (h = inicio; h < fin; h++) {
        if (h < horaMinima) continue;                       /* ya paso hoy */
        if (turnoOcupado(reservas, pFecha->clave, h)) continue;
        horas[cont++] = h;
    }
    return cont;
}