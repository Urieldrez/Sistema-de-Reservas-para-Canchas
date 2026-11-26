#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "entrada.h"
#include "fecha.h"
#include "turno.h"
#include "cliente.h"
#include "pila_undo.h"

void mostrarMenuPrincipal(void) {
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

/* Muestra hoy y los proximos dias y devuelve la fecha elegida */
static bool elegirFecha(tFecha *pFecha) {
    tFecha f;
    int i, op;

    printf("\n--- FECHA ---\n");
    for (i = 0; i <= DIAS_ADELANTE; i++) {
        obtenerFechaDeDia(i, &f);
        printf("%d - %s %s%s\n", i + 1, f.nombreDia, f.texto,
               f.esHoy ? " (hoy)" : "");
    }
    printf("Opcion: ");
    op = leerEntero();
    if (op < 1 || op > DIAS_ADELANTE + 1) return false;

    obtenerFechaDeDia(op - 1, pFecha);
    return true;
}

/* Muestra franja y horarios libres, y devuelve la hora elegida, o -1 */
static int elegirHora(tArbol *reservas, const tFecha *pFecha) {
    int opFranja, opHora, cant, i, horaMinima, fin;
    int horas[12];

    printf("\n--- TURNO / FRANJA ---\n");
    printf("1 - Maniana (09-12hs)\n");
    printf("2 - Tarde (14-18hs)\n");
    printf("3 - Noche (19-01hs)\n");
    printf("Opcion: ");
    opFranja = leerEntero();
    if (!franjaValida(opFranja)) return -1;

    horaMinima = horaMinimaPara(pFecha);
    cant = obtenerHorasLibres(reservas, pFecha, opFranja, horaMinima, horas);

    printf("\n--- HORARIOS DISPONIBLES ---\n");
    if (cant == 0) {
        fin = obtenerFinFranja(opFranja);
        if (horaMinima >= fin) {
            printf("Esa franja ya termino para hoy. Elegi otra franja u otro dia.\n");
        } else {
            printf("No hay horarios disponibles en esa franja para esa fecha.\n");
        }
        return -1;
    }

    for (i = 0; i < cant; i++) {
        printf("%d. %s %02d:00hs\n", i + 1, pFecha->nombreDia, horas[i] % 24);
    }

    printf("Opcion: ");
    opHora = leerEntero();
    if (opHora < 1 || opHora > cant) return -1;
    return horas[opHora - 1];
}

void menuRegistrarCliente(void) {
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

void menuNuevaReserva(tArbol **pReservas) {
    tReserva r;
    tAccion accion;
    tCliente *cliente;
    tFecha fecha;
    int dni, op, hora;

    printf("\n--- NUEVA RESERVA ---\n");
    printf("DNI del cliente: ");
    dni = leerEntero();
    cliente = buscarClientePorDni(dni);
    if (cliente == NULL) {
        printf("Cliente no registrado. Registrelo primero (opcion 1).\n");
        return;
    }
    printf("Cliente vinculado: %s (Tel: %s)\n", cliente->nombre, cliente->telefono);

    printf("\n--- DEPORTE ---\n");
    printf("1 - Futbol\n2 - Hockey\nOpcion: ");
    op = leerEntero();
    if (op == 1)      strcpy(r.deporte, "Futbol");
    else if (op == 2) strcpy(r.deporte, "Hockey");
    else { printf("Opcion de deporte invalida.\n"); return; }

    if (!elegirFecha(&fecha)) {
        printf("Fecha invalida.\n");
        return;
    }

    hora = elegirHora(*pReservas, &fecha);
    if (hora == -1) {
        printf("Turno invalido o sin horarios disponibles.\n");
        return;
    }

    strcpy(r.fecha, fecha.texto);
    strcpy(r.diaSemana, fecha.nombreDia);
    r.fechaClave = fecha.clave;
    r.horaInicio = hora;

    printf("\n--- METODO DE PAGO ---\n");
    printf("1 - Efectivo\n2 - Transferencia\n3 - Tarjeta\nOpcion: ");
    op = leerEntero();
    if (op == 1)      strcpy(r.metodoPago, "Efectivo");
    else if (op == 2) strcpy(r.metodoPago, "Transferencia");
    else if (op == 3) strcpy(r.metodoPago, "Tarjeta");
    else { printf("Metodo de pago invalido.\n"); return; }

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

void menuListarReservas(tArbol *reservas) {
    visualizarOrdenadoPorFecha(reservas);
}

void menuCancelarReserva(tArbol **pReservas) {
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

void menuDeshacer(tArbol **pReservas) {
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