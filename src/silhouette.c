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
		(*silhouette)[i] = calculate_point_silhouette(clusters, i, Cluster_get_medoid(clusters[i]),
				parameters, dists, garbage_collector, function_caster);
		List points = Cluster_get_points(clusters[i]);	
		Node cur_node = List_get_first_node(points);
		while(cur_node != NULL)
		{
			(*silhouette)[i] += calculate_point_silhouette(clusters, i, Node_get_data(cur_node),
					parameters, dists, garbage_collector, function_caster);
			cur_node = Node_get_next(cur_node);
		}
		(*silhouette)[i] /= List_get_length(points) + 1;
		average_silhouette += (*silhouette)[i];
	}
	return average_silhouette / parameters->number_of_clusters;
}

double calculate_average(Cluster cluster, void *point, double **dists,
	FunctionCaster function_caster)
{
	List points = Cluster_get_points(cluster);
	Node cur_node = List_get_first_node(points);
	int point_idx = function_caster->get_object_index(point);
	int medoid_idx = function_caster->get_object_index(Cluster_get_medoid(cluster));
	double average = dists[point_idx][medoid_idx];
	while(cur_node != NULL)
	{
		void *data = Node_get_data(cur_node);
		int point_idx = function_caster->get_object_index(point);
		int data_idx = function_caster->get_object_index(data);
		average += dists[point_idx][data_idx];
		cur_node = Node_get_next(cur_node);
	}
	average /= List_get_length(points) + 1;
	return average;
}

double calculate_point_silhouette(Cluster *clusters, int i, void *sil_point,
		Params parameters, double **dists,
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster)
{
	double alpha = calculate_average(clusters[i], sil_point,
			dists, function_caster);
	double minimum_distance;
	int idx = find_second_nearest_cluster(clusters,
			parameters->number_of_clusters, clusters[i],
			sil_point, dists, &minimum_distance,
			garbage_collector, function_caster);
	double beta = calculate_average(clusters[idx], sil_point,
			dists, function_caster);
	 return (beta - alpha)/fmax(alpha, beta);
}
