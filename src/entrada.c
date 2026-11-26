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

int leerEnteroEnRango(const char *prompt, int min, int max) {
    int valor;
    while (1) {
        printf("%s", prompt);
        valor = leerEntero();
        if (valor >= min && valor <= max) {
            return valor;
        }
        printf("Ingresa un numero entre %d y %d.\n", min, max);
    }
}