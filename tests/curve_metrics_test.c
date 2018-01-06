#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "../src/curve_metrics.h"
#include "../src/data_parser.h"
#include "../src/helper_functions.h"

extern unsigned int dimension;

char * frechet_identity_test(Curve P)
{
	double **distances;
	double dist = Frechet_distance(P, P, &distances);
	int i;
	for(i=0; i<Curve_get_points_count(P); i++)
	{
		free(distances[i]);
		distances[i] = NULL;
	}
	free(distances);
	distances = NULL;
	mu_assert("identity property is not satisfied", dist == 0.0);
	return NULL;
}

char * frechet_non_negativity_test(Curve P, Curve Q)
{

	double **distances;
	double dist = Frechet_distance(P, Q, &distances);
	int i;
	for(i=0; i<Curve_get_points_count(P); i++)
	{
		free(distances[i]);
		distances[i] = NULL;
	}
	free(distances);
	distances = NULL;
	mu_assert("non-negativity property is not satisfied", dist >= 0.0);
	return NULL;
}

char * frechet_symmetry_test(Curve P, Curve Q)
{
	double **distances;
	double dist1 = Frechet_distance(P, Q, &distances);
	int i;
	for(i=0; i<Curve_get_points_count(P); i++)
	{
		free(distances[i]);
		distances[i] = NULL;
	}
	free(distances);
	distances = NULL;
	double dist2 = Frechet_distance(Q, P, &distances);
	for(i=0; i<Curve_get_points_count(Q); i++)
	{
		free(distances[i]);
		distances[i] = NULL;
	}
	free(distances);
	distances = NULL;
	mu_assert("symmetry property is not satisfied", dist1 == dist2);
	return NULL;
}

char * frechet_triangle_inequality(Curve P, Curve Q, Curve S)
{
	double **distances;
	double dist_pq = Frechet_distance(P, Q, &distances);
	int i;
	for(i=0; i<Curve_get_points_count(P); i++)
	{
		free(distances[i]);
		distances[i] = NULL;
	}
	free(distances);
	distances = NULL;
	double dist_ps = Frechet_distance(P, S, &distances);
	for(i=0; i<Curve_get_points_count(P); i++)
	{
		free(distances[i]);
		distances[i] = NULL;
	}
	free(distances);
	distances = NULL;
	double dist_qs = Frechet_distance(Q, S, &distances);
	for(i=0; i<Curve_get_points_count(Q); i++)
	{
		free(distances[i]);
		distances[i] = NULL;
	}
	free(distances);
	distances = NULL;
	mu_assert("triangle inequality property is not satisfied", dist_pq <= dist_ps + dist_qs);
	return NULL;
}
