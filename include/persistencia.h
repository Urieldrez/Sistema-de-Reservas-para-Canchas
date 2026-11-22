#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <stdio.h>
#include "reserva.h"
#include "cliente.h"

/* Persistencia de Reservas (ABB) */
void guardarArbolBinario(tArbol *pArbol, FILE *archivo);
void persistirReservas(tArbol *pArbol);
void cargarReservas(tArbol **pArbol);

/* Persistencia de Clientes (lista enlazada) */
void guardarListaClientesBinaria(tListaClientes *pLista, FILE *archivo);
void persistirClientes(tListaClientes *pLista);
void cargarClientes(void);

#endif