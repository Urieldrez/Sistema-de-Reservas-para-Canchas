#ifndef ENTRADA_H
#define ENTRADA_H

/* TAD Entrada: lectura segura de datos desde teclado */

/* Lee una linea de texto y quita el salto de linea final */
void leerTexto(char *destino, int tam);

/* Lee un entero. Devuelve -1 si la entrada no es un numero */
int leerEntero(void);

/* Muestra prompt y repite hasta que se ingrese un entero entre min y max (inclusive) */
int leerEnteroEnRango(const char *prompt, int min, int max);

#endif