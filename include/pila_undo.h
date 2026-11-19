#define PILA_UNDO_H

#include <stdbool.h>
// #include "reserva.h"

/* Tipos de accion (sin enum) */
#define ACCION_ALTA  0   /* se inserto una reserva: deshacer = eliminarla */
#define ACCION_BAJA  1   /* se cancelo una reserva: deshacer = reinsertarla */

/* Tipos de datos personalizados */
typedef struct {
    int tipo;            /* ACCION_ALTA o ACCION_BAJA */
    //tReserva reserva;    /* copia de la reserva afectada */
} tAccion;

typedef struct nodoUndo {
    tAccion data;
    struct nodoUndo *siguiente;
} tPilaUndo;

/* Declaracion de la variable */
extern tPilaUndo *pilaUndo;

/* Declaracion de prototipos */
void inicializarPilaUndo();
bool pilaUndoVacia(tPilaUndo * );
void apilarAccion(tAccion);
bool desapilarAccion(tAccion * );
tAccion cimaAccion(tPilaUndo * );
void liberarPilaUndo();