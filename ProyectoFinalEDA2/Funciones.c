#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Funciones.h"
#include <math.h>

#define n 256

// Declaración de funciones
void llenaArreglo(int* a);
void obtenerHistograma(unsigned char* pixeles, int* a, int totalPixeles);
void generarCDF(int* a, int* cdf);
int CDFMin(int* cdf);
void EqCDF(int* cdf, int* eqCdf, int totalPixeles);
unsigned char* EqImage(int* eqCdf, unsigned char* pixeles, int alto, int ancho);
void generarArchivoCSV(int* a, int* eqCdf);

void llenaArreglo(int *a) {
    int i;
    printf("Arreglo: ");
    for (i = 0; i < n; i++) {
        a[i] = 0;
    }
}

void obtenerHistograma(unsigned char* pixeles, int* a, int totalPixeles) {
    // Obtener el histograma
    for (int i = 0; i < totalPixeles; i++) {
        unsigned char pixel = pixeles[i]; // Obtener el valor del pixel

        a[pixel]++;
    }
}

void generarCDF(int* a, int* cdf) {
    // Generar el CDF
    cdf[0] = a[0];
    for (int i = 1; i < n; i++) {
        cdf[i] = a[i] + cdf[i - 1];
    }
}

int CDFMin(int* cdf) {
    int cdfMin = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (cdf[i] != 0 && cdf[i] < cdfMin) {
            cdfMin = cdf[i];
        }
    }
    return cdfMin;
}

void EqCDF(int* cdf, int* eqCdf, int totalPixeles) {
    int cdfMin = CDFMin(cdf);
    for (int i = 0; i < n; i++) {
        double h = round((((double)(cdf[i] - cdfMin) / (totalPixeles - cdfMin)) * (n - 2)) + 1);
        eqCdf[i] = (int)h;
    }
}

unsigned char* EqImage(int* eqCdf, unsigned char* pixeles, int alto, int ancho) {
    int tam = alto * ancho;
    unsigned char* eqIma = (unsigned char*)malloc(tam * sizeof(unsigned char));
    for (int i = 0; i < tam; i++) {
        eqIma[i] = eqCdf[pixeles[i]];
    }

    return eqIma;
}

void generarArchivoCSV(int* a, int* eqCdf) {
    FILE *csvFile = fopen("file.csv", "w+"); // <- Cuidado si no existe la carpate
    if (csvFile == NULL) {
        printf("Error al abrir el archivo CSV.\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(csvFile, "%d,%d,%d\n", i, a[i], eqCdf[i]);
    }
    fclose(csvFile);
}

