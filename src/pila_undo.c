#include <stdio.h>
#include <stdlib.h>
#include "pila_undo.h"

tPilaUndo *pilaUndo = NULL;

void inicializarPilaUndo(void) {
    pilaUndo = NULL;
}

bool pilaUndoVacia(tPilaUndo *pPila) {
    return pPila == NULL;
}

void apilarAccion(tAccion pAccion) {
    tPilaUndo *nuevoNodo = (tPilaUndo *)malloc(sizeof(tPilaUndo));
    if (nuevoNodo == NULL) {
        printf("\nNo se pudo registrar la accion para deshacer!\n");
        return;
    }
    nuevoNodo->data = pAccion;
    nuevoNodo->siguiente = pilaUndo;
    pilaUndo = nuevoNodo;
}

bool desapilarAccion(tAccion *pAccion) {
    tPilaUndo *nodoSuprimir;
    if (pilaUndoVacia(pilaUndo)) {
        return false;
    }
    nodoSuprimir = pilaUndo;
    *pAccion = nodoSuprimir->data;
    pilaUndo = nodoSuprimir->siguiente;
    free(nodoSuprimir);
    return true;
}

/* Devuelve la cima sin desapilar. Si la pila esta vacia, tipo = -1 */
tAccion cimaAccion(tPilaUndo *pPila) {
    tAccion c;
    c.tipo = -1;
    if (!pilaUndoVacia(pPila)) {
        c = pPila->data;
    }
    return c;
}

void liberarPilaUndo(void) {
    tAccion descartada;
    while (desapilarAccion(&descartada)) {
    }
}