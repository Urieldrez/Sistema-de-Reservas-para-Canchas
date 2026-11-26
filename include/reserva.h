#ifndef RESERVA_H
#define RESERVA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --- TIPOS DE DATOS Y TADs --- */
typedef struct {
    int idReserva;
    char deporte[25];          /* "Futbol" o "Hockey" */
    char fecha[32];            /* Ej: "16/11/2026" (para mostrar) */
    int fechaClave;            /* Ej: 20261116 (para ordenar y comparar) */
    char diaSemana[12];        /* Ej: "Lunes" */
    int horaInicio;            /* Ej: 9, 10, 11... */
    int dni;                   /* DNI del cliente */
    char metodoPago[20];       /* "Efectivo", "Transferencia", "Tarjeta" */
} tReserva;

typedef struct nodoArbol {
    tReserva info;
    struct nodoArbol *hijoIzq;
    struct nodoArbol *hijoDer;
} tArbol;

/* --- PROTOTIPOS (TAD Reserva y ABB) --- */
void crearArbol(tArbol **pArbol);
bool arbolVacio(tArbol *pArbol);
tArbol *maximoElemento(tArbol *pArbol);
tArbol *minimoElemento(tArbol *pArbol);
int generarIdSiguiente(tArbol *pArbol);
bool insertarReserva(tReserva pInfo, tArbol **pArbol);
void visualizarInOrden(tArbol *pArbol, int flag);
tReserva *buscarReserva(tArbol *pArbol, int pId);
bool existeReserva(tArbol *pArbol, int pId);
bool eliminarReserva(tArbol **pArbol, int pId);
void liberarArbol(tArbol **pArbol);

/* --- Disponibilidad y listado ordenado --- */
bool turnoOcupado(tArbol *pArbol, int pFechaClave, int pHora);
int contarReservas(tArbol *pArbol);
int recolectarReservas(tArbol *pArbol, tReserva *arreglo, int indice);
void visualizarOrdenadoPorFecha(tArbol *pArbol);

#endif