#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

extern unsigned int dimension;

struct vec
{
	double **points;
	unsigned int length;
};

Vector Vector_init(double **points, unsigned int length)
{
	Vector vector = malloc(sizeof(struct vec));
	vector->points = points;
	vector->length = length;
	return vector;
}

void Vector_resize(Vector vector, unsigned int new_length)
{
	int i;
	int j;
	if(vector->length == new_length)
	{
		return;
	}
	else if(new_length > vector->length)
	{
		vector->points = realloc(vector->points, sizeof(double *)*new_length);
		/* initialize new points*/
		for(i=vector->length; i<new_length; i++)
		{
			vector->points[i] = malloc(sizeof(double)*dimension);
			for(j=0; j<dimension; j++)
			{
				vector->points[i][j] = 0.0;
			}
		}
	}
	else
	{
		/* free old points */
		for(i=new_length; i<vector->length; i++)
		{
			free(vector->points[i]);
			vector->points[i] = NULL;
		}
		vector->points = realloc(vector->points, sizeof(double *)*new_length);
	}
	vector->length = new_length;
}

/* Concatenates two vectors. Result is in the first vector */
void Vector_concat(Vector v1, Vector v2)
{
	v1->points = realloc(v1->points, sizeof(double *) * (v1->length + v2->length));
	int i;
	for(i=0; i<v2->length; i++)
	{
		v1->points[i + v1->length] = v2->points[i];
	}
	v1->length = v1->length + v2->length;
	free(v2->points);
	v2->points = NULL;
	free(v2);
	v2 = NULL;
}

void Vector_print(Vector vector, FILE *stream)
{
	int i;
	printf("Total points: %d\n", vector->length);
	fprintf(stream, "<");
	for(i=0; i<vector->length; i++)
	{
		if(i!=0)
		{
			fprintf(stream, ", ");
		}
		fprintf(stream, "(");
		int j;
		for(j=0; j<dimension; j++)
		{
			if(j!=0)
			{
				fprintf(stream, ", ");
			}
			fprintf(stream, "%f", vector->points[i][j]);
		}
		fprintf(stream, ")");
	}
	fprintf(stream, ">");
}

void Vector_destroy(Vector vector)
{
	int i;
	for(i=0; i<vector->length; i++)
	{
		free(vector->points[i]);
		vector->points[i] = NULL;
	}
	free(vector->points);
	vector->points = NULL;

	vector->length = 0;
	free(vector);
	vector = NULL;
}

bool Vector_equal(Vector v1, Vector v2)
{
	if(v1->length != v2->length)
	{
		return false;
	}

	int i;
	int j;
	for(i=0; i<v1->length; i++)
	{
		for(j=0; j<dimension; j++)
		{
			if(v1->points[i][j] != v2->points[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

unsigned int Vector_get_length(Vector vector)
{
	return vector->length;
}
double ** Vector_get_points(Vector vector)
{
	return vector->points;
}
