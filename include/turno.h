#ifndef TURNO_H
#define TURNO_H

#include <stdbool.h>
#include "reserva.h"
#include "fecha.h"

/* TAD Turno: franjas horarias y horarios libres */

/* Opciones de franja (las usa el menu) */
#define FRANJA_MANIANA 1
#define FRANJA_TARDE   2
#define FRANJA_NOCHE   3

bool franjaValida(int opcionFranja);
int obtenerInicioFranja(int opcionFranja);   /* hora de inicio, -1 si es invalida */
int obtenerFinFranja(int opcionFranja);      /* hora de fin (exclusiva), -1 si es invalida */

/* Primera hora permitida para esa fecha (si es hoy, no se reservan horas pasadas) */
int horaMinimaPara(const tFecha *pFecha);

/* Carga en horas[] las horas libres de la franja. Devuelve la cantidad. */
int obtenerHorasLibres(tArbol *reservas, const tFecha *pFecha, int opcionFranja,
                       int horaMinima, int horas[]);

#endif