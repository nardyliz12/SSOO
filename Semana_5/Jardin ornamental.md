## El jardín ornamental

El problema en general nos ayuda a aprender la sincronización de concurrencia y manejo de eventos independientes. En resumen, el problema nos dice que hay dos torniquetes independientes que detectan cuando pasa un visitante, cada torniquetes manda una señal a la computadora para contar a los visitantes, asimismo, estos dos eventos se encuentran en paralelo, no se sabe cuál llega primero, el problema nos indica que hay 20 visitantes pasando por cada torniquetes, es decir, en total 40 visitantes deben ser contados, por lo que, nuestro objetivo es modelar esto en software de forma que la computadora cuente exactamente visitantes al final.

Para resolver este problema, necesitamos contar de manera segura el número total de visitantes que ingresan al jardín a través de dos torniquetes independientes, evitando condiciones de carrera (race conditions). Usaremos mutex para garantizar que el coteo sea correcto.

## Implementación en lenguaje C:
```c
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
    pthread_jo(torniquete2, NULL);

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

### Pasos principales:

- Inicializa un contador global visitantes = 0 para contar a las personas.

- Crea dos hilos (torniquete1 y torniquete2), cada uno ejecutando la función torniquete().

#### Cada hilo:

- Simula el paso de 20 visitantes.

#### Por cada visitante:

- Espera un tiempo aleatorio breve (usleep()), para imitar la llegada aleatoria.

- Bloquea el contador (pthread_mutex_lock) para incrementar el número de visitantes de manera segura (sin errores por ejecución simultánea).

- Desbloquea el contador (pthread_mutex_unlock) después de actualizar.

- Imprime el avance del conteo.

- El programa espera (pthread_join) a que ambos hilos terminen.

- Al final, verifica si el total de visitantes es el esperado (40) y muestra un mensaje si fue correcto o hubo error.

#### ¿Por qué se usa mutex?

- Para evitar problemas de concurrencia, ya que ambos torniquetes podrían intentar modificar visitantes al mismo tiempo.

- Mutex garantiza que solo un torniquete a la vez pueda cambiar el valor del contador.
