/* Pila de acciones para deshacer (implementada con punteros) */
#include <stdio.h>
#include <stdlib.h>
#include "pila_undo.h"

// Declaracion de variables 
tPilaUndo * pilaUndo;

// Implementacion de funciones
void inicializarPilaUndo(){
    pilaUndo = NULL;
}

bool pilaUndoVacia(tPilaUndo *pPila){
    return (pPila == NULL);
}

void apilarAccion(tAccion pAccion){
    tPilaUndo * nuevoNodo;
    nuevoNodo = (tPilaUndo *)malloc(sizeof(tPilaUndo));
    if (nuevoNodo == NULL){
        printf("\nNo se pudo registrar la accion para deshacer!\n");
        return;
    }
    nuevoNodo->data = pAccion;
    nuevoNodo->siguiente = pilaUndo;
    pilaUndo = nuevoNodo;
}

bool desapilarAccion(tAccion *pAccion){
    tPilaUndo * nodoSuprimir;
    if (pilaUndoVacia(pilaUndo)){
        return false;
    }
    nodoSuprimir = pilaUndo;
    *pAccion = nodoSuprimir->data;
    pilaUndo = pilaUndo->siguiente;
    free(nodoSuprimir);
    return true;
}

// Halla la cima e inicializa la variable "tipo" en -1
tAccion cimaAccion(tPilaUndo *pPila) {
    tAccion c;
    c.tipo = -1;
    if (!pilaUndoVacia(pPila)){
        c = pPila->data;
    }
    return c;
}

// libera/elimina cada nodo
void liberarPilaUndo(){
    tAccion descartada;
    while (desapilarAccion(&descartada)){
    }
}