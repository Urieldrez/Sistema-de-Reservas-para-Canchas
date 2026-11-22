#include "persistencia.h"

/* ===== PERSISTENCIA DE RESERVAS (ABB) ===== */

void guardarArbolBinario(tArbol *pArbol, FILE *archivo) {
    if (!arbolVacio(pArbol)) {
        fwrite(&(pArbol->info), sizeof(tReserva), 1, archivo);
        guardarArbolBinario(pArbol->hijoIzq, archivo);
        guardarArbolBinario(pArbol->hijoDer, archivo);
    }
}

void persistirReservas(tArbol *pArbol) {
    FILE *archivo = fopen("data/reservas.dat", "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo data/reservas.dat para escritura.\n");
        return;
    }
    guardarArbolBinario(pArbol, archivo);
    fclose(archivo);
}

/* Como se guarda en preorden, insertar en ese orden reconstruye el mismo arbol */
void cargarReservas(tArbol **pArbol) {
    FILE *archivo = fopen("data/reservas.dat", "rb");
    tReserva r;

    if (archivo == NULL) return;  /* primera ejecucion: no hay archivo */

    while (fread(&r, sizeof(tReserva), 1, archivo) == 1) {
        insertarReserva(r, pArbol);
    }
    fclose(archivo);
}

/* ===== PERSISTENCIA DE CLIENTES (lista enlazada) ===== */

void guardarListaClientesBinaria(tListaClientes *pLista, FILE *archivo) {
    tListaClientes *actual = pLista;
    while (actual != NULL) {
        fwrite(&(actual->data), sizeof(tCliente), 1, archivo);
        actual = actual->siguiente;
    }
}

void persistirClientes(tListaClientes *pLista) {
    FILE *archivo = fopen("data/clientes.dat", "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo data/clientes.dat para escritura.\n");
        return;
    }
    guardarListaClientesBinaria(pLista, archivo);
    fclose(archivo);
}

void cargarClientes(void) {
    FILE *archivo = fopen("data/clientes.dat", "rb");
    tCliente c;

    if (archivo == NULL) return;  /* primera ejecucion: no hay archivo */

    while (fread(&c, sizeof(tCliente), 1, archivo) == 1) {
        agregarCliente(c);
    }
    fclose(archivo);
}