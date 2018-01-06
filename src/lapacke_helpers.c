#include <openblas/cblas.h>
#include <lapacke/lapacke.h>
#include "lapacke_helpers.h"

void LAPACKE_inverse(double *array, unsigned int size)
{
	int ipiv[size];
	LAPACKE_dgetrf(LAPACK_COL_MAJOR, size, size, array, size, ipiv);
	LAPACKE_dgetri(LAPACK_COL_MAJOR, size, array, size, ipiv);
}

void LAPACKE_multiplication(double *matA, unsigned int rowsA, double *matB, unsigned int rowsB, unsigned int colsB, double *result)
{
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, rowsA, colsB, rowsB, 1.0, matA, rowsB, matB, colsB, 0.0, result, colsB);
}
