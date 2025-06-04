#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAX_HILOS 10

// Estructura para la cola de impresión
typedef struct {
    int id_hilo;
} TrabajoImpresion;

// Variables globales
sem_t mutex_impresora;     // Semáforo para exclusión mutua
sem_t mutex_cola;          // Semáforo para proteger la cola
TrabajoImpresion cola_impresion[MAX_HILOS];
int frente_cola = 0;
int final_cola = 0;

// Función para simular la impresión
void imprimir_documento(int id_hilo) {
    printf("Hilo %d esta imprimiendo el documento...\n", id_hilo);
    sleep(1 + rand() % 5); // Tiempo aleatorio entre 1 y 5 segundos
    printf("Hilo %d termino de imprimir\n", id_hilo);
}

// Función para agregar trabajo a la cola
void encolar_trabajo(int id_hilo) {
    sem_wait(&mutex_cola);
    
    cola_impresion[final_cola].id_hilo = id_hilo;
    final_cola = (final_cola + 1) % MAX_HILOS;
    
    sem_post(&mutex_cola);
}

// Función para sacar trabajo de la cola
TrabajoImpresion desencolar_trabajo() {
    sem_wait(&mutex_cola);
    
    TrabajoImpresion trabajo = cola_impresion[frente_cola];
    frente_cola = (frente_cola + 1) % MAX_HILOS;
    
    sem_post(&mutex_cola);
    return trabajo;
}

// Función ejecutada por cada hilo
void* funcion_hilo(void* arg) {
    int id_hilo = *((int*)arg);
    
    // Cada hilo imprime exactamente una vez
    encolar_trabajo(id_hilo);
    
    // Esperar turno para imprimir
    sem_wait(&mutex_impresora);
    
    // Verificar si es nuestro turno (FIFO)
    TrabajoImpresion trabajo_actual = cola_impresion[frente_cola];
    while (trabajo_actual.id_hilo != id_hilo) {
        sem_post(&mutex_impresora);
        usleep(100000); // Pequeña espera
        sem_wait(&mutex_impresora);
        trabajo_actual = cola_impresion[frente_cola];
    }
    
    // Imprimir documento
    imprimir_documento(id_hilo);
    
    // Eliminar el trabajo de la cola
    desencolar_trabajo();
    
    // Liberar el semáforo
    sem_post(&mutex_impresora);
    
    return NULL;
}

int main() {
    pthread_t hilos[MAX_HILOS];
    int ids_hilos[MAX_HILOS];
    
    srand(time(NULL));
    
    // Inicializar semáforos
    sem_init(&mutex_impresora, 0, 1);
    sem_init(&mutex_cola, 0, 1);
    
    // Crear hilos
    for (int i = 0; i < MAX_HILOS; i++) {
        ids_hilos[i] = i + 1;
        pthread_create(&hilos[i], NULL, funcion_hilo, &ids_hilos[i]);
    }
    
    // Esperar a que todos los hilos terminen
    for (int i = 0; i < MAX_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    // Destruir semáforos
    sem_destroy(&mutex_impresora);
    sem_destroy(&mutex_cola);
    
    printf("Todos los documentos han sido impresos.\n");
    return 0;
}
