#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILES 100
#define MAX_FILENAME 50
#define MAX_CONTENT 1000

typedef struct {
    char nombre[MAX_FILENAME];
    char contenido[MAX_CONTENT];
    bool activo;
} Archivo;

Archivo sistema_archivos[MAX_FILES];
int archivos_count = 0;
int i;

// Función para limpiar el buffer de entrada
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Función para buscar un archivo por nombre
int buscar_archivo(const char *nombre) {
    for ( i = 0; i < archivos_count; i++) {
        if (sistema_archivos[i].activo && strcmp(sistema_archivos[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

// Función para crear un nuevo archivo
void crear_archivo() {
    if (archivos_count >= MAX_FILES) {
        printf("Error: Se ha alcanzado el limite maximo de archivos.\n");
        return;
    }

    char nombre[MAX_FILENAME];
    printf("Ingrese el nombre del archivo: ");
    fgets(nombre, MAX_FILENAME, stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    if (buscar_archivo(nombre) != -1) {
        printf("Error: Ya existe un archivo con ese nombre.\n");
        return;
    }

    printf("Ingrese el contenido del archivo: ");
    char contenido[MAX_CONTENT];
    fgets(contenido, MAX_CONTENT, stdin);
    contenido[strcspn(contenido, "\n")] = '\0';

    Archivo nuevo;
    strncpy(nuevo.nombre, nombre, MAX_FILENAME);
    strncpy(nuevo.contenido, contenido, MAX_CONTENT);
    nuevo.activo = true;

    sistema_archivos[archivos_count++] = nuevo;
    printf("Archivo '%s' creado exitosamente.\n", nombre);
}

// Función para actualizar un archivo existente
void actualizar_archivo() {
    char nombre[MAX_FILENAME];
    printf("Ingrese el nombre del archivo a actualizar: ");
    fgets(nombre, MAX_FILENAME, stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    int index = buscar_archivo(nombre);
    if (index == -1) {
        printf("Error: No se encontro el archivo '%s'.\n", nombre);
        return;
    }

    printf("Contenido actual:\n%s\n", sistema_archivos[index].contenido);
    printf("Ingrese el nuevo contenido: ");

    char nuevo_contenido[MAX_CONTENT];
    fgets(nuevo_contenido, MAX_CONTENT, stdin);
    nuevo_contenido[strcspn(nuevo_contenido, "\n")] = '\0';

    strncpy(sistema_archivos[index].contenido, nuevo_contenido, MAX_CONTENT);
    printf("Archivo '%s' actualizado exitosamente.\n", nombre);
}

// Función para eliminar un archivo
void eliminar_archivo() {
    char nombre[MAX_FILENAME];
    printf("Ingrese el nombre del archivo a eliminar: ");
    fgets(nombre, MAX_FILENAME, stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    int index = buscar_archivo(nombre);
    if (index == -1) {
        printf("Error: No se encontro el archivo '%s'.\n", nombre);
        return;
    }

    sistema_archivos[index].activo = false;
    printf("Archivo '%s' eliminado exitosamente.\n", nombre);
}

// Función para listar todos los archivos
void listar_archivos() {
    printf("\n=== ARCHIVOS DISPONIBLES ===\n");
    int count = 0;
    for (i = 0; i < archivos_count; i++) {
        if (sistema_archivos[i].activo) {
            printf("%d. %s\n", ++count, sistema_archivos[i].nombre);
        }
    }

    if (count == 0) {
        printf("No hay archivos disponibles.\n");
    } else {
        printf("Total: %d archivo(s)\n", count);
    }
}

int main() {
    int opcion;
    do {
        printf("\n=== SISTEMA DE GESTION DE ARCHIVOS ===\n");
        printf("1. Crear archivo\n");
        printf("2. Actualizar archivo\n");
        printf("3. Eliminar archivo\n");
        printf("4. Listar archivos\n");
        printf("5. Salir\n");
        printf("Seleccione una opcion: ");

        scanf("%d", &opcion);
        limpiar_buffer();

        switch (opcion) {
            case 1:
                crear_archivo();
                break;
            case 2:
                actualizar_archivo();
                break;
            case 3:
                eliminar_archivo();
                break;
            case 4:
                listar_archivos();
                break;
            case 5:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opción no valida. Intente nuevamente.\n");
        }
    } while (opcion != 5);

    return 0;
}
