//reserva.h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --- TIPOS DE DATOS Y TADs --- */
typedef struct {
    int idReserva;             
    char deporte[25];          // "Futbol" o "Hockey"
    char dia[20];              // Modificado para recibir formato como "Lunes 16/11/2026"
    char hora[10];             // Ej: "18:00"
    char nombreCliente[50];    
    char dni[15];              
    char metodoPago[20];       // "Efectivo", "Transferencia", "Tarjeta"
} tReserva;

typedef struct nodoArbol {
    tReserva info;
    struct nodoArbol* hijoIzq;
    struct nodoArbol* hijoDer;
} tArbol;

/* --- PROTOTIPOS DE FUNCIONES (TAD Reserva y ABB) --- */
void crearArbol(tArbol** pArbol);
bool arbolVacio(tArbol* pArbol);
tArbol* maximoElemento(tArbol* pArbol);
int generarIdSiguiente(tArbol* pArbol);
void insertarNodo(tReserva pInfo, tArbol** pArbol);
void visualizarInOrden(tArbol* pArbol, int flag);
bool existeReserva(tArbol* pArbol, int pId);
tArbol* minimoElemento(tArbol* pArbol);
void eliminarReserva(tArbol** pArbol, int pId);