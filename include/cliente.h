#include <stdbool.h>

typedef char tString[60];

typedef struct {
    int dni;
    tString nombre;
    tString telefono;
} tCliente;

typedef struct nodoCliente {
    tCliente data;
    struct nodoCliente *siguiente;
} tListaClientes;

extern tListaClientes *listaClientes;

void inicializarListaClientes(void);
bool listaClientesVacia(tListaClientes *pLista);
tCliente *buscarClientePorDni(int pDni);
void insertarCliente(tCliente pCliente);
void mostrarClientes(void);
int contarClientes(void);
void liberarListaClientes(void);

