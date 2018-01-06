#include <stdlib.h>
#include <math.h>
#include "nearest_curve.h"
#include "curve_metrics.h"
#include "hash_data.h"
#include "grid.h"

extern unsigned int dimension;

/* bruteforce algorithm for nearest neighbor problem */
Curve NearestCurve_bruteforce(List curve_list, Curve query, double (* curve_metric)(Curve, Curve), double *min_dist)
{
	*min_dist = INFINITY;
	Curve min_curve = NULL;
	Node curnode = List_get_first_node(curve_list);
	while(curnode != NULL)
	{
		Curve cur_curve = (Curve) Node_get_data(curnode);
		double cur_dist = curve_metric(cur_curve, query);
		if(cur_dist < *min_dist)
		{
			*min_dist = cur_dist;
			min_curve = cur_curve;
		}
		curnode = Node_get_next(curnode);
	}
	return min_curve;
}

/* LSH algorithm for nearest neighbor problem */
Curve NearestCurve_LSH(HashTable *hashes, Curve query, Params parameters, unsigned int *rand_for_hash, double ***shift_values, double *min_dist, bool *found_vector)
{
	int i;
	*min_dist = INFINITY;
	Curve min_curve = NULL;
	*found_vector = false;
	int max_vector_points = 0;

	HashData *query_hash = malloc(sizeof(HashData) * parameters->number_of_hash_tables);
	for(i=0; i<parameters->number_of_hash_tables; i++)
	{
		Vector grid_vector = Grid_build_vector(parameters->grid_density,
				shift_values[i],
				query,
				parameters->number_of_grid_curves,
				&max_vector_points);
		query_hash[i] = HashData_init(query, grid_vector);

		/* find hash for query */
		HashData_set_hash_value(query_hash[i], classic_hash((void *) query_hash[i], (void *) rand_for_hash));
		unsigned int bucket_loc = HashData_get_hash_value(query_hash[i])%HashTable_get_bucket_count(hashes[i]);

		/* go to the backet and search for the same vector */
		List bucket = HashTable_get_buckets(hashes[i])[bucket_loc];
		Node curbucket = List_get_first_node(bucket);
		while(curbucket != NULL)
		{
			HashData hdata = (HashData) Node_get_data(curbucket);
			/* if an equal vector is found, calculate the distance */
			if(Vector_equal(HashData_get_grid_vector(hdata), HashData_get_grid_vector(query_hash[i])))
			{
				*found_vector = true;
				double **distances = NULL;
				double dist = parameters->metric_func(HashData_get_curve(hdata),
						HashData_get_curve(query_hash[i]), &distances);
				int m;
				for(m=0; m<Curve_get_points_count(HashData_get_curve(query_hash[i])); m++)
				{
					free(distances[m]);
					distances[m] = NULL;
				}
				free(distances);
				distances = NULL;
				if(dist < *min_dist)
				{
					*min_dist = dist;
					min_curve = HashData_get_curve(hdata);
				}
			}
			curbucket = Node_get_next(curbucket);
		}
	}

	/* if no equal vector has been found then calculate the distance from all curves in the bucket and pick the minimum */
	if(min_curve == NULL)
	{
		*found_vector = false;
		for(i=0; i<parameters->number_of_hash_tables; i++)
		{
			/* go to the backet */
			List bucket = HashTable_get_buckets(hashes[i])[HashData_get_hash_value(query_hash[i])% HashTable_get_bucket_count(hashes[i])];
			Node curbucket = List_get_first_node(bucket);
			while(curbucket != NULL)
			{
				HashData hdata = (HashData) Node_get_data(curbucket);
				/* find minimum distance */
				double **distances;
				double dist = parameters->metric_func(HashData_get_curve(hdata),
						HashData_get_curve(query_hash[i]), &distances);
				int m;
				for(m=0; m<Curve_get_points_count(HashData_get_curve(query_hash[i])); m++)
				{
					free(distances[m]);
					distances[m] = NULL;
				}
				free(distances);
				distances = NULL;
				if(dist < *min_dist)
				{
					*min_dist = dist;
					min_curve = HashData_get_curve(hdata);
				}
				curbucket = Node_get_next(curbucket);
			}
		}
	}
	for(i=0; i<parameters->number_of_hash_tables; i++)
	{
		HashData_destroy(query_hash[i]);
		query_hash[i] = NULL;
	}
	free(query_hash);
	query_hash = NULL;
	return min_curve;
}
