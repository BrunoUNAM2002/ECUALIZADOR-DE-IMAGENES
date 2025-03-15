#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

// Declaración de funciones
void llenaArreglo(int* a);
void obtenerHistograma(unsigned char* pixeles, int* a, int totalPixeles);
void generarCDF(int* a, int* cdf);
int CDFMin(int* cdf);
void EqCDF(int* cdf, int* eqCdf, int totalPixeles);
unsigned char* EqImage(int* eqCdf, unsigned char* pixeles, int alto, int ancho);
void generarArchivoCSV(int* a, int* eqCdf);

#endif // FUNCIONES_H_INCLUDED
