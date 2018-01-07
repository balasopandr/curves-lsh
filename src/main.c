#include <stdio.h>
#include <stdlib.h>
#include <lapacke/lapacke.h>
#include "lapacke_helpers.h"
#include "lapacke_helpers.h"
#include "helper_functions.h"
#include "global_defs.h"

unsigned int dimension = 2;

int main(int argc, char **argv)
{
	// double my_arr[] = {1, 1, 5, 2, 1, 6, 3, 4, 0};
	// double my_arr2[] = {1, 0, 5, 2, 1, 6, 3, 4, 0};
	double my_arr3[] = {1,0,0,0, 0,0,0,2, 0,3,0,0, 0,0,0,0, 2,0,0,0};
	// double my_arr4[] = {1,1,0,0,1,1,1,2,1};
	// print_array_col_major(my_arr4, 3, 3, stdout);
	// printf("\n");
	// array_print(my_arr4, 3, 3, COL_MAJOR, stdout);

	double *U = NULL;
	double *S = NULL;
	double *V = NULL;
	// LAPACKE_inverse(my_arr, 3);
	// double *result = LAPACKE_malloc(sizeof(double)*3*3);
// 
	// LAPACKE_multiplication(my_arr, 3, my_arr2, 3, 3, result);
// 
	// print_array_col_major(result, 3, 3, stdout);

	// print_array_col_major(my_arr, 3, 3, stdout);
	// double det = LAPACKE_determinant(my_arr, 3);
	// printf("det: %f\n", det);
	
	double *copy = array_transpose(my_arr3, 4, 5);
	printf("INITIAL\n");
	array_print(copy, 5, 4, COL_MAJOR, stdout);
	LAPACKE_svd(copy, 5, 4, &U, &S, &V);

	/* TODO: unit test multiply */
	// double pinakas1[] = {1,2,3, 4,5,6};
	// double pinakas2[] = {1,2, 3,4};
	// double *result = NULL;
	// LAPACKE_multiplication(pinakas1, 3, 2, pinakas2, 2, 2, &result);
	// array_print(result, 3,2, COL_MAJOR, stdout);
	// LAPACKE_free(result);
	// result = NULL;

	/* TODO: unit test SVD */
	// printf("U 5,5\n");
	// array_print(U, 5, 5, COL_MAJOR, stdout);
	// printf("S 1, 4\n");
	// array_print(S, 1, 4, COL_MAJOR, stdout);
	// printf("V 4,4 \n");
	// array_print(V, 4, 4, COL_MAJOR, stdout);
// 
	// double *trans = array_transpose(V, 4, 4);
	// printf("transpose V 4,4\n");
	// array_print(trans, 4,4,COL_MAJOR,stdout);
// 
	// double *realS = malloc(sizeof(double)*20);
	// for(int i=0; i<4; i++)
	// {
		// for(int j=0; j<5; j++)
		// {
			// realS[i*5 + j] = 0.0;
			// if(i==j)
			// {
				// realS[i*5 + j] = S[j];
			// }
		// }
	// }


	// printf("realS 5,4\n");
	// array_print(realS, 5,4,COL_MAJOR,stdout);
// 
	// double *result = NULL;
	// LAPACKE_multiplication(U, 5, 5, realS, 5, 4, &result);
// 
	// printf("RESULT \n");
	// array_print(result, 5, 4, COL_MAJOR, stdout);
// 
	// double *new_result = NULL;
	// LAPACKE_multiplication(result, 5, 4, trans, 4, 4, &new_result);
// 
	// printf("CHECK \n");
	// array_print(new_result, 5, 4, COL_MAJOR, stdout);
// 
	// LAPACKE_free(new_result);
	// new_result = NULL;
	// LAPACKE_free(result);
	// result = NULL;
	free(copy);
	copy = NULL;
	free(trans);
	trans = NULL;
	free(realS);
	realS = NULL;
	LAPACKE_free(U);
	U = NULL;
	LAPACKE_free(S);
	S = NULL;
	LAPACKE_free(V);
	V = NULL;
	return 0;
}
