#ifndef PILA_UNDO_H
#define PILA_UNDO_H

#include <stdbool.h>
#include "reserva.h"

/* Tipos de accion */
#define ACCION_ALTA  0   /* se inserto una reserva: deshacer = eliminarla */
#define ACCION_BAJA  1   /* se cancelo una reserva: deshacer = reinsertarla */

typedef struct {
    int tipo;                 /* ACCION_ALTA o ACCION_BAJA */
    tReserva reserva;         /* copia de la reserva afectada */
} tAccion;

typedef struct nodoUndo {
    tAccion data;
    struct nodoUndo *siguiente;
} tPilaUndo;

extern tPilaUndo *pilaUndo;

void inicializarPilaUndo(void);
bool pilaUndoVacia(tPilaUndo *pPila);
void apilarAccion(tAccion pAccion);
bool desapilarAccion(tAccion *pAccion);
tAccion cimaAccion(tPilaUndo *pPila);
void liberarPilaUndo(void);

#endif