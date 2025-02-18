#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define BASE_DIR "miServidorWebDir"

// APARTADO 1
void crear_directorio(const char *nombre) {
    char ruta[256];
    snprintf(ruta, sizeof(ruta), "%s/%s", getenv("HOME"), nombre);
    
    if (mkdir(ruta, 0755) == -1) {
        perror("Error al crear el directorio");
        exit(-1);
    }
    printf("Directorio creado: %s\n", ruta);
    exit(0);
}

void eliminar_directorio(const char *nombre) {
    char ruta[256];
    snprintf(ruta, sizeof(ruta), "%s/%s", getenv("HOME"), nombre);
    
    if (rmdir(ruta) == -1) {
        perror("Error al eliminar el directorio");
        exit(-2);
    }
    printf("Directorio eliminado: %s\n", ruta);
    exit(0);
}

// APARTADO 3
void *eliminar_archivo(void *arg) {
    char *ruta = (char *)arg;
    if (unlink(ruta) == -1) {
        perror("Error al eliminar el archivo");
        pthread_exit((void *)-5);
    }
    printf("Archivo eliminado: %s\n", ruta);
    pthread_exit((void *)0);
}

void *copiar_archivo(void *arg) {
    char **rutas = (char **)arg;
    char buffer[1024];
    int fd_origen, fd_destino, n;
    
    fd_origen = open(rutas[0], O_RDONLY);
    if (fd_origen == -1) {
        perror("Error al abrir archivo de origen");
        pthread_exit((void *)-7);
    }
    
    fd_destino = open(rutas[1], O_WRONLY | O_CREAT, 0644);
    if (fd_destino == -1) {
        perror("Error al crear archivo de destino");
        close(fd_origen);
        pthread_exit((void *)-7);
    }
    
    while ((n = read(fd_origen, buffer, sizeof(buffer))) > 0) {
        write(fd_destino, buffer, n);
    }
    
    close(fd_origen);
    close(fd_destino);
    printf("Archivo copiado de %s a %s\n", rutas[0], rutas[1]);
    pthread_exit((void *)0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso incorrecto\n");
        return -1;
    }
    
    // ORDEN 1
    if (strcmp(argv[1], "addDir") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            crear_directorio(argv[2]);
        } else {
            wait(NULL);
        }
    }
    
    // ORDEN 2
    else if (strcmp(argv[1], "rmDir") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            eliminar_directorio(argv[2]);
        } else {
            wait(NULL);
        }
    }
    
    // ORDEN 3
    else if (strcmp(argv[1], "addArchivo") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            execlp("cp", "cp", argv[2], argv[3], NULL);
        }
    }
    
    // ORDEN 4
    else if (strcmp(argv[1], "addArchivoCeros") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            char ruta[256];
            snprintf(ruta, sizeof(ruta), "%s/%s/%s", getenv("HOME"), BASE_DIR, argv[2]);
            int fd = open(ruta, O_WRONLY | O_CREAT, 0644);
            if (fd == -1) {
                perror("Error al crear archivo");
                exit(-4);
            }
            int size = atoi(argv[3]);
            char *zeros = calloc(size, sizeof(char));
            write(fd, zeros, size);
            free(zeros);
            close(fd);
            printf("Archivo de ceros creado: %s\n", ruta);
            exit(0);
        }
    }
    
    // ORDEN 5
    else if (strcmp(argv[1], "rmArchivo") == 0) {
        pthread_t hilo;
        char ruta[256];
        snprintf(ruta, sizeof(ruta), "%s/%s/%s", getenv("HOME"), BASE_DIR, argv[2]);
        pthread_create(&hilo, NULL, eliminar_archivo, (void *)ruta);
        pthread_join(hilo, NULL);
    }
    
    // ORDEN 6
    else if (strcmp(argv[1], "getArchivo") == 0) {
        pthread_t hilo;
        char *rutas[2];
        rutas[0] = argv[2];
        rutas[1] = argv[3];
        pthread_create(&hilo, NULL, copiar_archivo, (void *)rutas);
        pthread_join(hilo, NULL);
    }
    else {
        fprintf(stderr, "Orden no reconocida\n");
        return -1;
    }
    
    return 0;
}
