#include "persistencia.h"


//PERSISTENCIA DE RESERVAS (ABB)

void guardarArbolBinario(tArbol* pArbol, FILE* archivo) {
    if (!arbolVacio(pArbol)) {
        fwrite(&(pArbol->info), sizeof(tReserva), 1, archivo);
        guardarArbolBinario(pArbol->hijoIzq, archivo);
        guardarArbolBinario(pArbol->hijoDer, archivo);
    }
}

void persistirReservas(tArbol* pArbol) {
    FILE* archivo = fopen("data/reservas.dat", "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo data/reservas.dat para escritura.\n");
        return;
    }
    guardarArbolBinario(pArbol, archivo);
    fclose(archivo);
}

//PERSISTENCIA DE CLIENTES (Lista Enlazada)

void guardarListaClientesBinaria(tListaClientes* pLista, FILE* archivo) {
    tListaClientes* actual = pLista;
    // Recorremos la lista enlazada hasta el final
    while (actual != NULL) {
        fwrite(&(actual->data), sizeof(tCliente), 1, archivo);
        actual = actual->siguiente;
    }
}

void persistirClientes(tListaClientes* pLista) {
    FILE* archivo = fopen("data/clientes.dat", "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo data/clientes.dat para escritura.\n");
        return;
    }
    guardarListaClientesBinaria(pLista, archivo);
    fclose(archivo);
}
