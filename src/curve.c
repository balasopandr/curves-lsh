#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "curve.h"

extern unsigned int dimension;

struct c
{
	char *id;
	unsigned int points_count;		
	double **points;
	unsigned int cluster_idx;
	double cluster_dist;
	int curve_idx;
};

Curve Curve_create(char *id, unsigned int points_count, double **points, int curve_idx)
{
	Curve curve = NULL;

	curve = malloc(sizeof(struct c));

	/* make a hard copy because memory of the string id will be free'd */
	curve->id = malloc(sizeof(char)*(strlen(id)+1));
	strncpy(curve->id, id, strlen(id)+1);
	curve->points_count = points_count;
	/* no hard copy because of large amount of points */
	curve->points = points;
	curve->cluster_idx = -1;
	curve->cluster_dist = INFINITY;
	curve->curve_idx = curve_idx;

	return curve;
}

void Curve_print(Curve curve, FILE *stream)
{
	fprintf(stream, "%s\t%d\t", curve->id, curve->points_count);
	int i;
	int j;
	for(i=0; i<curve->points_count; i++)
	{
		if(i!=0)
		{
			fprintf(stream, ", ");
		}
		fprintf(stream, "(");
		for(j=0; j<dimension; j++)
		{
			if(j!=0)
			{
				fprintf(stream, ", ");
			}
			fprintf(stream, "%f", curve->points[i][j]);
		}
		fprintf(stream, ")");
	}
}

void Curve_delete(Curve curve)
{
	free(curve->id);
	curve->id = NULL;
	int i;
	for(i=0; i<curve->points_count; i++)
	{
		free(curve->points[i]);
		curve->points[i] = NULL;
	}
	free(curve->points);
	curve->points = NULL;

	free(curve);
	curve = NULL;
}

unsigned int Curve_get_points_count(Curve curve)
{
	return curve->points_count;
}
char * Curve_get_id(Curve curve)
{
	return curve->id;
}
double ** Curve_get_points(Curve curve)
{
	return curve->points;
}

unsigned int Curve_get_cluster_idx(Curve curve)
{
	return curve->cluster_idx;
}

int Curve_get_curve_idx(Curve curve)
{
	return curve->curve_idx;
}

void Curve_set_cluster_idx(Curve curve, unsigned int cluster_idx)
{
	curve->cluster_idx = cluster_idx;
}

double Curve_get_cluster_dist(Curve curve)
{
	return curve->cluster_dist;
}

void Curve_set_cluster_dist(Curve curve, double cluster_dist)
{
	curve->cluster_dist = cluster_dist;
}

bool Curve_is_mean(Curve curve)
{
	return strncmp(Curve_get_id(curve), "mean curve", strlen("mean curve")) == 0;
}

void Curve_del_mean_curve(Curve curve)
{
	if(Curve_is_mean(curve))
	{
		Curve_delete(curve);
	}
}

Curve Curve_copy(Curve curve)
{
	Curve new_curve = NULL;
	new_curve = malloc(sizeof(struct c));

	new_curve->id = malloc(sizeof(char)*(strlen(curve->id)+1));
	strncpy(new_curve->id, curve->id, strlen(curve->id)+1);
	new_curve->points_count = curve->points_count;
	new_curve->cluster_idx = curve->cluster_idx;
	new_curve->cluster_dist = curve->cluster_dist;
	new_curve->curve_idx = curve->curve_idx;

	if(new_curve->curve_idx != -1)
	{
		new_curve->points = curve->points;
	}
	else
	{
		int i,j;
		new_curve->points = malloc(sizeof(double *)*new_curve->points_count);
		for(i=0; i<new_curve->points_count; i++)
		{
			new_curve->points[i] = malloc(sizeof(double) * dimension);
			for(j=0; j<dimension; j++)
			{
				new_curve->points[i][j] = curve->points[i][j];
			}
		}
	}
	return new_curve;
}
