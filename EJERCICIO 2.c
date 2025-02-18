#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
// Apartado 1
// Apartado 1.1
#define SIZE 10

// Apartado 1.2
typedef struct {
    int base;
    int exp;
    int potencia;
} potencia_t;

// Apartado 1.3
typedef potencia_t* potenciaP_t;

// Apartado 1.4
void setBaseExp(potencia_t *p, int base, int exp) {
    if (p != NULL) {
        p->base = base;
        p->exp = exp;
        p->potencia = -1;
    }
}

// Apartado 1.5
int getPotencia(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// Apartado 1.6
typedef void* (*void_func_t)(void*);
void setPotenciaEst(potencia_t *p) {
    if (p != NULL) {
        p->potencia = getPotencia(p->base, p->exp);
    }
}

// Apartado 2
// Apartado 2.1
void initArrayEst(potencia_t arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        arr[i].base = i + 1;
        arr[i].exp = 0;
        arr[i].potencia = 1;
    }
}

// Apartado 2.2
void printArrayEst(potencia_t arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        printf("arr[%d]: base: %d exp: %d potencia: %d\n", i, arr[i].base, arr[i].exp, arr[i].potencia);
    }
}

// Apartado 2.3
void* calcuPotHeb(void* arg) {
    potencia_t *p = (potencia_t*)arg;
    if (p != NULL) {
        p->potencia = getPotencia(p->base, p->exp);
    }
    return NULL;
}

// Apartado 3
int main() {
    // Apartado 3.1
    potencia_t arr1[SIZE], arr2[SIZE];
    pthread_t threads[SIZE];

    // Apartado 3.2
    initArrayEst(arr1);

    // Apartado 3.3
    printArrayEst(arr1);

    // Apartado 3.4
    for (int i = 0; i < SIZE; i++) {
        setBaseExp(&arr1[i], i, 2);
    }

    // Apartado 3.5
    printArrayEst(arr1);

    // Apartado 3.6
    for (int i = 0; i < SIZE; i++) {
        pid_t pid = fork();
        if (pid == 0) { 
            exit(getPotencia(arr1[i].base, arr1[i].exp));
        }
    }

    // Apartado 3.7
    printArrayEst(arr1);

    // Apartado 3.8
    initArrayEst(arr2);
    
    // Apartado 3.10
    printArrayEst(arr2);

    // Apartado 3.11
    for (int i = 0; i < SIZE; i++) {
        setBaseExp(&arr2[i], i, 2);
    }

    // Apartado 3.12
    printArrayEst(arr2);

    // Apartado 3.13
    for (int i = 0; i < SIZE; i++) {
        pthread_create(&threads[i], NULL, calcuPotHeb, (void*)&arr2[i]);
    }
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Apartado 3.14
    printArrayEst(arr2);
    return 0;
}
