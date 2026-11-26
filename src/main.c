#include <stdio.h>
#include "reserva.h"
#include "cliente.h"
#include "pila_undo.h"
#include "persistencia.h"
#include "entrada.h"
#include "menu.h"

int main(void) {
    tArbol *reservas;
    int opcion;

    crearArbol(&reservas);
    inicializarListaClientes();
    inicializarPilaUndo();

    cargarClientes();
    cargarReservas(&reservas);

    do {
        mostrarMenuPrincipal();
        opcion = leerEntero();

        switch (opcion) {
            case 1: menuRegistrarCliente(); break;
            case 2: mostrarClientes(); break;
            case 3: menuNuevaReserva(&reservas); break;
            case 4: menuListarReservas(reservas); break;
            case 5: menuCancelarReserva(&reservas); break;
            case 6: menuDeshacer(&reservas); break;
            case 0: printf("Guardando datos...\n"); break;
            default: printf("Opcion invalida.\n"); break;
        }
    } while (opcion != 0);

    persistirReservas(reservas);
    persistirClientes(listaClientes);

    liberarPilaUndo();
    liberarListaClientes();
    liberarArbol(&reservas);
    return 0;
}