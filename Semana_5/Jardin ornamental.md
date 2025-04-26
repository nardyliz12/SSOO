## El jardín ornamental

El problema en general nos ayuda a aprender la sincronización de concurrencia y manejo de eventos independientes. En resumen, el problema nos dice que hay dos torniquetes independientes que detectan cuando pasa un visitante, cada torniquetes manda una señal a la computadora para contar a los visitantes, asimismo, estos dos eventos se encuentran en paralelo, no se sabe cuál llega primero, el problema nos indica que hay 20 visitantes pasando por cada torniquetes, es decir, en total 40 visitantes deben ser contados, por lo que, nuestro objetivo es modelar esto en software de forma que la computadora cuente exactamente visitantes al final.

Para resolver este problema, necesitamos contar de manera segura el número total de visitantes que ingresan al jardín a través de dos torniquetes independientes, evitando condiciones de carrera (race conditions). Usaremos mutex para garantizar que el coteo sea correcto.

## Implementación en lenguaje C:
```
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Para usleep

#define TOTAL_VISITANTES 40
#define VISITANTES_POR_TORNIQUETE 20

int visitantes = 0; // Contador compartido
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el contador

// Función que simula el ingreso de visitantes por un torniquete
void* torniquete(void* arg) {
    char* nombre = (char*) arg;
    for (int i = 0; i < VISITANTES_POR_TORNIQUETE; i++) {
        usleep((rand() % 100) * 1000); // Simula un pequeño retraso aleatorio (0-100 ms)
        
        pthread_mutex_lock(&mutex); // 🔒 Bloquear el acceso al contador
        visitantes++; // Incrementar el contador de manera segura
        printf("%s: Visitante contado. Total hasta ahora: %d\n", nombre, visitantes);
        pthread_mutex_unlock(&mutex); // 🔓 Liberar el acceso
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // Inicializar semilla aleatoria para usleep

    pthread_t torniquete1, torniquete2;

    // Crear dos hilos (simulando dos torniquetes)
    pthread_create(&torniquete1, NULL, torniquete, "Torniquete 1");
    pthread_create(&torniquete2, NULL, torniquete, "Torniquete 2");

    // Esperar a que ambos hilos terminen
    pthread_join(torniquete1, NULL);
    pthread_join(torniquete2, NULL);

    // Verificar el resultado
    printf("\nVisitantes totales: %d (esperado: %d)\n", visitantes, TOTAL_VISITANTES);

    if (visitantes == TOTAL_VISITANTES) {
        printf("✅ Conteo correcto.\n");
    } else {
        printf("❌ Error en el conteo.\n");
    }

    return 0;
}
```

