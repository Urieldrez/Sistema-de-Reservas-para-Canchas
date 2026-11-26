# Sistema de Reservas para Canchas

Programa de consola en C para gestionar reservas de canchas de futbol y hockey: registro de clientes, turnos por franja horaria, listado ordenado por fecha, cancelacion y deshacer.

## Funcionalidades

- **Registrar cliente**: DNI (valido y sin repetir), nombre y telefono.
- **Listar clientes**.
- **Nueva reserva**: se elige cliente (por DNI), deporte, dia (hoy y los 7 dias siguientes), franja horaria y horario libre. Luego se elige el metodo de pago.
- **Listar reservas**: ordenadas por fecha y hora.
- **Cancelar reserva**: por ID de reserva.
- **Deshacer ultima accion**: revierte el ultimo alta o cancelacion (orden inverso).

En cualquier paso de una reserva podes escribir `0` para volver al paso anterior o `C` para cancelar toda la operacion.

### Franjas horarias

| Franja   | Horarios de inicio |
|----------|--------------------|
| Maniana  | 09:00 a 12:00      |
| Tarde    | 14:00 a 17:00      |
| Noche    | 19:00 a 00:00      |

Los turnos ya ocupados no se muestran. Si la fecha elegida es hoy, tampoco se muestran las horas que ya pasaron.

## Estructura del proyecto

```
Sistema-de-Reservas-para-Canchas/
├── src/
│   ├── main.c           # Punto de entrada y bucle del menu
│   ├── menu.c           # Opciones del menu e interaccion con el usuario
│   ├── reserva.c        # ABB de reservas y listado ordenado
│   ├── cliente.c        # Lista enlazada de clientes
│   ├── pila_undo.c      # Pila para deshacer acciones
│   ├── persistencia.c   # Lectura y escritura de archivos binarios
│   ├── entrada.c        # Lectura segura de texto y numeros
│   ├── fecha.c          # Fechas y hora actual (time.h)
│   └── turno.c          # Franjas y horarios libres
├── include/
│   ├── reserva.h
│   ├── cliente.h
│   ├── pila_undo.h
│   ├── persistencia.h
│   ├── entrada.h
│   ├── fecha.h
│   ├── turno.h
│   └── menu.h
├── data/
│   ├── reservas.dat     # Se genera al salir
│   └── clientes.dat     # Se genera al salir
├── Makefile
└── README.md
```

## Requisitos

- Compilador C compatible con C99 (`gcc`).
- En Linux: `make` (opcional, pero recomendado).
- En Windows: MinGW-w64 (gcc para Windows).

## Compilacion y ejecucion

Antes de ejecutar, la carpeta `data/` tiene que existir en la raiz del proyecto. Los archivos se guardan y leen desde ahi, con rutas relativas, por lo que el programa debe ejecutarse siempre desde la carpeta raiz.

### Linux (Debian/Ubuntu)

Con `make`:

```bash
mkdir -p data
make
./turnos
```

Para recompilar desde cero:

```bash
make clean
make
```

Sin `make`:

```bash
gcc -Wall -Wextra -std=c99 -g -Iinclude src/*.c -o turnos
./turnos
```

### Windows (MinGW con PowerShell o terminal de VS Code)

Instalar gcc (una opcion):

```powershell
winget search winlibs
winget install -e --id <ID_QUE_APARECE>
```

Despues de instalar, cerrar y volver a abrir VS Code y verificar con `gcc --version`.

Compilar desde la carpeta raiz:

```powershell
mkdir data
gcc -Wall -Wextra -std=c99 -g -Iinclude src/*.c -o turnos.exe
.\turnos.exe
```

Nota: `make` no viene instalado en Windows por defecto. Si preferis usarlo, instalalo con MSYS2 o usa el comando de gcc de arriba.

### Windows con WSL

Si tenes WSL instalado, podes usar exactamente los pasos de Linux desde la terminal de Ubuntu/Debian.

## Archivos de datos

- `data/clientes.dat` y `data/reservas.dat` son binarios. No se leen con un editor de texto.
- Se generan al salir del programa con la opcion `0`.
- Si cambia la estructura de `tReserva` o `tCliente` (por ejemplo, al agregar campos), los archivos viejos dejan de ser compatibles y hay que borrarlos: `rm -f data/*.dat`.
- Los archivos `.dat` no se suben al repositorio (estan en `.gitignore`).

## Notas importantes

- **El ejecutable `turnos` es para Linux.** No funciona en Windows. En Windows hay que compilar y se genera `turnos.exe`.
- La pila de deshacer vive solo durante la ejecucion; no se guarda en archivo.
- La fecha y la hora salen del reloj de la computadora. Si estan mal configuradas, los turnos disponibles tambien.
- Las reservas no pueden hacerse con mas de 7 dias de anticipacion.

## Autores

- Romero Uriel Alberto
- Luque Lourdes
- Lopez Antonella
- Lescano Lucas

Algoritmos y Estructuras de Datos II - FaCENA (UNNE), 2026.