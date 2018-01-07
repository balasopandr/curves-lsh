#include <openblas/cblas.h>
#include <lapacke/lapacke.h>
#include "lapacke_helpers.h"
#include "helper_functions.h"
#include "global_defs.h"

void LAPACKE_inverse(double *array, unsigned int size)
{
	int ipiv[size];
	LAPACKE_dgetrf(LAPACK_COL_MAJOR, size, size, array, size, ipiv);
	LAPACKE_dgetri(LAPACK_COL_MAJOR, size, array, size, ipiv);
}

void LAPACKE_multiplication(double *matA, unsigned int rowsA, unsigned int colsA,
		double *matB, unsigned int rowsB, unsigned int colsB, double **result)
{
	*result = LAPACKE_malloc(sizeof(double)*rowsA*colsB);
	int lda = max(rowsA, colsA);
	int ldb = max(rowsB, colsB);
	int ldc = max(rowsA, colsB);
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, rowsA, colsB, colsA,
			1.0, matA, lda, matB, ldb, 0.0, *result, ldc);
}

double LAPACKE_determinant(double *array, int size)
{
	int ipiv[size];
	int info;
	LAPACK_dgetrf(&size, &size, array, &size, ipiv, &info);
	double det = 0.0;
	if(info != 0)
	{
		return det;
	}
	det = 1.0;
	for(int i=0; i<size; i++)
	{
		if(ipiv[i] != i+1)
		{
			det = -det*array[i*size +i];
		}
		else
		{
			det = det*array[i*size +i];
		}
	}
	return det;
}

int LAPACKE_svd(double *array, int rows, int cols, double **U,
		double **S, double **V)
{
	lapack_int info, ldarray, ldv;

	ldarray = max(rows, cols);
	ldv = cols;
	*U = LAPACKE_malloc(sizeof(double)*rows*rows);
	for(int i=0; i<rows*rows; i++)
	{
		if(i<rows*cols)
		{
			(*U)[i] = array[i];
		}
		else
		{
			(*U)[i] = 0.0;
		}
	}
	// array_print(*U, rows, cols, COL_MAJOR, stdout);
	*S = LAPACKE_malloc(sizeof(double)*cols);
	*V = LAPACKE_malloc(sizeof(double)*ldv*ldv);
	for(int i=0; i<ldv*ldv;i++)
	{
		(*V)[i] = 0.0;
	}
	double *stat = LAPACKE_malloc(sizeof(double)*max(rows+cols, 6));

	info = LAPACKE_dgesvj(LAPACK_COL_MAJOR, 'G', 'U', 'V', rows, cols, *U, ldarray, *S, ldv, *V, ldv, stat);

	LAPACKE_free(stat);
	stat = NULL;
	return info;
}
