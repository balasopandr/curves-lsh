#include <stdlib.h>
#include <math.h>
#include "silhouette.h"
#include "cluster_helpers.h" 

double calculate_silhouette(Cluster *clusters, Params parameters,
		double **silhouette, double **dists, 
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster)
{
	int i;
	double average_silhouette = 0.0;
	(*silhouette) = malloc(sizeof(double)*(parameters->number_of_clusters));
	for(i=0; i<parameters->number_of_clusters; i++)
	{
		(*silhouette)[i] = 0.0;
		List points = Cluster_get_points(clusters[i]);	
		Node cur_node = List_get_first_node(points);
		while(cur_node != NULL)
		{
			double alpha = calculate_average(clusters[i], Node_get_data(cur_node),
					dists, function_caster);
			double minimum_distance;
			int idx = find_second_nearest_cluster(clusters, 
					parameters->number_of_clusters, clusters[i],
					Node_get_data(cur_node), dists, &minimum_distance,
					garbage_collector, function_caster);
			double beta = calculate_average(clusters[idx], Node_get_data(cur_node),
					dists, function_caster);
			(*silhouette)[i] += (beta - alpha)/fmax(alpha, beta);
			cur_node = Node_get_next(cur_node);
		}
		(*silhouette)[i] /= List_get_length(points);
		average_silhouette += (*silhouette)[i];
	}
	return average_silhouette / parameters->number_of_clusters;
}

double calculate_average(Cluster cluster, void *point, double **dists,
	FunctionCaster function_caster)
{
	List points = Cluster_get_points(cluster);
	Node cur_node = List_get_first_node(points);
	double average = 0.0;
	while(cur_node != NULL)
	{
		void *data = Node_get_data(cur_node);
		int point_idx = function_caster->get_object_index(point);
		int data_idx = function_caster->get_object_index(data);
		average += dists[point_idx][data_idx];
		cur_node = Node_get_next(cur_node);
	}
	average /= List_get_length(points);
	return average;
}

