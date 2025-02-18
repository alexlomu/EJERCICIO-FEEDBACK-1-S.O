#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 10

// Apartado 1
typedef struct {
    int base;
    int exp;
    int potencia;
} potencia_t;

typedef potencia_t* potenciaP_t;

void setBaseExp(potencia_t *p, int base, int exp) {
    if (p != NULL) {
        p->base = base;
        p->exp = exp;
        p->potencia = -1;
    }
}

int getPotencia(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

typedef void* (*void_func_t)(void*);
void setPotenciaEst(potencia_t *p) {
    if (p != NULL) {
        p->potencia = getPotencia(p->base, p->exp);
    }
}

// Apartado 2
void initArrayEst(potencia_t arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        arr[i].base = i + 1;
        arr[i].exp = 0;
        arr[i].potencia = 1;
    }
}

void printArrayEst(potencia_t arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        printf("arr[%d]: base: %d exp: %d potencia: %d\n", i, arr[i].base, arr[i].exp, arr[i].potencia);
    }
}

void* calcuPotHeb(void* arg) {
    potencia_t *p = (potencia_t*)arg;
    if (p != NULL) {
        p->potencia = getPotencia(p->base, p->exp);
    }
    return NULL;
}

// Apartado 3
int main() {
    potencia_t arr1[SIZE], arr2[SIZE];
    pthread_t threads[SIZE];

    initArrayEst(arr1);
    printArrayEst(arr1);

    for (int i = 0; i < SIZE; i++) {
        setBaseExp(&arr1[i], i, 2);
    }
    printArrayEst(arr1);

    for (int i = 0; i < SIZE; i++) {
        pid_t pid = fork();
        if (pid == 0) { 
            exit(getPotencia(arr1[i].base, arr1[i].exp));
        }
    }

    initArrayEst(arr2);
    printArrayEst(arr2);

    for (int i = 0; i < SIZE; i++) {
        setBaseExp(&arr2[i], i, 2);
    }
    printArrayEst(arr2);

    for (int i = 0; i < SIZE; i++) {
        pthread_create(&threads[i], NULL, calcuPotHeb, (void*)&arr2[i]);
    }
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printArrayEst(arr2);
    return 0;
}
