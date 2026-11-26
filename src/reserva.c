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
        printf(" Deporte: %s | Dia: %s %s - Hora: %02d:00hs\n",
               pArbol->info.deporte, pArbol->info.diaSemana,
               pArbol->info.fecha, pArbol->info.horaInicio);
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

/* ---------- Disponibilidad ---------- */

/* Recorre el arbol y devuelve true si hay una reserva con esa fecha y hora */
bool turnoOcupado(tArbol *pArbol, int pFechaClave, int pHora) {
    if (arbolVacio(pArbol)) return false;
    if (pArbol->info.fechaClave == pFechaClave && pArbol->info.horaInicio == pHora)
        return true;
    return turnoOcupado(pArbol->hijoIzq, pFechaClave, pHora) ||
           turnoOcupado(pArbol->hijoDer, pFechaClave, pHora);
}

int contarReservas(tArbol *pArbol) {
    if (arbolVacio(pArbol)) return 0;
    return 1 + contarReservas(pArbol->hijoIzq) + contarReservas(pArbol->hijoDer);
}

/* Copia todas las reservas a un arreglo. Devuelve la cantidad copiada. */
int recolectarReservas(tArbol *pArbol, tReserva *arreglo, int indice) {
    if (arbolVacio(pArbol)) return indice;
    indice = recolectarReservas(pArbol->hijoIzq, arreglo, indice);
    arreglo[indice++] = pArbol->info;
    return recolectarReservas(pArbol->hijoDer, arreglo, indice);
}

static int compararPorFecha(const void *a, const void *b) {
    const tReserva *ra = (const tReserva *)a;
    const tReserva *rb = (const tReserva *)b;
    if (ra->fechaClave != rb->fechaClave) return ra->fechaClave - rb->fechaClave;
    return ra->horaInicio - rb->horaInicio;
}

/* Lista las reservas ordenadas por fecha y hora */
void visualizarOrdenadoPorFecha(tArbol *pArbol) {
    int total = contarReservas(pArbol);
    tReserva *arreglo;
    int i;
    tCliente *cli;

    if (total == 0) {
        printf("No hay reservas registradas.\n");
        return;
    }

    arreglo = (tReserva *)malloc(sizeof(tReserva) * total);
    if (arreglo == NULL) {
        printf("No hay memoria para listar.\n");
        return;
    }

    recolectarReservas(pArbol, arreglo, 0);
    qsort(arreglo, total, sizeof(tReserva), compararPorFecha);

    printf("\n===================================================\n");
    printf("        LISTADO DE RESERVAS (por fecha)\n");
    printf("===================================================\n");
    for (i = 0; i < total; i++) {
        cli = buscarClientePorDni(arreglo[i].dni);
        printf("ID Turno: %d\n", arreglo[i].idReserva);
        printf(" %s %s - %02d:00hs | %s\n",
               arreglo[i].diaSemana, arreglo[i].fecha, arreglo[i].horaInicio,
               arreglo[i].deporte);
        printf(" Cliente: %s (DNI: %d) | Pago: %s\n",
               cli != NULL ? cli->nombre : "no registrado",
               arreglo[i].dni, arreglo[i].metodoPago);
        printf("---------------------------------------------------\n");
    }
    free(arreglo);
}