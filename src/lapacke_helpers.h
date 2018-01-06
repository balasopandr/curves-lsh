#ifndef __LAPACKE_HELPERS__
#define __LAPACKE_HELPERS__

void LAPACKE_inverse(double *array, unsigned int size);
void LAPACKE_multiplication(double *matA, unsigned int rowsA, double *matB, unsigned int rowsB, unsigned int colsB, double *result);
#endif
