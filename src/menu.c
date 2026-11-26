#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "entrada.h"
#include "fecha.h"
#include "turno.h"
#include "cliente.h"
#include "pila_undo.h"

/* Pasos de la reserva (sin enum, igual que en pila_undo.h) */
#define PASO_DNI        0
#define PASO_DEPORTE    1
#define PASO_FECHA      2
#define PASO_FRANJA     3
#define PASO_HORA       4
#define PASO_PAGO       5
#define PASO_CONFIRMADO 6
#define PASO_CANCELADO  7

/* Valor que devuelve leerOpcionReserva cuando el usuario escribe C */
#define CANCELAR_TODO   -2

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

/* Lee una opcion en el rango. Si el usuario escribe C (o c), devuelve CANCELAR_TODO. */
static int leerOpcionReserva(const char *prompt, int min, int max) {
    char linea[50];
    int valor;

    while (1) {
        printf("%s", prompt);
        if (fgets(linea, sizeof(linea), stdin) == NULL) return CANCELAR_TODO;
        if (linea[0] == 'c' || linea[0] == 'C') return CANCELAR_TODO;
        if (sscanf(linea, "%d", &valor) == 1 && valor >= min && valor <= max) {
            return valor;
        }
        printf("Ingresa un numero entre %d y %d, o C para cancelar.\n", min, max);
    }
}

void menuRegistrarCliente(void) {
    tCliente c;

    c.dni = 0;
    while (1) {
        printf("DNI (0 para cancelar): ");
        c.dni = leerEntero();
        if (c.dni == 0) break;
        if (c.dni < 0) {
            printf("DNI invalido. Ingresa solo numeros.\n");
            continue;
        }
        if (buscarClientePorDni(c.dni) != NULL) {
            printf("Ya existe un cliente con el DNI %d. Ingresa otro.\n", c.dni);
            continue;
        }
        break;
    }
    if (c.dni == 0) {
        printf("Operacion cancelada.\n");
        return;
    }

    do {
        printf("Nombre: ");
        leerTexto(c.nombre, sizeof(c.nombre));
        if (strlen(c.nombre) == 0) printf("El nombre no puede estar vacio.\n");
    } while (strlen(c.nombre) == 0);

    printf("Telefono: ");
    leerTexto(c.telefono, sizeof(c.telefono));

    if (agregarCliente(c)) {
        printf("Cliente registrado correctamente.\n");
    } else {
        printf("No se pudo registrar el cliente.\n");
    }
}

