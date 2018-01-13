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

double * array_copy(double *array, int size)
{
	double *copy = malloc(sizeof(double)*size);
	for(int i=0; i<size; i++)
	{
		copy[i] = array[i];
	}
	return copy;
}

double * array_transpose(double *array, int row, int col)
{
	double *transpose = malloc(sizeof(double)*row*col);
	int i, j;
	for(i=0; i<row; i++)
	{
		for(j=0; j<col; j++)
		{
			transpose[i*col + j] = array[j*row +i];
		}
	}
	return transpose;
}

void array_print(double *array, int row, int col, int major, FILE *stream)
{
	for(int i=0; i<row; i++)
	{
		fprintf(stream, "[");
		for(int j=0; j<col; j++)
		{
			int u;
			if(major == COL_MAJOR)
			{
				u = j*row + i;
			}
			else
			{
				u = i*col + j;
			}
			fprintf(stream, "%10.6f ", array[u]);
		}
		fprintf(stream, "]\n");
	}
}

void array_addition(double *arr1, double *arr2, int size, int a, int b)
{
	for(int i=0; i<size; i++)
	{
		arr1[i] = a*arr1[i] + b*arr2[i];
	}
}

double * convert_matrix(double **matrix, int rows, int cols)
{
	double *array = malloc(sizeof(double)*rows*cols);
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			int x = j*rows + i;
			array[x] = matrix[i][j];
		}
	}
	return array;
}
