#include <stdio.h>
#include <math.h>
#include "mean_curve.h"
#include "minunit.h"
#include "../src/free_memory.h"
#include "../src/curve_metrics.h"
#include "../src/curve.h"
#include "../src/pair_int.h"

void pair_print(void *pair, FILE *stream)
{
	PairInt_print((PairInt) pair, stream);
}

char * MeanCurve_distance_check(Curve P, Curve Q)
{
	double **distances;
	double pq_dist = Frechet_distance(P, Q, &distances);
	void del_func(void *par)
	{
		PairInt_delete((PairInt)par);
	}

	/* calculate mean curve */
	List traversal = OptimalTraversal(Curve_get_points_count(P),
			Curve_get_points_count(Q), distances);
	Curve meanCurve = MeanFrechetCurve(P, Q, traversal);
	free_double_array(distances, Curve_get_points_count(P));
	distances = NULL;
	printf("%f <= %d <= %d\n", fmax(Curve_get_points_count(P),
				Curve_get_points_count(Q)), List_get_length(traversal),
			Curve_get_points_count(P) + Curve_get_points_count(Q));

	double pm_dist = Frechet_distance(P, meanCurve, &distances);
	free_double_array(distances, Curve_get_points_count(P));
	double qm_dist = Frechet_distance(Q, meanCurve, &distances);
	free_double_array(distances, Curve_get_points_count(Q));

	printf("MESSAGE: PQ: %f >= (PM: %f & QM: %f)\n", pq_dist, pm_dist, qm_dist);

	List_destroy(traversal, &del_func);
	Curve_delete(meanCurve);
	mu_assert("MeanCurve distance check was unsuccessful",
			(pq_dist >= pm_dist) && (pq_dist >= qm_dist));
	return NULL;
}
