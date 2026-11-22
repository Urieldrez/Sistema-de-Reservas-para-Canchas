#include "reserva.h"
#include "cliente.h"

void crearArbol(tArbol **pArbol) {
    *pArbol = NULL;
}

bool arbolVacio(tArbol *pArbol) {
    return pArbol == NULL;
}

tArbol *maximoElemento(tArbol *pArbol) {
    if (arbolVacio(pArbol)) return NULL;
    if (pArbol->hijoDer != NULL) return maximoElemento(pArbol->hijoDer);
    return pArbol;
}

tArbol *minimoElemento(tArbol *pArbol) {
    if (arbolVacio(pArbol)) return NULL;
    if (pArbol->hijoIzq != NULL) return minimoElemento(pArbol->hijoIzq);
    return pArbol;
}

int generarIdSiguiente(tArbol *pArbol) {
    tArbol *max = maximoElemento(pArbol);
    if (max == NULL) return 1;
    return max->info.idReserva + 1;
}

/* Inserta sin imprimir nada. Devuelve false si el ID ya existe o no hay memoria. */
bool insertarReserva(tReserva pInfo, tArbol **pArbol) {
    if (arbolVacio(*pArbol)) {
        *pArbol = (tArbol *)malloc(sizeof(tArbol));
        if (*pArbol == NULL) return false;
        (*pArbol)->info = pInfo;
        (*pArbol)->hijoIzq = NULL;
        (*pArbol)->hijoDer = NULL;
        return true;
    }
    if (pInfo.idReserva < (*pArbol)->info.idReserva)
        return insertarReserva(pInfo, &((*pArbol)->hijoIzq));
    if (pInfo.idReserva > (*pArbol)->info.idReserva)
        return insertarReserva(pInfo, &((*pArbol)->hijoDer));
    return false;  /* ID duplicado */
}

void visualizarInOrden(tArbol *pArbol, int flag) {
    tCliente *cli;

    if (flag == 0) {
        printf("\n===================================================\n");
        printf("                LISTADO DE RESERVAS\n");
        printf("===================================================\n");
    }
    if (!arbolVacio(pArbol)) {
        visualizarInOrden(pArbol->hijoIzq, 1);

        cli = buscarClientePorDni(pArbol->info.dni);
        printf("ID Turno: %d\n", pArbol->info.idReserva);
        printf(" Deporte: %s | Dia: %s - Hora: %s\n",
               pArbol->info.deporte, pArbol->info.dia, pArbol->info.hora);
        printf(" Cliente: %s (DNI: %d)\n",
               cli != NULL ? cli->nombre : "no registrado", pArbol->info.dni);
        printf(" Pago: %s\n", pArbol->info.metodoPago);
        printf("---------------------------------------------------\n");

        visualizarInOrden(pArbol->hijoDer, 1);
    }
}

/* Devuelve un puntero a la reserva con ese ID, o NULL si no existe */
tReserva *buscarReserva(tArbol *pArbol, int pId) {
    if (arbolVacio(pArbol)) return NULL;
    if (pId < pArbol->info.idReserva) return buscarReserva(pArbol->hijoIzq, pId);
    if (pId > pArbol->info.idReserva) return buscarReserva(pArbol->hijoDer, pId);
    return &(pArbol->info);
}

bool existeReserva(tArbol *pArbol, int pId) {
    return buscarReserva(pArbol, pId) != NULL;
}

/* Elimina sin imprimir nada. Devuelve true si la reserva fue eliminada. */
bool eliminarReserva(tArbol **pArbol, int pId) {
    tArbol *aux;
    int idSucesor;

    if (arbolVacio(*pArbol)) return false;

    if (pId < (*pArbol)->info.idReserva)
        return eliminarReserva(&((*pArbol)->hijoIzq), pId);
    if (pId > (*pArbol)->info.idReserva)
        return eliminarReserva(&((*pArbol)->hijoDer), pId);

    if ((*pArbol)->hijoIzq == NULL) {
        aux = *pArbol;
        *pArbol = (*pArbol)->hijoDer;
        free(aux);
    } else if ((*pArbol)->hijoDer == NULL) {
        aux = *pArbol;
        *pArbol = (*pArbol)->hijoIzq;
        free(aux);
    } else {
        aux = minimoElemento((*pArbol)->hijoDer);
        idSucesor = aux->info.idReserva;
        (*pArbol)->info = aux->info;
        eliminarReserva(&((*pArbol)->hijoDer), idSucesor);
    }
    return true;
}

void liberarArbol(tArbol **pArbol) {
    if (*pArbol != NULL) {
        liberarArbol(&((*pArbol)->hijoIzq));
        liberarArbol(&((*pArbol)->hijoDer));
        free(*pArbol);
        *pArbol = NULL;
    }
}