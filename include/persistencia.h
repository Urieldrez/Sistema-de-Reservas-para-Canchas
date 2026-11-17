#include <stdio.h>
#include "reserva.h"

//Persistencia de Reservas (ABB)
void guardarArbolBinario(tArbol* pArbol, FILE* archivo);
void persistirReservas(tArbol* pArbol);

//Persistencia de Clientes (Lista Enlazada)
void guardarListaClientesBinaria(tListaClientes* pLista, FILE* archivo);
void persistirClientes(tListaClientes* pLista);
