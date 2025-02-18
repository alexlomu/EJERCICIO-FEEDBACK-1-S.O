#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//Apartado 1
#define N 10 
#define NHP 35 

typedef struct {
    int id;
    int val;
} hebval_t;

hebval_t array[N];
int insert_pos = 0;
sem_t mutex, full, empty;

//Apartado 2
void* hebraProductora(void* arg) {
    hebval_t* dato = (hebval_t*) arg;
    
    sem_wait(&empty); 
    sem_wait(&mutex); 
    //Apartado 2.1
    array[insert_pos] = *dato;
    insert_pos++;
    //Apartado 2.2
    if (insert_pos == N) { 
        insert_pos = 0;
        sem_post(&full); 
    }
    //Apartado 2.3
    sem_post(&mutex);
    return NULL;
}

//Apartado 3
void* hebraConsumidora(void* arg) {
    sem_wait(&full); 
    sem_wait(&mutex);
    
    printf("Contenido del array: [");
    for (int i = 0; i < N; i++) {
        printf("[id %d, val %d]", array[i].id, array[i].val);
        if (i < N - 1) printf(", ");
    }
    printf("]\n");
    
    sem_post(&mutex);
    return NULL;
}

//Apartado 4
int main() {
    //Apartado 4.1
    pthread_t productores[NHP], consumidor;
    hebval_t datos[NHP];
    
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, N);
    
    //Apartado 4.2
    for (int i = 0; i < N; i++) {
        array[i].id = -1;
        array[i].val = -1;
    }
    
    //Apartado 4.3
    for (int i = 0; i < NHP; i++) {
        datos[i].id = i;
        datos[i].val = i;
        pthread_create(&productores[i], NULL, hebraProductora, &datos[i]);
    }
    
    //Apartado 4.4
    pthread_create(&consumidor, NULL, hebraConsumidora, NULL);
    
    //Apartado 4.5
    for (int i = 0; i < NHP; i++) {
        pthread_join(productores[i], NULL);
    }
    pthread_join(consumidor, NULL);
    

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    return 0;
}

