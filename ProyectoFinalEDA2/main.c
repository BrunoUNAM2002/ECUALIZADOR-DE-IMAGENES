#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"
#define N 256

void llenaArreglo(int* a) {
    for (int i = 0; i < N; i++) {
        a[i] = 0;
    }
}

void obtenerhistograma( unsigned char *pixeles, int ancho, int alto, int *a, int canales) {
    int i, j, k;

    for (i = 0; i < ancho; i++) {
        for (j = 0; j < alto; j++) {
            int indice = (j * ancho + i) * canales;
            int valor = pixeles[indice];
            a[valor]++;
        }
    }
}

void generarCDF(int* a, int* cdf) {
    cdf[0] = a[0];
    for (int i = 1; i < N; i++) {
        cdf[i] = a[i] + cdf[i - 1];
    }
}

int CDFMin(int* cdf) {
    int cdfMin = INT_MAX;
    for (int i = 0; i < N; i++) {
        if (cdf[i] != 0 && cdf[i] < cdfMin) {
            cdfMin = cdf[i];
        }
    }
    return cdfMin;
}

void EqCDF(int* cdf, int* eqCdf, int totalPixeles) {
    int cdfMin = CDFMin(cdf);
    for (int i = 0; i < N; i++) {
        double h = round((((double)(cdf[i] - cdfMin) / (totalPixeles - cdfMin)) * (N - 2)) + 1);
        eqCdf[i] = (int)h;
    }
}

unsigned char* EqImage(int* eqCdf, unsigned char* pixeles, int alto, int ancho) {
    int tam = alto * ancho;
    unsigned char *eqIma = (unsigned char*)malloc(tam * sizeof(unsigned char));
    for (int i = 0; i < tam; i++) {
        eqIma[i] = eqCdf[pixeles[i]];
    }
    return eqIma;
}

void generarArchivoCSV(  int *histograma,  int *eqHistograma) {
    FILE *file = fopen("ProyectoFinalEDA2/output/histo_secuencial.csv", "w+");
    fprintf(file, "valor, histo, eqHisto\n");
    for (int i = 0; i < N; i++) {
        fprintf(file, "%d, %d, %d\n", i, histograma[i], eqHistograma[i]);
    }
    fclose(file);
}

int main() {
    int ancho, alto, canales;
    int* a = (int*)malloc(N * sizeof(int));
    llenaArreglo(a);
   // char *srcPath;
    printf("Agregue la ruta de la imagen: ");
    char *srcPath = "";
    //scanf("%s", &srcPath);
    unsigned char* pixeles = stbi_load("japan_gray_1_chan.jpg", &ancho, &alto, &canales, 0);

    if (pixeles == NULL) {
        printf("No se pudo cargar la imagen %s :(\n", srcPath);
        free(a);
        return 0;
    }
    else
  {
    printf("\nImagen cargada correctamente: %dx%d pixeles con %d canales.\n", ancho, alto, canales);
  }

    int totalPixeles = ancho * alto;

    obtenerhistograma(pixeles, ancho, alto, a, canales);

    int* cdf = (int*)malloc(N * sizeof(int));
    generarCDF(a, cdf);

//    printf("CDF:\n");
  //  for (int i = 0; i < N; i++) {
    //    printf("Valor %d: %d\n", i, cdf[i]);
    //}

    int cdfMin = CDFMin(cdf);
    if (cdfMin != -1) {
        printf("El valor mínimo del CDF es: %d\n", cdfMin);
    } else {
        printf("No se encontro un valor minimo.\n");
    }

    int* eqCdf = (int*)malloc(N * sizeof(int));
    EqCDF(cdf, eqCdf, totalPixeles);
    unsigned char* eqIma = EqImage(eqCdf, pixeles, ancho, alto);
    int* b = (int*)malloc(N * sizeof(int)); 
    obtenerhistograma(eqIma, ancho, alto, b, canales);
    stbi_write_jpg("ProyectoFinalEDA2/output/ima_eq_secuencial.jpg", ancho, alto, canales , eqIma, 100);
	if(b!=NULL)
	printf("Imagen Generada");
	
    generarArchivoCSV(a, b);

    free(pixeles);
    free(a);
    free(cdf);
    free(eqCdf);
    free(eqIma);
    free(b);
    return 0;
}
