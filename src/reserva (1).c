#include "reserva.h"

void crearArbol(tArbol** pArbol) {
    *pArbol = NULL;
}

bool arbolVacio(tArbol* pArbol) {
    return pArbol == NULL;
}

tArbol* maximoElemento(tArbol* pArbol) {
    if (arbolVacio(pArbol)) {
        return NULL;
    } else if (pArbol->hijoDer != NULL) {
        return maximoElemento(pArbol->hijoDer);
    } else {
        return pArbol;
    }
}

int generarIdSiguiente(tArbol* pArbol) {
    tArbol* max = maximoElemento(pArbol);
    if (max == NULL) {
        return 1;
    } else {
        return max->info.idReserva + 1;
    }
}

void insertarNodo(tReserva pInfo, tArbol** pArbol) {
    if (arbolVacio(*pArbol)) {
        *pArbol = (tArbol*) malloc(sizeof(tArbol));
        if (*pArbol != NULL) {
            (*pArbol)->info = pInfo;
            (*pArbol)->hijoIzq = NULL;
            (*pArbol)->hijoDer = NULL;
            printf("\nReserva registrada con exito! (ID Asignado: %d)\n", pInfo.idReserva);
        } else {
            printf("Error: No queda memoria disponible.\n");
        }
    } else {
        if (pInfo.idReserva < (*pArbol)->info.idReserva) {
            insertarNodo(pInfo, &((*pArbol)->hijoIzq));
        } else if (pInfo.idReserva > (*pArbol)->info.idReserva) {
            insertarNodo(pInfo, &((*pArbol)->hijoDer));
        } else {
            printf("\nError! ID duplicado.\n");
        }
    }
}

void visualizarInOrden(tArbol* pArbol, int flag) {
    if (flag == 0) {
        printf("\n===================================================\n");
        printf("            LISTADO DE RESERVAS          \n");
        printf("===================================================\n");
    }
    if (!arbolVacio(pArbol)) {
        visualizarInOrden(pArbol->hijoIzq, 1);
        printf("ID Turno: %d\n", pArbol->info.idReserva);
        printf(" Deporte: %s | Dia: %s - Hora: %s\n", pArbol->info.deporte, pArbol->info.dia, pArbol->info.hora);
        printf(" Cliente: %s (DNI: %s)\n", pArbol->info.nombreCliente, pArbol->info.dni);
        printf(" Pago: %s\n", pArbol->info.metodoPago);
        printf("---------------------------------------------------\n");
        visualizarInOrden(pArbol->hijoDer, 1);
    }
}

bool existeReserva(tArbol* pArbol, int pId) {
    if (arbolVacio(pArbol)) {
        return false;
    } else {
        if (pId < pArbol->info.idReserva) {
            return existeReserva(pArbol->hijoIzq, pId);
        } else if (pId > pArbol->info.idReserva) {
            return existeReserva(pArbol->hijoDer, pId);
        } else {
            return true;
        }
    }
}

tArbol* minimoElemento(tArbol* pArbol) {
    if (arbolVacio(pArbol)) {
        return NULL;
    } else if (pArbol->hijoIzq != NULL) {
        return minimoElemento(pArbol->hijoIzq);
    } else {
        return pArbol;
    }
}

void eliminarReserva(tArbol** pArbol, int pId) {
    tArbol* aux;
    if (arbolVacio(*pArbol)) {
        printf("\nNo se encontro ninguna reserva con el ID: %d.\n", pId);
    } else {
        if (pId < (*pArbol)->info.idReserva) {
            eliminarReserva(&((*pArbol)->hijoIzq), pId);
        } else if (pId > (*pArbol)->info.idReserva) {
            eliminarReserva(&((*pArbol)->hijoDer), pId);
        } else {
            if ((*pArbol)->hijoIzq == NULL) {
                aux = *pArbol;
                *pArbol = (*pArbol)->hijoDer;
                free(aux);
                printf("\nReserva cancelada exitosamente.\n");
            } else if ((*pArbol)->hijoDer == NULL) {
                aux = *pArbol;
                *pArbol = (*pArbol)->hijoIzq;
                free(aux);
                printf("\nReserva cancelada exitosamente.\n");
            } else {
                aux = minimoElemento((*pArbol)->hijoDer);
                (*pArbol)->info = aux->info;
                eliminarReserva(&((*pArbol)->hijoDer), (*pArbol)->info.idReserva);
            }
        }
    }
}
