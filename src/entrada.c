#include <stdio.h>
#include <string.h>
#include "entrada.h"

void leerTexto(char *destino, int tam) {
    if (fgets(destino, tam, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    }
}

int leerEntero(void) {
    char linea[50];
    int valor;
    if (fgets(linea, sizeof(linea), stdin) != NULL && sscanf(linea, "%d", &valor) == 1) {
        return valor;
    }
    return -1;
}