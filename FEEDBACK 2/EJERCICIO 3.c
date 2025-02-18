#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// Apartado 1
void concatenar_archivos(const char *directorio, const char *archivo1, const char *archivo2, const char *archivo_salida) {
    char ruta1[512], ruta2[512], ruta_salida[512];
    snprintf(ruta1, sizeof(ruta1), "%s/%s", directorio, archivo1);
    snprintf(ruta2, sizeof(ruta2), "%s/%s", directorio, archivo2);
    snprintf(ruta_salida, sizeof(ruta_salida), "%s/%s", directorio, archivo_salida);

    int fd1 = open(ruta1, O_RDONLY);
    if (fd1 < 0) {
        perror("Error abriendo archivo 1");
        exit(EXIT_FAILURE);
    }

    int fd2 = open(ruta2, O_RDONLY);
    if (fd2 < 0) {
        perror("Error abriendo archivo 2");
        close(fd1);
        exit(EXIT_FAILURE);
    }

    int fd_salida = open(ruta_salida, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_salida < 0) {
        perror("Error creando archivo de salida");
        close(fd1);
        close(fd2);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_leidos;

    while ((bytes_leidos = read(fd1, buffer, BUFFER_SIZE)) > 0) {
        write(fd_salida, buffer, bytes_leidos);
    }

    while ((bytes_leidos = read(fd2, buffer, BUFFER_SIZE)) > 0) {
        write(fd_salida, buffer, bytes_leidos);
    }

    printf("Archivos concatenados en %s\n", ruta_salida);

    close(fd1);
    close(fd2);
    close(fd_salida);
}

// Apartado 2
void listar_archivos(const char *directorio) {
    DIR *dir = opendir(directorio);
    if (!dir) {
        perror("Error abriendo directorio");
        exit(EXIT_FAILURE);
    }

    struct dirent *entrada;
    struct stat info;
    char ruta_completa[512];

    printf("Listado de archivos en %s:\n", directorio);
    while ((entrada = readdir(dir)) != NULL) {
        if (entrada->d_name[0] == '.') continue; 

        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", directorio, entrada->d_name);
        if (stat(ruta_completa, &info) == 0) {
            printf("Archivo: %s, Inodo: %lu, Enlaces duros: %lu\n", 
                    entrada->d_name, info.st_ino, info.st_nlink);
        } else {
            perror("Error obteniendo información del archivo");
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc == 5) {
        // Apartado 1
        concatenar_archivos(argv[1], argv[2], argv[3], argv[4]);
    } else if (argc == 2) {
        // Apartado 2
        listar_archivos(argv[1]);
    } else {
        fprintf(stderr, "Uso:\n");
        fprintf(stderr, "%s <directorio> <archivo1> <archivo2> <archivo_salida> (para concatenación)\n", argv[0]);
        fprintf(stderr, "%s <directorio> (para listar archivos e inodos)\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
