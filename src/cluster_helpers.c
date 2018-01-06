#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cluster_helpers.h"
#include "helper_functions.h"
#include "global_defs.h"

/* finds in which range [P(i-1), P(i)] the element is
 * and returns the index */
unsigned int interval_binary_search(Cluster *clusters, unsigned int num_of_clusters,
		void **points, unsigned int num_of_points, PossibleMedoid *array,
		double element, int left, int right)
{
	if(right >= left)
	{
		int mid = left + (right-left)/2;
		double lower, upper;
		if(mid == 0)
		{
			lower = 0.0;	
		}
		else
		{
			lower = array[mid-1].partial_sum;
		}
		upper = array[mid].partial_sum;

		int go = in_interval(lower, upper, element);
		if(go == LEFT)
		{
			return interval_binary_search(clusters, num_of_clusters,
					points, num_of_points, array, element, left, mid-1);
		}
		else if(go == RIGHT)
		{
			return interval_binary_search(clusters, num_of_clusters,
					points, num_of_points, array, element, mid+1, right);
		}
		else
		{
			return array[mid].idx;
		}
	}

	/* error: if this happens it means that the element does not belong in any range */
	return -1;
}

int find_min_cluster(Cluster *clusters, unsigned int num_of_clusters,
		void *point, double **dists, double *minimum_distance, 
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster)
{
	int i;
	double min_dist = INFINITY;
	int min_cluster = -1;
	for(i=0; i<num_of_clusters; i++)
	{
		double cur_dist;
		if(function_caster->dists_condition(Cluster_get_medoid(clusters[i])))
		{
			double **distances;
			cur_dist = function_caster->point_dist(point, Cluster_get_medoid(clusters[i]), &distances);
			garbage_collector(distances, function_caster->get_object_points(point));
			distances = NULL;
		}
		else
		{
			int point_idx = function_caster->get_object_index(point);
			cur_dist = dists[point_idx][Cluster_get_medoid_idx(clusters[i])];
		}
		if(cur_dist < min_dist)
		{
			min_dist = cur_dist;
			min_cluster = i;
		}
	}
	*minimum_distance = min_dist;
	return min_cluster;
}

double minimization_function(void **points, unsigned int number_of_points, 
		Cluster *clusters, Params parameters, double **dists,
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster)
{
	int i;
	double sum = 0;
	for(i=0; i<number_of_points; i++)
	{
		double minimum_distance;
		find_min_cluster(clusters, parameters->number_of_clusters,
				points[i], dists, &minimum_distance, garbage_collector,
				function_caster);
		sum += SQR(minimum_distance);
	}
	return sum;
}

int find_second_nearest_cluster(Cluster *clusters, unsigned int num_of_clusters,
		Cluster best_cluster, void *point, double **dists,
		double *minimum_distance, 
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster)
{
	int i;
	double min_dist = INFINITY;
	int second_nearest_cluster = -1;
	for(i=0; i<num_of_clusters; i++)
	{
		if(clusters[i] == best_cluster)
		{
			continue;
		}
		double cur_dist;
		if(function_caster->dists_condition(Cluster_get_medoid(clusters[i])))
		{
			double **distances;
			cur_dist = function_caster->point_dist(point, Cluster_get_medoid(clusters[i]), &distances);
			garbage_collector(distances, function_caster->get_object_points(point));
			distances = NULL;
		}
		else
		{
			int point_idx = function_caster->get_object_index(point);
			cur_dist = dists[point_idx][Cluster_get_medoid_idx(clusters[i])];
		}
		if(cur_dist < min_dist)
		{
			min_dist = cur_dist;
			second_nearest_cluster = i;
		}
	}
	*minimum_distance = min_dist;
	return second_nearest_cluster;
}
