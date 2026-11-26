#ifndef MENU_H
#define MENU_H

#include "reserva.h"

/* TAD Menu: interaccion con el usuario */

void mostrarMenuPrincipal(void);
void menuRegistrarCliente(void);
void menuNuevaReserva(tArbol **pReservas);
void menuListarReservas(tArbol *reservas);
void menuCancelarReserva(tArbol **pReservas);
void menuDeshacer(tArbol **pReservas);

#endif