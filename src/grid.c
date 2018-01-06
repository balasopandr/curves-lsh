#include <stdlib.h>
#include <math.h>
#include "grid.h"
#include "vector.h"

extern unsigned int dimension;

static bool duplicate_point(double *point1, double *point2);

/* projects a curve on a given grid */
Vector Grid_curve_project(double grid_density, double *shift_value, Curve curve, int *max_points)
{
	int i;
	double **vector_points;
	unsigned int points_count = Curve_get_points_count(curve);
	double **points = Curve_get_points(curve);

	vector_points = malloc(sizeof(double *)* points_count);
	for(i=0; i<points_count; i++)
	{
		vector_points[i] = NULL;
	}

	/* the first point can't have a duplicate */
	vector_points[0] = Grid_point_project(grid_density, shift_value, points[0]);
	double *prev = vector_points[0];
	double *grid_point;
	int j = 1;
	for(i=1; i<points_count; i++)
	{
		grid_point = Grid_point_project(grid_density, shift_value, points[i]);
		if(duplicate_point(grid_point, prev))
		{
			/* don't store duplicates */
			free(grid_point);
			grid_point = NULL;
		}
		else
		{
			vector_points[j] = grid_point;
			j++;
			prev = grid_point;
		}
	}
	int vector_points_count = j;
	if(vector_points_count > *max_points)
	{
		*max_points = vector_points_count;
	}
	vector_points = realloc(vector_points, sizeof(double *) * vector_points_count);
	return Vector_init(vector_points, vector_points_count);
}

/* projects a point on a given grid */
double * Grid_point_project(double grid_density, double *shift_value, double *point)
{
	int j;
	double *grid_point = malloc(sizeof(double) * dimension);
	for(j=0; j<dimension; j++)
	{
		grid_point[j] = (round((point[j] - shift_value[j])/grid_density))*grid_density;
	}

	return grid_point;
}

/* check for duplucate points */
static bool duplicate_point(double *point1, double *point2)
{
	int j;
	for(j=0; j<dimension; j++)
	{
		if(point1[j] != point2[j])
		{
			return false;
		}
	}
	return true;
}

/* builds k-grid vectors and returns the concatenated vector */
Vector Grid_build_vector(double grid_density, double **shift_value, Curve curve, unsigned int k, int *max_points)
{
	int i;
	Vector grid_vector;
	grid_vector = Grid_curve_project(grid_density, shift_value[0], curve, max_points);
	for(i=1; i<k; i++)
	{
		Vector_concat(grid_vector, Grid_curve_project(grid_density, shift_value[i], curve, max_points));
	}
	return grid_vector;
}
