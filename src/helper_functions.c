#include <stdlib.h>
#include <stdarg.h>
#include "helper_functions.h"
#include "global_defs.h"

extern unsigned int dimension;

double uniform_random(double min, double max)
{
	return min + (rand() / (RAND_MAX + 1.0))*(max - min);
}

int uniform_random_int(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

/* returns random vectors with uniform distribution */
double ** create_urandom_vectors(unsigned int count, double min, double max)
{
	int i;
	int j;
	double **vectors = malloc(sizeof(double *) *count);
	for(i=0; i<count; i++)
	{
		vectors[i] = malloc(sizeof(double) * dimension);
		for(j=0; j<dimension; j++)
		{
			vectors[i][j] = uniform_random(min, max);
		}
	}
	return vectors;
}

FILE * open_or_die(char *filename, char *mode, char *errmsg)
{
	FILE *file = fopen(filename, mode);
	if(file == NULL)
	{
		perror(errmsg);
		exit(EXIT_FAILURE);
	}
	return file;
}

double max_double_array(double *array, unsigned int num_of_elements, int *arg_max)
{
	int i;
	double maximum = array[0];
	*arg_max = 0;
	for(i=1; i<num_of_elements; i++)
	{
		if(array[i] > maximum)
		{
			maximum = array[i];
			*arg_max = i;
		}
	}
	return maximum;
}

/* returns the position of the element relative to the interval
 * left, right, or in the interval */
int in_interval(double lower, double upper, double element)
{
	if(lower <= element && upper >= element)
	{
		return IN;
	}
	else if(element < lower)
	{
		return LEFT;
	}
	else if(element > upper)
	{
		return RIGHT;
	}
	else
	{
		return NOT_IN;
	}
}
