#ifndef __LAPACKE_HELPERS__
#define __LAPACKE_HELPERS__

void LAPACKE_inverse(double *array, unsigned int size);
void LAPACKE_multiplication(double *matA, unsigned int rowsA, unsigned int colsA,
		double *matB, unsigned int rowsB, unsigned int colsB, double **result);
double LAPACKE_determinant(double *array, int size);
int LAPACKE_svd(double *array, int rows, int cols, double **U,
		double **S, double **V);
#endif
