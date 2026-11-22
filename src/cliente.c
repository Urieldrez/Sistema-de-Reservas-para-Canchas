#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"

/* Declaracion de variable global */
tListaClientes *listaClientes = NULL;

void inicializarListaClientes(void) {
    listaClientes = NULL;
}

bool listaClientesVacia(tListaClientes *pLista) {
    return pLista == NULL;
}

/* Devuelve el cliente encontrado o NULL si no existe */
tCliente *buscarClientePorDni(int pDni) {
    tListaClientes *aux = listaClientes;
    while (aux != NULL) {
        if (aux->data.dni == pDni) {
            return &(aux->data);
        }
        aux = aux->siguiente;
    }
    return NULL;
}

/* Agrega al final sin imprimir nada. Devuelve false si el DNI es invalido,
   ya existe o no hay memoria. */
bool agregarCliente(tCliente pCliente) {
    tListaClientes *nuevoNodo;
    tListaClientes *aux;

    if (pCliente.dni <= 0 || buscarClientePorDni(pCliente.dni) != NULL) {
        return false;
    }

    nuevoNodo = (tListaClientes *)malloc(sizeof(tListaClientes));
    if (nuevoNodo == NULL) {
        return false;
    }
    nuevoNodo->data = pCliente;
    nuevoNodo->siguiente = NULL;

    if (listaClientesVacia(listaClientes)) {
        listaClientes = nuevoNodo;
    } else {
        aux = listaClientes;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevoNodo;
    }
    return true;
}

void mostrarClientes(void) {
    tListaClientes *aux = listaClientes;
    if (listaClientesVacia(listaClientes)) {
        printf("\nNo hay clientes registrados.\n");
        return;
    }
    while (aux != NULL) {
        printf("\nDNI: %d | Nombre: %s | Telefono: %s\n",
               aux->data.dni, aux->data.nombre, aux->data.telefono);
        aux = aux->siguiente;
    }
}

int contarClientes(void) {
    int contador = 0;
    tListaClientes *aux = listaClientes;
    while (aux != NULL) {
        contador++;
        aux = aux->siguiente;
    }
    return contador;
}

void liberarListaClientes(void) {
    tListaClientes *nodoSuprimir;
    while (listaClientes != NULL) {
        nodoSuprimir = listaClientes;
        listaClientes = listaClientes->siguiente;
        free(nodoSuprimir);
    }
}