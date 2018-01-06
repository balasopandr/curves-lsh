#include <stdio.h>
#include <stdlib.h>
#include "lapacke_helpers.h"
#include "helper_functions.h"

unsigned int dimension = 2;

int main(int argc, char **argv)
{
	double my_arr[] = {1, 0, 5, 2, 1, 6, 3, 4, 0};
	double my_arr2[] = {1, 0, 5, 2, 1, 6, 3, 4, 0};
	LAPACKE_inverse(my_arr, 3);
	double *result = NULL;

	LAPACKE_multiplication(my_arr, 3, my_arr2, 3, 3, result);

	print_array_col_major(result, 3, 3, stdout);
	return 0;
}
