#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"

/* Declaracion de variable global*/
tListaClientes *listaClientes = NULL;

void inicializarListaClientes(void) {
    listaClientes = NULL;
}

bool listaClientesVacia(tListaClientes *pLista) {
    return (pLista == NULL);
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

/* Inserta al final, evitando clientes con DNI repetido */
void insertarCliente(tCliente pCliente) {
    tListaClientes *nuevoNodo;
    tListaClientes *aux;

    if (pCliente.dni <= 0 || buscarClientePorDni(pCliente.dni) != NULL) {
        printf("\nDNI invalido o cliente ya registrado.\n");
        return;
    }

    nuevoNodo = (tListaClientes *)malloc(sizeof(tListaClientes));
    if (nuevoNodo == NULL) {
        printf("\nNo se pudo reservar memoria para el cliente.\n");
        return;
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
    printf("\nCliente registrado correctamente.\n");
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
