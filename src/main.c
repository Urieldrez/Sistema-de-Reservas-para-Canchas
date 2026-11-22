#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reserva.h"
#include "cliente.h"
#include "pila_undo.h"
#include "persistencia.h"

/* ---------- Funciones auxiliares de entrada ---------- */

static void leerTexto(char *destino, int tam) {
    if (fgets(destino, tam, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    }
}

/* Devuelve -1 si la entrada no es un numero */
static int leerEntero(void) {
    char linea[50];
    int valor;
    if (fgets(linea, sizeof(linea), stdin) != NULL && sscanf(linea, "%d", &valor) == 1) {
        return valor;
    }
    return -1;
}

static void mostrarMenu(void) {
    printf("\n===== MENU =====\n");
    printf("1. Registrar cliente\n");
    printf("2. Listar clientes\n");
    printf("3. Nueva reserva\n");
    printf("4. Listar reservas\n");
    printf("5. Cancelar reserva\n");
    printf("6. Deshacer ultima accion\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}

/* ---------- Opciones del menu ---------- */

static void menuRegistrarCliente(void) {
    tCliente c;

    printf("DNI: ");
    c.dni = leerEntero();
    printf("Nombre: ");
    leerTexto(c.nombre, sizeof(c.nombre));
    printf("Telefono: ");
    leerTexto(c.telefono, sizeof(c.telefono));

    if (agregarCliente(c)) {
        printf("Cliente registrado correctamente.\n");
    } else {
        printf("DNI invalido o cliente ya registrado.\n");
    }
}

static void menuNuevaReserva(tArbol **pReservas) {
    tReserva r;
    tAccion accion;
    tCliente *cliente;
    int dni, op;

    printf("DNI del cliente: ");
    dni = leerEntero();
    cliente = buscarClientePorDni(dni);
    if (cliente == NULL) {
        printf("Cliente no registrado. Registrelo primero (opcion 1).\n");
        return;
    }
    printf("Cliente: %s\n", cliente->nombre);

    printf("Deporte (1-Futbol, 2-Hockey): ");
    op = leerEntero();
    if (op == 1) {
        strcpy(r.deporte, "Futbol");
    } else if (op == 2) {
        strcpy(r.deporte, "Hockey");
    } else {
        printf("Opcion de deporte invalida.\n");
        return;
    }

    printf("Dia (ej: Lunes 16/11/2026): ");
    leerTexto(r.dia, sizeof(r.dia));
    printf("Hora (ej: 18:00): ");
    leerTexto(r.hora, sizeof(r.hora));

    printf("Metodo de pago (1-Efectivo, 2-Transferencia, 3-Tarjeta): ");
    op = leerEntero();
    if (op == 1) {
        strcpy(r.metodoPago, "Efectivo");
    } else if (op == 2) {
        strcpy(r.metodoPago, "Transferencia");
    } else if (op == 3) {
        strcpy(r.metodoPago, "Tarjeta");
    } else {
        printf("Metodo de pago invalido.\n");
        return;
    }

    r.idReserva = generarIdSiguiente(*pReservas);
    r.dni = dni;

    if (insertarReserva(r, pReservas)) {
        accion.tipo = ACCION_ALTA;
        accion.reserva = r;
        apilarAccion(accion);
        printf("Reserva registrada con ID %d.\n", r.idReserva);
    } else {
        printf("No se pudo registrar la reserva.\n");
    }
}

static void menuListarReservas(tArbol *reservas) {
    if (arbolVacio(reservas)) {
        printf("No hay reservas registradas.\n");
    } else {
        visualizarInOrden(reservas, 0);
    }
}

static void menuCancelarReserva(tArbol **pReservas) {
    int id;
    tReserva *r;
    tAccion accion;

    printf("ID de la reserva a cancelar: ");
    id = leerEntero();

    r = buscarReserva(*pReservas, id);
    if (r == NULL) {
        printf("No existe una reserva con ID %d.\n", id);
        return;
    }

    accion.tipo = ACCION_BAJA;
    accion.reserva = *r;          /* copiar ANTES de eliminar */
    eliminarReserva(pReservas, id);
    apilarAccion(accion);
    printf("Reserva cancelada.\n");
}

static void menuDeshacer(tArbol **pReservas) {
    tAccion accion;

    if (!desapilarAccion(&accion)) {
        printf("No hay acciones para deshacer.\n");
        return;
    }

    if (accion.tipo == ACCION_ALTA) {
        eliminarReserva(pReservas, accion.reserva.idReserva);
        printf("Se deshizo el alta de la reserva %d.\n", accion.reserva.idReserva);
    } else if (accion.tipo == ACCION_BAJA) {
        if (insertarReserva(accion.reserva, pReservas)) {
            printf("Se restauro la reserva %d.\n", accion.reserva.idReserva);
        } else {
            printf("No se pudo restaurar la reserva %d.\n", accion.reserva.idReserva);
        }
    }
}

/* ---------- Programa principal ---------- */

int main(void) {
    tArbol *reservas;
    int opcion;

    crearArbol(&reservas);
    inicializarListaClientes();
    inicializarPilaUndo();

    cargarClientes();
    cargarReservas(&reservas);

    do {
        mostrarMenu();
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