void menuNuevaReserva(tArbol **pReservas) {
    tReserva r;
    tAccion accion;
    tCliente *cliente = NULL;
    tFecha fecha = {0};
    tFecha f;
    int paso = PASO_DNI;
    int dni = 0, opDeporte = 0, opFecha = 0, opFranja = 0, opHora = 0, opPago = 0;
    int horaMinima, cant = 0, fin, i;
    int horas[12];

    printf("\n--- NUEVA RESERVA ---\n");
    printf("(En cualquier paso, escribi C para cancelar toda la operacion)\n");

    while (paso != PASO_CONFIRMADO && paso != PASO_CANCELADO) {
        switch (paso) {

        case PASO_DNI:
            dni = leerOpcionReserva("DNI del cliente (0 o C para cancelar): ", 0, 2147483647);
            if (dni == 0 || dni == CANCELAR_TODO) {
                paso = PASO_CANCELADO;
                break;
            }
            cliente = buscarClientePorDni(dni);
            if (cliente == NULL) {
                printf("No existe un cliente con ese DNI. Ingresa otro, o registralo con la opcion 1.\n");
                break;
            }
            printf("Cliente vinculado: %s (Tel: %s)\n", cliente->nombre, cliente->telefono);
            paso = PASO_DEPORTE;
            break;

        case PASO_DEPORTE:
            printf("\n--- DEPORTE ---\n");
            printf("0 - Volver\n1 - Futbol\n2 - Hockey\n");
            printf("C - Cancelar operacion\n");
            opDeporte = leerOpcionReserva("Opcion: ", 0, 2);
            if (opDeporte == CANCELAR_TODO) {
                paso = PASO_CANCELADO;
            } else if (opDeporte == 0) {
                paso = PASO_DNI;
            } else {
                strcpy(r.deporte, opDeporte == 1 ? "Futbol" : "Hockey");
                paso = PASO_FECHA;
            }
            break;

        case PASO_FECHA:
            printf("\n--- FECHA ---\n");
            printf("0 - Volver\n");
            for (i = 0; i <= DIAS_ADELANTE; i++) {
                obtenerFechaDeDia(i, &f);
                printf("%d - %s %s%s\n", i + 1, f.nombreDia, f.texto,
                       f.esHoy ? " (hoy)" : "");
            }
            printf("C - Cancelar operacion\n");
            opFecha = leerOpcionReserva("Opcion: ", 0, DIAS_ADELANTE + 1);
            if (opFecha == CANCELAR_TODO) {
                paso = PASO_CANCELADO;
            } else if (opFecha == 0) {
                paso = PASO_DEPORTE;
            } else {
                obtenerFechaDeDia(opFecha - 1, &fecha);
                paso = PASO_FRANJA;
            }
            break;

        case PASO_FRANJA:
            printf("\n--- TURNO / FRANJA ---\n");
            printf("0 - Volver\n");
            printf("1 - Maniana (09-12hs)\n");
            printf("2 - Tarde (14-18hs)\n");
            printf("3 - Noche (19-01hs)\n");
            printf("C - Cancelar operacion\n");
            opFranja = leerOpcionReserva("Opcion: ", 0, 3);
            if (opFranja == CANCELAR_TODO) {
                paso = PASO_CANCELADO;
            } else if (opFranja == 0) {
                paso = PASO_FECHA;
            } else {
                paso = PASO_HORA;
            }
            break;

        case PASO_HORA:
            horaMinima = horaMinimaPara(&fecha);
            cant = obtenerHorasLibres(*pReservas, &fecha, opFranja, horaMinima, horas);

            if (cant == 0) {
                fin = obtenerFinFranja(opFranja);
                if (horaMinima >= fin) {
                    printf("Esa franja ya termino para hoy. Elegi otra franja u otro dia.\n");
                } else {
                    printf("No hay horarios disponibles en esa franja para esa fecha. Elegi otra.\n");
                }
                paso = PASO_FRANJA;
                break;
            }

            printf("\n--- HORARIOS DISPONIBLES ---\n");
            printf("0 - Volver\n");
            for (i = 0; i < cant; i++) {
                printf("%d. %s %02d:00hs\n", i + 1, fecha.nombreDia, horas[i] % 24);
            }
            printf("C - Cancelar operacion\n");
            opHora = leerOpcionReserva("Opcion: ", 0, cant);
            if (opHora == CANCELAR_TODO) {
                paso = PASO_CANCELADO;
            } else if (opHora == 0) {
                paso = PASO_FRANJA;
            } else {
                r.horaInicio = horas[opHora - 1];
                paso = PASO_PAGO;
            }
            break;

        case PASO_PAGO:
            printf("\n--- METODO DE PAGO ---\n");
            printf("0 - Volver\n1 - Efectivo\n2 - Transferencia\n3 - Tarjeta\n");
            printf("C - Cancelar operacion\n");
            opPago = leerOpcionReserva("Opcion: ", 0, 3);
            if (opPago == CANCELAR_TODO) {
                paso = PASO_CANCELADO;
            } else if (opPago == 0) {
                paso = PASO_HORA;
            } else {
                if (opPago == 1)      strcpy(r.metodoPago, "Efectivo");
                else if (opPago == 2) strcpy(r.metodoPago, "Transferencia");
                else                  strcpy(r.metodoPago, "Tarjeta");
                paso = PASO_CONFIRMADO;
            }
            break;

        default:
            paso = PASO_CANCELADO;
            break;
        }
    }

    if (paso == PASO_CANCELADO) {
        printf("Operacion cancelada.\n");
        return;
    }

    /* Completar los datos de la reserva con lo elegido */
    strcpy(r.fecha, fecha.texto);
    strcpy(r.diaSemana, fecha.nombreDia);
    r.fechaClave = fecha.clave;
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