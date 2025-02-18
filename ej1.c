#include <stdio.h>

typedef struct {
    int arrInt[10];   
    int arrSize;      
    int arrAdd;       
} arrayLength_t;

// Apartado 1
int initArray(arrayLength_t *arr) {
    if (arr == NULL) return -1;
    for (int i = 0; i < 10; i++) {
        arr->arrInt[i] = -1;
    }
    arr->arrSize = 0;
    arr->arrAdd = 0;
    return 0;
}

// Apartado 2
void printArr(const arrayLength_t *arr) {
    if (arr == NULL) return;
    printf("{[");
    for (int i = 0; i < 10; i++) {
        printf("%d", arr->arrInt[i]);
        if (i < 9) printf(", ");
    }
    printf("], %d, %d}\n", arr->arrSize, arr->arrAdd);
}

int addElement(arrayLength_t *arr, int value) {
    if (arr == NULL || value < 0) return -1;
    if (arr->arrSize >= 10) return -1;
    
    arr->arrInt[arr->arrSize] = value;
    arr->arrSize++;
    arr->arrAdd += value;
    return 0;
}

// Apartado 3
int getArrSize(const arrayLength_t *arr) {
    if (arr == NULL) return -1;
    return arr->arrSize;
}

int getArrAdd(const arrayLength_t *arr) {
    if (arr == NULL) return -1;
    return arr->arrAdd;
}

int getElement(const arrayLength_t *arr, int index) {
    if (arr == NULL || index < 0 || index >= 10) return -1;
    return arr->arrInt[index];
}

// Apartado 4
int setElement(arrayLength_t *arr, int index, int value) {
    if (arr == NULL || index < 0 || index >= 10 || value < 0) return -1;
    
    arr->arrAdd = arr->arrAdd - arr->arrInt[index] + value;
    arr->arrInt[index] = value;
    return 0;
}

// Apartado 5
int resetArr(arrayLength_t *arr) {
    return initArray(arr);
}

// Apartado 6
int main() {
    arrayLength_t al1, al2;
    if (initArray(&al1) != 0 || initArray(&al2) != 0) {
        printf("Error en inicialización\n");
        return -1;
    }
    
    for (int i = 0; i < 10; i++) {
        if (addElement(&al1, i * 10) != 0) {
            printf("Error al añadir elemento %d en al1\n", i * 10);
        }
    }
    printArr(&al1);
    
    for (int i = 1; i < 10; i += 2) {
        if (setElement(&al1, i, i) != 0) {
            printf("Error al actualizar elemento en posición %d\n", i);
        }
    }
    printArr(&al1);
    
    for (int i = 0; i < 10; i += 2) {
        if (addElement(&al2, al1.arrInt[i]) != 0) {
            printf("Error al copiar elemento de al1 a al2 en posición %d\n", i);
        }
    }
    
    for (int i = al2.arrSize, j = 0; i < 10 && j < 5; i++, j++) {
        if (addElement(&al2, j) != 0) {
            printf("Error al añadir valor %d en al2\n", j);
        }
    }
    
    printArr(&al2);
    return 0;
}
