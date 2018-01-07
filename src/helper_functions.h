#ifndef __HELPER__FUNCTIONS__
#define __HELPER__FUNCTIONS__
#include <stdio.h>
#include <stdint.h>

double uniform_random(double min, double max);
double ** create_urandom_vectors(unsigned int count, double min, double max);
FILE * open_or_die(char *filename, char *mode, char *errmsg);
int uniform_random_int(int min, int max);
double max_double_array(double *array, unsigned int num_of_elements, int *arg_max);
int in_interval(double lower, double upper, double element);
double * array_copy(double *array, int size);
double * array_transpose(double *array, int row, int col);
void array_print(double *array, int row, int col, int major, FILE *stream);

/* seperates a double into mantisa, exponent and sign parts */
typedef union
{
	double d;
	struct
	{
		uint64_t mantisa : 52;
		uint64_t exponent : 11;
		uint64_t sign : 1;
	} parts;
} double_cast;

#endif
